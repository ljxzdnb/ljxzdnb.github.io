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
