#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义国家信息结构体
typedef struct {
    char name[20];
    int gold;
    int silver;
    int bronze;
} Country;

// 比较函数，用于qsort排序
int compare(const void *a, const void *b) {
    const Country *countryA = (const Country *)a;
    const Country *countryB = (const Country *)b;
    
    // 按金牌数降序排列
    if (countryA->gold != countryB->gold)
        return countryB->gold - countryA->gold;
    // 金牌数相等，按银牌数降序排列
    else if (countryA->silver != countryB->silver)
        return countryB->silver - countryA->silver;
    // 金银牌数相等，按铜牌数降序排列
    else if (countryA->bronze != countryB->bronze)
        return countryB->bronze - countryA->bronze;
    // 金银铜牌数均相等，按国家名称字母序升序排列
    else
        return strcmp(countryA->name, countryB->name);
}

int main() {
    int n;
    scanf("%d", &n); // 输入国家的个数
    
    // 分配内存存储国家信息
    Country *countries = (Country *)malloc(n * sizeof(Country));
    
    // 输入每个国家的信息
    for (int i = 0; i < n; i++) {
        scanf("%s %d %d %d", countries[i].name, &countries[i].gold, &countries[i].silver, &countries[i].bronze);
    }
    
    // 使用qsort函数按照规则排序
    qsort(countries, n, sizeof(Country), compare);
    
    // 输出排名结果
    for (int i = 0; i < n; i++) {
        printf("%s %d %d %d\n", countries[i].name, countries[i].gold, countries[i].silver, countries[i].bronze);
    }
    
    // 释放动态分配的内存
    free(countries);
    
    return 0;
}

