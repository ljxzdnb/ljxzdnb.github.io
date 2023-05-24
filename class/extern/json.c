#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_JSON_DEPTH 32

typedef enum {
    JSON_TYPE_UNKNOWN,
    JSON_TYPE_OBJECT,
    JSON_TYPE_ARRAY,
    JSON_TYPE_STRING,
    JSON_TYPE_NUMBER,
    JSON_TYPE_BOOLEAN,
    JSON_TYPE_NULL
} json_type_t;

typedef struct json_value_s json_value_t;

struct json_value_s {
    json_type_t type;
    union {
        char *string_val;
        double number_val;
        int boolean_val;
        json_value_t **array_items;
        struct {
            char **keys;
            json_value_t **values;
            int count;
        } object_val;
    } data;
};

json_value_t *parse_json(const char *json_str);
void free_json(json_value_t *json);
json_value_t *get_json_key(const char *key, const json_value_t *json);

static const char *parse_string(const char *str, char **out_str);
static const char *parse_number(const char *str, double *out_num);
static const char *parse_boolean(const char *str, int *out_bool);
static const char *skip_whitespace(const char* str);

// 解析字符串类型的值
static const char* parse_string(const char* str, char** out_str) {
    if (*str != '\"') return NULL; // 不是合法的字符串类型
    ++str; // 跳过开头引号
    size_t len = 0;

    while (1) {
        if (*(str + len) == '\\') { // 处理转义字符
            switch(*(str + len + 1)) {
                case '\"':
                case '\\':
                case '/':
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    ++len;
                    break;
            }
        } else if (*(str + len) == '\"') { // 字符串结束
            break;
        }

        ++len; // 统计字符数
    }

    char *string_val = (char *)malloc(len + 1);
    memcpy(string_val, str, len);
    *(string_val + len) = '\0';

    *out_str = string_val;

    return str + len + 1;
}

// 解析数字类型的值
static const char* parse_number(const char* str, double* out_num) {
    char *end_ptr;
    double num = strtod(str, &end_ptr);

    if (str == end_ptr) { // 没有成功解析出数字
        return NULL;
    }

    *out_num = num;

    return end_ptr;
}

// 解析布尔类型的值
static const char* parse_boolean(const char* str, int* out_bool) {
    if (*str == 't') {
        *out_bool = 1;
        return str + 4; // true 的长度是4个字符，即 "true"
    } else if (*str == 'f') {
        *out_bool = 0;
        return str + 5; // false 的长度是5个字符，即 "false"
    } else {
        return NULL; // 不是合法的布尔类型
    }
}

// 跳过空白字符（包括空格、制表符和换行符）
static const char* skip_whitespace(const char* str) {
     while (*str && (*str == ' ' || *str == '\t' || *str == '\n')) {
         ++str;
     }

     return str;
}

// 解析 JSON 值
static const char* parse_value(const char* str, json_value_t** out_val) {
    str = skip_whitespace(str);

    switch (*str) {
        case '{': { // 对象类型
            json_value_t* obj_val = (json_value_t *)malloc(sizeof(json_value_t));
            obj_val->type = JSON_TYPE_OBJECT;

            ++str; // 跳过开头的花括号

            // 解析键值对
            int count = 0;
            while (*str && count < MAX_JSON_DEPTH) {
                if (*str == '}') { // 遇到对象结束符
                    ++str;
                    obj_val->data.object_val.count = count;
                    obj_val->data.object_val.keys =
                            (char **)malloc(count * sizeof(char *));
                    obj_val->data.object_val.values =
                            (json_value_t **)malloc(count * sizeof(json_value_t *));
                    break;
                } else if (*str == '\"') { // 解析键
                    char *key_str;
                    str = parse_string(++str, &key_str);
                    *(obj_val->data.object_val.keys + count) = key_str;

                    str = skip_whitespace(str);

                    if (*str != ':') { // 没有找到冒号，不是合法的键值对
                        free_json(obj_val);
                        return NULL;
                    }

                    ++str; // 跳过冒号

                    str = skip_whitespace(str);

                    json_value_t* value;
                     str = parse_value(str, &value); // 解析值
                     *(obj_val->data.object_val.values + count++) = value;

                     str = skip_whitespace(str);

                     if (*str == ',') { // 处理多个键值对之间的逗号
                         ++str;
                         str = skip_whitespace(str);
                     }
                } else {
                    free_json(obj_val);
                    return NULL; // 不是合法的对象类型
                }
            }

            if (count >= MAX_JSON_DEPTH) { // 对象深度过大，可能出现栈溢出等问题
                free_json(obj_val);
                return NULL;
            }

            *out_val = obj_val;

            break;
        }
        case '[': { // 数组类型
            json_value_t* array_val = (json_value_t *)malloc(sizeof(json_value_t));
            array_val->type = JSON_TYPE_ARRAY;

            ++str; // 跳过开头的方括号

            int count = 0;

            while (*str && count < MAX_JSON_DEPTH) {
                 json_value_t* value;
                 str = parse_value(str, &value); // 解析值

                 *(array_val->data.array_items + count++) = value;

                 str = skip_whitespace(str);

                 if (*str == ',') { // 处理多个数组元素之间的逗号
                     ++str;
                     str = skip_whitespace(str);
                 } else if (*str == ']') { // 遇到数组结束符
                     ++str;
                     array_val->data.array_items =
                             (json_value_t **)realloc(array_val->data.array_items,
                                                      sizeof(json_value_t*) * count);
                     break;
                 } else {
                     free_json(array_val); // 不是合法的数组类型
                     return NULL;
                 }
             }

             if (count >= MAX_JSON_DEPTH) { // 数组深度过大，可能出现栈溢出等问题
                 free_json(array_val);
                 return NULL;
             }

            *out_val = array_val;

            break;
        }
        case '\"': { // 字符串类型
            char *string_val;
            str = parse_string(str, &string_val);

            json_value_t* string_obj =
                    (json_value_t *)malloc(sizeof(json_value_t));
            string_obj->type = JSON_TYPE_STRING;
            string_obj->data.string_val = string_val;

            *out_val = string_obj;

            break;
        }
        case 't':
        case 'f': { // 布尔类型
            int bool_val;
            str = parse_boolean(str, &bool_val);

             json_value_t* bool_obj =
                     (json_value_t *)malloc(sizeof(json_value_t));
             bool_obj->type = JSON_TYPE_BOOLEAN;
             bool_obj->data.boolean_val = bool_val;

             *out_val = bool_obj;

             break;
         }
         case 'n': { // null 类型
             ++str; // 跳过开头的 n

             if (*str == 'u' && *(str + 1) == 'l' && *(str + 2) == 'l') {
                 str += 3; // 跳过 ull

                 json_value_t* null_obj =
                         (json_value_t *)malloc(sizeof(json_value_t));
                 null_obj->type = JSON_TYPE_NULL;

                 *out_val = null_obj;
             } else { // 不是合法的 null 类型
                 return NULL;
             }

             break;
         }
         default: { // 数字类型
              double num_val;
              str = parse_number(str, &num_val);

              json_value_t* num_obj =
                      (json_value_t *)malloc(sizeof(json_value_t));
              num_obj->type = JSON_TYPE_NUMBER;
              num_obj->data.number_val = num_val;

              *out_val = num_obj;
              break;
         }
    }

    return str;
}

// 解析 JSON 字符串
json_value_t* parse_json(const char* json_str) {
    if (!json_str || !strlen(json_str)) return NULL; // 空字符串，不是合法的 JSON

    json_value_t* root;
    const char* end_ptr = parse_value(skip_whitespace(json_str), &root);

    if (end_ptr == NULL) { // 解析失败
        free_json(root);
        return NULL;
    }

    return root;
}

// 释放 JSON 对象内存
void free_json(json_value_t *json) {
     if (!json) return;

     switch (json->type) {
         case JSON_TYPE_OBJECT:
             for (int i = 0; i < json->data.object_val.count; ++i) {
                 free_json(*(json->data.object_val.values + i));
                 free(*(json->data.object_val.keys + i));
             }

             free(json->data.object_val.keys);
             free(json->data.object_val.values);

             break;

         case JSON_TYPE_ARRAY:
             for (int i = 0; i < json->data.object_val.count; ++i) {
                 free_json(*(json->data.array_items + i));
             }

             free(json->data.array_items);

             break;

         case JSON_TYPE_STRING:
            if (json->data.string_val != NULL)
                free(json->data.string_val);
            break;

         default:
            break;
     }

     free(json);
}

// 获取指定键的值
json_value_t *get_json_key(const char *key, const json_value_t *json) {
     if (key == NULL || strlen(key) == 0 || json == NULL ||
             json->type != JSON_TYPE_OBJECT) {
         return NULL;
     }

     for (int i = 0; i < json->data.object_val.count; ++i) {
         if (!strcmp(key, *(json->data.object_val.keys + i))) {
             return *(json->data.object_val.values + i);
         }
     }

     return NULL;
}

