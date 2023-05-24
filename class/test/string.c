#include "string.h"


void _str_mpool_f(string *this,size_t size){
    _mpool_mpool_f((mpool*)this,size);
}

void _str__mpool_f(string *this){
    _mpool__mpool_f((mpool*)this);
}

void * _str_malloc_f(string *this, size_t size){
    _mpool_malloc_f((mpool*)this,size);
}

void _str_free_f(string *this, void *p){
    _mpool_free_f((mpool*)this,p);
}

void _str_reset_f(string *this){
    _mpool_reset_f((mpool*)this);
}

void _str_monitor_f(string *this, char *tk){
    _mpool_monitor_f((mpool*)this,tk);
}

struct _str_piece_t * _str_piece_f(string *this, const char *str, size_t length) {
    VALUE_INIT(this->secrange,32);
    int size = length;
    struct _str_piece_t *head = NULL;
    struct _str_piece_t *tmp = NULL;
    head = tmp;
    do {
        static int i = 0;
        tmp = _str_malloc_f(this,sizeof (_str_piece));
        tmp->str = _str_malloc_f(this,this->secrange);
        memcpy(tmp->str,str+i*this->secrange,this->secrange);
        size -= this->secrange;
        if(i%piece_range == 0) this->ankhor[piece_range].head = tmp;
        tmp = tmp->next;
        i++;
    }while(size > this->secrange);
    return head;
}

void _str_string_f(string* this,char* str,int length) {
    VALUE_INIT(this->str,str);
    VALUE_INIT(this->secrange,32);
    int size = length;
    int piece_range = (length - 1 / (STRPIECE))+1;
    _str_mpool_f(this,4096);
    struct _str_piece_t *tmp = this->ankhor[0].head;
    do {
        static int i = 0;
        tmp = _str_malloc_f(this,sizeof (_str_piece));
        tmp->str = _str_malloc_f(this,this->secrange);
        memcpy(tmp->str,str+i*this->secrange,this->secrange);
        size -= this->secrange;
        if(i%piece_range == 0) this->ankhor[piece_range].head = tmp;
        tmp = tmp->next;
        i++;
    }while(size > this->secrange);
}

void _str__string_f(string *this) {
    if (this == NULL) return;
    free(this->str);
}

void _str_add_f(string *this, const char *str, size_t len) {
    if (cs == NULL || str == NULL || *str == '\0') return;

    if (len == 0) len = strlen(str);

    c_string_ensure_space(cs, len);
    memmove(cs->str + cs->len, str, len);
    cs->len += len;
    cs->str[cs->len] = '\0';
}

void c_string_append_char(string *cs, char c) {
    if (cs == NULL) return;
    c_string_ensure_space(cs, 1);
    cs->str[cs->len] = c;
    cs->len++;
    cs->str[cs->len] = '\0';
}

void c_string_append_int(string *cs, int val) {
    char str[12];

    if (cs == NULL) return;

    snprintf(str, sizeof(str), "%d", val);
    c_string_append_str(cs, str, 0);
}

void c_string_front_str(string *cs, const char *str, size_t len) {
    if (cs == NULL || str == NULL || *str == '\0') return;

    if (len == 0) len = strlen(str);

    c_string_ensure_space(cs, len);
    memmove(cs->str + len, cs->str, cs->len);
    memmove(cs->str, str, len);
    cs->len += len;
    cs->str[cs->len] = '\0';
}

void c_string_front_char(string *cs, char c) {
    if (cs == NULL) return;
    c_string_ensure_space(cs, 1);
    memmove(cs->str + 1, cs->str, cs->len);
    cs->str[0] = c;
    cs->len++;
    cs->str[cs->len] = '\0';
}

void c_string_front_int(string *cs, int val) {
    char str[12];

    if (cs == NULL) return;

    snprintf(str, sizeof(str), "%d", val);
    c_string_front_str(cs, str, 0);
}

void c_string_clear(string *cs) {
    if (cs == NULL) return;
    stringruncate(cs, 0);
}

void stringruncate(string *cs, size_t len) {
    if (cs == NULL || len >= cs->len) return;

    cs->len = len;
    cs->str[cs->len] = '\0';
}

void c_string_drop_begin(string *cs, size_t len) {
    if (cs == NULL || len == 0) return;

    if (len >= cs->len) {
        c_string_clear(cs);
        return;
    }

    cs->len -= len;
    /* +1 to move the NULL. */
    memmove(cs->str, cs->str + len, cs->len + 1);
}

void c_string_drop_end(string *cs, size_t len) {
    if (cs == NULL || len == 0) return;

    if (len >= cs->len) {
        c_string_clear(cs);
        return;
    }
    cs->len -= len;
    cs->str[cs->len] = '\0';
}

size_t c_string_len(const string *cs) {
    if (cs == NULL) return 0;
    return cs->len;
}

const char *c_string_peek(const string *cs) {
    if (cs == NULL) return NULL;
    return cs->str;
}

char *c_string_dump(const string *cs, size_t *len) {
    char *out;

    if (cs == NULL) return NULL;

    if (len != NULL) *len = cs->len;
    out = malloc(cs->len + 1);
    memcpy(out, cs->str, cs->len + 1);
    return out;
}

int main() {
    struct c_string *cs = c_string_create();
    c_string_append_str(cs, "123", 0);
    c_string_append_char(cs, 'a');
    c_string_append_int(cs, 5);
    printf("%s \n", c_string_peek(cs));
    c_string_front_str(cs, "789", 0);
    printf("%s \n", c_string_peek(cs));
    c_string_drop_begin(cs, 2);
    printf("%s \n", c_string_peek(cs));
    c_string_drop_end(cs, 2);
    printf("%s \n", c_string_peek(cs));
    c_string_destroy(cs);
    return 0;
}
