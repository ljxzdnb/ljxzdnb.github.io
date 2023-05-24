// ring_queue.c
#include "ring.h"

#define CAS(ptr, old, new) __sync_bool_compare_and_swap(ptr, old, new)
#define is_power_of_2(x) ((x) != 0 && (((x)&((x)-1))==0))
void _ring_recvchar_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%s\n", (char*)arg);
}

void _ring_recvnum_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%d\n", *(int*)arg);
}

void _ring_print_f(ring *this){}

void _ring_set_f(ring *this,char *mem,void* arg) {
#define _ring_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _ring_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;
    CLASS_RING_PUB
}
#undef _ring_PTR
#undef _ring_PUB

#define _ring_PTR _ring_PUB
void* _ring_get_f(ring *this,char *mem) {
#define _ring_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
    CLASS_RING_PUB
}
#undef _ring_PUB

void* _ring_pos_f(ring *this,char *mem) {
#define _ring_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
    CLASS_RING_PUB
}
#undef _ring_PUB
#undef _ring_SLT

void _ring_mpool_f(ring *this,size_t size){
    _mpool_mpool_f((mpool*)this,size);
}

void _ring__mpool_f(ring *this){
    return _mpool__mpool_f((mpool*)this);
}

void * _ring_malloc_f(ring *this, size_t size){
    _mpool_malloc_f((mpool*)this,size);
}

void _ring_free_f(ring *this, void *p){
    _mpool_free_f((mpool*)this,p);
}

void _ring_reset_f(ring *this){
    _mpool_reset_f((mpool*)this);
}

void _ring_monitor_f(ring* this, char *tk){
    _mpool_monitor_f((mpool*)this,tk);
}

int ring_queue_is_empty(ring* this) {
    return (this->ringhead == this->ringtail) && this->same_cycle;
}
int ring_queue_is_full(ring* this) {
    return (this->ringhead == this->ringtail) && !this->same_cycle;
}

void _ring_ringbuffer_f(ring* this, int item_size, int capacity) {
    VALUE_INIT(this->pagesize,4096);
    this->item_size = item_size;
    this->capacity = capacity;
    if(this->data==NULL) _ring_mpool_f(this,this->pagesize);
    this->pbuf = (char*)_ring_malloc_f(this,item_size * capacity);
    if (!this->pbuf) {
    }
    if(!is_power_of_2(capacity)){
        perror("capacity must be power of 2\n");
        return;
    }
    this->item_size = item_size;
    this->capacity = capacity;
    this->same_cycle = 1;
}

void _ring__ringbuffer_f(ring* this) {
    _ring_free_f(this,this->pbuf);
}

void _ring_synfile_f(ring *this,char* filename){
    VALUE_INIT(this->synfd,open(filename, O_RDWR|O_CREAT,0666));
    write(this->synfd,this->pbuf,this->item_size * this->capacity);
    _ring__ringbuffer_f(this);
    this->pbuf = (char *)mmap(0,this->item_size * this->capacity, PROT_READ | PROT_WRITE, MAP_SHARED, this->synfd, 0);
    if(this->pbuf == MAP_FAILED){
        perror("mmap");
    }

//    munmap((void *)this->pbuf, this->item_size * this->capacity);
//    close(this->synfd);
}

void _ring_push_f(ring* this, void* pitem) {
    // try to set write flag
    while (1) {
        if (ring_queue_is_full(this)) {
            break;
        }
        if (CAS(&this->write_flag, 0, 1)) {   // set write flag successfully
#ifdef __DEBUG__
            printf("push value:%d\n",*(int*)pitem);
#endif
            memcpy(this->pbuf + this->ringtail * this->item_size, pitem, this->item_size);
            this->ringtail = (this->ringtail + 1) & (this->capacity-1);
            if (0 == this->ringtail) {    // a new cycle
                this->same_cycle = 0;     // tail is not the same cycle with head
            }
            break;
        }
    }
    // push data
    // reset write flag
    CAS(&this->write_flag, 1, 0);
}
void _ring_pop_f(ring* this, void* pitem) {
    // try to set read flag
    while (1) {
        if (ring_queue_is_empty(this)) {
            perror("fail queue is empty");
        }
        if (CAS(&this->read_flag, 0, 1)) {    // set read flag successfully
            memcpy(pitem, this->pbuf + this->ringhead * this->item_size, this->item_size);
#ifdef __DEBUG__
            printf("pop value:%d\n",*(int*)pitem);
#endif
            memset(this->pbuf + this->ringhead * this->item_size,0,this->item_size);
            this->ringhead = (this->ringhead + 1) & (this->capacity-1);
            if (0 == this->ringhead) {
                this->same_cycle = 1;     // head is now the same cycle with tail
            }
            break;
        }
    }
    // read data
    // reset read flag
    CAS(&this->read_flag, 1, 0);
}
#ifdef __ZIP__
void _ring_compress_f(ring* this) {
    this->destlen = compressBound(this->capacity*this->item_size);
    this->zbuf = (char*)_ring_malloc_f(this,this->destlen);
    compress(this->zbuf,&this->destlen,this->pbuf,this->capacity*this->item_size);
}

void _ring_uncompress_f(ring* this) {
    this->srclen = this->capacity*this->item_size;
    uncompress(this->pbuf,&this->srclen,this->zbuf,this->destlen);
}
#endif


#ifdef CLASS_SOCK_H
void _ring_socket_f(ring *this,int protofamily,int type){
    _sock_socket_f((sock*)this,protofamily,type);
}

void _ring_epoll_f(ring* this,int num){
    _sock_epoll_f((sock*)this,num);
}
void _ring_epctl_f(ring* this,int cmd,struct epoll_event* ev){
    _sock_epctl_f((sock*)this,cmd,ev);
}

int _ring_wait_f(ring* this){
    _sock_wait_f((sock*)this);
}

void _ring_fcntl_f(ring *this,int cmd,long arg){
    _sock_fcntl_f((sock*)this,cmd,arg);
}


uint32_t _ring_host_f(ring *this,char* ip){
    _sock_host_f((sock*)this,ip);
}

//_ring_LUC(CLASS,void,bind,struct socket_in*,addr)

void _ring_bind_f(ring *this,char* ip,uint16_t port){
    _sock_bind_f((sock*)this,ip,port);
}

void _ring_listen_f(ring *this,int num){
    _sock_listen_f((sock*)this,num);
}

void _ring_accept_f(ring *this,int index){
    _sock_accept_f((sock*)this,index);
}

void _ring_connect_f(ring *this,char* ip,uint16_t port){
    _sock_connect_f((sock*)this,ip,port);
}

void _ring_send_f(ring *this,char* buffer,int size){
    _sock_send_f((sock*)this,buffer,size);
}

void _ring_recv_f(ring *this,char* buffer,int size){
    _sock_recv_f((sock*)this,buffer,size);
}

void _ring_close_f(ring *this,int index){
    _sock_close_f((sock*)this, index);
}

void _ring_sendbuffer_f(ring *this){
    while (1) {
        if (ring_queue_is_empty(this)) {
            perror("fail queue is empty");
        }
        if (CAS(&this->read_flag, 0, 1)) {    // set read flag successfully
            _sock_send_f((sock*)this,this->pbuf + this->ringhead * this->item_size,this->item_size);
            memset(this->pbuf + this->ringhead * this->item_size,0,this->item_size);
            this->ringhead = (this->ringhead + 1) & (this->capacity-1);
            if (0 == this->ringhead) {
                this->same_cycle = 1;     // head is now the same cycle with tail
            }
            break;
        }
    }
    // read data
    // reset read flag
    CAS(&this->read_flag, 1, 0);
}

void _ring_recvbuffer_f(ring *this){
    // try to set write flag
    while (1) {
//        if (ring_queue_is_full(this)) {
//            break;
//        }
        if (CAS(&this->write_flag, 0, 1)) {   // set write flag successfully
            _sock_recv_f((sock*)this,this->pbuf + this->ringtail * this->item_size, this->item_size);
            printf("recvnum:%d\n",*(int*)(this->pbuf + this->ringtail * this->item_size));
            if(this->synfd != 0) msync( this->pbuf, this->item_size * this->capacity, MS_ASYNC);
            perror("msync");
            this->ringtail = (this->ringtail + 1) & (this->capacity-1);
            if (0 == this->ringtail) {    // a new cycle
                this->same_cycle = 0;     // tail is not the same cycle with head
            }
            break;
        }
    }
    // push data
    // reset write flag
    CAS(&this->write_flag, 1, 0);
}
#endif
