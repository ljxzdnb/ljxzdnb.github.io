# [平衡二叉树 C语言代码实现](https://mp.weixin.qq.com/s/E7-_MdJxZu76xtVgR7rlWA)

## 1. 什么是平衡二叉树

平衡二叉树，我们也称【二叉平衡搜索树/AVL】,树中任何节点的两个子树的高度最大差别为1，巴拉巴拉。。。(*https://baike.baidu.com/item/AVL树/10986648?fr=aladdin*)

但是有个注意的点: 平衡二叉树的前提是 二叉排序树(*https://baike.baidu.com/item/二叉搜索树/7077855?fr=aladdin*)

这篇博客主要总结平衡二叉树，所以，二叉排序树知识不会提及，但是会用到。

如果想要看 排序二叉树调整为 平衡二叉树 旋转相关内容的，调整至 第5节。

**平衡二叉树**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1VbQdg7vUwLjCUdCBB5qRPvr7T1PbO6MLFuZ68Twevp9K2AhS8l9cng/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**非平衡二叉树**

最小不平衡子树节点为 130

左子树深度为 1，右子树深度为3 ，其差值大于1，所以不平衡

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia18Q18sgFspkWqGqIVnxmDy3k7o9qqDyicnbvShTKnrHz0cQ69Z1HiaBXw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## 2. 如何判断二叉树最小不平衡子树

最小不平衡子树为 130 这颗子树(黄色标注)

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1y0Bv1nB7pNJerX8vhiaDtVumhv47zKibWH10iciciaWljPkUibtAleYibDFeg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

判定最小不平衡子树的关键就在于，判断 **这棵树的左子树 和 右字数 深度之差是否大于1，若大于1 ，则证明该树不平衡**

检查二叉树是否平衡函数代码实现

```c
typedef struct {
        int data; // 数据节点
        struct TreeNode *left; // 指向左子树
        struct TreeNode *right; // 指向右子树
} TreeNode , *PTreeNode;

// 记录平衡二叉树
bool BalanceTrue = false;
// 最小不平衡子树地址
TreeNode *rjt = NULL;

// 检查二叉树是否平衡，若不平衡 BalanceTrue 为 true
int checkTreeBalance(TreeNode *root) {
        if (NULL == root) { return 0; }
        int x = checkTreeBalance(root->left);
        int y = checkTreeBalance(root->right);

        // 若检测到最小不平衡二叉树后，不进行后面的检查
        if (BalanceTrue) return 0;
    
        int xx = abs(x-y);

        if (xx > 1) {
                // 左子树 和 右子树 相差大于1 ， 二叉树不平衡
                BalanceTrue = true;
                rjt = root;
        }
         
        return (x>y?x+1:y+1);
}
```

程序执行结果

```bash
# gcc -w -g -std=c11 BalanceTree.c 
# 
# ./a.out 
当前二叉树遍历
前序遍历: 580    130     80      160     150     158     210     1590    900     2100    1900
中序遍历: 80     130     150     158     160     210     580     900     1590    1900    2100
二叉树不平衡，不平衡子树根节点为: 130
# 
```

## 3. 二叉树不平衡情况

在一颗平衡二叉树的前提下，插入和删除一个节点，都有可能会引起二叉树不平衡，不平衡的情况主要有以下四种

**左左更高**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1HgicRSSGw8cTbQtT6j67keVfCibKibbiaMSLzXLV13mS2wLuz9s9oBUOBA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**左右更高**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1zXhDqiaicL9fkrW9kpAXxLDQ6iauGdtiaOkHUuNZGLofQNWDugfkooSpog/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**右左更高**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1xlAzfJX2uHSr412Joj9GCAPZwG9WdXJALIqxG702dRQoaVcibsyTR9Q/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**右右更高**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1uibQgYXJ1rAhnP87iatKmUQaib1C8lTBiaZ31PghbXSwMagNdibWl4VK1Rg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## 4. 判断不平衡二叉树哪边高

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1zgEy7RuQKr9UibGuT3TquibKvM7KpWkA3icRmDwvhm1KhDESgWunMZNnA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1qAMA063icvicVWYPS50cgbUjaUpL5hfRA1Dfczf5qBkCYYVhEYWwcZ7w/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

如上图红色所示，可以先根据最小不平衡二叉树**左子树**或者**右子树**高，上图所示，为右子树高，则将最小不平衡二叉树的右子树作为树根节点，继续判断子树的**左子树**或者**右子树**高。
比如上图的结果是**右左较高**，若进行调整的话，为 **先让不平衡子树右节点的树先向右旋转，然后再向左旋转**。

**判断不平衡二叉树哪边高代码实现**

```c
typedef struct {
        int data; // 数据节点
        struct TreeNode *left; // 指向左子树
        struct TreeNode *right; // 指向右子树
} TreeNode , *PTreeNode;

// 记录平衡二叉树
bool BalanceTrue = false;
// 最小不平衡子树地址
TreeNode *rjt = NULL;

// 返回二叉树树高
int treeHeight(TreeNode *root) {
        if (NULL == root) return 0;
        int ll = treeHeight(root->left);
        int rr = treeHeight(root->right);
        return (ll>rr?ll+1:rr+1);
}

int main() {
    /*  构建二叉树
    	判断平衡，获取最小不平衡子树， 将数据存入 rjt 中
    	输出二叉树 前序/中序
    */
    if (BalanceTrue) {
        printf("二叉树不平衡，不平衡子树根节点为: %d\n",rjt->data);
    } else {
        return 0;
    };


    int ll = treeHeight(rjt->left);
    int rr = treeHeight(rjt->right);
    if (1 < ll - rr) {
        printf("左子树高\n");
        TreeNode *rjt_ll = rjt->left;

        int child_ll = treeHeight(rjt_ll->left);
        int child_rr = treeHeight(rjt_ll->right);
        if (child_ll > child_rr) {
            printf("左子树更高\n");
        } else if (child_rr > child_ll) {
            printf("右字数更高");
        }

    } else if (1 <  rr - ll) {
        printf("右子数高\n");
        TreeNode *rjt_rr = rjt->right;

        int child_ll = treeHeight(rjt_rr->left);
        int child_rr = treeHeight(rjt_rr->right);
        if (child_ll > child_rr) {
            printf("左子树更高\n");
        } else if (child_rr > child_ll) {
            printf("右字数更高");
        }

    }

    return 0;
}
```

**输出**

```bash
# gcc BalanceTree.c -w -g -std=c11
# 
# ./a.out 
当前二叉树遍历
前序遍历:130    80      160     150     158     210
中序遍历:80     130     150     158     160     210
二叉树不平衡，不平衡子树根节点为: 130
右子数高
左子树更高
# 
```

## 5. 如何调整平衡二叉树

所谓的旋转，其实是修改指针指向的值，仅此而已。

二叉树不平衡有四种情况

**左左更高**

原始二叉树，若要调整为平衡二叉树，需要**整棵树向右旋转**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1pS4GA6oAWPp907ribhbKoBOYfs7hER0r1gwAYmibtAmCO5GwicHbdgCHw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

调整1:**整棵树向右旋转**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia13jvgke21yPHNxcxmF44exXicrMdEYqoy6icrDnYWiazKEvfevlS34qTuA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**左右更高**

原始二叉树，若要调整为平衡二叉树，需要 **先让不平衡子树左节点先向左旋转，然后再向右旋转**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1qvdITu1BJicwicWkv4axyAL1QIhUo9vjcfWicBczeWwHcnH8ncQcoWrhw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

调整1: 先让**不平衡子树左节点的树**先向左旋转

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1CIIJtWIOfKa9EunfNMI7xrxph28jPZBByQa5iajLNlVwhklxQLYEf2A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

调整2: **整棵树向右**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1ywSwric2icYfxPNIEQ8ia4vE2rRSTOHzp71Nz5ialuGozpUjkADd4UN3zA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**右左更高**

原始二叉树，若要调整为平衡二叉树，需要 **先让不平衡子树右节点的树先向右旋转，然后再向左旋转**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1ez6sonC5hibLNyzFjnibDhctllrM04dsicmWOSbKb4Qc8MxqErZemicfrQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

调整1: 先让**不平衡子树右节点的树**先向右旋转

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1DSuyR6H1rEklSZNpx7qrPG7vLG6ia3DfvDF2HKlKjmDALRcI1XUIR9Q/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

调整2: **整棵树向左**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1DfriaINibwOXAROvOibAjZroJFeUib8ibv7JcLFxkNzDhGs8tcqZCkeX72g/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**右右更高**

原始二叉树，若要调整为平衡二叉树，需要 **整棵树向左旋转**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia1RoFJUHmofiaDoRq5S8BroK3llIROKI1E5jh7n5YRypZ4jpP1X0vEichA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

调整1: **整棵树向左旋转**

![图片](https://mmbiz.qpic.cn/mmbiz_png/wqfIPAmgib2VIRtyUPx7QpbARdFvYhFia18lxpuAVX2sOZRC2ImbfRBxgwVhZt8SBiaibEiaSSY6ufhiapWk31icD6bXw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## 全部代码

```c
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <math.h>

typedef struct {
	int data; // 数据节点
	struct TreeNode *left; // 指向左子树
	struct TreeNode *right; // 指向右子树
} TreeNode , *PTreeNode;

// 记录平衡二叉树
bool BalanceTrue = false;
// 最小不平衡子树地址
TreeNode *rjt = NULL;

// 检查二叉树是否平衡，若不平衡 BalanceTrue 为 true
int checkTreeBalance(TreeNode *root) {
	if (NULL == root) { return 0; }
	int x = checkTreeBalance(root->left);
	int y = checkTreeBalance(root->right);
	
	// 若检测到最小二叉树后，不进行后面的检查
	if (BalanceTrue) return 0;
	int xx = abs(x-y);

	if (xx > 1) {
		// 左子树 和 右子树 相差大于1 ， 二叉树不平衡
		BalanceTrue = true;
		rjt = root;
	}
	 
	return (x>y?x+1:y+1);
}

// 返回二叉树树高
int treeHeight(TreeNode *root) {
	if (NULL == root) return 0;
	int ll = treeHeight(root->left);
	int rr = treeHeight(root->right);
	return (ll>rr?ll+1:rr+1);
}

// 父节点查询
TreeNode* queryTopData(TreeNode *root,int data) {
	// 空地址异常抛出
	if (NULL == root) return NULL;
	
	// top: 父节点 ，如果为Null, 该节点为父节点
	// tmp: 遍历查询节点 
	TreeNode *top = NULL;
	TreeNode *tmp = root;

	while (tmp != NULL) {
		if (data == tmp->data) {
			// 节点为 返回Null
			if (NULL == top) return NULL;
			return top;
		}

		top = tmp;
		if (data > tmp->data) {
			tmp = tmp->right;
		} else if (data < tmp->data) {
			tmp = tmp->left;
		}
	}
	return NULL;
}

// 左左旋转
//
// 不平衡二叉树
//       70
//      /   \
//    50    80
//   /  \    
//  40  60
//  /
// 30
//
// 旋转后平衡二叉树(向右旋转)
//
//    50
//  /   \
// 40    70
// /     /  \
//30   60    80
//
bool turnLL(TreeNode **root , TreeNode *notBalanceRoot) {
	
	if ((*root) != notBalanceRoot) {
		printf("左左旋转,非根节点\n");
		// 非根节点
		TreeNode *lleft = notBalanceRoot->left;
		TreeNode *lright = lleft->right;
		
		// 查找父节点
		TreeNode *fdata = queryTopData((*root),notBalanceRoot->data);
		if (NULL == fdata) return false;

		lleft->right = notBalanceRoot;
		notBalanceRoot->left = lright;
		
		if (notBalanceRoot == fdata->left) {
			fdata->left = lleft;
		} else if (notBalanceRoot == fdata->right) {
			fdata->right = lleft;
		}
		return true;

	} else {
		// 根节点
		printf("左左旋转,是根节点\n");
		TreeNode *lleft = notBalanceRoot->left;
		TreeNode *absroot = lleft;
		TreeNode *lright = lleft->right;
		

		lleft->right = notBalanceRoot;
		notBalanceRoot->left = lright;

		(*root) = absroot;
		return true;
	}

}

// 左右旋转
//不平衡二叉树
//      70
//     /   \
//    50    80
//    / \    
//   40 60
//  /   
// 55
//
//左子树向左
//      70
//     /   \
//    60    80
//    /
//   50
//  /  \    
// 40  55
//                                                           
//                                                                   
// 整棵树向右
// 
//     60
//    /   \
//   50    70
//  /  \     \
// 40  55    80
//
bool turnLR(TreeNode **root , TreeNode *notBalanceRoot) {
	if ((*root) != notBalanceRoot) {
		printf("左右旋转，非根节点");
		
		TreeNode *lleft = notBalanceRoot->left;
		TreeNode *leftRight = lleft->right;
		TreeNode *leftRightLeft = leftRight->left;

		// 第一次调整
		leftRight->left = lleft;
		lleft->right = leftRightLeft;
		notBalanceRoot->left = leftRight;


		// 查找父节点
		TreeNode *fdata = queryTopData((*root),notBalanceRoot->data);
		//if (NULL != fdata) printf("fdata: %d\n",fdata->data);
		
		// 第二次调整
		lleft = notBalanceRoot->left;
		leftRight = lleft->right;
		
		lleft->right = notBalanceRoot;
		notBalanceRoot->left = leftRight;

		
		if (notBalanceRoot == fdata->left) {
			fdata->left = lleft;
		} else if (notBalanceRoot == fdata->right) {
			fdata->right = lleft;
		}
	} else {
		printf("左右旋转，是根节点\n");

		TreeNode *lleft = notBalanceRoot->left;
		TreeNode *leftRight = lleft->right;
		TreeNode *leftRightLeft = leftRight->left;

		// 第一次调整
		leftRight->left = lleft;
		lleft->right = leftRightLeft;
		notBalanceRoot->left = leftRight;
		
		// 第二次调整
		lleft = notBalanceRoot->left;
		leftRight = lleft->right;
		
		lleft->right = notBalanceRoot;
		notBalanceRoot->left = leftRight;

		(*root) = lleft;
	}
}

// 右左旋转
//不平衡二叉树
//   70
//  /  \
// 50   80
//     /  \
//    75  88
//     \
//     77
//
//左子树向右
//   70
//  /  \
// 50   75
//     /  \
//    77  80
//         \
//         88
//                                                                                                           
//                                                                                                                  
//                                                                                                                      
//整棵树向左
//     75
//    /  \
//   70  80
//  /  \   \ 
// 50  77  88 
//
bool turnRL(TreeNode **root , TreeNode *notBalanceRoot) {
	TreeNode *rright = notBalanceRoot->right;
	TreeNode *rightLeft = rright->left;
	TreeNode *rightLeftRight = rightLeft->right;

	// 第一次调整
	rightLeft->right = rright;
	rright->left = rightLeftRight;
	notBalanceRoot->right = rightLeft;

	// 查找父节点
	TreeNode *fdata = queryTopData((*root),notBalanceRoot->data);
	//if (NULL != fdata) printf("fdata: %d\n",fdata->data);

	// 第二次调整
	rright = notBalanceRoot->right;
	rightLeft = rright->left;

	rright->left = notBalanceRoot;
	notBalanceRoot->right = rightLeft;

	if ((*root) != notBalanceRoot) {
		printf("右左旋转，非根节点\n");
		if (notBalanceRoot == fdata->left) {
			fdata->left = rright;
		} else if (notBalanceRoot == fdata->right) {
			fdata->right = rright;
		}
	
	} else {
		printf("右左旋转，是根节点\n");
		(*root) = rright;
	}
}

// 右右旋转
// 
// 不平衡二叉树
//  70
// /  \
//50   80
//    /  \
//   75  88
//      /
//     85
//
//
//
//向左旋转
//    80
//   /  \
//  70   88
// /  \   /  
//50  75 85  
bool turnRR(TreeNode **root , TreeNode *notBalanceRoot) {
	if ((*root) != notBalanceRoot) {
		printf("右右旋转,非根节点");
		TreeNode *rright = notBalanceRoot->right;
		TreeNode *rleft = rright->left;

		// 查找父节点
		TreeNode *fdata = queryTopData((*root),notBalanceRoot->data);
		if (NULL != fdata) printf("fdata: %d\n",fdata->data);

                rright->left = notBalanceRoot;
                notBalanceRoot->right = rleft;

                if (notBalanceRoot == fdata->left) {
                        fdata->left = rright;
                } else if (notBalanceRoot == fdata->right) {
                        fdata->right = rright;
                }				

	} else {
		// 右右旋转，是根节点
		printf("右右旋转,是根节点\n");
		TreeNode *rright = notBalanceRoot->right;
		TreeNode *absroot = rright;
		TreeNode *rleft = rright->left;

		rright->left = notBalanceRoot;
		notBalanceRoot->right = rleft;

		(*root) = absroot;

	}
}

// 二叉树前序遍历
void Print1(TreeNode *root) {
	if (NULL == root) return;
	printf("%d\t",root->data);
	Print1(root->left);
	Print1(root->right);
}

// 二叉树中序遍历
void Print2(TreeNode *root) {
	if (NULL == root) return;
	Print2(root->left);
	printf("%d\t",root->data);
	Print2(root->right);
}

// 二叉树后续遍历
void Print3(TreeNode *root) {
	if (NULL == root) return;
	Print3(root->left);
	Print3(root->right);
	printf("%d\t",root->data);
}

// 插入二叉树节点
TreeNode* addNode(TreeNode *root,int data) {
	if (NULL == root) {
		// 头节点插入
		TreeNode *Node = (TreeNode *)malloc(sizeof(TreeNode));
		if (NULL == Node) {
			printf("新节点申请内存失败\n");
			return NULL;
		}
		Node->data = data;

		return Node;
	}

	TreeNode *tmp = root;
	TreeNode *top = NULL;

	// 找到合适的最尾巴节点
	while (NULL != tmp) {
		top = tmp;
		if (tmp->data == data) {
			printf("已经存在该节点，节点地址: %p\n",tmp);
			return root;
		}
		if (tmp->data < data) {
			tmp = tmp->right;
		} else if (tmp->data > data) {
			tmp = tmp->left;
		}
	}

	TreeNode *Node = (TreeNode *)malloc(sizeof(TreeNode));
	Node->data = data;
	if (NULL == Node) {
		printf("申请新节点内存失败\n");
		return root;
	}

	// 链接节点
	if (data > top->data) {
		top->right = Node;
	} else if (data < top->data) {
		top->left = Node;
	}

	return root;
}


// 删除二叉排序树节点
bool DeleteTreeNode(TreeNode **TreeRoot,int data) {
	if (NULL == (*TreeRoot)) return false;

	printf("删除节点: %d\n",data);

	TreeNode *tmp = (*TreeRoot);
	TreeNode *top = NULL;
	
	while (tmp != NULL) {
		if (tmp->data == data) {
			// 叶子节点
			if ((NULL == tmp->left) && (NULL == tmp->right)) {
				// 叶子节点
				if (NULL == top) {
					// 仅有根节点的叶子节点
					free(tmp);
					return true;
				} else {
					// 其他的叶子节点
					TreeNode *lastNode = top;
					if (tmp == lastNode->left) {
						lastNode->left = NULL;
					} else if (tmp == lastNode->right) {
						lastNode->right = NULL;
					}
					free(tmp);
					return true;
				}
			} else {
				// 非叶子节点
				// 算法为: 
				// 默认算法为: 1.  当删除该节点时，获取该树右子树最左子节点
				//             2.  当右子树为空时，此时应该获取左子树最右端子节点

				if (NULL != tmp->right) {
					// 方案 1
					TreeNode *tmp2 = tmp->right;
					TreeNode *top2 = NULL;

					// 找到最后一个节点
					while (tmp2->left != NULL) {
						top2 = tmp2;
						tmp2 = tmp2->left;
					}

					// 删除老的节点
					tmp->data = tmp2->data;
					// 只有右子树节点 没有左子树节点
					if (NULL == top2) {
						tmp->right = NULL;

					} else {
						top2->left = NULL;
					}
					free(tmp2);
				} else {
					// 方案 2
					TreeNode *tmp2 = tmp->left;
					TreeNode *top2 = NULL;

					// 找到最后一个节点
					while (tmp2->right != NULL) {
						tmp2 = tmp2->right;
					}

					// 删除老的节点
					tmp->data = tmp2->data;
					if (NULL == top2) {
						tmp->left = NULL;
					} else {
						top2->right = NULL;
					}
					free(tmp2);
				}
				
			}
		} else {
			top = tmp;
			if (data > tmp->data) {
				tmp = tmp->right;
			} else {
				tmp = tmp->left;
			}
		}
	}
	return false;
}

// 二叉树平衡调整
bool treeBalance(TreeNode **root) {
	checkTreeBalance((*root));
	while (BalanceTrue) {
		printf("二叉树不平衡,最小不平衡子树数据结点: %d\n",rjt->data);
		TreeNode *tmp;

		if (1 < treeHeight(rjt->left) - treeHeight(rjt->right)) {
			// 对于不平衡二叉树而言，左子树比右子树高
			//
			//printf("左\n");
			if (rjt->left != NULL) {
				tmp = rjt->left;
				int ll = treeHeight(tmp->left);
				int rr = treeHeight(tmp->right);
				
				if (ll > rr) {
					// 对于不平衡子树 左子树 而言， 左子树比右子树高
					// 左左旋转

					turnLL(root,rjt);

				} else {
					// 对于不平衡子树 左子树 而言， 右子树比左子树高
					// 左右旋转
					//
					turnLR(root ,rjt);
				}
			} 
		} else if (1 < treeHeight(rjt->right) - treeHeight(rjt->left)) {
			// 对于不平衡二叉树而言，右子树比左子树高
			//
			//printf("右\n");
			if (rjt->right != NULL) {
				tmp = rjt->right;
				int ll = treeHeight(tmp->left);
				int rr = treeHeight(tmp->right);

				if (ll > rr) {
					//右左旋转
					turnRL(root,rjt);
				} else {
					//右右旋转
					turnRR(root,rjt);
				}
			}		
		}
		
		BalanceTrue = false;
		checkTreeBalance((*root));
		printf("二叉树调整平衡后数据结点:\n");
		printf("前序遍历:");
		Print1(*root);
		printf("\n");
		printf("中序遍历:");
		Print2(*root);
		printf("\n");
		printf("\n");
	}

}

int main() {
	TreeNode *root = NULL;

	printf("平衡二叉树插入测试\n");
	int nums[] = {65,60,70,55,40,63,69,66,68,77};
	int i;
	for (i=0;i<sizeof(nums)/sizeof(int);i++) {
		printf("插入数据: %d\n",nums[i]);

		root = addNode(root,nums[i]);
		if (NULL == root) {
			printf("首节点申请失败"); 
			return -1;
		}

		treeBalance(&root);
		sleep(1);

	}
	printf("\n当前二叉树遍历\n");
	printf("前序遍历:");
	Print1(root);
	printf("\n");
	printf("中序遍历:");
	Print2(root);
	printf("\n");
	//return 0;

	printf("\n\n平衡二叉树删除测试\n");

	for (i=2;i<5;i++) {
		DeleteTreeNode(&root,nums[i]);

		treeBalance(&root);
		sleep(1);
	}

	printf("\n当前二叉树遍历\n");
	printf("前序遍历:");
	Print1(root);
	printf("\n");
	printf("中序遍历:");
	Print2(root);
	printf("\n");
		
	return 0;
}
```

程序执行结果

```bash
# gcc BalanceTree.c -w -g -std=c11
# 
# ./a.out 
平衡二叉树插入测试
插入数据: 65
插入数据: 60
插入数据: 70
插入数据: 55
插入数据: 40
二叉树不平衡,最小不平衡子树数据结点: 60
左左旋转,非根节点
二叉树调整平衡后数据结点:
前序遍历:65     55      40      60      70
中序遍历:40     55      60      65      70

插入数据: 63
二叉树不平衡,最小不平衡子树数据结点: 65
左右旋转，是根节点
二叉树调整平衡后数据结点:
前序遍历:60     55      40      65      63      70
中序遍历:40     55      60      63      65      70

插入数据: 69
插入数据: 66
二叉树不平衡,最小不平衡子树数据结点: 70
左左旋转,非根节点
二叉树调整平衡后数据结点:
前序遍历:60     55      40      65      63      69      66      70
中序遍历:40     55      60      63      65      66      69      70

插入数据: 68
二叉树不平衡,最小不平衡子树数据结点: 65
右左旋转，非根节点
二叉树调整平衡后数据结点:
前序遍历:60     55      40      66      65      63      69      68      70
中序遍历:40     55      60      63      65      66      68      69      70

插入数据: 77
二叉树不平衡,最小不平衡子树数据结点: 60
右右旋转,是根节点
二叉树调整平衡后数据结点:
前序遍历:66     60      55      40      65      63      69      68      70      77
中序遍历:40     55      60      63      65      66      68      69      70      77


当前二叉树遍历
前序遍历:66     60      55      40      65      63      69      68      70      77
中序遍历:40     55      60      63      65      66      68      69      70      77


平衡二叉树删除测试
删除节点: 70
删除节点: 55
删除节点: 40
二叉树不平衡,最小不平衡子树数据结点: 60
右左旋转，非根节点
二叉树调整平衡后数据结点:
前序遍历:66     63      60      65      69      68      77
中序遍历:60     63      65      66      68      69      77


当前二叉树遍历
前序遍历:66     63      60      65      69      68      77
中序遍历:60     63      65      66      68      69      77
# 
```