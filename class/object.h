#ifndef CLASS_OBJECT_H
#define CLASS_OBJECT_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdint.h>
#ifdef _GRAPHVIZ_PLOT
#include <graphviz/gvc.h>
#endif
#include "nuklear.h"

#define likely(x) __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

//----------------------------------------------------------------------------------------------------------------------
#define _object_PUB_DEF(CLASS)                                      \
    CLASS##PTR(CLASS,char*,name)                                    \
    CLASS##PUB(CLASS,struct CLASS##_class_t*,parent)                \
    CLASS##PUB(CLASS,struct CLASS##_class_t*,children)              \

/*    X(int(*member),(const char *))\ */

#define _object_PIV_DEF(CLASS)                                      \
    CLASS##PIV(CLASS,void*,this)                                    \
    CLASS##PIV(CLASS,int,ID)                                        \
    CLASS##PIV(CLASS,int,pub)                                       \
    CLASS##PIV(CLASS,void*,ptr)                                     \
    CLASS##PIV(CLASS,struct nk_rect, bounds)                        \
    CLASS##PIV(CLASS,struct nk_color, color)                        \
/*    X(int(*member),(const char *))\ */

#define _object_FUC_DEF(CLASS) \
    CLASS##VUC(CLASS,void,print,void)                               \
    CLASS##FUC(CLASS,void,set,char*,mem,void*,arg)                  \
    CLASS##LUC(CLASS,void*,pos,char*,mem)                           \
    CLASS##LUC(CLASS,void*,get,char*,mem)                           \

#define _object_SIG_DEF(CLASS) \
    CLASS##SIG(CLASS,void,sendchar,char*, ptr) \
    CLASS##SIG(CLASS,void,sendnum,int, num) \

#define _object_SLT_DEF(CLASS) \
    CLASS##SLT(CLASS,void,recvchar,char*, ptr) \
    CLASS##SLT(CLASS,void,recvnum,int, num) \

#define CONNECT(SINGAL,SLOT) ({typeof(SINGAL) __tmp = SINGAL;typeof(SLOT) _tmp = SLOT;(void)(&_tmp == &__tmp);SINGAL=SLOT;})

#define lambda(FORM,EXP,ARG,...) ({typeof(ARG) ret = (typeof(ARG))0;typeof(ARG) __ARRAY[] = {ARG,##__VA_ARGS__}; \
                    __ARGINIT__ FORM\
                    if (strstr(#FORM,",")){\
                    for (int i = sizeof((typeof(ARG)[]){ARG,__VA_ARGS__})/sizeof(typeof(ARG)) - 1; i > 0 ; --i){ \
                        *px = __ARRAY[i-1];*py = __ARRAY[i];\
                        __ARRAY[i-1] = EXP;}}\
                    else{\
                    for (int i = 0; i < sizeof((typeof(ARG)[]){ARG,__VA_ARGS__})/sizeof(typeof(ARG)) ; ++i){ \
                        *px = __ARRAY[i];\
                        __ARRAY[i] = EXP;\
                        }\
                    }\
                    &__ARRAY[0];})

#define CLASS_DEF(TYPE,CLASS,...) CLASS##TYPE##_DEF(CLASS,##__VA_ARGS__)
#define VALUE_INIT(NAME,VALUE) NAME = likely(NAME == 0 ) ? VALUE : NAME;
#define VALUE_CHARGE(NAME,VALUE) NAME = unlikely(NAME == 0 ) ? VALUE : NAME;


#define NEW(CLASS,PTR)({\
    typeof(PTR) __TMP__ = CLASS();\
    memcpy(__TMP__.this,PTR.this,PTR.pub);\
    __TMP__;})

#endif
