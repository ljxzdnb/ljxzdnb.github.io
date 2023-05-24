#define CONNECT(SINGAL,SLOT) ({typeof(SINGAL) __tmp = SINGAL;typeof(SLOT) _tmp = SLOT;(void)(&_tmp == &__tmp);SINGAL=SLOT;})
#include <stdio.h>
void print(char* str){
    printf("%s\n",str);
}

struct test_t{
	int ID;
	struct test_t* self;
	void (*fuc)(char* this);
};

struct text_t{
    int ID;
    struct test_t* self;
    void (*fuc)(char* this);
};


int main(){
	struct test_t test;
    struct text_t text;
    text.fuc = print;
    CONNECT(test.fuc,text.fuc);
//    test.fuc=text.fuc;
    test.fuc("ljx");
}
