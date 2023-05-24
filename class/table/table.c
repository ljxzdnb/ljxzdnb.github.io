#include "table.h"



//----------------------------------------------------------------------------------------------------------------------

void _table_recvchar_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucppppp:%s\n", (char*)arg);
}

void _table_recvnum_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%d\n", *(int*)arg);
}
//----------------------------------------------------------------------------------------------------------------------

void _table_print_f(table *this){}

void _table_set_f(table *this,char *mem,void* arg) {
#define _table_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _table_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;   
CLASS_DEF(PUB,_table_,_mpool_,_object_)
}
#undef _table_PTR
#undef _table_PUB

#define _table_PTR _table_PUB
void* _table_get_f(table *this,char *mem) {
#define _table_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;    
CLASS_DEF(PUB,_table_,_mpool_,_object_)
}
#undef _table_PUB

void* _table_pos_f(table *this,char *mem) {
#define _table_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;    
CLASS_DEF(PUB,_table_,_mpool_,_object_)
}
#undef _table_PUB
#undef _table_SLT

void _table_mpool_f(table *this,size_t size){
    _mpool_mpool_f((mpool*)this,size);
}

void _table__mpool_f(table *this){
    _mpool__mpool_f((mpool*)this);
}

void * _table_malloc_f(table *this, size_t size){
    _mpool_malloc_f((mpool*)this,size);
}

void _table_free_f(table *this, void *p){
    return _mpool_free_f((mpool*)this,p);
}

void _table_reset_f(table *this){
    return _mpool_reset_f((mpool*)this);
}

void _table_monitor_f(table *this, char *tk){
    return _mpool_monitor_f((mpool*)this,tk);
}

static _table_node *parse_table(table* this);
static _table_node *parse_object(table* this);
static _table_node *parse_array(table* this);
static _table_node *parse_value(table* this);
static _table_node *parse_string(table* this);
static _table_node *parse_number(table* this);
static _table_node *parse_boolean(table* this);


void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

static _table_node *parse_table(table* this) {
    this->mpool(4096);
    _table_node *node = this->malloc(sizeof(_table_node));
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

static _table_node *parse_object(table* this) {
    _table_node *head = NULL, *tail = NULL;
    this->index++; // skip '{'
    while (this->str[this->index] != '}') {
        _table_node *node = this->malloc(sizeof(_table_node));
        node->key = parse_string(this)->value.string;
        if (this->str[this->index] != ':') {
            error("Invalid JSON string: expected ':' after name");
        }
        this->index++; // skip ':'
        node->value = parse_value(this)->value;
        if (this->get(node->key) != NULL) {
            *(_table_value*)this->get(node->key) = node->value;  
        }
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

static _table_node *parse_array(table* this) {
    this->index++; // skip '['
    _table_node *node = NULL;
    _table_node *tmp = NULL;
    int index = 0;
    while (this->str[this->index] != ']') {
        tmp = node;
        node = this->malloc((index+1)*sizeof(_table_node));
        memcpy(node,tmp,index*sizeof(_table_node));
        this->free(tmp);
        node[index].key = NULL;
        node[index].value = parse_value(this)->value;
        if (this->str[this->index] == ',') {
            this->index++; // skip ','
            index++;
        } else if (this->str[this->index] != ']') {
            error("Invalid JSON string: expected ',' or ']' after value");
        }
    
    }
    // node = this->malloc((index+1)*sizeof(_table_node));
    // memcpy(node,tmp,(index+1)*sizeof(_table_node));
    // free(tmp);
    this->index++; // skip ']'
    return node;
}

static _table_node *parse_value(table* this) {
    _table_node *node = this->malloc(sizeof(_table_node));
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

static _table_node *parse_string(table* this) {
    _table_node *node = this->malloc(sizeof(_table_node));
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

static _table_node *parse_number(table* this) {
    _table_node *node = this->malloc(sizeof(_table_node));
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

static _table_node *parse_boolean(table* this) {
    _table_node *node = this->malloc(sizeof(_table_node));
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

void _table_dump_f(table* this,char* str){
    this->str = str;
    this->index = 0;
    this->root = parse_table(this);
    this->monitor("table space");
    // printf("%f\n",this->root->value.object->value.array->value.number);
    // printf("%p\n",this->root->value.object);
    // printf("%p\n",this->root->value);
    //printf("%s\n",(char*)(*(_table_node**)this->root)->key);
    // printf("%f\n",*(double*)(*(_table_node**)this->root)->value.array->value.number);
    //printf("%f\n",*(double*)(*(_table_node**)this->root)->value.array->value.number);
    // // printf("%s\n",this->root->value.object->value.string);
    //printf("%s\n",this->root->value.object->next->key);
    //printf("%f\n",this->root->value.object->value.array[1].value.number);
    printf("%s\n",this->ljx);
    // //printf("%d\n",*this->aus);
    // printf("%f\n",*this->age);
    printf("ljxxxxxxxxxxxxxxxxxxxxxxxs%f\n",**(double**)&this->arr[1]);
    // printf("%f\n",**(double**)&this->arr[1].value);
    //printf("%f\n",**(double**)&this->arr[0]);
}


