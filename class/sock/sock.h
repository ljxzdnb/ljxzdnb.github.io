#ifndef CLASS_SOCK_H
#define CLASS_SOCK_H

#include <sys/socket.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <netdb.h>
#include "../mpool/mpool.h"

#define CLASS_SOCK_PUB CLASS_DEF(PUB,_sock_,_mpool_,_object_)
#define CLASS_SOCK_PIV CLASS_DEF(PIV,_sock_,_mpool_,_object_)
#define CLASS_SOCK_FUC CLASS_DEF(FUC,_sock_,_mpool_,_object_)
#define CLASS_SOCK_SIG CLASS_DEF(SIG,_sock_,_mpool_,_object_)
#define CLASS_SOCK_SLT CLASS_DEF(SLT,_sock_,_mpool_,_object_)

void _sock_recvchar_f(void* arg);

void _sock_recvnum_f(void* arg);

//----------------------------------------------------------------------------------------------------------------------
static int _sock = 0;

#define _sock_PUB_DEF(CLASS,_CLASS,...)        \
    _CLASS##PUB_DEF(CLASS,##__VA_ARGS__)        \
    CLASS##PUB(CLASS,int,size)                  \
    CLASS##PUB(CLASS,int,type)                  \
    CLASS##PUB(CLASS,int,protol)                \
    CLASS##PUB(CLASS,int*,serverfd)             \
    CLASS##PUB(CLASS,int*,clientfd)             \
    CLASS##PUB(CLASS,int,epollfd)               \
    CLASS##PUB(CLASS,struct epoll_event,evsize[__EVSIZE__]) \
    CLASS##PUB(CLASS,struct sockaddr_in*,server)\
    CLASS##PUB(CLASS,struct sockaddr_in*,client)\
    CLASS##PUB(CLASS,int,protofamily)

#define _sock_PIV_DEF(CLASS,_CLASS,...)        \
    _CLASS##PIV_DEF(CLASS,##__VA_ARGS__)

#define _sock_FUC_DEF(CLASS,_CLASS,...)        \
    _CLASS##FUC_DEF(CLASS,##__VA_ARGS__)        \
    CLASS##FUC(CLASS,void,socket,int,protofamily,int,type)\
    CLASS##FUC(CLASS,void,fcntl,int,cmd,long,arg)\
    CLASS##LUC(CLASS,uint32_t,host,char*,ip)    \
    CLASS##FUC(CLASS,void,bind,char*,ip,uint16_t,port)\
    CLASS##LUC(CLASS,void,listen,int,num)       \
    CLASS##LUC(CLASS,void,accept,int,index)     \
    CLASS##LUC(CLASS,void,close, int,index)     \
    CLASS##LUC(CLASS,void,epoll,int,num)       \
    CLASS##FUC(CLASS,void,epctl,int,cmd,struct epoll_event*,ev) \
    CLASS##VUC(CLASS,int,wait,void)       \
    CLASS##FUC(CLASS,void,connect,char*,ip,uint16_t,port)\
    CLASS##FUC(CLASS,void,send,char*,buffer,int,size)\
    CLASS##FUC(CLASS,void,recv,char*,buffer,int,size)


#define _sock_SIG_DEF(CLASS,_CLASS,...) \
    _CLASS##SIG_DEF(CLASS,##__VA_ARGS__)

#define _sock_SLT_DEF(CLASS,_CLASS,...) \
    _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)

#define _sock_PUB(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _sock_PIV(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _sock_PTR(CLASS,TYPE,MEMBER) TYPE MEMBER;
#define _sock_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE (*MEMBER)(ATYPE_ ARG_,ATYPE__ ARG__);
#define _sock_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _sock_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE (*MEMBER)(ATYPE_);
#define _sock_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_s,
#define _sock_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) CLASS##MEMBER##_r,

typedef enum _sock_signal_t{
    CLASS_SOCK_SIG
    _sock_SIGNAL_NUM
}_sock_signal;
typedef enum _sock_slot_t{
    CLASS_SOCK_SLT
    _sock_SLOT_NUM
}_sock_slot;
#undef _sock_SIG
#undef _sock_SLT


#define _sock_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
#define _sock_SLT(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE (*MEMBER)(ATYPE_ ARG_);
typedef struct _sock_class_t{
    CLASS_SOCK_PUB
    CLASS_SOCK_PIV
    CLASS_SOCK_FUC
    CLASS_SOCK_SIG
    CLASS_SOCK_SLT
#ifdef _GRAPHVIZ_PLOT
    Agraph_t *_node;
    Agedge_t *_next,*_prev;
#endif
}sock;

typedef struct _sock_pub_t{
    CLASS_SOCK_PUB
}_sock_pub;

#undef _sock_SIG
#undef _sock_SLT
#undef _sock_PUB
#undef _sock_PTR
#undef _sock_FUC
#undef _sock_LUC
#undef _sock_VUC


#define _sock_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) TYPE CLASS##MEMBER##_f(sock* this,ATYPE_ ARG_,ATYPE__ ARG__);
#define _sock_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_) TYPE CLASS##MEMBER##_f(sock* this,ATYPE_ ARG_);
#define _sock_VUC(CLASS,TYPE,MEMBER,ATYPE_) TYPE CLASS##MEMBER##_f(sock* this);
CLASS_SOCK_FUC
#undef _sock_FUC
#undef _sock_LUC
#undef _sock_VUC




//#########################################################################################################################################
#define _sock_FUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__)     \
inline TYPE MEMBER##_sock(ATYPE_ ARG_,ATYPE__ ARG__){              \
    return CLASS##MEMBER##_f(&object_##_sock,ARG_,ARG__);          \
}                                                                   \
object_##_sock.MEMBER = MEMBER##_sock;                            \

#define _sock_LUC(CLASS,TYPE,MEMBER,ATYPE_,ARG_)                   \
inline TYPE MEMBER##_sock(ATYPE_ ARG_){                            \
    return CLASS##MEMBER##_f(&object_##_sock,ARG_);                \
}                                                                   \
object_##_sock.MEMBER = MEMBER##_sock;                            \

#define _sock_VUC(CLASS,TYPE,MEMBER,ATYPE_)                        \
inline TYPE MEMBER##_sock(ATYPE_){                                 \
    return CLASS##MEMBER##_f(&object_##_sock);                     \
}                                                                   \
object_##_sock.MEMBER = MEMBER##_sock;                            \


#define Lsock(...) __lsock(_sock,__VA_ARGS__)
#define __lsock(_sock,...)({                                        \
    sock object_##_sock = {                                         \
    .this = &object_##_sock,                                        \
    .ptr = &object_##_sock.print,                                   \
    .pub = sizeof(_sock_##pub),                                     \
    .ID = _sock,                                                    \
    __VA_ARGS__                                                     \
    };                                                              \
    CLASS_SOCK_FUC                                                  \
    _sock++;                                                        \
    object_##_sock;                                                 \
})
#undef _sock_SIG
//#undef _sock_SLT
#undef _sock_PUB
#undef _sock_PTR
// #undef _sock_FUC
// #undef _sock_LUC
#endif
