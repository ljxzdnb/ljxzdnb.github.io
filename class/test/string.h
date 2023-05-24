#ifndef CLASS_STRING_H
#define CLASS_STRING_H

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpool.h"

#define CLASS_STR_PUB CLASS_DEF(PUB,_str_,_mpool_,_object_)
#define CLASS_STR_PIV CLASS_DEF(PIV,_str_,_mpool_,_object_)
#define CLASS_STR_FUC CLASS_DEF(FUC,_str_,_mpool_,_object_)
#define CLASS_STR_SIG CLASS_DEF(SIG,_str_,_mpool_,_object_)
#define CLASS_STR_SLT CLASS_DEF(SLT,_str_,_mpool_,_object_)

void _str_recvchar_f(void* arg);

void _str_recvnum_f(void* arg);
//----------------------------------------------------------------------------------------------------------------------
static int _str = 0;
#define STRPIECE (1<<2)
#define STRSESSION 32

typedef struct _str_piece_t {
    //_table_type type;
    char *str;
    struct _str_piece_t *next;
} _str_piece;

typedef struct _str_modify_t {
    //_table_type type;
    char* (*modify)(_str_piece target,char str,size_t len);
    _str_piece head;
    struct _str_modify_t *next;
} _str_modify;

#define _str_PUB_DEF(CLASS,_CLASS,...)                      \
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__)                    \
    CLASS##PUB(CLASS,_str_modify*,ankhor[_str_session])

#define _str_PIV_DEF(CLASS,_CLASS,...)                      \
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)                    \
    CLASS##PIV(CLASS,char*,str)                             \
    CLASS##PIV(CLASS,int,secrange)

#define _str_FUC_DEF(CLASS,_CLASS,...) \
    CLASS##FUC_DEF(CLASS,void,string,char*,str)\
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__)

#define _str_SIG_DEF(CLASS,_CLASS,...)         \
    _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _str_SLT_DEF(CLASS,_CLASS,...)         \
    _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

static const size_t c_string_min_size = 32;

#define _str_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _str_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _str_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _str_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _str_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _str_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _str_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) CLASS##MEMBER##_s,
#define _str_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) CLASS##MEMBER##_r,

typedef enum _str_signal_t{
    CLASS_STR_SIG
    _str_SIGNAL_NUM
}_str_signal;
typedef enum _str_slot_t{
    CLASS_STR_SLT
    _str_SLOT_NUM
}_str_slot;
#undef _str_SIG
#undef _str_SLT

#define _str_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) ATYPE__ ARG__;
typedef union _str_data_t{
    CLASS_STR_SIG
}_str_data;
#undef _str_SIG

#define _str_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _str_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _str_class_t{
    CLASS_STR_PUB
    CLASS_STR_PIV
    CLASS_STR_FUC
    CLASS_STR_SIG
    CLASS_STR_SLT
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}string;

typedef struct _str_pub_t{
    CLASS_STR_PUB
}_str_pub;

#undef _str_SIG
#undef _str_SLT
#undef _str_PUB
#undef _str_PTR
#undef _str_FUC
#undef _str_LUC
#undef _str_VUC

#define _str_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(string* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _str_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(string* this,ATYPE_ ARG_);
#define _str_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(string* this);
CLASS_STR_FUC
#undef _str_FUC
#undef _str_LUC
#undef _str_VUC

#endif