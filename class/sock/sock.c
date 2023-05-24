//
// Created by ljx on 5/6/23.
//
#include "sock.h"

void _sock_recvchar_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%s\n", (char*)arg);
}

void _sock_recvnum_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%d\n", *(int*)arg);
}

void _sock_print_f(sock *this){}

void _sock_set_f(sock *this,char *mem,void* arg) {
#define _sock_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _sock_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;
    CLASS_SOCK_PUB
}
#undef _sock_PTR
#undef _sock_PUB

#define _sock_PTR _sock_PUB
void* _sock_get_f(sock *this,char *mem) {
#define _sock_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
    CLASS_SOCK_PUB
}
#undef _sock_PUB

void* _sock_pos_f(sock *this,char *mem) {
#define _sock_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
    CLASS_SOCK_PUB
}
#undef _sock_PUB
#undef _sock_SLT

void _sock_mpool_f(sock *this,size_t size){
    _mpool_mpool_f((mpool*)this,size);
}

void _sock__mpool_f(sock *this){
    return _mpool__mpool_f((mpool*)this);
}

void * _sock_malloc_f(sock *this, size_t size){
    _mpool_malloc_f((mpool*)this,size);
}

void _sock_free_f(sock *this, void *p){
    _mpool_free_f((mpool*)this,p);
}

void _sock_reset_f(sock *this){
    _mpool_reset_f((mpool*)this);
}

void _sock_monitor_f(sock* this, char *tk){
    _mpool_monitor_f((mpool*)this,tk);
}

void _sock_epoll_f(sock* this,int num){
    this->epollfd = epoll_create(num);
    if (this->epollfd <= 0) {
        printf("create epfd in %s err %s\n", __func__, strerror(errno));
    }
}

void _sock_epctl_f(sock* this,int cmd,struct epoll_event* ev){
    int ret = epoll_ctl(this->epollfd, cmd, ev->data.fd, NULL);
//#ifdef __DEBUG__
//    if(ret == -1)
//    {
//        perror("epoll_ctl del cfd error");
//        exit(1);
//    }
//    close(this->epollfd);
//#elif
}
int _sock_wait_f(sock* this){
    int ret = epoll_wait(this->epollfd, this->evsize, sizeof (this->evsize)/sizeof (this->evsize[0]), -1);
}

/*const char* tojson(class *this,char *json) {
#define _sock_PUB(MEMTYPE,MEMBER) json = (char *)malloc(sizeof (char)*(strlen(#MEMBER)+strlen(&this->MEMBER)+1)) ;printf();
OBJECT_PUB_DEF
}
#undef _sock_PUB*/

//#########################################################################################################################################

void _sock_socket_f(sock *this,int protofamily,int type){
    printf("socket\n");
    VALUE_INIT(this->pagesize,4096);
    VALUE_INIT(this->protol,0);
    VALUE_INIT(this->size,1);
    if(this->data == NULL) _sock_mpool_f(this,this->pagesize);
    this->serverfd = _sock_malloc_f(this,(this->size+1)*(sizeof(int)));
    this->clientfd  = _sock_malloc_f(this,(this->size+1)*(sizeof(int)));
    this->protofamily = protofamily;
    this->type = type;
    for(int i = 1 ;i <= this->size;i++){
        this->serverfd[i] = socket(protofamily,type,this->protol);
#ifdef __DEBUG__
        if(this->serverfd[i] < 0) printf("socket failed : %s\n", strerror(errno));
#endif
    }
    this->serverfd[0]   = this->serverfd[1];
    this->server = _mpool_malloc_f((mpool*)this,(this->size+1)*(sizeof(struct sockaddr_in)));
}


void _sock_fcntl_f(sock *this,int cmd,long arg){
    printf("fcntl\n");
    for(int i = 1 ;i <= this->size;i++){
        fcntl(this->serverfd[i],cmd,arg);
    }
}


uint32_t _sock_host_f(sock *this,char* ip){
    struct hostent* host;
    struct in_addr addr;
    if (ip == NULL){
        addr.s_addr = htonl(INADDR_ANY);
    }
    else if (strstr(ip,"www")){
        host = gethostbyname(ip);
        char** pp = host->h_addr_list;
        addr.s_addr = *((unsigned int *)*pp);
        printf("%s address is %s\n", ip, inet_ntoa(addr));
    }
    else if(strstr(ip,".")){
        addr.s_addr = inet_addr(ip);
    }
    else{
        addr.s_addr = htonl(INADDR_ANY);
    }
    return addr.s_addr;
}

//_sock_LUC(CLASS,void,bind,struct socket_in*,addr)

void _sock_bind_f(sock *this,char* ip,uint16_t port){
    printf("bind\n");
    int flag = 1;
    for(int i = 1 ;i <= this->size;i++){
        this->server[i].sin_family = this->protofamily;
        this->server[i].sin_addr.s_addr = _sock_host_f(this,ip);
        this->server[i].sin_port = htons(port+i-1);
        bind(this->serverfd[i],(struct sockaddr *) &this->server[i], sizeof(struct sockaddr_in));
    }
}

void _sock_listen_f(sock *this,int num){
    printf("listen\n");
    for(int i = 0 ;i < this->size;i++){
        int ret = listen(this->serverfd[i],num);
#ifdef __DEBUG__
        if(ret < 0) printf("listen failed : %s\n", strerror(errno));
#endif
    }
}

void _sock_accept_f(sock *this,int index){
    printf("accept\n");
    this->client = _mpool_malloc_f((mpool*)this,this->size*(sizeof(struct sockaddr_in)));
    int size = sizeof(struct sockaddr_in);
    this->clientfd[0] = accept(this->serverfd[index],(struct sockaddr*)&this->client[index],&size);
#ifdef __DEBUG__
    if(this->clientfd[index] < 0) printf("accept failed : %s\n", strerror(errno));
#endif
    this->clientfd[index] = this->clientfd[0];
}

void _sock_connect_f(sock *this,char* ip,uint16_t port){
    printf("connect\n");
    struct sockaddr_in addr;
    addr.sin_family = this->protofamily;
    addr.sin_addr.s_addr = _sock_host_f(this,ip);
    addr.sin_port = htons(port);
    int ret = connect(this->serverfd[0],(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
#ifdef __DEBUG__
    if(ret < 0) printf("connect failed : %s\n", strerror(errno));
#endif
}

void _sock_send_f(sock *this,char* buffer,int size){
    printf("send\n");
    int ret = send(this->serverfd[0],buffer,size,0);
#ifdef __DEBUG__
    if(ret < 0) printf("send failed : %s\n", strerror(errno));
#endif
}

void _sock_recv_f(sock *this,char* buffer,int size){
    printf("recv\n");
    struct sockaddr_in addr;
    int ret = recv(this->clientfd[0],buffer,size,0);
#ifdef __DEBUG__
    if(ret < 0) printf("recv failed : %s\n", strerror(errno));
#endif
}

void _sock_close_f(sock *this,int index){
    printf("close\n");
    int ret = close(this->serverfd[index]);
#ifdef __DEBUG__
    if(ret < 0) printf("accept failed : %s\n", strerror(errno));
#endif
}
