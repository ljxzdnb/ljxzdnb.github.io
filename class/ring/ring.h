// ring_queue.h

#pragma once
#include <sys/mman.h>
#include "../main.h"
#ifndef CLASS_RING_H
#define CLASS_RING_H

#if defined(CLASS_SOCK_H)
#define CLASS_RSOCK_FUC(CLASS) CLASS##VUC(CLASS,void,recvbuffer,void) \
                        CLASS##VUC(CLASS,void,sendbuffer,void)

#define CLASS_RING_PUB CLASS_DEF(PUB,_ring_,_sock_,_mpool_,_object_)
#define CLASS_RING_PIV CLASS_DEF(PIV,_ring_,_sock_,_mpool_,_object_)
#define CLASS_RING_FUC CLASS_DEF(FUC,_ring_,_sock_,_mpool_,_object_)
#define CLASS_RING_SIG CLASS_DEF(SIG,_ring_,_sock_,_mpool_,_object_)
#define CLASS_RING_SLT CLASS_DEF(SLT,_ring_,_sock_,_mpool_,_object_)
#else
#define CLASS_RSOCK_FUC
#define CLASS_RING_PUB CLASS_DEF(PUB,_ring_,_mpool_,_object_)
#define CLASS_RING_PIV CLASS_DEF(PIV,_ring_,_mpool_,_object_)
#define CLASS_RING_FUC CLASS_DEF(FUC,_ring_,_mpool_,_object_)
#define CLASS_RING_SIG CLASS_DEF(SIG,_ring_,_mpool_,_object_)
#define CLASS_RING_SLT CLASS_DEF(SLT,_ring_,_mpool_,_object_)
#endif
void _ring_recvchar_f(void* arg);

void _ring_recvnum_f(void* arg);

static int _ring = 0;

#ifdef __ZIP__
#define CLASS_ZIP_PUB        _ring_PUB(_ring_,void*,zbuf)                \
                             _ring_PUB(_ring_,size_t,srclen)             \
                             _ring_PUB(_ring_,size_t,destlen)

#define CLASS_ZIP_FUC        _ring_VUC(_ring_,void,uncompress,void)      \
                             _ring_VUC(_ring_,void,compress,void)
#else
#define CLASS_ZIP_PUB
#define CLASS_ZIP_FUC
#endif

#define _ring_PUB_DEF(CLASS,_CLASS,...)         \
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__)        \
    CLASS_ZIP_PUB                               \
    CLASS##PUB(CLASS,char*,pbuf)                \
    CLASS##PUB(CLASS,int,synfd)                 \
    CLASS##PUB(CLASS,int,item_size)             \
    CLASS##PUB(CLASS,int,capacity)              \
    CLASS##PUB(CLASS,int,write_flag)   \
    CLASS##PUB(CLASS,int,read_flag)    \
    CLASS##PUB(CLASS,int,ringhead)     \
    CLASS##PUB(CLASS,int,ringtail)     \
    CLASS##PUB(CLASS,int,same_cycle)

#define _ring_PIV_DEF(CLASS,_CLASS,...)        \
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)

#define _ring_FUC_DEF(CLASS,_CLASS,...)                             \
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__)                            \
    CLASS_ZIP_FUC                                                   \
    CLASS_RSOCK_FUC(CLASS)					                        \
    CLASS##FUC(CLASS,void,ringbuffer,int,item_size, int,capacity)   \
    CLASS##VUC(CLASS,void,_ringbuffer,void)   			            \
    CLASS##LUC(CLASS,void,synfile,char*,filename)                   \
    CLASS##LUC(CLASS,void,push,void*,item_size)                     \
    CLASS##LUC(CLASS,void,pop,void*,item_size)                      \



#define _ring_SIG_DEF(CLASS,_CLASS,...) \
    _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _ring_SLT_DEF(CLASS,_CLASS,...) \
    _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

#define _ring_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _ring_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _ring_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _ring_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _ring_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _ring_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _ring_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_s,
#define _ring_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_r,

typedef enum _ring_signal_t{
    CLASS_RING_SIG
    _ring_SIGNAL_NUM
}_ring_signal;
typedef enum _ring_slot_t{
    CLASS_RING_SLT
    _ring_SLOT_NUM
}_ring_slot;
#undef _ring_SIG
#undef _ring_SLT

#define _ring_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _ring_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _ring_class_t{
    CLASS_RING_PUB
    CLASS_RING_PIV
    CLASS_RING_FUC
    CLASS_RING_SIG
    CLASS_RING_SLT
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}ring;

typedef struct _ring_pub_t{
    CLASS_RING_PUB
}_ring_pub;

#undef _ring_SIG
#undef _ring_SLT
#undef _ring_PUB
#undef _ring_PTR
#undef _ring_FUC
#undef _ring_LUC
#undef _ring_VUC

#define _ring_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(ring* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _ring_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(ring* this,ATYPE_ ARG_);
#define _ring_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(ring* this);
CLASS_RING_FUC
#undef _ring_FUC
#undef _ring_LUC
#undef _ring_VUC

#define _ring_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)     \
inline TYPE MEMBER##_ring(ATYPE_ ARG_,ATYPE__ ARG__){              \
    return CLASS##MEMBER##_f(&object_##_ring,ARG_,ARG__);          \
}                                                                   \
object_##_ring.MEMBER = MEMBER##_ring;                            \

#define _ring_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                   \
inline TYPE MEMBER##_ring(ATYPE_ ARG_){                            \
    return CLASS##MEMBER##_f(&object_##_ring,ARG_);                \
}                                                                   \
object_##_ring.MEMBER = MEMBER##_ring;                            \

#define _ring_VUC(CLASS,TYPE,MEMBER,ATYPE_)                        \
TYPE MEMBER##_ring(ATYPE_){                                 \
    CLASS##MEMBER##_f(&object_##_ring);                     \
}                                                                   \
object_##_ring.MEMBER = MEMBER##_ring;                            \

#define Lring(...) __lring(_ring,__VA_ARGS__)
#define __lring(_ring,...)({                                        \
    ring object_##_ring = {                                         \
    .this = &object_##_ring,                                        \
    .ptr = &object_##_ring.print,                                   \
    .pub = sizeof(_ring_##pub),                                     \
    .ID = _ring,                                                    \
    __VA_ARGS__                                                     \
    };                                                              \
    CLASS_RING_FUC                                                  \
    _ring++;                                                        \
    object_##_ring;                                                 \
})
#undef _ring_SIG
//#undef _ring_SLT
#undef _ring_PUB
#undef _ring_PTR
// #undef _ring_FUC
// #undef _ring_LUC
#endif
