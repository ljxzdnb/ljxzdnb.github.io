#ifndef CLASS_TABLE_H
#define CLASS_TABLE_H


#include <stdarg.h>
#include <stdbool.h>
#include "../mpool/mpool.h"


void _table_recvchar_f(void* arg);

void _table_recvnum_f(void* arg);
//----------------------------------------------------------------------------------------------------------------------
static int _table = 0;

typedef enum {
    _table_NULL,
    _table_BOOL,
    _table_NUMBER,
    _table_STRING,
    _table_ARRAY,
    _table_OBJECT
}_table_type;

typedef union {
    double* number;
    char* string;
    int* boolean;
    struct table_node_t *array;
    struct table_node_t *object;
} _table_value;

typedef struct table_node_t {
    _table_value value;
    //_table_type type;
    char *key;
    struct table_node_t *next;
} _table_node;

#define _table_PUB_DEF(CLASS,_CLASS,...)                \
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__)                \
    CLASS##PUB(CLASS,char*,ljx)                         \
    CLASS##PUB(CLASS,double*,age)                       \
    CLASS##PUB(CLASS,int,aus)                           \
    CLASS##PUB(CLASS,_table_node*,arr)                  

#define _table_PIV_DEF(CLASS,_CLASS,...)                \
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)                \
    CLASS##PIV(CLASS,char*,str)                         \
    CLASS##PIV(CLASS,int,index)                         \
    CLASS##PIV(CLASS,_table_node*,root)                 

#define _table_FUC_DEF(CLASS,_CLASS,...)                \
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__)                \
    CLASS##LUC(CLASS,void,dump,char*,str)               


#define _table_SIG_DEF(CLASS,_CLASS,...)                \
     _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _table_SLT_DEF(CLASS,_CLASS,...)                \
     _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

#define _table_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _table_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _table_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _table_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _table_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _table_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _table_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_s,
#define _table_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_r,

typedef enum _table_signal_t{
    CLASS_DEF(SIG,_table_,_mpool_,_object_)
    _table_SIGNAL_NUM
}_table_signal;
typedef enum _table_slot_t{
    CLASS_DEF(SLT,_table_,_mpool_,_object_)
    _table_SLOT_NUM
}_table_slot;
#undef _table_SIG
#undef _table_SLT

#define _table_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _table_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _table_class_t{
    CLASS_DEF(PUB,_table_,_mpool_,_object_)
    CLASS_DEF(PIV,_table_,_mpool_,_object_)
    CLASS_DEF(FUC,_table_,_mpool_,_object_)
    CLASS_DEF(SIG,_table_,_mpool_,_object_)
    CLASS_DEF(SLT,_table_,_mpool_,_object_)
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}table;

typedef struct _table_pub_t{
    CLASS_DEF(PUB,_table_,_mpool_,_object_)
}_table_pub;

#undef _table_SIG
#undef _table_SLT
#undef _table_PUB
#undef _table_PTR
#undef _table_FUC
#undef _table_LUC
#undef _table_VUC

#define _table_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(table* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _table_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(table* this,ATYPE_ ARG_);
#define _table_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(table* this);
    CLASS_DEF(FUC,_table_,_mpool_,_object_)
#undef _table_FUC
#undef _table_LUC
#undef _table_VUC

//----------------------------------------------------------------------------------------------------------------------


#define _table_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)         \
inline TYPE MEMBER##_table(ATYPE_ ARG_,ATYPE__ ARG__){                  \
    return CLASS##MEMBER##_f(&object_##_table,ARG_,ARG__);              \
}                                                                       \
object_##_table.MEMBER = MEMBER##_table;                                \

#define _table_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                       \
inline TYPE MEMBER##_table(ATYPE_ ARG_){                                \
    return CLASS##MEMBER##_f(&object_##_table,ARG_);                    \
}                                                                       \
object_##_table.MEMBER = MEMBER##_table;                                \

#define _table_VUC(CLASS,TYPE,MEMBER,ATYPE_)                            \
inline TYPE MEMBER##_table(ATYPE_){                                     \
    return CLASS##MEMBER##_f(&object_##_table);                         \
}                                                                       \
object_##_table.MEMBER = MEMBER##_table;                                \

#define Ltable(START,...) __ltable(_table,START,__VA_ARGS__)
#define __ltable(_table,START,...)({                                    \
    table object_##_table = {                                           \
    .this = &object_##_table,                                           \
    .ptr = &object_##_table.print,                                      \
    .pub = sizeof(_table_##pub),                                        \
    __VA_ARGS__                                                         \
    };                                                                  \
    CLASS_DEF(FUC,_table_,_mpool_,_object_)                             \
    _table++;                                                           \
    object_##_table;                                                    \
})
#undef _table_SIG
//#undef _table_SLT
#undef _table_PUB
#undef _table_PTR
// #undef _table_FUC
// #undef _table_LUC
#endif
