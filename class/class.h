#ifndef _LIST_CLASS_H
#define _LIST_CLASS_H


#include <stdarg.h>
#include <stdbool.h>
#include "object.h"

void _class_recvchar_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucppppp:%s\n", arg);
}

void _class_recvnum_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%d\n", arg);
}

//----------------------------------------------------------------------------------------------------------------------
static int _class = 0;

#define CLASS_PUB_DEF(_CLASS)\
    _CLASS

#define CLASS_PIV_DEF(_CLASS)\
    _CLASS

#define CLASS_FUC_DEF(_CLASS) \
    _CLASS

#define CLASS_SIG_DEF(_CLASS) \
    _CLASS

#define CLASS_SLT_DEF(_CLASS) \
    _CLASS

#define _class_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _class_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _class_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _class_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _class_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _class_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _class_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) CLASS##MEMBER##_s,
#define _class_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) CLASS##MEMBER##_r,

typedef enum _class_signal_t{
    CLASS_SIG_DEF(OBJECT_SIG_DEF(_class_))
    _class_SIGNAL_NUM
}_class_signal;
typedef enum _class_slot_t{
    CLASS_SIG_DEF(OBJECT_SLT_DEF(_class_))
    _class_SLOT_NUM
}_class_slot;
#undef _class_SIG
#undef _class_SLT

#define _class_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) ATYPE__ ARG__;
typedef union _class_data_t{
    CLASS_SIG_DEF(OBJECT_SIG_DEF(_class_))
}_class_data;
#undef _class_SIG

#define _class_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _class_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _class_class_t{
    CLASS_PUB_DEF(OBJECT_PUB_DEF(_class_))
    CLASS_PIV_DEF(OBJECT_PIV_DEF(_class_))
    CLASS_FUC_DEF(OBJECT_FUC_DEF(_class_))
    CLASS_SIG_DEF(OBJECT_SIG_DEF(_class_))
    CLASS_SLT_DEF(OBJECT_SLT_DEF(_class_))
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}class;

typedef struct _class_pub_t{
    CLASS_PUB_DEF(OBJECT_PUB_DEF(_class_))
}_class_pub;

#undef _class_SIG
#undef _class_SLT
#undef _class_PUB
#undef _class_PTR
#undef _class_FUC
#undef _class_LUC
#undef _class_VUC

const static void _class_print_f(class *this){}

const static void _class_set_f(class *this,char *mem,void* arg) {
#define _class_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _class_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;   
CLASS_PUB_DEF(OBJECT_PUB_DEF(_class_))
}
#undef _class_PTR
#undef _class_PUB

#define _class_PTR _class_PUB
const static void* _class_get_f(class *this,char *mem) {
#define _class_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return *(MEMTYPE*)&this->MEMBER;    
CLASS_PUB_DEF(OBJECT_PUB_DEF(_class_))
}
#undef _class_PUB

const static void* _class_pos_f(class *this,char *mem) {
#define _class_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;    
CLASS_PUB_DEF(OBJECT_PUB_DEF(_class_))
}
#undef _class_PUB

const static void _class_inject_f(class *this,_class_signal index,void* slot){
    this->signal[index] = slot;
}

const static void _class_emit_f(class *this,_class_signal index,_class_data* arg){
#define _class_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) if(CLASS##MEMBER##_s == index) { typeof(this->MEMBER) callback = (typeof(this->MEMBER))this->signal[index];callback(arg->ARG__);};
//#define SLT(TYPE,MEMBER,...) TYPE (*MEMBER)(__VA_ARGS__);
CLASS_SIG_DEF(OBJECT_SIG_DEF(_class_))
}
#undef _class_SIG
#undef _class_SLT

/*const char* tojson(class *this,char *json) {
#define _class_PUB(MEMTYPE,MEMBER) json = (char *)malloc(sizeof (char)*(strlen(#MEMBER)+strlen(&this->MEMBER)+1)) ;printf();
OBJECT_PUB_DEF
}
#undef _class_PUB*/

#define _class_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)  \
inline TYPE MEMBER##_class(ATYPE_ ARG_,ATYPE__ ARG__){     \
    return CLASS##MEMBER##_f(&object_##_class,ARG_,ARG__);     \
}                                                   \
object_##_class.MEMBER = MEMBER##_class;            \

#define _class_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                   \
inline TYPE MEMBER##_class(ATYPE_ ARG_){                            \
    return CLASS##MEMBER##_f(&object_##_class,ARG_);                \
}                                                                   \
object_##_class.MEMBER = MEMBER##_class;                            \

#define _class_VUC(CLASS,TYPE,MEMBER,ATYPE_)                        \
inline TYPE MEMBER##_class(ATYPE_){                                 \
    return CLASS##MEMBER##_f(&object_##_class);                     \
}                                                                   \
object_##_class.MEMBER = MEMBER##_class;                            \

#define Lobject(...) __lobject(_class,__VA_ARGS__)
#define _class_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) .slot[_class_##MEMBER##_r] = CLASS##MEMBER##_f,
#define __lobject(_class,...)({                                     \
    struct _class_class_t object_##_class = {                       \
    CLASS_SLT_DEF(OBJECT_SLT_DEF(_class_))                          \
    .this = &object_##_class,                                       \
    .ptr = &object_##_class.print,                                  \
    .pub = sizeof(_class_##pub),                                    \
    __VA_ARGS__                                                     \
    };                                                              \
    CLASS_FUC_DEF(OBJECT_FUC_DEF(_class_))                          \
    _class++;                                                       \
    object_##_class;                                                \
})
#undef OTHER_SIG_DEF
#undef OTHER_MEM_DEF
#undef _class_SIG
//#undef _class_SLT
#undef _class_PUB
#undef _class_PTR
// #undef _class_FUC
// #undef _class_LUC
#endif
