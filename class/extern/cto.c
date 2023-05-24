#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
typedef int     BOOL;
#define TRUE    1
#define FALSE   0

// 用来存储主程和协程的上下文的数据结构
typedef struct _Context_ {
    jmp_buf mainBuf;
    jmp_buf coBuf;
} Context;

// 上下文全局变量
Context gCtx;

// 恢复
#define resume() \
    if (0 == setjmp(gCtx.mainBuf)) \
    { \
        longjmp(gCtx.coBuf, 1); \
    }

// 挂起
#define yield() \
    if (0 == setjmp(gCtx.coBuf)) \
    { \
        longjmp(gCtx.mainBuf, 1); \
    }

// 在协程中执行的函数
void fuc1(void *arg)
{
    while (TRUE)  // 死循环
    {
        printf("\n*** coroutine: working \n");
        // 模拟耗时操作
        for (int i = 0; i < 10; ++i)
        {
            fprintf(stderr, ".");
            usleep(1000 * 200);
        }
        printf("\n*** coroutine: suspend \n");
        
        // 让出 CPU
        yield();
    }
}
void fuc2(void *arg)
{
    while (TRUE) // 死循环
    {
        printf("\n=== main: working \n");

        // 模拟耗时操作
        for (int i = 0; i < 10; ++i)
        {
            fprintf(stderr, ".");
            usleep(1000 * 200);
        }

        printf("\n=== main: suspend \n");
        
        // 放弃 CPU，让协程执行
        resume();
    }
}
// 启动一个协程
// 参数1：func 在协程中执行的函数
// 参数2：func 需要的参数
typedef void (*pf)(void *);
BOOL start_coroutine(pf func, void *arg)
{
    // 保存主程的跳转点
    if (0 == setjmp(gCtx.mainBuf))
    {
        func(arg); // 调用函数
        return TRUE;
    }

    return FALSE;
}

int main()
{
    // 启动一个协程
    start_coroutine(fuc1, NULL);
    start_coroutine(fuc2, NULL);
    
    return 0;
}


