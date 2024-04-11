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

dp:

```c
int longestValidParentheses(char* s) {
    int maxans = 0, n = strlen(s);
    if (n == 0) return 0;
    int dp[n];
    memset(dp, 0, sizeof(dp));
    for (int i = 1; i < n; i++) {
        if (s[i] == ')') {
            if (s[i - 1] == '(') {
                dp[i] = (i >= 2 ? dp[i - 2] : 0) + 2;
            } else if (i - dp[i - 1] > 0 && s[i - dp[i - 1] - 1] == '(') {
                dp[i] = dp[i - 1] + ((i - dp[i - 1]) >= 2 ? dp[i - dp[i - 1] - 2] : 0) + 2;
            }
            maxans = fmax(maxans, dp[i]);
        }
    }
    return maxans;
}

```

stack:

```c
int longestValidParentheses(char* s) {
    int maxans = 0, n = strlen(s);
    int stk[n + 1], top = -1;
    stk[++top] = -1;
    for (int i = 0; i < n; i++) {
        if (s[i] == '(') {
            stk[++top] = i;
        } else {
            --top;
            if (top == -1) {
                stk[++top] = i;
            } else {
                maxans = fmax(maxans, i - stk[top]);
            }
        }
    }
    return maxans;
}
```

栈：

具体做法是我们始终保持栈底元素为当前已经遍历过的元素中「最后一个没有被匹配的右括号的下标」，这样的做法主要是考虑了边界条件的处理，栈里其他元素维护左括号的下标：
对于遇到的每个‘(’ ，我们将它的下标放入栈中
对于遇到的每个 ‘)’ ，我们先弹出栈顶元素表示匹配了当前右括号：
如果栈为空，说明当前的右括号为没有被匹配的右括号，我们将其下标放入栈中来更新我们之前提到的「最后一个没有被匹配的右括号的下标」
如果栈不为空，当前右括号的下标减去栈顶元素即为「以该右括号为结尾的最长有效括号的长度」
我们从前往后遍历字符串并更新答案即可。

需要注意的是，如果一开始栈为空，第一个字符为左括号的时候我们会将其放入栈中，这样就不满足提及的「最后一个没有被匹配的右括号的下标」，为了保持统一，我们在一开始的时候往栈中放入一个值为 −1 的元素。

```c
//栈
#define MAX(a , b) ((a) > (b) ? (a) : (b))
int longestValidParentheses(char * s){
    int len = strlen(s);
    int max = 0;
    int str[len+1];//定义栈
    int top = -1;
    str[++top] = -1;//定义标准
    for(int i = 0; i < len; i++)//遍历字符串
    {
        if(s[i] == '(')//入栈
        {
            str[++top] = i;
        }
        if(s[i] == ')')//出栈
        {
            --top;
            if(top == -1)//重新定义标准
            {
                str[++top] = i;
            }
            else
            {
                max = MAX(max , (i - str[top]));
            }
        }
    }
    return max;
}
```

[42. 接雨水](https://leetcode.cn/problems/trapping-rain-water/)

给定 `n` 个非负整数表示每个宽度为 `1` 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

 

**示例 1：**

![img](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/10/22/rainwatertrap.png)

```
输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出：6
解释：上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 
```

**示例 2：**

```
输入：height = [4,2,0,3,2,5]
输出：9
```

**提示：**

- `n == height.length`
- `1 <= n <= 2 * 104`
- `0 <= height[i] <= 105`



对于下标 i，下雨后水能到达的最大高度等于下标 i 两边的最大高度的最小值，下标 iii 处能接的雨水量等于下标 i 处的水能到达的最大高度减去 height。

朴素的做法是对于数组 height 中的每个元素，分别向左和向右扫描并记录左边和右边的最大高度，然后计算每个下标位置能接的雨水量。假设数组 height 的长度为 n，该做法需要对每个下标位置使用 O(n) 的时间向两边扫描并得到最大高度，因此总时间复杂度是 O(n^2)。

上述做法的时间复杂度较高是因为需要对每个下标位置都向两边扫描。如果已经知道每个位置两边的最大高度，则可以在O(n)的时间内得到能接的雨水总量。使用动态规划的方法，可以在 O(n) 的时间内预处理得到每个位置两边的最大高度。

创建两个长度为 n 的数组 leftMax 和 rightMax。对于 0 <= i<n，leftMax[i]表示下标 i 及其左边的位置中，height 的最大高度，rightMax[i] 表示下标 i及其右边的位置中，height的最大高度。

显然，leftMax[0]=height[0]，rightMax[n−1]=height[n−1]。两个数组的其余元素的计算如下：

* 当 $$1 \le i \le n-1 时， leftMax[i]=max⁡(leftMax[i−1],height[i])$$；

* 当 $$0 \le i \le n-2 时，rightMax[i]=max⁡(rightMax[i+1],height[i])$$.

因此可以正向遍历数组 height得到数组 leftMax 的每个元素值，反向遍历数组 height得到数组 rightMax的每个元素值。

在得到数组 leftMax和 rightMax 的每个元素值之后，对于$$ 0 \le i<n$$，下标 i处能接的雨水量等于 min⁡(leftMax[i],rightMax[i])−height[i] 。遍历每个下标位置即可得到能接的雨水总量。

动态规划做法可以由下图体现。

![fig1](https://assets.leetcode-cn.com/solution-static/42/1.png)

```c
int trap(int* height, int heightSize) {
    int n = heightSize;
    if (n == 0) 
        return 0;
    int leftMax[n];
    memset(leftMax, 0, sizeof(leftMax));
    leftMax[0] = height[0];
    for (int i = 1; i < n; ++i) 
        leftMax[i] = fmax(leftMax[i - 1], height[i]);

    int rightMax[n];
    memset(rightMax, 0, sizeof(rightMax));
    rightMax[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; --i) 
        rightMax[i] = fmax(rightMax[i + 1], height[i]);

    int ans = 0;
    for (int i = 0; i < n; ++i) 
        ans += fmin(leftMax[i], rightMax[i]) - height[i];
    
    return ans;
}
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

```c
// 回溯法求解
#define MAX_SIZE 1430  // 卡特兰数: 1, 1, 2, 5, 14, 42, 132, 429, 1430
void generate(int left, int right, int n, char *str, int index, char **result, int *returnSize) {
    if (index == 2 * n) { // 当前长度已达2n
        result[(*returnSize)] =  (char*)calloc((2 * n + 1), sizeof(char));
        strcpy(result[(*returnSize)++], str);
        return;
    }
    // 如果左括号数量不大于 n，可以放一个左括号
    if (left < n) {
        str[index] = '(';
        generate(left + 1, right, n, str, index + 1, result, returnSize);
    }
    // 如果右括号数量小于左括号的数量，可以放一个右括号
    if (right < left) {
        str[index] = ')';
        generate(left, right + 1, n, str, index + 1, result, returnSize);
    }
}
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char** generateParenthesis(int n, int *returnSize) {
    char *str = (char*)calloc((2 * n + 1), sizeof(char));
    char **result = (char **)malloc(sizeof(char *) * MAX_SIZE);
    *returnSize = 0;
    generate(0, 0, n, str, 0, result, returnSize);
    return result;
}
```



[「算法入门笔记」卡特兰数](https://zhuanlan.zhihu.com/p/97619085)

## **一、引言**

卡特兰数（Catalan number）是 **组合数学** 中一个常出现在各种 **计数问题** 中的 **数列**。

数列的前几项为：1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862，...

本文将会选取几个经典的卡特兰问题，难度先易后难，带领读者逐个击破解决，最后给出相关的解题模板。

## 二、**经典问题**

### **2.1 进出栈序列**

这是一道 **最经典** 的入门级卡特兰数题目，如果能把这题看懂，相信后面的题目也能迎刃而解。

**题目描述**

n 个元素进栈序列为：1，2，3，4，...，n，则有多少种出栈序列。

**思路**

我们将进栈表示为 +1，出栈表示为 -1，则 1 3 2 的出栈序列可以表示为：+1 -1 +1 +1 -1 -1。

![img](https://pic2.zhimg.com/80/v2-fe28b25ed263230250d0a3c68344b0d5_1440w.webp)

根据栈本身的特点，每次出栈的时候，必定之前有元素入栈，即对于每个 -1 前面都有一个 +1 相对应。因此，出栈序列的 **所有前缀和** 必然大于等于 0，并且 +1 的数量 **等于** -1 的数量。

接下来让我们观察一下 n = 3 的一种出栈序列：+1 -1 -1 +1 -1 +1。序列前三项和小于 0，显然这是个非法的序列。

如果将 **第一个** 前缀和小于 0 的前缀，即前三项元素都进行取反，就会得到：-1 +1 +1 +1 -1 +1。此时有 3 + 1 个 +1 以及 3 - 1 个 -1。

因为这个小于 0 的前缀和必然是 -1，且 -1 比 +1 多一个，取反后，-1 比 +1 少一个，则 +1 变为 n + 1 个，且 -1 变为 n - 1 个。进一步推广，对于 n 元素的每种非法出栈序列，都会对应一个含有 n + 1 个 +1 以及 n - 1个 -1 的序列。

如何证明这两种序列是一一对应的？

假设非法序列为 A，对应的序列为 B。每个 A 只有一个"**第一个前缀和小于 0 的前缀**"，所以每个 A 只能产生一个 B。而每个 B 想要还原到 A，就需要找到"**第一个前缀和大于 0 的前缀**"，显然 B 也只能产生一个 A。

![img](https://pic3.zhimg.com/80/v2-1224b08274913efa2cd7dbb31f8e6262_1440w.webp)

每个 B 都有 n + 1 个 +1 以及 n - 1 个 -1，因此 B 的数量为 $C_{2n}^{n+1}$ ，相当于在长度为 2n 的序列中找到`n + 1`个位置存放 +1。相应的，非法序列的数量也就等于 $C_{2n}^{n+1}$。

出栈序列的总数量共有 $C_{2n}^{n}$，因此，合法的出栈序列的数量为  $C_{2n}^{n+1}-C_{2n}^{n}=\frac{C_{2n}^{n}}{n+1}$ 。

此时我们就得到了卡特兰数的通项  $C_{2n}^{n+1}-C_{2n}^{n}=\frac{C_{2n}^{n}}{n+1}$，至于具体如何计算结果将会在后面进行介绍。

### **2.2 括号序列**

**题目描述**

n 对括号，则有多少种 “括号匹配” 的括号序列

![img](https://pic3.zhimg.com/80/v2-e5785ad4be18724da3059efd87307706_1440w.webp)

**思路**

左括号看成 +1，右括号看成 -1，那么就和上题的进出栈一样，共有 $\frac{C_{2n}^{n}}{n+1}$ 种序列。

### **2.3 二叉树**

**题目描述**

`n + 1` 个叶子节点能够构成多少种形状不同的（国际）满二叉树

（国际）满二叉树定义：如果一棵二叉树的结点要么是叶子结点，要么它有两个子结点，这样的树就是满二叉树。

![img](https://pic2.zhimg.com/80/v2-e1fcde1b4cf9b5d3dbac91fbe90d5065_1440w.webp)

**思路**

使用深度优先搜索这个满二叉树，向左扩展时标记为 +1，向右扩展时标记为 -1。

由于每个非叶子节点都有两个左右子节点，所有它必然会先向左扩展，再向右扩展。总体下来，左右扩展将会形成匹配，即变成进出栈的题型。`n + 1`个叶子结点会有 2n 次扩展，构成$\frac{C_{2n}^{n}}{n+1}$ 种形状不同的满二叉树。

![img](https://pic1.zhimg.com/80/v2-b21b64ee36af600e1c9d989f79306a6c_1440w.webp)

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
    int left=0,right=heightSize-1,max=0;
    while(left<right){
        max = fmax( max, (right - left) * fmin(height[left], height[right]) );
        height[left]<height[right] ? left++ : right--;
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

```c
bool _exist(char **board, int row, int col, char *word, int y, int x)
{
    if (*word == '\0')
        return true;
    if (y < 0 || y >= row || x < 0 || x >= col || *word != board[y][x])
        return false;
    board[y][x] = '\0';
    bool result = _exist(board, row, col, word + 1, y, x - 1) ||
                  _exist(board, row, col, word + 1, y - 1, x) ||
                  _exist(board, row, col, word + 1, y, x + 1) ||
                  _exist(board, row, col, word + 1, y + 1, x);
    board[y][x] = *word;
    return result;
}
bool exist(char **board, int boardSize, int *boardColSize, char *word)
{
    for (int y = 0; y < boardSize; y++)
        for (int x = 0; x < boardColSize[0]; x++)
            if (board[y][x] == word[0] && _exist(board, boardSize, boardColSize[0], word, y, x))
                return true;
    return false;
}
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

解题思路
此处撰写解题思路
使用中心扩散法寻找回文数组，但是回文数组有两种可能，奇数个和偶数个。
设置两个指针left和right，分别位于对称中心的两边，向两边扩散，中心对称点是重复字符串则当成一个字符处理。

代码

```c
char * longestPalindrome(char * s){
    if(strlen(s)==0||strlen(s)==1) return s;
    int i,start,left,right,count,len;
    start = len =0;
    for(i=0;s[i]!='\0';i+=count){
        count = 1;
        left=i-1;
        right = i+1;
        while(s[right]!='\0'&&s[i]==s[right]){ //处理重复字符串
            right++;
            count++;
        }
        while(left>=0 && s[right]!='\0' && s[left] == s[right]){
            left--;
            right++;
        }
        if(right-left-1>len){
            start = left+1;
            len = right-left-1;
        }
    }
    s[start + len] = '\0';      // 原地修改返回
    return s + start;
}
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

```c
bool isValid(char* s) {
    char stk[10010];
    int tt = -1;//这里默认下标从-1开始（个人习惯）
    for(int i = 0;i < strlen(s);i++){
        if(s[i] == '(' || s[i] == '[' || s[i] == '{') stk[++tt] = s[i];
        else{
            if(s[i] == ')'){
                if(tt == -1 || stk[tt] != '(') return false;
                else if(stk[tt] == '(') tt--;
            }else if(s[i] == ']'){
                if(tt == -1 || stk[tt] != '[') return false;
                else if(stk[tt] == '[') tt--;
            }else if(s[i] == '}'){
                if(tt == -1 || stk[tt] != '{') return false;
                else if(stk[tt] == '{') tt--;
            }
        }
    }
    return tt == -1;
}
```



# [单词拆分](https://leetcode.cn/problems/word-break/):(huawei)

给你一个字符串 `s` 和一个字符串列表 `wordDict` 作为字典。如果可以利用字典中出现的一个或多个单词拼接出 `s` 则返回 `true`。

**注意：**不要求字典中出现的单词全部都使用，并且字典中的单词可以重复使用。

 

**示例 1：**

```
输入: s = "leetcode", wordDict = ["leet", "code"]
输出: true
解释: 返回 true 因为 "leetcode" 可以由 "leet" 和 "code" 拼接成。
```

**示例 2：**

```
输入: s = "applepenapple", wordDict = ["apple", "pen"]
输出: true
解释: 返回 true 因为 "applepenapple" 可以由 "apple" "pen" "apple" 拼接成。
     注意，你可以重复使用字典中的单词。
```

**示例 3：**

```
输入: s = "catsandog", wordDict = ["cats", "dog", "sand", "and", "cat"]
输出: false
```

解题思路
算法思想就是动态规划，dp[i]的含义为s的前i个字母组成的子串是否可以被拆分为字典中的单词。

大致思路：
以“leetcode”为例，字典为“leet”和“code”，显然dp[4]为true,我们要的是dp[8] （即dp[sLen]）
观察可知dp[8]可以由dp[4]和字典推导来，就是知道了dp[4]为true，从s+4开始，枚举看之后的单词是否能与
字典匹配。语言比较苍白，直接看代码吧（附注释）

代码

```c
bool wordBreak(char * s, char ** wordDict, int wordDictSize){
    int sLen = strlen(s);
    bool dp[sLen + 1];
    memset(dp, false, sizeof(dp));
    dp[0] = true;           /* 初始化，以便状态转移 */
    for (int i = 1; i <= sLen; i++) /* 从s的第一个字母开始枚举 */
    /* 枚举到第i个字母时，i之前的dp已经全部算出来了，我们再枚举字典中的单词，根据每个单词的
    长度决定分割点，假设分割点是k，当分割点后的子串能与字典单词匹配且dp[k]为true，则dp[i]为true
    */
        for (int j = 0; j < wordDictSize; j++) {  
            int len = strlen(wordDict[j]);
            int k = i - len;        /* 分割点是由i和字典单词长度决定的 */
            if (k < 0)
                continue;
            dp[i] = (dp[k] && !strncmp(s + k, wordDict[j], len)) || dp[i];
                                /* 这里注意要限制长度，故用strncmp */
                                /* 进一步追求速度可用哈希法等代替strncmp */
        }
    
  /* for (int i = 1; i <= sLen; i++)
        printf("%d ", dp[i]); */
    return dp[sLen];
}

```


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

该解法是比较暴力的遍历解法
首先是遍历整个二维数组（碰到'.'就跳过）把所有数都放入check（自己编写）中

在check中我们实现一次性对三个条件进行检查

首先是行：
由于我们传入了原数组的坐标
并且写了一个循环使得a从0到9不断增加
所以只要保持j（传入的纵坐标）不变不断增加横坐标就可以遍历整个行，（跳过自身：a和i相同时条件不能漏）

列同理

再者时33的范围（本题最妙的一个地方）
首先确定传入的坐标处于哪个范围
由i/3和j/3可以得出（一共有9个范围）
之后再分别给(i/3)和(j/3)乘3可以得到初始位置
之后随着a从0-9增加
如何使得在范围遍历呢
已知(0-9)%3会经历3个（0-3）的循环
而（0-9）/3会在（0-2）时等于0，（3-5）时等于1，（6-8）时等于2（很类似2进制）
已知这两个条件就可以进行遍历
设m=3(i/3)+a%3
设n=3*(j/3)+a/3
就可以进行遍历

之后再进行整体判断就可以遍历所有数组元素
详见注释

```c
bool check(char** board,int num,int i,int j){
    for(int a=0;a<9;a++){
        int m=3*(i/3)+a%3;
        int n=3*(j/3)+a/3;
        if((board[m][n]==num&&m!=i&&n!=j)||(board[i][a]==num&&a!=j)||(board[a][j]==num&&a!=i)){
            return false;
        }
    }
    return true;
}


bool isValidSudoku(char** board, int boardSize, int* boardColSize){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[i][j]=='.')
                continue;
        if(!check(board,board[i][j],i,j))
            return false;
        }
    }
    return true;
}
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

```py
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

```c
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char map[10][5]={"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
char *path;
char **ans;
int pathsize,anssize,n;
void backtrace(int idx,char *digits)
{
    if(idx==n) //注意c语言字符串处理
    {
        char *temp=(char*)malloc(sizeof(char)*(n+1));
        memcpy(temp,path,sizeof(char)*n);
        temp[n]=0;
        ans[anssize++]=temp;
        return;
    }
    char *words=map[digits[idx]-'0'];
    for(int i=0;i<strlen(words);i++) //选或不选当前字符
    {
        path[pathsize++]=words[i];
        backtrace(idx+1,digits);
        pathsize--;
    }
}
char ** letterCombinations(char * digits, int* returnSize){
    n=strlen(digits);
    path=(char*)malloc(sizeof(char)*n);
    ans=(char**)malloc(sizeof(char*)*300);
    anssize=pathsize=0;
    if(n==0)
    {
        *returnSize=0;
        return ans;
    }
    backtrace(0,digits);
    *returnSize=anssize;
    return ans;
}
```



复杂度分析

时间复杂度：O(3^m×4^n)，其中 mmm 是输入中对应 333 个字母的数字个数（包括数字 222、333、444、555、666、888），nnn 是输入中对应 444 个字母的数字个数（包括数字 777、999），m+nm+nm+n 是输入数字的总个数。当输入包含 mmm 个对应 333 个字母的数字和 nnn 个对应 444 个字母的数字时，不同的字母组合一共有 3m×4n3^m \times 4^n3 m×4 n种，需要遍历每一种字母组合。

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

[118. 杨辉三角](https://leetcode.cn/problems/pascals-triangle/)

给定一个非负整数 *`numRows`，*生成「杨辉三角」的前 *`numRows`* 行。

在「杨辉三角」中，每个数是它左上方和右上方的数的和。

![img](https://pic.leetcode-cn.com/1626927345-DZmfxB-PascalTriangleAnimated2.gif)

 

**示例 1:**

```
输入: numRows = 5
输出: [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]
```

**示例 2:**

```
输入: numRows = 1
输出: [[1]]
```

 

**提示:**

- `1 <= numRows <= 30`

```c
int** generate(int numRows, int* returnSize, int** returnColumnSizes) {
    int** ret = malloc(sizeof(int*) * numRows);
    *returnSize = numRows;
    *returnColumnSizes = malloc(sizeof(int) * numRows);
    for (int i = 0; i < numRows; ++i) {
        ret[i] = malloc(sizeof(int) * (i + 1));
        (*returnColumnSizes)[i] = i + 1;
        ret[i][0] = ret[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            ret[i][j] = ret[i - 1][j] + ret[i - 1][j - 1];
        }
    }
    return ret;
}
```

python3

```py
class Solution:
    def generate(self, numRows: int) -> List[List[int]]:
        if numRows == 0: return []
        res = [[1]]
        while len(res) < numRows:
            newRow = [a+b for a, b in zip([0]+res[-1], res[-1]+[0])]
            res.append(newRow)      
        return res
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

[44. 通配符匹配](https://leetcode.cn/problems/wildcard-matching/)

给你一个输入字符串 (`s`) 和一个字符模式 (`p`) ，请你实现一个支持 `'?'` 和 `'*'` 匹配规则的通配符匹配：

- `'?'` 可以匹配任何单个字符。
- `'*'` 可以匹配任意字符序列（包括空字符序列）。

判定匹配成功的充要条件是：字符模式必须能够 **完全匹配** 输入字符串（而不是部分匹配）。

 

**示例 1：**

```
输入：s = "aa", p = "a"
输出：false
解释："a" 无法匹配 "aa" 整个字符串。
```

**示例 2：**

```
输入：s = "aa", p = "*"
输出：true
解释：'*' 可以匹配任意字符串。
```

**示例 3：**

```
输入：s = "cb", p = "?a"
输出：false
解释：'?' 可以匹配 'c', 但第二个 'a' 无法匹配 'b'。
```

 

**提示：**

- `0 <= s.length, p.length <= 2000`
- `s` 仅由小写英文字母组成
- `p` 仅由小写英文字母、`'?'` 或 `'*'` 组成

我发在了我的CSDN博客，https://blog.csdn.net/Xixo0628/article/details/105874722

读完题目以后第一个反应就是动态规划，主要有这么几种情况要讨论：

if(p[i]==s[j]) ,dp\[i][j] =dp\[i-1][j-1]
if(p[i] == '?'),dp\[i][k] = dp\[i-1][k-1]
if(p[i]=='*'),dp\[i][j] = dp\[i-1][j-1] || dp\[i][j-1] || dp\[i-1][j];
初始边界处理，特别要注意当p是以若干个连续的‘*’开头的情况
初步代码如下：

```c
bool isMatch(char * s, char * p){
    int len_s = strlen(s), len_p = strlen(p);
    
    int** dp = (int**)malloc(sizeof(int*)*(len_s+1));
    for (int i = 0; i <= len_s; ++i){
        *(dp+i) = (int*)malloc(sizeof(int)*(len_p+1));
        memset(*(dp+i), 0, sizeof(int)*(len_p+1));
    }
    
    dp[0][0] = 1;
    for (int i = 1; i <= len_p; ++i){ 
        // 考虑边界条件,p前面若干位为*就在对应位置赋值1，当p出现非*字符时跳出
        if (p[i-1] == '*')
            dp[0][i] = 1;
        else
            break;
    }
    
    for (int i = 1; i <= len_s; i++){
        for (int j = 1; j <= len_p; j++){
            if (s[i-1] == p[j-1])
                dp[i][j] = dp[i-1][j-1];
            else{
                if (p[j-1] == '?')
                    dp[i][j] = dp[i-1][j-1];
                else if (p[j-1] == '*')
                    dp[i][j] = dp[i-1][j-1] || dp[i][j-1] || dp[i-1][j];
            }
        }
    }
    
    return dp[len_s][len_p];
}
```
运行结果：


效率相当一般。肯定还存在优化空间。

首先是动态规划空间的申请，因为其实变量里面只存储true和false，因此完全不需要用int，改用bool就可以从4字节降到3字节。

其次如果使用calloc来申请空间，就不需要调用memset函数了（这个优化效果应该微乎其微）

最后我尝试了一下先动p的下标，再动s的下标，发现多了一种可以快速判断的情况：当p结尾是*的情况下，可以直接把该位所有对应下标置true，否则直接跳出循环（不去改变数值就是保持为false）。
感觉说的可能还没有代码清楚。这种情况的代码块如下

```c
if(p[p_index] == '*') {
    for (s_index = 0; s_index < s_len + 1; s_index++) {
        if(dp[p_index][s_index]) {
            for (; s_index < s_len + 1; s_index++) {
                dp[p_index + 1][s_index] = true;
            }
            break;
        }
    }
}
```

优化动态规划代码

```c
bool isMatch(char * s, char * p){
    int s_len = strlen(s);
    int p_len = strlen(p);
    if (p_len <= 1){
        if (s_len == 0 && p_len == 0) return true;  //两者均空
        else if (p[0] == '*') return true;   //p为单字符‘*’
        else if (s_len != 1) return false;  //s长度大于1
        else if (p[0] == '?' || s[0] == p[0]) return true;  //都为1位且匹配
    }
    // s做行，p做列
    bool **dp = (bool **) calloc(p_len + 1, sizeof(bool *));
    int i = 0;
    for (i = 0; i < p_len + 1; i++) {
        dp[i] = (bool *) calloc(s_len + 1, sizeof(bool));
    }
    dp[0][0] = true;
    int s_index = 0, p_index = 0;
    for (p_index = 0; p_index < p_len; p_index++) {
        if(p[p_index] == '*') {
            for (s_index = 0; s_index < s_len + 1; s_index++) {
                if(dp[p_index][s_index]) {
                    for (; s_index < s_len + 1; s_index++) {
                        dp[p_index + 1][s_index] = true;
                    }
                    break;
                }
            }
        } else if (p[p_index] == '?') {
            for (s_index = 0; s_index < s_len; s_index++) {
                dp[p_index + 1][s_index + 1] = dp[p_index][s_index];
            } 
        } else {
            for (s_index = 0; s_index < s_len; s_index++) {
            //把s[s_index]==p[p_index]的if判断改成用&&连接
                dp[p_index + 1][s_index + 1] = dp[p_index][s_index] && (s[s_index] == p[p_index]);
            }
        }
    }
    return dp[p_len][s_len];
}
```


优化后运行结果：


不得不说，用时和内存消耗都明明减半了，一个排名翻倍了，一个排名一个点都没有动，真是太奇怪了。

空间还是申请了太多，还有没有什么好办法？动态规划空间开销几乎是固定的那么多，还能优化吗？
我注意到其实生成p_index的那一整行的下标时，我其实只用到了p_index - 1 那一行，而且最后也只用返回最后一行的值，不需要返回匹配的方式意味着我不需要回溯，可以把用过的数据覆盖掉！也就是说，我完全可以只申请两行空间然后交替使用——就像桐人老爷和爱丽丝两个人爬塔一样，用两根棍子，一根垫脚、一根用于向上攀登，当用于攀登的棍子沦为垫脚后就把原来垫脚的那根回收，并再次将它作位向上攀登用的新棍子——虽然我只有两行空间，但是合理利用可以达到够用的目的！交替使用的办法就是把行号改成对2取余数。好吧,我承认之前那个桐人老爷的例子不够生动,换种说法吧,比如要过一条河,只有两块木板,怎么过?之前正常方法就是木板一路铺过去就能过河,现在我每走到一块木板上,就得把后面的木板抽出来放在前面,以此类推,没错就是这个意思,如果还没解释清楚的话就请看代码吧......

我修改了代码，尝试了一下发现会产生一个新的bug：那就是我之前为了用break跳出了循环（因为后面位默认是0），现在因为有一个覆盖重写的问题导致后面那几位可能不是0，产生结果将一些false输出为true。所以我需要每个循环重写每一位。（这个开销应该不算大）

两块木板过河_究极省空间的动态规划

```c
bool isMatch(char * s, char * p){
    int s_len = strlen(s);
    int p_len = strlen(p);
    if (p_len <= 1){
        if (s_len == 0 && p_len == 0) return true;  //两者均空
        else if (p[0] == '*') return true;   //p为单字符‘*’
        else if (s_len != 1) return false;  //s长度大于1
        else if (p[0] == '?' || s[0] == p[0]) return true;  //都为1位且匹配
    }
    // s做行，p做列
    bool **dp = (bool **) calloc(2 , sizeof(bool *));
    int i = 0;
    for (i = 0; i < 2 ;i++) {
        dp[i] = (bool *) calloc(s_len + 1, sizeof(bool));
    }
    dp[0][0] = true;
    int s_index = 0, p_index = 0;
    for (p_index = 0; p_index < p_len; p_index++) {
    	if (p_index >= 1) dp[(p_index+1)%2][0] = dp[(p_index)%2][0] && (p[p_index] == '*');
        // putchar('\n');
        if(p[p_index] == '*') {
            for (s_index = 0; s_index < s_len + 1; s_index++) {
               if(dp[p_index%2][s_index]) {
                    for (; s_index < s_len + 1; s_index++) {
                        dp[(p_index + 1)%2][s_index] = true;
                        // printf("%d,",dp[(p_index + 1)%2][s_index]);
                    }
                    break;
                }else {
                    dp[(p_index + 1)%2][s_index] = false;
                    // printf("%d,",dp[(p_index + 1)%2][s_index]);
                }
            }
        } else if (p[p_index] == '?') {
            for (s_index = 0; s_index < s_len; s_index++) {
                dp[(p_index + 1)%2][s_index + 1] = dp[p_index%2][s_index];
                // printf("%d,",dp[(p_index + 1)%2][s_index + 1]); 
            } 
        } else {
            for (s_index = 0; s_index < s_len; s_index++) {
            //把s[s_index]==p[p_index]的if判断改成用&&连接
                dp[(p_index + 1)%2][s_index + 1] = dp[p_index%2][s_index] && (s[s_index] == p[p_index]);
                // printf("%d,",dp[(p_index + 1)%2][s_index + 1]);
            }
        }
    }
    return dp[p_len%2][s_len];
}
```

没错,就是这样!运行结果的空间开销令我满意:
看到这个结果,我真是感动得泪流满面,因为我调试了大半天.....请看图:

可能看起来我改的代码不多,但是化身为bug斗士两小时的我眼泪流下来……这一切只为了实现一个两块木板过河的动态规划.说一下最关键的一步吧:

if (p_index >= 1) dp\[(p_index+1)%2][0] = dp\[(p_index)%2][0] && (p[p_index] == '*');
就是这行命令,因为之前如果不强行省空间的话只需要dp\[0][0]=1,就可以了,其他行默认就有dp[*][0] = 1,用两块木板的话,dp\[2k+1][0] 默认就是1了……就是为了找&&改好这个bug，我费了九牛二虎之力。当然还有其他bug，这个是我认为最难找也最难改的一个。
看到这里的小伙伴，应该就算是出于同情也该给我点个赞吧……

好了，以上就是动态规划方法了，我认为我已经优化到了极致，那有没有其他办法解决这个问题呢？在拟写解答的过程中，我就注意到了一个方法：深度优先搜索。
需要回溯的地方只有通配符出现的时候，用链个变量记录连续若干个 * 的最后一个的时候两个变量各自的位置，当一次深度优先搜索失败的时候就回溯到这个变量指示的位置。又是到了我最喜欢的名言：

talk is cheap,show me the code

深度优先搜索&&回溯

```c
bool isMatch(char * s, char * p){
    int j = 0;
    int start = 0;
    int last = 0;
    for(int i = 0; i < strlen(s);) {
        if(j < strlen(p) && (s[i] == p[j] || p[j] == '?')){
            i++;
            j++;
        } else if(j < strlen(p) && p[j] == '*') {
            last = i;
            start = ++j;
        } else if(start != 0){
            i = ++last;
            j = start;
        } else {
            return false;
        }
    }
    for(; j<strlen(p)&& p[j]=='*'; ++j);
    return j==strlen(p);
}
```

哇，这长度，这空间，不用跑就知道，结果一定很棒！

放弃动态规划以后的运行结果


妈妈咪啊，结果比我想象得还要好……深刻诠释了什么叫做动态规划从入门到优化到debug到放弃。

朋友，都看到这里了，难道不愿意给在这道题目上死磕了半天的我点一个小小的赞嘛~

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

[最小覆盖子串](https://leetcode.cn/problems/minimum-window-substring/)

给你一个字符串 `s` 、一个字符串 `t` 。返回 `s` 中涵盖 `t` 所有字符的最小子串。如果 `s` 中不存在涵盖 `t` 所有字符的子串，则返回空字符串 `""` 。

 

**注意：**

- 对于 `t` 中重复字符，我们寻找的子字符串中该字符数量必须不少于 `t` 中该字符数量。
- 如果 `s` 中存在这样的子串，我们保证它是唯一的答案。

 

**示例 1：**

```
输入：s = "ADOBECODEBANC", t = "ABC"
输出："BANC"
解释：最小覆盖子串 "BANC" 包含来自字符串 t 的 'A'、'B' 和 'C'。
```

**示例 2：**

```
输入：s = "a", t = "a"
输出："a"
解释：整个字符串 s 是最小覆盖子串。
```

**示例 3:**

```
输入: s = "a", t = "aa"
输出: ""
解释: t 中两个字符 'a' 均应包含在 s 的子串中，
因此没有符合条件的子字符串，返回空字符串。
```

 

**提示：**

- `m == s.length`
- `n == t.length`
- `1 <= m, n <= 105`
- `s` 和 `t` 由英文字母组成

思路
这题要用滑动窗口来解决。

// 滑动窗口模板
for () {
    // 将新进来的右边的数据，计算进来
    // 更新数据

    // 判断窗口数据是否不满足要求了
    while (窗口数据不满要求 && left < arrSize) {
        // 移除left数据，更新窗口数据
        left++;    
    }
    right++;
}
用 map[] 当作哈希表，统计字符串 t 中各元素的哈希值，用 tLen 统计还未找到的字符串 t 中字母的个数。

右指针 right 从字符串 s 头移动到字符串 s 尾: s 中每个字母的对应的哈希表 -1，若字符串 s 有字符串 t 的字母（即哈希值被减一之前 >0），则 tlen-- ；

如果出现匹配的子串（即 tlen == 0 ，此时所有 t 中字母对应的哈希值都 ==0，非 t 字母对应的哈希值 <0）：更新最小匹配子串的起点 start 和长度 result ，然后 left++ 。不过在 left++ 之前要先更新窗口数据，即先将left的字母的哈希值 +1，如果加一后其值 >0，则这个字母在 t 中存在，所以要 tlen++ 。

if (minlen != INT32_MAX) ，找到了最小匹配字串；若未找到最小匹配子串，返回""。

代码

```c
char* minWindow(char* s, char* t) {
    int map[256] = { 0 };   //用 map[] 统计字符串 t 中各字母的个数
    int subLength = 0, result = INT32_MAX;
    int start = 0;
    int sLen = strlen(s), tLen = strlen(t); //用 tLen 统计还未找到的字符串 t 中字母的个数
    for (int i = 0; i < tLen; i++) {    //统计字符串 t 中各字母的个数
        map[t[i]]++;
    }
    for (int left = 0, right = 0; right < sLen; right++) {
        if (map[s[right]]-- > 0) {  //s 中每个字母的对应的哈希表 -1
            tLen--; //若是对应字符串 t 的字母，则更新tLen的值
        }
        while (tLen == 0) {     //出现匹配的子串
            subLength = right - left + 1;
            if (subLength < result) {   //更新最小匹配子串的起点 start 和长度 result
                start = left;
                result = subLength;
            }
            if (++map[s[left]] > 0) {   //更新窗口数据：map[] 和 tLen
                tLen++;
            }
            left++;     //窗口左端left右移一位
        }
    }
    if (result != INT32_MAX) {  //找到了最小匹配字串
        char* res = (char*)malloc(sizeof(char) * (result + 1));
        *res = '\0';
        strncat(res, s + start, result);
        return res;
    }
    return "";  //没有找到最小匹配字串
}
```

[49. 字母异位词分组](https://leetcode.cn/problems/group-anagrams/)

给你一个字符串数组，请你将 **字母异位词** 组合在一起。可以按任意顺序返回结果列表。

**字母异位词** 是由重新排列源单词的所有字母得到的一个新单词。

 

**示例 1:**

```
输入: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
输出: [["bat"],["nat","tan"],["ate","eat","tea"]]
```

**示例 2:**

```
输入: strs = [""]
输出: [[""]]
```

**示例 3:**

```
输入: strs = ["a"]
输出: [["a"]]
```

 

**提示：**

- `1 <= strs.length <= 104`
- `0 <= strs[i].length <= 100`
- `strs[i]` 仅包含小写字母

[3. 无重复字符的最长子串](https://leetcode.cn/problems/longest-substring-without-repeating-characters/)

给定一个字符串 `s` ，请你找出其中不含有重复字符的 **最长子串** 的长度。

**示例 1:**

```
输入: s = "abcabcbb"
输出: 3 
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
```

**示例 2:**

```
输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
```

**示例 3:**

```
输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
```

 

**提示：**

- `0 <= s.length <= 5 * 104`
- `s` 由英文字母、数字、符号和空格组成

```c
int lengthOfLongestSubstring(char * s){
    int sLen = strlen(s);
    int left=0, right = 0;
    int res=0, cnt=0;
    int tmp[128] = {0};
    
    while(right < sLen) {
        if(0 == tmp[ s[right] ]) {
            tmp[ s[right++] ]=1;
            res = res > ++cnt ? res : cnt;
        }
        else {
            tmp[ s[left++] ] = 0;
            cnt--;
        }
    }
    return res;
}
```

[1004. 最大连续1的个数 III](https://leetcode.cn/problems/max-consecutive-ones-iii/)

给定一个二进制数组 `nums` 和一个整数 `k`，如果可以翻转最多 `k` 个 `0` ，则返回 *数组中连续 `1` 的最大个数* 。

 

**示例 1：**

```
输入：nums = [1,1,1,0,0,0,1,1,1,1,0], K = 2
输出：6
解释：[1,1,1,0,0,1,1,1,1,1,1]
粗体数字从 0 翻转到 1，最长的子数组长度为 6。
```

**示例 2：**

```
输入：nums = [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], K = 3
输出：10
解释：[0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1]
粗体数字从 0 翻转到 1，最长的子数组长度为 10。
```

 

**提示：**

- `1 <= nums.length <= 105`
- `nums[i]` 不是 `0` 就是 `1`
- `0 <= k <= nums.length`

```c
int longestOnes(int* A, int ASize, int K){
    int r = 0, l = 0,sum = 0,len = 0;
    while (r < ASize) {
        if (sum < K+1) 
            sum += (!A[r++]);
        else 
            sum -= (!A[l++]);
        len = (r - l > len & sum < K+1) ? r-l :len;
    }
    return len;
}
```

[1423. 可获得的最大点数](https://leetcode.cn/problems/maximum-points-you-can-obtain-from-cards/)

几张卡牌 **排成一行**，每张卡牌都有一个对应的点数。点数由整数数组 `cardPoints` 给出。

每次行动，你可以从行的开头或者末尾拿一张卡牌，最终你必须正好拿 `k` 张卡牌。

你的点数就是你拿到手中的所有卡牌的点数之和。

给你一个整数数组 `cardPoints` 和整数 `k`，请你返回可以获得的最大点数。

 

**示例 1：**

```
输入：cardPoints = [1,2,3,4,5,6,1], k = 3
输出：12
解释：第一次行动，不管拿哪张牌，你的点数总是 1 。但是，先拿最右边的卡牌将会最大化你的可获得点数。最优策略是拿右边的三张牌，最终点数为 1 + 6 + 5 = 12 。
```

**示例 2：**

```
输入：cardPoints = [2,2,2], k = 2
输出：4
解释：无论你拿起哪两张卡牌，可获得的点数总是 4 。
```

**示例 3：**

```
输入：cardPoints = [9,7,7,9,7,7,9], k = 7
输出：55
解释：你必须拿起所有卡牌，可以获得的点数为所有卡牌的点数之和。
```

**示例 4：**

```
输入：cardPoints = [1,1000,1], k = 1
输出：1
解释：你无法拿到中间那张卡牌，所以可以获得的最大点数为 1 。 
```

**示例 5：**

```
输入：cardPoints = [1,79,80,1,1,1,200,1], k = 3
输出：202
```

 

**提示：**

- `1 <= cardPoints.length <= 10^5`
- `1 <= cardPoints[i] <= 10^4`
- `1 <= k <= cardPoints.length`

```c
/* 滑动窗口法：一、left移动到k的位置，right为0
二、每次left减小一位，right增加一位。时间复杂度为O（K） */
int maxScore(int* cardPoints, int cardPointsSize, int k){
    int left;
    int right = 0;
    int i, j;
    int sum = 0;
    int max;
    for (i = 0; i < k; i++) {
        sum += cardPoints[i];
    }
    max = sum;
    if(k == cardPointsSize) return max;
    left = k - 1;
    while ((left >= 0) && (left + right + 1 == k)) {
        sum -= cardPoints[left];
        left--;
        right++;
        sum += cardPoints[cardPointsSize - right];
        max = max > sum ? max : sum;
    }
    return max;
}
```

[1438. 绝对差不超过限制的最长连续子数组](https://leetcode.cn/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/)

给你一个整数数组 `nums` ，和一个表示限制的整数 `limit`，请你返回最长连续子数组的长度，该子数组中的任意两个元素之间的绝对差必须小于或者等于 `limit` *。*

如果不存在满足条件的子数组，则返回 `0` 。

 

**示例 1：**

```
输入：nums = [8,2,4,7], limit = 4
输出：2 
解释：所有子数组如下：
[8] 最大绝对差 |8-8| = 0 <= 4.
[8,2] 最大绝对差 |8-2| = 6 > 4. 
[8,2,4] 最大绝对差 |8-2| = 6 > 4.
[8,2,4,7] 最大绝对差 |8-2| = 6 > 4.
[2] 最大绝对差 |2-2| = 0 <= 4.
[2,4] 最大绝对差 |2-4| = 2 <= 4.
[2,4,7] 最大绝对差 |2-7| = 5 > 4.
[4] 最大绝对差 |4-4| = 0 <= 4.
[4,7] 最大绝对差 |4-7| = 3 <= 4.
[7] 最大绝对差 |7-7| = 0 <= 4. 
因此，满足题意的最长子数组的长度为 2 。
```

**示例 2：**

```
输入：nums = [10,1,2,4,7,2], limit = 5
输出：4 
解释：满足题意的最长子数组是 [2,4,7,2]，其最大绝对差 |2-7| = 5 <= 5 。
```

**示例 3：**

```
输入：nums = [4,2,2,2,4,4,2,2], limit = 0
输出：3
```

 

**提示：**

- `1 <= nums.length <= 10^5`
- `1 <= nums[i] <= 10^9`
- `0 <= limit <= 10^9`

判定子数组是否满足条件任何两个元素的差的绝对值小于等于limit，可以转化为子数组的最大元素与最小元素的差小于等于limit。

使用两个队列max和min，需要保证子数组变化时，max的头部元素永远是当前子数组的最大值，min的头部元素永远是当前子数组的最小值。

容易想到，令max为单调非递增序列，当子数组的末尾新增元素x时，移除max尾部所有小于x的元素，然后将x加入max尾部；当子数组移除头部元素x时，若x与max的头部元素相等，则弹出max的头部元素。这两个操作保证子数组变化时，max的头部元素永远是当前子数组的最大值。针对min的处理与max相似，只是保证min为单调非递减序列即可。

设置两个指针pstart、pend分别代表子数组的开始和结束位置。通过移动这两个指针以及max、min两个队列，我们总能知道当前子数组是否满足条件。若不满足条件，则令pstart加一，若满足条件，则令pend加一。通过子数组的指针变化以及过程中条件的满足与否，可以得到满足条件的子数组最大长度。

```c
int longestSubarray(int* nums, int numsSize, int limit){
    int res = 0;
    int min[numsSize], max[numsSize], lo_min = 0, hi_min = 0, lo_max = 0, hi_max = 0;
    for (int i=0, j=0; j<numsSize; ) {
        if (hi_max == lo_max || max[lo_max] - min[lo_min] <= limit) {
            int x = nums[j++];
            while (hi_min > lo_min && min[hi_min - 1] > x) --hi_min;
            min[hi_min++] = x;
            while (hi_max > lo_max && max[hi_max - 1] < x) --hi_max;
            max[hi_max++] = x;
        } else {
            int x = nums[i++];
            if (min[lo_min] == x) ++lo_min;
            if (max[lo_max] == x) ++lo_max;
        }
        if (max[lo_max] - min[lo_min] <= limit && res < j - i) {
            res = j - i;
        }
    }
    return res;
}
```

[**HJ71** **字符串通配符**](https://www.nowcoder.com/practice/43072d50a6eb44d2a6c816a283b02036?tpId=37&tqId=21294&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=)

## 描述

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

数据范围：字符串长度： 1≤*s*≤100 

进阶：时间复杂度： *O*(*n*2) ，空间复杂度：O*(*n*) 

### 输入描述：

先输入一个带有通配符的字符串，再输入一个需要匹配的字符串

### 输出描述：

返回不区分大小写的匹配结果，匹配成功输出true，匹配失败输出false

## 示例1

输入：

```
te?t*.*
txt12.xls
```

复制

输出：

```
false
```

复制

## 示例2

输入：

```
z
zz
```

输出：

```
false
```

## 示例3

输入：

```
pq
pppq
```

输出：

```
false
```

## 示例4

输入：

```
**Z
0QZz
```

输出：

```
true
```

## 示例5

输入：

```
?*Bc*?
abcd
```

输出：

```
true
```

## 示例6

输入：

```
h*?*a
h#a
```

输出：

```
false
```

说明：

```
根据题目描述可知能被*和?匹配的字符仅由英文字母和数字0到9组成，所以?不能匹配#，故输出false      
```

## 示例7

输入：

```
p*p*qp**pq*p**p***ppq
pppppppqppqqppqppppqqqppqppqpqqqppqpqpppqpppqpqqqpqqp
```

输出：

```
false
```

```c
#include<stdio.h>
#include<string.h>
#include<ctype.h>

int match(char *str1, char *str2)
{
    if(str1[0]=='\0' && str2[0]=='\0')
        return 1;
    else if(str1[0]=='\0' || str2[0]=='\0')
        return 0;
    if(str1[0]=='?' || str1[0]==str2[0])
        return match(str1+1, str2+1);
    if(str1[0]=='*')
    {
        if(*(str1+1)=='*')
            return (match(str1+2, str2) || match(str1+2, str2+1) || match(str1+1,str2+1));
        else
            return (match(str1+1, str2) || match(str1+1, str2+1) || match(str1, str2+1));
    }
    return 0;
}

int main()
{
    char str1[101], str2[101];
    scanf("%s %s", str1, str2);
    int flag = 1;
    for(int i=0; i<strlen(str1); i++)
    {
        if(str1[i]>='A' && str1[i]<='Z')
            str1[i] += 32;
    }
    for(int i=0; i<strlen(str2); i++)
    {
        if(str2[i]>='A' && str2[i]<='Z')
            str2[i] += 32;
        if(isalpha(str2[i])==0 && isdigit(str2[i])==0 && str2[i]!='.')
            flag = 0;
    }
    if(flag && match(str1, str2))
        printf("true\n");
    else
        printf("false\n");
    return 0;
}

```

[74. 搜索二维矩阵](https://leetcode.cn/problems/search-a-2d-matrix/)

给你一个满足下述两条属性的 `m x n` 整数矩阵：

- 每行中的整数从左到右按非严格递增顺序排列。
- 每行的第一个整数大于前一行的最后一个整数。

给你一个整数 `target` ，如果 `target` 在矩阵中，返回 `true` ；否则，返回 `false` 。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2020/10/05/mat.jpg)

```
输入：matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3
输出：true
```

**示例 2：**

![img](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/11/25/mat2.jpg)

```
输入：matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 13
输出：false
```

 

**提示：**

- `m == matrix.length`
- `n == matrix[i].length`
- `1 <= m, n <= 100`
- `-104 <= matrix[i][j], target <= 104`

```c
bool searchMatrix(int** matrix, int matrixSize, int* matrixColSize, int target) {
    int m = matrixSize, n = matrixColSize[0];
    int low = 0, high = m * n - 1;
    while (low <= high) {
        int mid = (high - low) / 2 + low;
        int x = matrix[mid / n][mid % n];
        if (x < target) {
            low = mid + 1;
        } else if (x > target) {
            high = mid - 1;
        } else {
            return true;
        }
    }
    return false;
}
```

[55. 跳跃游戏](https://leetcode.cn/problems/jump-game/)

给你一个非负整数数组 `nums` ，你最初位于数组的 **第一个下标** 。数组中的每个元素代表你在该位置可以跳跃的最大长度。

判断你是否能够到达最后一个下标，如果可以，返回 `true` ；否则，返回 `false` 。

 

**示例 1：**

```
输入：nums = [2,3,1,1,4]
输出：true
解释：可以先跳 1 步，从下标 0 到达下标 1, 然后再从下标 1 跳 3 步到达最后一个下标。
```

**示例 2：**

```
输入：nums = [3,2,1,0,4]
输出：false
解释：无论怎样，总会到达下标为 3 的位置。但该下标的最大跳跃长度是 0 ， 所以永远不可能到达最后一个下标。
```

 

**提示：**

- `1 <= nums.length <= 104`
- `0 <= nums[i] <= 105`

```c
bool canJump(int* nums, int numsSize) {
    for (int i = 0, cur = 0; i < numsSize; i++) {
        if (i <= cur) {
            cur = fmax(cur, i + nums[i]);
            if (cur >= numsSize - 1) {
                return true;
            }
        } else {
            break;
        }
    }
    return false;
}
```

[45. 跳跃游戏 II](https://leetcode.cn/problems/jump-game-ii/)

给定一个长度为 `n` 的 **0 索引**整数数组 `nums`。初始位置为 `nums[0]`。

每个元素 `nums[i]` 表示从索引 `i` 向前跳转的最大长度。换句话说，如果你在 `nums[i]` 处，你可以跳转到任意 `nums[i + j]` 处:

- `0 <= j <= nums[i]` 
- `i + j < n`

返回到达 `nums[n - 1]` 的最小跳跃次数。生成的测试用例可以到达 `nums[n - 1]`。

 

**示例 1:**

```
输入: nums = [2,3,1,1,4]
输出: 2
解释: 跳到最后一个位置的最小跳跃数是 2。
     从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。
```

**示例 2:**

```
输入: nums = [2,3,0,1,4]
输出: 2
```

```c
int jump(int* nums, int numsSize) {
    int steps = 0;
    int maxReach = 0;
    int lastJump = 0;

    for (int i = 0; i < numsSize - 1; ++i) {
        maxReach = (i + nums[i] > maxReach) ? i + nums[i] : maxReach;

        if (i == lastJump) {
            lastJump = maxReach;
            ++steps;

            if (maxReach >= numsSize - 1) {
                return steps;
            }
        }
    }

    return 0;
}

```





[337. 打家劫舍 III](https://leetcode.cn/problems/house-robber-iii/)

小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为 `root` 。

除了 `root` 之外，每栋房子有且只有一个“父“房子与之相连。一番侦察之后，聪明的小偷意识到“这个地方的所有房屋的排列类似于一棵二叉树”。 如果 **两个直接相连的房子在同一天晚上被打劫** ，房屋将自动报警。

给定二叉树的 `root` 。返回 ***在不触动警报的情况下** ，小偷能够盗取的最高金额* 。

 

**示例 1:**

![img](https://assets.leetcode.com/uploads/2021/03/10/rob1-tree.jpg)

```
输入: root = [3,2,3,null,3,null,1]
输出: 7 
解释: 小偷一晚能够盗取的最高金额 3 + 3 + 1 = 7
```

**示例 2:**

![img](https://assets.leetcode.com/uploads/2021/03/10/rob2-tree.jpg)

```
输入: root = [3,4,5,1,3,null,1]
输出: 9
解释: 小偷一晚能够盗取的最高金额 4 + 5 = 9
```

 

**提示：**



- 树的节点数在 `[1, 104]` 范围内
- `0 <= Node.val <= 104`

```c
struct SubtreeStatus {
    int selected;
    int notSelected;
};

struct SubtreeStatus dfs(struct TreeNode *node) {
    if (!node) {
        return (struct SubtreeStatus){0, 0};
    }
    struct SubtreeStatus l = dfs(node->left);
    struct SubtreeStatus r = dfs(node->right);
    int selected = node->val + l.notSelected + r.notSelected;
    int notSelected = fmax(l.selected, l.notSelected) + fmax(r.selected, r.notSelected);
    return (struct SubtreeStatus){selected, notSelected};
}

int rob(struct TreeNode *root) {
    struct SubtreeStatus rootStatus = dfs(root);
    return fmax(rootStatus.selected, rootStatus.notSelected);
}
```

[104. 二叉树的最大深度](https://leetcode.cn/problems/maximum-depth-of-binary-tree/)

二叉树的 **最大深度** 是指从根节点到最远叶子节点的最长路径上的节点数。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2020/11/26/tmp-tree.jpg)

```
输入：root = [3,9,20,null,null,15,7]
输出：3
```

**示例 2：**

```
输入：root = [1,null,2]
输出：2
```

 

**提示：**

- 树中节点的数量在 `[0, 104]` 区间内。
- `-100 <= Node.val <= 100`

```c
int maxDepth(struct TreeNode* root) {
    if(root) 
    {
        int depth;
        depth = fmax(maxDepth(root->left), maxDepth(root->right)) + 1;
        return depth;
    }
    return 0;
}
```

[111. 二叉树的最小深度](https://leetcode.cn/problems/minimum-depth-of-binary-tree/)

给定一个二叉树，找出其最小深度。

最小深度是从根节点到最近叶子节点的最短路径上的节点数量。

**说明：**叶子节点是指没有子节点的节点。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2020/10/12/ex_depth.jpg)

```
输入：root = [3,9,20,null,null,15,7]
输出：2
```

**示例 2：**

```
输入：root = [2,null,3,null,4,null,5,null,6]
输出：5
```

```c
int minDepth(struct TreeNode *root) {
    if (root == NULL) {
        return 0;
    }

    if (root->left == NULL && root->right == NULL) {
        return 1;
    }

    int min_depth = INT_MAX;
    if (root->left != NULL) {
        min_depth = fmin(minDepth(root->left), min_depth);
    }
    if (root->right != NULL) {
        min_depth = fmin(minDepth(root->right), min_depth);
    }

    return min_depth + 1;
}

```



[114. 二叉树展开为链表](https://leetcode.cn/problems/flatten-binary-tree-to-linked-list/)

给你二叉树的根结点 `root` ，请你将它展开为一个单链表：

- 展开后的单链表应该同样使用 `TreeNode` ，其中 `right` 子指针指向链表中下一个结点，而左子指针始终为 `null` 。
- 展开后的单链表应该与二叉树 [**先序遍历**](https://baike.baidu.com/item/先序遍历/6442839?fr=aladdin) 顺序相同。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2021/01/14/flaten.jpg)

```
输入：root = [1,2,5,3,4,null,6]
输出：[1,null,2,null,3,null,4,null,5,null,6]
```

**示例 2：**

```
输入：root = []
输出：[]
```

**示例 3：**

```
输入：root = [0]
输出：[0]
```

```c
void flatten(struct TreeNode* root) {
    struct TreeNode* curr = root;
    while (curr != NULL) {
        if (curr->left != NULL) {
            struct TreeNode* next = curr->left;
            struct TreeNode* predecessor = next;
            while (predecessor->right != NULL) {
                predecessor = predecessor->right;
            }
            predecessor->right = curr->right;
            curr->left = NULL;
            curr->right = next;
        }
        curr = curr->right;
    }
}

```

[236. 二叉树的最近公共祖先](https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/)

给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。

[百度百科](https://baike.baidu.com/item/最近公共祖先/8918834?fr=aladdin)中最近公共祖先的定义为：“对于有根树 T 的两个节点 p、q，最近公共祖先表示为一个节点 x，满足 x 是 p、q 的祖先且 x 的深度尽可能大（**一个节点也可以是它自己的祖先**）。”

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2018/12/14/binarytree.png)

```
输入：root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
输出：3
解释：节点 5 和节点 1 的最近公共祖先是节点 3 。
```

**示例 2：**

![img](https://assets.leetcode.com/uploads/2018/12/14/binarytree.png)

```
输入：root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4
输出：5
解释：节点 5 和节点 4 的最近公共祖先是节点 5 。因为根据定义最近公共祖先节点可以为节点本身。
```

**示例 3：**

```
输入：root = [1,2], p = 1, q = 2
输出：1
```

```c
typedef struct TreeNode TreeNode;

struct TreeNode* lowestCommonAncestor(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q) {
    // 如果一个节点的左右节点同时存在p和q，则该节点为最近的公共祖先
    if(!root || root == p || root == q) return root;

    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);

    if(left && right) return root;

    return !left ? right : left;
}

```

[112. 路径总和](https://leetcode.cn/problems/path-sum/)

给你二叉树的根节点 `root` 和一个表示目标和的整数 `targetSum` 。判断该树中是否存在 **根节点到叶子节点** 的路径，这条路径上所有节点值相加等于目标和 `targetSum` 。如果存在，返回 `true` ；否则，返回 `false` 。

**叶子节点** 是指没有子节点的节点。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2021/01/18/pathsum1.jpg)

```
输入：root = [5,4,8,11,null,13,4,7,2,null,null,null,1], targetSum = 22
输出：true
解释：等于目标和的根节点到叶节点路径如上图所示。
```

**示例 2：**

![img](https://assets.leetcode.com/uploads/2021/01/18/pathsum2.jpg)

```
输入：root = [1,2,3], targetSum = 5
输出：false
解释：树中存在两条根节点到叶子节点的路径：
(1 --> 2): 和为 3
(1 --> 3): 和为 4
不存在 sum = 5 的根节点到叶子节点的路径。
```

**示例 3：**

```
输入：root = [], targetSum = 0
输出：false
解释：由于树是空的，所以不存在根节点到叶子节点的路径。
```

 

**提示：**

- 树中节点的数目在范围 `[0, 5000]` 内
- `-1000 <= Node.val <= 1000`
- `-1000 <= targetSum <= 1000`

```c
bool hasPathSum(struct TreeNode* root, int targetSum){
    if(root == NULL){
        return false;
    }
    if(root -> left == NULL && root -> right == NULL){
        return targetSum == root -> val;
    }
    return hasPathSum(root -> left,targetSum - root -> val) || hasPathSum(root -> right,targetSum - root -> val);
}

```

[113. 路径总和 II](https://leetcode.cn/problems/path-sum-ii/)

给你二叉树的根节点 `root` 和一个整数目标和 `targetSum` ，找出所有 **从根节点到叶子节点** 路径总和等于给定目标和的路径。

**叶子节点** 是指没有子节点的节点。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2021/01/18/pathsumii1.jpg)

```
输入：root = [5,4,8,11,null,13,4,7,2,null,null,5,1], targetSum = 22
输出：[[5,4,11,2],[5,8,4,5]]
```

**示例 2：**

![img](https://assets.leetcode.com/uploads/2021/01/18/pathsum2.jpg)

```
输入：root = [1,2,3], targetSum = 5
输出：[]
```

[124. 二叉树中的最大路径和](https://leetcode.cn/problems/binary-tree-maximum-path-sum/)

二叉树中的 **路径** 被定义为一条节点序列，序列中每对相邻节点之间都存在一条边。同一个节点在一条路径序列中 **至多出现一次** 。该路径 **至少包含一个** 节点，且不一定经过根节点。

**路径和** 是路径中各节点值的总和。

给你一个二叉树的根节点 `root` ，返回其 **最大路径和** 。

 

**示例 1：**

![img](https://assets.leetcode.com/uploads/2020/10/13/exx1.jpg)

```
输入：root = [1,2,3]
输出：6
解释：最优路径是 2 -> 1 -> 3 ，路径和为 2 + 1 + 3 = 6
```

**示例 2：**

![img](https://assets.leetcode.com/uploads/2020/10/13/exx2.jpg)

```
输入：root = [-10,9,20,null,null,15,7]
输出：42
解释：最优路径是 15 -> 20 -> 7 ，路径和为 15 + 20 + 7 = 42
```

```c
int max;
int calculate(struct TreeNode* root){
    if(root == NULL){
        return 0;
    }
    int left_num = fmax(0,calculate(root -> left));
    int right_num = fmax(0,calculate(root -> right));
    max = fmax(max,left_num + right_num + root -> val);
    return root -> val + fmax(left_num,right_num);
}
int maxPathSum(struct TreeNode* root) {
    max = INT_MIN;//初始化max
    calculate(root);
    return max;
}
```

[面试题 04.12. 求和路径](https://leetcode.cn/problems/paths-with-sum-lcci/)

给定一棵二叉树，其中每个节点都含有一个整数数值(该值或正或负)。设计一个算法，打印节点数值总和等于某个给定值的所有路径的数量。注意，路径不一定非得从二叉树的根节点或叶节点开始或结束，但是其方向必须向下(只能从父节点指向子节点方向)。

**示例:**
给定如下二叉树，以及目标和 `sum = 22`，

```
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \    / \
        7    2  5   1
```

返回:

```
3
解释：和为 22 的路径有：[5,4,11,2], [5,8,4,5], [4,11,7]
```

提示：

- `节点总数 <= 10000`

```c
struct TreeNode* inorderSuccessor(struct TreeNode* root, struct TreeNode* p){
    if (root == NULL) return NULL;

    if (root->val <= p->val) {
        return inorderSuccessor(root->right, p);
    } else {
        struct TreeNode* q = inorderSuccessor(root->left, p);
        return  ((q == NULL) ? root : q);
    }
}
```

