#ifndef CLASS_JSON_H
#define CLASS_JSON_H


#include <stdarg.h>
#include <stdbool.h>
#include "../mpool/mpool.h"


void _json_recvchar_f(void* arg);

void _json_recvnum_f(void* arg);
//----------------------------------------------------------------------------------------------------------------------
static int _json = 0;

typedef enum {
    _json_NULL,
    _json_BOOL,
    _json_NUMBER,
    _json_STRING,
    _json_ARRAY,
    _json_OBJECT
}_json_type;

typedef union {
    double* number;
    char* string;
    int* boolean;
    struct json_node_t *array;
    struct json_node_t *object;
} _json_value;

typedef struct json_node_t {
    _json_value value;
    //_json_type type;
    char *key;
    struct json_node_t *next;
} _json_node;

#ifdef __REFLECT__
//#define _json_nodes_cfg(CLASS,SUC) \
//    CLASS##ARR_(CLASS,SUC,int*,depth)\
//    CLASS##ARR_(CLASS,SUC,char*,sn)\
//    CLASS##ARR_(CLASS,SUC,char*,channelId)\
//    CLASS##ARR_(CLASS,SUC,char*,connect_port)\
//    CLASS##ARR_(CLASS,SUC,char*,product_key)\
//    CLASS##ARR_(CLASS,SUC,char*,tcp_ip_addr)\
//    CLASS##ARR_(CLASS,SUC,char*,tcp_port)\
//    CLASS##ARR_(CLASS,SUC,char*,template_id)\
//    CLASS##ARR_(CLASS,SUC,char*,term_addr)\

#endif

#define _json_PUB_DEF(CLASS,_CLASS,...)                 \
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__)                \

#ifdef __REFLECT__
//    CLASS##PUB(CLASS,char*,version)                     \
//    CLASS##ARR(CLASS,nodes_cfg,nodes_cfg)               \

#endif

#define _json_PIV_DEF(CLASS,_CLASS,...)                 \
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)                \
    CLASS##PIV(CLASS,char*,str)                         \
    CLASS##PIV(CLASS,int,index)                         \
    CLASS##PIV(CLASS,int,arrindex)                      \
    CLASS##PIV(CLASS,_json_node*,root)

#define _json_FUC_DEF(CLASS,_CLASS,...)                 \
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__)                \
    CLASS##LUC(CLASS,void,dump,char*,str)               \
    CLASS##MUC(CLASS,_json_node*,value,char*,format)

#define _json_SIG_DEF(CLASS,_CLASS,...)                \
     _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _json_SLT_DEF(CLASS,_CLASS,...)                \
     _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

#define _json_PTR(CLASS,TYPE,MEMBER)
#define _json_PUB(CLASS,TYPE,MEMBER)
#define _json_ARR(CLASS,TYPE,MEMBER) struct TYPE{CLASS##MEMBER(CLASS,MEMBER)} MEMBER;
#define _json_ARR_(CLASS,SUC,TYPE,MEMBER) TYPE MEMBER;
CLASS_DEF(PUB,_json_,_mpool_,_object_)
#undef _json_ARR
#undef _json_PUB
#undef _json_PTR
#undef _json_ARR_

#define _json_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _json_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _json_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _json_STC(CLASS,TYPE,MEMBER) struct TYPE* MEMBER;
#define _json_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _json_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _json_MUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_, ...);
#define _json_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _json_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_s,
#define _json_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_r,


typedef enum _json_signal_t{
    CLASS_DEF(SIG,_json_,_mpool_,_object_)
    _json_SIGNAL_NUM
}_json_signal;
typedef enum _json_slot_t{
    CLASS_DEF(SLT,_json_,_mpool_,_object_)
    _json_SLOT_NUM
}_json_slot;
#undef _json_SIG
#undef _json_SLT

#define _json_ARR(CLASS,TYPE,MEMBER) struct TYPE* MEMBER;

#define _json_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _json_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);

typedef struct _json_class_t{
    CLASS_DEF(PUB,_json_,_mpool_,_object_)
    CLASS_DEF(PIV,_json_,_mpool_,_object_)
    CLASS_DEF(FUC,_json_,_mpool_,_object_)
    CLASS_DEF(SIG,_json_,_mpool_,_object_)
    CLASS_DEF(SLT,_json_,_mpool_,_object_)
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}json;

typedef struct _json_pub_t{
    CLASS_DEF(PUB,_json_,_mpool_,_object_)
}_json_pub;
#undef _json_ARR
#undef _json_SIG
#undef _json_SLT
#undef _json_PUB
#undef _json_PTR
#undef _json_FUC
#undef _json_LUC
#undef _json_MUC
#undef _json_VUC
#undef _json_ARR_nodes_cfg

#define _json_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(json* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _json_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(json* this,ATYPE_ ARG_);
#define _json_MUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(json* this,ATYPE_ ARG_, va_list* ap);
#define _json_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(json* this);
    CLASS_DEF(FUC,_json_,_mpool_,_object_)
#undef _json_FUC
#undef _json_LUC
#undef _json_VUC
#undef _json_MUC


//----------------------------------------------------------------------------------------------------------------------


#define _json_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)         \
inline TYPE MEMBER##_json(ATYPE_ ARG_,ATYPE__ ARG__){                  \
    return CLASS##MEMBER##_f(&object_##_json,ARG_,ARG__);              \
}                                                                       \
object_##_json.MEMBER = MEMBER##_json;                                \

#define _json_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                       \
inline TYPE MEMBER##_json(ATYPE_ ARG_){                                \
    return CLASS##MEMBER##_f(&object_##_json,ARG_);                    \
}                                                                       \
object_##_json.MEMBER = MEMBER##_json;                                \

#define _json_MUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                       \
inline TYPE MEMBER##_json(ATYPE_ ARG_,...){                                \
    va_list arg;                                                       \
    va_start(arg,ARG_);                                                 \
    _json_node* tmp = CLASS##MEMBER##_f(&object_##_json,ARG_,&arg);      \
    va_end(arg);                                                                   \
    return tmp;                                                         \
}                                                                       \
object_##_json.MEMBER = MEMBER##_json;                                \

#define _json_VUC(CLASS,TYPE,MEMBER,ATYPE_)                            \
inline TYPE MEMBER##_json(ATYPE_){                                     \
    return CLASS##MEMBER##_f(&object_##_json);                         \
}                                                                       \
object_##_json.MEMBER = MEMBER##_json;                                \

#define Ljson(START,...) __ljson(_json,START,__VA_ARGS__)
#define __ljson(_json,START,...)({                                    \
    json object_##_json = {                                           \
    .this = &object_##_json,                                           \
    .ptr = &object_##_json.print,                                      \
    .pub = sizeof(_json_##pub),                                        \
    __VA_ARGS__                                                         \
    };                                                                  \
    CLASS_DEF(FUC,_json_,_mpool_,_object_)                             \
    _json++;                                                           \
    object_##_json;                                                    \
})
#undef _json_SIG
//#undef _json_SLT
#undef _json_PUB
#undef _json_PTR
// #undef _json_FUC
// #undef _json_LUC
#endif
