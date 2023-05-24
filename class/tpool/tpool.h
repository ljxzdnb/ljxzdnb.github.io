#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>
#include<semaphore.h>
#pragma once
#include "../main.h"
#ifndef CLASS_TPOOL_H
#define CLASS_TPOOL_H


#if defined(CLASS_RING_H)
#define CLASS_TPOOL_PUB CLASS_DEF(PUB,_tpool_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_TPOOL_PIV CLASS_DEF(PIV,_tpool_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_TPOOL_FUC CLASS_DEF(FUC,_tpool_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_TPOOL_SIG CLASS_DEF(SIG,_tpool_,_ring_,_sock_,_mpool_,_object_)
#define CLASS_TPOOL_SLT CLASS_DEF(SLT,_tpool_,_ring_,_sock_,_mpool_,_object_)
#elif defined(CLASS_SOCK_H)
#define CLASS_TPOOL_PUB CLASS_DEF(PUB,_tpool_,_sock_,_mpool_,_object_)
#define CLASS_TPOOL_PIV CLASS_DEF(PIV,_tpool_,_sock_,_mpool_,_object_)
#define CLASS_TPOOL_FUC CLASS_DEF(FUC,_tpool_,_sock_,_mpool_,_object_)
#define CLASS_TPOOL_SIG CLASS_DEF(SIG,_tpool_,_sock_,_mpool_,_object_)
#define CLASS_TPOOL_SLT CLASS_DEF(SLT,_tpool_,_sock_,_mpool_,_object_)
#else
#define CLASS_TPOOL_PUB CLASS_DEF(PUB,_tpool_,_mpool_,_object_)
#define CLASS_TPOOL_PIV CLASS_DEF(PIV,_tpool_,_mpool_,_object_)
#define CLASS_TPOOL_FUC CLASS_DEF(FUC,_tpool_,_mpool_,_object_)
#define CLASS_TPOOL_SIG CLASS_DEF(SIG,_tpool_,_mpool_,_object_)
#define CLASS_TPOOL_SLT CLASS_DEF(SLT,_tpool_,_mpool_,_object_)
#endif
//----------------------------------------------------------------------------------------------------------------------
static int _tpool = 0;
void _tpool_recvchar_f(void* arg);
void _tpool_recvnum_f(void* arg);

#define _tpool_PUB_DEF(CLASS,_CLASS,...)\
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__) \
    CLASS##PUB(CLASS,struct _tpool_task_t*,taskQ)\
    CLASS##PUB(CLASS,pthread_t,managerID) \
    CLASS##PUB(CLASS,pthread_t*,threadIDs)\
    CLASS##PUB(CLASS,int,synflag) \
    CLASS##PUB(CLASS,sem_t*,symsem) \
    CLASS##PUB(CLASS,int,busyNum) \
    CLASS##PUB(CLASS,int,liveNum) \
    CLASS##PUB(CLASS,int,exitNum) \
    CLASS##PUB(CLASS,int,queueCapacity) \
    CLASS##PUB(CLASS,int,queueFront) \
    CLASS##PUB(CLASS,int,queueRear)\
    CLASS##PUB(CLASS,int,shutdown) \
    CLASS##PUB(CLASS,pthread_mutex_t,mutexpool)\
    CLASS##PUB(CLASS,pthread_mutex_t,mutexbusyNum) \
    CLASS##PUB(CLASS,pthread_cond_t,notFull)\
    CLASS##PUB(CLASS,pthread_cond_t,notEmpty)\
    CLASS##PUB(CLASS,int,minNum) \
    CLASS##PUB(CLASS,int,maxNum)\
    CLASS##PUB(CLASS,int,queueSize)


#define _tpool_PIV_DEF(CLASS,_CLASS,...)\
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)

#define _tpool_FUC_DEF(CLASS,_CLASS,...) \
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__) \
    CLASS##FUC(CLASS,void,tpool,int,max,int,min)\
    CLASS##FUC(CLASS,void,add,void*,fuc,void*,arg)\
    CLASS##VUC(CLASS,int,_tpool,void)

#define _tpool_SIG_DEF(CLASS,_CLASS,...) \
    _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _tpool_SLT_DEF(CLASS,_CLASS,...) \
    _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

#define _tpool_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _tpool_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _tpool_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _tpool_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _tpool_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _tpool_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _tpool_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_s,
#define _tpool_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_r,

typedef enum _tpool_signal_t{
    CLASS_TPOOL_SIG
    _tpool_SIGNAL_NUM
}_tpool_signal;
typedef enum _tpool_slot_t{
    CLASS_TPOOL_SLT
    _tpool_SLOT_NUM
}_tpool_slot;
#undef _tpool_SIG
#undef _tpool_SLT

#define _tpool_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _tpool_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _tpool_t{
    CLASS_TPOOL_PUB
    CLASS_TPOOL_PIV
    CLASS_TPOOL_FUC
    CLASS_TPOOL_SIG
    CLASS_TPOOL_SLT
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}tpool;

typedef struct _tpool_pub_t{
    CLASS_TPOOL_PUB
}_tpool_pub;

#undef _tpool_SIG
#undef _tpool_SLT
#undef _tpool_PUB
#undef _tpool_PTR
#undef _tpool_FUC
#undef _tpool_LUC
#undef _tpool_VUC

#define _tpool_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(tpool* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _tpool_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(tpool* this,ATYPE_ ARG_);
#define _tpool_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(tpool* this);
CLASS_TPOOL_FUC
#undef _tpool_FUC
#undef _tpool_LUC
#undef _tpool_VUC

//##############################################################################################################################

#define _tpool_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)     \
inline TYPE MEMBER##_tpool(ATYPE_ ARG_,ATYPE__ ARG__){              \
    return CLASS##MEMBER##_f(&object_##_tpool,ARG_,ARG__);          \
}                                                                   \
object_##_tpool.MEMBER = MEMBER##_tpool;                            \

#define _tpool_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                   \
inline TYPE MEMBER##_tpool(ATYPE_ ARG_){                            \
    return CLASS##MEMBER##_f(&object_##_tpool,ARG_);                \
}                                                                   \
object_##_tpool.MEMBER = MEMBER##_tpool;                            \

#define _tpool_VUC(CLASS,TYPE,MEMBER,ATYPE_)                        \
inline TYPE MEMBER##_tpool(ATYPE_){                                 \
    return CLASS##MEMBER##_f(&object_##_tpool);                     \
}                                                                   \
object_##_tpool.MEMBER = MEMBER##_tpool;                            \

#define Ltpool(...) __ltpool(_tpool,__VA_ARGS__)
#define __ltpool(_tpool,...)({                                      \
    tpool object_##_tpool = {                                       \
    .this = &object_##_tpool,                                       \
    .ptr = &object_##_tpool.print,                                  \
    .pub = sizeof(_tpool_##pub),                                    \
    __VA_ARGS__                                                     \
    };                                                              \
    CLASS_TPOOL_FUC                                                 \
    _tpool++;                                                       \
    object_##_tpool;                                                \
})
#undef _tpool_SIG
//#undef _tpool_SLT
#undef _tpool_PUB
#undef _tpool_PTR
// #undef _tpool_FUC
// #undef _tpool_LUC
#endif
