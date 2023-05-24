#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_JSON_LEN 1024

typedef enum {
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} json_type;

typedef struct {
    json_type type;
    union {
        int boolean_val;
        double number_val;
        char *string_val;
        struct json_node_t *array_val;
        struct json_node_t *object_val;
    };
} json_value;

typedef struct json_node_t {
    char *name;
    json_value value;
    struct json_node_t *next;
} json_node;

char *json_str;
int json_pos = 0;

json_node *parse_json();
json_node *parse_object();
json_node *parse_array();
json_node *parse_value();
json_node *parse_string();
json_node *parse_number();
json_node *parse_boolean();

void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        error("Usage: json_parser <json_string>");
    }
    json_str = argv[1];
    json_node *root = parse_json();
    printf("Root node:\n");
    if (root->name != NULL) {
        printf("  Name: %s\n", root->name);
    }
    switch (root->value.type) {
        case JSON_NULL:
            printf("  Value: null\n");
            break;
        case JSON_BOOLEAN:
            printf("  Value: %s\n", root->value.boolean_val ? "true" : "false");
            break;
        case JSON_NUMBER:
            printf("  Value: %f\n", root->value.number_val);
            break;
        case JSON_STRING:
            printf("  Value: %s\n", root->value.string_val);
            break;
        case JSON_ARRAY:
            printf("  Value: [array]\n");
            break;
        case JSON_OBJECT:
            printf("  Value: {object}\n");
            break;
    }
    return 0;
}

json_node *parse_json() {
    json_node *node = malloc(sizeof(json_node));
    node->name = NULL;
    if (json_str[json_pos] == '{') {
        node->value.type = JSON_OBJECT;
        node->value.object_val = parse_object();
    } else if (json_str[json_pos] == '[') {
        node->value.type = JSON_ARRAY;
        node->value.array_val = parse_array();
    } else {
        error("Invalid JSON string: must start with object or array");
    }
    return node;
}

json_node *parse_object() {
    json_node *head = NULL, *tail = NULL;
    json_pos++; // skip '{'
    while (json_str[json_pos] != '}') {
        json_node *node = malloc(sizeof(json_node));
        node->name = parse_string()->value.string_val;
        if (json_str[json_pos] != ':') {
            error("Invalid JSON string: expected ':' after name");
        }
        json_pos++; // skip ':'
        node->value = parse_value()->value;
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        if (json_str[json_pos] == ',') {
            json_pos++; // skip ','
        } else if (json_str[json_pos] != '}') {
            error("Invalid JSON string: expected ',' or '}' after value");
        }
    }
    json_pos++; // skip '}'
    return head;
}

json_node *parse_array() {
    json_node *head = NULL, *tail = NULL;
    json_pos++; // skip '['
    while (json_str[json_pos] != ']') {
        json_node *node = malloc(sizeof(json_node));
        node->name = NULL;
        node->value = parse_value()->value;
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        if (json_str[json_pos] == ',') {
            json_pos++; // skip ','
        } else if (json_str[json_pos] != ']') {
            error("Invalid JSON string: expected ',' or ']' after value");
        }
    }
    json_pos++; // skip ']'
    return head;
}

json_node *parse_value() {
    json_node *node = malloc(sizeof(json_node));
    node->name = NULL;
    switch (json_str[json_pos]) {
        case '{':
            node->value.type = JSON_OBJECT;
            node->value.object_val = parse_object();
            break;
        case '[':
            node->value.type = JSON_ARRAY;
            node->value.array_val = parse_array();
            break;
        case '"':
            node->value.type = JSON_STRING;
            node->value.string_val = parse_string()->value.string_val;
            break;
        case 't':
        case 'f':
            node->value.type = JSON_BOOLEAN;
            node->value.boolean_val = parse_boolean()->value.boolean_val;
            break;
        default:
            node->value.type = JSON_NUMBER;
            node->value.number_val = parse_number()->value.number_val;
            break;
    }
    return node;
}

json_node *parse_string() {
    json_node *node = malloc(sizeof(json_node));
    node->name = NULL;
    int start_pos = ++json_pos; // skip leading quote
    while (json_str[json_pos] != '"') {
        if (json_str[json_pos] == '\\') {
            json_pos++; // skip escape character
        }
        json_pos++;
    }
    int len = json_pos - start_pos;
    char *str = malloc(len + 1);
    strncpy(str, json_str + start_pos, len);
    str[len] = '\0';
    node->value.type = JSON_STRING;
    node->value.string_val = str;
    json_pos++; // skip trailing quote
    return node;
}

json_node *parse_number() {
    json_node *node = malloc(sizeof(json_node));
    node->name = NULL;
    char *endptr;
    double val = strtod(json_str + json_pos, &endptr);
    if (endptr == json_str + json_pos) {
        error("Invalid JSON string: expected number");
    }
    node->value.type = JSON_NUMBER;
    node->value.number_val = val;
    json_pos = endptr - json_str;
    return node;
}

json_node *parse_boolean() {
    json_node *node = malloc(sizeof(json_node));
    node->name = NULL;
    if (strncmp(json_str + json_pos, "true", 4) == 0) {
        node->value.type = JSON_BOOLEAN;
        node->value.boolean_val = 1;
        json_pos += 4;
    } else if (strncmp(json_str + json_pos, "false", 5) == 0) {
        node->value.type = JSON_BOOLEAN;
        node->value.boolean_val = 0;
        json_pos += 5;
    } else {
        error("Invalid JSON string: expected 'true' or 'false'");
    }
    return node;
}

