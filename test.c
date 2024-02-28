#include <stdio.h>
#include <stdlib.h>

// 定义比较函数，按照元素的最低位进行比较
int compare(const void *a, const void *b) {
    int num1 = *(int *)a;
    int num2 = *(int *)b;
    int last_digit_num1 = abs(num1 % 10); // 获取num1的最低位
    int last_digit_num2 = abs(num2 % 10); // 获取num2的最低位

    // 如果最低位相同，则按照原始顺序排序
    if (last_digit_num1 == last_digit_num2)
        return 0;

    return last_digit_num1 - last_digit_num2; // 按最低位从小到大排序
}

int main() {
    int nums[1000]={1,2,5,-21,22,11,55,-101,42,8,7,32};
    int num, i = 12;
/*
    // 读取输入数组
    while (scanf("%d,", &num) == 1) {
        nums[i++] = num;
    }
*/

    // 使用qsort函数进行排序
    qsort(nums, i, sizeof(int), compare);

    // 输出排序后的数组
    for (int j = 0; j < i; j++) {
        printf("%d", nums[j]);
        if (j != i - 1) printf(",");
    }

    return 0;
}
