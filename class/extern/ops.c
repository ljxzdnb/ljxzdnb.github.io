#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int (*operation(char op))(int, int) {
    if (op == '+') {
        return &add;
    } else if (op == '-') {
        return &sub;
    } else {
        printf("Invalid operation\n");
        return NULL;
    }
}

int (*(*get_operation(int num))(char))(int, int){
    if (num == 1){
        return operation;
    }
    else{
        return NULL;
    }
}

int main() {
    int x = 10, y = 5;

    printf("%d + %d = %d\n", x, y, get_operation(1)('+')(x, y));
    printf("%d - %d = %d\n", x, y, operation('-')(x, y));
    
    return 0;
}

