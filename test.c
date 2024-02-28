#include <stdio.h>

#define MAX_ROWS 50
#define MAX_COLS 50

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int M, N;
    int matrix[MAX_ROWS][MAX_COLS];

    // 读取输入
    scanf("%d %d", &M, &N);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            scanf("%d", &matrix[i][j]);
        }
    }

    // 动态规划求解
    for (int i = 1; i < M; ++i) {
        matrix[i][0] += matrix[i - 1][0];
    }
    for (int j = 1; j < N; ++j) {
        matrix[0][j] += matrix[0][j - 1];
    }
    for (int i = 1; i < M; ++i) {
        for (int j = 1; j < N; ++j) {
            matrix[i][j] += max(matrix[i - 1][j], matrix[i][j - 1]);
        }
    }

    // 输出结果
    printf("%d\n", matrix[M - 1][N - 1]);

    return 0;
}

