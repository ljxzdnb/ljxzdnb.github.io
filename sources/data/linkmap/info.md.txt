## III:C语言描述数据结构 —— [单链表](https://blog.csdn.net/weixin_59913110/article/details/126103367#t7)

前言
对于链表这部分，在数据结构里面是非常重要的。链表的学习周期长一些，博客的内容会多一些，所以对于链表的描述我将分为多篇。基本概念、基本实现、基本习题、进阶习题我都会通过博客向大家分享。那么本篇是对链表有一个初步的认识，并且能够掌握最基本的单链表的增删查改。

1. 顺序表的问题及思考
在学习链表之前，我们回顾以下顺序表，那么我们来思考下面几个问题：

中间/头部的插入删除，时间复杂度为O(N)
增容需要申请空间，拷贝数据，释放旧空间。这就会造成不小的空间消耗
增容一般是呈 2 倍的增长，势必会有一定的空间浪费。例如当前容量为 100，满了以后增容到 200 ，我们再继续插入 5 个数据，那么就浪费了 95 个数据空间。
那么我们要解决上述问题，就必须学习链表。所以带着问题去学习，那么将事半功倍。

2. 链表的基本概念和基本结构
基本概念：链表是一种物理存储结构上非连续、非顺序的存储结构，数据元素的逻辑顺序是通过链表中的指针链接次序实现的。  

如何理解物理存储结构是不连续的呢？

![img](https://img-blog.csdnimg.cn/0f101816b04247a282012b09c8370dc5.png)

 链表与顺序表不一样的是，顺序表它是一个数组，但是链表不是，所有的连续数据都是通过指针来链接的。那么现在又引出了一个问题，链表的每个数据都要链接下一个数据，而链接的方式是通过指针，那么如何做到呢？我们可以运用结构体来解决这个问题，并且提出结点的概念。

![img](https://img-blog.csdnimg.cn/dfa51cfc62724f36af04bb85270fb6f0.png)

可以看到，每个结点都有数据域和指针域，指针是指向下一个结点的地址，那么需要补充的是，第一个结点的地址需要用一个指针来保存，最后一个结点的指针是指向空指针。 保存第一个结点的地址的意义在于：可以找到链表的头结点，从而进一步访问结点之后的结点。最后一个结点的指针指向空指针的意义在于：避免野指针的问题，客观上增加一种链表的结束条件，例如字符串的结束标志 '\0' 。


![img](https://img-blog.csdnimg.cn/10a2924820cb41139aaa9d2166281df4.png)

那么我们就得出了链表的三个特征：

1. **链表的结构在逻辑上是连续的，但是在物理上不一定连续**
2. **现实中的结点一般都是从堆上申请出来的**
3. **从堆上申请的空间，是按照一定的策略来分配的，两次申请的空间可能连续，也可能不连续** 

### 3. 链表的分类 

链表的结构是有多种的，单向、双向、循环等等，但我们主要的侧重点在单向的不带哨兵卫的链表。

![img](https://img-blog.csdnimg.cn/e7c3cd0f70514b96a249a1eb5604783e.png)

### 4. 链表的实现

```c
//结点的声明
typedef int LLDataType;
typedef struct LinkList
{
	LLDataType val;
	struct LinkList* next;
}LL;
```

**存储头结点的指针在主函数中定义：**

```c
#include "LinkList.h"
int main()
{
	LL* head = NULL;
	return 0;
}
```

**头插的实现：**

```
//创建新结点
static LL* CreateNode(LLDataType x)
{
	LL* RetNode = (LL*)calloc(1, sizeof(LL));
	assert(RetNode);
	RetNode->val = x;
	RetNode->next = NULL;
	return RetNode;
}
//头插
void ListPushHead(LL** phead, LLDataType x)
{
	assert(phead);
	//此时我们还没有结点，需要创建一个结点
	LL* NewNode = CreateNode(x);
	//新结点的指针要指向以前的头结点
	NewNode->next = *phead;
	//头指针的内容更新为新结点的地址
	*phead = NewNode;
}
```

需要注意，即使存储头结点的指针是一个指针，但我们在函数内操作时，需要改变指针的指向位置，所以需要使用二级指针。我们可能对最后两条代码存在疑问，我在这里解释一下：

![img](https://img-blog.csdnimg.cn/d5b3f749329a45bba05bdf57ea0f985b.png)

**链表打印实现：**

```c
//打印
void ListPrint(LL* phead)
{
	LL* cur = phead;//使用临时变量来操作链表，但因为是传址调用，不使用也是可以的
	while (cur)
	{
		printf("%d->", cur->val);
		cur = cur->next;
	}
	printf("NULL\n");
}
```

 **测试：头插一个数据以及打印：**

```c
#include "LinkList.h"
int main()
{
	LL* head = NULL;
	ListPushHead(&head, 5);
	ListPrint(head);
	return 0;
}
```

**尾插的实现：**

```c
//尾插
void LIstPushBack(LL** phead, LLDataType x)
{
	assert(phead);
	LL* newnode = CreateNode(x);
	//如果链表没有结点，新结点直接作为头结点
	if (*phead == NULL)
	{
		*phead = newnode;
	}
	else
	{
		LL* tail = *phead;
		//向后寻找链表的最后一个结点
		while (tail->next != NULL)
		{
			tail = tail->next;
		}
		tail->next = newnode;
	}
}
```

可以看到，我们尾插的情况是有两个的，一个链表里面没有结点，那么就让这个要尾插的结点直接为头。而另一种则是链表中存在结点，我们需要遍历找到最后一个结点，并让这个结点的指针指向新的结点。同时需要注意，我们传的参数是二级指针，我们要解引用才能找到存放头结点地址的指针。

![img](https://img-blog.csdnimg.cn/725ae43afb874ca0a60276c77261db5d.png)

我们思考一个问题：我们能否像头插一样定义一个指针，专门用来存储最后一个结点的地址呢？当然可以，不过我们需要对函数的参数做出一些改变。

**头插更改：**

```c
//头插
void ListPushHead(LL** phead, LL** ptail,LLDataType x)
{
	//assert(phead);
	此时我们还没有结点，需要创建一个结点
	//LL* NewNode = CreateNode(x);
	新结点的指针要指向以前的头结点
	//NewNode->next = *phead;
	头指针的内容更新为新结点的地址
	//*phead = NewNode;
 
	assert(phead);
	//此时我们还没有结点，需要创建一个结点
	LL* NewNode = CreateNode(x);
	//链表为空时，第一个头插的结点就是尾结点
	if (*phead == NULL)
	{
		*ptail = NewNode;
	}
	//新结点的指针要指向以前的头结点
	NewNode->next = *phead;
	//头指针的内容更新为新结点的地址
	*phead = NewNode;
}
```

**尾插更改：**

```c
//尾插
void LIstPushBack(LL** phead,LL** ptail, LLDataType x)
{
	//assert(phead);
	//LL* NewNode = CreateNode(x);
	如果链表没有结点，新结点直接作为头结点
	//if (*phead == NULL)
	//{
	//	*phead = NewNode;
	//}
	//else
	//{
	//	LL* tail = *phead;
	//	//向后寻找链表的最后一个结点
	//	while (tail->next != NULL)
	//	{
	//		tail = tail->next;
	//	}
	//	tail->next = NewNode;
	//}
 
	assert(phead && ptail);
	LL* NewNode = CreateNode(x);
	//如果链表为空，那么头尾指针都指向这个新结点
	if (*phead == NULL)
	{
		*ptail = *phead = NewNode;
	}
	else
	{
		(*ptail)->next = NewNode;//尾结点的指针指向新结点
		(*ptail) = (*ptail)->next;//尾指针指向新结点
	}
}
```

**我们对修改之后的代码进行数据测试：**

```c
#include "LinkList.h"
int main()
{
	LL* head = NULL;
	LL* tail = NULL;//专门用来存储最后一个结点的地址
	ListPushHead(&head,&tail, 5);
	ListPushHead(&head, &tail, 6);
	ListPushHead(&head, &tail, 7);
	ListPrint(head);
 
	LIstPushBack(&head,&tail, 3);
	LIstPushBack(&head, &tail, 4);
	LIstPushBack(&head, &tail, 5);
	ListPrint(head);
 
	return 0;
}
```

![img](https://img-blog.csdnimg.cn/65398ad7352a495dbee6f81ed6f53ba4.png)

**头删的实现：**

```c
//头删
void ListPopHead(LL** phead, LL** ptail)
{
	assert(phead && ptail);
	assert(*phead);//链表为空不需要删除，强制退出
	LL* cur = *phead;
	//如果链表仅有一个结点
	if (cur->next == NULL)
	{
		free(cur);//释放结点
		*phead = *ptail = NULL;//存储头尾结点地址的指针置空
	}
	//链表有两个或两个以上结点时
	else
	{
		*phead = cur->next;
		free(cur);
		//尾指针不需要变
	}
}
```

**尾删的实现：**

```c
//尾删
void ListPopBack(LL** phead, LL** ptail)
{
	assert(phead && ptail);
	assert(*phead);//链表里面没有结点不需要删除 
	LL* cur = *ptail;
	LL* NewPtail = *phead;
	//找到倒数第二个结点
	while (NewPtail->next != *ptail)
	{
		NewPtail = NewPtail->next;
	}
	//对删除之前的尾结点释放
	free(cur);
	//存储尾结点地址的指针更新
	*ptail = NewPtail;
	(*ptail)->next = NULL;
	
}
```

**测试：头删和尾删：**

```c
#include "LinkList.h"
int main()
{
	LL* head = NULL;
	LL* tail = NULL;//专门用来存储最后一个结点的地址
	ListPushHead(&head,&tail, 5);
	ListPushHead(&head, &tail, 6);
	ListPushHead(&head, &tail, 7);
	ListPrint(head);
 
	LIstPushBack(&head,&tail, 3);
	LIstPushBack(&head, &tail, 4);
	LIstPushBack(&head, &tail, 5);
	ListPrint(head);
 
	ListPopHead(&head, &tail);
	ListPrint(head);
	ListPopBack(&head, &tail);
	ListPrint(head);
	return 0;
}
```

![img](https://img-blog.csdnimg.cn/7231f179036d48fd8f9217b129ee32b6.png)

**数据的查找：**

```c
//查找
LL* ListFind(LL* phead,LLDataType x)
{
	LL* cur = phead;
	while (cur)
	{
		if (cur->val == x)
			return cur;
		cur = cur->next;
	}
	return NULL;
}
```

我们的目的是找到数据的结点地址，如果确实有结点存储这个数据，那么就返回结点地址，否则返回空。如果有多个结点存储同一个数据，那么返回第一个结点的地址。这是符合常理的设计方法。

**在 pos 位置插入数据：**

```c
//在 pos 位置插入
void ListInsert(LL** phead, LL** ptail, LL* pos, LLDataType x)
{
	assert(phead && ptail);
	assert(*phead);//确保链表有结点
	//如果 pos 位置与头结点位置重合就头插
	if (pos == *phead)
	{
		ListPushHead(phead, ptail, x);
	}
	else
	{
		//在 pos 位置之前插入
		LL* cur = *phead;
		while (cur->next != pos)
		{
			cur = cur->next;
		}
		LL* NewNode = CreateNode(x);
		cur->next = NewNode;
		NewNode->next = pos;
	}
}
```

**测试：在 pos 位置插入数据(与查找配合)：**

```c
#include "LinkList.h"
int main()
{
	LL* head = NULL;
	LL* tail = NULL;//专门用来存储最后一个结点的地址
	ListPushHead(&head,&tail, 5);
	ListPushHead(&head, &tail, 6);
	ListPushHead(&head, &tail, 7);
	ListPrint(head);
 
	LIstPushBack(&head,&tail, 3);
	LIstPushBack(&head, &tail, 4);
	LIstPushBack(&head, &tail, 5);
	ListPrint(head);
 
	ListPopHead(&head, &tail);
	ListPrint(head);
	ListPopBack(&head, &tail);
	ListPrint(head);
 
	ListInsert(&head, &tail, ListFind(head,5), 50);
	ListPrint(head);
 
	return 0;
}
```

![img](https://img-blog.csdnimg.cn/856c1a37dc5749b0bbcb0332cb26c840.png)

**在 pos 位置删除结点：**

```c
//在 pos 位置删除
void ListPopInsert(LL** phead, LL** ptail, LL* pos)
{
	assert(phead && ptail);
	assert(*phead);//确保链表有结点
	//如果 pos 位置是头结点就头删
	if (pos == *phead)
	{
		ListPopHead(phead, ptail);
	}
	//如果 pos 位置是尾结点就尾删
	else if (pos == *ptail)
	{
		ListPopBack(phead, ptail);
	}
	else
	{
		LL* cur = *phead;
		while (cur->next != pos)
		{
			cur = cur->next;
		}
		cur->next = pos->next;
		free(pos);
	}
}
```

![img](https://img-blog.csdnimg.cn/7c85740a606d48d18e7cce89c9755b73.png)

### 5. 完整各文件代码

#### test.c 源文件

```c
#define _CRT_SECURE_NO_WARNINGS 1
 
#include "LinkList.h"
int main()
{
	LL* head = NULL;
	LL* tail = NULL;//专门用来存储最后一个结点的地址
	ListPushHead(&head,&tail, 5);
	ListPushHead(&head, &tail, 6);
	ListPushHead(&head, &tail, 7);
	ListPrint(head);
 
	LIstPushBack(&head,&tail, 3);
	LIstPushBack(&head, &tail, 4);
	LIstPushBack(&head, &tail, 5);
	ListPrint(head);
 
	ListPopHead(&head, &tail);
	ListPrint(head);
	ListPopBack(&head, &tail);
	ListPrint(head);
 
	ListInsert(&head, &tail, ListFind(head,5), 50);
	ListPrint(head);
 
	ListPopInsert(&head, &tail, ListFind(head, 5));
	ListPrint(head);
 
 
	return 0;
}
```

#### ListLink.c 源文件

```c
#define _CRT_SECURE_NO_WARNINGS 1
#include "LinkList.h"
 
//创建新结点
static LL* CreateNode(LLDataType x)
{
	LL* RetNode = (LL*)calloc(1, sizeof(LL));
	assert(RetNode);
	RetNode->val = x;
	RetNode->next = NULL;
	return RetNode;
}
//头插
void ListPushHead(LL** phead, LL** ptail,LLDataType x)
{
	//assert(phead);
	此时我们还没有结点，需要创建一个结点
	//LL* NewNode = CreateNode(x);
	新结点的指针要指向以前的头结点
	//NewNode->next = *phead;
	头指针的内容更新为新结点的地址
	//*phead = NewNode;
 
	assert(phead);
	//此时我们还没有结点，需要创建一个结点
	LL* NewNode = CreateNode(x);
	//链表为空时，第一个头插的结点就是尾结点
	if (*phead == NULL)
	{
		*ptail = NewNode;
	}
	//新结点的指针要指向以前的头结点
	NewNode->next = *phead;
	//头指针的内容更新为新结点的地址
	*phead = NewNode;
}
//打印
void ListPrint(LL* phead)
{
	LL* cur = phead;//使用临时变量来操作链表，但因为是传址调用，不使用也是可以的
	while (cur)
	{
		printf("%d->", cur->val);
		cur = cur->next;
	}
	printf("NULL\n");
}
//尾插
void LIstPushBack(LL** phead,LL** ptail, LLDataType x)
{
	//assert(phead);
	//LL* NewNode = CreateNode(x);
	如果链表没有结点，新结点直接作为头结点
	//if (*phead == NULL)
	//{
	//	*phead = NewNode;
	//}
	//else
	//{
	//	LL* tail = *phead;
	//	//向后寻找链表的最后一个结点
	//	while (tail->next != NULL)
	//	{
	//		tail = tail->next;
	//	}
	//	tail->next = NewNode;
	//}
 
	assert(phead && ptail);
	LL* NewNode = CreateNode(x);
	//如果链表为空，那么头尾指针都指向这个新结点
	if (*phead == NULL)
	{
		*ptail = *phead = NewNode;
	}
	else
	{
		(*ptail)->next = NewNode;//尾结点的指针指向新结点
		(*ptail) = (*ptail)->next;//尾指针指向新结点
	}
}
//头删
void ListPopHead(LL** phead, LL** ptail)
{
	assert(phead && ptail);
	assert(*phead);//链表为空不需要删除，强制退出
	LL* cur = *phead;
	//如果链表仅有一个结点
	if (cur->next == NULL)
	{
		free(cur);//释放结点
		*phead = *ptail = NULL;//存储头尾结点地址的指针置空
	}
	//链表有两个或两个以上结点时
	else
	{
		*phead = cur->next;
		free(cur);
		//尾指针不需要变
	}
}
//尾删
void ListPopBack(LL** phead, LL** ptail)
{
	assert(phead && ptail);
	assert(*phead);//链表里面没有结点不需要删除 
	LL* cur = *ptail;
	LL* NewPtail = *phead;
	//找到倒数第二个结点
	while (NewPtail->next != *ptail)
	{
		NewPtail = NewPtail->next;
	}
	//对删除之前的尾结点释放
	free(cur);
	//存储尾结点地址的指针更新
	*ptail = NewPtail;
	(*ptail)->next = NULL;
}
//查找
LL* ListFind(LL* phead,LLDataType x)
{
	LL* cur = phead;
	while (cur)
	{
		if (cur->val == x)
			return cur;
		cur = cur->next;
	}
	return NULL;
}
//在 pos 位置插入
void ListInsert(LL** phead, LL** ptail, LL* pos, LLDataType x)
{
	assert(phead && ptail);
	assert(*phead);//确保链表有结点
	//如果 pos 位置与头结点位置重合就头插
	if (pos == *phead)
	{
		ListPushHead(phead, ptail, x);
	}
	else
	{
		//在 pos 位置之前插入
		LL* cur = *phead;
		while (cur->next != pos)
		{
			cur = cur->next;
		}
		LL* NewNode = CreateNode(x);
		cur->next = NewNode;
		NewNode->next = pos;
	}
}
//在 pos 位置删除
void ListPopInsert(LL** phead, LL** ptail, LL* pos)
{
	assert(phead && ptail);
	assert(*phead);//确保链表有结点
	//如果 pos 位置是头结点就头删
	if (pos == *phead)
	{
		ListPopHead(phead, ptail);
	}
	//如果 pos 位置是尾结点就尾删
	else if (pos == *ptail)
	{
		ListPopBack(phead, ptail);
	}
	else
	{
		LL* cur = *phead;
		while (cur->next != pos)
		{
			cur = cur->next;
		}
		cur->next = pos->next;
		free(pos);
	}
}
```

#### ListLink.h 头文件

```c
#define _CRT_SECURE_NO_WARNINGS 1
 
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
//结点的声明
typedef int LLDataType;
typedef struct LinkList
{
	LLDataType val;
	struct LinkList* next;
}LL;
//存放头结点地址的指针
 
 
void ListPushHead(LL** phead, LL** ptail,LLDataType x);//头插
void ListPrint(LL* phead);//打印
void LIstPushBack(LL** phead,LL** ptail ,LLDataType x);//尾插 
void ListPopHead(LL** phead, LL** ptail);//头删
void ListPopBack(LL** phead, LL** ptail);//尾删
LL* ListFind(LL* phead,LLDataType x);//查找
void ListInsert(LL** phead, LL** ptail, LL* pos, LLDataType x);//在 pos 位置插入
void ListPopInsert(LL** phead, LL** ptail, LL* pos);//删除 pos 位置的数据
```

