#include <stdio.h>
#include <stdlib.h>

char* load_file(const char* filename) {
    FILE* file = fopen(filename, "rb"); // 以二进制方式打开文件
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END); // 定位到文件末尾
    long size = ftell(file); // 获取文件大小
    fseek(file, 0, SEEK_SET); // 定位到文件开头
    
    char* buffer = (char*) malloc(size + 1); // 分配缓冲区
    fread(buffer, size, 1, file); // 读取文件内容到缓冲区
    buffer[size] = '\0'; // 添加字符串结束符
    
    fclose(file); // 关闭文件
    
    return buffer;
}

int main() {
    char* data = load_file("example.txt");
    if (data != NULL) {
        printf("%s", data);
        free(data);
    }
    return 0;
}

