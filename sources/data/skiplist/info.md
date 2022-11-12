## VIII:[让你的代码更加优雅的编程技巧-跳转表](https://mp.weixin.qq.com/s/dDm-uTosbtBHV-d4Z5u-mw)

> 跳表比较好理解，但是实际用代码来表示，还是有点复杂的。
>
> 实现的方法不唯一

### 1. 什么是跳表

跳表是 **链表** + **索引** 的一种数据结构 ，是以空间换取时间的方式，关于跳表参考: https://baike.baidu.com/item/跳表/22819833?fr=aladdin

### 2. 跳表概念

跳表在原有链表的基础上，增加索引，从而可以进行二分查找，提高搜寻效率。

原始链表

```
Head ——> 1 ——> 8 ——> 12 ——> 23 ——> 55 ——> NULL   
```

新增了索引的链表(跳表)

```
Head2 ————————> 8 ———————————————————————> NULL 
Head1 ————————> 8 —————————> 23 —————————> NULL 
Head0 ——> 1 ——> 8 ——> 12 ——> 23 ——> 55 ——> NULL 
```

Head0 , Head1 , Head2 上都是真实的节点，这就是以空间换取时间

例如算上Head, 元素数据一共有 6 个，而添加索引后，元素一共有 11 个

### 3. 跳表增删查规则

#### **3.1 跳表数据节点**

数据节点可以和链表节点一致 ，也可以定义如下节点，除了数据外，有指针指向 前一个/后一个/上一个/下一个 节点，以便后续查找操作。

```
typedef struct {
        int data;
        struct Node *next; // 后一个节点
        struct Node *last; // 前一个节点
        struct Node *up; // 上一个节点
        struct Node *down; // 下一个节点
} Node;
```

#### **3.2 跳表初始化**

当跳表有多少层的时候，应当建立多少个头结点，例如: 跳表为3层

```
Head2 ——> NULL
Head1 ——> NULL
Head0 ——> NULL
```

#### 3.3 查找

> 删除/新增 都会进行查询才操作，无非是删除/新增索引而已。

例如有如下数据

```
Head2 —————————————————————> 23 —————————> NULL 
Head1 ————————> 8 —————————> 23 —————————> NULL 
Head0 ——> 1 ——> 8 ——> 12 ——> 23 ——> 55 ——> NULL
```

要查找 **13**这个节点

**去除无效层**

例如: **Head2** 后面第一个节点的数据 **23** ， 而 **23** 大于 **13** , 所以 **Head2** 没有数据匹配查询，故需要跳到下面一层，至 **Head1** 上进行查询。

**查询至Head0层**

**去除无效层**后数据进入了 **Head1** , 在**Head1**上进行匹配，当匹配到 **23** 时，**23**大于**13**，将**23**标记为 查询结束点，**对23的上一个节点 8 进行 向下指针操作**，进入 **Head0**层的**8**节点。

**查找实际数据**

从**Head0**层的8 进行查找，直至 **查询结束标记点**(head1 23), 查询的数据分别为 8 ， 12 ，23 查询结束，未找到数据。

#### **3.4 新增**

> 新增操作需要记录索引寻址过程，以便后续新增索引。

**头结点插入**

头结点插入一定是 **去除无效层** 至Head0 , 且 **Head0的第一个节点都比插入节点要大**的情况下

例如:

如下跳表，插入 2

```
Head2 —————————————————————> 23 —————————> NULL 
Head1 ————————> 8 —————————> 23 —————————> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 23 ——> 55 ——> NULL  
```

**尾结点插入**

头结点插入一定是 **去除无效层** 至Head0 , 且 **Head0的第一个节点都比插入节点要小**，直至NULL节点的情况下

例如:

如下跳表，插入 65

```
Head2 —————————————————————> 23 —————————> NULL 
Head1 ————————> 8 —————————> 23 —————————> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 23 ——> 55 ——> NULL  
```

**中间节点插入**

除开以上2种情况，其余情况为 中间节点插入

**新增索引**

> 抛硬币的方法，当数据量达到一定规模的时候，一定是趋近于 50%的。

所以跳表会越来越趋向于如下形式

```
    3
    3       7
1   3   5   7   9
1 2 3 4 5 6 7 8 9
```

判断是否需要新增索引，采取抛硬币的方法来判断，即: 随机数 取余 为 0 则需要新增，否则不需要。

例如如下跳表，插入 65

```
Head2 —————————————————————> 23 —————————> NULL 
Head1 ————————> 8 —————————> 23 —————————> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 23 ——> 55 ——> NULL  
```

寻址应该为
Head2: 23
Head1: 23

元素数据插入后为

```
Head2 —————————————————————> 23 ———————————————> NULL 
Head1 ————————> 8 —————————> 23 ———————————————> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 23 ——> 55 ——> 65 —> NULL  
```

当插入65节点后，若判断需要索引的时候，则先为 Head1 添加索引，添加位置为 寻址地址之后，寄 Head1: 23

```
Head2 —————————————————————> 23 ———————————————> NULL 
Head1 ————————> 8 —————————> 23 —————————> 65 —> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 23 ——> 55 ——> 65 —> NULL  
```

继续判断，若不需要添加索引，则插入结束

若还需要添加索引，则继续上述操作，直至 索引层 达到最高层

#### **3.5 删除**

删除首先是查找操作【3.3 查找】

若未找到该节点，则删除失败

若找到了该节点，则应当提到该数据最高索引层，再从高到低删除

例如:

如下跳表，删除 23

```
Head2 —————————————————————> 23 ———————————————> NULL 
Head1 ————————> 8 —————————> 23 —————————> 65 —> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 23 ——> 55 ——> 65 —> NULL  
```

找到 Head0 23 后，应该向上找到 Head2 23 ,然后从高向低删除，若删除后，该索引没有数据了，则索引层减1

则删除Head2 23 后数据如下

```
Head1 ————————> 8 —————————> 23 —————————> 65 —> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 23 ——> 55 ——> 65 —> NULL  
```

删除Head1 23 后数据如下

```
Head1 ————————> 8 ———————————————————————> 65 —> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 23 ——> 55 ——> 65 —> NULL 
```

删除Head0 23后数据如下

```
Head1 ————————> 8 ————————————————> 65 —> NULL 
Head0 ——> 3 ——> 8 ——> 12 ——> 55 ——> 65 —> NULL 
```

### 4. 代码

skipList.c

```c
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

int MaxLevel = 8; // 最大层数
int currLevel = 0; // 当前层数

// 数据节点
typedef struct {
        int data;
        struct Node *next;
        struct Node *last;
        struct Node *up;
        struct Node *down;
} Node;

// 记录索引寻址过程
typedef struct {
        int level;
        struct Node *node;
} skipStep;

// 判断是否需要新增索引, 抛硬币
bool randNum() {
        if(0 == (rand() % 2))
                return true;

        return false;
}

// 新增节点
bool add(Node *SL[] , int data) {

        printf("新增节点: %d\n",data);
        int level = currLevel;

        Node *Head = NULL;
        Node *tmp = NULL;
        Node *last = NULL;

        // 初始化索引 数据为 Head 地址
        skipStep steps[MaxLevel];
        int i;
        for (i=0;i<MaxLevel;i++) {
                steps[i].level = 0;
                steps[i].node = SL[i];
                Node *ss = steps[i].node;
        }


        // 赛选无效层
        Head = SL[level];
        tmp = Head->next;

        while ((level > 0) && (data < tmp->data)) {
                level--;
                Head = SL[level];
                tmp = Head->next;
        }

        // 根据索引寻找Head0数据节点
        while ((level > 0)) {

                while (tmp != NULL) {
                        if (data < tmp->data) {
                                steps[level].level = level;
                                if (NULL != last) 
                                        steps[level].node = last;
                                tmp = last->down;
                                level--;
                                break;
                        }

                        last = tmp;
                        tmp = tmp->next;
                }
                if (NULL == tmp) {
                        steps[level].level = level;
                        if (NULL != last) 
                                steps[level].node = last;
                        tmp = last->down;
                        level--;

                }
        }

        // Head0 数据合适的节点
        while (tmp != NULL) {
                if (data < tmp->data) {
                        break;
                }
                last = tmp;
                tmp = tmp->next;
        }

        // 新增节点
        Node *newData = (Node *)malloc(sizeof(Node));
        newData->data = data;
        newData->up = NULL;
        newData->down = NULL;
        newData->last = NULL;
        newData->next = NULL;

        int k = 0;

        // Head0 插入原始数据
        if (NULL == last ) {
                // 头结点

                Head = SL[0];
                Node *headNext = Head->next;
                if (NULL != headNext) {
                        newData->next = headNext;
                        headNext->last = newData;

                        newData->last = Head;


                } 

                Head->next = newData;
                newData->last = Head;


        } else if ( NULL == tmp) {
                // 尾节点
                last->next = newData;
                newData->last = last;


        } else {
                // 中间节点
                newData->next = tmp;
                tmp->last = newData;

                newData->last = last;
                last->next = newData;
        }

        // 构建索引
        while (randNum()) {
                k++;
                if (k >= MaxLevel) break;

                // 新增索引数据
                Node *newIndex = (Node *)malloc(sizeof(Node));
                newIndex->data = data;
                newIndex->up = NULL;
                newIndex->down = NULL;
                newIndex->next = NULL;
                newIndex->last = NULL;

                // 建立上下级关系
                newIndex->down = newData;
                newData->up = newIndex;

                Node *node = steps[k].node;

                // node->next
                Node *nextIndex = node->next;


                node->next = newIndex;
                newIndex->last = node;

                newIndex->next = nextIndex;
                if (NULL != nextIndex) 
                        nextIndex->last = newIndex;

                newData = newIndex;

                // 判断是否需要新增索引层数
                if (k > currLevel) 
                        currLevel = k;
        }
}


// 初始化头结点
Node *initSkipList(Node *skipList[]) {
        int i;
        for (i=0;i<MaxLevel;i++) {
                Node *newHead = (Node *)malloc(sizeof(Node));
                if (NULL == newHead) {
                        printf("%d 层 头结点申请失败\n");
                        return NULL;
                }
                newHead->data = -1-i;
                newHead->down = NULL;
                newHead->up = NULL;
                newHead->next = NULL;
                newHead->last = NULL;

                skipList[i] = newHead;


        }
        return skipList;
}

// 打印跳表数据
void PrintSkipList(Node *SL[]) {
        if (NULL == SL) {
                return;
        };

        int level = currLevel;
        //int level = MaxLevel;

        int i;
        for (i=level;i>=0;i--) {
                Node *Head = SL[i];

                Node *tmp = Head->next;
                printf("第%d层\t\t",i);
                while (NULL != tmp) {
                        printf(" %d\t",tmp->data);

                        tmp = tmp->next;
                }
                printf("\n");
        }
}

// 查询数据
Node *query(Node *SL[] , int data) {
        printf("查询数据: %d\n",data);

        int level = currLevel;

        Node *Head = NULL;
        Node *tmp = NULL;
        Node *last = NULL;

        Head = SL[level];
        tmp = Head->next;
        int endQuery = -1;

        // 筛除无效层
        while ((level > 0) && (data < tmp->data)) {
                level--;
                endQuery = tmp->data;
                Head = SL[level];
                tmp = Head->next;
        }

        // 根据索引定位到Head0层
        while ((level > 0 )) {

                while (tmp != NULL) {
                        if (data < (tmp->data)) {
                                level--;
                                endQuery = tmp->data;
                                tmp = last->down;
                                break;
                        }

                        last = tmp;
                        tmp = tmp->next;
                }
                if (NULL == tmp) {
                        tmp = last->down;
                        endQuery = -1;
                        level--;
                }

        }

        // 查询实际数据
        while (NULL != tmp) {
                if (endQuery != -1)
                        if (tmp->data > endQuery) {
                                        tmp = NULL;
                                        break;
                        }
                if (tmp->data == data) {
                        break;
                }
                tmp = tmp->next;
        }
        // 返回查询的数据节点，若没有查询到，应当返回NULL ,否则返回实际的地址
        return tmp;
}

// 删除数据
bool del(Node *SL[],int data) {
        printf("删除数据: %d\n",data);

        // 找到节点地址
        Node *tmp = query(SL,data);

        if (NULL == tmp) {
                printf("未找到节点,删除失败\n");
                return false;
        }
        int level = 0;
        Node *t_last = NULL;
        Node *t_next = NULL;


        // 找到该数据最高索引
        while (NULL != tmp->up) {
                level++;
                tmp = tmp->up;
        }

        // 由上至下删除索引/数据
        while (tmp != NULL) {
                t_last = tmp->last;
                t_next = tmp->next;

                Node *t_down = tmp->down;

                if (t_last == NULL) {
                        printf("上一个节点不可能为空，删除失败,层数: %d\n",level);
                        return false;
                }

                t_last->next = t_next;

                if (NULL != t_next)
                        t_next->last = t_last;
                else
                        t_last->next = NULL;

                if ((t_last == SL[level]) && (NULL == t_next)) {
                        currLevel--;

                }
                free(tmp);

                tmp = t_down;
                level--;
        }

        return true;


}

int main() {

        Node *SL[MaxLevel];

        Node *skipList = initSkipList(SL);
        if (NULL == SL) {
                printf("skipList 申请失败\n");
                return -1;
        }

        // 测试新增
        int num[] = {1,3,2,10,8,9,22,30,29,120,99,78,55,76,21};
        int i;
        for (i=0;i<sizeof(num)/sizeof(int);i++) {
                add(skipList,num[i]);
        }
        PrintSkipList(SL);

        // 测试删除
        int delNum[] = {99,9,78,55,3,1,28,78};
        for (i=0;i<sizeof(delNum)/sizeof(int);i++) {
                del(skipList,delNum[i]);
        }
        PrintSkipList(SL);

        printf("\n");
        return 0;
}
```

**执行结果**

```c
# gcc skipList.c -w -g
# ./a.out 
新增节点: 1
新增节点: 3
新增节点: 2
新增节点: 10
新增节点: 8
新增节点: 9
新增节点: 22
新增节点: 30
新增节点: 29
新增节点: 120
新增节点: 99
新增节点: 78
新增节点: 55
新增节点: 76
新增节点: 21
第5层            99
第4层            99
第3层            76      99
第2层            9       76      99
第1层            3       9       29      30      76      78      99
第0层            1       2       3       8       9       10      21      22      29      30      55      76      78      99      120
删除数据: 99
查询数据: 99
删除数据: 9
查询数据: 9
删除数据: 78
查询数据: 78
删除数据: 55
查询数据: 55
删除数据: 3
查询数据: 3
删除数据: 1
查询数据: 1
删除数据: 28
查询数据: 28
未找到节点,删除失败
删除数据: 78
查询数据: 78
未找到节点,删除失败
第3层            76
第2层            76
第1层            29      30      76
第0层            2       8       10      21      22      29      30      76      120

# 
```