#### **[二叉树的前序遍历](https://www.nowcoder.com/practice/5e2135f4d2b14eb8a5b06fab4c938635?tpId=295&tqId=2291302&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj)**

给你二叉树的根节点 root ，返回它节点值的 前序 遍历。

示例 1：

![img](https://uploadfiles.nowcoder.com/images/20211111/392807_1636599059575/FE67E09E9BA5661A7AB9DF9638FB1FAC)

输入：

```
{1,#,2,3}
```

返回值：

```
[1,2,3]
```

```c
void preOrderTraversal(struct TreeNode* node, int* ret, int* returnSize) {
    if(!node) return;
    ret[(*returnSize)++] = node->val;
    preOrderTraversal(node->left, ret, returnSize);
    preOrderTraversal(node->right, ret, returnSize);
}

int* preorderTraversal(struct TreeNode* root, int* returnSize){
    int* ret = (int*)malloc(sizeof(int) * 100);
    *returnSize = 0;
    preOrderTraversal(root, ret, returnSize);
    return ret;
}

```

```c
void inOrder(struct TreeNode* node, int* ret, int* returnSize) {
    if(!node) return;
    inOrder(node->left, ret, returnSize);
    ret[(*returnSize)++] = node->val;
    inOrder(node->right, ret, returnSize);
}

int* inorderTraversal(struct TreeNode* root, int* returnSize){
    int* ret = (int*)malloc(sizeof(int) * 100);
    *returnSize = 0;
    inOrder(root, ret, returnSize);
    return ret;
}

```

```c
void postOrder(struct TreeNode* node, int* ret, int* returnSize) {
    if(!node) return;
    postOrder(node->left, ret, returnSize);
    postOrder(node->right, ret, returnSize);
    ret[(*returnSize)++] = node->val;
}

int* postorderTraversal(struct TreeNode* root, int* returnSize){
    int* ret= (int*)malloc(sizeof(int) * 100);
    *returnSize = 0;
    postOrder(root, ret, returnSize);
    return ret;
}

```



#### [二叉树的层序遍历](https://leetcode.cn/problems/binary-tree-level-order-traversal/)

给你二叉树的根节点 `root` ，返回其节点值的 **层序遍历** 。 （即逐层地，从左到右访问所有节点）。

**示例 1：**

![](/home/ljxnb/lib/leetcode/tree1.jpg)

输入：root = [3,9,20,null,null,15,7]
输出：[[3],[9,20],[15,7]]
示例 2：

输入：root = [1]
输出：[[1]]
示例 3：

输入：root = []
输出：[]

迭代实现
广度优先遍历是按层层推进的方式，遍历每一层的节点。题目要求的是返回每一层的节点值，所以这题用广度优先来做非常合适。
广度优先需要用队列作为辅助结构，我们先将根节点放到队列中，然后不断遍历队列。

![](/home/ljxnb/lib/leetcode/f3c4f288f91ef62095c5fe6c9132e5efaf774d78ab5a508c4c262a79390a4a3c-二叉树层次遍历-1.jpg)

首先拿出根节点，如果左子树/右子树不为空，就将他们放入队列中。第一遍处理完后，根节点已经从队列中拿走了，而根节点的两个孩子已放入队列中了，现在队列中就有两个节点 2 和 5。

![](/home/ljxnb/lib/leetcode/4c26563a26b356ec727a90fd52dd5fea8b0fd5d638b3632383c1c0b376297b4d-二叉树层次遍历-2.jpg)

第二次处理，会将 `2` 和 `5` 这两个节点从队列中拿走，然后再将 2 和 5 的子节点放入队列中，现在队列中就有三个节点 `3`，`4`，`6`。

![](/home/ljxnb/lib/leetcode/ffb14166d055e682bab11a985456b6e3281c4089bae8282a5f6f1e9c8c81d8b8-二叉树层次遍历-3.jpg)

我们把每层遍历到的节点都放入到一个结果集中，最后返回这个结果集就可以了。
时间复杂度： O(n)*O*(*n*)
空间复杂度：O(n)*O*(*n*)
代码如下:

```python
class Solution(object):
	def levelOrder(self, root):
		"""
		:type root: TreeNode
		:rtype: List[List[int]]
		"""
		if not root:
			return []
		res = []
		queue = [root]
		while queue:
			# 获取当前队列的长度，这个长度相当于 当前这一层的节点个数
			size = len(queue)
			tmp = []
			# 将队列中的元素都拿出来(也就是获取这一层的节点)，放到临时list中
			# 如果节点的左/右子树不为空，也放入队列中
			for _ in xrange(size):
				r = queue.pop(0)
				tmp.append(r.val)
				if r.left:
					queue.append(r.left)
				if r.right:
					queue.append(r.right)
			# 将临时list加入最终返回结果中
			res.append(tmp)
		return res
```

#### 递归实现

用广度优先处理是很直观的，可以想象成是一把刀横着切割了每一层，但是深度优先遍历就不那么直观了。

![](/home/ljxnb/lib/leetcode/1df38946dbf6129193e35a5d1ade36e3c91fc68c702c37def6e7ee15d973388d-4.jpg)

我们开下脑洞，把这个二叉树的样子调整一下，摆成一个田字形的样子。田字形的每一层就对应一个 list。

![](/home/ljxnb/lib/leetcode/367726d56045ab65cd9bf34af1f4b98408dfa02669c0d2bb88b4aeb53143cf1f-5.jpg)

按照深度优先的处理顺序，会先访问节点 1，再访问节点 2，接着是节点 3。
之后是第二列的 4 和 5，最后是第三列的 6。

每次递归的时候都需要带一个 index(表示当前的层数)，也就对应那个田字格子中的第几行，如果当前行对应的 list 不存在，就加入一个空 list 进去。

动态演示如下：

![](/home/ljxnb/lib/leetcode/aeed09e12573ec00d83663bb4f77562e8904ac58cdb2cbe6e995f2ac33b12934-0203_1.gif)

时间复杂度：O(N)*O*(*N*)
空间复杂度：O(h)*O*(*h*)，`h` 是树的高度

代码如下:

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution(object):
	def levelOrder(self, root):
		"""
		:type root: TreeNode
		:rtype: List[List[int]]
		"""
		if not root:
			return []
		res = []
		def dfs(index,r):
			# 假设res是[ [1],[2,3] ]， index是3，就再插入一个空list放到res中
			if len(res)<index:
				res.append([])
			#  将当前节点的值加入到res中，index代表当前层，假设index是3，节点值是99
			# res是[ [1],[2,3] [4] ]，加入后res就变为 [ [1],[2,3] [4,99] ]
			res[index-1].append(r.val)
			# 递归的处理左子树，右子树，同时将层数index+1
			if r.left:
				dfs(index+1,r.left)
			if r.right:
				dfs(index+1,r.right)
		dfs(1,root)
		return res
```

```
输入
[3,9,20,null,null,15,7]
输出
[[3],[9,20],[15,7]]
```

