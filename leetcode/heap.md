[堆数据结构](https://www.geeksforgeeks.org/heap-data-structure/?ref=shm)

堆是一种特殊的基于树的数据结构，其中树是完全二叉树。通常，堆可以有两种类型：

1. **Max-Heap**：在 Max-Heap 中，存在于根节点的键必须是所有子节点中存在的键中最大的。对于该二叉树中的所有子树，相同的属性必须递归地为真。
2. **Min-Heap**：在 Min-Heap 中，根节点上存在的键必须是其所有子节点中存在的键中的最小值。对于该二叉树中的所有子树，相同的属性必须递归地为真。

![堆数据结构](https://media.geeksforgeeks.org/wp-content/cdn-uploads/MinHeapAndMaxHeap.png)

# [二叉堆](https://www.geeksforgeeks.org/binary-heap/)

二叉堆是具有以下属性的二叉树。
1）它是一棵完整的树（除了可能的最后一层，所有层都被完全填满，最后一层的所有键都尽可能左）。二叉堆的这一特性使它们适合存储在数组中。

\2) 二叉堆是最小堆或最大堆。在最小二叉堆中，根处的键必须是二叉堆中所有键中的最小值。对于二叉树中的所有节点，相同的属性必须递归地为真。Max Binary Heap 类似于 MinHeap。

**最小堆的例子：**

```
         10     10
         / \    / \  
       20  100 15  30  
      /   / \  / \
    30  40 50 100 40
```

**二叉堆是如何表示的？**
二叉堆是一棵完全二叉树。二叉堆通常表示为一个数组。

- 根元素将位于 Arr[0]。
- 下表显示了第 i个节点的其他节点的索引，即 Arr[i]：

![二叉堆树](https://media.geeksforgeeks.org/wp-content/cdn-uploads/binaryheap.png)

- 有关详细信息，请参阅[二叉堆的数组表示](https://www.geeksforgeeks.org/array-representation-of-binary-heap/)。

- **堆的应用：**
  **1)** [堆排序](https://www.geeksforgeeks.org/heap-sort/)：堆排序使用二叉堆在 O(nLogn) 时间内对数组进行排序。

- **2)**优先级队列：优先级队列可以使用二叉堆高效实现，因为它支持在 O(logn) 时间内进行 insert()、delete() 和 extractmax()、reduceKey() 操作。二叉堆和斐波那契堆是二叉堆的变体。这些变体也有效地执行联合。

- **3）**图算法：优先级队列特别用于图算法，如[Dijkstra 的最短路径](https://www.geeksforgeeks.org/greedy-algorithms-set-7-dijkstras-algorithm-for-adjacency-list-representation/)和[Prim 的最小生成树](https://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/)。

- **4）**使用堆可以有效地解决许多问题。参见以下示例。
  a)[数组中的第 K 个最大元素](https://www.geeksforgeeks.org/k-largestor-smallest-elements-in-an-array/)。
  b）[对几乎排序的数组进行排序/](https://www.geeksforgeeks.org/nearly-sorted-algorithm/)
  c）[合并K个排序数组](https://www.geeksforgeeks.org/merge-k-sorted-arrays/)。

- **Min Heap 上的操作：**
  **1)** getMini()：返回Min Heap 的根元素。此操作的时间复杂度为 O(1)。

- **2)** extractMin()：从MinHeap中移除最小元素。此操作的时间复杂度为 O(Logn)，因为此操作需要在删除 root 后维护堆属性（通过调用 heapify()）。

- **3）** reduceKey()：减小key的值。该操作的时间复杂度为 O(Logn)。如果一个节点的reduces key值大于该节点的父节点，那么我们不需要做任何事情。否则，我们需要向上遍历来修复被破坏的堆属性。

- **4)** insert()：插入一个新的key需要O(Logn)时间。我们在树的末尾添加一个新键。如果新键大于它的父键，那么我们不需要做任何事情。否则，我们需要向上遍历来修复被破坏的堆属性。

- **5）** delete()：删除一个key也需要O(Logn)时间。我们通过调用 reductionKey() 将要删除的键替换为最小无限。在 reductionKey() 之后，负无穷大的值必须到达根，所以我们调用 extractMin() 来删除键。