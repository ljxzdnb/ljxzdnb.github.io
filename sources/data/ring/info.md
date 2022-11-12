## V:[裸机中环形队列与RTOS中消息队列的区别](https://mp.weixin.qq.com/s/i9NvzQniZOX_mRechLGxZg)

“环形队列”和“消息队列”在嵌入式领域有应用非常广泛，相信有经验的嵌入式软件工程师对它们都不陌生。

但经常看到一些初学者问一些相关的问题，今天就来分享一下关于“环形队列”和“消息队列”的内容。

**环形队列**

环形队列是在实际编程极为有用的数据结构，**它是一个首尾相连的FIFO的数据结构，采用数组的线性空间，数据组织简单，能很快知道队列是否满为空，能以很快速度的来存取数据。**

环形队列通常用于通信领域，比如UART、USB、CAN、网络等。

**1.环形队列实现原理**

内存上没有环形的结构，因此环形队列实上是数组的线性空间来实现。当数据到了尾部它将转回到0位置来处理。

因此环列队列的逻辑：**将数组元素q[0]与q[MAXN-1]连接，形成一个存放队列的环形空间。**

为了方便读写，还要用数组下标来指明队列的读写位置。head/tail.其中head指向可以读的位置，tail指向可以写的位置。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_jpg/ju1DzqX8iaOkqEjIHHV1kx82CfbfoAHnrEkAicrRxP7F41H93mjaxTcicZzSEnECDxLLwNNEictG17BLcz2GgRr1JQ/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

环形队列的关键是判断队列为空，还是为满。当tail追上head时，队列为满时；当head追上tail时，队列为空。但如何知道谁追上谁，还需要一些辅助的手段来判断.

如何判断环形队列为空，为满有两种判断方法：

**a.附加一个标志位tag**

- 当head赶上tail，队列空，则令tag=0
- 当tail赶上head，队列满，则令tag=1

**b.限制tail赶上head，即队尾结点与队首结点之间至少留有一个元素的空间。**

- 队列空：  head==tail
- 队列满：  (tail+1)% MAXN ==head

**2.附加标志实现原理**

**a.采用第一个环形队列有如下结构:**

```c
typedef struct ringq{
   int head; /* 头部，出队列方向*/
   int tail; /* 尾部，入队列方向*/ 
   int tag ;
   int size ; /* 队列总尺寸 */
   int space[RINGQ_MAX]; /* 队列空间 */
}RINGQ;
```

初始化状态：

```c
q->head = q->tail = q->tag = 0;
```

队列为空：

```c
( q->head == q->tail) && (q->tag == 0)
```

队列为满 ：

```c
((q->head == q->tail) && (q->tag == 1))
```

入队操作，如队列不满，则写入：

```c
q->tail =  (q->tail + 1) % q->size ;
```

出队操作，如果队列不空，则从head处读出。

下一个可读的位置在：

```c
q->head =  (q->head + 1) % q->size
```

**头文件ringq.h:**

```c
#ifndef __RINGQ_H__
#define __RINGQ_H__

#ifdef __cplusplus
extern "C" {
#endif 

#define QUEUE_MAX 20

typedef struct ringq{
   int head; /* 头部，出队列方向*/
   int tail; /* 尾部，入队列方向*/ 
   int tag ; /* 为空还是为满的标志位*/
    int size ; /* 队列总尺寸 */
   int space[QUEUE_MAX]; /* 队列空间 */
}RINGQ;

/* 
  第一种设计方法:
     当head == tail 时，tag = 0 为空，等于 = 1 为满。
*/

extern int ringq_init(RINGQ * p_queue);
extern int ringq_free(RINGQ * p_queue);

/* 加入数据到队列 */
extern int ringq_push(RINGQ * p_queue,int data);

/* 从队列取数据 */
extern int ringq_poll(RINGQ * p_queue,int *p_data);


#define ringq_is_empty(q) ( (q->head == q->tail) && (q->tag == 0))

#define ringq_is_full(q) ( (q->head == q->tail) && (q->tag == 1))

#define print_ringq(q) printf("ring head %d,tail %d,tag %d\n", q->head,q->tail,q->tag);
#ifdef __cplusplus
}
#endif 

#endif /* __RINGQ_H__ */
```

**源代码 ringq.c：**

```c
#include <stdio.h>
#include "ringq.h"

int ringq_init(RINGQ * p_queue)
{
   p_queue->size = QUEUE_MAX ;
   p_queue->head = 0;
   p_queue->tail = 0;
   p_queue->tag = 0;

   return 0;
}

int ringq_free(RINGQ * p_queue)
{
  return 0;
}


int ringq_push(RINGQ * p_queue,int data)
{
  print_ringq(p_queue);
  if(ringq_is_full(p_queue))
   {
     printf("ringq is full\n");
     return -1;
   }
   p_queue->space[p_queue->tail] = data;
   p_queue->tail = (p_queue->tail + 1) % p_queue->size ;

   /* 这个时候一定队列满了*/
   if(p_queue->tail == p_queue->head)
    {
       p_queue->tag = 1;
    }
    return p_queue->tag ;  
}

int ringq_poll(RINGQ * p_queue,int * p_data)
{
   print_ringq(p_queue);
  if(ringq_is_empty(p_queue))
   {
		printf("ringq is empty\n");
   		return -1;
   }

   *p_data = p_queue->space[p_queue->head];
   p_queue->head = (p_queue->head + 1) % p_queue->size ;

    /* 这个时候一定队列空了*/
   if(p_queue->tail == p_queue->head)
    {
		p_queue->tag = 0;
    }    
    return p_queue->tag ;
}
```

看到源代码，相信大家就明白其中原理了。其实还有不采用tag，或者其他一些标志的方法，这里就不进一步展开讲述了，感兴趣的读者可以自行研究一下。

**消息队列**

在RTOS中基本都有消息队列这个组件，也是使用最常见的组件之一。

**1.消息队列的基本概念**

**消息队列是一种常用于任务间通信的数据结构，队列可以在任务与任务间、中断和任务间传递信息，实现了任务接收来自其他任务或中断的不固定长度的消息。**

通过消息队列服务，任务或中断服务程序可以将一条或多条消息放入消息队列中。同样，一个或多个任务可以从消息队列中获得消息。

使用消息队列数据结构可以实现任务异步通信工作。

**2.消息队列的特性**

[**RTOS**](https://mp.weixin.qq.com/s?__biz=MzUxNjgxMDE4OQ==&mid=2247485144&idx=6&sn=5387907f2cafa42c08c7792235ed6ca7&scene=21#wechat_redirect)消息队列，常见特性：

- 消息支持先进先出方式排队，支持异步读写工作方式。
- 读写队列均支持超时机制。
- 消息支持后进先出方式排队，往队首发送消息（LIFO）。
- 可以允许不同长度（不超过队列节点最大值）的任意类型消息。
- 一个任务能够从任意一个消息队列接收和发送消息。
- 多个任务能够从同一个消息队列接收和发送消息。
- 当队列使用结束后，可以通过删除队列函数进行删除。

**3.消息队列的原理**

这里以 [FreeRTOS](https://mp.weixin.qq.com/s?__biz=MzUxNjgxMDE4OQ==&mid=2247485144&idx=6&sn=5387907f2cafa42c08c7792235ed6ca7&scene=21#wechat_redirect) 为例进行说明。FreeRTOS 的消息队列控制块由多个元素组成，**当消息队列被创建时，系统会为控制块分配对应的内存空间，用于保存消息队列的一些信息如消息的存储位置，头指针 pcHead、尾指针 pcTail、消息大小 uxItemSize 以及队列长度 uxLength 等**。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/ju1DzqX8iaOkqEjIHHV1kx82CfbfoAHnraCKen5TPflbVRB7iaJGIdpxIiazO7lU64D3yL0LzcayOf7RkM3SZnvaw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

比如创建消息队列：

```c
xQueue = xQueueCreate(QUEUE_LEN, QUEUE_SIZE);
```

任务或者中断服务程序都可以给消息队列发送消息，当发送消息时，如果队列未满或者允许覆盖入队，FreeRTOS 会将消息拷贝到消息队列队尾，否则，会根据用户指定的阻塞超时时间进行阻塞，在这段时间中，如果队列一直不允许入队，该任务将保持阻塞状态以等待队列允许入队。当其它任务从其等待的队列中读取入了数据（队列未满），该任务将自动由阻塞态转移为就绪态。当等待的时间超过了指定的阻塞时间，即使队列中还不允许入队，任务也会自动从阻塞态转移为就绪态，此时发送消息的任务或者中断程序会收到一个错误码 errQUEUE_FULL。

发送紧急消息的过程与发送消息几乎一样，唯一的不同是，当发送紧急消息时， 发送的位置是消息队列队头而非队尾，这样，接收者就能够优先接收到紧急消息，从而及时进行消息处理。

当某个任务试图读一个队列时，其可以指定一个阻塞超时时间。在这段时间中，如果队列为空，该任务将保持阻塞状态以等待队列数据有效。当其它任务或中断服务程序往其等待的队列中写入了数据，该任务将自动由阻塞态转移为就绪态。当等待的时间超过了指定的阻塞时间，即使队列中尚无有效数据，任务也会自动从阻塞态转移为就绪态。

**当消息队列不再被使用时，应该删除它以释放系统资源，一旦操作完成， 消息队列将被永久性的删除。**

消息队列的运作过程具体见下图：

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/ju1DzqX8iaOkqEjIHHV1kx82CfbfoAHnribbzVl9BMoENLCxflEVgUFkhqBVicRziaoldYcXPiaABPvMCdcFBdpN9RQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**4.消息队列的阻塞机制**

**出队阻塞：**当且仅当消息队列有数据的时候，任务才能读取到数据，可以指定等待数据的阻塞时间。

**入队阻塞：**当且仅当队列允许入队的时候，发送者才能成功发送消息；队列中无可用消息空间时，说明消息队列已满，此时，系统会根据用户指定的阻塞超时时间将任务阻塞。

假如有多个任务阻塞在一个消息队列中，那么这些阻塞的任务将按照任务优先级进行排序，优先级高的任务将优先获得队列的访问权。

**“环形队列”和“消息队列”的异同**

通过以上分析，你会发现“环形队列”和“消息队列”之间有很多共同点：

1.他们都是一种数据结构，结构中都包含头、尾、标志等信息；

**2.它们都是分配一块连续的内存空间，且都可以分配多个队列。**

3.应用场景类似，有大量吞吐数据的情况下，比如通信领域。

...

当然，他们也有一些不同点：

1.“环形队列”可以独立使用，也可以结合操作系统使用。而消息队列依赖[**RTOS**](https://mp.weixin.qq.com/s?__biz=MzI4MDI4MDE5Ng==&mid=2247502058&idx=2&sn=adf2c30eef005c29c51949bb84785e2e&scene=21#wechat_redirect)（有些RTOS的参数信息）。

2.“环形队列”占用资源更小，更适合于资源较小的系统中。

3.“消息队列”结合RTOS应用更加灵活，比如延时、中断传输数据等。
