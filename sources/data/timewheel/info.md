## [定时器方案之红黑树与最小堆、时间轮详解](https://mp.weixin.qq.com/s/9BocgF8RKXcjGhe22LNxyw)

## 前言

  定时器在后端服务器开发过程中，是非常重要的一个组件，因为不可避免的会有许多延时任务需要被处理。那么到底该如何来组织大量的定时任务呢？

本专栏知识点是通过**零声教育**的线上课学习，进行梳理总结写下文章，对c/c++linux课程感兴趣的读者，可以点击文末**【阅读原文】**详细查看课程的服务。

## 定时器概述

## 定时器的使用场景

- 心跳检测
- 技能冷却
- 武器冷却
- 倒计时
- CSDN文章的定时发布
- 其它需要使用定时机制的功能

### 定时器与其他组件的关系

**第一种**：定时器通常是与网络组件一起工作，网络事件和时间事件在一个线程当中配合使⽤；例如nginx、redis，我们将epoll_wait的第四个参数timeout设置为最近要触发的定时器的时间差，这样就可以兼顾对网络事件的处理，又可以兼顾对时间事件的处理。

```c
while (!quit) {
	int now = get_now_time();// 单位：ms
	//找出最近要触发的定时器时间
	int timeout = get_nearest_timer() - now;
	if (timeout < 0) timeout = 0;
	int nevent = epoll_wait(epfd, ev, nev, timeout);
	for (int i=0; i<nevent; i++) {
	    //... ⽹络事件处理
	}
	update_timer(); // 时间事件处理
}
```

但是epoll_wait毕竟涉及到内核态与用户态的切换，以及网络事件处理的时间开销，所以定时事件就会一段时间的延时了。换句话说，受网络事件处理和系统调用的影响，定时器误差有点大。

那么这个定时器误差大该如何解决？可以使用定时信号进行解决，在nginx中就是利用定时信号来打断epoll_wait来解决定时器误差大这个问题，其中用红黑树来组织定时器（ngx_event_process_init函数中设置了时间信号，每隔固定时间触发，时间信号的处理函数只是设置ngx_event_timer_alarm = 1，但他会中断ngx_process_events中epoll_wait的处理，epoll_wait返回后，调用ngx_time_update更新时间，接着返回到函数ngx_process_events_and_timers中处理，ngx_process_events_and_timers中，会调用ngx_event_expire_timers，查询超时的事件并处理）。

**第二种**，网络事件和时间事件在不同线程当中处理；例如skynet；单独开一个线程用来处理定时任务

```c
// 第⼆种 在其他线程添加定时任务
void* thread_timer(void * thread_param) {
    init_timer();
    while (!quit) {
        update_timer(); // 更新检测定时器，并把定时事件发送到消息队列中
        sleep(t); // 这⾥的 t 要⼩于 时间精度
    }
    clear_timer();
    return NULL;
}
pthread_create(&pid, NULL, thread_timer, &thread_param);
```

## 大量定时任务怎么处理

  如果有大量的定时任务，我们首先要想到用哪一个数据结构去组织这些大量的定时任务。定时器的本质是越近要触发的任务，其优先级越高。也就是说，需要根据时间这个key来排序。那么有序的数据结构有哪些呢？红黑树，最小堆，跳表，时间轮。

- 红黑树(单线程)：nginx
- 跳表(单线程)：redis
- 最小堆(单线程)：libevent，go(最小四叉堆)，libev(最小四叉堆)；大部分都是用最小堆来实现定时器
- 时间轮(多线程)：netty，kafka，skynet，crontab

## 定时器设计

## 接口设计

```c
// 创建定时器
void init_timer();
// 添加定时任务
Node* add_timer(int expire, callback cb);
// 取消定时任务
bool cancel_timer(Node* node);
// 找到最近要发⽣的定时任务
Node* find_nearest_timer();
// 执行到期任务
void expire_timer();
// 清除定时器
void clear_timer();
```

从接口上我们可以看出定时器数据结构的基本要求：

- 能够快速插入删除结点
- 能够快速找到最小的结点

## 数据结构的抉择

  本文对红黑树与最小堆以及跳表不做详细介绍，主要介绍时间轮

红黑树：插入 O(logn) 删除 O(logn) 快速找到最小的结点 O(logn)

跳表：插入 O(logn) 删除 O(logn)快速找到最小的结点 O(1)

最小堆：插入 O(logn) 删除 O(logn)快速找到最小的结点 O(1)

时间轮（哈希表+链表）：插入 O(logn) 删除 O(logn)快速找到最小的结点 O(1)（存在踏空问题，后续介绍）

在上面也写过，红黑树和最小堆适用于单线程，其核心原因在于如果在多线程环境下，锁红黑树或者最小堆，那就要锁整个，粒度太大了。而时间轮的时间复杂度可以看到点O(1)，粒度较小，所以时间轮更适合多线程环境。

## 时间轮

## 时间轮的概念

![图片](https://mmbiz.qpic.cn/mmbiz_png/hvfVgmqNZrqgErA7lDXw1kzV6GIv2AuwBVb303IfliasiaX3Rg4vE4chLbqzBMh7icicFBYftAOcyCaHiaaCKe3pViaA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



编辑



添加图片注释，不超过 140 字（可选）



 从时钟表盘出发，如何从数据结构来描述秒表的运转？对于时钟来说，它的时间精度是1秒。

![图片](https://mmbiz.qpic.cn/mmbiz_png/hvfVgmqNZrqgErA7lDXw1kzV6GIv2Auwzd81eXgsCThIEibxFmqAYmQjFUZhJISojShAam6FWG7b9z9oWbAEEXg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



编辑切换为居中



添加图片注释，不超过 140 字（可选）



时间轮参考时钟进行理解，秒针tick走一圈，则分针走一格，分针走一圈则时针走一格。随着时间tick的踏步，任务不断的从上层流到下一层，最终流到秒针轮上，当秒针走到对应的格子上时，执行链表内的所有任务。

如图所示，秒针和分钟对应60个格子，秒针每走一步，则执行其对应格子指向链表内的时间任务。比如现在tick=1，要添加一个3s后的任务，则在第4格链表中添加一个任务即可，如果要在60秒后执行一个任务，由于60大于了秒针的范围，则要把该任务放到分钟上。可以看到秒针的时间精度一格是1秒，而分钟的一格时间精度是60秒，时针的一格时间精度是60*60秒。

正因为如此，当分钟指向第一个格子上时，会把其对应的链表任务重新映射到下一层，即秒针。当时针走到11时，会把对应任务重新映射到分针上面。

由此可见，秒针轮保存着即将要执行的任务，而别的轮的时间跨度则越来越大，随着时间的流逝，任务会慢慢的从上层流到秒针轮中进行执行。

注意到上面写的重新映射了吗，这意味着时间轮无法删除任务，那么这个问题该如何解？我们可以添加一个删除标记，在函数回调中根据这个标记判断是否需要处理。

```c
truct timer_node {
	struct timer_node *next;
	uint32_t expire;
    handler_pt callback;
    uint8_t cancel;//是否删除
};
```

## 设计单层级时间轮

  场景：客户端每 5 秒钟发送心跳包；服务端若 10 秒内没收到⼼跳数据，则清除连接；

 普通做法：我们假设使用 map<int, conn*> 来存储所有连接数；每秒检测 map 结构，那么每秒需要遍历所有的连接，如果这个map结构包含一万条连接，那么我们做了很多有效检测；考虑极端情况，刚添加进来的连接，下一秒就需要去检测，实际上只需要10秒后检测就够了。

时间轮做法：只需检测快过期的连接， 采用hash数组+链表形式，相同过期时间的放入一个数组，因此，每次只需检测最近过期的数组即可，不需要遍历所有。

时间轮需要考虑两个因素，1. 时间轮的大小，2. 时间精度。

\1. 时间轮的大小

  因为10秒一检测，所以时间轮的大小要大于10。我们一般将时间轮的大小，也就是数组长度，设置为 2n

因为我们总是要进行取余操作,m%n在计算机内部等于 m−n∗floor(m/n) 乘法除法运算效率太低，我们可以通过位运算来优化。

![图片](https://mmbiz.qpic.cn/mmbiz_png/hvfVgmqNZrqgErA7lDXw1kzV6GIv2AuwyUqR4icd7CXVsrhbTGdEhWMBmOYp0ohvELLbWBxRVEVBUibO4qZA3p8w/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)







添加图片注释，不超过 140 字（可选）



### 2. 时间精度

  时间精度就要看业务需求了，目前的需求是以秒为单位，那么时间精度就设置为秒即可

### 空推进问题

![图片](https://mmbiz.qpic.cn/mmbiz_png/hvfVgmqNZrqgErA7lDXw1kzV6GIv2AuwmEqc2M57VJHTbHgUdfeqaKbOfno7ibuToyhuoAVHp4YO26JlgUxDUfg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



编辑



添加图片注释，不超过 140 字（可选）



对于单层级的时间轮来说，如果大小设置太大了，就会出现踏空的现象，空推进。这里时间轮设置了1024大小，但是定时任务只有两个，从5到1022都是没有任务的，那么这就是空推进的情况。这是分布式定时器必须要解决的问题，分布式定时器一般都是用单层级时间轮。

那么怎么解决这个问题呢？第一种做法就是使用辅助数据结构最小堆+单层级时间轮。用最小堆告诉tick下一次检测是第几个格子，直接跳跃，而不是一格一格的走。时间精度设置不当也会造成空推进。第二种做法就是多层级时间轮。

## 多层级时间轮

定时任务时间跨度特别大，有几秒的任务，几个小时的任务，几天的定时任务。那么对于单层级时间轮来说，无论它怎么设置都解决不了这个问题，肯定会出现空推进的问题。

那么我们设计把最近要触发的定时任务放到第一层，几分钟的放到第二层，几个小时的放到第三层…这就是多层级的意思。

![图片](https://mmbiz.qpic.cn/mmbiz_png/hvfVgmqNZrqgErA7lDXw1kzV6GIv2Auw3jo8YebJfGXABdjYRmlKSrDYHxhhKWCicdshXJO4ZaJbXwhkIbFlemw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



编辑切换为居中



添加图片注释，不超过 140 字（可选）



我们已时钟这个时间轮来举例

1. 3秒后：假设现在有一个3秒后的任务，3秒<60，所以放在第一层上，idx=(tick+3)%60,将时间任务添加到数组idx位置上。
2. 59秒后：设置现在有59秒后的任务，59<60,idx=(tick+59)%60；
3. 60秒后：现在有一个60秒后的任务，60>=60,所以该任务要放在第二层分针的轮上。idx=((tick+60)/60 % 60)，设tick=1，那么idx=1
4. 61秒后：61>=60,idx=((tick+60)/60 % 60)，设tick=1，那么idx还是等于1

 可以看到分钟的一格是从60s~119s，这也就意味着，第一层是精确的时间，下面的都是稀疏的时间。

![图片](https://mmbiz.qpic.cn/mmbiz_png/hvfVgmqNZrqgErA7lDXw1kzV6GIv2AuwnjyibvH9oIMfcf9ToDwyHricN45822Wub259IIzEDa3WxN7y9oBZlIZA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



编辑



添加图片注释，不超过 140 字（可选）



分针或时针移动一格：将对应索引中的定时任务重新映射到上一层。idx=(记录的时间-tick)%60

  为什么多层级可以解决空推进问题？因为我们只关注秒针那一层，把时间跨度比较大的都放在别的层。

 对于我们的tick来说，我们可以通过tick%60来算出在秒针走到了哪，tick/60%60算出分针，tick只需要一直++即可。我们可以看到除了秒针曾的0号格子有数据，下面的都没有数据，但实际上，最后一层的0号各自也可能有数据。为什么呢？

 什么情况下，最后一层的0号索引有数据，我们可以看到tick的实际范围是0~43199。tick=2, tick+43199,那么现在必然是放到第三层，但是tick+43199=43201, 43201>43199则43201%43200=1，现在1/60/60 %12 =0,此时放到0号索引。简而言之，因为我们限定了tick的范围，所以造成了最后一层0号索引有值。

## Skynet定时器实现方案

  假设时间精度为 10ms ；在第 1 层级每 10ms 移动1格；每移动⼀格执⾏该格⼦当中所有的定时任务；当第 1 层指针从 255 格开始移动，此时层级 2 移动⼀格；层级 2 移动⼀格的⾏为定义为，将该格当中的定时任务重新映射到层级 1 当中；同理，层级 2 当中从 63 格开始移动，层级 3 格⼦中的定时任务重新映射到层级 2 ; 以此类推层级 4 往层级 3 映射，层级 5 往层级 4 映射；

![图片](https://mmbiz.qpic.cn/mmbiz_png/hvfVgmqNZrqgErA7lDXw1kzV6GIv2Auwwkp7UmePVn8sCERlEcVZvUgZWD6ctejQIjYe5sGPXVzfAQaTicC5FYQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



编辑切换为居中



添加图片注释，不超过 140 字（可选）



### skynet中定时器数据结构

  采用时间轮方式，hash表+链表实现。其中time为32位无符号整数， 记录时间片对应数组near[256] ，表示即将到来的定时任务， t[4][64]，表示较为遥远的定时任务。

```c
struct timer_node {  //时间节点
 struct timer_node *next;
    uint32_t expire; //到期滴答数
};
struct link_list {  // 链表
  struct timer_node head;
  struct timer_node *tail;
};
struct timer {
 struct link_list near[256];  // 即将到来的定时器
    struct link_list t[4][64]; // 相对较遥远的定时器
    struct spinlock lock;
    uint32_t time;  // 记录当前滴答数
    uint64_t starttime;
    uint64_t current;
    uint64_t current_point;
};
```

![图片](https://mmbiz.qpic.cn/mmbiz_png/hvfVgmqNZrqgErA7lDXw1kzV6GIv2AuwYEZcWAWZWVZn4xjibqMzM8PgVn0jhOLpDQWO4iacn2BOh60dLHExcX6A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



编辑切换为居中



添加图片注释，不超过 140 字（可选）



接口介绍

### 定时器初始化

```c
// skynet_start.c
// skynet 启动入口
void
skynet_start(struct skynet_config * config) {
    ...
    skynet_timer_init();
    ...
}
// skynet_timer.c
void
skynet_timer_init(void) {
    // 创建全局timer结构 TI
    TI  = timer_create_timer();
    uint32_t current = 0;
    systime(&TI->starttime, &current);
    TI->current = current;
    TI->current_point = gettime();
}
```

### 添加定时器

  通过skynet_server.c中的cmd_timeout调用skynet_timeout添加新的定时器

```c
// TI为全局的定时器指针
static struct timer * TI = NULL; 
int skynet_timeout(uint32_t handle, int time, int session) {
    ...
    struct timer_event event;
    event.handle = handle;  // callback
    eveng.session = session;
    // 添加新的定时器节点
    timer_add(TI, &event, sizeof(event), time);
    return session;
}
// 添加新的定时器节点
static void timer_add(struct timer *T, void 8arg, size_t sz, int time) {
    // 给timer_node指针分配空间，还需要分配timer_node + timer_event大小的空间，
    // 之后通过node + 1可获得timer_event数据
    struct timer_node *node = (struct timer_node *)skynet_malloc(sizeof(*node)+sz);
    memcpy(node+1,arg,sz);
    SPIN_LOCK(T);
    node->expire=time+T->time;
    add_node(T, node);
    SPIN_UNLOCK(T);
}
 
// 添加到定时器链表里，如果定时器的到期滴答数跟当前比较近(<2^8)，表示即将触发定时器添加到T->near数组里
// 否则根据差值大小添加到对应的T->T[i]中
static void add_node(struct timer *T, struct timer_node *node) {
    ...
}
```

### 驱动方式

  skynet启动时，会创建一个线程专门跑定时器，每帧(0.0025s)调用skynet_updatetime()

```c
// skynet_start.c
static void * 
thread_timer(void *p) {
    struct monitor * m = p;
    skynet_initthread(THREAD_TIMER);
    for (;;) {
        skynet_updatetime();  // 调用timer_update
        skynet_socket_updatetime();
        CHECK_ABORT
        wakeup(m,m->count-1);
        usleep(2500);  // 2500微秒 = 0.0025s
        if (SIG) {
            signal_hup();
            SIG = 0;
        }
    }
    ...
}
```

每个定时器设置一个到期滴答数，与当前系统的滴答数(启动时为0，1滴答1滴答往后跳，1滴答==0.01s ) 比较得到差值interval;

 如果interval比较小(0 <= interval <= 28 −1) 表示定时器即将到来，保存在第一部分前= 28

个定时器链表中；否则找到属于第二部分对用的层级中。

```c
// skynet_timer.c
void 
skynet_updatetime(void) {
    ...
    uint32_t diff = (uint32_t)(cp - TI->current_point); 
    TI->current_point = cp;
    TI->current += diff;
    // diff单位为0.01s
    for (i = 0; i < diff; i++){
        timer_update(TI);        
    }
}
static void
timer_update(struct timer *T) {
    SPIN_LOCK(T);
    timer_execute(T); // 检查T->near是否位空，有就处理到期定时器
    timer_shift(T);  // 时间片time++，移动高24位的链表
    timer_execute(T);
    SPIN_UNLOCK(T);
}
// 每帧从T->near中触发到期得定时器
static inline void
timer_execute(struct timer *T) {
  ...
}
// 遍历处理定时器链表中所有的定时器
static inline void
dispatch_list(struct timer_node *current) {
    ...
}
// 将高24位对应的4个6位的数组中的各个元素的链表往低位移
static void
timer_shift(struct timer *T) {
    ...
}
// 将level层的idx位置的定时器链表从当前位置删除，并重新add_node
static void move_list(struct timer *T, int level, int idx) {
 
}
```

添加定时任务

首先检查节点的expire与time的高24位是否相等，相等则将该节点添加到expire低8位值对应数组near的元素的链表中，不相等则进行下一步

检查expire与time的高18位是否相等，相等则将该节点添加到expire低第9位到第14位对应的6位二进制值对应数组t[0]的元素的链表中，否则进行下一步

检查expire与time的高12位是否相等，相等则将该节点添加到expire低第15位到第20位对应的6位二进制值对应数组t[1]的元素的链表中，如果不相等则进行下一步

检查expire与time的高6位是否相等，相等则将该节点添加到expire低第21位到第26位对应的6位二进制值对应数组t[2]的元素的链表中，如果不相等则进行下一步

将该节点添加到expire低第27位到第32位对应的6位二进制值对应数组t[3]的元素的链表中

```c
// 添加到定时器链表里，如果定时器的到期滴答数跟当前比较近(<2^8)，表示即将触发定时器添加到T->near数组里
// 否则根据差值大小添加到对应的T->T[i]中
void add_node(timer_t *T, timer_node_t *node) {
    uint32_t time = node->expire;
    uint32_t current_time = T->time;
    uint32_t msec = time - current_time;
    if (msec < TIME_NEAR) { //[0, 0x100)
        // time % 256
        link(&T->near[time & TIME_NEAR_MASK], node);
    }
    else if (msec < (1 << (TIME_NEAR_SHIFT + TIME_LEVEL_SHIFT))) {//[0x100, 0x4000)
        // floor(time/2^8) % 64
        link(&T->t[0][((time >> TIME_NEAR_SHIFT) & TIME_LEVEL_MASK)], node);
    }
    else if (msec < (1 << (TIME_NEAR_SHIFT + 2 * TIME_LEVEL_SHIFT))) {//[0x4000, 0x100000)
        // floor(time/2^14) % 64
        link(&T->t[1][((time >> (TIME_NEAR_SHIFT + TIME_LEVEL_SHIFT)) &
                       TIME_LEVEL_MASK)], node);
    }
    else if (msec < (1 << (TIME_NEAR_SHIFT + 3 * TIME_LEVEL_SHIFT))) {//[0x100000, 0x4000000)
        // floor(time/2^20) % 64
        link(&T->t[2][((time >> (TIME_NEAR_SHIFT + 2 * TIME_LEVEL_SHIFT)) &
                       TIME_LEVEL_MASK)], node);
    }
    else {//[0x4000000, 0xffffffff]
        // floor(time/2^26) % 64
        link(&T->t[3][((time >> (TIME_NEAR_SHIFT + 3 * TIME_LEVEL_SHIFT)) &
                       TIME_LEVEL_MASK)], node);
    }
}
```

重新映射

```c
void timer_shift(timer_t *T) {
    int mask = TIME_NEAR;
    uint32_t ct = ++T->time; // 第⼀层级指针移动 ++ ⼀次代表10ms
    if (ct == 0) {
        move_list(T, 3, 0);
    }
    else {
        // floor(ct / 256)
        uint32_t time = ct >> TIME_NEAR_SHIFT;
        int i = 0;
        // ct % 256 == 0 说明是否移动到了 不同层级的 最后⼀格
        while ((ct & (mask - 1)) == 0) {
            int idx = time & TIME_LEVEL_MASK;
            if (idx != 0) {
                move_list(T, i, idx); // 这⾥发⽣重新映射，将i+1层级idx格⼦中的
                定时任务重新映射到i层级中
            }
            mask <<= TIME_LEVEL_SHIFT;
            time >>= TIME_LEVEL_SHIFT;
            ++i;
        }
    }
}
```