# [hikvision test](https://tieba.baidu.com/photo/p?kw=%E9%AB%98%E9%80%9A&ie=utf-8&flux=1&tid=6977888561&pic_id=86be72094b36acaf859ec7a06bd98d1000e99cc5&pn=1&fp=2&see_lz=1)

## 1: 定义一个常量指针并将其初始化用如下那个语句正确实现？

const int a = 10;

int const *ptr = &a;

```c
A:const int ic = i
B:const int *pic = &ic
C:int i = i
D:int *const cpi = &ic
```

```c
int main()
{
int i = -1;                                    //正确
const int ic = i;                          //正确，常量并初始化c
ic = 10;			//assignment of read-only variable ‘ic’
const int *pic = &ic;                  //正确，pic是指向const对象的指针，ic是const对象
*pic = NULL;		//assignment of read-only location ‘*pic’
int *const ipc = &ic;                  //错误，ipc是const指针，ic是const对象，“初始化”: 无法从“const int *__w64 ”转换为“int *const ”
const int *const ipcc = &ic;      //正确，ipcc是指向const对象的指针，ic是const对象,并没有修改ipcc的值
printf("%d\n",*pic);
return 0;
}
```

## 2: 不能定义为用户标识符的是（）2ong //c语言的标识符必须以字母或下划线开头

## 3:如下代码片段运行后a,b,c的结果分别是多少（）7,7,13

```c
int a = 6,b = 7,c ;
c = a+++b;
```



C语言会同意一些"令人震惊"的结构，下面的结构是合法的吗，我们来看看几个例子。

c = a+++b；

以下代码是合法的吗，咋的一看不禁有这样的疑问？

int a = 5, b = 7, c;

c = a+++b;

这个代码确实不咋符合习惯的写法，但是不管你相不相信，上面的例子是完全合乎语法的。问题是编译器如何处理它？




根据最处理原则，编译器应该能够尽可能处理所有合法的用法。因此，上面的代码会被处理成：

c = a++ + b;

我们来测试一下：

```c
#include <stdio.h>

int main()

{

  int a = 5, b = 7, c;

  c = a+++b;

  printf("a = %d,b = %d,c = %d",a,b,c);

  return 0;

}
```

输出结果如下：

a=6,b=7,c=12


其执行顺序：

b不变，c = a + b;，则c = 5 + 7 = 12

a++，那么a = 6;

c = a+++++b;


上面那么可能只是开胃菜，那么这个表达式呢，有什么想法？咱们二话不说，直接上代码测试。

```c
#include <stdio.h>

int main()

{

  int a = 5, b = 7, c;

  c = a+++++b;

  //c = (a++) + (++b);

  //c = ((a++)++) + b;

  printf("a = %d,b = %d,c = %d",a,b,c);

  return 0;

}
```

编译结果如下：

![img](https://i0.hdslb.com/bfs/article/f46469dd689fe0b8043ef4a21855eb2df4afd321.png@711w_153h_progressive.webp)


虽然一看就知道意思是(a++)+(++b)；，但是编译就通不过，我们把括号加上，c = (a++) + (++b);，编译通过，那么出错信息大概是：括号影响了优先级？我们从侧面以及原理来解析一下。


侧面解析一下

对于a+++++b这一段代码，编译系统从左至右扫描整条语句，先遇到a++，判断出来是一个a的后缀自加运算；

然后接着扫描，遇到一个+，+是一个二目运算符，它的左边已经有一个运算数a++了，系统就向右搜索第二个运算数；

又遇到一个+，++比+的运算级别要高，这时，编译系统就将两个+看成一个整体来处理；

既然是++，编译系统就认定，肯定它的左边或右边有一个变量，编译系统先搜索左边，发现++，不是变量；

再搜索右边，发现+b，+b是什么东西？编译系统是无法搞明白的；

因此它就认为++是一个缺少左值的自增运算符，于是提示错误给用户：lvalue required as increment operand

原理解析一下

C语言在这里遵循词法解析的贪婪匹配原则。优先匹配尽可能多字符的符号，无论是否有语法错误（因为词法分析时还没有语法检查）。

于是a+++++b会被当作a ++ ++ + b，这是非法的表达式，因此产生编译错误。

这个问题在ISO C99标准中直接以示例描述，原文如下：

“EXAMPLE 2 The program fragment x+++++y is parsed as x ++ ++ + y, which violates a constraint on increment operators, even though the parse x ++ + ++ y might yield a correct expression.

那么，也许是c = ((a++)++) + b;这个意思呢？

我们编译结果依然报错，如下：

![img](https://i0.hdslb.com/bfs/article/c63a1448765a007902a925857f52d7047c99a801.png@711w_153h_progressive.webp)


这涉及到“左值”的问题。百度百科定义如下：

“左值(lvalue) 是B语言/C语言/C++语言等类C语言中的一类表达式。“左”(left)的原意是指可以放在赋值符号“=”的左边，但其实也表示能作为&和++等操作符的操作数（B语言中已经如此）。而且，现代C/C++中的含义已经不局限于此。lvalue的l被重新解释为location。这也对应于ISO C11/ISO C++11的内存位置(memory location)。

总结：a存在左值，可以有表达式：a++。但是（a++）不存在左值，无法继续执行（a++）++操作，所以最终报错。




怎么样才能编译正确呢？

a+++++b在编译时会报错，那么a++ + ++b呢？来看以下代码：

```c
#include <stdio.h>

int main()

{

int a = 5, b = 7, c;

c = a++ + ++b;

printf("a = %d,b = %d,c = %d",a,b,c);

return 0;

}
```

结果如下：

![img](https://i0.hdslb.com/bfs/article/c8b5513950edb1b32e045ef41a158516dc2093af.png@573w_132h_progressive.webp)


其代码与c = (a++) + (++b);结果一样，说明是正确的，其按照下面顺序执行：

先执行b自加，b变为8；相当于：b = b+ 1;

求a与b之和，赋给c；相当于：c = a + b ;//c = 5+8;

执行第二步之后，a自加1：a++;

c=(++a,b++,a++,++b);

这个表达式看着爽不爽？我们知道自增自减运算，表示对自身进行a=a+1或者a=a-1的运算。

++a表示在调用前就a+1，a++表示在调用后+1。

int c=(++a,b++,a++,++b);这个逗号隔开的表示用最后一个式子对C进行赋值，测试如下：

```c
#include <stdio.h>

int main()

{

int a = 5, b = 7, c;

c=(++a,b++,a++,++b);

printf("a = %d,b = %d,c = %d",a,b,c);

return 0;

}
```

输出的结果如下：

![img](https://i0.hdslb.com/bfs/article/dc31f2a74be1647140d8b9452d6f6fda550866e5.png@596w_120h_progressive.webp)


这段执行的顺序如下

先执行++a，a=6；

再执行b++，b=8；

接下来a++，a=7；

再执行++b，b=9；

把最后一个的式子b=9的值赋给c，所以c等于9。如果改成c=(++a,b++,a++,b++);，那么c就是等于8，因为是调用后再自增。

## 4: int a[100],对指针变量p的正确定义和初始化。

```
A:int p=&a;
B:int p=*a;
C:int *p=a;
D:int *p=&a;
```

```c
#include <stdio.h>

int size(int* p){
    int length = 0;
    while (*p){
        length++;
        p++;
    }
    return length;
}

void main()
{
    int a[100]={1,2,3};
    int *p = a;
    int *n = &a;
    int (*q)[100] = &a; 
    printf("%d,%d,%d\n",*p,*n,(*q)[0]);
    printf("%d,%d",sizeof(a) / sizeof(a[0]),size((int *)q));//size(a)
}
```

fib.c:8:14: warning: initialization of ‘int *’ from incompatible pointer type ‘int (*)[100]’ [-Wincompatible-pointer-types]
    8 |     int *n = &a;
      |              ^

1,1,1
100,3

## 5: 程序运行结果是什么？

```c
#include <stdio.h>

void main()
{
    int a = 10,b=20,c=30;
    printf("\n %d,%d,%d\n",a+b+c,(b=b*2),(c=c*2));
}
```

110,40,60

| **优先级**                                                   | **运算符**       | **名称或含义**           | **使用形式**             | **结合方向** | **说明**   |
| ------------------------------------------------------------ | ---------------- | ------------------------ | ------------------------ | ------------ | ---------- |
| **1**                                                        | **[]**           | 数组下标                 | 数组名[常量表达式]       | 左到右       | --         |
| **()**                                                       | 圆括号           | (表达式）/函数名(形参表) | --                       |              |            |
| **.**                                                        | 成员选择（对象） | 对象.成员名              | --                       |              |            |
| **->**                                                       | 成员选择（指针） | 对象指针->成员名         | --                       |              |            |
|                                                              |                  |                          |                          |              |            |
| **2**                                                        | **-**            | 负号运算符               | -表达式                  | **右到左**   | 单目运算符 |
| **~**                                                        | 按位取反运算符   | ~表达式                  |                          |              |            |
| **++**                                                       | 自增运算符       | ++变量名/变量名++        |                          |              |            |
| **--**                                                       | 自减运算符       | --变量名/变量名--        |                          |              |            |
| xxxxxxxxxx /**  * of_get_mac_address - 从设备树中提取MAC地址 * @np - 设备节点指针 * @成功：MAC（6字节）的首地址；失败：NULL */void *of_get_mac_address(struct device_node *np);c | 取值运算符       | *指针变量                |                          |              |            |
| **&**                                                        | 取地址运算符     | &变量名                  |                          |              |            |
| **!**                                                        | 逻辑非运算符     | !表达式                  |                          |              |            |
| **(类型)**                                                   | 强制类型转换     | (数据类型)表达式         | --                       |              |            |
| **sizeof**                                                   | 长度运算符       | sizeof(表达式)           | --                       |              |            |
|                                                              |                  |                          |                          |              |            |
| **3**                                                        | **/**            | 除                       | 表达式/表达式            | 左到右       | 双目运算符 |
| *****                                                        | 乘               | 表达式*表达式            |                          |              |            |
| **%**                                                        | 余数（取模）     | 整型表达式%整型表达式    |                          |              |            |
| **4**                                                        | **+**            | 加                       | 表达式+表达式            | 左到右       | 双目运算符 |
| **-**                                                        | 减               | 表达式-表达式            |                          |              |            |
| **5**                                                        | **<<**           | 左移                     | 变量<<表达式             | 左到右       | 双目运算符 |
| **>>**                                                       | 右移             | 变量>>表达式             |                          |              |            |
|                                                              |                  |                          |                          |              |            |
| **6**                                                        | **>**            | 大于                     | 表达式>表达式            | 左到右       | 双目运算符 |
| **>=**                                                       | 大于等于         | 表达式>=表达式           |                          |              |            |
| **<**                                                        | 小于             | 表达式<表达式            |                          |              |            |
| **<=**                                                       | 小于等于         | 表达式<=表达式           |                          |              |            |
| **7**                                                        | **==**           | 等于                     | 表达式==表达式           | 左到右       | 双目运算符 |
| **！=**                                                      | 不等于           | 表达式!= 表达式          |                          |              |            |
|                                                              |                  |                          |                          |              |            |
| **8**                                                        | **&**            | 按位与                   | 表达式&表达式            | 左到右       | 双目运算符 |
| **9**                                                        | **^**            | 按位异或                 | 表达式^表达式            | 左到右       | 双目运算符 |
| **10**                                                       | **\|**           | 按位或                   | 表达式\|表达式           | 左到右       | 双目运算符 |
| **11**                                                       | **&&**           | 逻辑与                   | 表达式&&表达式           | 左到右       | 双目运算符 |
| **12**                                                       | **\|\|**         | 逻辑或                   | 表达式\|\|表达式         | 左到右       | 双目运算符 |
|                                                              |                  |                          |                          |              |            |
| **13**                                                       | **?:**           | 条件运算符               | 表达式1?表达式2: 表达式3 | **右到左**   | 三目运算符 |
|                                                              |                  |                          |                          |              |            |
| **14**                                                       | **=**            | 赋值运算符               | 变量=表达式              | **右到左**   | --         |
| **/=**                                                       | 除后赋值         | 变量/=表达式             | --                       |              |            |
| ***=**                                                       | 乘后赋值         | 变量*=表达式             | --                       |              |            |
| **%=**                                                       | 取模后赋值       | 变量%=表达式             | --                       |              |            |
| **+=**                                                       | 加后赋值         | 变量+=表达式             | --                       |              |            |
| **-=**                                                       | 减后赋值         | 变量-=表达式             | --                       |              |            |
| **<<=**                                                      | 左移后赋值       | 变量<<=表达式            | --                       |              |            |
| **>>=**                                                      | 右移后赋值       | 变量>>=表达式            | --                       |              |            |
| **&=**                                                       | 按位与后赋值     | 变量&=表达式             | --                       |              |            |
| **^=**                                                       | 按位异或后赋值   | 变量^=表达式             | --                       |              |            |
| **\|=**                                                      | 按位或后赋值     | 变量\|=表达式            | --                       |              |            |
|                                                              |                  |                          |                          |              |            |
| **15**                                                       | **，**           | 逗号运算符               | 表达式,表达式,…          | 左到右       | --         |

## 6: 广播的mac地址

```
A: 00-00-00-00-00
B: FF-FF-FF-FF
C: 00-00-00-FF-FF-FF
D: FF-FF-FF-FF-FF-FF
```

[MAC 地址（单播、组播、广播地址分类）](https://blog.csdn.net/qq_40309341/article/details/119004455)

1、单播 MAC 地址是指第一个字节的最低位是 0 的 MAC 地址；

2、组播 MAC 地址是指第一个字节的最低位是 1 的 MAC 地址；

3、广播 MAC 地址是指每个比特都是 1 的 MAC 地址。广播 MAC 地址是组播 MAC 地址的一个特例。

![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/a9300a90993b096ed0e3d6a294d70188.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/f5f3b034d99f7c03c2d48b3e4c68d856.png)

只要在下图中有一处对应，即可判断，其实只要记住开头就可以了 。若都不对应，则为非法Mac地址。

开头为： ？0 为单播 ；
开头为： ？1 为组播 ；
语句为： ff-ff… 为广播 ；
以上都不是： 为非法 。

详解：一个单播 MAC 地址（例如 BIA 地址）标识了一块特定的网卡；一个组播 MAC 地址标识的是一组网卡；广播 MAC 地址是组播 MAC 地址的一个特例，它标识了所有的网卡。从上图我们可以发现，并非一个 MAC 地址的前 3 个字节都是 OUI，只有单播 MAC 地址的前 3 个字节才是 OUI，而组播或广播 MAC 地址的前 3 个字节一定不是 OUI。特别需要说明的是，OUI 的第一个字节的最低位一定是 0。

注：
广义上的组播地址，只要是高位第八位为1的，都是属于组播地址。
只是我们组播中常见 的组播数据报文是以01005e 开头的，并不冲突。

## 7：UDP缓存区满了，数据是否丢失（）是

**UNP编程：16---UDP之（[UDP套接字的数据缓冲区与数据丢失](https://blog.51cto.com/u_15346415/5171352)）**

一、UDP套接字的数据接收缓冲区
事实上每个UDP套接字都有一个接收缓冲区， 到达该套接字的每个数据报都进入这个套接字接收缓冲区。当进程调用recvfrom时，缓冲区中 的下一个数据报以FIFO（先入先出）顺序返回给进程。这样，在进程能够读该套接字中任何已 排好队的数据报之前，如果有多个数据报到达该套接字，那么相继到达的数据报仅仅加到该套 接字的接收缓冲区中。然而这个缓冲区的大小是有限的。我们已在7.5节随SO_RCVBUF套接字选 项讨论了这个大小以及如何增大它
二、UDP与TCP的不同
不同之处
TCP每连接一个客户端，然后fork，这样，每一个连接都会有自己的数据接收缓冲区
UDP不会与客户端建立连接，只有一个缓冲区用来接受所有数据
其次，该函数提供的是一个迭代服务器（iterative server），而不是像TCP服务器那样可以提 供一个并发服务器。其中没有对fork的调用，因此单个服务器进程就得处理所有客户。一般来 说，大多数TCP服务器是并发的，而大多数UDP服务器是迭代的
下图总结了第5章中的TCP客户/服务器在两个客户与服务器建立连接时的情形（服务器主机上有两个已连接套接字，其中每一个都有各自的套接字接收缓冲区）

![UNP编程：16---UDP之（UDP套接字的数据缓冲区与数据丢失）_数据](https://s2.51cto.com/images/blog/202112/31181032_61ced7187d87d1151.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=)

下图展示了两个客户发送数据报到UDP服务器的情形（其中只有一个服务器进程，它仅有的单个套接字用于接收所有到达的数据报并发回所有的 响应。该套接字有一个接收缓冲区用来存放所到达的数据报）

![UNP编程：16---UDP之（UDP套接字的数据缓冲区与数据丢失）_套接字_02](https://s2.51cto.com/images/blog/202112/31181032_61ced718aef8b19310.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=)

三、UDP数据包丢失案例与数据缓冲区
服务端向服务端发送2000个1400字节的数据，然后服务端接收
程序运行前、后，我们用netstat -s查看UDP的丢包率
服务端

```c
#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<signal.h>

#define MAXLINE 1024
static int count;

int Socket(int domain,int type,int protocal);
void Bind(int sockfd,struct sockaddr* serveraddr,socklen_t len);
void dg_echo(int sockfd,struct sockaddr* cliaddr,socklen_t len);
static void recvfrom_int(int sig);

int main(int argc,char *argv[])
{
    if(argc!=3){
        perror("please enter [server ip] [server port]");
        exit(1);
    }

    int sockFd=Socket(AF_INET,SOCK_DGRAM,0);

    struct sockaddr_in serverAddr;
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    if(inet_aton(argv[1],(struct in_addr*)&serverAddr.sin_addr)==0){
        perror("inet_aton");
        exit(3);
    }

    ssize_t len=sizeof(serverAddr);
    Bind(sockFd,(struct sockaddr*)&serverAddr,len);

    struct sockaddr_in clientAddr;
    bzero(&clientAddr,sizeof(clientAddr));
    socklen_t clilen=sizeof(clientAddr);
    dg_echo(sockFd,(struct sockaddr*)&clientAddr,clilen);

    exit(0);
}

int Socket(int domain,int type,int protocal)
{
    int sockFd=socket(domain,type,protocal);
    if(sockFd<0){
        perror("socket");
        exit(2);
    }
    return sockFd;
}

void Bind(int sockfd,struct sockaddr* serveraddr,socklen_t len)
{
    if(bind(sockfd,serveraddr,len)==-1){
        perror("bind");
        exit(4);
    }
}

void dg_echo(int sockfd,struct sockaddr* cliaddr,socklen_t clilen)
{
    int len;
    char mesg[MAXLINE];

    signal(SIGINT,recvfrom_int);
    for(;;){
        len=clilen;
        recvfrom(sockfd,mesg,MAXLINE,0,cliaddr,&len);
        count++;
    }
}

static void recvfrom_int(int sig)
{
    printf("received %d datagrams\n",count);
    exit(0);
}
```


客户端

```c
#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>


#define NDG 2000
#define DGLEN 1400

int Socket(int domain,int type,int protocal);
void dg_cli();

int main(int argc,char *argv[])
{
    if(argc!=3){
        perror("please enter [server ip] [server port]");
        exit(1);
    }

    int sockFd=Socket(AF_INET,SOCK_DGRAM,0);

    struct sockaddr_in serverAddr;
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    if(inet_aton(argv[1],(struct in_addr*)&serverAddr.sin_addr)==0){
        perror("inet_aton");
        exit(3);
    }

    socklen_t len=sizeof(serverAddr);
    dg_cli(stdin,sockFd,(struct sockaddr*)&serverAddr,len);

    exit(0);
}

int Socket(int domain,int type,int protocal)
{
    int sockFd=socket(domain,type,protocal);
    if(sockFd<0){
        perror("socket");
        exit(2);
    }
    return sockFd;
}

void dg_cli(FILE *fp,int sockfd,const struct sockaddr* pserveraddr,socklen_t servlen)
{
    int i;
    char sendline[DGLEN];
    for(i=0;i<NDG;++i)
    {
        ssize_t len=sendto(sockfd,sendline,DGLEN,0,pserveraddr,servlen);
        if(len==-1){
            perror("sendto");
            exit(4);
        }
    }
}

```

左边是服务端，中间是客户端，最右边是程序运行之前UDP的情况


程序运行之后，netstat -s再查看一下数据，可以看到有数据丢失

四、使用setsockopt设置UDP的数据缓冲区
我们承上面的案例，使用setsockopt设置服务端UDP数据报的接收缓冲区大小

```c
static void	recvfrom_int(int);
static int	count;

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE];

	Signal(SIGINT, recvfrom_int);

	n = 220 * 1024;
	Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n));

	for ( ; ; ) {
		len = clilen;
		Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

		count++;
	}
}

static void
recvfrom_int(int signo)
{
	printf("\nreceived %d datagrams\n", count);
	exit(0);
}
```

运行前：

![UNP编程：16---UDP之（UDP套接字的数据缓冲区与数据丢失）_套接字_05](https://s2.51cto.com/images/blog/202112/31181033_61ced7199f97741978.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=)

运行后：可以看到数据没有丢失

![UNP编程：16---UDP之（UDP套接字的数据缓冲区与数据丢失）_套接字_06](https://s2.51cto.com/images/blog/202112/31181033_61ced719c157d22788.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_30,g_se,x_10,y_10,shadow_20,type_ZmFuZ3poZW5naGVpdGk=)

由UDP给某个特定套接字排队的UDP数据报数目受限于该套接字接收缓冲区的大小。我们 可以使用SO_RCVBUF套接字选项修改该值，如7.5节所述。在FreeBSD下UDP套接字接收缓冲区 的默认大小为42 080字节，也就是只有30个1400字节数据报的容纳空间。如果我们增大套接字 接收缓冲区的大小，那么服务器有望接收更多的数据报。图8-22给出了对图8-20中dg_echo函数 的修改，把套接字接收缓冲区设置为240 KB
在Sun主机上运行这个服务器程序，在RS/6000主机上运行其客户程序，接收到的数据报计数现 在变为103。这比前面使用默认套接字接收缓冲区的例子稍有改善，不过仍然不能从根本上解决问题

在上面代码中我们为什么把接收套接字缓冲区大小设为220×1 024字节呢？FreeBSD5.1中 一个套接字接收缓冲区的最大大小默认为262 144字节（256×1 024），但是由于缓冲区分配策 略（见TCPv2第2章），真实的限制是233 016字节。许多基于4.3BSD的早期系统把一个套接字 缓冲区的大小限制为52 000字节左右

## 8：与c语言相比，汇编语言的可移植性（）

C语言与汇编语言相比，可移植性好。

## 9：一个8位二进制整数，若采用补码表示，且由4个1和4个0组成，则[最小值](https://m.nowcoder.com/questionTerminal?uuid=4f1ff0ae60914e929ef38e80fc14e308)为（）-121

求最小值首先考虑的是负值，所以最高位符号位用去一个1，因为是补码表示，所以真值是求反并且最低位进1，因此补码的低位优先填1，可以得出补码表示为10000111，再次求得

原码11111001，真值为-121

## 13：如下减少内存碎片的措施中错误的是（）A

```
A:使用多少，分配多少，不需要对齐，减少内存消耗
B:一次性申请大内存，而且避免反复申请小内存（减少内存分割）
C:使用内存池
D:内存开销不大且空间消耗固定的情况下，少用动态内存分配，使用栈空间
```



### 问题

## 简述线程和进程有什么优缺点？有什么应用场景？

简答：

多进程优点
①编程相对容易；通常不需要考虑锁和同步资源的问题。
②更强的容错性：比起多线程的一个好处是一个进程崩溃了不会影响其他进程。
③有内核保证的隔离：数据和错误隔离。 对于使用如C/C++这些语言编写的本地代码，错误隔离是非常有用的：采用多进程架构的程序一般可以做到一定程度的自恢复；（master守护进程监控所有worker进程，发现进程挂掉后将其重启）。

多线程的优点：
①创建速度快，方便高效的数据共享
共享数据：多线程间可以共享同一虚拟地址空间；多进程间的数据共享就需要用到共享内存、信号量等IPC技术。
②较轻的上下文切换开销 - 不用切换地址空间，不用更改寄存器，不用刷新TLB。
③提供非均质的服务。如果全都是计算任务，但每个任务的耗时不都为1s，而是1ms-1s之间波动；这样，多线程相比多进程的优势就体现出来，它能有效降低“简单任务被复杂任务压住”的概率。

应用场景
多进程应用场景
1.nginx主流的工作模式是多进程模式（也支持多线程模型）
2.几乎所有的web server服务器服务都有多进程的，至少有一个守护进程配合一个worker进程，例如apached,httpd等等以d结尾的进程包括init.d本身就是0级总进程，所有你认知的进程都是它的子进程；
3.chrome浏览器也是多进程方式。 （原因：①可能存在一些网页不符合编程规范，容易崩溃，采用多进程一个网页崩溃不会影响其他网页；而采用多线程会。②网页之间互相隔离，保证安全，不必担心某个网页中的恶意代码会取得存放在其他网页中的敏感信息。）
4.redis也可以归类到“多进程单线程”模型（平时工作是单个进程，涉及到耗时操作如持久化或aof重写时会用到多个进程）

多线程应用场景
线程间有数据共享，并且数据是需要修改的（不同任务间需要大量共享数据或频繁通信时）。
提供非均质的服务（有优先级任务处理）事件响应有优先级。
单任务并行计算，在非CPU Bound的场景下提高响应速度，降低时延。
与人有IO交互的应用，良好的用户体验（键盘鼠标的输入，立刻响应）
案例：
桌面软件，响应用户输入的是一个线程，后台程序处理是另外的线程；

## 联合体对齐规则

一、什么是联合体？

在C语言中，变量的定义是分配存储空间的过程。一般的，每个变量都具有其独有的存储空间，那么可不可以在同一个内存空间中存储不同的数据类型（不是同事存储）呢？

答案是可以的，使用`联合体`就可以达到这样的目的。`联合体也叫共用体`，在C语言中定义联合体的关键字是`union`。

定义一个联合类型的一般形式为：

```
union 联合名
{
成员表
};
```

成员表中含有若干成员，成员的一般形式为：`类型说明符 成员名`。其占用的字节数与成员中最大数据类型占用的字节数。

与`结构体（struct）`、`枚举（enum）`一样，联合体也是一种构造类型：

![图片](https://mmbiz.qpic.cn/mmbiz_png/PnO7BjBKUz9NB0qeH3xWIibibGqL4lz9TEt0y5ZakibDNrDTRDa8XGhmcJID5OszHCpJ8um4zjT3WfX6xtbhhv4sg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

二、联合体变量的定义方法

创建联合和创建结构的方式相同，需要一个联合模板和联合变量。下面是几种定义联合体变量的方法：

方法一：先创建模板再定义变量

```c
// 创建联合体模板union perdata
union perdata
{
    int Class;
    char Office;
};
// 使用该联合体模板创建两个变量a, b
union perdata a,b;
```

此处，`perdata`是联合体名，该名字是由我们任意定的，但是尽量起个有意义的名称。其相当于一个模板，可以使用这个模板去定义变量`a、b`。定义的时候不要忘了`union`。

方法二：同时创建模板和变量

```c
// 创建联合体模板union perdata的同时定义两个变量a、b
union perdata
{
    int Class;
    char Office;
}a,b;
```

方法三：省略联合体名

```
union
{
    int Class;
    char Office;
}a,b;
```

相对于方法一与方法二，此处省略了联合体名。虽然更简洁了，但是因为没有了名字，后面就不能用该联合体定义新的变量。

方法四：使用typedef

```c
// 联合体模板union perdata重新命名为perdata_U
typedef union perdata
{
    int Class;
    char Office;
}perdata_U;
// 使用新名字perdata_U创建两个变量a, b
perdata_U a,b;
```

此处使用`typedef`为联合体模板`union perdata`定义一个别名`perdata_U`。关于`typedef`关键字可查看往期笔记：

三、初始化联合体

联合体的初始化与结构体不同，联合体只能存储一个值。联合体有三种初始化方法：

```c
perdata_U a;
a.Class = 10;
perdata_U b = a;		/* 1、把一个联合初始化为另一个同类型的联合；*/
perdata_U c = {20};		/* 2、初始化联合的第一个成员；*/
perdata_U d = {.Office = 30};   /* 3、根据C99标准，使用指定初始化器。*/
```

四、联合体的应用示例

1、检测当前处理器是大端模式还是小端模式？

之前分享的[**《什么是大小端模式？**](http://mp.weixin.qq.com/s?__biz=MzU5MzcyMjI4MA==&mid=2247485077&idx=1&sn=390246ff62b559a60a98405bf8efaa21&chksm=fe0d6852c97ae1442e0987734df6c8b0a18582fc9504b4cbab0f3552657ff379838aea0d85a1&scene=21#wechat_redirect)[**》**](http://mp.weixin.qq.com/s?__biz=MzU5MzcyMjI4MA==&mid=2247485077&idx=1&sn=390246ff62b559a60a98405bf8efaa21&chksm=fe0d6852c97ae1442e0987734df6c8b0a18582fc9504b4cbab0f3552657ff379838aea0d85a1&scene=21#wechat_redirect)中已经有介绍怎么判断当前处理器的大小端问题：

![图片](https://mmbiz.qpic.cn/mmbiz_png/PnO7BjBKUz9NB0qeH3xWIibibGqL4lz9TEDvoHdAYUt7WopbIPSiaTickcPjSYvVPO1Q45aFRKRgSbpkRugDrlAYSw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

现在，可以使用联合体来做判断：

![图片](https://mmbiz.qpic.cn/mmbiz_png/PnO7BjBKUz9NB0qeH3xWIibibGqL4lz9TE5uPeXp5G8JEIAgia8djyu02H0Je1nnFjrhjorIChzSI7Hf2yVHdgh2A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

分离高低字节

单片机中经常会遇见分离高低字节的操作，比如进行计时中断复位操作时往往会进行

> (65535-200)/256,
>
> (65535-200)%256

这样的操作，而一个除法消耗四个机器周期，取余也需要进行一系列复杂的运算，如果在短时间内需要进行很多次这样的运算无疑会给程序带来巨大的负担。

其实进行这些操作的时候我们需要的仅仅是高低字节的数据分离而已，这样利用联合体我们很容易降低这部分开销。

3、寄存器封装

看看`TI`固件库中寄存器是怎么封装的：

![图片](https://mmbiz.qpic.cn/mmbiz_png/PnO7BjBKUz9NB0qeH3xWIibibGqL4lz9TE95STFX2KP4KibicVqHBoWBpBB88osp8ECylB44BCEn7ibFnSQkZCS6Ttg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/mmbiz_png/PnO7BjBKUz9NB0qeH3xWIibibGqL4lz9TEiaNxVtnWwxsEWTaVqlQQLRUDZlBCe5Ex6ROULGiaF9J55D6TlkrojsNg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

所有的寄存器被封装成联合体类型的变量，联合体里边的成员是一个`32bit`的整数及一个结构体，该结构体以位域的形式体现。这样就可以达到直接操控寄存器的某些位了。

比如，我们要设置`PA0`引脚的`GPAQSEL1`寄存器的`[1:0]`两位都为1，则我们只操控两个`bit`就可以很方便的这么设置：

```c
GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 3
```

或者直接操控整个寄存器：

```c
GpioCtrlRegs.GPAQSEL1.all |=0x03
```



## 结构体对齐规则

结构体：
结构体（struct）是由一系列具有相同类型或不同类型的数据构成的集合。因为这一特性，方便了开发者在使用的过程中可以将需要的不同的数据类型放在一起，做成我们需要的数据类型，通过结构体封装一些属性来组成新的类型。在我们实现链表的时候也很好的体现了这一点

但是，结构体在使用的过程中需要格外的注意！！！

结构体的大小及对齐规则：
结构体的大小不是结构体体中数据简单的相加，因为我们主流的计算机使用的都是32bit字长的CPU，对这类型的CPU取4个字节的数要比取一个字节要高效、方便。这就是内存对齐的由来。
每个平台上的编译器都有自己的默认“对齐系数”(也叫对齐模数)。开发者也可以通过预编译命令#pragma pack(n)，n=1,2,4,8,16来改变这一系数，n为指定的“对齐系数”。

结构体对齐规则：
1、结构体(struct)的数据成员，第一个数据成员放在offset为0的位置，以后每个数据成员在对齐数的整数倍的位置。
对齐数=编译器默认的对齐数与该成员大小的较小值。
VS中默认对齐数为8，linux默认对齐数为4.
2、结构体总大小为最大对齐数（每个成员都有一个对齐数）的整数倍。
3、如果嵌套了结构体的情况，嵌套的结构体对齐到自己的最大对齐数的整数倍处，结构体的整体大小就是所有最大对齐数（含嵌套结构体的对齐数）的整数倍。

```c
struct A
{
	int a;
	char b;
	double c;
};
//sizeof(A)=16;

struct B
{
	char b;
	double c;
	int a;
};


//sizeof(B)=24
```

struct A和struct B里面的数据除了顺序不同，里面的数据是一样的，但是编译之后发现它们的结构体长度是不一样的。所以数据的顺序不同结构体的对齐方式也不同，结构体的大小自然不同

struct A ：a是4字节，b是1字节，不符合对齐规则，所以b自动补全3个字节刚好和a加起来是8字节完成对齐，c是8字节，所以加起来是16字节

struct B ：b是一个字节，不符合对齐规则，不全7个字节完成对齐，c是8个字节，b是4个字节，不符合对齐规则，不全4个字节完成对齐，所以struct B的大小是24
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190728183940685.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDE4NTA5Nw==,size_16,color_FFFFFF,t_70)

## [踩内存](https://blog.csdn.net/u014485485/article/details/104714934/)

### 1.什么是踩内存

访问了不合法的地址 。通俗一点就是访问了不属于自己的地址。如果这块地址分配给了另一个变量使用，就会破坏别人的数据。从而导致程序运行异常，挂死，输出图像破图等。

### 2.踩内存的可能的情形

**1）内存访问越界**

a)数组访问越界；

b)字符串操作越界；

**2）非法指针**

a) 使用了空指针；

b) 使用了释放掉的指针；

c) 指针类型转换错误；

**3）栈溢出；**

**4）多线程读写的数据没有加锁保护**

**5）多线程使用了线程不安全的函数**



### 3.如何排查

1）删减模块代码，缩小排查范围；

2）加log，打印内存地址/值；

3）将被踩内存设置成只读，在调用栈中查看谁在写；

还有一些结合内核分析的高级手段，待学习

4.如何避免
写代码时严格遵守编程规范。

1）数组访问边界检查，防止溢出，关于数组大小等关键数值使用宏代替；

2）使用strcpy,strcat,sprintf,strcmp,strcasecmp等字符串操作函数需谨慎，可以以使用strncpy,strlcpy,strncat,strlcat,snprintf,strncmp,strncasecmp等代替防止读写越界；

memcpy在内存重叠时不保证拷贝是正确的，memmove在内存重叠时保证拷贝是正确的；

3）指针释放后及时置空，必要时使用指针非空检查；

4）不要定义过大的局部变量，例如数组，容易造成栈溢出；

5）变量定义好后及时赋初值，特别像结构体这种；

6）代码嵌套适量，防止资源重复申请/释放；

7）多线程时做好线程保护；

 

踩内存定位的成本很高。一旦写出内存踩踏的代码，排查的成本是按规范写代码的成本的1000x。



下面关于堆，栈的说法错误的是（）堆容易产生内存碎片

栈简介
栈由操作系统（编译器）自动分配释放 ，用于存放函数的参数值、局部变量等，其操作方式类似于数据结构中的栈。

堆简介
堆由开发人员分配和释放，容易产生内存碎片（不连续的无法利用的地址空间）。 若开发人员不释放，程序结束时由 OS（操作系统） 回收，分配方式类似于链表。

堆与栈区别
堆与栈实际上是操作系统对进程占用的内存空间的两种管理方式，主要有如下几种区别：
（1）管理方式不同。栈由操作系统自动分配释放，无需我们手动控制；堆的申请和释放工作由程序员控制，容易产生内存泄漏；

（2）空间大小不同。每个进程拥有的栈的大小要远远小于堆的大小。理论上，程序员可申请的堆大小为虚拟内存的大小，进程栈的大小 64bits 的 Windows 默认 1MB，64bits 的 Linux 默认 10MB；

（3）生长方向不同。堆的生长方向向上，内存地址由低到高；栈的生长方向向下，内存地址由高到低。

（4）分配方式不同。堆都是动态分配的，没有静态分配的堆。栈有2种分配方式：静态分配和动态分配。静态分配是由操作系统完成的，比如局部变量的分配。动态分配由alloca函数进行分配，但是栈的动态分配和堆是不同的，他的动态分配是由操作系统进行释放，无需我们手工实现。

（5）分配效率不同。栈效率高，堆效率低。 栈由系统自动分配，速度较快。但程序员是无法控制。堆分配的内存速度较慢，不易产生内存碎片栈由操作系统自动分配，会在硬件层级对栈提供支持：分配专门的寄存器存放栈的地址，压栈出栈都有专门的指令执行，这就决定了栈的效率比较高。堆则是由C/C++提供的库函数或运算符来完成申请与管理，实现机制较为复杂，频繁的内存申请容易产生内存碎片。显然，堆的效率比栈要低得多。

（6）存放内容不同。
栈存放的内容，函数返回地址、相关参数、局部变量和寄存器内容等。
堆，一般情况堆顶使用一个字节的空间来存放堆的大小，而堆中具体存放内容是由程序员来填充的。

C程序的存储空间是如何分配

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210224203513967.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDkzMzQxOQ==,size_16,color_FFFFFF,t_70)

### 第二题

![在这里插入图片描述](https://img-blog.csdnimg.cn/2021022421443040.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDkzMzQxOQ==,size_16,color_FFFFFF,t_70)

死锁：
在线程间共享多个资源的时候，如果两个线程分别占有一部分资源并且同时等待对方的资源，就会造成死锁，尽管死锁很少发生，但一旦发生就会造成应用的停止响应。
产生死锁的四个必要条件：
1、互斥条件：一个资源每次只能被一个进程使用。
2、请求与保持条件：一个进程因请求资源而阻塞时，对已获得的资源保持不放。
3、不剥夺条件:进程已获得的资源，在末使用完之前，不能强行剥夺。
4、循环等待条件:若干进程之间形成一种头尾相接的循环等待资源关系。
预防死锁
1、有序分配锁资源可以预防死锁

2、银行家算法避免死锁（不是检测）
银行家算法是从当前状态出发，逐个按安全序列检查各客户谁能完成其工作，然后假定其完成工作且归还全部贷款，再进而检查下一个能完成工作的客户，…。如果所有客户都能完成工作，则找到一个安全序列，银行家才是安全的。
解除死锁
剥夺死锁进程的所有资源可以解除死锁

有时，一个线程需要同时访问两个或更多不同的共享资源，而每个资源又都由不同的互斥量管理。当超过一个线程加锁同一组互斥量时，就有可能发生死锁；

两个或两个以上的进程在执行过程中，因争夺共享资源而造成的一种互相等待的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁。

死锁的几种场景：

- 忘记释放锁
- 重复加锁（重复加相同的锁）
- 多线程多锁，抢占锁资源

![img](https://img.jbzj.com/file_images/article/202205/202205040858131.png)

```c
//多线程多锁，抢占锁资源
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// 创建2个互斥量
pthread_mutex_t mutex1, mutex2;
void * workA(void * arg) {
    pthread_mutex_lock(&mutex1);
    sleep(1);
    pthread_mutex_lock(&mutex2);
    printf("workA....\n");
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}
void * workB(void * arg) {
    pthread_mutex_lock(&mutex2);
    sleep(1);
    pthread_mutex_lock(&mutex1);
    printf("workB....\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}
int main() {
    // 初始化互斥量
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    // 创建2个子线程
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, workA, NULL);
    pthread_create(&tid2, NULL, workB, NULL);
    // 回收子线程资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    // 释放互斥量资源
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    return 0;
}
```



```C
/*
    读写锁的类型 pthread_rwlock_t
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
    案例：8个线程操作同一个全局变量。
    3个线程不定时写这个全局变量，5个线程不定时的读这个全局变量
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// 创建一个共享数据
int num = 1;
// pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
void * writeNum(void * arg) {
    while(1) {
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++write, tid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}
void * readNum(void * arg) {
    while(1) {
        pthread_rwlock_rdlock(&rwlock); 
        printf("===read, tid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}
int main() {
   pthread_rwlock_init(&rwlock, NULL);
    // 创建3个写线程，5个读线程
    pthread_t wtids[3], rtids[5];
    for(int i = 0; i < 3; i++) {
        pthread_create(&wtids[i], NULL, writeNum, NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&rtids[i], NULL, readNum, NULL);
    }
    // 设置线程分离
    for(int i = 0; i < 3; i++) {
       pthread_detach(wtids[i]);
    }
    for(int i = 0; i < 5; i++) {
         pthread_detach(rtids[i]);
    }
    pthread_exit(NULL);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
```

实现void* memcpy(void *dst,const void *src,int size)

```c
#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<assert.h>
 
void* my_memcpy(void* dest, const void* src, size_t num)
{
	assert(dest != NULL);
	assert(src != NULL);
	while (num--)
	{
		*(char*)dest = *(char*)src;
		dest+=1;
		src+=1;
	}
	return dest;
}
int main()
{
	int i = 0;
	int arr1[] = { 1,2,3,4,5 };
	int arr2[5] = { 0 };
	my_memcpy(arr2, arr1, sizeof(arr1));
	for (i = 0; i < 5;i++)
	{
		printf("%d\n", arr2[i]);
	}
	return 0;
}
```

c语言中static关键字用法：

1、static+局部变量：静态局部变量使用static修饰符定义，即使在声明时未赋初值，编译器也会把它初始化为0。且静态局部变量存储于进程的全局数据区，即使函数返回，它的值也会保持不变。

2、static+全局变量：普通全局变量对整个工程可见，其他文件可以使用extern外部声明后直接使用。也就是说其他文件不能再定义一个与其相同名字的变量了（否则编译器会认为它们是同一个变量）。
静态全局变量仅对当前文件可见，其他文件不可访问，其他文件可以定义与其同名的变量，两者互不影响。

3、static+函数

静态函数只能在声明它的文件中可见，其他文件不能引用该函数
不同的文件可以使用相同名字的静态函数，互不影响

# [c语言中static关键字用法详解](https://blog.csdn.net/guotianqing/article/details/79828100)

static关键字在c语言中比较常用，使用恰当能够大大提高程序的模块化特性，有利于扩展和维护。

但是对于c语言初学者，static由于使用灵活，并不容易掌握。本文就static在c语言中的应用进行总结，供参考使用。错漏之处，请不吝指正。

最后一节加入了c++面向对象中static的使用特性，当作拓展阅读。


在程序中使用static
变量
1. 局部变量
普通局部变量是再熟悉不过的变量了，在任何一个函数内部定义的变量（不加static修饰符）都属于这个范畴。编译器一般不对普通局部变量进行初始化，也就是说它的值在初始时是不确定的，除非对其显式赋值。

>  普通局部变量存储于进程栈空间，使用完毕会立即释放。


静态局部变量使用static修饰符定义，即使在声明时未赋初值，编译器也会把它初始化为0。且静态局部变量存储于进程的全局数据区，即使函数返回，它的值也会保持不变。

> 变量在全局数据区分配内存空间；编译器自动对其初始化
> 其作用域为局部作用域，当定义它的函数结束时，其作用域随之结束

小程序体会一下静态局部变量的威力：

```c
#include <stdio.h>

void fn(void)
{
    int n = 10;

    printf("n=%d\n", n);
    n++;
    printf("n++=%d\n", n);
}

void fn_static(void)
{
    static int n = 10;

    printf("static n=%d\n", n);
    n++;
    printf("n++=%d\n", n);
}

int main(void)
{
    fn();
    printf("--------------------\n");
    fn_static();
    printf("--------------------\n");
    fn();
    printf("--------------------\n");
    fn_static();

    return 0;
}
```

```
-> % ./a.out 
n=10
n++=11
--------------------
static n=10
n++=11
--------------------
n=10
n++=11
--------------------
static n=11
n++=12
```

2. 全局变量
全局变量定义在函数体外部，在全局数据区分配存储空间，且编译器会自动对其初始化。

普通全局变量对整个工程可见，其他文件可以使用extern外部声明后直接使用。也就是说其他文件不能再定义一个与其相同名字的变量了（否则编译器会认为它们是同一个变量）。

静态全局变量仅对当前文件可见，其他文件不可访问，其他文件可以定义与其同名的变量，两者互不影响。

在定义不需要与其他文件共享的全局变量时，加上static关键字能够有效地降低程序模块之间的耦合，避免不同文件同名变量的冲突，且不会误使用。

函数
函数的使用方式与全局变量类似，在函数的返回类型前加上static，就是静态函数。其特性如下：
静态函数只能在声明它的文件中可见，其他文件不能引用该函数
不同的文件可以使用相同名字的静态函数，互不影响
非静态函数可以在另一个文件中直接引用，甚至不必使用extern声明

下面两个文件的例子说明使用static声明的函数不能被另一个文件引用：

```c
/* file1.c */
#include <stdio.h>

static void fun(void)
{
    printf("hello from fun.\n");
}

int main(void)
{
    fun();
    fun1();

    return 0;
}

/* file2.c */
#include <stdio.h>

static void fun1(void)
{
    printf("hello from static fun1.\n");
}

```

使用 `gcc file1.c file2.c`编译时，错误报告如下：

```c
/tmp/cc2VMzGR.o：在函数‘main’中：
static_fun.c:(.text+0x20)：对‘fun1’未定义的引用
collect2: error: ld returned 1 exit status
```

修改文件，不使用static修饰符，可在另一文件中引用该函数：

```c
/* file1.c */
#include <stdio.h>

void fun(void)
{
    printf("hello from fun.\n");
}

/* file2.c */
int main(void)
{
    fun();

    return 0;
}
```

同样使用 `gcc file1.c file2.c`编译，编译通过，运行结果如下：

```
-> % ./a.out 
hello from fun.
```

## 4:下列那一种文件物理结构只能顺序存取

一、顺序结构（顺序存取和随机存取）
优点
1、简单：存储与管理都简单，且容易实现。
2、支持顺序存取和随机存取。
3、顺序存取速度快。
4、所需的磁盘寻道次数和寻道时间最少。
缺点
1、需要为每个文件预留若干物理块以满足文件增长的部分需要。
2、不利于文件插入和删除。
二、链式结构（顺序存取）
优点
1、提高了磁盘空间利用率，不需要为每个文件预留物理块。
2、有利于文件插入和删除。
3、有利于文件动态扩充。
缺点
1、存取速度慢，不适于随机存取。
2、当物理块间的连接指针出错时，数据丢失。
3、更多的寻道次数和寻道时间。
4、链接指针占用一定的空间，降低了空间利用率。
三、索引结构（顺序存取和随机存取）
优点
1、不需要为每个文件预留物理块。
2、既能顺序存取，又能随机存取。
3、满足了文件动态增长、插入删除的要求。
缺点
1、较多的寻道次数和寻道时间。
2、索引表本身带来了系统开销。如：内外存空间，存取时间等
四、hash（哈希）文件（顺序存取和随机存取）
哈希文件中，是使用一个函数（算法）来完成一种将关键字映射到存储器地址的映射，根据用户给出的关键字，经函数计算得到目标地址，再进行目标的检索。

## 5: 不是引起操作系统重新选择进程的原因是() 有新进程进入就绪在状态

1.运行的时间片跑完

2.运行进程出错

3.运行进程要等待某一事件

## 6: 分时调度策略的是()SCHED_OTHER

我们在用pthread创建线程时，可以指定调度策略policy——SCHED_OTHER（默认）、SCHED_RR和SCHED_FIFO。这里TALK一下两个实时策略——SCHED_RR和SCHED_FIFO。

> [SCHED_RR与SCHED_FIFO调度策略不同点](https://www.freesion.com/article/6754347548/)

## 9: 初始化非法() A

const int ic = i

const int *pic = &ic

int i = 1

int *const cpi = &ic 

const的作用：
1、 修饰变量：
C语言中采用const修饰变量，功能是对变量声明为只读特性，并保护变量值以防被修改。举例说明如下：

```c
const int i = 5;
```

上面这个例子表明，变量i具有只读特性，不能够被更改；若想对i重新赋值，如i = 10；则是错误的。
2、 修饰数组：
C语言中const还可以修饰数组，举例如下：

```c
const int array[5] = {1,2,3,4,5};
array[0] = array[0]+1; //错误
```

数组元素与变量类似，具有只读属性，不能被更改；一旦更改，如程序将会报错。
3、 修饰指针：
C语言中const修饰指针要特别注意，共有两种形式，一种是用来限定指向空间的值不能修改；另一种是限定指针不可更改。举例说明如下：

```c
int i = 5;
int j = 6;
int k = 7;
const int * p1 = &i; //定义1 即其指向空间的值不可改变
int * const p2 =&j; //定义2 限定的是指针
```

上面定义了两个指针p1和p2。
在定义1中const限定的是p1，即其指向空间的值不可改变，若改变其指向空间的值如p1=20，则程序会报错；但p1的值是可以改变的，对p1重新赋值如p1=&k是没有任何问题的。
在定义2中const限定的是指针p2，若改变p2的值如p2=&k，程序将会报错；但p2，即其所指向空间的值可以改变，如p2=80是没有问题的，程序正常执行。
4、 修饰函数参数：
const关键字修饰函数参数，对参数起限定作用，防止其在函数内部被修改。所限定的函数参数可以是普通变量，也可以是指针变量。举例如下：

```c
void fun1(const int i)
{
其它语句
……
i++; //对i的值进行了修改，程序报错
其它语句
}

void fun2(const int *p)
{
其它语句
……
(*p)++; //对p指向空间的值进行了修改，程序报错
其它语句
}
```

## 10: 宏展开

```c
#define str(arg) #arg
#define name_y(y) name_y
#define adjoin(arg) name_##y
如下宏展开之后的结果是？
str(inter),name_y(1),adjoin(hik)

inter,name_1,name_y
```

#define 的作用
在编译预处理时，对程序中所有出现的“宏名”，都用宏定义中的字符串去代换，这称为“宏代换”或“宏展开”。
在Ｃ或C++语言中，“宏”分为有参数和无参数两种。
（1） 无参宏定义
无参宏的宏名后不带参数。其定义的一般形式为：

#define 标识符 字符串

其中的“#”表示这是一条预处理命令。凡是以“#”开头的均为预处理命令。“define”为宏定义命令。“标识符”为所定义的宏名。“字符串”可以是常数、表达式、格式串等。
例如：

　　#define M (a+b)
（2） 无参宏定义
c语言允许宏带有参数。在宏定义中的参数称为形式参数，在宏调用中的参数称为实际参数。对带参数的宏，在调用中，不仅要宏展开，而且要用实参去代换形参。
带参宏定义的一般形式为：
#define 宏名(形参表) 字符串（在字符串中含有各个形参。）

带参宏调用的一般形式为：
宏名(形参表)

例如：

#define M(y) ((y)*(y)+3*(y)) /*宏定义*/
....
k=M(5); /*宏调用*/
....
在宏调用时，用实参5去代替形参y，经预处理宏展开后的语句为:
k=5*5+3*5
（3） 防止重复定义
#define 条件编译
头文件(.h)可以被头文件或C文件包含；
重复包含（重复定义）
由于头文件包含可以嵌套，那么C文件就有可能包含多次同一个头文件，就可能出现重复定义的问题的。
通过条件编译开关来避免重复包含（重复定义）
例如

#ifndef __headerfileXXX__

#define __headerfileXXX__
…
文件内容
…
#endif

宏定义中#与##的作用

<font color="red">**#是把宏参数转化为字符串的运算符，##是把两个宏参数连接的运算符。**</font>

```c
#define str(arg) #arg 则宏STR(inter)展开时为”inter”
#define name_y(y) name_y 则宏name_y(1)展开时仍为name_y
#define NAME(y) name_##y 则宏NAME(1)展开为name_1
#define DECLARE(name, type) typename##_##type##_type，
```

则宏DECLARE(val, int)展开为int val_int_type
注意事项
当宏参数是另一个宏的时候，需要注意的是凡宏定义里有用’#’或’##’的地方宏参数是不会再展开.
即， 只有当前宏生效, 参数里的宏！不！会！生！效 ！！！！
举例

```c
#define A          (2)
#define STR(s)     #s
#define CONS(a,b)  int(a##e##b)
printf("int max: %s\n",  STR(INT_MAX));    // INT_MAX# include<climits>
printf("%s\n", CONS(A, A));                // compile error --- int(AeA)
```

两句print会被展开为：

```c
printf("int max: %s\n","INT_MAX");
printf("%s\n", int(AeA));
```

解决方法：
解决这个问题的方法很简单. 加多一层中间转换宏. 加这层宏的用意是把所有宏的参数在这层里全部展开,

```c
#define A           (2)
#define _STR(s)     #s
#define STR(s)      _STR(s)          // 转换宏
#define _CONS(a,b)  int(a##e##b)
#define CONS(a,b)   _CONS(a,b)       // 转换宏

结果：
printf("int max: %s\n",STR(INT_MAX));
//输出为: int max:0x7fffffff
//STR(INT_MAX) -->  _STR(0x7fffffff) 然后再转换成字符串； 

printf("%d\n", CONS(A, A));
//输出为：200
//CONS(A, A) -->  _CONS((2), (2))  --> int((2)e(2))
```

