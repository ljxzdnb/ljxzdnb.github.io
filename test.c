#include <stdio.h>
#include <string.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int longestValidPassword(char *s) {
    int len = strlen(s);
    int maxLength = 0;

    for (int i = 0; i < len; i++) {
        // 从当前位置向两侧扩展寻找对称的密码串
        int left = i, right = i;
        while (left >= 0 && right < len && s[left] == s[right]) {
            maxLength = max(maxLength, right - left + 1);
            left--;
            right++;
        }

        left = i;
        right = i + 1;
        while (left >= 0 && right < len && s[left] == s[right]) {
            maxLength = max(maxLength, right - left + 1);
            left--;
            right++;
        }
    }

    return maxLength;
}

int main() {
    char str[2501]; // 为了容纳'\0'结束符，字符串长度设为2501
    scanf("%s", str);
    int result = longestValidPassword(str);
    printf("%d\n", result);
    return 0;
}

