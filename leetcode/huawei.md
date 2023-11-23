# 真题:

有一个数组 a[N] 顺序存放 0 ~ N-1 ，要求每隔两个数删掉一个数，到末尾时循环至开头继续进行，求最后一个被删掉的数的原始下标位置。以 8 个数 (N=7) 为例 :｛ 0，1，2，3，4，5，6，7 ｝，0 -> 1 -> 2 (删除) -> 3 -> 4 -> 5 (删除) -> 6 -> 7 -> 0 (删除),如此循环直到最后一个数被删除。

```python
while True:
    try:
        num = int(input())
        result = [i for i in range(num)]
        count = 1
        while len(result) != 1:
            temp = []
            for i in range(len(result)):
                if count%3!=0:
                    temp.append(result[i])
                count += 1
            result = temp
            print(result)
        print(result[0])
    except Exception:
        break
```

[0, 1, 3, 4, 6, 7]
[1, 3, 6, 7]
[3, 6]
[3, 6]
[6]
6

[0,1,2,3,4,5,6]

[0,1,3,4,6]

[0,3,4]

[0,3]

[3]

# 约瑟夫问题

约瑟夫问题是个著名的问题：N个人围成一圈，第一个人从1开始报数，报M的将被杀掉，下一个人接着从1开始报。如此反复，最后剩下一个，求最后的胜利者。
例如只有三个人，把他们叫做A、B、C，他们围成一圈，从A开始报数，假设报2的人被杀掉。

●首先A开始报数，他报1。侥幸逃过一劫。
●然后轮到B报数，他报2。非常惨，他被杀了
●C接着从1开始报数
●接着轮到A报数，他报2。也被杀死了。
●最终胜利者是C
$$
f(N,M)=(f(N−1,M)+M)\%N
$$
●f(N,M)表示，N个人报数，每报到M时杀掉那个人，最终胜利者的编号
●f(N−1,M)表示，N-1个人报数，每报到M时杀掉那个人，最终胜利者的编号
下面我们不用字母表示每一个人，而用数字。
1、2、3、4、5、6、7、8、9、10、11
表示11个人，他们先排成一排，假设每报到3的人被杀掉。

●刚开始时，头一个人编号是1，从他开始报数，第一轮被杀掉的是编号3的人。

●编号4的人从1开始重新报数，这时候我们可以认为编号4这个人是队伍的头。第二轮被杀掉的是编号6的人。

●编号7的人开始重新报数，这时候我们可以认为编号7这个人是队伍的头。第三轮被杀掉的是编号9的人。

●……

●第九轮时，编号2的人开始重新报数，这时候我们可以认为编号2这个人是队伍的头。这轮被杀掉的是编号8的人。

●下一个人还是编号为2的人，他从1开始报数，不幸的是他在这轮被杀掉了。

●最后的胜利者是编号为7的人。

**将上面表格的每一行看成数组，这个公式描述的是：幸存者在这一轮的下标位置**
●f ( 1 , 3 )：只有1个人了，那个人就是获胜者，他的下标位置是0
●f ( 2 , 3 ) = ( f ( 1 , 3 ) + 3 ) % 2 = 3 % 2 = 1 ：在有2个人的时候，胜利者的下标位置为1
●f ( 3 , 3 ) = ( f ( 2 , 3 ) + 3 ) % 3 = 4 % 3 = 1 ：在有3个人的时候，胜利者的下标位置为1
●f ( 4 , 3 ) = ( f ( 3 , 3 ) + 3 ) % 4 = 4 % 4 = 0 ：在有4个人的时候，胜利者的下标位置为0
●……
f● ( 11 , 3 ) = 6 f(11,3)=6 f(11,3)=6

```python
def jsloop(n ,k):
    p = 0
    for i in range(2,n+1):
        p = (p + k) % i
    return p
```

# 素数之积

给定一个整数
$$
n \in (0, 2^{31}-1)
$$
，求该数是否为两素数之积。

```python
from math import sqrt

def isPrime(s):
    if (s <= 1):
        return False
    for i in range(2, int(sqrt(s))):
        if (s % i == 0):
            return False
    return True
def isPrimeable(n):
    a,b = 0,0
    for i in range(2,int(sqrt(n))):
        if isPrime(i):
            if (n % i == 0):
                if (isPrime(n / i)):
                    a = i
                    b = int(n / i)
                else:
                    continue
            else:
                continue
        else:
            continue
    return a,b
```

# 流水线

题目描述
一个工厂有 m 条流水线，来并行完成 n 个独立的作业，该工厂设置了一个调度系统，在安排作业时，总是优先执行处理时间最短的作业。现给定流水线个数 m，需要完成的作业数 n，每个作业的处理时间分别为 t1，t2…tn。请你编程计算处理完所有作业的耗时为多少？当 n ＞ m 时，首先处理时间短的 m 个作业进入流水线，其他的等待，当某个作业完成时，依次从剩余作业中取处理时间最短的进入处理。

输入描述

第一行为 2 个整数（采取空格分隔），分别表示流水线个数 m 和作业数 n。第二行输入 n 个整数（采取空格分福），表示每个作业的处理时长 t1，t2…tn。0<m, n＜100，0<t1,t2...tn<100

输出描述

输出处理完所有作业的总时长输入

示例

输入：
3 5
8 4 3 2 10

输出
13

说明
先安排时间为 2,3, 4 的 3 个作业。第一条流水线先完成作业，然后调度剩余时间最短的作业 8。第二条流水线完成作业，然后调度剩余时间最短的作业 10。总共耗时就是第二条流水线完成作业时间 13(3+10)

```python
while 1:
        try:
            m=3
            n=5
            l=[8 ,4 ,3 ,2 ,10]
            l.sort()
 
            if m>=n:
                print(max(l))
            else:
                ll=l[:m]
                l=l[m:]
 
                while len(l)>0:
                    tmp=min(ll)
                    ll[ll.index(tmp)]+=l[0]
                    l.remove(l[0])
 
            print(max(ll))
 
            break
 
        except Exception as e:
            print(e)
            break
```



# 完全波形信号

给定一个由 0 和 1 组成的字符串。一个信号定义为由 0 开头和结尾，中间不会有连续两个 0 的一个子串。求输入中最长的 0、1 交替的波形信号。

直接正则替换，能一行解决的绝对不写第二行

```python
import re

s = '0' + input() + '0'

waves = re.sub(r'0{2,}', '0|0', s).split('|')[1:-1]

ans = ''

for w in waves:
    if all(w[i] != w[i + 1] for i in range(len(w) - 1)):
        ans = max(ans, w, key=len)

print(ans or -1)
```



# 哥哥弟弟分玩具 弟弟计算玩具重量方法是不带进位的二进制加法 要求兄弟所得玩具重量在弟弟眼中相同 求哥哥最大玩具重量

input:
3            // 一共三个玩具
3 5 6        // 重量分别是 3 5 6
output：
11           // 弟弟眼中 5 + 6 -> 101 + 110 = 11 == 3 所以哥哥最多拿 5 + 6 = 11

```python
cnt = int(input())
weights = list(map(int, input().split()))

if reduce(lambda x, y: x ^ y, weights) != 0:
    print(-1)
else:
    print(sum(weights) - min(weights))

```

第三题其实看到不带进位的二进制和很容易想到异或
然后我的做法是遍历 range(1 << cnt) 分配玩具
每一种状态分别对应一种玩具分配方法
例如 110011 就是第0、1、4、5个玩具给一个人 2、3给另一个人
做完优化的时候发现其实玩具重量相等可以互换实际上只需要遍历一半就OK
这时候突然想到既然必须要求所有玩具异或和为0 否则无法分配
异或性质
a = b -> a ^ c = b ^ c
0 ^ c = c
故在玩具可分配情况下 单拿出任何一个玩具 剩下玩具的异或和与此玩具相等
那只要挑个垃圾给弟弟就行了
位运算真神奇
开始的代码大概是这样的

```python
cnt = int(input())
weights = list(map(int, input().split()))

try:
    if reduce(lambda x, y: x ^ y, weights) != 0:
        print(-1)
        raise
    ans = 0
    for i in range(1, (1 << cnt) - 1):
        <!-- 哥哥叫solo 弟弟叫啥忘了 -->
        solo0 = solo1 = coco0 = coco1 = 0
        for j in range(cnt):
            if i & 1 == 1:
                solo1 += weights[j]
                coco1 ^= weights[j]
            else:
                solo0 += weights[j]
                coco0 ^= weights[j]
        if coco0 == coco1:
            ans = max(ans, solo0, solo1)
    print(ans)
```

# 字符串分割-水仙花数

题目描述
给定非空字符串s，将该字符串分割成一些子串，使每个子串的ASCII码值的和均为水仙花数。
1、若分割不成功，则返回0
2、若分割成功且分割结果不唯一，则返回-1
3、若分割成功且分割结果唯一，则返回分割后子串的数目

输入描述
输入字符串的最大长度为200

输出描述
根据题目描述的情况，返回相应的结果

备注
“水仙花数”是指一个三位数，每位上数字的立方和等于该数字本身，如371是'水仙花数'，因371=3^3+7^3+1^3

```python
#以下代码用例通过率为：11.1%
s = input()
l = []
tem = []
c = []
for i in s:
    l.append(ord(i))
for i in range(len(s)):
    for j in range(i,len(s)+1):
        if l[i:j] not in tem and len(str(sum(l[i:j]))) == 3:
            tem.append(str(sum(l[i:j])))
for i in tem:
    if int(i[0])**3+int(i[1])**3+int(i[2])**3 == int(i):
        c.append(int(i))
if len(c) == 0:
    print(0)
else:
    if sum(l) == sum(c):
        print(len(c))
    else:
        print(-1)
```

# 用连续自然数之和来表达整数

作者：牛客614273554号
链接：https://www.nowcoder.com/discuss/625512

1.输入正整数T，输出T可以由连续自然数的和表示的所有情况及其数量。  

 如输入：9 

 输出： 

 9=9

 9=4+5

 9=2+3+4

 Result=3 

解释：9可以由3种形式的连续自然数的和表示，Result为3。 

读了一下题目，说是给一个数，让你输出它的所有可由联系自然数的和表示的情况，和数量。 

根据题目中给出的输出样例，如果存在多组输出的话，应该是按照数字个数从少到多来输出，最后再补充一个 Result=X，其中X代表有多少组。 

为此，我好好回忆了一把高中数学学习序列的时候，如果从m 一直加连续的数，加到n，那么结果是“首尾相加 乘以 个数 除以 2” 
$$
t = (m+n)(n-m+1)\div2
$$
如果 改为： 从 m开始加，一共加 i个数，结果是： 开始加，一共加 i个数，结果是
$$
t=(m+0)+(m+1)+(m+2)+……+(m+i−1)\\
=(m+m+i−1)×(m+i−1−m+1)÷2\\
=(2×m+i−1)×i÷2
$$
于是，根据这个公式，反推出 m，得到：
$$
m=(2×t÷i−i+1)÷2
$$
于是，我让i 从 2 开始 （没必要从1开始，因为1个数相加得到t的话，那么 m一定等于 t ）,只要能让m是个整数，没有小数点，那就是满足条件的解。 

m是整数的条件是 里面所有除法都能整数，包括： 
$$
2×t÷i\\
(2×t+i−1)×i÷2
$$
  如果满足，则 t = m+(m+1)+(m+2)+……+(m+i-1) 

  于是代码就出来了：

```python
while 1:
    try:
        num = int(input())
        
        dp = [[num]]

        def dfs(n):
            total = sum(n)
            if total == num:
                dp.append(n)
            elif total > num:
                return
            else:
                dfs(n + [n[-1]+1])

        for i in range(1, (num//2 + 1)):
            dfs([i])

        print(len(dp))
    except Exception as e:
        break
```

```python
while True:
    try:
        t = int(input())
        cnt = 1
        i = 2
        print("{}={}".format(t,t))
        while (i*(i-1)/2<t):
            m = 0
            if (2 * t % i == 0 and (2 * t / i - i + 1) % 2 == 0):
                m = int((2 * t / i - i + 1) / 2)
                cnt += 1
                print("{}={}".format(t,"+".join([str(m+j) for j in range(i)])))
            i += 1
        print(cnt)
    except:
        break
```

```c++
#include<iostream>
using namespace std;
int main() 
{
    int t;
    cin >> t;

    //首先将 t=t打印出来
    cout << t << "=" << t << endl;

    int cnt = 1;
    // i 表示组成t的连续的数字的个数，从 2个开始，因为1个已经打印过了。
    for (int i = 2; i *(i - 1)/2 < t ; i++)
    {
        int m = 0;
        /*
        根据数学公式，从m 连续加到n，结果是 t = (m+n)(n-m+1)/2，
        如果从 m 开始 加 i个数，即从 m 加到 m+i-1 ，得到 t = (2m+i-1)*i/2
        因此推导出， m = (2*t/i - i + 1 )/ 2 , 
        如果 m的计算公式中， 2*t/i 没有出现小数点，且 m也没出现小数点，则这就是一个可用的解
        */
        if (2 * t % i == 0 && (2 * t / i - i + 1) % 2 == 0) 
        {
            m = (2 * t / i - i + 1) / 2;
            cnt++;
            cout << t << "=";
            for (int j = 1; j < i; j++)
                cout << m++ << "+";
            cout << m << endl;
        }
    }

    cout << "Result:" << cnt;
    return 0;
}
```

**HJ19** **简单错误记录**

开发一个简单错误记录功能小模块，能够记录出错的代码所在的文件名称和行号。
处理:
1.记录最多8条错误记录，对相同的错误记录(即文件名称和行号完全匹配)只记录一条，错误计数增加；(文件所在的目录不同，文件名和行号相同也要合并)
2.超过16个字符的文件名称，只记录文件的最后有效16个字符；(如果文件名不同，而只是文件名的后16个字符和行号相同，也不要合并)

3、 输入的文件可能带路径，记录文件名称不能带路径。**也就是说，哪怕不同路径下的文件，如果它们的名字的后16个字符相同，也被视为相同的错误记录**

4、循环记录时，只以第一次出现的顺序为准，后面重复的不会更新它的出现时间，仍以第一次为准

输入描述：

每组只包含一个测试用例。一个测试用例包含一行或多行字符串。每行包括带路径文件名称，行号，以空格隔开。

输出描述：

将所有的记录统计并将结果输出，格式：文件名 代码行数 数目，一个空格隔开，如：

示例1

输入：

```
D:\zwtymj\xccb\ljj\cqzlyaszjvlsjmkwoqijggmybr 645
E:\je\rzuwnjvnuz 633
C:\km\tgjwpb\gy\atl 637
F:\weioj\hadd\connsh\rwyfvzsopsuiqjnr 647
E:\ns\mfwj\wqkoki\eez 648
D:\cfmwafhhgeyawnool 649
E:\czt\opwip\osnll\c 637
G:\nt\f 633
F:\fop\ywzqaop 631
F:\yay\jc\ywzqaop 631
D:\zwtymj\xccb\ljj\cqzlyaszjvlsjmkwoqijggmybr 645
```

输出：

```
rzuwnjvnuz 633 1
atl 637 1
rwyfvzsopsuiqjnr 647 1
eez 648 1
fmwafhhgeyawnool 649 1
c 637 1
f 633 1
ywzqaop 631 2
```

说明：

```
由于D:\cfmwafhhgeyawnool 649的文件名长度超过了16个字符，达到了17，所以第一个字符'c'应该被忽略。
记录F:\fop\ywzqaop 631和F:\yay\jc\ywzqaop 631由于文件名和行号相同，因此被视为同一个错误记录，哪怕它们的路径是不同的。
由于循环记录时，只以第一次出现的顺序为准，后面重复的不会更新它的出现时间，仍以第一次为准，所以D:\zwtymj\xccb\ljj\cqzlyaszjvlsjmkwoqijggmybr 645不会被记录。  
```

```python
la = []
d = {}
while True:
    try:
        ss = input().split()
        a = (ss[0].split('\\')[-1][-16:],ss[1])
        if a not in la:
            la.append(a)
            d[a] = 1
        else:
            d[a] += 1
    except:
        break
for i in la[-8:]:
    print(i[0],i[1],d[i])
```

# **HJ88** **扑克牌大小**

扑克牌游戏大家应该都比较熟悉了，一副牌由54张组成，含3~A，2各4张，小王1张，大王1张。牌面从小到大用如下字符和字符串表示（其中，小写joker表示小王，大写JOKER表示大王）:)
3 4 5 6 7 8 9 10 J Q K A 2 joker JOKER
输入两手牌，两手牌之间用“-”连接，每手牌的每张牌以空格分隔，“-”两边没有空格，如：4 4 4 4-joker JOKER
请比较两手牌大小，输出较大的牌，如果不存在比较关系则输出ERROR

基本规则：
（1）输入每手牌可能是个子，对子，顺子（连续5张），三个，炸弹（四个）和对王中的一种，不存在其他情况，由输入保证两手牌都是合法的，顺子已经从小到大排列；
（2）除了炸弹和对王可以和所有牌比较之外，其他类型的牌只能跟相同类型的存在比较关系（如，对子跟对子比较，三个跟三个比较），不考虑拆牌情况（如：将对子拆分成个子）
（3）大小规则跟大家平时了解的常见规则相同，个子，对子，三个比较牌面大小；顺子比较最小牌大小；炸弹大于前面所有的牌，炸弹之间比较牌面大小；对王是最大的牌；
（4）输入的两手牌不会出现相等的情况。

答案提示：
（1）除了炸弹和对王之外，其他必须同类型比较。
（2）输入已经保证合法性，不用检查输入是否是合法的牌。

（3）输入的顺子已经经过从小到大排序，因此不用再排序了.

数据范围：保证输入合法

```python
#Python版
#一楼正解， 只能是牌中的一种。
# -*- coding:utf-8 -*-
import sys

if __name__ == '__main__':
    while True:
        tp = sys.stdin.readline().strip()
        if not tp:
            break
        if 'joker JOKER' in tp:
            print 'joker JOKER'
        else:
            strs = ['3','4','5','6','7','8','9','10','J','Q','K','A','2','joker','JOKER']
            s1,s2 = tp.split('-')
            ss1 = s1.split(' ')
            ss2 = s2.split(' ')
            len1 = len(ss1)
            len2 = len(ss2)
            if len1 ==4 and len2!=4:
                print s1
            elif len2==4 and len1!=4:
                print s2
            elif len1==len2:
                ind1 = strs.index(ss1[0])
                ind2 = strs.index(ss2[0])
                if ind1>ind2:
                    print s1
                elif ind1<ind2:
                    print s2
            else:
                    print 'ERROR'

```

# **HJ90** **合法IP**

IPV4地址可以用一个32位无符号整数来表示，一般用点分方式来显示，点将IP地址分成4个部分，每个部分为8位，表示成一个无符号整数（因此正号不需要出现），如10.137.17.1，是我们非常熟悉的IP地址，一个IP地址串中没有空格出现（因为要表示成一个32数字）。

现在需要你用程序来判断IP是否合法。

数据范围：数据组数：
$$
1\le t\le 18
$$
进阶：时间复杂度：O(n)\*O*(*n*) ，空间复杂度：O(n)\*O*(*n*) 

```python
while True:
    try:
        ip=input().split(".")
        isValid=True
        if len(ip) != 4 or '' in ip:
            print("NO")
        else:
            for i in ip:
                if i == "0":
                    pass
                elif 0 <int(i)<= 255 and str(int(i)) == i:
                    pass
                else:
                    isValid=False
                    break
            print("YES" if isValid else "NO")
    except:
        break
```

# **HJ17** **坐标移动**

开发一个坐标计算工具， A表示向左移动，D表示向右移动，W表示向上移动，S表示向下移动。从（0,0）点开始移动，从输入字符串里面读取一些坐标，并将最终输入结果输出到输出文件里面。

输入：

合法坐标为A(或者D或者W或者S) + 数字（两位以内）

坐标之间以;分隔。

非法坐标点需要进行丢弃。如AA10; A1A; $%$; YAD; 等。

下面是一个简单的例子 如：

A10;S20;W10;D30;X;A1A;B10A11;;A10;

处理过程：

起点（0,0）

\+  A10  = （-10,0）

\+  S20  = (-10,-20)

\+  W10 = (-10,-10)

\+  D30 = (20,-10)

\+  x  = 无效

\+  A1A  = 无效

\+  B10A11  = 无效

\+ 一个空 不影响

\+  A10 = (10,-10)

结果 （10， -10）

$$
数据范围：每组输入的字符串长度满足  :1 \le n \le 10000 \\
-2^{31} \le x,y \le 2^{31} -1 且数字部分仅含正数 \\
$$


```python
while True:
    try:
        string = input().strip().split(';')
        start = [0, 0]
        for i in string:
            if not i: continue
            try:
                if i[0] == 'A':
                    start[0]-=int(float(i[1:]))
                if i[0] == 'D':
                    start[0]+=int(float(i[1:]))
                if i[0] == 'S':
                    start[1]-=int(float(i[1:]))
                if i[0] == 'W':
                    start[1]+=int(float(i[1:]))
            except:
                continue
        print("({},{})".format(start[0],start[1]))
    except:
        break
```

# **HJ20** **密码验证合格程序**

密码要求:

1.长度超过8位

2.包括大小写字母.数字.其它符号,以上四种至少三种

3.不能有长度大于2的包含公共元素的子串重复 （注：其他符号不含空格或换行）

数据范围：输入的字符串长度满足 
$$
1 \le n \le 100
$$

输入描述：

一组字符串。

输出描述：

如果符合要求输出：OK，否则输出NG

```
输入：
021Abc9000
021Abc9Abc1
021ABC9000
021$bc9000
复制
输出：
OK
NG
NG
OK
```

```python
def check(s):
    if len(s) <= 8:
        return 0
    a, b, c, d = 0, 0, 0, 0
    for item in s:
        if ord('a') <= ord(item) <= ord('z'):
            a = 1
        elif ord('A') <= ord(item) <= ord('Z'):
            b = 1
        elif ord('0') <= ord(item) <= ord('9'):
            c = 1
        else:
            d = 1
    if a + b + c + d < 3:
        return 0
    for i in range(len(s)-3):
        if len(s.split(s[i:i+3])) >= 3:
            return 0
    return 1

while 1:
    try:
        print('OK' if check(input()) else 'NG')
    except:
        break
```

# **HJ24** **合唱队**

N 位同学站成一排，音乐老师要请最少的同学出列，使得剩下的 K 位同学排成合唱队形。

设K*K*位同学从左到右依次编号为 1，2…，K ，他们的身高分别为T_1,T_2,…,T_K*T*1,*T*2,…,*T**K* ，若存在i(1\leq i\leq K)*i*(1≤*i*≤*K*) 使得T_1<T_2<......<T_{i-1}<T_i*T*1<*T*2<......<*T**i*−1<*T**i* 且 T_i>T_{i+1}>......>T_K*T**i*>*T**i*+1>......>*T**K*，则称这K*K*名同学排成了合唱队形。

通俗来说，能找到一个同学，他的两边的同学身高都依次严格降低的队形就是合唱队形。

例子：

xxxxxxxxxx /**  * of_get_mac_address - 从设备树中提取MAC地址 * @np - 设备节点指针 * @成功：MAC（6字节）的首地址；失败：NULL */void *of_get_mac_address(struct device_node *np);c

123 123 124 122不是合唱队形，因为前两名同学身高相等，不符合要求

123 122 121 122不是合唱队形，因为找不到一个同学，他的两侧同学身高递减。

你的任务是，已知所有N位同学的身高，计算最少需要几位同学出列，可以使得剩下的同学排成合唱队形。

**注意：不允许改变队列元素的先后顺序** **且** **不要求最高同学左右人数必须相等**

数据范围：
$$
1 \le n \le 3000
$$
输入：

```
8
186 186 150 200 160 130 197 200
```

输出：

```
4
```

说明：

```
由于不允许改变队列元素的先后顺序，所以最终剩下的队列应该为186 200 160 130或150 200 160 130           
```

# **HJ26** **字符串排序**

编写一个程序，将输入字符串中的字符按如下规则排序。

规则 1 ：英文字母从 A 到 Z 排列，不区分大小写。

如，输入： Type 输出： epTy

规则 2 ：同一个英文字母的大小写同时存在时，按照输入顺序排列。

如，输入： BabA 输出： aABb

规则 3 ：非英文字母的其它字符保持原来的位置。

如，输入： By?e 输出： Be?y

输入描述：

输入字符串

输出描述：

输出字符串

```
输入：
A Famous Saying: Much Ado About Nothing (2012/8).
输出：
A aaAAbc dFgghh: iimM nNn oooos Sttuuuy (2012/8).
```

```python
while True:
    try:
        s = input()
        a = ''
        for i in s:
            if i.isalpha():
                a += i
        b = sorted(a, key=str.upper)
        index = 0
        d = ''
        for i in range(len(s)):
            if s[i].isalpha():
                d += b[index]
                index += 1
            else:
                d += s[i]
        print(d)
    except:
        break
```

sorted(iterable,str.upper)就可以实现1）字符由A到Z的排序2）能够实现同字母（A与a算同字母），由输入先后书序排列。
str.isalpha实现的是如果字符串至少有一个字符并且所有字符都是字母则返回 True，否则返回 False。

# **HJ27** **查找兄弟单词**

定义一个单词的“兄弟单词”为：交换该单词字母顺序（注：可以交换任意次），而不添加、删除、修改原有的字母就能生成的单词。

兄弟单词要求和原来的单词不同。例如： ab 和 ba 是兄弟单词。 ab 和 ab 则不是兄弟单词。

现在给定你 n 个单词，另外再给你一个单词 x ，让你寻找 x 的兄弟单词里，按字典序排列后的第 k 个单词是什么？

注意：字典中可能有重复单词。

输入描述：

输入只有一行。 先输入字典中单词的个数n，再输入n个单词作为字典单词。 然后输入一个单词x 最后后输入一个整数k

输出描述：

第一行输出查找到x的兄弟单词的个数m 第二行输出查找到的按照字典顺序排序后的第k个兄弟单词，没有符合第k个的话则不用输出。

```
输入：
6 cab ad abcd cba abc bca abc 1
输出：
3
bca
说明：
abc的兄弟单词有cab cba bca，所以输出3
经字典序排列后，变为bca cab cba，所以第1个字典序兄弟单词为bca         
```

```python
while 1:
    try:
        s=input().split()
        l,x,k=s[1:len(s)-2],s[-2],int(s[-1])
        a=sorted(x)
        nl=[]
        for i in l:
            if sorted(i)==a and i!=x: #当字符串里元素与x相等，但又不等于x时，为兄弟元素
                nl.append(i)
        print(len(nl))
        if nl:
            print(sorted(nl)[k-1])
    except:
        break
```



# **HJ29** **字符串加解密**

对输入的字符串进行加解密，并输出。

加密方法为：

当内容是英文字母时则用该英文字母的后一个字母替换，同时字母变换大小写,如字母a时则替换为B；字母Z时则替换为a；

当内容是数字时则把该数字加1，如0替换1，1替换2，9替换0；

其他字符不做变化。

解密方法为加密的逆过程。

输入描述：

第一行输入一串要加密的密码
第二行输入一串加过密的密码

输出描述：

第一行输出加密后的字符
第二行输出解密后的字符

```python
def check(a,b):
    L1 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    L2 = "BCDEFGHIJKLMNOPQRSTUVWXYZAbcdefghijklmnopqrstuvwxyza1234567890"
    result = ""
    if b == 1:
        for i in a:
            result += L2[L1.index(i)]
    elif b == -1:
        for i in a:
            result += L1[L2.index(i)]
    return result
while True:
    try:
        print(check(input(), 1))
        print(check(input(),-1))
    except:
        break
```



# **HJ33** **整数与IP地址间的转换**

原理：ip地址的每段可以看成是一个0-255的整数，把每段拆分成一个二进制形式组合起来，然后把这个二进制数转变成一个长整数。
举例：一个ip地址为10.0.3.193
每段数字       相对应的二进制数
10          00001010
0            00000000
3            00000011
193        11000001

组合起来即为：00001010 00000000 00000011 11000001,转换为10进制数就是：167773121，即该IP地址转换后的数字就是它了。

数据范围：保证输入的是合法的 IP 序列

* 输入描述：

1 输入IP地址
2 输入10进制型的IP地址

* 输出描述：

1 输出转换成10进制的IP地址
2 输出转换后的IP地址

```python
def transfer_ip2num(ip):
    ip_list = ip.split('.')
    bin_str = ''
    for x in ip_list:
        bin_str += bin(int(x))[2:].rjust(8,'0')
    return int('0b'+ bin_str, 2)
 
def transfer_num2ip(num):
    result = ''
    bin_str = bin(int(num))[2:].rjust(32,'0')
    for idx in range(4):
        result = result + str(int('0b' + bin_str[idx*8:idx*8+8], 2)) + '.'
    return result[:-1]
 
if __name__ == '__main__':
    while True:
        try:
            print(transfer_ip2num(input()))
            print(transfer_num2ip(input()))
        except EOFError: break
```



# **HJ41** **称砝码**

描述

现有n种砝码，重量互不相等，分别为 m1,m2,m3…mn ；
每种砝码对应的数量为 x1,x2,x3...xn 。现在要用这些砝码去称物体的重量(放在同一侧)，问能称出多少种不同的重量。

注：

称重重量包括 0

输入描述：

对于每组测试数据：
第一行：n --- 砝码的种数(范围[1,10])
第二行：m1 m2 m3 ... mn --- 每种砝码的重量(范围[1,2000])
第三行：x1 x2 x3 .... xn --- 每种砝码对应的数量(范围[1,10])

输出描述：

利用给定的砝码可以称出的不同的重量数

示例1

输入：

```
2
1 2
2 1
```

输出：

```
5
```

说明：

```
可以表示出0，1，2，3，4五种重量。   
```

# **HJ45** **名字的漂亮度**

描述

给出一个字符串，该字符串仅由小写字母组成，定义这个字符串的“漂亮度”是其所有字母“漂亮度”的总和。
每个字母都有一个“漂亮度”，范围在1到26之间。没有任何两个不同字母拥有相同的“漂亮度”。字母忽略大小写。

给出多个字符串，计算每个字符串最大可能的“漂亮度”。

本题含有多组数据。

输入描述：

第一行一个整数N，接下来N行每行一个字符串

输出描述：

每个字符串可能的最大漂亮程度

输入：

```
2
zhangsan
lisi
```

输出：

```
192
101
```

说明：

```
对于样例lisi，让i的漂亮度为26，l的漂亮度为25，s的漂亮度为24，lisi的漂亮度为25+26+24+26=101.   
```

```python
while True:
    try:
        n = int(input())
        for i in range(n):
            each_name = input()
            beauty = 0
            
            # 字典放名字中每种字母对应出现到次数
            dict1 = {}
            for c in each_name:
                dict1[c] = each_name.count(c)
            # 每种字母的出现次数从大到小排列
            times_list = sorted(dict1.values(), reverse=True)
            
            # 次数从大到小以此乘以26,25,24...
            for j in range(len(times_list)):
                beauty += (26 - j) * times_list[j]
            print(beauty)
        
    except:
        break
```

# **HJ43** **迷宫问题**

描述

定义一个二维数组 N*M ，如 5 × 5 数组下所示：

int maze\[5][5] = {
0, 1, 0, 0, 0,
0, 1, 1, 1, 0,
0, 0, 0, 0, 0,
0, 1, 1, 1, 0,
0, 0, 0, 1, 0,
};


它表示一个迷宫，其中的1表示墙壁，0表示可以走的路，只能横着走或竖着走，不能斜着走，要求编程序找出从左上角到右下角的路线。入口点为[0,0],既第一格是可以走的路。

输入描述：

输入两个整数，分别表示二维数组的行数，列数。再输入相应的数组，其中的1表示墙壁，0表示可以走的路。数据保证有唯一解,不考虑有多解的情况，即迷宫只有一条通道。

输出描述：

左上角到右下角的最短路径，格式如样例所示。

示例1

输入：

```
5 5
0 1 0 0 0
0 1 1 1 0
0 0 0 0 0
0 1 1 1 0
0 0 0 1 0
```

输出：

```
(0,0)
(1,0)
(2,0)
(2,1)
(2,2)
(2,3)
(2,4)
(3,4)
(4,4)
```

```python
def dfs(matrix, x, y, path, dst):
    if (x,y) == dst:
        path.append((x,y))
        for i in path:
            print("({},{})".format(i[0],i[1]))
        return
     
    if not 0<=x<len(matrix) or not 0<=y<len(matrix[0]) \
    or matrix[x][y] == 1 or (x,y) in path:
        return
    
    path.append((x,y))
    for nx, ny in [[x+1,y], [x-1,y], [x,y+1], [x,y-1]]:
    	dfs(matrix, nx, ny, path, dst)
    path.pop()
while True:
    try:
        x, y = list(map(int, input().split()))
        dst = (x-1, y-1)
        matrix = []
        for i in range(x):
            matrix.append(list(map(int, input().split())))
        dfs(matrix, 0, 0, [], dst)
    except:
        break
```



# **HJ67** **24点游戏算法**

描述

给出4个1-10的数字，通过加减乘除运算，得到数字为24就算胜利,除法指实数除法运算,运算符仅允许出现在两个数字之间,本题对数字选取顺序无要求，但每个数字仅允许使用一次，且需考虑括号运算

此题允许数字重复，如3 3 4 4为合法输入，此输入一共有两个3，但是每个数字只允许使用一次，则运算过程中两个3都被选取并进行对应的计算操作。

输入描述：

读入4个[1,10]的整数，数字允许重复，测试用例保证无异常数字。

输出描述：

对于每组案例，输出一行表示能否得到24点，能输出true，不能输出false

示例1

输入：

```
7 2 1 10
```

输出：

```
true
```

```python
def f(num_list, target):
    if len(num_list) == 1:
        if num_list[0] == target:
            return 1
        else:
            return 0
    else:
        for n in num_list:
            list1 = num_list.copy()
            list1.remove(n)
            temp = f(list1, target + n) + f(list1, target - n) + f(list1, target * n) + f(list1, target / n)
            if temp>0:
                return temp
        return 0
 
line = input().split(' ')
num_list = [int(num) for num in line]
if f(num_list, 24)>0:
    print('true')
else:
    print('false')
```

```c
#include<stdio.h>
int v[4]={0};  //对数据进行标记，未使用则置0
int dfs(int *a,int s){
    if(s==24)
        return 1;
    for(int i=0;i<4;i++){
        if(v[i]==0){
            v[i]=1;
            if(dfs(a,s+a[i])||dfs(a,s*a[i])||dfs(a,s-a[i])||dfs(a,s/a[i]))
                return 1;
            v[i]=0; //未找到则回退
        }
    }
    return 0;
}
int main(){
    int a[4];
    for(int i=0;i<4;i++)
        scanf("%d",&a[i]);
    int s=a[0]; //用于记录数据总和
    v[0]=1;
    if(dfs(a,a[0])==1) //进行搜索判断
        printf("true");
    else
        printf("false");
    return 0;
}

```



# HJ107 求解立方根 

描述
计算一个浮点数的立方根，不使用库函数。
保留一位小数。

输入描述：
待求解参数，为double类型（一个实数）

输出描述：
输出参数的立方根。保留一位小数。

```python
import sys
a = 1/3
for n in sys.stdin:
    n = float(n)
    if n > 0:
        print("%.1f"%(n**a)) 
    else:
        print("-"+"%.1f"%((n-2*n)**a))
```

# **HJ63** **DNA序列**

一个 DNA 序列由 A/C/G/T 四个字母的排列组合组成。 G 和 C 的比例（定义为 GC-Ratio ）是序列中 G 和 C 两个字母的总的出现次数除以总的字母数目（也就是序列长度）。在基因工程中，这个比例非常重要。因为高的 GC-Ratio 可能是基因的起始点。

给定一个很长的 DNA 序列，以及限定的子串长度 N ，请帮助研究人员在给出的 DNA 序列中从左往右找出 GC-Ratio 最高且长度为 N 的第一个子串。

DNA序列为 ACGT 的子串有: ACG , CG , CGT 等等，但是没有 AGT ， CT 等等

输入：

```
ACGT
2
```

输出：

```
CG
```

说明：

```
ACGT长度为2的子串有AC,CG,GT3个，其中AC和GT2个的GC-Ratio都为0.5，CG为1，故输出CG   
```

```python
a = input()
n = int(input())
out = 0
for i in range(len(a)):
    s = a[i:i+n].count('G') + a[i:i+n].count('C')
    if s > out:
        out = s
        strs = a[i:i+n]
print(strs)
```



# HJ69 **矩阵乘法**

描述
如果A是个x行y列的矩阵，B是个y行z列的矩阵，把A和B相乘，其结果将是另一个x行z列的矩阵C。这个矩阵的每个元素是由下面的公式决定的
$$
C_{ij} = \sum_{k=0}^{y-1}A_{ik}*B_{kj}(其中0<i<x,0<j<z)
$$
矩阵的大小不超过100*100
输入描述：
第一行包含一个正整数x，代表第一个矩阵的行数
第二行包含一个正整数y，代表第一个矩阵的列数和第二个矩阵的行数
第三行包含一个正整数z，代表第二个矩阵的列数
之后x行，每行y个整数，代表第一个矩阵的值
之后y行，每行z个整数，代表第二个矩阵的值

输出描述：
对于每组输入数据，输出x行，每行z个整数，代表两个矩阵相乘的结果
示例1

输入：
16
8
7
17 19 16 19 14 1 14 9 
7 2 7 9 16 14 16 12 
13 3 3 17 5 9 8 16 
1 14 16 10 13 13 14 1 
13 13 15 4 7 2 6 16 
16 15 5 5 15 13 1 11 
11 5 0 16 14 7 7 15 
0 16 4 7 16 6 0 15 
2 14 11 2 17 17 5 12 
8 13 11 10 1 17 10 8 
15 16 17 15 7 8 13 14 
5 19 11 3 11 14 5 4 
9 16 13 11 15 18 0 3 
15 3 19 9 5 14 12 3 
9 8 7 11 18 19 14 18 
12 19 9 1 0 18 17 10 
5 18 16 19 6 12 5 
1 17 1 5 9 16 3 
14 16 4 0 19 3 6 
11 9 15 18 11 17 13 
5 5 19 3 16 1 12 
12 13 19 1 10 5 18 
19 18 6 18 19 12 3 
15 11 6 5 10 17 19 

输出：
1020 1490 1063 1100 1376 1219 884
966 1035 1015 715 1112 772 920
822 948 888 816 831 920 863
855 1099 828 578 1160 717 724
745 1076 644 595 930 838 688
635 1051 970 600 880 811 846
748 879 952 772 864 872 878
526 722 645 335 763 688 748
764 996 868 362 1026 681 897
836 1125 785 637 940 849 775
1082 1476 996 968 1301 1183 953
609 987 717 401 894 657 662
700 1083 1022 527 1016 746 875
909 1162 905 722 1055 708 720
1126 1296 1240 824 1304 1031 1196
905 1342 766 715 1028 956 749

# HJ16 **购物单**

描述

王强决定把年终奖用于购物，他把想买的物品分为两类：主件与附件，附件是从属于某个主件的，下表就是一些主件与附件的例子：

| 主件   | 附件           |
| ------ | -------------- |
| 电脑   | 打印机，扫描仪 |
| 书柜   | 图书           |
| 书桌   | 台灯，文具     |
| 工作椅 | 无             |

如果要买归类为附件的物品，必须先买该附件所属的主件，且每件物品只能购买一次。

每个主件可以有 0 个、 1 个或 2 个附件。附件不再有从属于自己的附件。

王强查到了每件物品的价格（都是 10 元的整数倍），而他只有 N 元的预算。除此之外，他给每件物品规定了一个重要度，用整数 1 **~** 5 表示。他希望在花费不超过 N 元的前提下，使自己的满意度达到最大。

满意度是指所购买的每件物品的价格与重要度的乘积的总和，假设设第i*i*件物品的价格为v[i]*v*[*i*]，重要度为w[i]*w*[*i*]，共选中了k*k*件物品，编号依次为j_1,j_2,...,j_k*j*1,*j*2,...,*j**k*，则满意度为：v[j_1]*w[j_1]+v[j_2]*w[j_2]+ … +v[j_k]*w[j_k]*v*[*j*1]∗*w*[*j*1]+*v*[*j*2]∗*w*[*j*2]+…+*v*[*j**k*]∗*w*[*j**k*]。（其中 * 为乘号）

请你帮助王强计算可获得的最大的满意度。

输入描述：

输入的第 1 行，为两个正整数N，m，用一个空格隔开：

（其中 N （ N<32000 ）表示总钱数， m （m <60 ）为可购买的物品的个数。）

从第 2 行到第 m+1 行，第 j 行给出了编号为 j-1 的物品的基本数据，每行有 3 个非负整数 v p q（其中 v 表示该物品的价格（ v<10000 ）， p 表示该物品的重要度（ 1 **~** 5 ）， q 表示该物品是主件还是附件。如果 q=0 ，表示该物品为主件，如果 q>0 ，表示该物品为附件， q 是所属主件的编号）

输出描述：

 输出一个正整数，为张强可以获得的最大的满意度。
输入：
50 5
20 3 5
20 3 5
10 3 0
10 2 0
10 1 0
输出：
130
说明：
由第1行可知总钱数N为50以及希望购买的物品个数m为5；
第2和第3行的q为5，说明它们都是编号为5的物品的附件；
第4~6行的q都为0，说明它们都是主件，它们的编号依次为3~5；
所以物品的价格与重要度乘积的总和的最大值为10\*1+20\*3+20\*3=130       

* 其实这题就是0-1背包问题

首先来看一下经典背包问题，稍作修改就可以得出这题的解答

**0-1背包问题**

问题描述：有一个背包可以装物品的总重量为W，现有N个物品，每个物品中w[i]，价值v[i]，用背包装物品，能装的最大价值是多少？

**定义状态转移数组dp\[i][j]，表示前i个物品，背包重量为j的情况下能装的最大价值。**

例如，dp[3][4]=6，表示用前3个物品装入重量为4的背包所能获得的最大价值为6，此时并不是3个物品全部装入，而是3个物品满足装入背包的条件下的最大价值。

状态转移方程：

dp\[i][j] = max(dp\[i-1][j], dp\[i-1][j-w[i]]+v[i])

dp\[i-1][j]表示当前物品不放入背包，dp\[i-1][j-w[i]]+v[i]表示当前物品放入背包，**即当前第i个物品要么放入背包，要么不放入背包**。

![](/home/ljxnb/lib/leetcode/381AA8BD8B495E376AC7CF56D2D252F4.gif)

```python
dp = [[0]*(n+1) for _ in range(m+1)]
for i in range(1,m+1):
    for j in range(1,n+1):
        if j-w[i]>=0:
            dp[i][j] = max(dp[i-1][j], dp[i-1][j-w[i]]+v[i])
        else:
            dp[i][j] = dp[i-1][j]
return dp[m][n]
```

# 斐波那契求解埃及分数

分子为1的分数称为埃及分数。现输入一个真分数(分子比分母小的分数，叫做真分数)，请将该分数分解为埃及分数。如：8/11 = 1/2+1/5+1/55+1/110。

注：真分数指分子小于分母的分数，分子和分母有可能gcd不为1！

如有多个解，请输出任意一个。

![](/home/ljxnb/lib/leetcode/11C15A20F2DB1F1ADB00E379DCE64270.png)

输入描述：

输入一个真分数，String型

输出描述：

输出分解后的string

示例1

输入：

```
8/11
2/4
```

输出：

```
1/2+1/5+1/55+1/110
1/3+1/6
```

说明：

```
第二个样例直接输出1/2也是可以的    
```

a/b = 1/(p1+1) + (a-r1)/b*(p1+1)

其中：

p1 = b/a

r1 = b%a

更新：

a = a-r1

b = b*(p1+1)

终止循环条件：

1. a==1:

除到头了，t=a/b

1. b%a==0:

a 可以被 b 整除，这个时候 t =1 /(b/a) = 1/ p1

```python
while True:
    try:
        a,b = list(map(int,input().split('/')))
        flag = True
        t=[]
        while flag:
            p = int(b/a)
            r = int(b%a)
            if r==0 and a > 1:
                t.append('1'+'/'+ str(p))
                flag=False
            elif a==1:
                t.append('1'+'/'+ str(b))
                flag = False
            else:
                t.append('1'+'/'+ str(p+1))
                a = a-r
                b = b*(p+1)
        print("+".join(t))
                
    except:
        break

```

# **HJ103** **Redraiment的走法**

Redraiment是走梅花桩的高手。Redraiment可以选择任意一个起点，从前到后，但只能从低处往高处的桩子走。他希望走的步数最多，你能替Redraiment研究他最多走的步数吗

输入描述：

数据共2行，第1行先输入数组的个数，第2行再输入梅花桩的高度

输出描述：

输出一个结果

输入：

```
6
2 5 1 5 4 5 
```

输出：

```
3
```

说明：

```
6个点的高度各为 2 5 1 5 4 5
如从第1格开始走,最多为3步, 2 4 5 ，下标分别是 1 5 6
从第2格开始走,最多只有1步,5
而从第3格开始走最多有3步,1 4 5， 下标分别是 3 5 6
从第5格开始走最多有2步,4 5， 下标分别是 5 6
所以这个结果是3。     
```

```python
while True:
    try:
        n = int(input())
        s = list(map(int,input().split()))
        dp= [1] *n
        for i in range(n):
            for j in range(i):
                if s[i]>s[j]:
                    dp[i] = max(1+dp[j],dp[i])
        print(max(dp))
    except:
        break
```

# **HJ77** **火车进站**

给定一个正整数N代表火车数量，0<N<10，接下来输入火车入站的序列，一共N辆火车，每辆火车以数字1-9编号，火车站只有一个方向进出，同时停靠在火车站的列车中，只有后进站的出站了，先进站的才能出站。

要求输出所有火车出站的方案，以字典序排序输出。

进阶：时间复杂度：O(n!)\*O*(*n*!) ，空间复杂度：O(n)\*O*(*n*) 

输入描述：

第一行输入一个正整数N（0 < N <= 10），第二行包括N个正整数，范围为1到10。

输出描述：

输出以字典序从小到大排序的火车出站序列号，每个编号以空格隔开，每个输出序列换行，具体见sample。

输入：

```
3
1 2 3
```

输出：

```
1 2 3
1 3 2
2 1 3
2 3 1
3 2 1
```

说明：

```
第一种方案：1进、1出、2进、2出、3进、3出
第二种方案：1进、1出、2进、3进、3出、2出
第三种方案：1进、2进、2出、1出、3进、3出
第四种方案：1进、2进、2出、3进、3出、1出
第五种方案：1进、2进、3进、3出、2出、1出
请注意，[3,1,2]这个序列是不可能实现的。     
```

```python
res = []

def dfs(wait, stack, out):
    if not wait and not stack:
        res.append(' '.join(map(str, out)))
    if wait: # 入栈
        dfs(wait[1:], stack + [wait[0]], out)
    if stack: # 出栈
        dfs(wait, stack[:-1], out + [stack[-1]])

while True:
    try:
        n, nums = int(input()), list(map(int, input().split()))
        dfs(nums, [], [])
        for i in sorted(res):
            print(i)
    except:
        break
```



# **HJ74** **参数解析**

在命令行输入如下命令：

xcopy /s c:\\ d:\\e，

各个参数如下：

参数1：命令字xcopy

参数2：字符串/s

参数3：字符串c:\\

参数4: 字符串d:\\e

请编写一个参数解析程序，实现将命令行各个参数解析出来。

解析规则：

1.参数分隔符为空格
2.对于用""包含起来的参数，如果中间有空格，不能解析为多个参数。比如在命令行输入xcopy /s "C:\\program files" "d:\"时，参数仍然是4个，第3个参数应该是字符串C:\\program files，而不是C:\\program，注意输出参数时，需要将""去掉，引号不存在嵌套情况。
3.参数不定长

4.输入由用例保证，不会出现不符合要求的输入

进阶：时间复杂度：O(n)\*O*(*n*) ，空间复杂度：O(n)\*O*(*n*) 

输入描述：

输入一行字符串，可以有空格

输出描述：

输出参数个数，分解后的参数，每个参数都独占一行

输入：

```
xcopy /s c:\\ d:\\e
```

输出：

```
4
xcopy
/s
c:\\
d:\\e
```

```python
while True:
    try:
        str1=str(input())
        str1=str1.replace(' ','\n')
        e = ''
        flag = False
        for i in str1:
            if i == '"':  # 经过一次引号则拨动一次开关
                flag = not flag
            elif flag == True and i == '\n':
                e += ' '
            else:
                e += i
        b = e.count('\n') + 1
        print(b)
        print(e)
    except:
        break
```



# **HJ92** **在字符串中找出连续最长的数字串**

描述

输入一个字符串，返回其最长的数字子串，以及其长度。若有多个最长的数字子串，则将它们全部输出（按原字符串的相对位置）

本题含有多组样例输入。

输入描述：

输入一个字符串。1<=len(字符串)<=200

输出描述：

输出字符串中最长的数字字符串和它的长度，中间用逗号间隔。如果有相同长度的串，则要一块儿输出（中间不要输出空格）。

示例1

输入：

```
abcd12345ed125ss123058789
a8a72a6a5yy98y65ee1r2
```

输出：

```
123058789,9
729865,2
```

说明：

```
样例一最长的数字子串为123058789，长度为9
样例二最长的数字子串有72,98,65，长度都为2    
```

- 把非数字的全部替换成空格，然后切割；
- 遍历一次得到最大长度；
- 再遍历一次，把符合上述最大长度的字符串取出作追加拼接。

```python
while True:
    try:
        s = input()
        for c in s:
            if not c.isdigit():
                s = s.replace(c, " ")
        s = s.split()
        max_flag = 0
        res = ""
        for c in s:
            if len(c)>max_flag:
                max_flag = len(c)
        for c in s:
            if len(c)==max_flag:
                res = res+c
        print(str(res)+','+str(max_flag))
    except:
        break
```



# **HJ52** **计算字符串的编辑距离**

描述

Levenshtein 距离，又称编辑距离，指的是两个字符串之间，由一个转换成另一个所需的最少编辑操作次数。许可的编辑操作包括**将一个字符替换成另一个字符，插入一个字符，删除一个字符**。编辑距离的算法是首先由俄国科学家 Levenshtein 提出的，故又叫 Levenshtein Distance 。

例如：

字符串A: abcdefg

字符串B: abcdef

通过增加或是删掉字符 ”g” 的方式达到目的。这两种方案都需要一次操作。把这个操作所需要的次数定义为两个字符串的距离。

要求：

给定任意两个字符串，写出一个算法计算它们的编辑距离。

输入描述：

每组用例一共2行，为输入的两个字符串

输出描述：

每组用例输出一行，代表字符串的距离

示例1

输入：

```
abcdefg
abcdef
```

输出：

```
1
```

这题考的是levenshtein距离的计算，需要运用动态规划去解决该类问题。

**传递公式**：

1. lev\[i][j]用来表示字符串a的[1...i]和字符串b[1...j]的levenshtein距离；

2. 插入和删除操作互为逆过程：a删除指定字符变b等同于b插入指定字符变a；

3. 如果a[i] == b[j]，则说明a[i]和b[j]分别加入a，b之后不会影响levenshtein距离，lev\[i][j] = lev\[i-1][j-1] + 0;

4. 如果a[i] != b[j]，则需要考虑3种情况的可能：
   1. a中插入字符，即lev\[i][j] = lev\[i-1][j] + 1;
   2. b中插入字符，即lev\[i][j] = lev\[i][j-1] + 1;
   3. a[i]替换成b[j]，lev\[i][j] = lev\[i-1][j-1] + 1;
   
5. 取这4种情况的最小值。

   ![](/home/ljxnb/lib/leetcode/D2B5CA33BD970F64A6301FA75AE2EB22.png)

```python
while True:
    try:
        A,B= input(),input()
        dp = [[i+j for j in range(len(B)+1)] for i in range(len(A)+1)]
        for i in range(1,len(A)+1):
            for j in range(1,len(B)+1):
                if A[i-1] == B[j-1]:
                    d = 0
                else:
                    d = 1
                dp[i][j] = min(dp[i][j-1]+1, dp[i-1][j]+1,dp[i-1][j-1]+d)
        print(dp[len(A)][len(B)])
        
    except:
        break
```

# [汉明距离](https://leetcode.cn/problems/hamming-distance/):

两个整数之间的 汉明距离 指的是这两个数字对应二进制位不同的位置的数目。

给你两个整数 x 和 y，计算并返回它们之间的汉明距离。

示例 1：

输入：x = 1, y = 4
输出：2
解释：
1   (0 0 0 1)
4   (0 1 0 0)
       ↑   ↑
上面的箭头指出了对应二进制位不同的位置。
示例 2：

输入：x = 3, y = 1
输出：1

```python
class Solution:
    def hammingDistance(self, x, y):
        return bin(x ^ y).count('1')
```

# **HJ64** **MP3光标位置**

输入描述：
输入说明：
1 输入歌曲数量
2 输入命令 U或者D

输出描述：
输出说明
1 输出当前列表
2 输出当前选中歌曲

```
输入：
10
UUUU
输出：
7 8 9 10
7
```

```python
while True:
    try:
        n = int(input())
        index = 0    #光标所在位置
        start = 0    #显示起始坐标
        control = input()
        l = [i for i in range(1,n+1)]    #生成歌曲列表
        for x in control:    #解析每个控制命令来做出相应处理
            if x == 'U':    #如果命令是‘U’
                if index == 0:    #在光标起始位置为0的情况下
                    index += -1+n    #光标移到最后一首歌曲
                    start = n-4    #起始显示坐标变为总长度-4
                else:
                    index += -1    #其他光标坐标-1
                    if index < start:    #如果光标坐标小于起始显示坐标
                        start = index    #那么让起始显示坐标=光标坐标
            if x == 'D':    #如果命令是‘U’
                if index == n-1:    #在光标起始位置为最后一首歌的情况下
                    index = 0    #光标移到第一首歌
                    start = 0    #起始显示坐标移到第一首歌
                else:
                    index += 1    #光标下移一首歌
                    if index > start+3:    #如果光标下移后坐标大于显示的最后一首歌的坐标
                        start += 1    #那么让起始显示坐标+1
        if n <= 4 :    #如果长度小于4，则直接打印所有歌
            print(*l)
        else:    #如果长度大于4，则打印起始坐标开始到起始坐标+4的歌单
            print(*l[start:start+4])
        print(l[index])    #打印当前歌曲
    except:
        break
```

# **HJ70** **矩阵乘法计算量估算**

描述

矩阵乘法的运算量与矩阵乘法的顺序强相关。
例如：

A是一个50×10的矩阵，B是10×20的矩阵，C是20×5的矩阵

计算A*B*C有两种顺序：((AB)C)或者(A(BC))，前者需要计算15000次乘法，后者只需要3500次。

编写程序计算不同的计算顺序需要进行的乘法次数。

进阶：时间复杂度：O(n)\*O*(*n*) ，空间复杂度：O(n)\*O*(*n*) 

输入描述：

输入多行，先输入要计算乘法的矩阵个数n，每个矩阵的行数，列数，总共2n的数，最后输入要计算的法则
计算的法则为一个字符串，仅由左右括号和大写字母（'A'~'Z'）组成，保证括号是匹配的且输入合法！

输出描述：

输出需要进行的乘法次数

示例1

输入：

```
3
50 10
10 20
20 5
(A(BC))
```

复制

输出：

```
3500
```

![](/home/ljxnb/lib/leetcode/AEE5D0F9ABC25CB0F7E6900BB800F64D.gif)

- 实现思路
  - 我们发现在规定的矩阵相乘的顺序中，是由括号所规定的
  - 每次我们读到一个新的矩阵时（标记为A-Z），我们将其放入栈中（维护一个列表）
  - 当读到右括号的时候，说明当前栈顶的两个矩阵需要进行运算，我们此时统计并累计本次运算的次数
  - 然后将上一步计算后的新矩阵重新入栈
  - 直到所有的右括号都被读完，说明所有步骤也计算完成
  - 返回最终的计算次数结果

```python
while True:
    try:
        n = int(input())
        arr = []
        order = []
        res = 0
        for i in range(n):
            arr.append(list(map(int, input().split())))    # 处理输入的矩阵行列数据
        f = input()
        for i in f:
            if i.isalpha():
                order.append(arr[ord(i)-65])               # 将字母转换成第几个矩阵的处理信息
            elif i == ')' and len(order) >= 2:             # 读到右括号就处理最近的两个矩阵相乘的结果
                b = order.pop()
                a = order.pop()
                res += a[1]*b[1]*a[0]                      # 累计到res中
                order.append([a[0], b[1]])
        print(res)
    except:
        break
```



# **HJ71** **字符串通配符**

描述

问题描述：在计算机中，通配符一种特殊语法，广泛应用于文件搜索、数据库、正则表达式等领域。现要求各位实现字符串通配符的算法。
要求：
实现如下2个通配符：
*：匹配0个或以上的字符（注：能被*和?匹配的字符仅由英文字母和数字0到9组成，下同）
？：匹配1个字符

注意：匹配时不区分大小写。

输入：
通配符表达式；
一组字符串。

输出：

返回不区分大小写的匹配结果，匹配成功输出true，匹配失败输出false

进阶：时间复杂度：O(n^2)\*O*(*n*2) ，空间复杂度：O(n)\*O*(*n*) 

输入描述：

先输入一个带有通配符的字符串，再输入一个需要匹配的字符串

输出描述：

返回不区分大小写的匹配结果，匹配成功输出true，匹配失败输出false

示例1

输入：

```
te?t*.*
txt12.xls
```

输出：

```
false
```

示例2

输入：

```
z
zz
```

输出：

```
false
```

示例3

输入：

```
pq
pppq
```

输出：

```
false
```

示例4

输入：

```
**Z
0QZz
```

输出：

```
true
```

示例5

输入：

```
?*Bc*?
abcd
```

输出：

```
true
```

```python
import re
while True:
    try:
        s1 = input().lower()
        s2 = input().lower()
        s1 = s1.replace('.', '\.').replace('?', '[0-9a-z]').replace('*','#')#不支持正则片配额
        s1 = re.sub('#+','[0-9a-z]*',s1)
        if bool(re.fullmatch(s1, s2)):
            print('true')
        else:
            print('false')
    except:
        break
```

# **HJ18** **识别有效的IP地址和掩码并进行分类统计**

描述

请解析IP地址和对应的掩码，进行分类识别。要求按照A/B/C/D/E类地址归类，不合法的地址和掩码单独归类。

所有的IP地址划分为 A,B,C,D,E五类

A类地址从1.0.0.0    到126.255.255.255

B类地址从128.0.0.0到191.255.255.255

C类地址从192.0.0.0到223.255.255.255

D类地址从224.0.0.0到239.255.255.255

E类地址从240.0.0.0 到255.255.255.255

私网IP范围是：

从10.0.0.0       到10.255.255.255

从172.16.0.0  到172.31.255.255

从192.168.0.0到192.168.255.255

子网掩码为二进制下前面是连续的1，然后全是0。（例如：255.255.255.32就是一个非法的掩码）

（注意二进制下全是1或者全是0均为非法子网掩码）

注意：

1. 类似于【0.*.*.*】和【127.*.*.*】的IP地址不属于上述输入的任意一类，也不属于不合法ip地址，计数时请忽略

2. 私有IP地址和A,B,C,D,E类地址是不冲突的

输入描述：

多行字符串。每行一个IP地址和掩码，用~隔开。

请参考帖子https://www.nowcoder.com/discuss/276处理循环输入的问题。

输出描述：

统计A、B、C、D、E、错误IP地址或错误掩码、私有IP的个数，之间以空格隔开。

* 示例1

输入：

```
10.70.44.68~255.254.255.0
1.0.0.1~255.0.0.0
192.168.0.2~255.255.255.0
19..0.~255.255.255.0
```

输出：

```
1 0 1 0 0 2 1
```

说明：

```
10.70.44.68~255.254.255.0的子网掩码非法，19..0.~255.255.255.0的IP地址非法，所以错误IP地址或错误掩码的计数为2；
1.0.0.1~255.0.0.0是无误的A类地址；
192.168.0.2~255.255.255.0是无误的C类地址且是私有IP；
所以最终的结果为1 0 1 0 0 2 1        
```

```python
import sys
import re
a,b,c,d,e,eip,ip=0,0,0,0,0,0,0
def aa(aa):
    global eip
    for i in range(4):
        if not aa[i].isdigit():
            eip+=1
            return False
    return True
def ss(uu):
    for i in range(len(uu)-1):
        if uu[i]=="0":
            if uu[i+1]=="1":
                return False
        if i==len(uu)-2:
            if uu.startswith("0")or uu.endswith("1"):
                return False
    return True
for u in sys.stdin:
    u=re.split("[.~]",u[0:-1])
    ui=u[0:4]
    uy=u[4:]
    unn=uyy=list(map(lambda x:bin(int(x))[2:],uy))
    for i in range(4):
        if len(uyy[i])<8:
            s="".join(["0" for i in range(8-len(uyy[i]))])
            uyy[i]=s+uyy[i]
    uu="".join(uyy)
    if ss(uu):
        if aa(ui):
            if int(ui[0])>=1 and int(ui[0])<=126:
                a+=1
                if int(ui[0])==10:
                    ip+=1
            elif int(ui[0])>=128 and int(ui[0])<=191:
                b+=1
                if int(ui[0])==172 and int(ui[1])>=16 and int(ui[1])<=31:
                    ip+=1
            elif int(ui[0])>=192 and int(ui[0])<=223:
                c+=1
                if int(ui[0])==192 and int(ui[1])==168:
                    ip+=1
            elif int(ui[0])>=224 and int(ui[0])<=239:
                d+=1
            elif int(ui[0])>=240 and int(ui[0])<=255:
                e+=1
            elif int(ui[0])==0:
                continue
            else:
                eip+=1
    else:
        eip+=1
print(a,b,c,d,e,eip,ip)
```



# **HJ95** **人民币转换**

描述

考试题目和要点：

1、中文大写金额数字前应标明“人民币”字样。中文大写金额数字应用壹、贰、叁、肆、伍、陆、柒、捌、玖、拾、佰、仟、万、亿、元、角、分、零、整等字样填写。

2、中文大写金额数字到“元”为止的，在“元”之后，应写“整字，如532.00应写成“人民币伍佰叁拾贰元整”。在”角“和”分“后面不写”整字。

3、阿拉伯数字中间有“0”时，中文大写要写“零”字，阿拉伯数字中间连续有几个“0”时，中文大写金额中间只写一个“零”字，如6007.14，应写成“人民币陆仟零柒元壹角肆分“。

4、10应写作“拾”，100应写作“壹佰”。例如，1010.00应写作“人民币壹仟零拾元整”，110.00应写作“人民币壹佰拾元整”

5、十万以上的数字接千不用加“零”，例如，30105000.00应写作“人民币叁仟零拾万伍仟元整”

输入描述：

输入一个double数

输出描述：

输出人民币格式

示例1

输入：

```
151121.15
```

输出：

```
人民币拾伍万壹仟壹佰贰拾壹元壹角伍分
```

示例2

输入：

```
1010.00
```

输出：

```
人民币壹仟零拾元整
```

```python
def fun(n, s =''):
    if n < 20:  # 由于10应写作“拾”，所以第一前1-19进行查字典处理
        s += dic[n]
    elif n < 100 :  # 大于20小于100的数
        if n % 10 >= 1:  # 非整十
            s += fun(n//10) + '拾' + fun(n % 10)
        else:  # 整十
            s += fun(n//10) + '拾'
    elif n < 1000: # 大于100小于1000的数
        if n % 100 >= 10:  # 十位不为0
            s += fun(n//100) + '佰' + fun(n % 100)
        elif n % 100 > 0:  # 个位不为零
            s += fun(n//100) + '佰零' + fun(n % 100)
        else:  # 个位为零
            s += fun(n//100) + '佰'
    elif n < 10000:  # 大于1000小于10000的数
        if n % 1000 >= 100:  # 百位不为零
            s += fun(n//1000) + '仟' + fun(n % 1000)
        elif n % 1000 > 0:  # 个位不为0
            s += fun(n//1000) + '仟零' + fun(n % 1000)
        else:  # 个位为0
            s += fun(n//1000) + '仟'
    elif n < 100000000:  # 大于10000小于100000000的数
        if n % 10000 >= 1000:  # 千位不为0时
            s += fun(n//10000) + '万' + fun(n % 10000)
        elif n % 10000 > 0:  # 个位不为0
            s += fun(n//10000) + '万零' + fun(n % 10000)
        else:  # 个位为0
            s += fun(n//10000) + '万'
    else:  # 大于100000000的数
        if n % 100000000 >= 10000000:  # 千万位不为0
            s += fun(n//10000) + '亿' + fun(n % 100000000)
        elif n % 100000000 > 0:  # 个位不为0
            s += fun(n//100000000) + '亿零' + fun(n % 100000000)
        else:  # 个位为0
            s += fun(n//100000000) + '亿'
    return s

while True:
    try:
        dic = {1:'壹', 2:'贰', 3:'叁', 4:'肆', 5:'伍', 6:'陆', 7:'柒', 8:'捌', 9:'玖', 10:'拾', 11:'拾壹', 12:'拾贰', 13:'拾叁', 14:'拾肆', 15:'拾伍', 16:'拾陆', 17:'拾柒', 18:'拾捌', 19:'拾玖'}
        n, f = map(int,input().split('.'))
        if n > 0:
            s = '人民币' + fun(n) + '元'
        else:
            s = '人民币'
        if f == 0:
            s += '整'
        elif f < 10:
            s += dic[f] + '分'
        elif f % 10 == 0:
            s += dic[f//10] + '角'
        else:
            s += dic[f//10] + '角' + dic[f % 10] + '分'
        print(s)
    except:
        break
```



# **HJ93** **数组分组**

描述

输入int型数组，询问该数组能否分成两组，使得两组中各元素加起来的和相等，并且，所有5的倍数必须在其中一个组中，所有3的倍数在另一个组中（不包括5的倍数），不是5的倍数也不是3的倍数能放在任意一组，可以将数组分为空数组，能满足以上条件，输出true；不满足时输出false。

输入描述：

第一行是数据个数，第二行是输入的数据

输出描述：

返回true或者false

示例1

输入：

```
4
1 5 -5 1
```

复制

输出：

```
true
```

复制

说明：

```
第一组：5 -5 1
第二组：1      
```

示例2

输入：

```
3
3 5 8
```

输出：

```
false
```

说明：

```
由于3和5不能放在同一组，所以不存在一种分法。
```

```python
while True:
    try:
        length = int(input())
        num_list = list(map(int, input().strip().split()))
        res = 0
        # 计算初始3,5倍数的差值
        for _ in range(length):
            i = num_list.pop(0)
            if i % 3 == 0:
                res += i
            elif i % 5 == 0:
                res -= i
            else:
                num_list.append(i)
        res = {res}
        # 结果计算，把之前结果，分别计算当前值的+和- 2种情况，然后把结果再放回去，给下一次计算
        while num_list:
            i = num_list.pop(0)
            res_plus = [x + i for x in res]
            res_plus.extend([x - i * 2 for x in res_plus])
            res = set(res_plus)
        # 最后如果0值在结果中，表示可以算出，如果不在则不行
        if 0 in res:
            print('true')
        else:
            print('false')
    except:
        break
```

```python
# 深度优先递归
# 题目要求将数组分为两组，5和3的倍数不能在同一组内，非5或3的倍数随意放在任意一组，要求分配之后两组和相等
# 根据题意将数组中的5和3分别放到两组中，然后递归穷举所有非5或3的倍数的分组情况
def dfs(three, five, other):
    if not other:
        if sum(three) == sum(five):
            return True
        else:
            return False
    if dfs(three+other[:1], five, other[1:]):
        return True
    if dfs(three, five+other[:1], other[1:]):
        return True
while True:
    try:
        n, nums = int(input()), list(map(int, input().split()))
        three, five, other = [], [], []
        for num in nums:
            if num % 3 == 0:
                three.append(num)
            elif num % 5 == 0:
                five.append(num)
            else:
                other.append(num)
        if dfs(three, five, other):
            print ('true')
        else:
            print ('false')
    except:
        break
```



# **HJ25** **数据分类处理**

描述

信息社会，有海量的数据需要分析处理，比如公安局分析身份证号码、 QQ 用户、手机号码、银行帐号等信息及活动记录。

采集输入大数据和分类规则，通过大数据分类处理程序，将大数据分类输出。

输入描述：

﻿一组输入整数序列I和一组规则整数序列R，I和R序列的第一个整数为序列的个数（个数不包含第一个整数）

输出描述：

﻿从R依次中取出R<i>，对I进行处理，找到满足条件的I： 

I整数对应的数字需要连续包含R<i>对应的数字。比如R<i>为23，I为231，那么I包含了R<i>，条件满足 。 

按R<i>从小到大的顺序:

(1)先输出R<i>； 

(2)再输出满足条件的I的个数； 

(3)然后输出满足条件的I在I序列中的位置索引(从0开始)； 

(4)最后再输出I。 

附加条件： 

(1)R<i>需要从小到大排序。相同的R<i>只需要输出索引小的以及满足条件的I，索引大的需要过滤掉 

(2)如果没有满足条件的I，对应的R<i>不用输出 

(3)最后需要在输出序列的第一个整数位置记录后续整数序列的个数(不包含“个数”本身)

序列I：15,123,456,786,453,46,7,5,3,665,453456,745,456,786,453,123（第一个15表明后续有15个整数） 

序列R：5,6,3,6,3,0（第一个5表明后续有5个整数） 

输出：*30, 3,6,0,123*,3,453,7,3,9,453456,13,453,14,123,6,7,1,456,2,786,4,46,8,665,9,453456,11,456,12,786

说明：

*30*----后续有30个整数

*3*----从小到大排序，第一个R<i>为0，但没有满足条件的I，不输出0，而下一个R<i>是3

*6---* *存在6个包含3的I* 

*0---* *123所在的原序号为0* 

*123---* *123包含3，满足条件* 

示例1

输入：

```
15 123 456 786 453 46 7 5 3 665 453456 745 456 786 453 123
5 6 3 6 3 0
```

输出：

```
30 3 6 0 123 3 453 7 3 9 453456 13 453 14 123 6 7 1 456 2 786 4 46 8 665 9 453456 11 456 12 786
```

说明：

```
将序列R：5,6,3,6,3,0（第一个5表明后续有5个整数）排序去重后，可得0,3,6。
序列I没有包含0的元素。
序列I中包含3的元素有：I[0]的值为123、I[3]的值为453、I[7]的值为3、I[9]的值为453456、I[13]的值为453、I[14]的值为123。
序列I中包含6的元素有：I[1]的值为456、I[2]的值为786、I[4]的值为46、I[8]的值为665、I[9]的值为453456、I[11]的值为456、I[12]的值为786。
最后按题目要求的格式进行输出即可。
```

![](/home/ljxnb/lib/leetcode/D2B5CA33BD970F64A6301FA75AE2EB223.png)

```python
while True:
    try:
        a=input().split()[1:]
        b=map(str,sorted(map(int,set(input().split()[1:]))))
        totalNum=0
        res=""
        for num in b:
            singleRes,count="",0
            for i,v in enumerate(a):
                if num in v:
                    singleRes+=str(i)+" "+v+" "
                    totalNum+=2
                    count+=1
            if count:
                singleRes=num+" "+str(count)+" "+singleRes
                totalNum+=2
            res+=singleRes
        print((str(totalNum)+" "+res).rstrip())

    except:
        break
```



# **HJ68** **成绩排序**

描述

给定一些同学的信息（名字，成绩）序列，请你将他们的信息按照成绩从高到低或从低到高的排列,相同成绩

都按先录入排列在前的规则处理。

例示：
jack   70
peter   96
Tom    70
smith   67

从高到低 成绩
peter   96
jack   70
Tom    70
smith   67

从低到高

smith   67

jack   70

Tom    70

peter   96

注：0代表从高到低，1代表从低到高

进阶：时间复杂度：O(nlogn)\*O*(*n**l**o**g**n*) ，空间复杂度：O(n)\*O*(*n*) 

输入描述：

第一行输入要排序的人的个数n，第二行输入一个整数表示排序的方式，之后n行分别输入他们的名字和成绩，以一个空格隔开

输出描述：

按照指定方式输出名字和成绩，名字和成绩之间以一个空格隔开

示例1

输入：

```
3
0
fang 90
yang 50
ning 70
```

输出：

```
fang 90
ning 70
yang 50
```

示例2

输入：

```
3
1
fang 90
yang 50
ning 70
```

输出：

```
yang 50
ning 70
fang 90
```

**注意点**：

1. 把下面的列表换成字典有可能过不了，因为有可能存在重名的同学；
2. 如果想要更快，可以在 while 的时候使用插入排序；
3. 这题我调了好久，就是因为一开始想省事直接用成绩的字符串排的。学的时候看到的例子都是 '8' > '3' 这种，弄得我以为数字字符串排序和数字一样，结果今天遇到了 '8' > '78'，明明觉得我逻辑无懈可击但我还是反复查逻辑和语法调了我几个小时，最后突然灵感一来才发现是这的问题，淦。

```python
while True:
    try:
        num = int(input())
        mode = int(input())
        ls = []
        while num:  # 以 (名字，成绩) 的格式存入列表
            entry = input().split(' ')
            ls.append((entry[0], int(entry[1])))
            num -= 1
        ls.sort(key=lambda x: x[1], reverse= False if mode else True)  # 根据成绩排序
        for e in ls:
            print(e[0], e[1])  # print 函数中“,”是加个空格，“+”是直接连接
    except:
        break
```

# **HJ48** **从单向链表中删除指定值的节点**

描述

输入一个单向链表和一个节点的值，从单向链表中删除等于该值的节点，删除后如果链表中无节点则返回空指针。

链表的值不能重复。

构造过程，例如输入一行数据为:

6 2 **1 2** **3 2** **5 1** **4 5** **7 2** 2

则第一个参数6表示输入总共6个节点，第二个参数2表示头节点值为2，剩下的2个一组表示第2个节点值后面插入第1个节点值，为以下表示:

1 2 表示为

2->1

链表为2->1

3 2表示为

2->3

链表为2->3->1

5 1表示为

1->5

链表为2->3->1->5

4 5表示为

5->4

链表为2->3->1->5->4

7 2表示为

2->7

链表为2->7->3->1->5->4

最后的链表的顺序为 2 7 3 1 5 4

最后一个参数为2，表示要删掉节点为2的值

删除 结点 2

则结果为 7 3 1 5 4

测试用例保证输入合法

输入描述：

输入一行，有以下4个部分：

1 输入链表结点个数
2 输入头结点的值
3 按照格式插入各个结点
4 输入要删除的结点的值

输出描述：

输出一行

输出删除结点后的序列，每个数后都要加空格

示例1

输入：

```
5 2 3 2 4 3 5 2 1 4 3
```

输出：

```
2 5 4 1
```

说明：

```
形成的链表为2->5->3->4->1
删掉节点3，返回的就是2->5->4->1  
```

示例2

输入：

```
6 2 1 2 3 2 5 1 4 5 7 2 2
```

输出：

```
7 3 1 5 4
```

```python
while True:
    try:
        s=input().split()
        n=int(s[0])
        res=[s[1]]
        rm=s[-1]
        for i in range(n-1):
            a=s[2+i*2]
            b=s[3+i*2]
            res.insert(res.index(b)+1,a)
        res.remove(rm)
        print(' '.join(res)+' ')
    except:
        break#没看懂...
```

# **HJ3** **明明的随机数**

明明生成了N*N*个1到500之间的随机整数。请你删去其中重复的数字，即相同的数字只保留一个，把其余相同的数去掉，然后再把这些数从小到大排序，按照排好的顺序输出。

输入描述：

第一行先输入随机整数的个数 N 。 接下来的 N 行每行输入一个整数，代表明明生成的随机数。 具体格式可以参考下面的"示例"。

输出描述：

输出多行，表示输入数据处理后的结果

示例1

输入：

```
3
2
2
1
```

输出：

```
1
2
```

说明：

```
输入解释：
第一个数字是3，也即这个小样例的N=3，说明用计算机生成了3个1到500之间的随机整数，接下来每行一个随机数字，共3行，也即这3个随机数字为：
2
2
1
所以样例的输出为：
1
2       
```

```python
while True:
    try:
        n = input()      #指定为N个数，输入
        lst = []         #指定一个空列表
        for i in range(int(n)):        #循环N次
            lst.append(int(input()))      #空集合中追加一个N个数中的某一个随机数
        uniq=set(lst)        #列表去重，但是会变成无序
        lst=list(uniq)       #集合转列表
        lst.sort()           #列表排序
        for i in lst:
            print(i)         #打印列表
    except:
        break
```

# **HJ39** **判断两个IP是否属于同一子网**

描述

IP地址是由4个0-255之间的整数构成的，用"."符号相连。

二进制的IP地址格式有32位，例如：10000011，01101011，00000011，00011000;每八位用十进制表示就是131.107.3.24

子网掩码是用来判断任意两台计算机的IP地址是否属于同一子网络的根据。

子网掩码与IP地址结构相同，是32位二进制数，由1和0组成，且1和0分别连续，其中网络号部分全为“1”和主机号部分全为“0”。

你可以简单的认为子网掩码是一串连续的1和一串连续的0拼接而成的32位二进制数，左边部分都是1，右边部分都是0。

利用子网掩码可以判断两台主机是否在同一子网中。

若两台主机的IP地址分别与它们的子网掩码进行逻辑“与”运算（按位与/AND）后的结果相同，则说明这两台主机在同一子网中。



示例：
I P 地址　 192.168.0.1
子网掩码　 255.255.255.0

转化为二进制进行运算：

I P 地址　 11000000.10101000.00000000.00000001
子网掩码　11111111.11111111.11111111.00000000

AND运算  11000000.10101000.00000000.00000000

转化为十进制后为：
192.168.0.0



I P 地址　 192.168.0.254
子网掩码　 255.255.255.0


转化为二进制进行运算：

I P 地址　11000000.10101000.00000000.11111110
子网掩码 11111111.11111111.11111111.00000000

AND运算 11000000.10101000.00000000.00000000

转化为十进制后为：
192.168.0.0

通过以上对两台计算机IP地址与子网掩码的AND运算后，我们可以看到它运算结果是一样的。均为192.168.0.0，所以这二台计算机可视为是同一子网络。

输入一个子网掩码以及两个ip地址，判断这两个ip地址是否是一个子网络。

若IP地址或子网掩码格式非法则输出1，若IP1与IP2属于同一子网络输出0，若IP1与IP2不属于同一子网络输出2。

注:

有效掩码与IP的性质为：

1. 掩码与IP每一段在 0 - 255 之间
2. 掩码的二进制字符串前缀为网络号，都由‘1’组成；后缀为主机号，都由'0'组成

输入描述：

3行输入，第1行是输入子网掩码、第2，3行是输入两个ip地址
题目的示例中给出了三组数据，但是在实际提交时，你的程序可以只处理一组数据（3行）。

输出描述：

若IP地址或子网掩码格式非法则输出1，若IP1与IP2属于同一子网络输出0，若IP1与IP2不属于同一子网络输出2

示例1

输入：

```
255.255.255.0
192.168.224.256
192.168.10.4
255.0.0.0
193.194.202.15
232.43.7.59
255.255.255.0
192.168.0.254
192.168.0.1
```

输出：

```
1
2
0
```

说明：

```
对于第一个例子:
255.255.255.0
192.168.224.256
192.168.10.4
其中IP:192.168.224.256不合法，输出1

对于第二个例子:
255.0.0.0
193.194.202.15
232.43.7.59
2个与运算之后，不在同一个子网，输出2

对于第三个例子，2个与运算之后，如题目描述所示，在同一个子网，输出0
```
