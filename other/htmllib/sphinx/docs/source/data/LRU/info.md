# [如何使用C链表实现 LRU 算法](https://mp.weixin.qq.com/s/iObqRVwOH_FcI6pQgc48eg)

# 什么是 LRU 算法 

## 

> LRU 是一种缓存淘汰策略。计算机的缓存容量有限，如果缓存满了就要删除一些内容，给新的内容腾位置。但是要删除哪些内容呢？我们肯定希望删掉那些没有用的缓存，而把有用的数据继续留在缓存中，方便之后继续使用。

> LRU 的全称是 Least Recently Used，也就是说我们认为最近使用过的数据应该是有用的，很久都没用过的数据应该是无用的，缓存满了就优先删除那些很久没有用过的数据。

## LRU 算法的特点

> 首先是缓存的大小是有限的。每次从缓存当中获取数据的时候，如果获取成功会将数据移动到最头部。同时新添加的元素也是在头部。当缓存大小达到上限之后，添加元素时会删除最久未使用的元素，也就是链表的最后一个元素，然后将新的元素插入在链表头。

## LRU 的应用场景

LRU 算法可以用来管理我们的缓存数据。控制我们的缓存大小，用较少的缓存空间达到更高的缓存数据。举例来说我们可以将一些不容易发生变化的数据且头部效应表中的数据加入到缓存当中。

## 编码实现

结构定义

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 默认容量
#define N 10

// 表示这个链表的长度信息
int len = 0;

//当前链表的元素个数信息
int count = 0;

typedef struct Node
{
    /* data */
    char *key;
    char *value;

    struct Node *next;
    struct Node *prev;
} Node;

// 链表的头节点和尾节点
struct Node *head;
struct Node *tail;

// 函数预声明
// 创建节点
Node *createNode(char *key, char *value);
// 插入节点到头部
void insertHead(Node *node);
// 移除尾部节点
void removeTail();
// 添加节点操作
void add(char *key, char *value);
// 删除链表中的一个节点
Node *deleteNode(Node *node);
// 获取指定key的值
char *get(char *key);
// 销毁数据
void destory();
```

插入操作

```c
// 获取一个元素
void add(char *key, char *value)
{
    Node *node = createNode(key, value);
    //第一个元素
    if (head == NULL)
    {
        insertHead(node);
        return;
    }

    //判断整个链表中是否存在整个元素
    Node *now = deleteNode(node);

    // 如果找到了元素 将元素移动至末尾 结束方法
    if (now != NULL)
    {
        // 设置新的值
        now->value = value;
        insertHead(now);
        return;
    }

    // 此时链表中是不存在这个元素
    // 判断链表的长度
    if (count >= len)
    {
        removeTail();
    }

    // 将新元素添加至末尾
    insertHead(node);
    return;
}
```

获取操作

```c
char *get(char *key)
{
    if (key == NULL)
    {
        return NULL;
    }

    // 寻找元素
    Node *node = createNode(key, NULL);
    Node *now = deleteNode(node);

    // 释放空间
    free(node);

    // 元素存在
    if (now != NULL)
    {
        //将元素调整到末尾
        insertHead(now);
        return now->value;
    }
    return NULL;
}
```

基本操作函数

```c
// 创建一个节点
Node *createNode(char *key, char *value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->prev = node->next = NULL;
    return node;
}

// 将节点插入到头节点部分
void insertHead(Node *node)
{
    // 元素为空时
    if (head == NULL)
    {
        tail = head = node;
        count++;
        return;
    }
    node->next = head;
    head->prev = node;
    // 移动链表的末尾指针
    head = node;
    // 计数
    count++;
}

// 移除
void removeTail()
{
    //移除最久未使用的那个元素
    Node *now = tail;
    if (now != NULL)
    {
        // 获取前一个节点
        Node *p = now->prev;

        if (p != NULL)
        {
            // 断开当前节点 同时移动尾节点
            p->next = NULL;
            tail = p;
        }
        else
        {
            head = tail = NULL;
        }
        // 当前节点置空
        now->prev = now->next = NULL;
        // 元素减少
        count--;
        // 释放空间
        free(now);
    }
}

// 链表中删除一个节点  删除成功返回被删除节点
Node *deleteNode(Node *node)
{
    Node *now = head;
    while (now != NULL)
    {
        // 存在节点
        if (strcmp(now->key, node->key) == 0)
        {
            // 获取前后节点
            Node *p = now->prev;
            Node *n = now->next;

            // 更新指向
            if (n != NULL)
            {
                n->prev = p;
            }
            else
            {
                tail = p;
            }

            if (p != NULL)
            {
                p->next = n;
            }
            else
            {
                head = n;
            }
            //当前节点置空
            now->prev = NULL;
            now->next = NULL;
            count--;
            break;
        }
        now = now->next;
    }
    return now;
}

// 销毁数据
void destory()
{
    Node *node = head;
    while (node != NULL)
    {
        Node *n = node;
        free(n);
        node = node->next;
    }
    len = 0;
    count = 0;
    head = tail = NULL;
}

// 从头节点开始打印整个链表
void printLink()
{
    Node *now = head;
    while (now != NULL)
    {
        printf("[key=%s,value=%s]", now->key, now->value);
        now = now->next;
    }
    printf("\n");
}
```

最后的测试函数

```c
int main()
{
    init(5);
    add("1", "1");
    add("2", "2");
    printLink();
    char *res = get("1");
    printLink();
    printf("value=%s\n", res);
    add("3", "3");
    add("4", "4");
    add("5", "5");
    add("6", "6");
    printLink();
    res = get("1");
    printLink();
    destory();
    return 0;
}

// 输出结果：
/*

[key=2,value=2][key=1,value=1]
[key=1,value=1][key=2,value=2]
value=1
[key=6,value=6][key=5,value=5][key=4,value=4][key=3,value=3][key=1,value=1]
[key=1,value=1][key=6,value=6][key=5,value=5][key=4,value=4][key=3,value=3]

*/

```