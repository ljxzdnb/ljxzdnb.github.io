#include <stdio.h>
#include <stdlib.h>

/**
 * 完全二叉堆结构体
 */
typedef struct heap {
    int size;     // 当前元素个数
    int capacity; // 堆容量
    int* data;    // 数组存储堆元素
} Heap;

/**
 * 创建一个空的堆
 * @param capacity 初始容量
 * @return 返回创建的堆
 */
Heap* create_heap(int capacity) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->data = (int*)malloc(sizeof(int) * capacity);
    return heap;
}

/**
 * 获取父节点的索引
 * @param index 子节点的索引
 * @return 父节点的索引
 */
int parent(int index) {
    return (index - 1) / 2;
}

/**
 * 获取左孩子的索引
 * @param index 父节点的索引
 * @return 左孩子的索引
 */
int left_child(int index) {
    return index * 2 + 1;
}

/**
 * 获取右孩子的索引
 * @param index 父节点的索引
 * @return 右孩子的索引
 */
int right_child(int index) {
    return index * 2 + 2;
}

void resize(Heap *heap) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, heap->capacity * sizeof(int));
    }
}


/**
 * 将新元素插入到堆中
 * @param heap 堆
 * @param value 新元素
 */
void insert(Heap* heap, int value) {
    resize(heap);
    if (heap->size >= heap->capacity) {
        printf("Heap overflow!\n");
        return;
    }
    heap->data[heap->size] = value; // 插入到最后一个位置
    int index = heap->size;
    heap->size++;
    // 上滤操作
    while (index > 0 && heap->data[index] < heap->data[parent(index)]) {
        int temp = heap->data[index];
        heap->data[index] = heap->data[parent(index)];
        heap->data[parent(index)] = temp;
        index = parent(index);
    }
}

/**
 * 获取堆中最小元素的索引
 * @param heap 堆
 * @return 最小元素的索引
 */
int get_min_index(Heap* heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        return -1;
    }
    return 0;
}

/**
 * 从堆中删除最小元素
 * @param heap 堆
 */
void delete_key(Heap* heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        return;
    }
    int index=0;
    heap->data[index] = heap->data[heap->size - 1]; // 将最后一个元素放到根节点处
    heap->size--;
    // 下滤操作
    while (left_child(index) < heap->size) {
        int smaller_child = left_child(index); // 先假设左孩子较小
        if (right_child(index) < heap->size && heap->data[right_child(index)] < heap->data[smaller_child]) {
            smaller_child = right_child(index); // 如果右孩子比左孩子小，则右孩子更小
        }
        if (heap->data[index] > heap->data[smaller_child]) { // 如果父节点比较小的孩子大，则交换两者
            int temp = heap->data[index];
            heap->data[index] = heap->data[smaller_child];
            heap->data[smaller_child] = temp;
        } else {
            break; // 父节点已经比孩子节点都小了，不需要再下滤了
        }
        index = smaller_child;
    }
}

/**
 * 递归查找小于指定数字的所有索引
 * @param heap 堆
 * @param index 当前检索的堆元素索引
 * @param value 指定的数字
 */
void find_less_than(Heap* heap, int index, int value) {
    if (index >= heap->size) {
        return; // 超出范围，结束递归
    }
    if (heap->data[index] < value) { // 当前元素小于指定数字，输出其索引，并递归处理其左右孩子
        printf("%d ", index);
        find_less_than(heap, left_child(index), value);
        find_less_than(heap, right_child(index), value);
    }
}

int main() {
    Heap* heap = create_heap(10);
    insert(heap, 5);
    insert(heap, 7);
    insert(heap, 2);
    insert(heap, 9);
    insert(heap, 1);
    insert(heap, 4);
    insert(heap, 8);
    printf("Heap: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
    printf("Min index: %d\n", get_min_index(heap));
    delete_key(heap);
    delete_key(heap);
    printf("Heap after delete min: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
    printf("Indexes less than 7: ");
    find_less_than(heap, 0, 7); // 从根节点开始递归查找
    printf("\n");
    return 0;
}

