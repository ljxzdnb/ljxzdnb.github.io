#define CHEACK(SINGAL,SLOT) ({typeof(SINGAL) __tmp = SLOT;(void)__tmp;})
#define CHEACKK(SINGAL,SLOT) ({typeof(SINGAL) __tmp = SINGAL;typeof(SLOT) _tmp = SLOT;(void)(&_tmp == &__tmp);printf("%p %p\n",&__tmp, &_tmp);})
#define CHEACKKK(SINGAL,SLOT) ({if((void*)SINGAL == (void*)SLOT && sizeof(SINGAL) == sizeof(SLOT)){printf("%s\n", "same type");}})

struct test_t{
    void (*fuc)(int);
    int (*fcc)(int);
    void *call;
}test;

struct text_t{
    int (*fuc)(char);
    void *slt[2];
}text;

void ll(int num){}

int check (void* fuc){
    test.call = ll;
    struct test_t ts;
    CHEACK(test.fuc,fuc);
    CHEACKK(test.fuc,fuc);
    // printf("%p\n",(void*)ts.fuc);
    // printf("%p\n",(void*)fuc);
    // printf("%d\n",sizeof(ts.fuc));
    // printf("%d\n",sizeof(fuc));
    CHEACKKK(ts.fuc,fuc);
    //CHEACKKK(test.fuc,fuc);
}


int main(){
    test.call = ll;
    struct text_t tx;
    tx.slt[0] = text.fuc;
    check(tx.slt[0]);
    //CHEACK(test.fuc,ll);
    //CHEACKK(test.fuc,test.fcc);
}
