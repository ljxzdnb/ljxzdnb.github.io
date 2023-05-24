#include <stdio.h>
#include <stdarg.h>

void func2(int arg1, ...) {
    va_list ap;
    va_start(ap, arg1);

    // 读取所有可变参数
    int arg2 = va_arg(ap, int);
    double arg3 = va_arg(ap, double);
    
    printf("func2: arg1=%d, arg2=%d, arg3=%.2lf\n", arg1, arg2, arg3);

    va_end(ap);
}

void func1(int arg1, ...) {
    va_list ap;
    va_start(ap, arg1);
    
    // 读取所有可变参数
    int arg2 = va_arg(ap, int);
    double arg3 = va_arg(ap, double);

    printf("func1: arg1=%d, arg2=%d, arg3=%.2lf\n", arg1, arg2, arg3);

    // 调用另一个带有可变参数的函数
    func2(arg1 + 1000, ap);

    va_end(ap);
}

int main() {
   func1(1234, 5678, 9.8765);

   return 0;
}

