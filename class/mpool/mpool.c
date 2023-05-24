//
// Created by 68725 on 2022/7/26.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpool.h"

#define PAGE_SIZE 4096
#define MP_ALIGNMENT 16
#define mp_align(n, alignment) (((n)+(alignment-1)) & ~(alignment-1))
#define mp_align_ptr(p, alignment) (void *)((((size_t)p)+(alignment-1)) & ~(alignment-1))



void _mpool_recvchar_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucppppp:%s\n", (char*)arg);
}

void _mpool_recvnum_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%d\n", *(int*)arg);
}

void _mpool_print_f(mpool *this){}

void _mpool_set_f(mpool *this,char *mem,void* arg) {
#define _mpool_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _mpool_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;   
CLASS_DEF(PUB,_mpool_,_object_)
}
#undef _mpool_PTR
#undef _mpool_PUB

#define _mpool_PTR _mpool_PUB
void* _mpool_get_f(mpool *this,char *mem) {
#define _mpool_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
CLASS_DEF(PUB,_mpool_,_object_)
}
#undef _mpool_PUB

void* _mpool_pos_f(mpool *this,char *mem) {
#define _mpool_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;    
CLASS_DEF(PUB,_mpool_,_object_)
}
#undef _mpool_PUB
#undef _mpool_SLT



void _mpool_mpool_f(mpool *this,size_t size);

void _mpool_destroy_f(mpool *this);

void * _mpool_malloc_f(mpool *this, size_t size);

void *mp_calloc(mpool *this, size_t size);

void _mpool_free_f(mpool *this, void *p);

void _mpool_reset_f(mpool *this);

void _mpool_monitor_f(mpool *this, char *tk);


void _mpool_reset_f(mpool *this) {
    struct mp_node_s *cur;
    struct mp_large_s *large;

    for (large = this->large; large; large = large->next) {
        if (large->alloc) {
            free(large->alloc);
        }
    }

    this->large = NULL;
    this->current = this->head;
    for (cur = this->head; cur; cur = cur->next) {
        cur->last = (unsigned char *) cur + sizeof(struct mp_node_s);
        cur->failed = 0;
        cur->quote = 0;
    }
}

//创建内存池
void _mpool_mpool_f(mpool *this,size_t size) {
    if (size < PAGE_SIZE || size % PAGE_SIZE != 0) {
        size = PAGE_SIZE;
    }
    //分配4k以上不用malloc，用posix_memalign
    /*
        int posix_memalign (void **memptr, size_t alignment, size_t size);
     */

    int ret = posix_memalign((void **) &this->data, MP_ALIGNMENT, size); //4K + mp_this_s
    if (ret < 0 ) {
        printf("mpool init failed");
    }
    this->large = NULL;
    this->current = this->head = (struct mp_node_s *) this->data;
    this->head->last = (unsigned char *) this->data + sizeof(struct mp_node_s);
    this->head->end = (unsigned char *) this->data + PAGE_SIZE;
    this->head->failed = 0;
}

//销毁内存池
void _mpool__mpool_f(mpool *this) {
    struct mp_large_s *large;
    for (large = this->large; large; large = large->next) {
        if (large->alloc) {
            free(large->alloc);
        }
    }

    struct mp_node_s *cur, *next;
    cur = this->head->next;

    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(this->data);
}

//size>4k
void *mp_malloc_large(mpool *this, size_t size) {
    unsigned char *big_addr;
    int ret = posix_memalign((void **) &big_addr, MP_ALIGNMENT, size); //size
    if (ret) {
        return NULL;
    }

    struct mp_large_s *large;
    //released struct large resume
    int n = 0;
    for (large = this->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->size = size;
            large->alloc = big_addr;
            return big_addr;
        }
        if (n++ > 3) {
            break;// 为了避免过多的遍历，限制次数
        }
    }
    large = _mpool_malloc_f(this,sizeof(struct mp_large_s));
    if (large == NULL) {
        free(big_addr);
        return NULL;
    }
    large->size = size;
    large->alloc = big_addr;
    large->next = this->large;
    this->large = large;
    return big_addr;
}

//new block 4k
void *mp_malloc_block(mpool *this, size_t size) {
    unsigned char *block;
    int ret = posix_memalign((void **) &block, MP_ALIGNMENT, PAGE_SIZE); //4K
    if (ret) {
        return NULL;
    }
    struct mp_node_s *new_node = (struct mp_node_s *) block;
    new_node->end = block + PAGE_SIZE;
    new_node->next = NULL;

    unsigned char *ret_addr = mp_align_ptr(block + sizeof(struct mp_node_s), MP_ALIGNMENT);

    new_node->last = ret_addr + size;
    new_node->quote++;

    struct mp_node_s *current = this->current;
    struct mp_node_s *cur = NULL;

    for (cur = current; cur->next; cur = cur->next) {
        if (cur->failed++ > 4) {
            current = cur->next;
        }
    }
    //now cur = last node
    cur->next = new_node;
    this->current = current;
    return ret_addr;
}

//分配内存
void *_mpool_malloc_f(mpool *this, size_t size) {
    if (size <= 0) {
        return NULL;
    }
    if (size > PAGE_SIZE - sizeof(struct mp_node_s)) {
        //large
        return mp_malloc_large(this, size);
    }
    else {
        //small
        unsigned char *mem_addr = NULL;
        struct mp_node_s *cur = NULL;
        cur = this->current;
        while (cur) {
            mem_addr = mp_align_ptr(cur->last, MP_ALIGNMENT);
            if (cur->end - mem_addr >= size) {
                cur->quote++;//引用+1
                cur->last = mem_addr + size;
                return mem_addr;
            }
            else {
                cur = cur->next;
            }
        }
        return mp_malloc_block(this, size);// open new space
    }
}

void *mp_calloc(mpool *this,size_t size) {
    void *mem_addr = this->malloc(size);
    if (mem_addr) {
        memset(mem_addr, 0, size);
    }
    return mem_addr;
}

//释放内存
void _mpool_free_f(mpool *this, void *p) {
    struct mp_large_s *large;
    for (large = this->large; large; large = large->next) {//大块
        if (p == large->alloc) {
            free(large->alloc);
            large->size = 0;
            large->alloc = NULL;
            return;
        }
    }
    //小块 引用-1
    struct mp_node_s *cur = NULL;
    for (cur = this->head; cur; cur = cur->next) {
//        printf("cur:%p   p:%p   end:%p\n", (unsigned char *) cur, (unsigned char *) p, (unsigned char *) cur->end);
        if ((unsigned char *) cur <= (unsigned char *) p && (unsigned char *) p <= (unsigned char *) cur->end) {
            cur->quote--;
            if (cur->quote == 0) {
                if (cur == this->head) {
                    this->head->last = (unsigned char *) this->data + sizeof(struct mp_node_s);
                }
                else {
                    cur->last = (unsigned char *) cur + sizeof(struct mp_node_s);
                }
                cur->failed = 0;
                this->current = this->head;
            }
            return;
        }
    }
}

void _mpool_monitor_f(mpool *this, char *tk) {
    printf("\r\n\r\n------start monitor poll------%s\r\n\r\n", tk);
    struct mp_node_s *head = NULL;
    int i = 0;
    for (head = this->head; head; head = head->next) {
        i++;
        if (this->current == head) {
            printf("current==>第%d块\n", i);
        }
        if (i == 1) {
            printf("第%02d块small block  已使用:%4ld  剩余空间:%4ld  引用:%4d  failed:%4d\n", i,(unsigned char *) head->last - (unsigned char *) this->data,head->end - head->last, head->quote, head->failed);
        }
        else {
            printf("第%02d块small block  已使用:%4ld  剩余空间:%4ld  引用:%4d  failed:%4d\n", i,(unsigned char *) head->last - (unsigned char *) head,head->end - head->last, head->quote, head->failed);
        }
    }
    struct mp_large_s *large;
    i = 0;
    for (large = this->large; large; large = large->next) {
        i++;
        if (large->alloc != NULL) {
            printf("第%d块large block  size=%d\n", i, large->size);
        }
    }
    printf("\r\n\r\n------stop monitor poll------\r\n\r\n");
}


#ifdef __MAIN__
int main() {
    mpool pool = Lmpool(.name="mp");
    pool.create(PAGE_SIZE);
    mpool *p = pool.this;
    pool.monitor("create memory pool");
#if 0
    printf("mp_align(5, %d): %d, mp_align(17, %d): %d\n", MP_ALIGNMENT, mp_align(5, MP_ALIGNMENT), MP_ALIGNMENT,
           mp_align(17, MP_ALIGNMENT));
    printf("mp_align_ptr(p->current, %d): %p, p->current: %p\n", MP_ALIGNMENT, mp_align_ptr(p->current, MP_ALIGNMENT),
           p->current);
#endif
    void *mp[30];
    int i;
    for (i = 0; i < 30; i++) {
        mp[i] = pool.malloc(512);
    }
    pool.monitor("申请512字节30个");

    for (i = 0; i < 30; i++) {
        pool.free(mp[i]);
    }
    pool.monitor("销毁512字节30个");

    int j;
    for (i = 0; i < 50; i++) {
        char *pp = mp_calloc(p, 32);
        for (j = 0; j < 32; j++) {
            if (pp[j]) {
                printf("calloc wrong\n");
                exit(-1);
            }
        }
    }
    pool.monitor("申请32字节50个");

    for (i = 0; i < 50; i++) {
        char *pp = pool.malloc(3);
    }
    pool.monitor("申请3字节50个");


    void *pp[10];
    for (i = 0; i < 10; i++) {
        pp[i] = pool.malloc(5120);
    }
    pool.monitor("申请大内存5120字节10个");

    for (i = 0; i < 10; i++) {
        pool.free(pp[i]);
    }
    pool.monitor("销毁大内存5120字节10个");

    pool.reset();
    pool.monitor("reset this");

    for (i = 0; i < 100; i++) {
        void *s = pool.malloc(256);
    }
    pool.monitor("申请256字节100个");

    pool.destroy();
    return 0;
}
#endif
