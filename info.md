#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// 定义字符比较函数
int compare(const void *a, const void *b) {
    char charA = *(char*)a;
    char charB = *(char*)b;

    // 将字符转换为小写字母进行比较
    char lowerA = tolower(charA);
    char lowerB = tolower(charB);

    // 如果字符不同，按照字母顺序排序
    if (lowerA != lowerB) {
        return lowerA - lowerB;
    }

    // 如果字符相同，按照原来的顺序排序
    return charA - charB;
}

int main() {
    char str[1001];
    char bp[1001]; // 用于保存非字母字符
    int bpIndex = 0; // bp 的索引

    fgets(str, sizeof(str), stdin); // 读取输入字符串

    // 遍历字符串，将非字母字符保存到 bp 中
    for (int i = 0; str[i] != '\0'; ++i) {
        if (!isalpha(str[i])) {
            bp[bpIndex++] = str[i];
        }
    }

    // 对字母部分进行排序
    qsort(str, strlen(str), sizeof(char), compare);

    int bpPos = 0;
    // 将排序后的字母字符串插入到带有非字母字符的字符串中
    for (int i = 0; str[i] != '\0'; ++i) {
        if (!isalpha(str[i])) {
            continue; // 如果是非字母字符，跳过
        }
        // 如果是字母字符，则插入排序后的字符
        while (!isalpha(bp[bpPos])) {
            bpPos++; // 跳过非字母字符
        }
        bp[bpPos++] = str[i];
    }

    printf("%s", bp); // 输出结果

    return 0;
}

