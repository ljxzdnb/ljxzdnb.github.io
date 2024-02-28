# [HJ37 统计每个月兔子的总数（斐波那契数）]()

题目描述
有一种兔子，从出生后第3个月起每个月都生一只兔子，小兔子长到第三个月后每个月又生一只兔子。

例子：假设一只兔子第3个月出生，那么它第5个月开始会每个月生一只兔子。
一月的时候有一只兔子，假如兔子都不死，问第n个月的兔子总数为多少？

数据范围：
输入满足 1≤n≤31

输入描述：
输入一个int型整数表示第n个月

输出描述：
输出对应的兔子总数

示例 1
输入：
3

输出：
2

参考代码

```python
def dfs(m, h, maxh):
    if h > maxh:
        return 1
    elif h < 3:
        return dfs(1, h+1, maxh)
    else:
        return dfs(1, h+1, maxh) + dfs(1, 1, maxh-h+1)

while 1:
    try:
        month = int(input())
        print(dfs(1, 1, month))
    except:
        break
```

```c
int dfs(int m,int h,int maxh){
	if(h > maxh){
		return 1;
	}
	else if (h < 3)
	{
		return dfs(1,h+1,maxh);
	}
	else{
		return dfs(1,h+1,maxh)+dfs(1,1,maxh-h+1);
	}
	
}
```

**定义：**

斐波那契数列指的是这样一个数列1,1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233，377，……，它的每一项都等于它前面两项之和。

**数列由来：**

因数学家列昂纳多·斐波那契以兔子繁殖为例子而引入，故又称为“兔子数列”。

一般而言，兔子在出生两个月后，就有繁殖能力，一对兔子每个月能生出一对小兔子来。如果所有兔子都不死，那么一年以后可以繁殖多少对兔子？

我们不妨拿新出生的一对小兔子分析一下：

第一个月小兔子没有繁殖能力，所以还是一对

两个月后，生下一对小兔对数共有两对

三个月以后，老兔子又生下一对，因为小兔子还没有繁殖能力，所以一共是三对

－－－－－－

依次类推可以列出下表：

![](/home/ljxnb/lib/leetcode/0M6XdXm2mE.WEBP)

幼仔对数=前月成兔对数

成兔对数=前月成兔对数+前月幼仔对数

总体对数=本月成兔对数+本月幼仔对数

可以看出幼仔对数、成兔对数、总体对数都构成了一个数列。这个数列有关十分明显的特点，那就是：前面相邻两项之和，构成了后一项。

# [斐波那契数](https://leetcode.cn/problems/fibonacci-number/):

1.动态规划

*F*(*n*)=*F*(*n*−1)+*F*(*n*−2)

```python
    def fib(self, N: int) -> int:
        A = [0, 1]
        for i in range(N - 1):
            A.append(A[-1] + A[-2])
        return A[N]
```

```c
#include <stdio.h>
#include <stdlib.h>
int fib(int N){
	int *P = (int *)calloc((N+1)*sizeof(int));
	*(P) = 0;
    *(P+1) = 1;
	for(int i=0;i<N-1;i++){
		*(P+2+i) = *(P+1+i)+*(P+i);
	}
	int end = *(P+N);
    free(P);
    P = NULL;
	return end;
}
```

```bash
#!/bin/bash
set -x

factorial()
{
    A=(0 1)
    for i in `seq 0 $1`;do
    j=`expr $i + 1`
    a=${A[$i]}
    b=${A[$j]}
    A+=(`expr $a + $b`)
    done
    echo ${A[@]}
}

input=4
val=$(factorial $input)
echo "Factorial of $input is : "$val
```
2.通项公式
$$
\begin{align*}
u_{k} &= \left(\genfrac{}{}{0pt}{0}{F_{n+1}}{F_{n}}\right)\\
& \left\{\genfrac{}{}{0pt}{0}{F_{n+2}=F_{n+1}+F_{n}}{F_{n+1}=F_{n+1}}\right .\\
u_{n+1} &= \left(\genfrac{}{}{0pt}{0}{1}{1}\genfrac{}{}{0pt}{0}{1}{0}\right)u_{n} = Au_{n}\\
u_{n} &= A^{n}u_{0},求A^{n}\\
det(A-\lambda{I})&= \left|\genfrac{}{}{0pt}{0}{1-\lambda}{1}\genfrac{}{}{0pt}{0}{1}{-\lambda}\right|=\lambda^{2}-\lambda-1\\
& \rightarrow{\lambda_{1}=\frac{1+\sqrt(5)}{2},\lambda_{2}=\frac{1-\sqrt(5)}{2}}\\
& 由初始值F_{0}=0,F_{1}=1, u_{0}=\left(\genfrac{}{}{0pt}{0}{1}{0}\right)\\
\left(\genfrac{}{}{0pt}{0}{F_{n+1}}{F_{n}}\right)&= u_{n} = A^{n}u_{0}=S\Lambda^{n}S^{-1}u_{0}\\
&= \left(\genfrac{}{}{0pt}{0}{\lambda_{1}}{1}\genfrac{}{}{0pt}{0}{\lambda_{2}}{1}\right)\left(\genfrac{}{}{0pt}{0}{\lambda_{1}^{k}}{0}\genfrac{}{}{0pt}{0}{0}{\lambda_{2}^{k}}\right)\left(\genfrac{}{}{0pt}{0}{1}{0}\right)=\frac{1}{\lambda_{1}-\lambda_{2}}\\
& Fibonacci数F_{n}是这个乘积的第二个分量 \\
F(n) &= \frac{1}{\sqrt{5}}[(\frac{1+\sqrt(5)}{2})^n-(\frac{1-\sqrt(5)}{2})^n] \\
\end{align*}
$$

```python
    def fib(self, n: int) -> int:
        sqrt5 = 5**0.5
        fibN = ((1 + sqrt5) / 2) ** n - ((1 - sqrt5) / 2) ** n
        return round(fibN / sqrt5)
```

```c
#include <math.h>
#include <stdio.h>

int fib(int n){
    double sqrt5 = sqrtf(5);
    double fibN = (pow((1+sqrt5)/2,n)- pow((1-sqrt5)/2,n));
    return (int)(fibN/sqrt5);
}

int main(){
    printf("%d\n",fib(4));
}
```

round() 方法返回浮点数 x 的四舍五入值，准确的说保留值将保留到离上一位更近的一端（四舍六入）。

# HJ6 质数因子（简单）

题目描述
功能:输入一个正整数，按照从小到大的顺序输出它的所有质因子（重复的也要列举）（如180的质因子为2 2 3 3 5 ）

数据范围：1 ≤ n ≤ 2×10**10 + 14

输入描述：
输入一个整数

输出描述：
按照从小到大的顺序输出它的所有质数的因子，以空格隔开。

示例1
输入：

180

输出：

```python
2 2 3 3 5
```

# 参考代码

```python
while 1:
    try:
        num = int(input())
        c = 2
        while c <= num**0.5:
            if num % c == 0:
                print(c, end=' ')
                num = num // c
            else:
                c += 1

        if num>2:
            print(num, end=' ')
    except:
        break
```

```c
#include <stdio.h>
#include <stdlib.h>

void fuc(int n){
	int c = 2;
	while (c <= n*0.5)
	{
		if (n % c == 0){
			printf("%d",c);
			n = n / c;
		}
		else{
			c += 1;
		}
	}
	if (n > 2){
		printf("%d\n",n);
	}
}
```




# 欢乐的周末

题目描述
小华和小为是很要好的朋友，他们约定周末一起吃饭通过手机交流，他们在地图上选择了多个聚餐地点（由于自然地形等原因，部分聚餐地点不可达），求小华和小为都能到达的聚餐地点有多少个？

输入描述：
第一行输入m和n，m代表地图的长度，n代表地图的宽度。
第二行开始具体输入地图信息，地图信息包含：
0 为通畅的道路
1 为障碍物（旦仅1为障碍物）
2 为小华或者小为，地图中必定有且仅有2个（非障碍物)
3 为被选中的聚餐地点（非障碍物）

输出描述：
可以被两方都到达的聚餐地点数量，行末无空格。

示例 1
输入：
4 4
2 1 0 3
0 1 2 1
0 3 0 0
0 0 0 0

输出：
2

说明：
第一行输入地图的长宽为3和4。
第二行开始为具体的地图，其中：3代表小华和小明选择的聚餐地点；2代表小华或者小明（确保有2个)；
代表可以通行的位置；1代表不可以通行的位置。
此时两者能都能到达的聚餐位置有2处。

示例 2
输入：
4 4
2 1 2 3
0 1 0 0
0 1 0 0
0 1 0 0

说明：
第一行输入地图的长宽为4和4。
第二行开始为具体的地图，其中：3代表小华和小明选择的聚餐地点；2代表小华或者小明（确保有2个）
代表可以通行的位置；1代表不可以通行的位置。
由于图中小华和小为之问有个阻隔，此时，没有两人都能到达的聚餐地址，故而返回0。

参考代码
首先我们需要遍历输入项，
构建二维数组；
找到起始点（也就是小为，小华的坐标）；
找到终止点（聚餐地点坐标）；
循环聚餐地点，看从小为，小华坐标出发能否都能到达；

```python
while 1:
    try:
        n, m = map(int, input().split())

        map_ = [input().split() for i in range(m)]
        # 小华小为的坐标
        start_spots = []
        # 聚餐点坐标
        end_spots = []

        for i in range(m):
            for j in range(n):
                if map_[i][j] == "2":
                    start_spots.append((i, j))
                elif map_[i][j] == "3":
                    end_spots.append((i, j))

        count = 0
        for end_ in end_spots:

            def dfs(si, sj, cache):
                if (si, sj) in cache:
                    return False
                cache.append((si, sj))
                # 控制地图边界
                if not (0 <= si < m and 0 <= sj < n):
                    return False
                # 遇到障碍物
                if map_[si][sj] == "1":
                    return False
                if (si, sj) == tuple(end_):
                    return True

                # 有四种情况可以向上下左右走
                return dfs(si+1, sj, cache) or dfs(si-1, sj, cache) \
                       or dfs(si, sj+1, cache) or dfs(si, sj-1, cache)

            # 已到达的坐标，避免重复计算
            cache_a = []
            cache_b = []
            if dfs(start_spots[0][0], start_spots[0][1], cache_a) \
                    and dfs(start_spots[1][0], start_spots[1][1], cache_b):
                count += 1

        print(count)
    except Exception as e:
        break

```



# 字符串格式化输出

输入形如 AB-ABC-cABd-Cb@ 的[字符串](https://so.csdn.net/so/search?q=字符串&spm=1001.2101.3001.7020)，输入待分隔长度k； 要求输出保留第一个“-”前面的字符串格式，后面的每k个字符一分格，每三个字符中，大写字母数多的三个字母转大写，小写字母数多的三个字母转小写，一样多的不处理。

示例1:

输入:

```python
AB-ABC-cABd-Cb@
2
12
```

输出:

```python
AB-AB-CC-AB-dc-b@
```

说明:

1. AB- 保留
2. ABCcABdCb@ 每三个字符一组判断大小写
3. ABC -> ABC
4. cAB -> CAB
5. dCb -> dcb
6. @ -> @
7. 转换后的再按长度 k 分隔

```python
while 1:
    try:
        head, tail = input().split("-", 1)
        k = int(input())

        tail = tail.replace("-", "")
        temp = ""
        for index in range(0, len(tail), 3):
            line = tail[index: index+3]
            count1 = 0
            count2 = 0
            for c in line:
                if "A" <= c <= "Z":
                    count1 += 1
                if "a" <= c <= "z":
                    count2 += 1
            if count1 == count2:
                temp += line
            elif count1 > count2:
                temp += line.upper()
            else:
                temp += line.lower()

		# 按长度k分隔处理后的字符串
        dp = []
        for index in range(0, len(temp), k):
            dp.append(temp[index: index + k])

        print(f"{head}-{'-'.join(dp)}")
        
        # 推导式写法
        # print(f"{head}-{'-'.join([temp[index: index+k] for index in range(0, len(temp), k)])}")
    except Exception as e:
        break
```



# 按身高和体重排队

题目描述
某学校举行运动会，学生们按编号(1、 2、 3…n)进行标识，现需要按照身高由低到高排列，对身高相同的人，按体重由轻到重排列；对于身高体重都相同的人，维持原有的编号顺序关系。请输出排列后的学生编号。
输入描述：
两个序列，每个序列由 n 个正整数组成（0<n<=100）。第一个序列中的数值代表身高，第二个序列中的数值代表体重。
输出描述：
排列结果，每个数值都是原始序列中的学生编号，编号从 1 开始。

示例 1
输入：

4
100 100 120 130
40 30 60 50
1
2
3
输出：

2 1 3 4
1
sorted 语法：
sorted(iterable, key=None, reverse=False)

参数说明：
iterable – 可迭代对象。
key – 主要是用来进行比较的元素，只有一个参数，具体的函数的参数就是取自于可迭代对象中，指定可迭代对象中的一个元素来进行排序。
reverse – 排序规则，reverse = True 降序 ， reverse = False 升序（默认）。
返回值
返回重新排序的列表。

多条件排序
当需要进行多条件排序时多可以使用 sorted 方法实现，按照需要的 排序规则定义 key=lambda ；

类似的题目还有 【华为机试真题 Python实现】奥运会奖牌榜的排名
根据 金牌数、银牌数、铜牌数、国家首字母 四个条件排序。

如果排序时 两个条件的排序 方向不同时，可以在逆序的条件前添加负号，例如：key=lambda x: (x[1], -x[2])

# 参考代码

```python
while 1:
    try:
        lens = int(input())
        a = list(map(int, input().split()))
        b = list(map(int, input().split()))

        nums = []
        for i in range(1, lens+1):
            nums.append((i, a[i-1], b[i-1]))

        nums = sorted(nums, key=lambda x: (x[1], x[2]))
        print(" ".join([str(i[0]) for i in nums]))
    except Exception as e:
        break
```

# 拼接 URL

# 题目描述

给定一个 URL [前缀和](https://so.csdn.net/so/search?q=前缀和&spm=1001.2101.3001.7020) URL 后缀,需要将其连接为一个完整的 URL,如果前缀结尾和后缀开头都没有“/” ,则需自动补上“/”连接符, 如果前缀结尾和后缀开头都有“/”，需自动去重。

例如:
1)前缀:/a;后缀:/b;连接后 URL 为: /a/b,
2)前缀: /a/;后缀:/b; 连接后 URL 为:/a/b,
3)前缀: /a;后缀:b;连接后 URL 为:/a/b,
4)前缀为空;后缀为空;连接后 URL 为:/

约束: 不用考虑前后缀 URL 不合法情况

输入描述：
URL 前缀（一个小于 100 的字符串）， URL 后缀（一个小于 100 的字符串）
输出描述：
拼接后的 URL.

示例 1
输入：/a /b
输出：/a/b

示例 2
输入：/a/ /b
输出：/a/b

示例 3
输入：/a b
输出：/a/b

示例 4
输入：
输出：/

参考代码

```python
while 1:
    try:
        sto = input()
        st = sto.split()
        a = ""
        b = ""
        if len(st) == 1:
            # 判断是 前缀还是后缀
            if sto.startswith(" "):
                b = st[0]
            else:
                a = st[0]
        elif len(st) == 2:
            a = st[0]
            b = st[1]
        else:
            print("/")
            continue

        # 我们要求 前缀不由 / 结尾
        # 结尾有多个 /
        while a.endswith("/"):
            a = a[:-1]

        # 开头有多个 /
        while b.startswith("/"):
            b = b[1:]

        # 我们要求 后缀由 / 开始
        b = f"/{b}"

        print(a+b)
    except Exception as e:
        break
```



# 最大括号深度

题目描述
现有一字符串仅由( , ) , { , } , [ , ]六种括号组成。

若字符串满足以下条件之一，则为无效字符串：

任一类型的左右括号数量不相等；
存在未按正确顺序（先左后右）闭合的括号。
输出括号的最大嵌套深度，若字符串无效则输出0.
0<=字符串长度<=100000

输入描述：
一个只包( , ) , { , } , [ , ]的字符串。

输出描述:
一个整数，最大括号深度

示例 1
输入:
[]

输出:
1

```python
while 1:
    try:
        nums = input()
        max_ = 0
        stack = []
        for c in nums:
            if c in "{[(":
                stack.append(c)
            else:
                if not stack:
                    print(0)
                    break
                max_ = max(max_, len(stack))
                cr = stack.pop()
                if f"{cr}{c}" not in ["{}", "[]", "()"]:
                    print(0)
                    break
        else:
            print(max_)
    except Exception as e:
        break
```

```python
    def isValid(self, s: str) -> bool:
        dic = {')':'(',']':'[','}':'{'}
        stack = []
        for i in s:
            if stack and i in dic:
                if stack[-1] == dic[i]: stack.pop()#匹配右括号出栈
                else: return False
            else: stack.append(i)#对左括号进行压栈
            
        return not stack
```

# 九宫格按键输入

题目描述
九宫格按键输入，判断输出，有英文和数字两个模式，默认是数字模式，数字模式直接输出数字，英文模式连续按同一个按键会依次出现这个按键上的字母，如果输入"/"或者其他字符，则循环中断

1（,.） 2（abc） 3（def）
4（ghi） 5（jkl） 6（mno）
7（pqrs） 8（tuv） 9（wxyz）
#0（空格） /

要求输入一串按键，输出屏幕显示

输入描述：
输入范围为数字 0~9 和字符’#'、 ’/’，输出屏幕显示，例如，
在数字模式下，输入 1234，显示 1234
在英文模式下，输入 1234，显示,adg

输出描述：
1、 #用于切换模式，默认是数字模式，执行#后切换为英文模式；
2、 /表示延迟，例如在英文模式下，输入 22/222，显示为 bc；
3、英文模式下，多次按同一键，例如输入 22222，显示为 b；

示例 1
输入： 123#222235/56
输出： 123adjjm

参考代码

```python
dct = {
    "1": ',.',
    "2": 'abc',
    "3": 'def',
    "4": 'ghi',
    "5": 'jkl',
    "6": 'mno',
    "7": 'pqrs',
    "8": 'tuv',
    "9": 'wxyz',
}


def func(nums):
    if nums and nums[0] in dct:
        que = dct[nums[0]]
        return que[len(nums) % len(que)-1]
    else:
        return nums


while 1:
    try:
        nums = input()

        # True 数字
        # False 拼音
        flg = True

        ret = ""
        bef = ""
        for c in nums:
            if c == "#":
                flg = not flg
                ret += func(bef)
                bef = ""
            elif flg:
                ret += func(bef)
                bef = ""
                ret += c
            else:
                if c == "/":
                    ret += func(bef)
                    bef = ""
                elif bef and c == bef[-1]:
                    bef += c
                else:
                    ret += func(bef)
                    bef = c
        if bef:
            ret += func(bef)

        print(ret)
    except Exception as e:
        break
```

```python

```



# 书籍叠放问题

# 题目描述

假设书本的叠放有这样的规则，当A书的长度和宽度都大于B书时，可以将其B书置于A的上方，堆叠摆放，请设计一个程序，根据输入的书本长宽，计算最多可以堆叠摆放多少本书？

示例 1
输入：
3
16 15
13 12
15 14

输出：
3

说明
这里代表有3本书，第1本长宽分别为16和15，第2本长宽为13和12，第3本长宽为15和14，它们可以按照 [13, 12],[15, 14],[16,15] 的顺序堆叠，所以输出3

参考代码

```python
while 1:
    try:
        n = int(input())

        nums = [list(map(int, input().split())) for _ in range(n)]
        dp = []
        lens = len(nums)

        def dfs(sub, count=1):
            dp.append(len(sub))
            for i in range(lens):
                if i not in sub:
                    if nums[i][0] < nums[sub[-1]][0] and nums[i][1] < nums[sub[-1]][1]:
                        dfs(sub+[i], count+1)

        for i in range(lens):
            dfs([i])

        print(max(dp))
    except Exception as e:
        break

```



# 分糖果

**输入描述：**
抓取的糖果数（<10000000000）：15

**输出描述：**
最少分至一颗糖果的次数：5

示例 1

输入：15
输出：5

> 解释：
> (1) 15+1=16;
> (2) 16/2=8;
> (3) 8/2=4;
> (4) 4/2=2;
> (5) 2/2=1;

参考代码

递归求解
结束条件
f(2) = 1

```python
def dfs(n):
    if n == 1:
        return 0
    if n == 2:
        return 1
    if n % 2:
        # 取出、放回 + 平均分配 共两次
        return min((dfs((n-1)/2) + 2, dfs((n+1)/2) + 2))
    else:
        # 平均分配均记一次
        return dfs(n/2) + 1

while 1:
    try:
        m = int(input())
        print(dfs(m))
    except Exception as e:
        break
```



# 圆桌队列

题目描述
一组人（n 个），围成一圈，从某人开始数到第三个的人出列，再接着从下一个人开始数，最终输出最终出列的人。

约瑟夫环是一个数学的应用问题：已知 n 个人（以编号 1，2， 3…n 分别表示） 围坐在一张圆桌周围。从编号为 k 的人开始报数，数到 m 的那个人出列；他的下一个人又从 1 开始报数，数到 m 的那个人又出列；依此规律重复下去，直到圆桌周围的人全部出列。

示例 1

输入：

```python
10
3
3
```

输出：

```python
7
```

```python
def jsloop(n,s,k):
    p = s - 1
    for i in range(2,n-k+2):
        p = (p + k) % i
    return p + 1
print(jsloop(10,3,3))
```



# HJ61 放苹果

题目描述
把m个同样的苹果放在n个同样的盘子里，允许有的盘子空着不放，问共有多少种不同的分法？

注意：如果有7个苹果和3个盘子，（5，1，1）和（1，5，1）被视为是同一种分法。

数据范围：
0≤m≤10 ，1≤n≤10 。

输入描述：
输入两个int整数

输出描述：
输出结果，int型

示例 1
输入：
7 3
输出：
8

说明：
放苹果分为两种情况，一种是有盘子为空，一种是每个盘子上都有苹果。
令(m,n)表示将m个苹果放入n个盘子中的摆放方法总数。
1.假设有一个盘子为空，则(m,n)问题转化为将m个苹果放在n-1个盘子上，即求得(m,n-1)即可
2.假设所有盘子都装有苹果，则每个盘子上至少有一个苹果，即最多剩下m-n个苹果，问题转化为将m-n个苹果放到n个盘子上
即求(m-n，n)

参考代码

```python
def dfs(m, n):
    if m<0 or n<0:
        return 0
    elif m==1 or n==1:
        return 1
    else:
        return dfs(m,n-1) + dfs(m-n,n)

while 1:
    try:
        m, n = map(int, input().split())
        
        print(dfs(m, n))
    except:
        break
```



# 分苹果

题目描述
A,B两个人把苹果分为两堆，A希望按照它的计算规则等分苹果，他的计算规则是按照二进制加法计算，并且不计算走位，12+5=9（1100+0101=9），
B的计算规则是十进制加法，包括正常进位，B希望在满足A的情况下获取苹果重量最多，输入苹果的数量和每个苹果重量，输出满足A的情况下获取的苹果总重量，
如果无法满足A的要求，输出-1。

输入描述：
输入第一行是苹果数量：3
输入第二行是每个苹果重量：3 5 6

输出描述：
输出第一行是B获取的苹果总重量：11

示例 1
输入：
3
3 5 6
输出：
11

示例 2
输入：
8
7258 6579 2602 6716 3050 3564 5396 1773
输出：
35165

参考代码

```python
while 1:
    try:
        n = int(input())
        nums = list(map(int, input().split()))
        max_ = sum(nums)

        dp = []

        def get_w(sub):
        	# 按照二进制加法计算，并且不计算走位， 计算重量
            t = 0
            for n in set(nums).difference(sub):
                # 按照二进制加法计算
                t = t ^ n
            return t

        def dfs(w, sub):
        	# w 是已经分过得(A)，按照二进制加法计算的重量
        	# sub 是未分配的苹果 (B)
            if w == get_w(sub):
                tot = sum(sub)
                # 取最大实际重量 保存在 dp
                dp.append(max(tot, max_ - tot))
                return
            elif len(sub) == len(nums) - 1:
                # 不能把苹果全部分给 某一堆
                return
            else:
                # 生成可能的 堆组合，计算进行重量比较
                for wi in nums:
                    if wi not in sub:
                        dfs(w ^ wi, sub.union({wi}))

        for w in nums:
            dfs(w, {w})

        if dp:
            print(max(dp))
        else:
            print(-1)
    except Exception as e:
        break

```



# 幼儿园找出同班的小朋友

# 题目描述

幼儿园两个班的小朋友在排队时混在了一起，每位小朋友都知道自己是否与前面一位小朋友同班，请你帮忙把同班的小朋友找出来。

小朋友的编号是整数，与前一位小朋友同班用Y表示，不同班用N表示。

输入描述：
输入为空格分开的小朋友编号和是否同班标志。

输出描述：
输出为两行，每一行记录一个班小朋友的编号，编号用空格分开，且：

编号需按照升序排列。
若只有一个班的小朋友，第二行为空行。
示例 1
输入：1/N 2/Y 3/N 4/Y
输出：
1 2
3 4

参考代码
准备两个列表A和B, 分别保存两个班的学生
第一个输入的同学默认保存在A
后面的以此判断N Y 确定要保存的列表

```python

while 1:
    try:
        nums = input().split()

        # 第一个同学
        start = nums[0].split('/')
        A = [start[0]]
        B = []

        temp = [A, B]  # 表示前一个同学的班级， 默认是 A 班级
        # 从第二个同学开始判断
        for n in nums[1:]:
            id_, f = n.split("/")

            if f == "Y":
                temp = temp
            else:
                # 不在一个班，需要交换班级顺序
                temp = temp[::-1]

            temp[0].append(id_)

        if A:
            print("".join(sorted(A, key=lambda x: int(x))))
        if B:
            print("".join(sorted(B, key=lambda x: int(x))))
    except Exception as e:
        print(traceback.print_exc())
        break

```

# 翻转单词

# 题目描述

给一个由英文单字和空格组成的[字符串](https://so.csdn.net/so/search?q=字符串&spm=1001.2101.3001.7020)，如：I am a student ,翻转[1 3]之间的单词，即得：I student a am .
n是翻转得下标，n,m给定的，如果n小于0,则从下标为0开始翻转，如果m大于等于字符转单词下标，则翻转到最后一个单词。

输入描述：
第一行是待处理的字符串
第二行翻转下标n, m

输出描述：
输出翻转后的字符串

示例 1
输入：
I am a boy she is girl
-1 4
输出：
she boy a am I is girl

参考代码
就行这种题也没有啥也解释的吧
按下标切片
对n-m 使用 [::-1]翻转
格式化输出

```python
while 1:
    try:
        s = input().split()
        n, m = list(map(int, input().split()))
        if n >= m:
            print(s)
        else:
            if n < 0:
                n = 0
            if m > len(s):
                m = len(s) - 1

            s0 = s[:n]
            s1 = s[n:m+1]
            s2 = s[m+1:]
            print(" ".join(s0 + s1[::-1] + s2))
    except Exception as e:
        break

```



# 停车位问题

# 题目描述

有一横排车位，有至少一个车位停了车，也至少有一个车位没停车。一个车位有车用1表示，无车用0表示。为了避免剐蹭，请为司机规划停在哪个车位，距离其他车中间间隔的车位最远。

输入描述：
一组数据，代表目前车位的状态。

输出描述：
当前车辆停车距离其他车辆的最大间距。

示例 1
输入：1 0 0 0 0 1 0 1 0
输出：3

新来的车停在 下标为1或者小标为4的位置，才会出现距离其他车辆最大距离3

参考代码
可以理解为找最多连续0的个数，在这个位置中停车就是距离其他车中间间隔的车位最远的位置

```python
while 1:
    try:
        nums = input().split()

        max_ = 0
        i = 0
        j = 1
        while j < len(nums):
            if nums[i] == nums[j] == "0":
                j += 1
            else:
            	# 最大连续0的个数
                max_ = max([max_, j - i])
                i = j
                j = j + 1
        else:
            max_ = max([max_, j - i])

        # 减去一个当前车的位置
        print(max_ - 1)
    except Exception as e:
        break
```



# 单词接龙游戏

题目描述
单词接龙的规则是：
用于接龙的单词首字母必须要前一个单词的尾字母相同；
当存在多个首字母相同的单词时，取长度最长的单词，如果长度也相等，则取字典序最小的单词；
已经参与接龙的单词不能重复使用。
现给定一组全部由小写字母组成单词数组，并指定其中的一个单词作为起始单词，进行单词接龙。

请输出最长的单词串，单词串是单词拼接而成，中间没有空格。

输入描述：
输入的第一行为一个非负整数，表示起始单词在数组中的索引K，0 <= K < N。
输入的第二行为一个非负整数，表示单词的个数N。
接下来的N行，分别表示单词数组中的单词。

输出描述：
输出一个字符串，表示最终拼接的单词串。

备注：
单词个数N的取值范围为[1, 20]
单个单词的长度的取值范围为[1, 30]

示例 1
输入：
0
6
word
dd
da
dc
dword
d

输出：
worddwordda

说明：
先确定起始单词word，再接以d开头的且长度最长的单词dword，剩余以d开头且长度最长的有dd、da、dc，则取字典序最小的da，所以最后输出worddwordda。

示例 2
输入：
4
6
word
dd
da
dc
dword
d

输出：
dwordda

说明：
先确定起始单词dword，剩余以d开头且长度最长的有dd、da、dc，则取字典序最小的da，所以最后输出dwordda。

```python
while 1:
    try:
        k = int(input())
        n = int(input())

        nums = [input() for _ in range(n)]

        # 接龙结果
        # 第一个单词是已知的，如：dword
        dp = [nums.pop(k)]

        while len(nums):
            # 备选单词
            cache = []
            for w in nums:
                # 上一个单词的尾字符 == 当前单词的首字符
                # cache 保存满足要求的所有备选单词
                # dd、da、dc
                if dp[-1][-1] == w[0]:
                    cache.append(w)
            if cache:
                # 对备选单词排序，取字典序最小的da
                cache = sorted(cache, key=lambda x: (-len(x), x))
                next_ = cache[0]
                nums.remove(next_)
                dp.append(next_)
            else:
                break

        print("".join(dp))
    except Exception as e:
        break

```

# 射击比赛排名

题目描述
给定一个射击比赛成绩单，包含多个选手若干次射击的成绩分数，请对每个选手按其最高三个分数之和进行降序排名，输出降序排名后的选手id序列。

题目解析：

给一个数字表示射击的次数，然后给几个选手进行（乱序）射击，生成对应的成绩！
条件如下：

一个选手可以有多个射击成绩的分数，且次序不固定
如果一个选手成绩少于3个，则认为选手的所有成绩无效，排名忽略该选手
如果选手的成绩之和相等，则相等的选手按照其id降序排列

输入描述：

输入第一行，一个整数N，表示该场比赛总共进行了N次射击，产生N个成绩分数（2<=N<=100）
输入第二行，一个长度为N整数序列，表示参与每次射击的选手id（0<=id<=99）
输入第三行，一个长度为N整数序列，表示参与每次射击选手对应的成绩（0<=成绩<=100）

输出描述：

符合题设条件的降序排名后的选手ID序列

示例 1
输入：
13
3,3,7,4,4,4,4,7,7,3,5,5,5
53,80,68,24,39,76,66,16,100,55,53,80,55

输出：
5,3,7,4

说明:
该场射击比赛进行了13次，参赛的选手为{3,4,5,7}
3号选手成绩53,80,55 最高三个成绩的和为188
4号选手成绩24,39,76,66 最高三个成绩的和为205
5号选手成绩53,80,55 最高三个成绩的和为188
7号选手成绩68,16,100 最高三个成绩的和为184
比较各个选手最高3个成绩的和，有4号>3号=5号>7号，由于3号和5号成绩相等，且id 5>3，所以输出 7,5,3,4

参考代码

```python
while 1:
    try:
        n = int(input())
        ids = input().split(',')
        nums = list(map(int, input().split(',')))

        dct = {}
		# 关联用户和成绩
        for i in range(n):
            id_ = ids[i]
            if id_ in dct:
                dct[id_].append(nums[i])
            else:
                dct[id_] = [nums[i]]

        dp = []
        # 剔除成绩少于三个的用户，并只保留最高三次成绩
        for k, v in dct.items():
            if len(v) >= 3:
                dp.append((k, sorted(v, reverse=True)[:3]))

		# 按最高三次成绩和排序
        dp = sorted(dp, key=lambda x: sum(x[1]), reverse=True)
        print(",".join([k for k, v in dp]))
    except Exception as e:
        break

```



# 考古学家

# 题目描述

有一大堆墓碑碎片，每个碎片就是一个[字符串](https://so.csdn.net/so/search?q=字符串&spm=1001.2101.3001.7020)，让你给出排列组合的所有可能，要去重。

比如 a b c 这么3块墓碑,排列组合有：
abc acb bac bca cab cba 这么多种。

输入描述：
输入第一行表示墓碑碎片

输出描述：
输出不重复的墓碑组合

示例 1
输入：a b ab
输出：abab aabb baab baba abba

墓碑是 a b ab ，则 要记得去重，结果是 abab aabb baab baba abba , 其实还有一个 abab,因为和第一个重了，所以要去掉。

参考代码
就是一个不重复的排列组合题

```python
while 1:
    try:
        chars = input().split()

        dp = []

        # 对碎片进行组合
        def dfs(sub):
            if len(sub) == len(chars):
                t = "".join(sub)
                # 剔除重复的结果
                # 比如 abab 可以是 a + b + ab  或 ab + a + b
                if t not in dp:
                    dp.append(t)
            else:
                for c in chars:
                    # 剔除重复的碎片
                    if c not in sub:
                        dfs(sub + [c])

        for c in chars:
            dfs([c])

        print(" ".join(dp))
    except Exception as e:
        break

```



# 树形目录操作

# 题目描述

树形目录，m行输入，[当前目录ID 父级目录ID]。最后一行为待删除目录ID，要求删除父级同时删除下级目录。 输出剩余目录。

示例1
输入：

6
3 1
5 1
4 3
10 5
11 5
12 4
5
解释：树的结构，删除 5 的同时需要删除 10， 11。

          	1
           /  \
         3     5
        /     /  \
       4     10  11
      /
    12 
输出：

3 1
4 3
12 4
参考代码

```python
while 1:
    try:
        num = int(input())

        lst = []
        tree = {}
        for _ in range(num):
            child, parent = input().split()
            if parent in tree:
                tree[parent].append(child)
            else:
                tree[parent] = [child]
            lst.append((child, parent))


        def del_child(parent_id):
            if parent_id in tree:
                for child_id in tree.pop(parent_id):
                    lst.remove((child_id, parent_id))
                    del_child(child_id)


        del_id = input()
        del_child(del_id)

        for c in lst:
            if c[0] != del_id:
                print(" ".join(c))

    except Exception:
        break

```



# 数格子

# 题目描述

给定一个 n*n 二维整型[数组](https://so.csdn.net/so/search?q=数组&spm=1001.2101.3001.7020)，在二维数组上随机分布着0和1的值。现输入一个坐标点，输出坐标周围一圈一的个数，需要提供测试用例，说明测试的场景。

**输入描述：**
n*n 二维整型数组
x, y

**输出描述：**
x, y 坐标，周边一圈中有几个值为1

示例1

输入：

```
4
1 1 1 0
1 1 1 1
1 0 1 1
1 1 1 1
0 3
```

输出：

```
3
```

```python

while 1:
    try:
        n = int(input())

        lst = []
        for _ in range(n):
            lst.append(list(map(int, input().split())))

        x, y = list(map(int, input().split()))

        count = 0
        for i in range(x - 1, x + 1 + 1):
            for j in range(y - 1, y + 1 + 1):
                # 中心点
                if i == x and j == y:
                    pass
                elif 0 <= i < len(lst) and 0 <= j < len(lst[0]):
                    count += lst[i][j]
        print(count)
    except Exception:
        break

```



# 黑板上的数字涂颜色

题目描述
疫情过后，希望小学终于又重新开学了，三年二班开学第一天的任务是将后面的黑板报重新制作。黑板上已经写了N个正整数，同学们需要给每个数分别上一种颜色。为了让黑板报即美观又有学习意义，老师要求同种颜色的所有数都可以被这种颜色中最小的那个数整除。现在请你帮帮小朋友们，算算最少需要多少种颜色才能给这N个数进行上色。

输入描述：
第一行有一个正整数N，其中1<=N<=100。

第二行有N个int型数（保证输入数据在[1,100]范围中），表示黑板上各个正整数的值。

输出描述：
输入只有一个整数，为最少需要的颜色种数。

示例 1
输入：
3
2 4 6

输出：
1

说明：
所有数都能被2整除

输入：
4
2 3 4 9
输出：
2

说明：
2与4涂一种颜色，4能被2整除
3与9涂另一种颜色，9能被3整除
不能涂同一种颜色

# 参考代码

```python
while 1:
    try:
        _ = input()
        nums = list(map(int, input().split()))

        dp = []
        while nums:
            # 取最小值 并将最小值的倍数 分为一组
            min_ = min(nums)
            ca = []
            i = 0
            while i < len(nums):
                if nums[i] % min_ == 0:
                    ca.append(nums[i])
                    nums.pop(i)
                else:
                    i += 1
            dp.append(ca)

        print(len(dp))
    except Exception as e:
        break

```



# 热点网站统计

题目描述
企业路由器的统计页面，有一个功能需要动态统计公司访问最多的网页 URL top N。

请设计一个算法，可以高效动态统计 Top N 的页面。

输入描述：
每一行都是一个 URL 或一个数字，如果是 URL，代表一段时间内的网页访问；

如果是一个数字 N，代表本次需要输出的 Top N 个 URL。

输入约束：
1、总访问网页数量小于 5000 个，单网页访问次数小于 65535 次；
2、网页URL 仅由字母，数字和点分隔符组成，且长度小于等于 127 字节；
3、数字是正整数，小于等于 10 且小于当前总访问网页数；

输出描述：
每行输入要对应一行输出，输出按访问次数排序的前 N 个 URL，用逗号分隔。

输出要求：
1、每次输出要统计之前所有输入，不仅是本次输入；
2、如果有访问次数相等的 URL，按 URL 的字符串字典序升序排列，输出排序靠前的 URL；

输入：
news.qq.com
news.sina.com.cn
news.qq.com
news.qq.com
game.163.com
game.163.com
www.huawei.com
www.cctv.com
3
www.huawei.com
www.cctv.com
www.huawei.com
www.cctv.com
www.huawei.com
www.cctv.com
www.huawei.com
www.cctv.com
www.huawei.com
3

输出：
news.qq.com,game.163.com,news.sina.com.cn
www.huawei.com,www.cctv.com,news.qq.com

示例 1

```python
lp = []
while True:
    try:
        inc = input()
        if inc[0].isalpha():
            lp.append(inc)
        else:
            out1 = sorted(lp,key=lambda x:-lp.count(x))
            out2 = list(set(out1))
            out2.sort(key=out1.index)
            # print(out2[0:int(inc)],inc)
            print(",".join(out2[0:int(inc)]))
    except:
        break
```



# 路灯问题

题目描述
在一段路径上，一共有N(N ∈ [1, 100000])座路灯，已知路灯间距均为100m。现给出每个路灯的照亮范围light[i],且照亮范围在区间[1, 100 * N]内。

求出该段路中，总共未照亮的路段长度。
N = 6, light[] = [50,20,80,20,30,300]

示例 1
输入：
6
50,20,80,20,30,300
输出：
30

示例 2
输入：
4
50,70,20,70
输出：
20

示例 3
输入：
6
50,20,60,20,30,230
输出：
60

示例 4
输入：
6
50,20,60,120,30,230
输出：
50

参考代码
以 50,70,20,70 为例

| \|   | \|   | \|   | \|   |
| ---- | ---- | ---- | ---- |
| 0    | 1    | 2    | 3    |
i=0 时 向右侧能照亮50，dp[0] = 50；
i=1 时 向左侧能照亮70， 向右侧能照亮70 , dp[0] = 50+70, dp[1] = 70;
i=2 时 向左侧能照亮20， 向右侧能照亮20, dp[1] = 70+20, dp[2] = 20;
i=3 时 向左侧能照亮70，dp[2] = 20+70;

dp = [100, 90, 90]

```python
while 1:
    try:
        n = int(input())
        nums = list(map(int, input().split(",")))

        def funcl(i, w):
            if w > 0 and i >= 0:
                nums[i] = max(nums[i], w)
                funcl(i - 1, w - 100)

        def funcr(i, w):
            if w > 0 and i < n-1:
                nums[i] = max(nums[i], w)
                funcr(i + 1, w - 100)

        for i in range(n):
            funcl(i-1, nums[i]-100)
            funcr(i, nums[i])

        ret = 0
        r = nums[0]
        for l in nums[1:]:
            ret += 100 - min(100, r+l)
            r = l
        print(ret)
    except Exception as e:
        break
```



# 消消乐游戏

题目描述
游戏规则：输入一个只包含英文字母的字符串，字符串中的两个字母如果相邻且相同，就可以消除。

在字符串上反复执行消除的动作，直到无法继续消除为止，此时游戏结束。

输出最终得到的字符串长度。

示例 1
输入：
aaaaaaa

输出：
1

示例 2
输入：
abcaacba

输出：
0

参考代码

```python
while 1:
    try:
        inp = input()
        i = 0
        while i < len(inp)-1:
            if inp[i] == inp[i+1]:
                inp = inp[:i] + inp[i+2:]
                i = max(0,i-1)#加速，0可以
            else:
                i += 1
        print(inp)
    except:
        break
```



# 火星文计算

题目描述
已知火星人使用的运算符为#、$，

其与地球人的等价公式如下：

x#y = 2x+3y+4
x$y = 3*x+y+2

其中x、y是无符号整数

地球人公式按C语言规则计算，火星人公式中，$的优先级高于#，相同的运算符，按从左到右的顺序计算 现有一段火星人的字符串报文，请你来翻译并计算结果。

输入描述：

火星人字符串表达式（结尾不带回车换行）

输入的字符串说明：

字符串为仅由无符号整数和操作符（#、$）

组成的计算表达式。例如：123#45#6778

用例保证字符串中，操作数与操作符之间没有任何分隔符。
用例保证操作数取值范围为32位无符号整数。
保证输入以及计算结果不会出现整型溢出。
保证输入的字符串为合法的求值报文，例如：123#45#6778
保证不会出现非法的求值报文，例如类似这样字符串：
#4$5 //缺少操作数
4$5# //缺少操作数
4#$5 //缺少操作数
4 $5 //有空格
3+4-5*6/7 //有其它操作符
12345678987654321$54321 //32位整数计算溢出

输出描述：

根据输入的火星人字符串输出计算结果（结尾不带回车换行）

示例 1
输入：
7#6$5#12

输出：
226

> 说明：
> 7#6$5#12
> =7#(3\*6+5+2)#12
> =7#25#12
> =(2\*7+3\*25+4)#12
> =93#12
> =2\*93+3\*12+4
> =226

```python
while 1:
    try:
        nums = input()
        # 无符号数
        index_list = [i for i, c in enumerate(nums) if c in "#$"]

		# 按操作符 分隔字符串
        stack = []
        s = 0
        for e in index_list:
            stack.append(int(nums[s:e]))
            stack.append(nums[e])
            s = e + 1
        else:
            stack.append(int(nums[s:]))

		# 计算 优先级高的$ f"3*{x}+{y}+2")
		r_stack = []
        while stack:
            item = stack.pop()
            if item == "$":
                x = stack.pop()
                y = r_stack.pop()
                stack.append(eval(f"3*{x}+{y}+2"))
            else:
                r_stack.append(item)
        stack += r_stack[::-1]

		# 计算 2*{x}+3*{y}+4"
        x = stack[0]
        for i in range(2, len(stack), 2):
            y = stack[i]
            x = eval(f"2*{x}+3*{y}+4")
        print(x)
    except Exception as e:
        break
```



# 数大雁

题目描述
题目详解见 【华为机试真题 Python实现】数大雁 题目详解
一群大雁往南飞，给定一个字符串记录地面上的游客听到的大雁叫声，请给出叫声最少由几只大雁发出。

具体的:

大雁发出的完整叫声为”quack“，因为有多只大雁同一时间嘎嘎作响，所以字符串中可能会混合多个”quack”。
大雁会依次完整发出”quack”，即字符串中’q’ ,‘u’, ‘a’, ‘c’, ‘k’ 这5个字母按顺序完整存在才能计数为一只大雁。如果不完整或者没有按顺序则不予计数。
如果字符串不是由’q’, ‘u’, ‘a’, ‘c’, ‘k’ 字符组合而成，或者没有找到一只大雁，请返回-1。

输入描述：
一个字符串，包含大雁quack的叫声。1 <= 字符串长度 <= 1000，字符串中的字符只有’q’, ‘u’, ‘a’, ‘c’, ‘k’。

输出描述：
大雁的数量

示例 1
输入：
quackquack

输出：
1

示例 2
输入：
qaauucqckk

输出：
-1

示例 3
输入：
quacqkuac

输出：
1

示例 4
输入：
qququaauqccauqkkcauqqkcauuqkcaaukccakkck

输出：
5

参考代码

![](/home/ljxnb/lib/leetcode/81d8abb6ae2746c8b5e4a523c623f42f.png)

模拟几只大雁可能是按上面的顺序发声，收集到的叫声可能是下面的字符串：
qququaauqccauqkkcauqqkcauuqkcaaukccakkck

怎么确认最少的个数呢，我们找到第一个q和第一个k下标，在这个范围内存在q的数量且q可以在后面的字符串中构成完成的quack时，就可以记为一个大雁

```python
while 1:
    try:
        chars = input()
        base = "quack"
        states = [0] * len(base)
        dp = []
        for i in range(len(chars)):
            index = base.index(chars[i])
            if index == 0:
                states[index] += 1
            else:
                if states[index - 1]:
                    states[index - 1] -= 1
                    states[index] += 1

                if base[-1] == chars[i]:
                    if states[-1] != 0:
                        # 计算剩余字符串是否满足剩余的叫声
                        temp = [t for t in states]
                        temp[-1] = 0
                        max_ = sum(temp)
                        for j in range(i, len(chars)):
                            index = base.index(chars[j])
                            if temp[index - 1]:
                                temp[index - 1] -= 1
                                temp[index] += 1
                            if temp[-1] == max_:
                                break
                        dp.append(temp[-1] + 1)
                        states[-1] -= 1

        print(max(dp) if dp else -1)
    except Exception as e:
        break

```



# 敏感字段加密

题目描述
给定一个由多个命令字组成的命令字符串：

字符串长度小于等于127字节，只包含大小写字母，数字，下划线和偶数个双引号；
命令字之间以一个或多个下划线_进行分割；
可以通过两个双引号””来标识包含下划线_的命令字或空命令字（仅包含两个双引号的命令字），双引号不会在命令字内部出现；
请对指定索引的敏感字段进行加密，替换为\*\*\*\*\*\*（6个*），并删除命令字前后多余的下划线_。
如果无法找到指定索引的命令字，输出字符串ERROR。

输入描述：
输入为两行，第一行为命令字索引K（从0开始），第二行为命令字符串S。

输出描述：
输出处理后的命令字符串，如果无法找到指定索引的命令字，输出字符串ERROR

示例 1
输入：
1
password__a12345678_timeout_100

输出：
password\_\*\*\*\*\*\*\_timeout_100

示例 2
输入：

2
aaa_password_"a12_45678"timeout__100""
输出：
aaa_password\_\*\*\*\*\*\*timeout_100""

参考代码

```python
while 1:
    try:
        k = int(input())
        nums = input()

        dp = []
        temp = ""
        for c in nums:
            # 按 下换线 双引号 拆分字符串
            # 子串中有同时有 下换线 双引号 的特殊处理
            if c == "_":
                if '"' not in temp and temp:
                    dp.append(temp)
                    temp = ""
                elif '"' in temp:
                    temp += c
            elif c == '"':
                if temp and '"' in temp:
                    temp += c
                    dp.append(temp)
                    temp = ""
                elif temp and '"' not in temp:
                    dp.append(temp)
                    temp = c
                elif not temp:
                    temp = c
            else:
                temp += c
        if temp:
            dp.append(temp)

        # 替换指定下标的 子串为 6个星号
        if k < len(dp):
            nums = nums.replace(dp[k], "*" * 6)
            while "__" in nums:
                nums = nums.replace("__", "_")
            print(nums)
        else:
            print("ERROR")
    except Exception as e:
        break

```

```python
while 1:
    try:
        k = int(input())
        nums = input()
        while "__" in nums:
            nums = nums.replace("__", "_")
        nums = list(nums.split("_"))
        out = nums[k]
        i = 0
        while "\"" in out and out.count("\"") %2 != 0:
            i += 1
            out += nums[k+i]
        else:
            if "\"" in out:
                temp = list(out.split("\""))
                for i in range(1,len(temp),2):
                    temp[i] = "*"*6
                out = "".join(temp)
                print(out)
            else:
                out = "*"*6

        print("_".join(nums[:k]+[out]+nums[k+i+1:]))
    except Exception as e:
        break
```



# We Are A Team

题目描述
总共有 n 个人在机房，每个人有一个标号（1<=标号<=n），他们分成了多个团队，需要你根据收到的 m 条消息判定指定的两个人是否在一个团队中，具体的：
1、消息构成为 a b c，整数 a、b 分别代表两个人的标号，整数 c 代表指令
2、c == 0 代表 a 和 b 在一个团队内
3、c == 1 代表需要判定 a 和 b 的关系，如果 a 和 b 是一个团队，输出一行’we are a team’,如果不是，输出一行’we are not a team’
4、c 为其他值，或当前行 a 或 b 超出 1~n 的范围，输出‘da pian zi’

输入描述：
1、第一行包含两个整数 n，m(1<=n,m<100000),分别表示有 n 个人和 m 条消息
2、随后的 m 行，每行一条消息，消息格式为：a b c(1<=a,b<=n,0<=c<=1)

输出描述:
1、c ==1,根据 a 和 b 是否在一个团队中输出一行字符串，在一个团队中输出‘we are a team’,不在一个团队中输出’we are not a team’
2、c 为其他值，或当前行 a 或 b 的标号小于 1 或者大于 n 时，输出字符串‘da pian zi’
3、如果第一行 n 和 m 的值超出约定的范围时，输出字符串’Null’

示例 1
输入：

```
5 7
1 2 0
4 5 0
2 3 0
1 2 1
2 3 1
4 5 1
1 5 1
```

输出：

```python
We are a team
We are a team
We are a team
We are not a team
```

示例 2

输入：

```python
5 6
1 2 0
1 2 1
1 5 0
2 3 1
2 5 1
1 3 2
```

输出：

```python
we are a team
we are not a team
we are a team
da pian zi
```

# 参考代码

```python
while 1:
    try:
        n, m = list(map(int, input().split()))

        nums = []
        for _ in range(m):
            tmp = input().split()
            a = int(tmp[0])
            b = int(tmp[1])
            com = tmp[2]
            nums.append((a, b, com))

        dp = []
        for a, b, com in nums:
            if not (1 <= a <= n and 1 <= b <= n):
                print("da pian zi")
                continue
            if com == "0":
                if dp:
                    for idx, val in enumerate(dp):
                        if val.intersection({a, b}):
                            dp[idx] = val.union({a, b})
                            break
                    else:
                        dp.append({a, b})
                else:
                    dp.append({a, b})
            elif com == "1":
                for i in dp:
                    if i.intersection({a, b}) == {a, b}:
                        print("we are a team")
                        break
                else:
                    print("we are not a team")
            else:
                print("da pian zi")
    except Exception as e:
        break

```



# 勾股数元祖

题目描述
如果 3 个正整致(a,b,c)满足a^2+b^2=c^2的关系,则称(a,b, c)为勾股数(著名的勾三股四弦五) ,为了探索勾股数的规律,我们定义如果勾股数(a,b,c)之间两两互质(即 a 与 b, a 与 c, b 与 c之间均互质) ,则其为勾股数元祖(例(3,4,5)是勾股数元祖, (6,8,10)则不是勾股数元祖) .请求出始定范围[N,M]内,所有的勾股数元祖。

输入描述：
起始范围 N, 1 <= N <= 200
结束范围 M, N<=M<= 200

输出描述：

a,b, c 请保证 a <b <c,输出格式:a b c;
多组勾股数元祖请按照 a 升序,b 升序,最后 c 升序的方式排序输出;
给定范围中如果找不到勾股数元担时,输出"NA"。
备注：

[N,M]范围表示 a,b,c 三个数均在[N,M]范围。
示例 1
输入：
1
20

输出：
3 4 5
5 12 13
8 15 17

参考代码
互质最大公约数为1

```python
def ac(a, b):
    # 返回最大公约数
    mod = 2  # 随便定义一个余数
    while mod != 0:  # 当余数等于0是最大公约数
        mod = a % b
        a = b
        b = mod
    return a

while 1:
    try:
        n = int(input())
        m = int(input())

        for a in range(n, m + 1):
            for b in range(a + 1, m + 1):
                for c in range(b + 1, m + 1):
                    # 互质 最大公约数为1
                    if a * a + b * b == c * c and ac(a, b) == ac(b, c) == ac(a, c) == 1:
                        print((a, b, c))

    except Exception as e:
        break

```

# 最长方连续方波信号

# 题目描述

输入一串方波信号，求取最长的完全连续交替方波信号，并将其输出，如果有相同长度的交替方波信号，输出任一即可，方波信号高位用1标识，低位用0标识，如图：

![](/home/ljxnb/lib/leetcode/875c29a54d174329bde7f0e7149aa146.png)

说明：

一个完整的信号一定以0开始然后以0结尾，即010是一个完整信号，但101，1010，0101不是
输入的一串方波信号是由一个或多个完整信号组成
两个相邻信号之间可能有0个或多个低位，如0110010，011000010
同一个信号中可以有连续的高位，如01110101011110001010，前14位是一个具有连续高位的信号
完全连续交替方波是指10交替，如01010是完全连续交替方波，0110不是

输入描述:
输入信号字符串（长度>=3且<=1024）： 0010101010110000101000010
注：输入总是合法的，不用考虑异常情况

输出描述:
输出最长的完全连续交替方波信号串： 01010 若不存在完全连续交替方波信号串，输出 -1

示例 1
输入:
00101010101100001010010
输出:
01010

备注:
输入信号串中有三个信号：0010101010110(第一个信号段) 0001010(第二个信号段) 010(第三个信号段)
第一个信号虽然有交替的方波信号段，但出现了11部分的连续高位，不算完全连续交替方波，在剩下的连续方波信号串中01010最长

参考代码

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

```python
while 1:
    try:
        nums = input()

        # 提取信号段，由0分隔的信号段
        dp = []
        cache = []
        for c in nums:
            if cache and cache[-1] == "0" == c:
                dp.append("".join(cache))
                cache = [c]
            else:
                cache.append(c)
        else:
            if cache:
                dp.append("".join(cache))

        # 取不包含11，长度最大的子串
        # 出现了11部分的连续高位，不算完全连续交替方波
        sub = max(dp, key=lambda x: 0 if "11" in x else len(x))
        print(sub)
    except Exception as e:
        break

```

# 高效的任务规划

题目描述
你有n台机器编号为1~n，每台都需要完成完成一项工作，机器经过配置后都能完成独立完成一项工作。假设第i台机器你需要花B分钟进行设置，然后开始运行，J分钟后完成任务。现在，你需要选择布置工作的顺序，使得用最短的时间完成所有工作。

注意，不能同时对两台进行配置，但配置完成的机器们可以同时执行他们各自的工作。

输入描述:
第一行输入代表总共有M组任务数据(1 < M <= 10) 。
每组数据第一行为一个整数指定机器的数量N(0< N <= 1000)。
随后的N行每行两个整数，第一个表示B(0<= B <= 10000)，第二个表示J(0 <= J <=10000)。
每组数据连续输入，不会用空行分隔。各组任务单独计时。

输出描述:
对于每组任务，输出最短完成时间，且每组的结果独占一行。例如，两组任务就应该有两行输出。

示例1
输入：

1

1
2 2
输出：
4

参考代码
优先设置耗时最长的设备，总的耗时应该是最短的
我们使用一组数据，模拟下：

1

3
4 3
5 10
1 10

下面是集中执行顺序，当先执行更耗时的任务是，并发能力更好，总耗时最短，所以根据耗时排序，计算任务执行时间就可以

![](/home/ljxnb/lib/leetcode/8fb7dc8e738543b19a153b17693f8456.png)

```python
while 1:
    try:
        m = int(input())
        for i in range(m):

            n = int(input())
            nums = [list(map(int, input().split())) for j in range(n)]
            nums = sorted(nums, key=lambda x: x[1], reverse=True)

            tal = 0
            bal = 0
            while nums:
                b, j = nums.pop(0)
                if bal >= b:
                    bal -= b
                else:
                    t = b - bal
                    tal += t

                if bal < j:
                    t = j - bal
                    bal += t
                    tal += t

            print(tal)
    except Exception as e:
        break

```

# 素数之积

题目描述

[RSA](https://so.csdn.net/so/search?q=RSA&spm=1001.2101.3001.7020)加密算法在⽹络安全世界中⽆处不在，它利⽤了极⼤整数因数分解的难度，数据越⼤，安全系数越⾼，给定⼀个32位整数，请对其进⾏因数分解，找出是哪两个素数的乘积。

示例 1

输入：8633
输出：89 97

参考代码

```python
dp = {}

def func(n):
    # 判断是否是素数
    if n <= 1:
        return False
    elif n == 2:
        return True
    elif n % 2 == 0:
        return False

    for i in range(3, n, 2):
        if n % i == 0:
            return False
    dp[n] = True
    return True


while 1:
    try:
        num = int(input())

        for i in range(2, num//2):
            c = num // i
            if c >= i and c*i == num and func(c) and func(i):
                print(f"{i} {c}")
                break
        else:
            print(-1)
    except Exception as e:
        break

```



# 喊 7 的次数重排

题目描述
喊 7 是一个传统的聚会游戏， N 个人围成一圈，按顺时针从 1 到 N 编号。编号为 1 的人从
1 开始喊数，下一个人喊的数字为上一个人的数字加 1，但是当数字是 7 的倍数或者数字本
身含有 7 的话，要喊"过"。

现给定一个长度为 N 的数组，存储了打乱顺序的每个人喊"过"的次数，请把它还原成正确的顺序，即数组的第 i 个元素存储编号 i 的人喊"过"的次数。

输入描述：
输入为一行，为空格分隔的喊"过"的次数

输出描述：
输出为一行，还原游戏中 N 个人正确的顺序

示例 1
输入：
0 1 0
输出：
1 0 0

说明
一共只有一次喊"过"，那只会发生在需要喊 7 时，按顺序，编号为 1 的人会遇到 7，故输出
1 0 0。注意，结束时的 K 不一定是 7，也可以是 8、 9 等，喊过的次数都是 1 0 0。

示例 2
输入：
0 0 0 2 1
输出：
0 2 0 1 0

说明
一共有三次喊"过"，发生在 7 14 17，按顺序，编号为 2 的人会遇到 7 17，编号为 4 的人会遇到 14，故输出 0 2 0 1 0。

参考代码
思路是 模拟喊的过程；
我们使用 dp 保存 每个人喊 7 的次数；
当 dp 中的最大次数 不小与 输入 的最大次数时退出。

```python
while 1:
    try:
        nums = list(map(int, input().split()))
        lens = len(nums)
        max_ = sum(nums)

        dp = [0] * lens
        i = 1
        while sum(dp) < max_:
            if i % 7 == 0 or "7" in str(i):
                dp[(i % lens) - 1] += 1
            i += 1

    except Exception as e:
        break
```



# 输出第n项的结果

题目描述
有一个数列A[n]，从A[0]开始每一项都是一个数字，数列中A[n+1]都是A[n]的描述

其中A[0]=1

规则如下：
A[0]:1
A[1]:11 含义其中A[0]=1是1个1 即11
表示A[0]从左到右连续出现了1次1
A[2]:21 含义其中A[1]=11是2个1 即21
表示A[1]从左到右连续出现了2次1
A[3]:1211 含义其中A[2]从左到右是由一个2和一个1组成 即1211
表示A[2]从左到右连续出现了一次2又连续出现了一次1
A[4]:111221 含义A[3]=1211 从左到右是由一个1和一个2两个1 即111221
表示A[3]从左到右连续出现了一次1又连续出现了一次2又连续出现了2次1

**输入描述：**
需要输出第n项的结果
0<= n <=59

**输出描述：**
数列内容

示例 1

输入：4
输出：111221

参考代码

根据 题目提到的规则，从`1`，不断地模拟生成下次的字符串，

```python
def countnum(temp):
    t = [temp[0]]
    for c in temp[1:]:
        # 记录连续的子串
        if t[-1] == c:
            t.append(c)
        else:
            # 遇到不同的字符，生成 长度+字符的
            dp.append((len(t), t[0]))
            t = [c]
    if t:
        dp.append((len(t), t[0]))

while 1:
    try:
        n = int(input())
        if n == 0:
            print("1")
        else:
            dp = [(1, "1")]

            # 从1开始生成指定规则的字符串
            # 第二次 temp 是11
            # 第三次 temp 是21
            for i in range(1, n):
                temp = ""
                for count, v in dp:
                    temp += f"{count}{v}"
                dp = []
                countnum(temp)
            temp = ""
            for count, v in dp:
                temp += f"{count}{v}"
            print(temp)
    except Exception as e:
        break

```

# 字符串压缩

题目描述

通过键盘输入一串小写字母(a~z)组成的[字符串](https://so.csdn.net/so/search?q=字符串&spm=1001.2101.3001.7020)。请编写一个字符串压缩程序， 将字符串中连续出席的重复字母进行压缩，并输出压缩后的字符串。

压缩规则：

1. 仅压缩连续重复出现的字符。比如字符串"abcbc"由于无连续重复字符，压缩后的字符串还是"abcbc".
2. 压缩字段的格式为"字符重复的次数+字符"。例如：字符串"xxxyyyyyyz"压缩后就成为"3x6yz"

示例1:
输入：

aabcccccaaa
输出：

2ab5c3a
示例2:
输入：

abbccd
输出：

a2b2cd
示例 3:
输入：

IIIIeeeeOODDDssppppooQQQQppplllhU
输出：

4I4e2O3D2s4p2o4Q3p3lhU
参考代码

```python
def countnum(temp):
    dp = []
    t = [temp[0]]
    for c in temp[1:]:
        # 记录连续的子串
        if t[-1] == c:
            t.append(c)
        else:
            # 遇到不同的字符，生成 长度+字符的
            dp.append((len(t), t[0]))
            t = [c]
    if t:
        dp.append((len(t), t[0]))
    return "".join([str(i[0])+i[1] for i in dp])
```

# 跳格子游戏

题目描述
地上共有N个格子，你需要跳完地上所有的格子，但是格子间是有强依赖关系的，跳完前一个格子后，后续的格子才会被开启，格子间的依赖关系由多组steps数组给出，steps[0]表示前一个格子,steps[1]表示steps[0]可以开启的格子:

比如[0,1]表示从跳完第0个格子以后第1个格子就开启了，
比如[2,1]，[2,3]表示跳完第2个格子后第1个格子和第3个格子就被开启了

请你计算是否能由给出的steps数组跳完所有的格子,如果可以输出yes，否则输出no

输入描述:
输入一个整数N表示总共有多少个格子，接着输入多组二维数组steps表示所有格子之间的依赖关系

输出描述:
如果能按照steps给定的依赖顺序跳完所有的格子输出yes
否则输出no

示例 1

输入：
3
0 1
0 2
输出：
yes

> 说明
> 总共有三个格子[0,1,2]，跳完0个格子后第1个格子就开启了，跳到第0个格子后第2个格子也被开启了，按照0->1->2或者0->2->1的顺序都可以跳完所有的格子

示例 2

输入：
2
1 0
0 1
输出：
no

> 说明
> 总共有2个格子，第1个格子可以开启第0格子，但是第1个格子又需要第0个格子才能开启，相互依赖，因此无法完成

示例 3

输入：
6
0 1
0 2
0 3
0 4
0 5
输出：
yes

> 说明
> 总共有6个格子，第0个格子可以开启第1,2,3,4,5个格子，所以跳完第0个格子之后其他格子都被开启了，之后按任何顺序可以跳完剩余的格子

示例 4

输入：
5
4 3
0 4
2 1
3 2
输出：
yes

> 说明
> 跳完第0个格子可以开启格子4，跳完格子4可以开启格子3，跳完格子3可以开启格子2，跳完格子2可以开启格子1，按照0->4->3->2->1这样就跳完所有的格子

示例 5
输入：
4
1 2
1 0
输出：
yes

说明
总共4个格子[0,1,2,3]，格子1和格子3没有前置条件所以默认开启，格子1可以开启格子0和格子2，所以跳到格子1之后就可以开启所有的格子，因此可以跳完所有格子
备注:
1 <= N <500
steps[i].length=2
0<=step[i][0]，step[i][1]<N

参考代码

```python
while 1:
    try:
        n = int(input())

        nums = []
        for _ in range(n):
            x = input().split()
            if x and len(x) == 2:
                nums.append(tuple(map(int, x)))
            else:
                break

        nums = sorted(nums, key=lambda x: x[0])

        # 格子开启状态表
        state = [1] * n
        for start, end in nums:
            state[end] = 0

        # 没有起始的格子
        if max(state) == 0:
            print("no")
            continue

        dp = [0]

        def dfs(s, link):
        	# 所有节点都已开启
            if len(link) == len(nums) and sorted(link, key=lambda x: x[0]) == nums:
                dp.append(1)
                return

            ends = []
            for start_, end_ in nums:
                if s == start_:
                    link.append((start_, end_))
                    ends.append(end_)

            for end_ in ends:
                dfs(end_, link)

		# 从状态开启的节点，进行递归
        for i in range(n):
            if state[i] == 1:
                dfs(i, [])

        if max(dp):
            print("yes")
        else:
            print("no")

    except Exception as e:
        break

```

# 树形目录操作

# 题目描述

树形目录，m行输入，[当前目录ID 父级目录ID]。最后一行为待删除目录ID，要求删除父级同时删除下级目录。 输出剩余目录。

示例1

输入：

```python
6
3 1
5 1
4 3
10 5
11 5
12 4
5
```

> 解释：树的结构，删除 5 的同时需要删除 10， 11。

```python
        1
       /  \
     3     5
    /     /  \
   4     10  11
  /
12 
```

输出：

```python
3 1
4 3
12 4
```

参考代码

```python
while 1:
    try:
        num = int(input())

        lst = []
        tree = {}
        for _ in range(num):
            child, parent = input().split()
            if parent in tree:
                tree[parent].append(child)
            else:
                tree[parent] = [child]
            lst.append((child, parent))


        def del_child(parent_id):
            if parent_id in tree:
                for child_id in tree.pop(parent_id):
                    lst.remove((child_id, parent_id))
                    del_child(child_id)


        del_id = input()
        del_child(del_id)

        for c in lst:
            if c[0] != del_id:
                print(" ".join(c))

    except Exception:
        break
```

# 到达终点

题目描述
给定一个数组，从第一个开始，正好走到数组最后，所使用的最少步骤数。

要求：
第一步从第一元素开始，第一步小于len/2（len 为数组的长度）。从第二步开始，只能以所在成员的数字走相应的步数，不能多也不能少, 如果目标不可达返回-1，输出最少的步骤数，不能往回走。

示例 1

输入：7 5 9 4 2 6 8 3 5 4 3 9
输出：2

示例 2

输入：1 2 3 7 1 5 9 3 2 1
输出：-1

参考代码

```python
while 1:
    try:
        nums = list(map(int, input().split()))

        def dfs(n, c):
            if n == len(nums) - 1:
                return c
            elif n < len(nums):
                return dfs(n+nums[n], c+1)

        dp = []
        for i in range(len(nums)//2):
            t = dfs(i, 1)
            if t:
                dp.append(t)

        if dp:
            print(min(dp))
        else:
            print(-1)
    except Exception as e:
        break

```

# 转骰子

题目描述
骰子是一个立方体，每个面一个数字，初始为左 1，右 2，前 3（观察者方向），后 4，上 5，下 6，用 123456 表示这个状态，
放置到平面上，
可以向左翻转（用 L表示向左翻转 1 次），
可以向右翻转（用 R 表示向右翻转 1 次），
可以向前翻转（用 F 表示向前翻转 1 次），
可以向后翻转（用 B 表示向后翻转 1 次），
可以逆时针旋转（用 A表示逆时针旋转 90 度），
可以顺时针旋转（用 C 表示顺时针旋转 90 度），
现从 123456 这个初始状态开始，根据输入的动作序列，计算得到最终的状态。

骰子的初始状态和初始状态转动后的状态如图所示

![](/home/ljxnb/lib/leetcode/5f4f686d06154db08f3415e97d0730ee.png)

**输入描述：**
输入一行，为只包含 LRFBAC 的字母序列，最大长度 50，字母可重复
**输出描述：**
输出最终状态

示例1

输入：

```python
LR
```

输出：

```python
123456
```

参考代码
从 123456，向左翻转 L , 563421，34位置保持不变；
从 123456，向左翻转 R , 653412，34位置保持不变；
从 123456，向前翻转 F , 125643，12位置保持不变；
从 123456，向后翻转 B , 126534，12位置保持不变；
从 123456，向左翻转 A , 431256，56位置保持不变；
从 123456，向左翻转 C , 342156，56位置保持不变；

在拆解小 数组下表变化：
L：nums = nums[4:] + nums[2:4] + nums\[:2][::-1]
R：nums = nums\[4:][::-1] + nums[2:4] + nums[:2]
F：nums = nums[:2] + nums[4:] + nums\[2:4][::-1]
B：nums = nums[:2] + nums\[4:][::-1] + nums[2:4]
A：nums = nums\[2:4][::-1] + nums[:2] + nums[4:]
C：nums = nums[2:4] + nums\[:2][::-1] + nums[4:]

然后顺序执行控制指令就可以了

```python
while 1:
    try:

        nums = "123456"
        comm = input()
        for c in comm:
            if c == "L":
                nums = f"{nums[4:]}{nums[2:4]}{nums[:2][::-1]}"
            elif c == "R":
                nums = f"{nums[4:][::-1]}{nums[2:4]}{nums[:2]}"
            elif c == "F":
                nums = f"{nums[:2]}{nums[4:]}{nums[2:4][::-1]}"
            elif c == "B":
                nums = f"{nums[:2]}{nums[4:][::-1]}{nums[2:4]}"
            elif c == "A":
                nums = f"{nums[2:4][::-1]}{nums[:2]}{nums[4:]}"
            else:
                nums = f"{nums[2:4]}{nums[:2][::-1]}{nums[4:]}"
        print(nums)

    except Exception as e:
        break

```

# 藏宝图

# 题目描述

牛牛拿到了一个藏宝图，顺着藏宝图的指示，牛牛发现了一个藏宝盒，藏宝盒上有一个机关，机关每次会显示两个[字符串](https://so.csdn.net/so/search?q=字符串&spm=1001.2101.3001.7020) s 和 t。

根据古老的传说，牛牛需要每次都回答 t 是否是 s 的子序列。注意，子序列不要求在原字符串中是连续的。

例如串 abc，它的子序列就有{空串,a,b,c,ab,ac,bc,abc}8 种。

输入描述：
每个输入包含一个测试用例。每个测试用例包含两行长度不超过 10 的不包含空格的可见 ASCII 字符串。

输出描述：
输出一行“Yes”或者“No”表示结果。

示例1

输入：

```python
x.nowcoder.com
ooo
```

输出：

```python
Yes
```

参考代码

```python
while 1:
    try:
        a = input()
        b = input()
        i = 0
        dp = ""
        for c in set(b):
            if b.count(c) != a.count(c):
                print("No")
                break
        print("yes")
    except Exception as e:
        break
```

猴子吃桃

题目描述
孙悟空喜欢吃蟠桃，一天他乘守卫蟠桃园的天兵天将离开了而偷偷的来到王母娘娘的蟠桃园偷吃蟠桃。

已知蟠桃园有 N 棵蟠桃树，第 i 棵蟠桃树上有 N[i]（大于 0）个蟠桃，天兵天将将在 H（不小于蟠桃树棵数）小时后回来。

孙悟空可以决定他吃蟠桃的速度 K（单位：个/小时），每个小时他会选则一颗蟠桃树，从中吃掉 K 个蟠桃，如果这棵树上的蟠桃数小于 K，他将吃掉这棵树上所有蟠桃，然后这一小时内不再吃其余蟠桃树上的蟠桃。孙悟空喜欢慢慢吃，但仍想在天兵天将回来前将所有蟠桃吃完。

求孙悟空可以在 H 小时内吃掉所有蟠桃的最小速度 K（K 为整数）。

输入描述：
从标准输入中读取一行数字，前面数字表示每棵数上蟠桃个数，最后的数字表示天兵天将将离开的时间。

输出描述：
吃掉所有蟠桃的最小速度 K（K 为整数）或输入异常时输出-1。

示例 1
输入：
3 11 6 7 8
输出：
4

参考代码
树的个数大于h时，已最大速度吃也吃不完，输出-1
最大速度是 一个树上的最大个数，可以从1 - max 分别计算，满足 sum(nums[i]/k) <= H，就是最小速度

```python
while 1:
    try:
        *nums, h = list(map(int, input().split()))

        if len(nums) > h:
            print(-1)
        else:
            for i in range(1, max(nums)+1):
                if sum([round(n/i+0.5) for n in nums]) <= h:
                    print(i)
                    break
            else:
                print(-1)

    except Exception as e:
        break
```

# 书籍叠放问题

题目描述

假设书本的叠放有这样的规则，当A书的长度和宽度都大于B书时，可以将其B书置于A的上方，堆叠摆放，请设计一个程序，根据输入的书本长宽，计算最多可以堆叠摆放多少本书？

示例 1

输入：
3
16 15
13 12
15 14

输出：
3

> 说明
> 这里代表有3本书，第1本长宽分别为16和15，第2本长宽为13和12，第3本长宽为15和14，它们可以按照 [13, 12],[15, 14],[16,15] 的顺序堆叠，所以输出3

参考代码

```python
while 1:
    try:
        n = int(input())

        nums = [list(map(int, input().split())) for _ in range(n)]

        dp = []
        lens = len(nums)

        def dfs(sub, count=1):
            if count >= lens:
                dp.append(len(sub))
            else:
                for i in range(lens):
                    if i not in sub:
                        if nums[i][0] < nums[sub[-1]][0] and nums[i][1] < nums[sub[-1]][1]:
                            dfs(sub+[i], count+1)

        for i in range(lens):
            dfs([i])

        print(max(dp))
    except Exception as e:
        break

```

# VLAN资源池

题目描述
Vlan是一种为局域网设备进行逻辑划分的技术，为了标识不同的vlan 引入了vlan id 1~4094之间的整数

定义一个vlan id 的资源池，资源池中连续的vlan用开始vlan-结束vlan表示，不连续的用单个整数表示，所有的vlan用英文逗号连接起来

现有一个vlan资源池，业务需要从资源池中申请一个vlan，需要你输出从vlan资源池中移除申请的vlan后的资源池

示例 1

输入：
5,1-3
10

输出：
1-3,5

> 资源池中有1 2 3 5
> 申请的资源不在资源池中
> 将原池升序输出为1-3,5

参考代码

```python
while 1:
    try:
        nums = input().split(",")
        vid = int(input())

        vlans = []
        # 还原 vid 
        # 比如从 1-4 还原为 1,2,3,4
        for p in nums:
            val = p.split("-")
            if len(val) == 1:
                s = e = val[0]
            else:
                s, e = val
            vlans += list(range(int(s), int(e)+1))

		# 移除申请的vlan
        if vid in vlans:
            vlans.remove(vid)

        vlans = sorted(vlans)


		# 重新生成 缩略格式
		# 1,2,3,4 转 1-4
        ret = []
        t = [vlans[0]]
        for vid in vlans[1:]:
            if t[-1] + 1 == vid:
                if len(t) < 2:
                    t.append(vid)
                else:
                    t[-1] = vid
            else:
                ret.append("-".join(map(str, t)))
                t = [vid]
        if t:
            ret.append("-".join(map(str, t)))

        print(",".join(ret))
    except Exception as e:
        break

```

# 最长的指定瑕疵度的元音子串

题目描述
开头和结尾都是元音字母（aeiouAEIOU）的字符串为元音字符串，其中混杂的非元音字母数量为其瑕疵度。

比如:
“a” 、 “aa”是元音字符串，其瑕疵度都为0
“aiur”不是元音字符串（结尾不是元音字符）
“abira”是元音字符串，其瑕疵度为2

给定一个字符串，请找出指定瑕疵度的最长元音字符子串，并输出其长度，如果找不到满足条件的元音字符子串，输出0。

输入描述：
首行输入是一个整数，表示预期的瑕疵度flaw，取值范围[0, 65535]。
接下来一行是一个仅由字符a-z和A-Z组成的字符串，字符串长度(0, 65535]。

输出描述：
输出为一个整数，代表满足条件的元音字符子串的长度。

示例 1
输入：
0
asdbuiodevauufgh

输出：
3

参考代码

```python
bct = "aeiou"

while 1:
    try:
        k = int(input())
        nums = input()

        n = len(nums)
        nums = [1 if c in bct else 0 for c in nums]
        print(nums)
        max_ = 0
        i = 0
        j = min(k, n)
        while j < len(nums):
            count = nums[i:j].count(0)
            if count < k:
                j += 1
            elif count == k:
                max_ = max([j-i, max_])
                j += 1
            else:
                i += 1
        else:
            max_ = max([j - i, max_])

        print(max_)
    except Exception as e:
        break
```

# 二叉树按照中序遍历输出

题目描述
根据给定的二叉树结构描述字符串，输出该二叉树按照中序遍历结果字符串。中序遍历顺序为:左子树，根结点，右子树。

输入描述：
由大小写字母、左右大括号、逗号组成的字符串:

字母代表一个节点值，左右括号内包含该节点的子节点。
左右子节点使用逗号分隔，逗号前为空则表示左子节点为空,没有逗号则表示右子节点
为空。
二叉树节点数最大不超过100。
注:输入字符串格式是正确的，无需考虑格式错误的情况。
输出描述：
输出一个字符串，为二叉树中序遍历各节点值的拼接结果。

示例 1

输入：a{b{d, e{g,h{,I}}},c{f}}
输出：dbgehiafc
dbgehiafc

参考代码

先构造树，再中序输出，输入：a{b{d, e{g,h{,I}}},c{f}}

```python
class Node:
	# 树节点
	# 对树结构体不了解的
	# 可看 二叉树相关算法：https://pycoder.blog.csdn.net/article/details/123502655
	# 实在不行直接跳过该题
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None


def get_node(c):
    # 判断是否是一个树节点
    # 不是树节点  创建节点返回
    if type(c) == Node:
        return c
    elif c:
        return Node(c)
    else:
        return None


def get_root(stack):
	# 构建树
	# 从最内层 开始 构建子树，如h{,I}
	# 将构建的子树 再放回 stack
	# 重新构建下一级
	# 类似 计算括号深度
    j = len(stack) - 1
    while j >= 0:
        if stack[j] == "{":
            break
        j -= 1

    if stack[j:][1] == ",":
        left_ = None
    else:
        left_ = get_node(stack[j:][1])

    if stack[j:][-1] == "," or "," not in stack[j:]:
        right_ = None
    else:
        right_ = get_node(stack[j:][-1])

    stack = stack[:j]
    root_ = get_node(stack.pop(-1))
    root_.left = left_
    root_.right = right_
    stack.append(root_)
    return stack


while 1:
    try:
        chars = input().replace(" ", "")

        # 构建树
        stack = []
        for c in chars:
            if c == "}":
                stack = get_root(stack)
            else:
                stack.append(c)

        dp = []

        # 中序输出
        # 树的前中后序、广度、深度
        # 可看 二叉树相关算法：https://pycoder.blog.csdn.net/article/details/123502655
        def dfs(root):
            if not root:
                return

            dfs(root.left)
            dp.append(root.val)
            dfs(root.right)

        dfs(stack[0])
        print("".join(dp))
    except Exception as e:
        break

```

# 补种未成活胡杨

题目描述

近些年来，我国防沙治沙取得显著成果。某沙漠新种植N棵胡杨（编号1-N），排成一排。

一个月后，有M棵胡杨未能成活。

现可补种胡杨K棵，请问如何补种（只能补种，不能新种），可以得到最多的连续胡杨树？

输入描述：
N 总种植数量
M 未成活胡杨数量
M 个空格分隔的数，按编号从小到大排列
K 最多可以补种的数量

其中：
1 <= N <= 100000
1 <= M <= N
0 <= K <= M

输出描述：
最多的连续胡杨棵树

示例 1
输入：
5
2
2 4
1

输出：
3

> 
> 说明：
> 补种到2或4结果一样，最多的连续胡杨棵树都是3。

参考代码

特殊的求最大连续子串，只是这个子串需要k取修正

首先生成树苗状态的二位数组，0表示未成活，1表示成活
使用滑动窗口求最大连续为1的子串，容许子串中0的个数小于k，应该可以把k个0补种为1

```python
while 1:
    try:
        n = int(input())
        m = int(input())
        nums = list(map(int, input().split()))
        k = int(input())

		# 首先生成树苗状态的二位数组
        nums = [0 if i + 1 in nums else 1 for i in range(n)]

        max_ = 0
        # 滑动窗口边界
        i = 0
        j = min(k, n)
        while j < len(nums):
            # 求子串中0的个数
            count = nums[i:j].count(0)
            if count < k:
                j += 1
            elif count == k:
                max_ = max([j-i, max_])
                j += 1
            else:
                i += 1
        else:
            max_ = max([j - i, max_])

        print(max_)
    except Exception as e:
        break

```

# 最少交换次数来组合小于k的整数(固定的滑动窗口)

题目描述

给出数字k,请输出所有结果小于k的整[数组](https://so.csdn.net/so/search?q=数组&spm=1001.2101.3001.7020)合到一起的最小交换次数。 组合一起是指满足条件的数字相邻，不要求相邻后在数组中的位置。

数据范围 -100 <= k <= 100 -100 <= 数组中的值 <= 100

示例 1

输入：
1 3 1 4 0
2
输出： 1

示例 2

输入：
0 0 1 0
2
输出： 0

示例 3

输入：
2 3 2
1
输出： 0

参考代码

将大于等于k的置为0，小于k的置为1；
1 3 1 4 0 转换后 1 0 1 0 1
一共有3个小于k的数，确定宽度为3的窗口向右滑动，窗口中1的数量越多移动的次数越少。

```python
while 1:
    try:
        nums = list(map(int, input().split()))
        k = int(input())
		n = len(nums)
        nums = [0 if num[i]>k else 1 for i in range(n)]
#        for i in range(len(nums)):
#            if nums[i] < k:
#                nums[i] = 1
#            else:
#                nums[i] = 0

        m = sum(nums)

        dp = []
        for i in range(n):
            dp.append(sum(nums[i: i+m]))

        print(m-max(dp))
    except Exception as e:
        break
```

数大雁

题目描述
题目详解见 【华为机试真题 Python实现】数大雁 题目详解
一群大雁往南飞，给定一个字符串记录地面上的游客听到的大雁叫声，请给出叫声最少由几只大雁发出。

具体的:

大雁发出的完整叫声为”quack“，因为有多只大雁同一时间嘎嘎作响，所以字符串中可能会混合多个”quack”。
大雁会依次完整发出”quack”，即字符串中’q’ ,‘u’, ‘a’, ‘c’, ‘k’ 这5个字母按顺序完整存在才能计数为一只大雁。如果不完整或者没有按顺序则不予计数。
如果字符串不是由’q’, ‘u’, ‘a’, ‘c’, ‘k’ 字符组合而成，或者没有找到一只大雁，请返回-1。

输入描述：
一个字符串，包含大雁quack的叫声。1 <= 字符串长度 <= 1000，字符串中的字符只有’q’, ‘u’, ‘a’, ‘c’, ‘k’。

输出描述：
大雁的数量

示例 1
输入：
quackquack

输出：
1

示例 2
输入：
qaauucqckk

输出：
-1

示例 3
输入：
quacqkuac

输出：
1

示例 4
输入：
qququaauqccauqkkcauqqkcauuqkcaaukccakkck

输出：
5

参考代码

```python
def minNumberOfFrogs(croakOfFrogs: str) -> int:
    q,u,a,c,k,m=0,0,0,0,0,1
    for i in croakOfFrogs:
        if i=='q':q+=1
        elif i=='u':u+=1
        elif i=='a':a+=1
        elif i=='c':c+=1
        elif i=='k':k+=1
        if q>=u>=a>=c>=k:
            if k==1:
                m=max(m,q)
                q-=1
                u-=1
                a-=1
                c-=1
                k-=1
        else:return -1
    if k or c or a or u or q:return -1
    return m
```

```python
while 1:
    try:
        chars = input()
        base = "quack"
        states = [0] * len(base)
        dp = []
        for i in range(len(chars)):
            index = base.index(chars[i])
            if index == 0:
                states[index] += 1
            else:
                if states[index - 1]:
                    states[index - 1] -= 1
                    states[index] += 1

                if base[-1] == chars[i]:
                    if states[-1] != 0:
                        # 计算剩余字符串是否满足剩余的叫声
                        temp = [t for t in states]
                        temp[-1] = 0
                        max_ = sum(temp)
                        for j in range(i, len(chars)):
                            index = base.index(chars[j])
                            if temp[index - 1]:
                                temp[index - 1] -= 1
                                temp[index] += 1
                            if temp[-1] == max_:
                                break
                        dp.append(temp[-1] + 1)
                        states[-1] -= 1

        print(max(dp) if dp else -1)
    except Exception as e:
        break

```

# 奖牌榜的排名

题目描述
奖牌榜的排名算法要求如下，首先是按照金牌🏅总数排序的，如果是同等金牌的情况下，再根据银牌🥈的总数进行排序，同样在金牌跟银牌数量相等的情况下，在根据铜牌🥉的数量进行排名。在遇到金牌🏅、银牌🥈跟铜牌🥉数量一样的情况下，根据国家的名称首字母序排序。

输入描述：
第一行输入要排序的国家的个数n，之后n行分别输入国家的名字、金牌总数、银牌总数和铜牌总数，以一个空格隔开。

输出描述：
按照降序方式输出国家的名字。

示例1

输入：

```python
8
China 9 4 2
Germany 12 10 5
Norway 16 8 13
Britain 1 1 0
Belgium 1 0 1
Italy 2 7 8
Netherlands 8 5 4
CzechRepublic 1 0 1
```

输出：

```python
Norway 16 8 13
Germany 12 10 5
China 9 4 2
Netherlands 8 5 4
Italy 2 7 8
Britain 1 1 0
Belgium 1 0 1
CzechRepublic 1 0 1
```

分析

就2022年北京冬奥会一共会产生327枚奖牌。其中金牌银牌铜牌各109枚。
使用 sorted 按排序规则生成排序值并格式化输出。

参考代码

```python
while 1:
    try:
        count = int(input())
        
        temp = {}
        for _ in range(count):
            rst = input().split()
            temp[rst[0]] = (int(rst[1]), int(rst[2]), int(rst[3]), (100 - ord(rst[0][0].upper())))

        dex_lst = sorted(temp.items(), key=lambda itm: itm[1], reverse=True)

        for name in dex_lst:
            print("%s %d %d %d" % (name[0], *name[1][:3]))
    except:
        break

```

# 整型数组按个位值排序

题目描述

给定一个非空整型[数组](https://so.csdn.net/so/search?q=数组&spm=1001.2101.3001.7020)，其元素数据类型为整型，请按照数组元素十进制最低位从小到大进行排序，十进制最低位相同的元素，相对位置保持不变。
当[数组元素](https://so.csdn.net/so/search?q=数组元素&spm=1001.2101.3001.7020)为负值时，十进制最低位等同于去除符号位后对应十进制值最低位。

输入描述:

给定一个非空数组，其元素数据类型为32位有符号整数，数组长度[1, 1000]

输出描述:

输出排序后的数组

示例1：

输入

```python
1,2,5,-21,22,11,55,-101,42,8,7,32
```

输出

```python
1,-21,11,-101,2,22,42,32,5,55,7,8
```

示例2：

输入

```python
19,-31,10,57,61,27,11,28,-94
```

输出

```python
10,-31,61,11,-94,57,27,28,19
```

参考代码

排序首先想到的就是 `sorted` 方法，使用 `lambada` 定义排序规则即可。

```python
while 1:
    try:
        nums = input().split(",")
        nums = sorted(nums, key=lambda x: str(x)[-1])
        print(",".join(nums))
    except Exception:
        break

```

# 磁盘容量排序

题目描述

磁盘的容量单位常用的有有 M， G， T 这三个等级，它们之间的换算关系是 1T=1000G，1G=1000M，现在给定 n 块磁盘的容量，请对它们按从小到大的顺序进行排序

例如给定 3 块盘的容量， 1T， 20M， 3G，排序后的结果未 20M， 3G， 1T

输入描述：
1、每个测试用例第一行包含一个整数 n(2<=n<=1000)，表示磁盘的个数，接下的 n 行，每行一个字符串（长度大于 2，小于 10），表示磁盘的容量，格式为： mv，其中 m 表示
容量大小， v 表示容量单位，例如 20M， 1T， 30G

2、磁盘容量 m 表示十进制数范围为 1 到 1000 的正整数，容量单位 v 的范围只包含题目中提到的 M， G， T 三种，换算关系如题目描述

输出描述：
输出 n 行，表示 n 块磁盘容量排序后的结果

示例 1

输入：

```
3
1T
20M
3G
```

输出：

```python
20M
3G
1T
```

示例 2

输入：

```python
3 
2G4M
3M2G
1T
```

输出：

```python
3M2G
2G4M
1T
```

参考代码

```python
while 1:
    try:
        dic = {}
        n = int(input())
        ls = [input() for i in range(n)]
        for word in ls:
            temp = word
            j = 0
            while j < len(temp):
                if temp[j] == "G":
                    if word in dic:
                        dic[word] += int(temp[:j]) * 1024
                    else:
                        dic[word] = int(temp[:j]) * 1024
                    temp = temp[j+1:]
                elif temp[j] == "T":
                    if word in dic:
                        dic[word] += int(temp[:j]) * 1024*1024
                    else:
                        dic[word] = int(temp[:j]) * 1024*1024
                    temp = temp[j+1:]
                elif temp[j] == "M":
                    if word in dic:
                        # print(dic[word],temp)
                        dic[word] += int(temp[:j])
                    else:
                        dic[word] = int(temp[:j])
                    temp = temp[j+1:]
                else:
                    j += 1
        out = sorted(dic.items(),key=lambda x:x[1])
        for o in out:
            print(o[0])
    except:
        break
```

# 最少面试官数

题目描述
公司正在组织集中面试，每场面试需要一个面试官，现在以数组的形式给出了每场面试的开始时间和结束时间的时间安排表 [si, ei] (si < ei)），为避免面试冲突，请问至少需要多少位面试官，才能满足这些面试安排。

示例 1
输入:
3
0 30
5 10
15 20
输出: 2

示例 2
输入:
2
7 10
2 4
输出: 1

参考代码
我们模拟有一条时间线，当有面试时 时间线上的范围 +1；
时间线上最大数值就是需要的面试官数；

```python
while 1:
    try:
        n = int(input())

        nums = []
        for _ in range(n):
            i = input().split()
            max_=max(max_,i[1])
            nums.append(list(map(int, i)))
        #nums = sorted(nums, key=lambda x: x[0])
        #max_ = max([e for s, e in nums])
        dp = [0] * (max_+1)
        for s, e in nums:
            for i in range(s, e+1):
                dp[i] += 1

        print(max(dp))
    except Exception as e:
        break
```

# 导师请吃火锅

题目描述
入职后，导师会请你吃饭，你选择了火锅。

火锅里会在不同时间下很多菜。

不同食材要煮不同的时间，才能变得刚好合适。你希望吃到最多的刚好合适的菜，但你的手速不够快，用m代表手速，每次下手捞菜后至少要过ms才能在捞（每次只能捞一个）。

那么用最合理的策略，最多能吃到多少刚好合适的菜？

输入描述：
第一行两个整数n，m，其中n代表往锅里下的菜的个数，m代表手速。
接下来有n行，每行有两个数x，y代表第x秒下的菜过y秒才能变得刚好合适。
（1 < n, m < 1000）
（1 < x, y < 1000）

输出描述：
输出一个整数代表用最合理的策略，最多能吃到刚好合适的菜的数量

示例 1
输入：
2 1
1 2
2 1

输出：
1

参考代码

菜熟的时间可以转换成时间轴 0 0 2 ，因为一个时刻只能捞一个 可以表示为 0 0 1，一个时刻也可以简化为 捞或者不捞 。

```python
while 1:
    try:
        n, m = list(map(int, input().split()))

        idx = []
        for _ in range(n):
            i, v = list(map(int, input().split()))
            idx.append(i+v)

        nums = [0] * (max(idx)+1)
        for i in idx:
            nums[i] = 1

        dp = []

        def dfs(t, data):
            # t 可以夹菜的时间, 因为每秒夹一次，也可作为时间轴
            if t >= len(nums):
                dp.append(sum(data))
                return
            # 等于 1 此时有菜，但是可以选择吃或者不吃，可能会影响后面夹菜的顺序
            if nums[t] == 1:
                # 1 表示吃，吃了要直接跳到m秒后
                # 没吃 过1s继续选择 吃或者不吃
                dfs(t+m, data + [1])
                dfs(t+1, data)
            else:
                dfs(t+1, data)

        dfs(1, [])

        print(max(dp))
    except Exception as e:
        break

```

# 跳格子游戏

题目描述
地上共有N个格子，你需要跳完地上所有的格子，但是格子间是有强依赖关系的，跳完前一个格子后，后续的格子才会被开启，格子间的依赖关系由多组steps数组给出，steps[0]表示前一个格子,steps[1]表示steps[0]可以开启的格子:

比如[0,1]表示从跳完第0个格子以后第1个格子就开启了，
比如[2,1]，[2,3]表示跳完第2个格子后第1个格子和第3个格子就被开启了

请你计算是否能由给出的steps数组跳完所有的格子,如果可以输出yes，否则输出no

说明：

你可以从一个格子跳到任意一个开启的格子
没有前置依赖条件的格子默认就是开启的
如果总数是N，则所有的格子编号为[0,1,2,3…N-1]连续的数组
输入描述:
输入一个整数N表示总共有多少个格子，接着输入多组二维数组steps表示所有格子之间的依赖关系

输出描述:
如果能按照steps给定的依赖顺序跳完所有的格子输出yes
否则输出no

示例 1

输入：
3
0 1
0 2
输出：
yes

> 说明
> 总共有三个格子[0,1,2]，跳完0个格子后第1个格子就开启了，跳到第0个格子后第2个格子也被开启了，按照0->1->2或者0->2->1的顺序都可以跳完所有的格子

示例 2

输入：
2
1 0
0 1
输出：
no

> 说明
> 总共有2个格子，第1个格子可以开启第0格子，但是第1个格子又需要第0个格子才能开启，相互依赖，因此无法完成

示例 3

输入：
6
0 1
0 2
0 3
0 4
0 5
输出：
yes

> 说明
> 总共有6个格子，第0个格子可以开启第1,2,3,4,5个格子，所以跳完第0个格子之后其他格子都被开启了，之后按任何顺序可以跳完剩余的格子

示例 4

输入：
5
4 3
0 4
2 1
3 2
输出：
yes

> 说明
> 跳完第0个格子可以开启格子4，跳完格子4可以开启格子3，跳完格子3可以开启格子2，跳完格子2可以开启格子1，按照0->4->3->2->1这样就跳完所有的格子



```python
while 1:
    try:
        n = int(input())

        nums = []
        for _ in range(n):
            x = input().split()
            if x and len(x) == 2:
                nums.append(tuple(map(int, x)))
            else:
                break

        nums = sorted(nums, key=lambda x: x[0])

        # 格子开启状态表
        state = [1] * n
        for start, end in nums:
            state[end] = 0

        # 没有起始的格子
        if max(state) == 0:
            print("no")
            continue

        dp = [0]

        def dfs(s, link):
        	# 所有节点都已开启
            if len(link) == len(nums) and sorted(link, key=lambda x: x[0]) == nums:
                dp.append(1)
                return

            ends = []
            for start_, end_ in nums:
                if s == start_:
                    link.append((start_, end_))
                    ends.append(end_)

            for end_ in ends:
                dfs(end_, link)

		# 从状态开启的节点，进行递归
        for i in range(n):
            if state[i] == 1:
                dfs(i, [])

        if max(dp):
            print("yes")
        else:
            print("no")

    except Exception as e:
        break

```

# 搜索矩阵

题目描述
实现一个程序 search_matrix(matrix),参数 matrix 是一个仅包含 0 或 1 两种数字的矩阵,程序应返回输入矩阵中包含的最大正方形子矩阵(长和宽相等)的区域面积。

例如:如果 matrix 是[“1010111111”,“000000011”,“1010110111”,“0000110001”,那么它看起来像下面的阵:

1 0 1 0 1 1 1 1 1 1
0 0 0 0 0 0 0 1 1 1
1 0 1 0 1 1 0 1 1 1
0 0 0 0 1 1 0 0 0 1

对于上面的输入,最大的子[矩阵](https://so.csdn.net/so/search?q=矩阵&spm=1001.2101.3001.7020)是一个 3x3 的矩阵,程序只要返回最大子矩阵的面积即可,如上面的矩阵即返回 9(3x3)。

第 1 行输入为一个数字 N,代表下面有几行
第 2 行到第 N+1 行是代表矩阵的 0 和 1 组成的字符串,每行的长度相同

返回一个数字,代表输入矩阵的最大正方子矩阵的面积。

示例 1

输入：

```python
3 
1 1 0
1 1 1
1 1 0
```

输出：

```python
6
```

参考代码

```python
def dfs(heights):
    stack = [(-1, -1)]
    H = heights + [0]
    ans = 0
    for i, h in enumerate(H):
        while stack[-1][1] > h:
            _, oh = stack.pop()
            s = (i - stack[-1][0] - 1) * oh
            ans = max(ans, s)
        stack.append((i, h))
    return ans


while 1:
    try:
        matrix = [input().split() for _ in range(int(input()))]
        if not matrix:
            print(0)
            break

        m = len(matrix)
        n = len(matrix[0])
        dp = [0] * n
        ans = 0
        for i in range(m):
            for j in range(n):
                dp[j] = 0 if matrix[i][j] == '0' else dp[j] + 1
            ans = max(ans, dfs(dp))

        print(ans)
    except Exception as e:
        break
```

# 计算三个矩形相交的面积

题目描述
给出3组点坐标(x, y, w, h)，-1000<x,y<1000，w,h为正整数。
(x, y, w, h)表示平面直角坐标系中的一个矩形：
x, y为矩形左上角坐标点，w, h向右w，向下h。
(x, y, w, h)表示x轴(x, x+w)和y轴(y, y-h)围成的矩形区域；
(0, 0, 2, 2)表示 x轴(0, 2)和y 轴(0, -2)围成的矩形区域；
(3, 5, 4, 6)表示x轴(3, 7)和y轴(5, -1)围成的矩形区域；
求3组坐标构成的矩形区域重合部分的面积。

输入描述：
3行输入分别为3个矩形的位置，分别代表“左上角x坐标”，“左上角y坐标”，“矩形宽”，“矩形高” -1000 <= x,y < 1000

输出描述：
输出3个矩形相交的面积，不相交的输出0。

示例 1
输入：
1 6 4 4
3 5 3 4
0 3 7 3

输出：
2

说明:
给定3个矩形A，B，C
A:左上角坐标(1,6)，宽4、高4B:左上角坐标(3,5)，宽3，高4c:左上角坐标(0,3)，宽7，高33个矩形的相交面积为2，如图所示:

参考代码

结题思路与[【华为机试真题 Python实现】计算矩形面积【2022 Q2 |200分】](https://pycoder.blog.csdn.net/article/details/124903531)一致

1. 根据输入确定矩形的左上角坐标和右下角坐标
2. 根据最值生成画布（二位数组）
3. 在二维数据上绘制矩形区域
4. 三个矩形相交区域数值为3

```python
"""
最终dp的值
dp = [[1, 1, 1, 0, 0, 0],
      [1, 1, 2, 1, 1, 1],
      [1, 1, 2, 1, 1, 1],
      [1, 2, 3, 2, 2, 1],
      [1, 2, 3, 2, 2, 1],
      [1, 2, 2, 1, 1, 0],
      [1, 1, 1, 0, 0, 0]
      ]

"""
while 1:
    try:
        xs = []
        ys = []

        nums = []
        for _ in range(3):
            x1, y1, w, h = list(map(int, input().split()))
            # x1, y1 左上角
            # x2, y2 右上角
            x2 = x1 + w
            y2 = y1 - h
            xs += [x1, x2]
            ys += [y1, y2]
            # 记录矩形坐标
            nums.append((x1, y1, x2, y2))

        min_x, max_x = min(xs), max(xs)
        min_y, max_y = min(ys), max(ys)

        # 坐标轴偏移量
        x = 0 - min_x
        y = 0 - min_y
        # 构建二维数据
        dp = [[0] * abs(max_y-min_y) for _ in range(abs(max_x-min_x))]

        for x1, y1, x2, y2 in nums:
            # 需要 转成 二维数组的下标
            for i in range(min((x2, x1)) + x, max((x2, x1)) + x):
                for j in range(min((y2, y1)) + y, max((y2, y1)) + y):
                    # 绘制矩形，最终的数值表示被矩形覆盖的次数
                    dp[i][j] += 1

        ret = 0
        for i in range(len(dp)):
            for j in range(len(dp[0])):
                # 数值为3，是三个矩形的交集
                if dp[i][j] == 3:
                    ret += 1
        print(ret)
    except Exception as e:
        break
```

# 金字塔

示例 1

题目描述

微商模式比较典型，下级每赚 100 元就要上交 15 元，给出每个级别的收入，求出金字塔尖
上的人收入。

输入：
1 0 223
2 0 323
3 2 1203
输出：
0（金字塔顶代理商）
105 （最终的钱数）

> 解释：
> 1 0 223
> 代理商代号：1
> 上级代理商代号 0
> 代理商转的钱 223
> 2 的最终收入等于 323 + 1203//100 *15 == 323 +180
> 0 的最终收入等于 (323 +180 + 223)//100 *15 == 105

参考代码

申请一个长度n+1的数组，保存下级对上级上交的收入，存储在上级对应的下标中。

```python
while 1:
    try:
        n = int(input())

        nums = [list(map(int, input().split())) for _ in range(n)]

        nums = sorted(nums, key=lambda x: x[1], reverse=True)
        print(nums)
        dp = [0] * (n + 1)
        start = 0
        for i, j, m in nums:
            print(i,j,m)
            start = min(j, start)
            dp[j] += ((m + dp[i])//100) * 15

        print(f"{start} {dp[start]}")
    except Exception as e:
        break
```

# 最大时间

# 题目描述

给定一个数组,里面有 6 个整数,求这个数组能够表示的最大 24 进制的时间是多少,输出这个时间,无法表示输出 invalid。

输入描述：
输入为一个整数数组,数组内有六个整数。输入整数数组长度为 6,不需要考虑其它长度,元素值为 0 或者正整数,6 个数字每个数字只能使用一次.

输出描述：
输出为一个 24 进制格式的时间,或者字符串"ivalid"
备注:输出时间格式为 xxxxx 式

示例 1
输入：0,2,3,0,5,6
输出：23:56:00

# 参考代码

```python
while 1:
    try:
        nums = input().split(',')

        def dfs(ids, max_, dp):
            sub = "".join([nums[i] for i in ids])
            if 0 <= int(sub) < max_ and len(sub) == 2:
                dp.append((sub, ids))
            else:
                for i in range(len(nums)):
                    if i not in ids:
                        dfs(ids+[i], max_, dp)

        def fun(max_):
            dp = []
            for i in range(len(nums)):
                dfs([i], max_, dp)
            dp = sorted(dp, key=lambda x: int(x[0]), reverse=True)
            if dp:
                for c in str(dp[0][0]):
                    nums.remove(c)
                return str(dp[0][0])

        # 取最大小时数
        h = fun(24)
        # 取最大分钟数
        m = fun(60)
        # 取最大秒数
        s = fun(60)

        if h and m and s:
            print(f"{h}:{m}:{s}")
        else:
            print("ivalid")

    except Exception as e:
        break

```

# 仿 LISP 运算

题目描述
LISP 语言唯一的语法就是括号要配对。 形如 (OP P1 P2 …)，括号内元素由单个空格分割。

其中第一个元素 OP 为操作符，后续元素均为其参数，参数个数取决于操作符类型

注意：参数 P1, P2 也有可能是另外一个嵌套的 (OP P1 P2 …) 当前 OP 类型为 add / sub / mul / div（全小写），分别代表整数的加减乘除法 简单起见，所有 OP 参数个数均为 2

举例:
输入： (mul 3 -7)
输出： -21

输入： (add 1 2)
输出： 3

输入： (sub (mul 2 4) (div 9 3))
输出： 5

输入： (div 1 0)
输出： error

题目涉及数字均为整数，可能为负；不考虑 32 位溢出翻转，计算过程中也不会发生 32 位溢出翻转 除零错误时，输出 “error”，除法遇除不尽，向下取整，即 3/2 = 1

输入描述：
输入为长度不超过 512 的字符串，用例保证了无语法错误

输出描述：
输出计算结果或者“error”

示例 1
输入： (div 12 (sub 45 45))

输出：error

示例 1
输入：
输出：

参考代码

```python
while 1:
    try:
        nums = input().split()
        stack_a = []
        stack_b = []

        for c in nums:
            if "(" in c:
                stack_a.append("(")
                stack_a.append(c[1:])
            elif ")" in c:
                stack_a.append(c[:c.index(")")])
                stack_a += list(c[c.index(")"):])
            else:
                stack_a.append(c)

        while stack_a:
            temp = stack_a.pop()
            if temp == ")":
                stack_b.append(temp)
            elif temp == "(":
                com = stack_b.pop()
                # add / sub / mul / div
                a = int(stack_b.pop())
                b = int(stack_b.pop())
                # 当前计算公式的右括号
                stack_b.pop()
                if com == "add":
                    stack_b.append(a+b)
                elif com == "sub":
                    stack_b.append(a-b)
                elif com == "mul":
                    stack_b.append(a*b)
                else:
                    if b == 0:
                        print("error")
                        break
                    stack_b.append(a//b)
            else:
                stack_b.append(temp)

        if stack_b:
            print(stack_b.pop())
    except Exception as e:
        break
```

# 平安果

题目描述
给定一个 M 行 N 列的矩阵（M*N 个格子），每个格子中放着一定数量的平安果。
你从左上角的各自开始，只能向下或者向右走，目的地是右下角的格子。
每走过一个格子，就把格子上的平安果都收集起来。求你最多能收集到多少平安果。

注意：当经过一个格子时，需要一次性把格子里的平安果都拿走。

限制条件： 1<N,M<=50；每个格子里的平安果数量是 0 到 1000（包含 0 和 1000） .

输入描述：
输入包含两部分：
第一行 M, N
接下来 M 行，包含 N 个平安果数量

输出描述：
一个整数 ， 最多拿走的平安果的数量

示例 1
输入：
2 4
1 2 3 40
6 7 8 90

输出：
136

```
1 3 6 46
7 14 22 136
```

参考代码
递归解法 可参考 【华为机试真题 Python实现】机器人走迷宫

```python
while 1:
    try:
        m, n = list(map(int, input().split()))

        nums = [list(map(int, input().split())) for _ in range(m)]

        dp = [[0]*n for _ in range(m)]

        for i in range(m):
            for j in range(n):
                if i == 0 and j == 0:
                    dp[i][j] = nums[i][j]
                elif i == 0:
                    dp[i][j] = dp[i][j-1] + nums[i][j]
                elif j == 0:
                    dp[i][j] = dp[i-1][j] + nums[i][j]
                else:
                    dp[i][j] = max((dp[i-1][j], dp[i][j-1])) + nums[i][j]
        print(dp[-1][-1])
    except Exception as e:
        break

```

# HJ65 查找两个字符串a,b中的最长公共子串

题目描述
查找两个字符串a,b中的最长公共子串。若有多个，输出在较短串中最先出现的那个。
注：子串的定义：将一个字符串删去前缀和后缀（也可以不删）形成的字符串。请和“子序列”的概念分开！

数据范围：字符串长度1≤length≤300
进阶：时间复杂度：O(n^3) ，空间复杂度：O(n)

输入描述：
输入两个字符串

输出描述：
返回重复出现的字符

示例1
输入：

```
abcdefghijklmnop
abcsafjklmnopqrstuvw
```

输出：

```python
jklmnop
```

参考代码

```python
while 1:
    try:
        str1 = input()
        str2 = input()

        start = 0
        end = 1
        
        # 若有多个，输出在较短串中最先出现的那个。
        if len(str1) < len(str2):
            str1, str2 = str2, str1
        
        # 查找两个字符串a,b中的最长公共子串
        sub = ""
        while end <= len(str2):
            if str2[start: end] in str1:
                if end-start > len(sub):
                    sub = str2[start: end]
                end += 1
            else:
                start = end
                end = start + 1
        print(sub)
    except:
        break
```

```c
#include <stdio.h>
#include <string.h>

#define MAX_STRLEN    300

// 函数原型
void findLongestCommonSubstring(char *str1, char *str2);

int main(void)
{
    char str1[MAX_STRLEN + 1];
    char str2[MAX_STRLEN + 1];
    
    // 输入两个字符串
    fgets(str1, sizeof(str1), stdin);
    fgets(str2, sizeof(str2), stdin);

    // 移除换行符
    str1[strcspn(str1, "\n")] = '\0';
    str2[strcspn(str2, "\n")] = '\0';

    // 调用函数查找最长公共子串
    findLongestCommonSubstring(str1, str2);

    return 0;  
}

// 查找最长公共子串的函数
void findLongestCommonSubstring(char *str1, char *str2) {
    int dp[MAX_STRLEN + 1][MAX_STRLEN + 1] = {0};
    int strLen1 = strlen(str1);
    int strLen2 = strlen(str2);
    int maxLen = 0;
    int index = 0;
    int i, j;

    // 交换字符串使得str1是较短的字符串
    if (strLen1 > strLen2) {
        char *temp = str1;
        str1 = str2;
        str2 = temp;
        int tempLen = strLen1;
        strLen1 = strLen2;
        strLen2 = tempLen;
    }

    // 使用动态规划查找最长公共子串
    for (i = 1; i <= strLen1; i++) {
        for (j = 1; j <= strLen2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLen) {
                    maxLen = dp[i][j];
                    index = i;
                }
            }
        }
    }

    // 输出最长公共子串
    for (i = 0; i < maxLen; i++) {
        printf("%c", str1[index - maxLen + i]);
    }
    printf("\n");
}

```



**HJ17** **坐标移动**

## 描述

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

数据范围：每组输入的字符串长度满足 1≤*n*≤10000 ，坐标保证满足 −2^31≤*x*,*y*≤2^31−1 ，且数字部分仅含正数

### 输入描述：

一行字符串

### 输出描述：

最终坐标，以逗号分隔

## 示例1

输入：

```
A10;S20;W10;D30;X;A1A;B10A11;;A10;
```

复制

输出：

```
10,-10
```

复制

## 示例2

输入：

```
ABC;AKL;DA1;
```

复制

输出：

```
0,0
```

```c
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义坐标结构体


// 根据方向和距离更新坐标
void updateCoordinate(int* x,int* y, char direction, int distance) {
    switch (direction) {
        case 'A':
            *x -= distance;
            break;
        case 'D':
            *x += distance;
            break;
        case 'W':
            *y += distance;
            break;
        case 'S':
            *y -= distance;
            break;
    }
}

int main() {
    char input[10001];
    fgets(input, sizeof(input), stdin); // 读取输入字符串
    int len = strlen(input);
    int x=0,y=0;// 初始坐标

    char* token = strtok(input, ";"); // 使用分号分割输入字符串
    while (token != NULL) {
        char direction;
        int distance;
        if (sscanf(token, "%c%d", &direction, &distance) == 2 && isdigit(token[strlen(token)-1])) { // 解析坐标
            updateCoordinate(&x,&y, direction, distance); // 更新坐标
        }
        token = strtok(NULL, ";");
    }

    printf("%d,%d\n", x, y); // 输出最终坐标
    return 0;
}
```



[**HJ18** **识别有效的IP地址和掩码并进行分类统计**](https://www.nowcoder.com/practice/de538edd6f7e4bc3a5689723a7435682?tpId=37&rp=1&ru=%2Fexam%2Foj%2Fta&qru=%2Fexam%2Foj%2Fta&sourceUrl=%2Fexam%2Foj%2Fta%3FtpId%3D37&difficulty=&judgeStatus=&tags=&title=&gioEnter=menu)

## 描述

请解析IP地址和对应的掩码，进行分类识别。要求按照A/B/C/D/E类地址归类，不合法的地址和掩码单独归类。

所有的IP地址划分为 A,B,C,D,E五类

A类地址从1.0.0.0到126.255.255.255;

B类地址从128.0.0.0到191.255.255.255;

C类地址从192.0.0.0到223.255.255.255;

D类地址从224.0.0.0到239.255.255.255；

E类地址从240.0.0.0到255.255.255.255



私网IP范围是：

从10.0.0.0到10.255.255.255

从172.16.0.0到172.31.255.255

从192.168.0.0到192.168.255.255

子网掩码为二进制下前面是连续的1，然后全是0。（例如：255.255.255.32就是一个非法的掩码）

（注意二进制下全是1或者全是0均为非法子网掩码）

注意：

1. 类似于【0.*.*.*】和【127.*.*.*】的IP地址不属于上述输入的任意一类，也不属于不合法ip地址，计数时请忽略
2. 私有IP地址和A,B,C,D,E类地址是不冲突的





### 输入描述：

多行字符串。每行一个IP地址和掩码，用~隔开。

请参考帖子https://www.nowcoder.com/discuss/276处理循环输入的问题。

### 输出描述：

统计A、B、C、D、E、错误IP地址或错误掩码、私有IP的个数，之间以空格隔开。

## 示例1

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

## 示例2

输入：

```
0.201.56.50~255.255.111.255
127.201.56.50~255.255.111.255
```

复制

输出：

```
0 0 0 0 0 0 0
```

复制

说明：

```
类似于【0.*.*.*】和【127.*.*.*】的IP地址不属于上述输入的任意一类，也不属于不合法ip地址，计数时请忽略         
```

```c
#include <stdio.h>

int main() {
    int ip[4], mk[4];       //IP地址、子网掩码
    int addrType[7] = {};   //地址类型
    int mask, typeId;       //掩码值、类型号
    int endAddr[] =         //类型尾地址
    {126, 191, 223, 239, 255};
    int validInput;

    while (validInput = scanf("%d.%d.%d.%d~%d.%d.%d.%d", //读取IP地址和子网掩码
                 ip, ip + 1, ip + 2, ip + 3,
                 mk, mk + 1, mk + 2, mk + 3) == 8) {
        // Check for incomplete IP address or subnet mask
        if (ip[0] < 0 || ip[1] < 0 || ip[2] < 0 || ip[3] < 0 ||
            mk[0] < 0 || mk[1] < 0 || mk[2] < 0 || mk[3] < 0 ||
            ip[0] > 255 || ip[1] > 255 || ip[2] > 255 || ip[3] > 255 ||
            mk[0] > 255 || mk[1] > 255 || mk[2] > 255 || mk[3] > 255) {
            addrType[5]++;  //错误IP或掩码+1
            continue;
        }

        if (ip[0] == 0 || ip[0] == 127) //若非指定IP类型
            continue;
        //合并子网掩码为掩码值
        mask = (mk[0] << 24) + (mk[1] << 16) + (mk[2] << 8) + mk[3];
        if (((~mask + 1) & ~mask) || !~mask) {  //若掩码非连续1后0
            addrType[5]++;  //错误IP或掩码+1
            continue;
        }
        if (ip[0] == 10 || (ip[0] == 192 && ip[1] == 168) ||
                (ip[0] == 172 && ip[1] > 15 && ip[1] < 32)) //若为私有地址
            addrType[6]++;  //私有IP类型+1
        for (typeId = 0; ip[0] > endAddr[typeId]; typeId++); //遍历查找匹配类型
        addrType[typeId]++; //对应IP类型+1
    }
    if (!validInput) {
        addrType[5]++;  //错误IP或掩码+1
    }
    for (typeId = 0; typeId < 7; typeId++)  //遍历地址类型
        printf("%d ", addrType[typeId]);    //打印IP类型数
}

```

