#include "heap.h"

int get_index(heap *this,int index){
    return *(int*)(this->data+(index)*this->size+this->off);
}

void _heap_recvchar_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucppppp:%s\n", (char*)arg);
}

void _heap_recvnum_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%d\n", *(int*)arg);
}

void _heap_print_f(heap *this){
    for (int i = 0; i < this->num; i++) {
        printf("%d ", get_index(this,i));
    }
    printf("\n");
}

void _heap_set_f(heap *this,char *mem,void* arg) {
#define _heap_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _heap_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;   
CLASS_DEF(PUB,_heap_,_object_)
}
#undef _heap_PTR
#undef _heap_PUB

#define _heap_PTR _heap_PUB
void* _heap_get_f(heap *this,char *mem) {
#define _heap_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
CLASS_DEF(PUB,_heap_,_object_)
}
#undef _heap_PUB

void* _heap_pos_f(heap *this,char *mem) {
#define _heap_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;    
CLASS_DEF(PUB,_heap_,_object_)
}
#undef _heap_PUB
#undef _heap_SLT

void _heap_create_f(heap *this,int size,int off){
    this->num = 0;
    this->size = size;
    this->off = off;
    this->capacity = 4;
    this->data = malloc(this->capacity*size);
}

int parent(int index) {
    return (index - 1) / 2;
}

int left_child(int index) {
    return index * 2 + 1;
}

int right_child(int index) {
    return index * 2 + 2;
}

void* pos_index(heap *this,int index){
    return this->data+(index)*this->size;
}

void _heap_callback_f(heap *this,void* fuc){
    ((void(*)(void*))fuc)(this->data);
}


void resize(heap *this) {
    if (this->num == this->capacity) {
        this->capacity *= 2;
        this->data = realloc(this->data, this->capacity * sizeof(this->size));
    }
}

void _heap_insert_f(heap* this, void* arg) {
    resize(this);
    if (this->num >= this->capacity) {
        printf("heap overflow!\n");
        return;
    }
    memcpy(pos_index(this,this->num),arg,this->size);
    int index = this->num;
    this->num++;
    // 上滤操作
    while (index > 0 && get_index(this,index) < get_index(this,parent(index))) {
        void* tmp = malloc(this->size);
        memcpy(tmp,pos_index(this,index),this->size);
        memcpy(pos_index(this,index),pos_index(this,parent(index)),this->size);
        memcpy(pos_index(this,parent(index)),tmp,this->size);
        index = get_index(this,parent(index));
    }
}

void _heap_delete_f(heap* this) {
    if (this->num == 0) {
        printf("heap is empty!\n");
        return;
    }
    int index= 0;
    memcpy(pos_index(this,index),pos_index(this,this->num-1),this->size);
    this->num--;
    // 下滤操作
    while (left_child(index) < this->num) {
        int smaller_child = left_child(index); // 先假设左孩子较小
        if (right_child(index) < this->num && get_index(this,right_child(index)) < get_index(this,smaller_child)) {
            smaller_child = right_child(index); // 如果右孩子比左孩子小，则右孩子更小
        }
        if (get_index(this,index) > get_index(this,smaller_child)) { // 如果父节点比较小的孩子大，则交换两者
            char* tmp = malloc(this->size);
            memcpy(tmp,pos_index(this,index),this->size);
            memcpy(pos_index(this,index),pos_index(this,smaller_child),this->size);
            memcpy(pos_index(this,smaller_child),tmp,this->size);
        } else {
            break; // 父节点已经比孩子节点都小了，不需要再下滤了
        }
        index = smaller_child;
    }
}

#ifdef __MAIN__
    int main(){
        heap tb = Lheap(.name="ljx");
    }
#endif
