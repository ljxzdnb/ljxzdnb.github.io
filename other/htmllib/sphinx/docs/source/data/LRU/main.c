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
// 销毁链表数据
void destory();

// 初始化
void init(int n)
{
    if (n > 0)
    {
        len = n;
    }
    else
    {
        //默认容量
        len = N;
    }
    head = tail = NULL;
    count = 0;
}

// 获取一个元素
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

