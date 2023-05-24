#ifndef CLASS_MPOOL_H
#define CLASS_MPOOL_H


#include <stdarg.h>
#include <stdbool.h>
#include "../object.h"

void _mpool_recvchar_f(void* arg);
void _mpool_recvnum_f(void* arg);

//----------------------------------------------------------------------------------------------------------------------
static int _mpool = 0;

struct mp_node_s {
    unsigned char *end;
    unsigned char *last;
    struct mp_node_s *next;
    int quote;
    int failed;
};

struct mp_large_s {
    struct mp_large_s *next;
    int size;
    void *alloc;
};

#define _mpool_PUB_DEF(CLASS,_CLASS,...)        \
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__)        \
    CLASS##PUB(CLASS,int,pagesize)              \
    CLASS##PUB(CLASS,struct mp_large_s *,large) \
    CLASS##PUB(CLASS,struct mp_node_s *,head)   \
    CLASS##PUB(CLASS,struct mp_node_s *,current)\
    CLASS##PUB(CLASS,void*,data)

#define _mpool_PIV_DEF(CLASS,_CLASS,...)        \
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)        \

#define _mpool_FUC_DEF(CLASS,_CLASS,...)        \
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__)        \
    CLASS##LUC(CLASS,void,mpool,size_t,size)   \
    CLASS##LUC(CLASS,void*,malloc,size_t,size)  \
    CLASS##LUC(CLASS,void,monitor,char*,msg)    \
    CLASS##LUC(CLASS,void,free,void*,ptr)       \
    CLASS##VUC(CLASS,void,_mpool,void)         \
    CLASS##VUC(CLASS,void,reset,void)           \

#define _mpool_SIG_DEF(CLASS,_CLASS,...)        \
    _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _mpool_SLT_DEF(CLASS,_CLASS,...)        \
    _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

#define _mpool_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _mpool_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _mpool_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _mpool_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _mpool_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _mpool_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _mpool_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_s,
#define _mpool_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_r,

typedef enum _mpool_signal_t{
    CLASS_DEF(SIG,_mpool_,_object_)
    _mpool_SIGNAL_NUM
}_mpool_signal;
typedef enum _mpool_slot_t{
    CLASS_DEF(SLT,_mpool_,_object_)
    _mpool_SLOT_NUM
}_mpool_slot;
#undef _mpool_SIG
#undef _mpool_SLT


#define _mpool_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _mpool_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _mpool_class_t{
    CLASS_DEF(PUB,_mpool_,_object_)
    CLASS_DEF(PIV,_mpool_,_object_)
    CLASS_DEF(FUC,_mpool_,_object_)
    CLASS_DEF(SIG,_mpool_,_object_)
    CLASS_DEF(SLT,_mpool_,_object_)
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}mpool;

typedef struct _mpool_pub_t{
    CLASS_DEF(PUB,_mpool_,_object_)
}_mpool_pub;

#undef _mpool_SIG
#undef _mpool_SLT
#undef _mpool_PUB
#undef _mpool_PTR
#undef _mpool_FUC
#undef _mpool_LUC
#undef _mpool_VUC

#define _mpool_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(mpool* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _mpool_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(mpool* this,ATYPE_ ARG_);
#define _mpool_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(mpool* this);
CLASS_DEF(FUC,_mpool_,_object_)
#undef _mpool_FUC
#undef _mpool_LUC
#undef _mpool_VUC

/*const char* tojson(mpool *this,char *json) {
#define _mpool_PUB(MEMTYPE,MEMBER) json = (char *)malloc(sizeof (char)*(strlen(#MEMBER)+strlen(&this->MEMBER)+1)) ;printf();
_object_PUB_DEF
}
#undef _mpool_PUB*/

#define _mpool_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)     \
inline TYPE MEMBER##_mpool(ATYPE_ ARG_,ATYPE__ ARG__){              \
    return CLASS##MEMBER##_f(&object_##_mpool,ARG_,ARG__);          \
}                                                                   \
object_##_mpool.MEMBER = MEMBER##_mpool;                            \

#define _mpool_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                   \
inline TYPE MEMBER##_mpool(ATYPE_ ARG_){                            \
    return CLASS##MEMBER##_f(&object_##_mpool,ARG_);                \
}                                                                   \
object_##_mpool.MEMBER = MEMBER##_mpool;                            \

#define _mpool_VUC(CLASS,TYPE,MEMBER,ATYPE_)                        \
inline TYPE MEMBER##_mpool(ATYPE_){                                 \
    return CLASS##MEMBER##_f(&object_##_mpool);                     \
}                                                                   \
object_##_mpool.MEMBER = MEMBER##_mpool;                            \

#define Lmpool(...) __lmpool(_mpool,__VA_ARGS__)
#define __lmpool(_mpool,...)({                                      \
    mpool object_##_mpool = {                                       \
    .this = &object_##_mpool,                                       \
    .ptr = &object_##_mpool.print,                                  \
    .pub = sizeof(_mpool_##pub),                                    \
    __VA_ARGS__                                                     \
    };                                                              \
    CLASS_DEF(FUC,_mpool_,_object_)                                 \
    _mpool++;                                                       \
    object_##_mpool;                                                \
})
#undef OTHER_SIG_DEF
#undef OTHER_MEM_DEF
#undef _mpool_SIG
//#undef _mpool_SLT
#undef _mpool_PUB
#undef _mpool_PTR
// #undef _mpool_FUC
// #undef _mpool_LUC
#endif

