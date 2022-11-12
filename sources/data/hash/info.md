## I:[C语言-哈希查找(HASH)-详解（完整代码）](https://blog.csdn.net/qz652219228/article/details/120105248)

### 原理：

> 用一个指针数组，来存储 每个链表的头节点 的首地址
>
> 如果要从 'NUM' 个数中查找数据
> 先对'NUM'/0.75,求得最大质数'N'        //(质数：只能被1和本身整除的数)
> 然后创建一个有'N'个元素的'指针数组'
> 然后将'NUM'个数分别对'N'取余
> 将每一个数保存在'余数'等于数组元素下标的链表中
> 然后进行查找是直接找对应的数组下标即可

### 实例解释

> 如果要从11个数中查找数据
> 然后11/0.75=14,求得最大质数13
> 然后创建一个有13个元素的'指针数组'
> 然后将'11个数'分别对'13取余'
> 将每一个数保存在'余数'等于'数组元素下标'的链表中        //---需要链表
> 然后进行查找是直接找对应的数组下标即可 

### 存储逻辑图

![img](https://img-blog.csdnimg.cn/2021090420361671.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBA57uG6Zuo6Z2S5bOm,size_20,color_FFFFFF,t_70,g_se,x_16)

### 需要的知识：

> 质数：只能被1和本身整除的数
> 指针数组：本质是个数组，数组里存的是指针
> 链表的操作：
>     创建：
>     遍历：
>     增删改查：
>     释放：free
> malloc：主动申请堆区空间        //(返回值类型)malloc(申请的空间的大小)
> int类型：4个字节
> 指针类型：4个字节(32位系统)，8个字节(64位系统)

### 附加:

> 哈希表查找算法的**时间复杂度**为O(n^1)
>
> [HASH](https://so.csdn.net/so/search?q=HASH&spm=1001.2101.3001.7020)查找**效率高的原因**：
>
> ​    查找某一个数，先求出这个数的余数，然后根据余数直接定位到对应的链表地址，然后在该链表里查找（链表里只有几个数据）--所以快！！

### 完整代码

```c
#include <stdio.h>
#include <stdlib.h>
 
#define N 13		//余数：也是指针数组的元素个数：也是链表的个数
#define ADDR_SIZE 8	//这个定义的是 指针数组的 每个指针的大小，(64位系统)8个字节
 
//hash表 链表的节点
typedef struct node {
    int data;//存数据
    struct node *next;//存指针
}HASH;//类型重命名-->HASH
 
//创建hash表(创建了十三个链表头节点)
HASH **create_hash()
{
    //申请创建一个指针数组，存13个头节点
    //先创建一个指针数组，指针数组可以在栈区申请（int *h[]），但是当前函数结束会被释放 
    //所以在堆区申请空间，指针数组的返回值 是二级指针 所以用HASH **h来接收
    //这块申请了一个 有13个位置的指针数组
    HASH **h = (HASH **)malloc(N * ADDR_SIZE);//(返回值类型)malloc(申请的空间的大小)
    int i = 0;
    //这块要填充这个指针数组
    //创建链表的头结点--先malloc申请出来一个头节点，分别把他们放到指针数组h[0]--h[12] 里
    for (i = 0; i < N; i++)
    {
    	h[i] = (struct node *)malloc(sizeof(struct node));//创建头节点
	h[i]->next = NULL;//初始化头节点 指针域
    }	//注意：再此申请的节点 都被保存到 （指针数组）h 里了
    return h;
}
 
//插入数据
int insert_hash_table(HASH **h, int data)//参数：指针数组，需要插入的数据
{
    
    //然后将'需要插入的数据'对'质数13取余'--确定好数据对应的 指针数组下标
    //找到指定的链表
    int key = data % N; 
	
    //根据指针数组的下标，确定对应的链表的头节点，
    //定义了一个结构体指针变量p 指向 指针数组的第[key]位对应的 链表
    struct node *p = h[key];//也可以直接操作h[key],定义一个指针好理解点
	
    //--头插法--插入数据
    struct node *temp;//定义了一个 结构体指针变量
    temp = (struct node *)malloc(sizeof(struct node));//malloc申请空间
    temp->data = data;//初始化一下
	
    //头插法 插入
    temp->next = p->next;//新定义节点的指针域 指向 头节点的下一个节点
    p->next = temp;//头节点的指针域，指向新定义的节点
	
    return 0;
}
 
//打印排好的hash表--遍历
int show_hash_table(struct node *head)//参数：对应链表的头节点--main函数多次调用
{
    //如果链表后面没有数据，则用---0---表示链表存在但是没有数据
    if (head->next == NULL)
    {
	puts("---0---");
	return -1;
    }
	
    //如果链表后面有数据，遍历链表，打印数据
    while(head->next != NULL)
    {	
	//由于头节点没有数据，所以，先移动指针，然后输出数据
	head = head->next;
	printf("%d  ", head->data);
    }
    putchar(10);//输出个换行符
    return 0;
}
//释放链表节点		
int free_hash_table(struct node *head)
{
    //如果链表后面没有数据，则无需释放
    if (head->next == NULL)
    {
	return 0;
    }  	
	
    //遍历这个链表-头删法释放
    while(head->next != NULL)
    {
	//定义一个结构体指针变量 来指向这个即将被删除的结构体 以便释放
	struct node *temp = head->next;
	head->next = head->next->next;//改变头结点指针域指向，删除节点
	printf("--%d--将被释放\n",temp->data);
	free(temp);//释放
	temp = NULL;//置空（防止被别的函数修改）
    }
    return 0;
}
//查找数据
int search_hash_table(HASH **h, int data)//参数：指针数组，需要查找的数据
{
    int key = data % N; //先把要查找的数据对 质数 取余，得到对应的下标
    struct node *p = h[key]; //根据下标找到对应链表，定义了一个结构体指针变量p，指向该链表
	
    //--循环遍历--对比--
    //循环遍历的结束条件是，p->next 域 为空(NULL)
    while (p->next != NULL)
    {
	if (p->next->data == data)
	{	
	    return 1;//找到返回1
	}
	p = p->next;//移动指针
    }
 
    //没有找到返回0
    return 0;
}
 
//程序的入口：
//假定数组有11个元素--> 11/0.75 ==> 14.67 ==> 最大质数 为 13
int main(int argc, const char *argv[])
{
    int a[11] = {100, 34, 14, 45, 46, 98, 68, 69, 7, 31, 26};
    //直接初始化了11个数值的数组
 
    //创建hash表
    HASH **h = create_hash();//为啥用二级指针：指针数组的返回值是二级指针
 
    //将数据按照格式插入到链表中
    int i = 0;
    int num = 0;
	
    //链表增加--多次调用-插入数组a的每个元素
    for (i = 0; i < 11; i++)//给 a[i] 使的
    {
	insert_hash_table(h, a[i]);	//链表的插入
    }
		
    printf("-------这是hash--------------------\n");	
    //打印hash表--打印每个指针数组元素所存储的链表
    for (i = 0; i < N; i++)
    {
	show_hash_table(h[i]);//链表的遍历
    }
	
    printf("--------hash表结束--------------------\n");
    printf("数组数据如下-->用于测试，无实质意义,遍历HASH表也是<---\n");
    for(i = 0;i < 11;i ++)
    {
	printf("%d  ",a[i]);	
    }
    putchar(10);
	
//  while(1)
//  {
	//查找
	printf("please input need 查找 de number >>");
	scanf("%d",&num);
	//由于输入字母，会造成死循环，所以也可以用char类型定义，或者加个判断（ASCII码）
	//指定数据判断是否存在-----查找
	if(search_hash_table(h, num) == 1)
	{
	    printf("---data %d is exists---\n", num); 
	}
	else
	{
	    printf("---data %d is not exists---\n", num);
	}
//  }
	
    //链表的释放
    for(i = 0;i < 11;i ++)
    {
	free_hash_table(h[i]);
    }
    printf("---链表释放完成---\n");
    free(h);
    printf("---指针数组释放---\n");
 
    return 0;
}
```

### 执行结果

#### 1.查找个不存在的

![img](https://img-blog.csdnimg.cn/20210904210232952.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBA57uG6Zuo6Z2S5bOm,size_17,color_FFFFFF,t_70,g_se,x_16)

### 2.查找个存在的

![img](https://img-blog.csdnimg.cn/20210904210842799.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBA57uG6Zuo6Z2S5bOm,size_15,color_FFFFFF,t_70,g_se,x_16)

### Concurrent HashMap

基本原理介绍
扩容：当 hash 桶超过 4 / 5 中有内容时候，重新开辟两倍大小的桶空间，并且把元素移动到新的空间中
分段加锁：将整个哈希表划分成多个子表，对每个子表进行加锁，保证子表之间访问互不干扰，进行两段式哈希，第一次定位到子表，第二次定位到所在子表的桶中。
update1
实测随机数据下面进行桶排序的速度是 C++ STL unordered_map 的四倍，但是在完全无哈希冲突的数据下面速度会慢不少，推测是因为扩容的逻辑比较耗时。后续可以改进扩容逻辑。

update2
扩容逻辑优化，实测下来随机情况下效率提升10%，完全无哈希冲突时效率提升250%。保持了各种情况下面效率为 C++ STL unordered_map 效率的三倍以上。接下来的任务是研究上锁，做成线程安全的。

update3
对串行 hashmap 加锁， 外层包装一个 分段加锁的 hashmap， 完成了类似 JDK 1.7 之前的 Concurrent HashMap 机制的分段式加锁 并发哈希表。

C++ STL unordered_map 和 con_segmap 效率对比 （执行代码见最下方）



hash_entry.h

```c
#pragma once
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define INIT_SIZE 4

typedef struct entry {
    void* key;
    void* val;
    struct entry* next;
}entry;

entry* create_entry(void* key, void* val, entry* next) {
    entry* ret = malloc(sizeof(entry));
    ret->key = key;
    ret->val = val;
    ret->next = next;
    return ret;
}

entry* create_entries(int n) {
    return calloc(n, sizeof(entry));
}

void lput(entry* head, entry* item, bool (*equals)(void* a, void* b)) {
    while(head->next != NULL) {
        entry* now = head->next;
        if((*equals)(now->key, item->key)) {
            now->val = item->val;
            return;
        }
        head = head->next;
    }
    head->next = item;
    item->next = NULL;
}

void* lget(entry* head, void* key, bool (*equals)(void* a, void* b)) {
    void* ret = NULL;
    while(head->next != NULL) {
        entry* now = head->next;
        if((*equals)(now->key, key)) {
            ret = now->val;
            break;
        }
        head = head->next;
    }
    return ret;
}

void* ldel(entry* head, void* key, bool (*equals)(void* a, void* b)) {
    while(head->next != NULL) {
        entry* now = head->next;
        if((*equals)(now->key, key)) {
            head->next = now->next;
            return now;
        }
        head = head->next;
    }
    return NULL;
}
```

con_hashmap.h

```c
#pragma once
#include "hash_entry.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define INIT_SIZE 4


typedef struct con_hashmap {
    size_t cap;
    size_t load;
    size_t size;
    entry** data;
    size_t hash_offset;
    uint32_t (*hash_func)(void* v);
    bool (*equals)(void* a, void* b);
    pthread_spinlock_t lock;
}con_hashmap;

con_hashmap* create_hashmap(uint32_t (*hash_func)(void* v), bool (*equals)(void* a, void* b)) {
    con_hashmap* ret = malloc(sizeof(con_hashmap));
    ret->cap = (1 << INIT_SIZE);
    ret->load = ret->size = ret->hash_offset = 0;
    ret->data = malloc(sizeof(entry*) * (1 << INIT_SIZE));
    entry* heads = create_entries(1 << INIT_SIZE);
    for(int i = 0; i < (1 << INIT_SIZE); i ++) {
        ret->data[i] = &heads[i];
    }
    ret->hash_func = hash_func;
    ret->equals = equals;
    pthread_spin_init(&ret->lock, PTHREAD_PROCESS_PRIVATE);
    return ret;
}

uint32_t _hv(con_hashmap* s, void* key) {
    return ((*s->hash_func)(key)) >> s->hash_offset;
}

void* _get_bucket(con_hashmap* s, uint32_t hv) {
    uint32_t bucket_id = hv & (s->cap - 1);
    return s->data[bucket_id];
}

void* _get_bucket_raw(entry** data, size_t cap, uint32_t hv) {
    uint32_t bucket_id = hv & (cap - 1);
    return data[bucket_id];
}

void _resize(con_hashmap* s) {
    size_t new_cap = s->cap << 1;
    size_t new_load = 0;
    entry* heads = create_entries(new_cap);
    entry** new_data = malloc(sizeof(entry*) * new_cap);
    for(int i = 0; i < new_cap; i ++) {
        new_data[i] = &heads[i];
    }

    for(int i = 0; i < s->cap; i ++) {
        entry* h = s->data[i];
        while(h->next != NULL) {
            entry* now = h->next;
            h->next = now->next;
            entry* bucket = _get_bucket_raw(new_data, new_cap, _hv(s, now->key));
            if(bucket->next == NULL){
                new_load ++;
            }
            lput(bucket, now, s->equals);
        }
    }

    entry* old_heads = s->data[0];
    entry** old_data = s->data;
    s->data = new_data;
    s->cap = new_cap;
    s->load = new_load;

    free(old_heads);
    free(old_data);
}

void* _get(con_hashmap* s, uint32_t hv, void* key) {
    entry* bucket = _get_bucket(s, hv);
    return lget(bucket, key, s->equals);
}

void* get(con_hashmap* s, void* key) {
    pthread_spin_lock(&s->lock);
    void* ret = _get(s, _hv(s, key), key);
    pthread_spin_unlock(&s->lock);
    return ret;
}

void _put(con_hashmap*s, uint32_t hv, void* key, void* val) {
    entry* bucket = _get_bucket(s, hv);
    if(bucket->next == NULL) {
        s->load ++;
    }
    if(s->cap * 4 <= s->load * 5) {
        _resize(s);
        bucket = _get_bucket(s, hv);
        if(bucket->next == NULL) {
            s->load ++;
        }
    }
    entry* item = create_entry(key, val, NULL);
    lput(bucket, item, s->equals);
    s->size ++;
}

void put(con_hashmap* s, void* key, void* val) {
    pthread_spin_lock(&s->lock);
    _put(s, _hv(s, key), key, val);
    pthread_spin_unlock(&s->lock);
}

void* _del(con_hashmap* s, uint32_t hv, void* key) {
    entry* bucket = _get_bucket(s, hv);
    void* ret = ldel(bucket, key, s->equals);
    if(bucket->next == NULL) {
        s->load --;
    }
    s->size --;
    return ret;
}

void* del(con_hashmap* s, void* key) {
    pthread_spin_lock(&s->lock);
    void* ret = _del(s, _hv(s, key), key);
    pthread_spin_unlock(&s->lock);
    return ret;
}

size_t size(con_hashmap* s) {
    return s->size;
}
```

con_segmap.h

```c
#pragma once
#include "con_hashmap.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define INIT_SIZE 4


typedef struct con_segmap {
    con_hashmap** submaps;
    uint32_t (*hash_func)(void* v);
    size_t con_level;
}con_segmap;

con_segmap* create_segmap(uint32_t (*hash_func)(void* v), bool (*equals)(void* a, void* b), size_t con_level) {
    con_segmap* ret = malloc(sizeof(con_segmap));
    ret->con_level = con_level;
    ret->hash_func = hash_func;
    ret->submaps = malloc(sizeof(con_hashmap*) * (1 << con_level));
    for(int i = 0; i < (1 << con_level); i ++) {
        ret->submaps[i] = create_hashmap(hash_func, equals);
        ret->submaps[i]->hash_offset = con_level;
    }
    return ret;
}

uint32_t __hv(con_segmap* s, void* x) {
    return (*s->hash_func)(x);
}

void sput(con_segmap* s, void* key, void* val) {
    uint32_t hv = __hv(s, key);
    uint32_t sub_id = hv & ((1 << s->con_level) - 1);
    pthread_spin_lock(&s->submaps[sub_id]->lock);
    _put(s->submaps[sub_id], hv >> s->con_level, key, val);
    pthread_spin_unlock(&s->submaps[sub_id]->lock);
}

void* sget(con_segmap* s, void* key) {
    uint32_t hv = __hv(s, key);
    uint32_t sub_id = hv & ((1 << s->con_level) - 1);
    pthread_spin_lock(&s->submaps[sub_id]->lock);
    void* ret = _get(s->submaps[sub_id], hv >> s->con_level, key);
    pthread_spin_unlock(&s->submaps[sub_id]->lock);
    return ret;
}

void* sdel(con_segmap* s, void* key) {
    uint32_t hv = __hv(s, key);
    uint32_t sub_id = hv & ((1 << s->con_level) - 1);
    pthread_spin_lock(&s->submaps[sub_id]->lock);
    void* ret = _del(s->submaps[sub_id], hv >> s->con_level, key);
    pthread_spin_unlock(&s->submaps[sub_id]->lock);
    return ret;
}

size_t ssize(con_segmap* s) {
    size_t ret = 0;
    for(int i = 0; i < (1 << s->con_level); i ++) {
        ret += s->submaps[i]->size;
    }
    return ret;
}
```

test.c

```c
#include "con_segmap.h"
#define MAXN 10000000
int c[MAXN];
int v[MAXN];
int a[MAXN];
int ptr, p;
uint32_t hash_func(void* x) {
    return *(int*)x;
}
bool equals(void* a, void* b) {
    return *(int*)a == *(int*)b;
}
int main()
{
    con_segmap* map = create_segmap(hash_func, equals, 4);
    for(int i = 0; i < MAXN; i ++) {
        v[i] = rand()%MAXN;
        int* cnt = sget(map, &v[i]);
        if (cnt == NULL) {
            c[ptr++] = 1;
            sput(map, &v[i], &c[ptr - 1]);
        } else {
            *cnt += 1;
        }
    }
    for(int i = 0; i < MAXN; i ++) {
        int* cnt = sget(map, &i);
        if(cnt == NULL) {
            continue;
        } else {
            while((*cnt) --) {
                a[p++] = i;
            }
        }
    }
}
```

