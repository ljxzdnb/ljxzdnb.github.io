## IV:C语言描述数据结构 —— [栈和队列](https://blog.csdn.net/weixin_59913110/article/details/126179062?utm_source=app&amp;app_version=5.3.0&amp;code=app_1562916241&amp;uLinkId=usr1mkqgl919blen)

### 1.栈

1.1 栈的概念以及结构
栈是一种特殊的线性表，其只允许在固定的一端进行插入和删除元素操作。进行数据插入和删除操作的一端称为栈顶，另一端称为栈底。栈中的数据元素遵守后进先出(先进后出)的原则。

压栈：栈的插入草错叫做进栈、压栈、入栈。

![img](https://img-blog.csdnimg.cn/e1f7b5123ac7436c9b6c8a3cbfb41ef0.png)

出栈：栈的删除操作叫做出栈。

![img](https://img-blog.csdnimg.cn/2ed1acc2d58c45a999fb07bcd51b31c0.png)

#### 1.2 栈的实现

我们介绍过顺序表和链表，那么要实现栈的数据结构用顺序表合适还是用链表合适呢？一般都是用顺序表，因为对于栈的数据结构来说，只涉及到尾插尾删，所以综合顺序表和链表，顺序表是更占优势的。 

![img](https://img-blog.csdnimg.cn/1cf6ccc66dd9451ebbb6c84ef1f0af01.png)

```c
//Stack.h 头文件
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//可以动态开辟空间的栈
typedef int StackData;
typedef struct Stack
{
	StackData* a;
	int top;//与顺序表的 size 一致
	int capacity;
}Stack;
 
void StackInit(Stack* ps);//栈初始化
void StackPush(Stack* ps,StackData x);//入栈
void StackPop(Stack* ps);//出栈
StackData StackTop(Stack* ps);//获取栈顶元素
bool StackEmpty(Stack* ps);//判断栈是否为空
int StackSize(Stack* ps);//计算栈元素个数
void StackDestroy(Stack* ps);//栈销毁
```

```c
#include "Stack.h"
//初始化栈
void StackInit(Stack* ps)
{
	assert(ps);
	//栈的初始容量置空
	ps->a = NULL;
	ps->top = ps->capacity = 0;
}
//入栈
void StackPush(Stack* ps,StackData x)
{
	assert(ps);
	//入栈只有一种方式，所以不需要将扩容独立封装
	if (ps->top == ps->capacity)
	{
		int NewCapacity = ps->capacity == 0 ? 4 : 2 * ps->capacity;
		StackData* tmp = (StackData*)realloc(ps->a, NewCapacity*sizeof(StackData));
		assert(tmp);
		ps->a = tmp;
		ps->capacity = NewCapacity;
	}
	ps->a[ps->top] = x;
	ps->top++;
}
//出栈
void StackPop(Stack* ps)
{
	assert(ps);
	assert(!StackEmpty(ps));
	ps->top--;
}
//获取栈顶元素
StackData StackTop(Stack* ps)
{
	assert(ps);
	return ps -> a[ps->top - 1];
}
//判断栈是否为空
bool StackEmpty(Stack* ps)
{
	assert(ps);
	return ps->top == 0;//等于 0 则返回true，即栈为空
}
//计算栈中元素个数
int StackSize(Stack* ps)
{
	assert(ps);
	return ps->top;
}
//栈销毁
void StackDestroy(Stack* ps)
{
	assert(ps);
	free(ps->a);
	ps->a = NULL;
	ps->top = ps->capacity = 0;
}
 
```

### 2. 队列

#### 2.1 队列的概念及结构

队列只运训在一端进行插入数据操作，在另一端进行删除数据操作的**特殊线性表**。队列具有先进先出的特点。进行插入操作的一段称为**队尾**，进行删除操作的一端称为**队头**。

![img](https://img-blog.csdnimg.cn/43b82551e19a40c88df29cde040a8b7c.png)

####  2.2 队列的实现

我们采用无头单向不循环链表来实现队列。因为队列的特性，用链表描述可以说只涉及到**尾插、头删**。那么单链表是非常符合这个特性的。

```c
// Queue.h 头文件
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
//链表的结点
typedef int QueueData;
typedef struct QueueNode
{
	QueueData val;
	struct QueueNode* next;
}QueueNode;
//存储头、尾结点地址的指针
typedef struct HeadTail
{
	QueueNode* head;
	QueueNode* tail;
	int size;//记录队列有几个元素
}HeadTail;
 
void QueueInit(HeadTail* p);//初始化队列
void QueuePush(HeadTail* p,QueueData x);//进入队列
QueueData QueueHead(HeadTail* p);//获取队头元素
QueueData QueueTail(HeadTail* p);//获取队尾元素
void QueuePop(HeadTail* p);//删除操作，出队
bool QueueEmpty(HeadTail* p);//检查队列是否为空
int QueueSize(HeadTail* p);//获取队列元素个数
void QueueDestroy(HeadTail* p);//销毁队列
```

```c
// Queue.c 源文件
#include "Queue.h"
//初始化
void QueueInit(HeadTail* p)
{
	assert(p);
	p->head = p->tail = NULL;
	p->size = 0;
}
//队列尾插
void QueuePush(HeadTail* p, QueueData x)
{
	assert(p);
	//创建一个新结点
	QueueNode* newnode = (QueueNode*)calloc(1, sizeof(QueueNode));
	assert(newnode);
	newnode->val = x;
	newnode->next = NULL;
	//如果链表内没有结点，头尾指针指向同一个结点
	if (p->head == NULL)
	{
		p->head = newnode;
		p->tail = newnode;
	}
	//否则尾指针需要变化
	else
	{
		p->tail->next = newnode;
		p->tail = newnode;
	}
	p->size++;
}
//获取队头元素
QueueData QueueHead(HeadTail* p)
{
	assert(p);
	assert(!QueueEmpty(p));
	return p->head->val;
}
//获取队尾元素
QueueData QueueTail(HeadTail* p)
{
	assert(p);
	assert(!QueueEmpty(p));
	return p->tail->val;
}
//删除、出队
void QueuePop(HeadTail* p)
{
	assert(p);
	assert(!QueueEmpty(p));
	//如果链表只有一个结点
	if (p->head == p->tail)
	{
		free(p->head);
		p->head = p->tail = NULL;
	}
	//否则进行头删
	else
	{
		QueueNode* cur = p->head;
		p->head = p->head->next;
		free(cur);
		cur = NULL;
	}
	p->size--;
}
//检测队列是否为空
bool QueueEmpty(HeadTail* p)
{
	assert(p);
	return p->head == NULL && p->tail == NULL;
}
//获取队列元素个数
int QueueSize(HeadTail* p)
{
	assert(p);
	return p->size;
}
//销毁队列
void QueueDestroy(HeadTail* p)
{
	assert(p);
	QueueNode* cur = p->head;
	while (cur)
	{
		QueueNode* del = cur;
		cur = cur->next;
		free(del);
	}
}
```

```c
// test.c 源文件
#include "Queue.h"
 
void TestQueue()
{
	HeadTail p;
	QueueInit(&p);
	
	QueuePush(&p, 1);
	QueuePush(&p, 2);
	QueuePush(&p, 3);
	QueuePush(&p, 4);
	printf("%d\n", QueueSize(&p));
	//进入队列的顺序为 1、2、3、4
	while (!QueueEmpty(&p))
	{
		//所以打印的顺序也为 1、2、3、4
		//先进先出
		printf("%d ", QueueHead(&p));
		QueuePop(&p);
	}
	printf("\n");
	printf("%d\n", QueueSize(&p));
 
	QueueDestroy(&p);
}
int main()
{
	TestQueue();
	return 0;
}
```
