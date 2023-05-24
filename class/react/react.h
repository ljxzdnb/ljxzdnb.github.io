//
// Created by ljx on 5/6/23.
//

#pragma once
#include "../main.h"
#ifndef CLASS_REACT_H
#define CLASS_REACT_H


#if defined(CLASS_TPOOL_H) && defined(CLASS_RING_H)
#define CLASS_REACT_PUB CLASS_DEF(PUB,_react_,_tpool_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_REACT_PIV CLASS_DEF(PIV,_react_,_tpool_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_REACT_FUC CLASS_DEF(FUC,_react_,_tpool_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_REACT_SIG CLASS_DEF(SIG,_react_,_tpool_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_REACT_SLT CLASS_DEF(SLT,_react_,_tpool_,_ring_,_sock_,_mpool_,_object_)
#elif defined(CLASS_RING_H)
#define CLASS_REACT_PUB CLASS_DEF(PUB,_react_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_REACT_PIV CLASS_DEF(PIV,_react_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_REACT_FUC CLASS_DEF(FUC,_react_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_REACT_SIG CLASS_DEF(SIG,_react_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_REACT_SLT CLASS_DEF(SLT,_react_,_ring_,_sock_,_mpool_,_object_)
#elif defined(CLASS_TPOOL_H)
#define CLASS_REACT_PUB CLASS_DEF(PUB,_react_,_tpool_,_sock_,_mpool_,_object_)
#define CLASS_REACT_PIV CLASS_DEF(PIV,_react_,_tpool_,_sock_,_mpool_,_object_)
#define CLASS_REACT_FUC CLASS_DEF(FUC,_react_,_tpool_,_sock_,_mpool_,_object_)
#define CLASS_REACT_SIG CLASS_DEF(SIG,_react_,_tpool_,_sock_,_mpool_,_object_)
#define CLASS_REACT_SLT CLASS_DEF(SLT,_react_,_tpool_,_sock_,_mpool_,_object_)
#else
#define CLASS_REACT_PUB CLASS_DEF(PUB,_react_,_sock_,_mpool_,_object_)
#define CLASS_REACT_PIV CLASS_DEF(PIV,_react_,_sock_,_mpool_,_object_)
#define CLASS_REACT_FUC CLASS_DEF(FUC,_react_,_sock_,_mpool_,_object_)
#define CLASS_REACT_SIG CLASS_DEF(SIG,_react_,_sock_,_mpool_,_object_)
#define CLASS_REACT_SLT CLASS_DEF(SLT,_react_,_sock_,_mpool_,_object_)
#endif

#define BUFFER_LENGTH           4096


void _react_recvchar_f(void* arg);
void _react_recvnum_f(void* arg);

struct header//保持相应头信息
{
    int status_code;//HTTP/1.1 '200' OK
    char content_type[128];//Content-Type: application/gzip
    long content_length;//Content-Length: 11683079
    char file_name[256];
};


typedef int (*NCALLBACK)(void *);
struct ntyevent {
    int fd;
    int events;
    void *arg;

    NCALLBACK callback;

    int status;
    char buffer[BUFFER_LENGTH];
    int length;

    //http param
    int method; //
    char resource[BUFFER_LENGTH];
    int ret_code;
    char Content_Type[512];
};
struct eventblock {
    struct eventblock *next;
    struct ntyevent *events;
};

static int _react = 0;

#define _react_PUB_DEF(CLASS,_CLASS,...)    \
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__)    \
    CLASS##PUB(_react_,int,blkcnt)          \
    CLASS##PUB(_react_,struct eventblock *,evblk)

#define _react_PIV_DEF(CLASS,_CLASS,...)\
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)

#define _react_FUC_DEF(CLASS,_CLASS,...) \
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__) \
    CLASS##FUC(_react_,void,run,char*,ip,uint16_t,port)

#define _react_SIG_DEF(CLASS,_CLASS,...) \
    _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _react_SLT_DEF(CLASS,_CLASS,...) \
    _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

#define _react_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _react_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _react_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _react_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _react_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _react_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _react_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_s,
#define _react_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_r,

typedef enum _react_signal_t{
    CLASS_REACT_SIG
    _react_SIGNAL_NUM
}_react_signal;
typedef enum _react_slot_t{
    CLASS_REACT_SLT
    _react_SLOT_NUM
}_react_slot;
#undef _react_SIG
#undef _react_SLT


#define _react_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _react_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _react_class_t{
    CLASS_REACT_PUB
    CLASS_REACT_PIV
    CLASS_REACT_FUC
    CLASS_REACT_SIG
    CLASS_REACT_SLT
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}react;

typedef struct _react_pub_t{
    CLASS_REACT_PUB
}_react_pub;

#undef _react_SIG
#undef _react_SLT
#undef _react_PUB
#undef _react_PTR
#undef _react_FUC
#undef _react_LUC
#undef _react_VUC

#define _react_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(react* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _react_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(react* this,ATYPE_ ARG_);
#define _react_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(react* this);
CLASS_REACT_FUC
#undef _react_FUC
#undef _react_LUC
#undef _react_VUC


//############################################################################################################

#define _react_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)     \
inline TYPE MEMBER##_react(ATYPE_ ARG_,ATYPE__ ARG__){              \
    return CLASS##MEMBER##_f(&object_##_react,ARG_,ARG__);          \
}                                                                   \
object_##_react.MEMBER = MEMBER##_react;                            \

#define _react_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                   \
inline TYPE MEMBER##_react(ATYPE_ ARG_){                            \
return CLASS##MEMBER##_f(&object_##_react,ARG_);                    \
}                                                                   \
object_##_react.MEMBER = MEMBER##_react;                            \

#define _react_VUC(CLASS,TYPE,MEMBER,ATYPE_)                        \
inline TYPE MEMBER##_react(ATYPE_){                                 \
return CLASS##MEMBER##_f(&object_##_react);                         \
}                                                                   \
object_##_react.MEMBER = MEMBER##_react;                            \

#define Lreact(...) __lreact(_react,__VA_ARGS__)
#define __lreact(_react,...)({                                      \
    react object_##_react = {                                       \
    .this = &object_##_react,                                       \
    .ptr = &object_##_react.print,                                  \
    .pub = sizeof(_react_##pub),                                    \
    __VA_ARGS__                                                     \
    };                                                              \
    CLASS_REACT_FUC                                                 \
    _react++;                                                       \
    object_##_react;                                                \
})
#undef OTHER_SIG_DEF
#undef OTHER_MEM_DEF
#undef _react_SIG
//#undef _react_SLT
#undef _react_PUB
#undef _react_PTR
// #undef _react_FUC
// #undef _react_LUC
#endif //CLASS_REACT_H
