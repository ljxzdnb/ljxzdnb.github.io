#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define SUM(ARG,...) ({ typeof(ARG) sum = (typeof(ARG))0;typeof(ARG) __ARRAY[] = {ARG,__VA_ARGS__}; \
                    for (int i = 0; i < sizeof((typeof(ARG)[]){ARG,__VA_ARGS__})/sizeof(typeof(ARG)); ++i) \
                        sum += __ARRAY[i]; \
                    sum; })

// #define lambda(FUC,ARG,...) ({ typeof(ARG) sum = (typeof(ARG))0;typeof(ARG) __ARRAY[] = {ARG,__VA_ARGS__}; \
//                     for (int i = 0; i < sizeof((typeof(ARG)[]){ARG,__VA_ARGS__})/sizeof(typeof(ARG)); ++i) \
//                         __ARRAY[i] = FUC((typeof(ARG)*)&__ARRAY[i]); \
//                      &__ARRAY;})


// #define lambda(LEFT,CON,RIGHT,ARG,...) ({ typeof(ARG) sum = (typeof(ARG))0;typeof(ARG) __ARRAY[] = {ARG,##__VA_ARGS__}; \
//                     for (int i = 1; i < sizeof((typeof(ARG)[]){ARG,__VA_ARGS__})/sizeof(typeof(ARG)); ++i) \
//                          __ARRAY[i] = LEFT(__ARRAY[i-1]) CON (__ARRAY[i])RIGHT; \
//                       &__ARRAY[sizeof((typeof(ARG)[]){ARG,__VA_ARGS__})/sizeof(typeof(ARG))-1];})



#define __ARGINIT__(x,...) typeof(ret) x;typeof(ret) __VA_ARGS__;typeof(ret)* px = NULL; typeof(ret)* py = NULL; px = &x __VA_OPT__(;py = &) __VA_ARGS__;

#define lambda(FORM,EXP,ARG,...) ({typeof(ARG) ret = (typeof(ARG))0;typeof(ARG) __ARRAY[] = {ARG,##__VA_ARGS__}; \
                    __ARGINIT__ FORM\
                    if (strstr(#FORM,",")){\
                    for (int i = sizeof((typeof(ARG)[]){ARG,__VA_ARGS__})/sizeof(typeof(ARG)) - 1; i > 0 ; --i){ \
                        *px = __ARRAY[i-1];*py = __ARRAY[i];\
                        __ARRAY[i-1] = EXP;}}\
                    else{\
                    for (int i = 0; i < sizeof((typeof(ARG)[]){ARG,__VA_ARGS__})/sizeof(typeof(ARG)) ; ++i){ \
                        *px = __ARRAY[i];\
                        __ARRAY[i] = EXP;}\                        
                    }\
                    &__ARRAY[0];})

#define range(STOP,...) ({int i = 0;for (__VA_OPT__(i = ) __VA_ARGS__;i<STOP;i)}) 


//LIST_SIG_DEF(MPOOL_SIG_DEF(_list_,OBJECT_SIG_DEF(_list_)))
//SLT_DEF(_list_,_object_,_mpool_,_list_)



//
//_object_SLT_DEF(_object_)
//_mpool_SLT_DEF(_mpool_,_object_SLT_DEF(_mpool_))


#define SLT_DEF(CLASS,_CLASS,...) _CLASS##SLT_DEF(CLASS,##__VA_ARGS__)
#define _object_SLT_DEF(CLASS,_CLASS,...)

// #define __ARGLEFT__(x,y) x 
// #define __ARGRIGHT__(x,y) y 

// #define ADD(NUM,_ARG,__ARG,...) ({\
//     int add(int x,int y,...){\
//         return add(x+y,##__VA_ARGS__);\
//     }\
//     add(_ARG,__ARG,##__VA_ARGS__);\
// })

// #define VARG(x,y) x
#define ARG(X) exp X
#define exp(x,y) y
/*
{
    "nodes_cfg": [
    {
        "channelId": "",
        "connect_port": "REMOTE_CH_01",
        "depth": 0,
        "product_key": "1559699700",
        "sn": "21881EFF0051_Fuel",
        "tcp_ip_addr": "127.0.0.1",
        "tcp_port": "5000",
        "template_id": "1559699700",
        "term_addr": "01"
    },
    {
        "channelId": "",
        "connect_port": "REMOTE_CH_01",
        "depth": 0,
        "product_key": "1559699700",
        "sn": "218812000008_test2",
        "tcp_ip_addr": "192.168.12.7",
        "tcp_port": "5001",
        "template_id": "1559699700",
        "term_addr": "02"
    },
    {
        "channelId": "",
        "connect_port": "REMOTE_CH_01",
        "depth": 0,
        "product_key": "1559699700",
        "sn": "218812000008_test3",
        "tcp_ip_addr": "192.168.12.7",
        "tcp_port": "5002",
        "template_id": "1559699700",
        "term_addr": "01"
    }],
"version": "4TQsUCjw5m8r"
}
*/

struct node_cfg {
    char* channelId;
    char* connect_port;
    char* depth;
    char* product_key;
    char* sn;
    char* tcp_ip_addr;
    char* tcp_port;
    char* template_id;
    char* term_addr;
};

struct config{
    struct node_cfg node_cfg;
    char* version;
}config;


//this->root->value.object->next->key
//config("virsion");
//config("node_cfg")("depth");

//get(r->o->s,...)
void* value(char* format,...){
    va_list ap;
    va_start(ap,format);
}


int main() {
    //fuc = (int(*)(int,int))({int x,int y;return x+y;});
    //printf("%d\n",fib(fib(fib(fib(1,2),3),4),5));
    int x,y;
    // x,y = 1;
    int a = 3, b = 1, c = 3,k=-5; 
    // // x,y = x+y;
    // printf("%d\n",y);
    // int *px = &x;
    // int *py = &y;
    // *px = 2;
    // *py = 3;
    // x,y = x+y;
    // printf("%d\n",y);
    // a,b = 3, 
    // printf("%d\b",b);
    printf("%d\n",*lambda((x,y),x+y,a,b,c,k));
    //printf("%d\n",
        *lambda((x),printf("%d\n",x ),a,b,c,k);
        //);
    //int z = NIM(a+b);
    //printf("The sum of %d, %d and %d is %d\n", a, b, c, SUM(num(6), b, c));
    //printf("The sum of %d, %d, %d and %d is %d\n", a, b, c,k, SUM(a, b, c, k));
    // int *ret = lambda(2*,+,+3,a,b,c,c);
    // printf("The sum of %d, %d and %d is %d\n", a, b, c, *ret);
    // float d = 1.1, e = 2.2, f = 3.4;
    //printf("The sum of %f, %f and %f is %f\n", d, e, f, SUM(d, e, f));
    return 0;
}


