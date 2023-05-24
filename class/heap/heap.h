#ifndef CLASS_HEAP_H
#define CLASS_HEAP_H


#include <stdarg.h>
#include <stdbool.h>
#include "../object.h"

void _heap_recvchar_f(void* arg);
void _heap_recvnum_f(void* arg);

//----------------------------------------------------------------------------------------------------------------------
static int _heap = 0;

#define _heap_PUB_DEF(CLASS,_CLASS,...)\
    CLASS##PUB(_heap_,int,num) \
    CLASS##PUB(_heap_,int,size) \
    CLASS##PUB(_heap_,int,off) \
    CLASS##PUB(_heap_,int,capacity) \
    CLASS##PUB(_heap_,void*,data) \
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__)

#define _heap_PIV_DEF(CLASS,_CLASS,...)\
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)

#define _heap_FUC_DEF(CLASS,_CLASS,...) \
    CLASS##FUC(_heap_,void,create,int,num,int,size)\
    CLASS##LUC(_heap_,void,callback,void*,fuc)\
    CLASS##LUC(_heap_,void,insert,void*,arg)\
    CLASS##VUC(_heap_,void,delete,void)\
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__)

#define _heap_SIG_DEF(CLASS,_CLASS,...) \
    _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _heap_SLT_DEF(CLASS,_CLASS,...) \
    _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

#define _heap_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _heap_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _heap_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _heap_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _heap_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _heap_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _heap_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_s,
#define _heap_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_r,

typedef enum _heap_signal_t{
    CLASS_DEF(SIG,_heap_,_object_)
    _heap_SIGNAL_NUM
}_heap_signal;
typedef enum _heap_slot_t{
    CLASS_DEF(SLT,_heap_,_object_)
    _heap_SLOT_NUM
}_heap_slot;
#undef _heap_SIG
#undef _heap_SLT

#define _heap_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _heap_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _heap_class_t{
    CLASS_DEF(PUB,_heap_,_object_)
    CLASS_DEF(PIV,_heap_,_object_)
    CLASS_DEF(FUC,_heap_,_object_)
    CLASS_DEF(SIG,_heap_,_object_)
    CLASS_DEF(SLT,_heap_,_object_)
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}heap;

typedef struct _heap_pub_t{
    CLASS_DEF(PUB,_heap_,_object_)
}_heap_pub;

#undef _heap_SIG
#undef _heap_SLT
#undef _heap_PUB
#undef _heap_PTR
#undef _heap_FUC
#undef _heap_LUC
#undef _heap_VUC

#define _heap_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(heap* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _heap_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(heap* this,ATYPE_ ARG_);
#define _heap_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(heap* this);
CLASS_DEF(FUC,_heap_,_object_)
#undef _heap_FUC
#undef _heap_LUC
#undef _heap_VUC




//############################################################################################################

#define _heap_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)     \
inline TYPE MEMBER##_heap(ATYPE_ ARG_,ATYPE__ ARG__){              \
    return CLASS##MEMBER##_f(&object_##_heap,ARG_,ARG__);          \
}                                                                   \
object_##_heap.MEMBER = MEMBER##_heap;                            \

#define _heap_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                    \
inline TYPE MEMBER##_heap(ATYPE_ ARG_){                             \
return CLASS##MEMBER##_f(&object_##_heap,ARG_);                     \
}                                                                   \
object_##_heap.MEMBER = MEMBER##_heap;                              \

#define _heap_VUC(CLASS,TYPE,MEMBER,ATYPE_)                         \
inline TYPE MEMBER##_heap(ATYPE_){                                  \
return CLASS##MEMBER##_f(&object_##_heap);                          \
}                                                                   \
object_##_heap.MEMBER = MEMBER##_heap;                              \

#define Lheap(...) __lheap(_heap,__VA_ARGS__)
#define __lheap(_heap,...)({                                      \
    heap object_##_heap = {                                       \
    .this = &object_##_heap,                                       \
    .ptr = &object_##_heap.print,                                  \
    .pub = sizeof(_heap_##pub),                                    \
    __VA_ARGS__                                                     \
    };                                                              \
    CLASS_DEF(FUC,_heap_,_object_)                                 \
    _heap++;                                                       \
    object_##_heap;                                                \
})
#undef _heap_SIG
//#undef _heap_SLT
#undef _heap_PUB
#undef _heap_PTR
// #undef _heap_FUC
// #undef _heap_LUC
#endif
