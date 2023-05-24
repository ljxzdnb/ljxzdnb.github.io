#include "json.h"

//----------------------------------------------------------------------------------------------------------------------

void _json_print_f(json *this){}

#define _json_ARR _json_PTR
void _json_set_f(json *this,char *mem,void* arg) {
#define _json_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _json_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;
CLASS_DEF(PUB,_json_,_mpool_,_object_)
}
#undef _json_ARR
#undef _json_PTR
#undef _json_PUB


#define _json_PTR _json_PUB
void* _json_get_f(json *this,char *mem) {
#define _json_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
#define _json_ARR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = _json_malloc_f(this,3*sizeof (struct MEMBER));CLASS##MEMBER(CLASS,MEMBER)
#define _json_ARR_(CLASS,SUC,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->SUC[this->arrindex].MEMBER;
CLASS_DEF(PUB,_json_,_mpool_,_object_)
return NULL;
}
#undef _json_ARR
#undef _json_PUB
#undef _json_ARR_

#define _json_ARR _json_PUB
void* _json_pos_f(json *this,char *mem) {
#define _json_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
CLASS_DEF(PUB,_json_,_mpool_,_object_)
}
#undef _json_PUB
#undef _json_ARR
#undef _json_SLT

void _json_mpool_f(json *this,size_t size){
    _mpool_mpool_f((mpool*)this,size);
}

void _json__mpool_f(json *this){
    _mpool__mpool_f((mpool*)this);
}

void * _json_malloc_f(json *this, size_t size){
    _mpool_malloc_f((mpool*)this,size);
}

void _json_free_f(json *this, void *p){
    return _mpool_free_f((mpool*)this,p);
}

void _json_reset_f(json *this){
    return _mpool_reset_f((mpool*)this);
}

void _json_monitor_f(json *this, char *tk){
    return _mpool_monitor_f((mpool*)this,tk);
}

static _json_node *parse_json(json* this);
static _json_node *parse_object(json* this);
static _json_node *parse_array(json* this);
static _json_node *parse_value(json* this);
static _json_node *parse_string(json* this);
static _json_node *parse_number(json* this);
static _json_node *parse_boolean(json* this);


static void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

static _json_node *parse_json(json* this) {
    _json_mpool_f(this,4096);
    _json_node *node = _json_malloc_f(this,sizeof(_json_node));
    node->key = NULL;
    if (this->str[this->index] == '{') {
        node->value.object = parse_object(this);
    } else if (this->str[this->index] == '[') {
        node->value.array = parse_array(this);
    } else {
        error("Invalid JSON string: must start with object or array");
    }
    return node;
}

static _json_node *parse_object(json* this) {
    _json_node *head = NULL, *tail = NULL;
    this->index++; // skip '{'
    while (this->str[this->index] != '}') {
        _json_node *node = _json_malloc_f(this,sizeof(_json_node));
        node->key = parse_string(this)->value.string;
        if (this->str[this->index] != ':') {
            error("Invalid JSON string: expected ':' after name");
        }
        this->index++; // skip ':'
//        void* cur = this->get(node->key);
        node->value = parse_value(this)->value;
//        if(cur != NULL) {
//            *(_json_value*)this->get(node->key) = node->value;
//        }
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        if (this->str[this->index] == ',') {
            this->index++; // skip ','
        } else if (this->str[this->index] != '}') {
            error("Invalid JSON string: expected ',' or '}' after value");
        }
    }
    this->index++; // skip '}'
    return head;
}

static _json_node *parse_array(json* this) {
    this->index++; // skip '['
    _json_node *node = NULL;
    _json_node *tmp = NULL;
    while (this->str[this->index] != ']') {
        tmp = node;
        node = this->malloc((this->arrindex+1)*sizeof(_json_node));
        memcpy(node,tmp,this->arrindex*sizeof(_json_node));
        this->free(tmp);
        node[this->arrindex].key = NULL;
        node[this->arrindex].value = parse_value(this)->value;
        if (this->str[this->index] == ',') {
            this->index++; // skip ','
            this->arrindex++;
        } else if (this->str[this->index] != ']') {
            error("Invalid JSON string: expected ',' or ']' after value");
        }
    
    }
    this->arrindex = 0;
    this->index++; // skip ']'
    return node;
}

static _json_node *parse_value(json* this) {
    _json_node *node = this->malloc(sizeof(_json_node));
    node->key = NULL;
    switch (this->str[this->index]) {
        case '{':
            node->value.object = parse_object(this);
            break;
        case '[':
            node->value.array = parse_array(this);
            break;
        case '"':
            node->value.string = parse_string(this)->value.string;
            break;
        case 't':
        case 'f':
            node->value.boolean = parse_boolean(this)->value.boolean;
            break;
        default:
            node->value.number = parse_number(this)->value.number;
            break;
    }
    return node;
}

static _json_node *parse_string(json* this) {
    _json_node *node = this->malloc(sizeof(_json_node));
    node->key = NULL;
    int start_index = ++this->index; // skip leading quote
    while (this->str[this->index] != '"') {
        if (this->str[this->index] == '\\') {
            this->index++; // skip escape character
        }
        this->index++;
    }
    int len = this->index - start_index;
    char *str = this->malloc(len + 1);
    strncpy(str, this->str + start_index, len);
    str[len] = '\0';
    node->value.string = str;
    this->index++; // skip trailing quote
    return node;
}

static _json_node *parse_number(json* this) {
    _json_node *node = this->malloc(sizeof(_json_node));
    node->key = NULL;
    char *endptr;
    double *val = this->malloc(sizeof(double));
    *val = strtod(this->str + this->index, &endptr);
    if (endptr == this->str + this->index) {
        error("Invalid JSON string: expected number");
    }
    node->value.number = val;
    this->index = endptr - this->str;
    return node;
}

static _json_node *parse_boolean(json* this) {
    _json_node *node = this->malloc(sizeof(_json_node));
    node->key = NULL;
    if (strncmp(this->str + this->index, "true", 4) == 0) {
        *node->value.boolean = 1;
        this->index += 4;
    } else if (strncmp(this->str + this->index, "false", 5) == 0) {
        node->value.boolean = 0;
        this->index += 5;
    } else {
        error("Invalid JSON string: expected 'true' or 'false'");
    }
    return node;

}

_json_node* _json_value_f(json* this,char* format,va_list* ap){
    int level=0;
    char* p,*arg=NULL;
    _json_node *tmp = this->root->value.object;
//    arg = va_arg(*ap,char*);
//    printf("sstring:%s\n",arg);
    for(p = format;*p;){
        switch (*p++) {
            case 'o':
                arg = va_arg(*ap,char*);
                while(strcmp(tmp->key,arg)){
                    tmp = tmp->next;
                }
                if(strcmp(tmp->key,arg)){
                    perror("arg error!");
                }
                break;
            case 'r':
                if(*p!='r'){
                    arg = va_arg(*ap,char*);
                    while(strcmp(tmp->key,arg)){
                        tmp = tmp->next;
                    }
                    if(strcmp(tmp->key,arg)){
                        perror("arg error!");
                    }
                    tmp = tmp->value.array[level].value.object;
                    level=0;
                }
                level++;
                break;
            default:
                break;
//                perror("error input");
//                break;
        }
    }
    _json_free_f(this,ap);
    return tmp;
}

void _json_dump_f(json* this,char* str){
    this->str = str;
    this->index = 0;
    this->root = parse_json(this);
    this->monitor("json space");
//    printf("%s\n",this->root->value.object->next->value.string);
//    printf("%s\n",this->root->value.object->value.array[1].value.object->next->next->next->next->value.string);
//     printf("%s\n",this->version);
//    printf("%s\n",this->nodes_cfg[1].sn);
    // printf("%f\n",*this->age);
    //printf("ljxxxxxxxxxxxxxxxxxxxxxxxs%f\n",**(double**)&this->arr[1]);
    // printf("%f\n",**(double**)&this->arr[1].value);
    //printf("%f\n",**(double**)&this->arr[0]);
}


char* load_file(const char* filename) {
    FILE* file = fopen(filename, "rb"); // 以二进制方式打开文件
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END); // 定位到文件末尾
    long size = ftell(file); // 获取文件大小
    fseek(file, 0, SEEK_SET); // 定位到文件开头

    char* buffer = (char*) malloc(size + 1); // 分配缓冲区
    fread(buffer, size, 1, file); // 读取文件内容到缓冲区
    buffer[size] = '\0'; // 添加字符串结束符

    fclose(file); // 关闭文件

    return buffer;
}

int main(int argc, char *argv[]) {
    char* str = load_file("../extern/config.json");
    json conf = Ljson();
    conf.dump(str);
//    printf("version:%s\n",*(char**)_json_value_f((json*)conf.this,"s","version"));
    printf("sn:%s\n",*(char**)conf.value("r->o","nodes_cfg","sn"));
//    conf.value("s","version");
    printf("version:%s\n",*(char**)conf.value("o","version"));
}