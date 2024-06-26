# [两数之和](https://leetcode.cn/problems/two-sum/):(hashmap)

输入：nums = [2,7,11,15], target = 26
输出：[2,3]
解释：因为 nums[2] + nums[3] == 26 ，返回 [2, 3] 。

|  2   |  7   |  11  |  15  |
| :--: | :--: | :--: | :--: |
|  i   |      |  j   |      |

```python
def twoSum(nums,target):
	dict = {}
	for i in range(len(nums)):
		if target - num[i] not in dict:
			dict[nums[i]] = i
		else:
			return [dict[target-num[i]],i]
```

```c
int* twoSum(int* nums, int numsSize, int target，int* returnSize) {
    int i,j;
    int *result=NULL;
    for(i=0;i<numsSize-1;i++)
    {
        for(j=i+1;j<numsSize;j++)
        {
            if(nums[i]+nums[j]==target)
            {
                 result=(int*)malloc(sizeof(int)*2);
                 result[0]=i;
                 result[1]=j;
                 return result;
            }
        }
    }
    return result;
}
```

```python
def twoSum(nums, target):
    hashmap={}
    for i,num in enumerate(nums):
        if hashmap.get(target - num) is not None:
            return [i,hashmap.get(target - num)]
        hashmap[num] = i #这句不能放在if语句之前，解决list中有重复值或target-num=num的情况
```

chatGPT：

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    int index;
} HashNode;

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    HashNode* hashTable = (HashNode*)malloc(sizeof(HashNode) * numsSize);

    for (int i = 0; i < numsSize; ++i) {
        int complement = target - nums[i];

        // 在哈希表中查找是否存在 complement
        for (int j = 0; j < i; ++j) {
            if (hashTable[j].key == complement) {
                int* result = (int*)malloc(sizeof(int) * 2);
                result[0] = hashTable[j].index;
                result[1] = i;
                *returnSize = 2;
                free(hashTable);
                return result;
            }
        }

        // 将当前元素加入哈希表
        hashTable[i].key = nums[i];
        hashTable[i].index = i;
    }

    *returnSize = 0;
    free(hashTable);
    return NULL;
}

int main() {
    int nums[] = {2, 7, 11, 15};
    int target = 9;
    int returnSize;

    int* result = twoSum(nums, sizeof(nums) / sizeof(nums[0]), target, &returnSize);

    if (result != NULL) {
        printf("Two Sum found: %d, %d\n", result[0], result[1]);
        free(result);
    } else {
        printf("Two Sum not found.\n");
    }

    return 0;
}
```

Uhash:

```c
struct hashTable {
    int key;
    int val;
    UT_hash_handle hh;
};

struct hashTable* hashtable;

struct hashTable* find(int ikey) {
    struct hashTable* tmp;
    HASH_FIND_INT(hashtable, &ikey, tmp);
    return tmp;
}

void insert(int ikey, int ival) {
    struct hashTable* it = find(ikey);
    if (it == NULL) {
        struct hashTable* tmp = malloc(sizeof(struct hashTable));
        tmp->key = ikey, tmp->val = ival;
        HASH_ADD_INT(hashtable, key, tmp);
    } else {
        it->val = ival;
    }
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    hashtable = NULL;
    for (int i = 0; i < numsSize; i++) {
        struct hashTable* it = find(target - nums[i]);
        if (it != NULL) {
            int* ret = malloc(sizeof(int) * 2);
            ret[0] = it->val, ret[1] = i;
            *returnSize = 2;
            return ret;
        }
        insert(nums[i], i);
    }
    *returnSize = 0;
    return NULL;
}

```





# [数组的相对排序](https://leetcode.cn/problems/relative-sort-array/):

给你两个数组，arr1 和 arr2，arr2 中的元素各不相同，arr2 中的每个元素都出现在 arr1 中。

对 arr1 中的元素进行排序，使 arr1 中项的相对顺序和 arr2 中的相对顺序相同。未在 arr2 中出现过的元素需要按照升序放在 arr1 的末尾。

示例 1：

输入：arr1 = [2,3,1,3,2,4,6,7,9,2,19], arr2 = [2,1,4,3,9,6]
输出：[2,2,2,1,4,3,3,9,6,7,19]
示例  2:

输入：arr1 = [28,6,22,8,44,17], arr2 = [22,28,8,6]
输出：[22,28,8,6,17,44]

```python
class Solution:
    def relativeSortArray(self, arr1: List[int], arr2: List[int]) -> List[int]:
        arr2 += sorted(set(arr1)-set(arr2))
        arr1.sort(key=arr2.index)
        return arr1
```

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int compare (const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

int get_pos(int *arr,int len,int key){
    int left = 0;
    int right = len - 1;
    while(left <= right)
    {
        int mid = (right - left) / 2 + left;
        if (arr[mid] == key)
        {
            return mid;
        }
        else if (arr[mid] > key)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return -1;
}

int *re_order(int *arg1,int len1,int *arg2,int len2){
    int *org = (int *) malloc(len2*sizeof (int));
    memcpy(org,arg2,len2*sizeof (int));
    qsort (arg2, len2, sizeof(int), compare);
    int *result = (int *) malloc(len1*sizeof (int));
    int *cout = (int *) malloc(len2*sizeof (int));
    for(int i=0;i<len1;i++){
        int ret = get_pos(arg2,len2,*(arg1+i));
        if (ret != -1) cout[ret]++;
    }

    int *tmp = result;
    for(int j=0;j<len2;j++){
        int pos = get_pos(arg2,len2,*(org+j));
        while(cout[pos]--){
            *tmp = *(org+j);
            tmp++;
        };
    };

    for(int k=0;k<len1;k++){
        printf("%d ",result[k]);
    }
//
//    return map;
};

int main(){
    int arr1[] = {2,3,1,3,2,4,6,7,9,2,19};
    int arr2[] = {2,1,4,3,9,6};
    re_order(arr1,sizeof (arr1)/sizeof (arr1[0]),arr2,sizeof (arr2)/sizeof (arr2[0]));
}

```



# [最长有效括号](https://leetcode.cn/problems/longest-valid-parentheses/):

输入：s = ")()())"

输出：4

解释：最长有效括号子串是 "()()"

|  )   |  (   |  )   |  (   |  )   |  )   |
| :--: | :--: | :--: | :--: | :--: | :--: |
|  0   |  0   |  0   |  0   |  0   |  0   |

```python
def longestValidParentheses(self, s: str) -> int:
    n = len(s)
    if n == 0:
        return 0
    dp = [0] * n
    res = 0
    for i in range(n):
        if i>0 and s[i] == ")":
            if  s[i - 1] == "(":
                dp[i] = dp[i - 2] + 2
            elif s[i - 1] == ")" and i - dp[i - 1] - 1 >= 0 and s[i - dp[i - 1] - 1] == "(":
                dp[i] = dp[i - 1] + 2 + dp[i - dp[i - 1] - 2]
            if dp[i] > res:
                 res = dp[i]
                    return res
```

```python
def longestValidParentheses(self, s: str) -> int:
    def isValid(x):
        stack = []
        for i in range(len(x)):
            if x[i] == '(':
                stack.append('(');
            elif stack!=[] and stack[-1] == '(':
                stack.pop();
            else:
                return False
            return stack==[]

    if len(s)<2:return 0
    n = len(s)
    for i in range(n if n%2==0 else n-1,0,-2):
        for j in range(n-i+1):
            if isValid(s[j:j+i]):
                return i
        return 0
```



# [括号生成](https://leetcode.cn/problems/generate-parentheses/)(动态规划):

数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且有效的括号组合。

```
输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
```

![](/home/ljxnb/lib/leetcode/1612765058-NToQkc-image.png)

```python
def generateParenthesis(self, n: int) -> List[str]:
    self.res = []
    def dfs(idx, idy, valid):
        if idx==n and idy==n:
            self.res.append(valid)
        if idx < n:
            dfs(idx+1, idy, valid+"(")
        if idy < idx:
            dfs(idx, idy+1, valid+")")
     dfs(0, 0, "")
     return self.res
```

```python
def generateParenthesis(self, n: int) -> List[str]:
    if n <= 0: return []
    res = []

    def dfs(paths, left, right):
        if left > n or right > left: return
        if len(paths) == n * 2:  # 因为括号都是成对出现的
            res.append(paths)
            return

        dfs(paths + '(', left + 1, right)  # 生成一个就加一个
        dfs(paths + ')', left, right + 1)

    dfs('', 0, 0)
    return res
```

xxxxxxxxxx79 1# A Huffman Tree Node2class node:3    def __init__(self, freq, symbol, left=None, right=None):4        # frequency of symbol5        self.freq = freq6​7        # symbol name (character)8        self.symbol = symbol9​10        # node left of current node11        self.left = left12​13        # node right of current node14        self.right = right15​16        # tree direction (0/1)17        self.huff = ''18​19# utility function to print huffman20# codes for all symbols in the newly21# created Huffman tree22​23​24def printNodes(node, val=''):25    # huffman code for current node26    newVal = val + str(node.huff)27​28    # if node is not an edge node29    # then traverse inside it30    if(node.left):31        printNodes(node.left, newVal)32    if(node.right):33        printNodes(node.right, newVal)34​35        # if node is edge node then36        # display its huffman code37    if(not node.left and not node.right):38        print(f"{node.symbol} -> {newVal}")39​40​41# characters for huffman tree42chars = ['a', 'b', 'c', 'd', 'e', 'f']43​44# frequency of characters45freq = [ 5, 9, 12, 13, 16, 45]46​47# list containing unused nodes48nodes = []49​50# converting characters and frequencies51# into huffman tree nodes52for x in range(len(chars)):53    nodes.append(node(freq[x], chars[x]))54​55while len(nodes) > 1:56    # sort all the nodes in ascending order57    # based on theri frequency58    nodes = sorted(nodes, key=lambda x: x.freq)59​60    # pick 2 smallest nodes61    left = nodes[0]62    right = nodes[1]63​64    # assign directional value to these nodes65    left.huff = 066    right.huff = 167​68    # combine the 2 smallest nodes to create69    # new node as their parent70    newNode = node(left.freq+right.freq, left.symbol+right.symbol, left, right)71​72    # remove the 2 nodes and add their73    # parent as new node among others74    nodes.remove(left)75    nodes.remove(right)76    nodes.append(newNode)77​78# Huffman Tree is ready!79printNodes(nodes[0])python

# [电话号码的字母组合](https://leetcode.cn/problems/letter-combinations-of-a-phone-number/):

给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。

给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。

```c
输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
```

```c
    def letterCombinations(self, digits: str) -> list:
        KEY = {'2': ['a', 'b', 'c'],
               '3': ['d', 'e', 'f'],
               '4': ['g', 'h', 'i'],
               '5': ['j', 'k', 'l'],
               '6': ['m', 'n', 'o'],
               '7': ['p', 'q', 'r', 's'],
               '8': ['t', 'u', 'v'],
               '9': ['w', 'x', 'y', 'z']}
        if digits == '':
            return []
        ans = ['']
        for num in digits:
            ans = [pre+suf for pre in ans for suf in KEY[num]]
        return ans
```

# [盛最多水的容器](https://leetcode.cn/problems/container-with-most-water/):(贪心)

![](/home/ljxnb/lib/leetcode/question_11.jpg)

给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。

找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。

返回容器可以储存的最大水量。

说明：你不能倾斜容器。

输入：[1,8,6,2,5,4,8,3,7]
输出：49
解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。

```c
int maxArea(int* height, int heightSize) {
    int left=0,right=heightSize-1,heigh=0,vec=0,max=0;
    while(left<right){
        heigh = height[left] > height[right]?height[right]:height[left];
        vec = heigh*(right-left);
        max = max > vec ?max:vec;
        if(height[left]<height[right]) left++;
        else right--;
    }
    return max;
}
```



```python
def maxArea(self, height: List[int]) -> int:
    l, r = 0, len(height) - 1
    ans = 0
    while l < r:
        area = min(height[l], height[r]) * (r - l)
        ans = max(ans, area)
        if height[l] <= height[r]:  # 移动较小的那一端
            l += 1
        else:
            r -= 1
    return ans
```

# [单词搜索](https://leetcode.cn/problems/word-search/):(DFS)

给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。如果 word 存在于网格中，返回 true ；否则，返回 false 。

单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。

![](/home/ljxnb/lib/leetcode/word2.jpg)

输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
输出：true

```python
    def exist(self, board: List[List[str]], word: str) -> bool:
        m, n = len(board), len(board[0])
        def dfs(x, y, i):
            if board[x][y] == word[i]:
                if i == len(word)-1: return True
                for nx, ny in [[x+1,y], [x-1,y], [x,y+1], [x,y-1]]:
                    if 0 <= nx < m and 0 <= ny < n and board[nx][ny] != '#':
                        c, board[x][y] = board[x][y], '#'
                        print(board)
                        if dfs(nx, ny, i+1): return True
                        board[x][y] = c
            return False
        return any(dfs(x, y, 0) for x in range(m) for y in range(n))
```

```
[['#', 'B', 'C', 'E'], ['B', 'F', 'C', 'S'], ['C', 'D', 'E', 'E']]
[['#', 'B', 'C', 'E'], ['#', 'F', 'C', 'S'], ['C', 'D', 'E', 'E']]
[['#', 'B', 'C', 'E'], ['#', 'F', 'C', 'S'], ['#', 'D', 'E', 'E']]
[['#', 'B', 'C', 'E'], ['#', 'F', 'C', 'S'], ['C', 'D', 'E', 'E']]
[['#', 'B', 'C', 'E'], ['B', 'F', 'C', 'S'], ['C', 'D', 'E', 'E']]
[['#', '#', 'C', 'E'], ['B', 'F', 'C', 'S'], ['C', 'D', 'E', 'E']]
[['#', '#', 'C', 'E'], ['B', 'F', 'C', 'S'], ['C', 'D', 'E', 'E']]
[['#', '#', '#', 'E'], ['B', 'F', 'C', 'S'], ['C', 'D', 'E', 'E']]
[['#', '#', '#', 'E'], ['B', 'F', '#', 'S'], ['C', 'D', 'E', 'E']]
[['#', '#', '#', 'E'], ['B', 'F', '#', 'S'], ['C', 'D', '#', 'E']]
[['#', '#', '#', 'E'], ['B', 'F', '#', 'S'], ['C', 'D', '#', 'E']]
True
```





# [最大矩形](https://leetcode.cn/problems/maximal-rectangle/):(前缀和)

给定一个仅包含 `0` 和 `1` 、大小为 `rows x cols` 的二维二进制矩阵，找出只包含 `1` 的最大矩形，并返回其面积。

![](/home/ljxnb/lib/leetcode/maximal.jpg)

输入：matrix = [["1","0","1","0","0"],["1","0","1","1","1"],["1","1","1","1","1"],["1","0","0","1","0"]]
输出：6
解释：最大矩形如上图所示。

前缀和思想求连续1的个数：
对于pre的计算，当前位置j为0，则pre[j]=0，当前位置为1，则需要加上上一层连续1的个数，即pre[j]+=1。
如第二行（下标从0开始）pre[2]=[3,1,3,2,2,0]，最后多一个0便于单调栈处理
单调栈计算最大矩形：
初始化栈，存入下标-1，便于处理所有元素弹出栈后的情况
当栈顶索引index对应的元素pre[index]大于当前索引为k的元素num时，将栈顶元素index弹出，因为栈中索引对应的值是单调递增的，所以此时栈顶元素stack[-1]到k-1之间的数均是不小于pre[index]的，此时以pre[index]为高的矩阵的长度为k-stack[-1]-1，面积S=pre[index]*(k-stack[-1]-1)
pre数组末尾添加0的目的，就是能将所有元素弹出栈，从而计算所有矩阵面积
利用res记录最大矩阵面积

```python
class Solution:
    def maximalRectangle(self, matrix: List[List[str]]) -> int:
        if not matrix:return 0
        m,n=len(matrix),len(matrix[0])
        # 记录当前位置上方连续“1”的个数
        pre=[0]*(n+1)
        res=0
        for i in range(m):
            for j in range(n):
                # 前缀和
                pre[j]=pre[j]+1 if matrix[i][j]=="1" else 0
            # 单调栈
            stack=[-1]
            for k,num in enumerate(pre):
                while stack and pre[stack[-1]]>num:
                    index=stack.pop()
                    res=max(res,pre[index]*(k-stack[-1]-1))
                stack.append(k)
        return res
```

我在 【84. 柱状图中最大的矩形】多种方法（Python3） 使用了多种方法来解决。 然而在这道题，我们仍然可以使用完全一样的思路去完成。 不熟悉的可以看下我的题解。本题解是基于那道题的题解来进行的。

拿题目给的例子来说：


[
  ["1","0","1","0","0"],
  ["1","0","1","1","1"],
  ["1","1","1","1","1"],
  ["1","0","0","1","0"]
]
我们逐行扫描得到 84. 柱状图中最大的矩形 中的 heights 数组：

![](/home/ljxnb/lib/leetcode/1608968003-FbvkxB-007S8ZIlly1ghlu7999xyj30t21cgtcn.jpg)

这样我们就可以使用`84. 柱状图中最大的矩形` 中的解法来进行了，这里我们使用单调栈来解。

下面的代码直接将 84 题的代码封装成 API 调用了。

```python
class Solution:
    def largestRectangleArea(self, heights: List[int]) -> int:
        n, ans = len(heights), 0
        if n != 0:
            ans = heights[0]
        for i in range(n):
            height = heights[i]
            for j in range(i, n):
                height = min(height, heights[j])
                ans = max(ans, (j - i + 1) * height)
        return ans

    def maximalRectangle(self, matrix: List[List[str]]) -> int:
        m = len(matrix)
        if m == 0: return 0
        n = len(matrix[0])
        heights = [0] * n
        ans = 0
        for i in range(m):
            for j in range(n):
                if matrix[i][j] == "0":
                    heights[j] = 0
                else:
                    heights[j] += 1
            ans = max(ans, self.largestRectangleArea(heights))
        return ans
```



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

round() 方法返回浮点数 x 的四舍五入值，准确的说保留值将保留到离上一位更近的一端（四舍六入）。



# [最长回文子串](https://leetcode.cn/problems/longest-palindromic-substring/):

输入：s = "babad"
输出："bab"
解释："aba" 同样是符合题意的答案。

有两种情况，一种是奇数个回文串，另一种是偶数个回文串，同时考虑即可。

首先，设定一个最大回文子串为空字符串，res = ''，之后再逐步更新维护这个res。

然后，遍历整个数组，i为指针。根据i的位置向前找回文子串，这里要注意，我们只需要去找长度大于res的回文子串就可以了，所以定位到回文子串的开头的前一位应该是：start = i - len(s) - 1, 但是start可能会小于0，因此做一个截断start = max(0, i-len(s)-1)。

最后，根据选定的回文子串的索引范围，对字符串做切片，并判断是否是回文子串。这里需要注意，此时分为开头讨论的两种情况，即奇数个回文子串和偶数个回文子串，命中其一种条件，就把寻找到的回文子串重新赋值给res，这样res的长度会随着指针的遍历越来越大，遍历结束后，res即为最长回文子串。

```python
    def longestPalindrome(self, s: str) -> str:
        res = ''
        for i in range(len(s)):
            start = max(i - len(res) -1, 0)
            temp = s[start: i+1]
            if temp == temp[::-1]:
                print(temp,1)
                res = temp
            else:
                temp = temp[1:]
                if temp == temp[::-1]:
                    res = temp
                print(temp,2)
            print(res,temp,3)
        return res
```

```
b 1
b b 3
a 2
a a 3
bab 1
bab bab 3
aba 2
aba aba 3
abad 2
aba abad 3
aba
```



# [有效的括号](https://leetcode.cn/problems/valid-parentheses/):(栈操作)

给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。

有效字符串需满足：

左括号必须用相同类型的右括号闭合。
左括号必须以正确的顺序闭合。

```python
    def isValid(self, s):
        while '{}' in s or '()' in s or '[]' in s:
            s = s.replace('{}', '')
            s = s.replace('[]', '')
            s = s.replace('()', '')
        return s == ''
```

![](/home/ljxnb/lib/leetcode/median_of_two_sorted_arrays/baa8829ac398e665eb645dca29eadd631e2b337e05022aa5a678e091471a4913-20.gif)

```python
    def isValid(self, s: str) -> bool:
        dic = {')':'(',']':'[','}':'{'}
        stack = []
        for i in s:
            if stack and i in dic:
                if stack[-1] == dic[i]: stack.pop()
                else: return False
            else: stack.append(i)
            
        return not stack
```

# [单词拆分](https://leetcode.cn/problems/word-break/):(huawei)

给你一个字符串 s 和一个字符串列表 wordDict 作为字典。请你判断是否可以利用字典中出现的单词拼接出 s 。

注意：不要求字典中出现的单词全部都使用，并且字典中的单词可以重复使用。

输入: s = "leetcode", wordDict = ["leet", "code"]
输出: true
解释: 返回 true 因为 "leetcode" 可以由 "leet" 和 "code" 拼接成。

![](/home/ljxnb/lib/leetcode/2a834dafa7bf590df1413fc742b07099854b6c6b842a5f7677564ccd044b5d69.png)

1. $$
   初始化 dp=[False,\cdots,False]dp=[False,⋯,False]，长度为 n+1。n 为字符串长度。\\dp[i] 表示 s的前 i位是否可以用 wordDictwordDict 中的单词表示。
   $$

2. $$
   初始化 dp[0]=True，空字符可以被表示。
   $$

3. $$
   遍历字符串的所有子串，遍历开始索引 i，遍历区间 [0,n)[0,n)：
   $$


* $$
  遍历结束索引 j，遍历区间 [i+1,n+1)[i+1,n+1)：
  $$

  * $$
    若 dp[i]=True 且 s[i,\cdots,j)s[i,⋯,j) 在 wordlistwordlist 中：\\dp[j]=True。解释：dp[i]=True 说明 s 的前 i 位可以用 \\wordDictwordDict 表示，则 s[i,\cdots,j)s[i,⋯,j)\\ 出现在 wordDictwordDict 中，
    说明 s 的前 j 位可以表示。
    $$
  
    


```python
    def wordBreak(self, s: str, wordDict: List[str]) -> bool:       
        n=len(s)
        dp=[False]*(n+1)
        dp[0]=True
        for i in range(n):
            for j in range(i+1,n+1):
                if(dp[i] and (s[i:j] in wordDict)):
                    dp[j]=True
        return dp[-1]
```

简便解法：

```python
def wordBreak(s: str, wordDict):
    for i in wordDict:
        s = s.replace(i, "")
    return s == ""
```

# [森林中的兔子](https://leetcode.cn/problems/rabbits-in-forest/):(huawei)

森林中有未知数量的兔子。提问其中若干只兔子 "还有多少只兔子与你（指被提问的兔子）颜色相同?" ，将答案收集到一个整数数组 answers 中，其中 answers[i] 是第 i 只兔子的回答。

输入：answers = [1,1,2]
输出：5
解释：
两只回答了 "1" 的兔子可能有相同的颜色，设为红色。 
之后回答了 "2" 的兔子不会是红色，否则他们的回答会相互矛盾。
设回答了 "2" 的兔子为蓝色。 
此外，森林中还应有另外 2 只蓝色兔子的回答没有包含在数组中。 
因此森林中兔子的最少数量是 5 只：3 只回答的和 2 只没有回答的。

数组中出现的每一个数字key代表某一种颜色的兔子有(key+1)只，如下图，我们举个例子，一目了然：

![](/home/ljxnb/lib/leetcode/1617507143-jQKEGE-image.png)

```python
    def numRabbits(self, answers: List[int]) -> int:
        ans_dic = {}
        for i in answers:
            if i not in ans_dic:
                ans_dic[i] = 1
            else:
                ans_dic[i] += 1
        res = 0
        for key, val in ans_dic.items():
            tmp = val // (key + 1)
            if val % (key+1) !=0:
                tmp += 1
            res += tmp*(key+1)
        return res
```

# [有效的数独:](https://leetcode.cn/problems/valid-sudoku/)

请你判断一个 9 x 9 的数独是否有效。只需要 根据以下规则 ，验证已经填入的数字是否有效即可。

数字 1-9 在每一行只能出现一次。
数字 1-9 在每一列只能出现一次。
数字 1-9 在每一个以粗实线分隔的 3x3 宫内只能出现一次。（请参考示例图）


注意：

一个有效的数独（部分已被填充）不一定是可解的。
只需要根据以上规则，验证已经填入的数字是否有效即可。
空白格用 '.' 表示。

![](/home/ljxnb/lib/leetcode/250px-sudoku-by-l2g-20050714svg.png)

```
输入：board = 
[["5","3",".",".","7",".",".",".","."]
,["6",".",".","1","9","5",".",".","."]
,[".","9","8",".",".",".",".","6","."]
,["8",".",".",".","6",".",".",".","3"]
,["4",".",".","8",".","3",".",".","1"]
,["7",".",".",".","2",".",".",".","6"]
,[".","6",".",".",".",".","2","8","."]
,[".",".",".","4","1","9",".",".","5"]
,[".",".",".",".","8",".",".","7","9"]]
输出：true
```

```python
class Solution:
    def isValidSudoku(self, board: List[List[str]]) -> bool:
        matrix_line = [set() for i in range(9)]
        matrix_column = [set() for i in range(9)]
        matrix_area = [set() for i in range(9)]
        for i in range(9):
            for j in range(9):
                item = board[i][j]
                pos = (i//3)*3 + j //3
                if item != '.':
                    if item not in matrix_line[i] and item not in matrix_column[j] and item not in matrix_area[pos]:
                        matrix_line[i].add(item)
                        matrix_column[j].add(item)
                        matrix_area[pos].add(item)
                    else:
                        return False
        return True
```

# 滑动窗口

给定一个含有 `n` 个正整数的数组和一个正整数 `target` **。**

找出该数组中满足其和 `≥ target` 的长度最小的 **连续子数组** `[numsl, numsl+1, ..., numsr-1, numsr]` ，并返回其长度**。**如果不存在符合条件的子数组，返回 `0` 。

**示例 1：**

```
输入：target = 7, nums = [2,3,1,2,4,3]
输出：2
解释：子数组 [4,3] 是该条件下的长度最小的子数组。
```

**示例 2：**

```
输入：target = 4, nums = [1,4,4]
输出：1
```

**示例 3：**

```
输入：target = 11, nums = [1,1,1,1,1,1,1,1]
输出：0
```

**提示：**

- `1 <= target <= 109`
- `1 <= nums.length <= 105`
- `1 <= nums[i] <= 105`

**进阶：**

- 如果你已经实现 `O(n)` 时间复杂度的解法, 请尝试设计一个 `O(n log(n))` 时间复杂度的解法。

题目解析

很经典的**滑动窗口**的题目。

具体操作如下：

1、初始化一个滑动窗口，左边界位置为 0，接下来开始设置右边界的位置，通过数组不断的去调整右边界的位置，所以一开始右边界的位置也是 0，如果数组为空，那么这个窗口就不存在；如果数组不为空，那么窗口就是第一个元素。

![](/home/ljxnb/lib/leetcode/640.png)

2、计算此时**滑动窗口**里面所有元素的值。

3、如果滑动窗口的元素和小于了 target，那么说明**还没有**找到了一个满足条件的**连续子数组**，需要继续扩充滑动窗口，扩充的方式就是让右边界向右移动。

![](/home/ljxnb/lib/leetcode/640 (1).png)

4、如果滑动窗口的元素和大于等于了 target，那么说明找到了一个满足条件的**连续子数组**，计算出这个**连续子数组**的长度来，把这个结果 result 和之前的保存的结果进行对比取最小值。

![](/home/ljxnb/lib/leetcode/640 (2).png)

5、基于 4 的情况，需要开始尝试缩小滑动窗口，去寻找更短的滑动窗口，缩小的方法就是不断的移动左边界，直到滑动窗口里面的元素和小于了 target 为止，在移动过程中，如果发现滑动窗口的元素和依旧大于等于了 target，继续缩小同时更新 result。

![](/home/ljxnb/lib/leetcode/640 (3).png)

6、最后需要注意一点， 一开始，result 可以初始化为一个超过数组长度的值，这样的目的是为了最后返回结果的时候判断 result 有没有被更新，如果没有被更新，并且滑动窗口的长度不可能为 result，因为超过了数组的长度，那就代表不存在符合条件的子数组，需要返回 0 ，比如 `target = 11`， `nums = [1,1,1,1,1,1,1,1]`， 先设置 `result = 9`，执行完后续代码，result 依旧为 9，代表 nums 里面找不到一个子数组和大于等于 11 ，需要返回 0。

```python
class Solution:
    def minSubArrayLen(self, target: int, nums: List[int]) -> int:
        # 滑动窗口的左端
        left = 0

        # 滑动窗口中所有元素的和
        sum = 0

        # 记录滑动窗口的长度，并且不断更新获取最小的那个
        # 一开始，result 可以初始化为一个超过数组长度的值
        # 这样的目的是为了最后返回结果的时候判断 result 有没有被更新
        # 如果没有被更新，并且滑动窗口的长度不可能为 result，因为超过了数组的长度
        # 那就代表不存在符合条件的子数组，需要返回 0 
        # 比如 target = 11, nums = [1,1,1,1,1,1,1,1]
        # 先设置 result = 9，执行完后续代码，result 依旧为 9
        # 代表 nums 里面找不到一个子数组和大于等于 11 ，需要返回 0
        result = len(nums) + 1
        
        # 滑动窗口的右端从 0 开始，这样，当 nums 为空时，可以直接跳出 for 循环
        for right in range(len(nums)) :

            # 滑动窗口中加入了 nums[right] 这个元素
            # 滑动窗口元素和需要发生变化
            sum += nums[right]

            # 变化之后需要判断一下，如果滑动窗口的元素和大于等于了 target
            # 那么这个时候就需要不断的向右移动 left，缩小滑动窗口的长度
            while sum >= target : 
                
                # 在获取到一个满足要求的子数组时，更新 result 的值
                result = min(result, right - left + 1)

                # 把 nums[left] 移除滑动窗口
                sum -= nums[left]

                # 即 left 向右移动
                left += 1

      
        # 返回结果
        return 0 if result == len(nums) + 1 else result
```

# 水果搬运问题(排序)

# 题目描述

一组工人搬运一批水果，用[一维数组](https://so.csdn.net/so/search?q=一维数组&spm=1001.2101.3001.7020)存储工人编号和水果名称以及搬运重量，要求先按水果分组，然后按搬运重量排序输出。

输入描述：
接下来的 N 行每行包括两个整数 p 和 q,分别代表每个工人的编号和搬运重量，以及一个字符串 m,代表水果的名称。

输出描述：
先按水果分组，然后按工人的搬运重量从小到大进行排序，并将排序后的信息打印出来。如果工人搬运的重量相同，则按照编号的大小从小到大排序，并且要求水果的输
出次序同输入次序。

示例 1

输入：

```
5
Apple 1 80
Apple 2 62
Apple 4 73
Orange 4 65
Orange 1 90
Apple 3 91
Orange 3 88
Orange 5 90
```

输出：

```
Apple 2 62
Apple 4 73
Apple 1 80
Apple 3 91
Orange 4 65
Orange 3 88
Orange 1 90
Orange 5 90
```

```python
while True:
    try:
        grep = []
        num = int(input())
        for i in range(num):
            grep.append(list(input().split()))
        grep.sort(key=lambda x:(x[0],x[2],x[1]))
        for i in range(len(grep)):
            print(" ".join(grep[i]))
    except:
        break
```

# 解密犯罪时间

题目描述
警察在侦破一个案件时，得到了线人给出的可能犯罪时间，形如 “HH:MM” 表示的时刻。

根据警察和线人的约定，为了隐蔽，该时间是修改过的。

解密规则为：利用当前出现过的数字，构造下一个距离当前时间最近的时刻，则该时间为可能的犯罪时间。

每个出现数字都可以被无限次使用。

输入描述：

形如HH:MM字符串，表示原始输入。

输出描述：

形如HH:MM的字符串，表示推理处理的犯罪时间。

备注
1.可以保证现任给定的字符串一定是合法的。
例如，“01:35”和“11:08”是合法的，“1:35”和“11:8”是不合法的。
2.最近的时刻可能在第二天。

示例 1
输入：20:12
输出：20:20

示例 2
输入：23:59
输出：22:22

示例 3
输入：12:58
输出：15:11

示例 4
输入：18:52
输出：18:55

参考代码
方法一：
生成近两天满足要求的时间，取最近的

```python
while 1:
    try:
        hh, mm = input().split(":")

        chars = set(list(hh) + list(mm))
        hhmm = hh+mm

        dp = []

        def dfs(hm_):
            lens = len(hm_)
            if lens == 2 and hm_ > "23":
                return
            elif lens == 3 and (hm_ > "235" or hm_[2] > "5"):
                return
            elif lens >= 4:
                if hm_ > hhmm:
                    dp.append(hm_)
                else:
                    dp.append("1"+hm_)
            else:
                for c in chars:
                    dfs(hm_+c)

        for c in chars:
            if c <= "2":
                dfs(c)

        ret = sorted(dp, key=lambda x: int(x))[0]
        if len(ret) == 5:
            print(f"{ret[1:3]}:{ret[3:]}")
        else:
            print(f"{ret[:2]}:{ret[2:]}")
    except Exception as e:
        break

```

# 数据分组

题目描述
给一个数组对元素分组，要求只有满足 1、每组里的数据都相等； 2、每组的元素个数都是 N（N>=2），满足两个条件才允许分组，返回从小到大的分组。

示例 1
输入：1,2,2,1
输出：
1,1
2,2

示例 2
输入：1,1,4,3,3
输出：
-1

参考代码

```python
while 1:
    try:
        nums = list(map(int, input().split(",")))

        dct = {}
        for c in nums:
            if c in dct:
                dct[c] += 1
            else:
                dct[c] = 1
        if len(set(dct.values())) == 1:
            count = max(dct.values())
            keys = sorted(dct.keys())

            for i in keys:
                print(",".join([str(i)]*count))
        else:
            print(-1)

    except Exception as e:
        break
```

# 员工工号问题

# 题目描述

公司员工的工号规则为:
小写字母+数字，总长度不能超过8位，x表示该工号类型可以容纳的员工人数，y表示字母的个数，请确定数字的最小个数。

示例 1

输入：260 1
输出：1

示例 2

输入：2600 1
输出：2

```python
while 1:
    try:
        max_, y = list(map(int, input().split()))

        z = 26**y
        for i in range(1, 9-y):
            if z * (10**i) >= max_:
                print(i)
                break

    except Exception as e:
        break
```

# 输出匹配字符串的开始下标

# 题目描述

给出母串和子串，输出子串能够在母串完全匹配的开始位置。

比如 asdfasdfa， fas 输出 3，就是最小下标。

**输入描述：**
第一行是母串
第二行是子串

**输出描述：**
输出子串能够在母串完全匹配的开始位置。

示例 1

输入：asdfasdfa
输出：fas

参考代码

```python
while 1:
    try:
        a = input()
        b = input()
        if b in a:
            print(a.index(b))
        else:
            print(-1)
    except Exception as e:
        break

```

# 带空白字符的字符串匹配

题目描述

给定两行空的[字符串](https://so.csdn.net/so/search?q=字符串&spm=1001.2101.3001.7020)，第一行的字符串包含了部分空白（空格、 tab），第二行字符串不包含任何空白，请从第一行字符串中匹配第二行字符串，匹配时忽略空白及 tab，输出
第一行字符串中第二行字符串出现的次数；

**输入描述：**
第一行输入小于 1K 的字符串，包含了部分空白（空格、 tab）
第二行输入小于 1K 的字符串，不包含任何空白

**输出描述：**
输出匹配到的次数（如果遇到连续字符如 bbb,查找 bb,记为匹配两次）

示例 1

输入：

```python
Abb ba
bb
```

输出：
2

```python
while 1:
    try:
        nums = input().replace(" ", "").replace("\t", "")
        sub = input()

        count = 0
        while len(nums):
            if sub in nums:
                i = nums.index(sub)
                nums = nums[i+1:]
                count += 1
            else:
                break

        print(count)
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

# 内存资源分配

题目描述
有一个简易内存池，内存按照大小粒度分类，每个粒度有若干个可用内存资源，用户会进行一系列内存申请，需要按需分配内存池中的资源，返回申请结果成功失败列表。
分配规则如下：
1.分配的内存要大于等于内存申请量，存在满足需求的内存就必须分配，优先分配粒度小的，但内存不能拆分使用。
2.需要按申请顺序分配，先申请的先分配。
3.有可用内存分配则申请结果为true，没有可用内存分配则返回false。
注：不考虑内存释放。

输入描述
第一行为内存池资源列表，包含内存粒度数据信息，粒度数据间用逗号分割，一个粒度信息内部用冒号分割，冒号前为内存粒度大小，冒号后为数量。资源列表不大于1024，每个粒度的数量不大于4096。
第二行为申请列表，申请的内存大小间用逗号分隔。申请列表不大于100000。

输出描述
输出为内存池分配结果。

示例
输入：
64:2,128:1,32:4,1:128
50,36,64,128,127

输出：
true,true,true,false,false

说明：
内存池资源包含：64K共2个、128K共1个、32K共4个、1K共128个的内存资源；
针对50,36,64,128,127的内存申请序列，分配的内存依次是：64,64,128,NULL,NULL,
第三次申请内存时已经将128分配出去，因此输出结果是：true,true,true,false,false

```python
s = input().split(",")
a = list(map(int,input().split(",")))
ds = {}
ls = ""
for i in s:
    size,n = map(int,i.split(":"))
    ls += n * (" "+str(size))
    ds[size] = n
tls = sorted(list(map(int,ls.split())))
res = []
def check(i):
    for j,v in enumerate(tls):
        if v >= i:
            tls.pop(j)
            return "true"
    return "false"
for i in a:
    res.append(check(i))
print(",".join(res))
```

# 模拟内存分配

请实现一个简易内存池，根据请求命令完成内存分配和释放。
内存池支持两种操作命令，REQUEST和RELEASE，其格式为：
REQUEST=请求的内存大小 表示请求分配指定大小内存，如果分配成功，返回分配到的内存首地址；如果内存不足，或指定的大小为0，则输出error。
RELEASE=释放的内存首地址 表示释放掉之前分配的内存，释放成功无需输出，如果释放不存在的首地址则输出error。
注意：
1.内存池总大小为100字节。
2.内存池地址分配必须是连续内存，并优先从低地址分配。
3.内存释放后可被再次分配，已释放的内存在空闲时不能被二次释放。
4.不会释放已申请的内存块的中间地址。
5.释放操作只是针对首地址所对应的单个内存块进行操作，不会影响其它内存块。
解答要求
时间限制: 1000ms, 内存限制: 256MB
首行为整数 N , 表示操作命令的个数，取值范围：0 < N <= 100。
接下来的N行, 每行将给出一个操作命令，操作命令和参数之间用 “=”分割。

输入样例1
2
REQUEST=10
REQUEST=20
输出样例1
0
10

输入样例2
5
REQUEST=10
REQUEST=20
RELEASE=0
REQUEST=20
REQUEST=10
输出样例2
0
10
30
0

# n皇后问题：
输入整数n，要求n个国际象棋的皇后，摆在n*n的棋盘上，互相不能攻击，输出全部方案。

输入：
输入一个正整数N。

输出：
程序输出N皇后问题的全部摆法。
行里的第i个数字如果是n，就代表第i行的皇后应该放在第n列。
皇后的行、列编号都是从1开始算起。

输入样例：
4
输出样例：
2 4 1 3
3 1 4 2

解题思路：
国际象棋皇后的走法是横、直、斜均可走，但每次只能走一格。

问题要求把n个皇后放在一个n*n的棋盘上，使得任何两个皇后都不能相互攻击，即它们不能同行，不能同列，也不能位于同一条对角线上。对于n= 1，问题的解很简单，而且很容易看出对于n=2和n=3来说，这个问题是无解的。

所以我们以4皇后问题为例，用递归法对它求解。
因为每个皇后都必须分别占据一行，我们需要做的不过是为棋盘上的每个皇后分配一列。

我们从空棋盘开始，然后把皇后1放到它所在行的第一个可能位置上，也就是第一行第一列。对于皇后2,在经过第一列和第二列的失败尝试之后，我们把它放在第一个可能的位置，就是格子(2, 3)，位于第二行第三列的格子。这被证明是一个死胡同，因为皇后3将没有位置可放。所以，该算法进行递归，把皇后2放在下一个可能位置(2, 4)上。这样皇后3就可以放在(3, 2)，这被证明是另一个死胡同。该算法然后就递归到底，把皇后1移到(1,2)， 接着皇后2到(2,4)， 皇后3到(3,1), 而皇后4到(4, 3)， 这就是该问题的一个解。

![](/home/ljxnb/lib/leetcode/20210327101853420.png)

```c
#include<stdio.h>
#include<math.h>
int N;//皇后的数量，以及棋盘的大小 N*N 
int queenPos[100];//皇后在棋盘中每一行的列号 
void NQueen(int k)//表示行位置 
{
	int i;//表示列位置 
	if(k==N)//当k=n时，就是要摆放最后一行的皇后 
	{
		for(i=0;i<N;i++)
		{
			printf("%d",queenPos[i]+1);	//C语言从0开始计数，所有输出需要加1		
		}		
	}
	else 
	{
		for(i=0;i<N;i++)//遍历每一个皇后 
		{
			int j;
			for(j=0;j<k;j++)
			{
				if(queenPos[j]==i||abs(queenPos[j] - i)==abs(k-j))//queenPos[j]相当于上一层的queenPos[k]的值 
				{
					break;
				}
				
			}
			if(j==k)						
	        {	        	     	
		        queenPos[k]=i;	
				NQueen(k+1);	
	        } 
	
		} 
	}
	return ;
	
} 
int main()
{
	scanf("%d",&N);//输入皇后数量 
    NQueen(0);	
	return 0;		
} 

```

```python
def noConflicts(board,current):
    for i in range(current):
        if (board[i] == board[current]):
            return False
        if current - i == abs(board[current] - board[i]):
            return False
    return True

def queens(board,current,size):
    print(board)
    if current == size:
        return True
    else:
        for i in range(size):
            board[current] = i
            if noConflicts(board, current):
                done = queens(board,current+1,size)
                if done:
                    return True
        return False

def calcQueens(size):
    board = [-1]*size
    return queens(board,0,size)
```

[135. 分发糖果](https://leetcode.cn/problems/candy/)

`n` 个孩子站成一排。给你一个整数数组 `ratings` 表示每个孩子的评分。

你需要按照以下要求，给这些孩子分发糖果：

- 每个孩子至少分配到 `1` 个糖果。
- 相邻两个孩子评分更高的孩子会获得更多的糖果。

请你给每个孩子分发糖果，计算并返回需要准备的 **最少糖果数目** 。

 

**示例 1：**

```
输入：ratings = [1,0,2]
输出：5
解释：你可以分别给第一个、第二个、第三个孩子分发 2、1、2 颗糖果。
```

**示例 2：**

```
输入：ratings = [1,2,2]
输出：4
解释：你可以分别给第一个、第二个、第三个孩子分发 1、2、1 颗糖果。
     第三个孩子只得到 1 颗糖果，这满足题面中的两个条件。
```

 

**提示：**

- `n == ratings.length`
- `1 <= n <= 2 * 104`
- `0 <= ratings[i] <= 2 * 104`

方法二：常数空间遍历
思路及解法

注意到糖果总是尽量少给，且从 111 开始累计，每次要么比相邻的同学多给一个，要么重新置为 111。依据此规则，我们可以画出下图：

其中相同颜色的柱状图的高度总恰好为 1,2,3…1,2,3 \dots1,2,3…。

而高度也不一定一定是升序，也可能是 …3,2,1\dots 3,2,1…3,2,1 的降序：

注意到在上图中，对于第三个同学，他既可以被认为是属于绿色的升序部分，也可以被认为是属于蓝色的降序部分。因为他同时比两边的同学评分更高。我们对序列稍作修改：

注意到右边的升序部分变长了，使得第三个同学不得不被分配 444 个糖果。

依据前面总结的规律，我们可以提出本题的解法。我们从左到右枚举每一个同学，记前一个同学分得的糖果数量为 $$\textit{pre}$$：

如果当前同学比上一个同学评分高，说明我们就在最近的递增序列中，直接分配给该同学 $$\textit{pre} + 1$$ 个糖果即可。

否则我们就在一个递减序列中，我们直接分配给当前同学一个糖果，并把该同学所在的递减序列中所有的同学都再多分配一个糖果，以保证糖果数量还是满足条件。

我们无需显式地额外分配糖果，只需要记录当前的递减序列长度，即可知道需要额外分配的糖果数量。

同时注意当当前的递减序列长度和上一个递增序列等长时，需要把最近的递增序列的最后一个同学也并进递减序列中。

这样，我们只要记录当前递减序列的长度 $$\textit{dec}$$，最近的递增序列的长度 $$\textit{inc}$$ 和前一个同学分得的糖果数量 $$\textit{pre}$$ 即可。

代码

```c
int candy(int* ratings, int ratingsSize) {
    int ret = 1;
    int inc = 1, dec = 0, pre = 1;
    for (int i = 1; i < ratingsSize; i++) {
        if (ratings[i] >= ratings[i - 1]) {
            dec = 0;
            pre = ratings[i] == ratings[i - 1] ? 1 : pre + 1;
            ret += pre;
            inc = pre;
        } else {
            dec++;
            if (dec == inc) {
                dec++;
            }
            ret += dec;
            pre = 1;
        }
    }
    return ret;
}
```

复杂度分析

时间复杂度：O(n)，其中 n 是孩子的数量。我们需要遍历两次数组以分别计算满足左规则或右规则的最少糖果数量。

空间复杂度：O(1)。我们只需要常数的空间保存若干变量。

[17. 电话号码的字母组合](https://leetcode.cn/problems/letter-combinations-of-a-phone-number/)

给定一个仅包含数字 `2-9` 的字符串，返回所有它能表示的字母组合。答案可以按 **任意顺序** 返回。

给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。

![img](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2021/11/09/200px-telephone-keypad2svg.png)

 

**示例 1：**

```
输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
```

**示例 2：**

```
输入：digits = ""
输出：[]
```

**示例 3：**

```
输入：digits = "2"
输出：["a","b","c"]
```

 

**提示：**

- `0 <= digits.length <= 4`
- `digits[i]` 是范围 `['2', '9']` 的一个数字。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generateCombinations(char *digits, char *current, int index, char *mapping[], char **result, int *resultIndex) {
    if (index == strlen(digits)) {
        // 当前组合已经完成
        current[index] = '\0';
        result[(*resultIndex)++] = strdup(current);
        return;
    }

    // 获取当前数字对应的字母
    char *letters = mapping[digits[index] - '2'];

    // 遍历当前数字对应的所有字母
    for (int i = 0; i < strlen(letters); i++) {
        current[index] = letters[i];
        generateCombinations(digits, current, index + 1, mapping, result, resultIndex);
    }
}

char ** letterCombinations(char *digits, int *returnSize) {
    if (digits == NULL || *digits == '\0') {
        *returnSize = 0;
        return NULL;
    }

    // 数字到字母的映射表
    char *mapping[] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

    // 计算结果数组的大小
    int resultSize = 1;
    for (int i = 0; i < strlen(digits); i++) {
        resultSize *= strlen(mapping[digits[i] - '2']);
    }

    // 分配结果数组的空间
    char **result = (char **)malloc(resultSize * sizeof(char *));
    int resultIndex = 0;

    // 分配当前组合的空间
    char *current = (char *)malloc((strlen(digits) + 1) * sizeof(char));

    // 生成字母组合
    generateCombinations(digits, current, 0, mapping, result, &resultIndex);

    // 释放临时空间
    free(current);

    // 设置返回结果的大小
    *returnSize = resultIndex;

    return result;
}

int main() {
    char digits[] = "23";
    int returnSize;
    char **result = letterCombinations(digits, &returnSize);

    // 打印结果
    printf("[");
    for (int i = 0; i < returnSize; i++) {
        printf("\"%s\"", result[i]);
        if (i < returnSize - 1) {
            printf(",");
        }
        free(result[i]);
    }
    printf("]\n");

    // 释放结果数组
    free(result);

    return 0;
}
```

复杂度分析

时间复杂度：$$O(3^m×4^n)$$，其中 mmm 是输入中对应 333 个字母的数字个数（包括数字 222、333、444、555、666、888），nnn 是输入中对应 444 个字母的数字个数（包括数字 777、999），m+nm+nm+n 是输入数字的总个数。当输入包含 mmm 个对应 333 个字母的数字和 nnn 个对应 444 个字母的数字时，不同的字母组合一共有 3m×4n3^m \times 4^n3 
m
 ×4 
n
  种，需要遍历每一种字母组合。

空间复杂度：O(m+n)，其中 mmm 是输入中对应 333 个字母的数字个数，nnn 是输入中对应 444 个字母的数字个数，m+nm+nm+n 是输入数字的总个数。除了返回值以外，空间复杂度主要取决于哈希表以及回溯过程中的递归调用层数，哈希表的大小与输入无关，可以看成常数，递归调用层数最大为 m+nm。



[51. N 皇后](https://leetcode.cn/problems/n-queens/)

按照国际象棋的规则，皇后可以攻击与之处在同一行或同一列或同一斜线上的棋子。

**n 皇后问题** 研究的是如何将 `n` 个皇后放置在 `n×n` 的棋盘上，并且使皇后彼此之间不能相互攻击。

给你一个整数 `n` ，返回所有不同的 **n 皇后问题** 的解决方案。

每一种解法包含一个不同的 **n 皇后问题** 的棋子放置方案，该方案中 `'Q'` 和 `'.'` 分别代表了皇后和空位。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2020/11/13/queens.jpg)

```
输入：n = 4
输出：[[".Q..","...Q","Q...","..Q."],["..Q.","Q...","...Q",".Q.."]]
解释：如上图所示，4 皇后问题存在两个不同的解法。
```

**示例 2：**

```
输入：n = 1
输出：[["Q"]]
```

 

**提示：**

- 1 <= n <= 9



N皇后是很经典的回溯算法问题，解数独也是，二者有异曲同工之妙；

回溯算法的我看过通俗易懂的讲解是：https://leetcode-cn.com/problems/n-queens/solution/hui-su-suan-fa-xiang-jie-by-labuladong/；

我主要想分享一下在做选择时 check 的过程，由于我在看别人题解时，回溯算法的整体思想都是一个路子，但是在判断该位置是否满足放置皇后的条件时的判断却各有章法，且无详细讲解；

思路
使用一个 visited 一维数组保存每层存放皇后的列下标，这样就可以知道每行放置皇后的位置； 例如：

visited[0] = 1 //第0行第1列放置皇后
visited[1] = 3 //第1行第3列放置皇后
在本行选择一列放置皇后时，只需判断该位置是否在前面已放置的皇后的攻击范围内（列，主对角线，次对角线）；

一行只放一个皇后,所以不用判断行

那如何判断呢? 举个例子:

n = 4;
假设已经选择了两行放置皇后,分别是: 第0行第1列(0,1), 第1行第3列(1,3);
故 visited = { 1, 3, 0, 0 };


现在在第 2 行选择列:

列: 排除当前列与已放置皇后在同一列,即 visited[0],visited[1]; col == visited[i]
对角线: 排除当前列与已放置皇后在相同对角线上
主对角线: 行 - 列相同 row - col == i - visited[i];
次对角线: 行 + 列相同 row + col == (visited[i] + i);


写好判断之后就是选择,撤销,满足保存;

参考代码

```c
bool check(int* visited, int row, int col) {
  for (int i = 0; i < row; i++)
    if (col == visited[i] || row + col == (visited[i] + i) || row - col == i - visited[i])
      return false;
  return true;
}
void backtrack(int n, int row, int* visited, char*** result, int* returnSize) {
  if (row == n) {
    result[*returnSize] = (char**)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
      result[*returnSize][i] = (char*)calloc(n + 1, sizeof(char));
      memset(result[*returnSize][i], '.', n);
      result[*returnSize][i][visited[i]] = 'Q';
    }
    (*returnSize)++;
    return;
  }
  for (int i = 0; i < n; i++) {
    if (!check(visited, row, i)) continue;
    visited[row] = i;
    backtrack(n, row + 1, visited, result, returnSize);
    // visited[row] = 0; //此句无关紧要，但是回溯的思想
  }
}
char*** solveNQueens(int n, int* returnSize, int** returnColumnSizes) {
  *returnSize = 0;
  int* visited = (int*)calloc(n, sizeof(int));
  char*** result = (char***)malloc(n * n * n * sizeof(char**));
  backtrack(n, 0, visited, result, returnSize);
  *returnColumnSizes = (int*)malloc(*returnSize * sizeof(int));
  for (int i = 0; i < *returnSize; i++)
    (*returnColumnSizes)[i] = n;
  return result;
}

```

[54. 螺旋矩阵](https://leetcode.cn/problems/spiral-matrix/)

给你一个 `m` 行 `n` 列的矩阵 `matrix` ，请按照 **顺时针螺旋顺序** ，返回矩阵中的所有元素。 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2020/11/13/spiral1.jpg)

```
输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
输出：[1,2,3,6,9,8,7,4,5]
```

**示例 2：**

![img](https://assets.leetcode.com/uploads/2020/11/13/spiral.jpg)

```
输入：matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
输出：[1,2,3,4,8,12,11,10,9,5,6,7]
```

 

**提示：**

- `m == matrix.length`
- `n == matrix[i].length`
- `1 <= m, n <= 10`
- `-100 <= matrix[i][j] <= 100`

 将各个方向的运动抽象成运动向量； 转向的条件在于碰到原矩阵边界或碰到已访问元素； 全部结束的条件在于转向后一个元素也没有访问到，这说明原矩阵的所有元素均已被访问，则全部结束即可；

```c
const int dirx[4] = { 0, 1, 0, -1 };
const int diry[4] = { 1, 0, -1, 0 };

bool Legal(int x, int y, int dx, int dy, int matrixSize, int* matrixColSize, int** isvisit) {
	int x1 = x + dx;
	int y1 = y + dy;
	if (x1 < 0 || x1 >= matrixSize || y1 < 0 || y1 >= matrixColSize[0] || isvisit[x1][y1]) {
		return false;
	}
	return true;
}

int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize) {
	*returnSize = matrixSize * (matrixColSize[0]);
	int* ra = (int*)malloc(sizeof(int) * (*returnSize));
	int ra_ind = 0;
	//初始化返回数组并维护当前赋值位置；
	int** isvisit = (int**)malloc(sizeof(int*) * (matrixSize));
	for (int i = 0; i < matrixSize; i++) {
		isvisit[i] = (int*)malloc(sizeof(int) * (*matrixColSize));
		memset(isvisit[i], 0, sizeof(int) * (*matrixColSize));
	}//初始化访问标志数组；
	int x = 0, y = -1;//初始化操作位置；
	int turn = 0;
	while (true)
	{
		int dx = dirx[(turn % 4)];
		int dy = diry[(turn % 4)];
		int flag = 0;
		while (Legal(x, y, dx, dy, matrixSize, matrixColSize, isvisit)) {
			//先进行判断，保证每次循环完成都会停在最后操作的位置上，而不是走到最后操作位置的下一个位置；
			flag = 1;
			x += dx;
			y += dy;
			isvisit[x][y] = 1;
			ra[ra_ind] = matrix[x][y];
			ra_ind++;
		}
		if (!flag) {
			break;
		}//如果在某次转向后一个元素也没有访问到，说明原矩阵元素已经全部访问完毕，那么就退出整个大循环；
		turn++;
	}
	return ra;
}

```

Mine:

```c
#include <stdlib.h>
#include <stdio.h>

const int dirx[4] = { 0, 1, 0, -1 };
const int diry[4] = { 1, 0, -1, 0 };

int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize) {
	*returnSize = matrixSize * (matrixColSize[0]);
	int* ra = (int*)malloc(sizeof(int) * (*returnSize));
	int ra_ind = 0;
    int col = matrixColSize[0];
    int row = matrixSize;
	//初始化返回数组并维护当前赋值位置；
	int x = 0, y = -1;//初始化操作位置；
	int turn = 0;
	while (col > 0 || row > 0)
	{
		int dx = dirx[(turn % 4)];
		int dy = diry[(turn % 4)];
        int coltmp = 0;
        int rowtmp = 0;
		while ((dy==0 && rowtmp++ <row) || (dx==0 && coltmp++ < col)) {
			//先进行判断，保证每次循环完成都会停在最后操作的位置上，而不是走到最后操作位置的下一个位置；
			x += dx;
			y += dy;
			ra[ra_ind] = matrix[x][y];
			ra_ind++;
		}
        if(dx == 0) 
            row--;
        else col--;
		turn++;
	}
	return ra;
}


//输入：matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
//输出：[1,2,3,4,8,12,11,10,9,5,6,7]

int main(){
    int colnum = 3;
//    int** matrix = malloc(3*sizeof (int*));
//    int a1[] = {1,2,3};
//    int a2[] = {4,5,6};
//    int a3[] = {7,8,9};
//    matrix[0] = a1;
//    matrix[1] = a2;
//    matrix[2] = a3;
    int* matx[] = {(int[]){1,2,3},(int[]){4,5,6},(int[]){7,8,9}};
    int returnsize;
    int* ret;
    ret = spiralOrder(matx, 3, &colnum, &returnsize);
    for(int i=0;i<returnsize;i++){
        printf("%d \n",ret[i]);
    }
}
```



[85. 最大矩形](https://leetcode.cn/problems/maximal-rectangle/)

给定一个仅包含 `0` 和 `1` 、大小为 `rows x cols` 的二维二进制矩阵，找出只包含 `1` 的最大矩形，并返回其面积。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2020/09/14/maximal.jpg)

```
输入：matrix = [["1","0","1","0","0"],["1","0","1","1","1"],["1","1","1","1","1"],["1","0","0","1","0"]]
输出：6
解释：最大矩形如上图所示。
```

**示例 2：**

```
输入：matrix = []
输出：0
```

**示例 3：**

```
输入：matrix = [["0"]]
输出：0
```

**示例 4：**

```
输入：matrix = [["1"]]
输出：1
```

**示例 5：**

```
输入：matrix = [["0","0"]]
输出：0
```

 

**提示：**

- `rows == matrix.length`
- `cols == matrix[0].length`
- `1 <= row, cols <= 200`
- `matrix[i][j]` 为 `'0'` 或 `'1'`

### 解题思路

![image.png](https://pic.leetcode-cn.com/dd7d0b6e71bb5fdfad37502174cd09f2fb393dc0be3a8bdb6cfbbfb750ca052d-image.png)

### 代码

```c
typedef struct {
    void **data;
    int top;
    int size;
} Stack;

Stack *StackCreate(int stackSize)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }

    stack->data = (void **)malloc(sizeof(void **) * (stackSize + 1));
    memset(stack->data, 0, sizeof(void **) * (stackSize + 1));
    stack->top = -1;
    stack->size = stackSize;
    return stack;
}

void StackFree(Stack *obj)
{
    if (obj->data != NULL) {
        free(obj->data);
        obj->data = NULL;
    }
    free(obj);
    obj = NULL;
    return;
}

bool IsStackEmpty(Stack *obj)
{
    return (obj->top == -1);
}

bool IsStackFull(Stack *obj)
{
    return (obj->top ==  obj->size);
}

void StackPush(Stack *obj, void *data)  // 泛型接口，使用void *
{
    if (IsStackFull(obj) == true) {
        return;
    }
    int top = obj->top;
    obj->data[++top] = data;
    obj->top = top;
    return;
}

void StackPop(Stack *obj)
{
    if (IsStackEmpty(obj) == true) {
        return;
    }
    void *data = obj->data[obj->top];
    free(data);
    data = NULL;
    obj->top--;
    return;
}

void *StackTop(Stack *obj)
{
    if (IsStackEmpty(obj) == true) {
        return NULL;
    }
    return (obj->data[obj->top]);
}

void StackClear(Stack *obj)
{
    if (IsStackEmpty(obj) == true) {
        return;
    }

    for (int i = 0; i <= obj->top; i++) {
        void *data = obj->data[i];
        if (data != NULL) {
            free(data);
            data = NULL;
        }
    }
    obj->top = -1;
    return;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
int maximalRectangle(char **matrix, int matrixSize, int *matrixColSize)
{
    int **height = (int **)malloc(sizeof(int *) * matrixSize);
    for (int i = 0; i < matrixSize; i++) {
        height[i] = (int *)malloc(sizeof(int) * (matrixColSize[i] + 1));
        for (int j = 0; j < matrixColSize[i]; j++) {
            if (i == 0) {
                height[i][j] = (matrix[i][j] == '1') ? 1 : 0;
            } else {
                height[i][j] = (matrix[i][j] == '1') ? (height[i - 1][j] + 1) : 0;
            }
        }
        height[i][matrixColSize[i]] = 0;
    }

    Stack *monotoneStack = StackCreate(matrixSize * matrixSize);
    int ans = 0;
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j <= matrixColSize[i]; j++) {
            while ((IsStackEmpty(monotoneStack) != true) && 
                    (height[i][*(int *)monotoneStack->data[monotoneStack->top]] >= height[i][j])) {
                int h = height[i][*(int *)monotoneStack->data[monotoneStack->top]];
                StackPop(monotoneStack);
                int sidx = ((IsStackEmpty(monotoneStack) == true) ? -1 : *(int *)(monotoneStack->data[monotoneStack->top]));
                ans = MAX(ans, h * (j - sidx - 1));
            }
            int *node = (int *)malloc(sizeof(int));
            *node = j;
            StackPush(monotoneStack, node); 
        }
        StackClear(monotoneStack);
    }

    StackFree(monotoneStack);
    return ans;
}
```

[10. 正则表达式匹配](https://leetcode.cn/problems/regular-expression-matching/)

给你一个字符串 `s` 和一个字符规律 `p`，请你来实现一个支持 `'.'` 和 `'*'` 的正则表达式匹配。

- `'.'` 匹配任意单个字符
- `'*'` 匹配零个或多个前面的那一个元素

所谓匹配，是要涵盖 **整个** 字符串 `s`的，而不是部分字符串。

 

**示例 1：**

```
输入：s = "aa", p = "a"
输出：false
解释："a" 无法匹配 "aa" 整个字符串。
```

**示例 2:**

```
输入：s = "aa", p = "a*"
输出：true
解释：因为 '*' 代表可以匹配零个或多个前面的那一个元素, 在这里前面的元素就是 'a'。因此，字符串 "aa" 可被视为 'a' 重复了一次。
```

**示例 3：**

```
输入：s = "ab", p = ".*"
输出：true
解释：".*" 表示可匹配零个或多个（'*'）任意字符（'.'）。
```

 

**提示：**

- `1 <= s.length <= 20`
- `1 <= p.length <= 20`
- `s` 只包含从 `a-z` 的小写字母。
- `p` 只包含从 `a-z` 的小写字母，以及字符 `.` 和 `*`。
- 保证每次出现字符 `*` 时，前面都匹配到有效的字符

解题思路
1、如果p为空，s为空匹配，s非空不匹配； 2、s非空，p == s || p == '.'时第一个字符匹配； 3、(p+1) != ''，则递归判断剩下的是否匹配 first_match && isMatch(++s, ++p) 4、(p+1) == '*'，则有两种情况匹配： a: *匹配0个字符，s匹配剩下的，即isMatch(s, p+2) b: *匹配1个字符，继续用p匹配剩下的s，即first_match && isMatch(s+1, p)

代码

```c
bool isMatch(char * s, char * p){
    if (!*p) return !*s;
    bool first_match = *s && (*s == *p || *p == '.');
    if (*(p+1) == '*') {
        return isMatch(s, p+2) || (first_match && isMatch(++s, p));
    }
    else {
        return first_match && isMatch(++s, ++p);
    }
}
```



递归法代码：

```c
bool isMatch(char * s, char * p){
	int s_pass,p_pass;
	while(1){
		if((*s||*p)==0)
			return true;
		else if(*p==0)
			return false;
		p_pass=s_pass=1;
		while(*(p+p_pass)=='*')
			p_pass++;
		if(p_pass==1){
			if(*p=='.'){
				if(*s==0)
					return false;
			}else{
				if(*p!=*s)
					return false;
			}
		}else{
			int i,max;
			if(*p=='.'){
				i=max=0;
				max=strlen(s);
				while(*(p+p_pass+i)){
					if(*(p+p_pass+i)=='*'){
						max++;
						if(*(p+p_pass+i-1)!='*')
							max++;
					}
					i++;
					max--;
				}	
			}else{
				max=0;
				while(*(s+max)==*p)
					max++;
			}
			for(i=0;i<=max;i++){
				if(isMatch(s+i,p+p_pass))
					return true;
			}
			return false;
		}
		s+=s_pass;
		p+=p_pass;
	}
}

```

动态规划法代码：

```c
bool isMatch(char * s, char * p){
    if(!(*s||*p)) return true;
    else if(!*p) return false;
    else{
        int i,j,slen,plen;
        bool* pb;
        slen=strlen(s);
        plen=strlen(p);
        pb=calloc(slen+1,sizeof(bool));
        pb[slen]=true;
        i=plen-1;
        while(i>=0){
            if(p[i]=='*'){
                i--;
                if(i<0||p[i]=='*') return false;
                else{
                    for(j=slen-1;j>=0;j--){
                        if((s[j]==p[i])||(p[i]=='.')) pb[j]=pb[j+1]||pb[j];
                    }
                    i--;
                }
            }else{
                for(j=0;j<slen;j++){
                    if((s[j]==p[i])||(p[i]=='.')) pb[j]=pb[j+1];
                    else pb[j]=false;
                }
                pb[slen]=false;
                i--;
            }
        }
        return pb[0];
    }
}

```

我是利用形式语言与自动机理论中的epsilon-NFA(简称NFA)做的。

先确定状态个数，假设正则式的长度为length(= strlen(p))，其中假设有k个星，那么， 我们的状态总数j就应该是length - k + 1, 最后一个状态是接受状态。

这其中的状态只分为两种类型，一种为确定的状态转化，另一种为有epsilon转化， 区分的标准就是看他后面有没有星，图示如下(设正则式ab*c)————

此外，为了节省内存，我以一个S1(int)来记录那个当前的状态集合，比方说当前状态0和 状态2被点亮，那么S1就为101(binary)，然后我觉得区分两类状态也可以用一个int来优化，但是最初并没有意识到，所以我用的是char的二维数组。 然后就可以运行NFA了，初学，如有疏漏，还望海涵。

```c
#define max 30
#define q0 0
#define no 0
#define yes 1
typedef char delta[2];
typedef delta Q[max];
typedef int S;
bool isMatch(char * s, char * p){
    Q state;
    char length = strlen(p), i, j;

    for(i = 0, j = 0; i < length - 1; ++i, ++j){
        if(p[i + 1] != '*'){
            state[j][0] = no;
            state[j][1] = p[i];
        }
        else{
            state[j][0] = yes;
            state[j][1] = p[i++];
        }
    }
    if(i == length - 1){
        state[j][0] = no;
        state[j++][1] = p[i];//j 是状态总数, 同时也是接受状态
    }
    S S1 = 1, S2 = 0;
    int temp = 0;
    while(temp < j && state[temp][0] == yes){
        ++temp;
        S1 |= 0x1 << temp;
    }
    length = strlen(s);
    for(i = 0; i < length; ++i){
        while(S1){
            char rank = j;
            while((S1 & (0x1 << rank)) == 0){
                --rank;
            }
            S1 -= 0x1 << rank;
            if(rank == j) continue;
            if(state[rank][1] == '.' || state[rank][1] == s[i]){
                char copy = state[rank][0] == no ? rank + 1 : rank;
                while(copy < j && state[copy][0] == yes){
                    S2 |= 0x1 << copy;
                    ++copy;
                }
                S2 |= 0x1 << copy;
            }
        }
        S1 = S2;
        S2 = 0;
    }
    return S1 & (0x1 << j);    
}
```

[498. 对角线遍历](https://leetcode.cn/problems/diagonal-traverse/)

给你一个大小为 `m x n` 的矩阵 `mat` ，请以对角线遍历的顺序，用一个数组返回这个矩阵中的所有元素。

**示例 1：**

![img](https://assets.leetcode.com/uploads/2021/04/10/diag1-grid.jpg)

```
输入：mat = [[1,2,3],[4,5,6],[7,8,9]]
输出：[1,2,4,7,5,3,6,8,9]
```

**示例 2：**

```
输入：mat = [[1,2],[3,4]]
输出：[1,2,3,4]
```

 

**提示：**

- `m == mat.length`
- `n == mat[i].length`
- `1 <= m, n <= 104`
- `1 <= m * n <= 104`
- `-105 <= mat[i][j] <= 105`

当横纵下标之和为偶数时，遍历方向为左下到右上，为奇数时相反。再对四种情况进行合并，不需要写四个分支,以横纵下标之和为奇数时为例。

```c
if(row==matrixSize-1) column++;
else{
    row++;
    if(column) column--;
}
```

另外需要注意当输入为空时，以下语句将出现执行错误。

```c
*returnSize=matrixSize*(*matrixColSize);
```

需要写成

```c
if(matrixSize==0){
    *returnSize=0;
    return 0;
}
```

完整代码如下。

```c
int* findDiagonalOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize){
    if(matrixSize==0){
        *returnSize=0;
        return 0;
    }
    int row=0,column=0,k=0;
    *returnSize=matrixSize*(*matrixColSize);
    int* res=malloc(*returnSize * sizeof(int));
    while(k<*returnSize){
        res[k]=matrix[row][column];
        if((row+column)%2){
            if(row==matrixSize-1) column++;
            else{
                row++;
                if(column) column--;
            }
        }
        else{
            if(column==*matrixColSize-1) row++;
            else{
                column++;
                if(row) row--;
            }
        }
        k++;
    }
    return res;
}

```

