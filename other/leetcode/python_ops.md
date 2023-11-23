```python
print("({},{})".format(10,11))
```

```python
b = sorted(a, key=str.upper)
```

```python
str1=str1.replace(' ','\n')
```

```python
s1 = s1.replace('.', '\.').replace('?', '[0-9a-z]').replace('*','#')#不支持正则匹配
s1 = re.sub('#+','[0-9a-z]*',s1)
   if bool(re.fullmatch(s1, s2)):
```

```python
matrix_line = [set() for i in range(9)]
```

```python
if ord('a') <= ord(item) <= ord('z'):
```

```python
if i.isalpha():
```

```python
times_list = sorted(dict1.values(), reverse=True)
```

```python
dict1[c] = each_name.count(c)
```

```python
dict = {}
for i in range(len(nums)):
	if target - num[i] not in dict:
```

```python
arr2 += sorted(set(arr1)-set(arr2))
   arr1.sort(key=arr2.index)
```

```python
any(dfs(x, y, 0) for x in range(m) for y in range(n))
```

```python
while a.endswith("/"):
while b.startswith("/"):
```

```python
cache = sorted(cache, key=lambda x: (-len(x), x))
```

```python
求质数：
        c = 2
        while c <= num**0.5:
            if num % c == 0:
                print(c, end=' ')
                num = num // c
            else:
                c += 1
```

```python
a = {"ac":["1","2"]}
print(a.pop("ac"))#['1', '2']
```

```python
n, *nums = input().split()#1 2 3 4
print(n,nums)#1 ['2', '3', '4']
```

```python
2
1 2
a = [input() for _ in range(len(input()))]
print(a)
```

# 全排列

# 题目描述

给定一个没有重复数字的序列，返回其所有可能的[全排列](https://so.csdn.net/so/search?q=全排列&spm=1001.2101.3001.7020)。

输入描述：
输入:[1,2,3]

输出描述：
输出:[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]

示例 1
输入：

```
1,2,3
```

输出：

```
[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
```

# 参考代码

```python
while 1:
    try:
        nums = list(map(int, input().split(",")))
        dp = []

        def dfs(s):
            if len(s) == len(nums):
                dp.append(s)
            else:
                for c in nums:
                    if c not in s:
                        dfs(s + [c])


        for c in nums:
            dfs([c])

        print(dp)
    except Exception as e:
        break
```

# 数字分解

# 题目描述

给你一个数可以分解成几组的连续的自然数之和？

比如 8 可以分解成 8，即 1 组
比如 6 可以分解成 6,123，即 2 组。

输入描述：
输入一个整数

输出描述：
输出可以分解的组数

示例 1
输入：8
输出：1

示例 2
输入：6
输出：2

参考代码
从题目给出的示例来看，每个数默认有一组可能，就是数值本身；
n本身 一些题里数值本身应该是不算一种可能的；
n = 0 + n, 除了 n=1 其他数都不是连续的。

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

