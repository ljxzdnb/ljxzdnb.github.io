#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main() {
    char str[1001];
    char* strs[32];
    while (fgets(str, sizeof(str), stdin) != NULL) {
        int len = strlen(str);
        int count = 0;
        bool inQuotes = false;

        // 替换空格为换行符
        for (int i = 0; i < len; ++i) {
            if (str[i] == '"') {
                inQuotes = !inQuotes;
            } else if (str[i] == ' ' && !inQuotes) {
                str[i] = '\n';
            }
        }

        // 统计参数个数并输出
        char* token = strtok(str, "\n");
        while (token != NULL) {
            // 去除引号
            strs[count] = token;
            ++count;
            token = strtok(NULL, "\n");
        }
        printf("%d\n", count);
        for(int i=0;i<count;i++){
            if (strs[i][0] == '"' && strs[i][strlen(strs[i]) - 1] == '"') {
                printf("%.*s\n", (int)strlen(strs[i]) - 2, strs[i] + 1);
            } else {
                printf("%s\n", strs[i]);
            }
        }
    }
    return 0;
}

