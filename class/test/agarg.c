#include <stdio.h>
#include <stdarg.h>

int add(int count, ...) {
    va_list ap;
    int i;
   static int ret;
    printf("%d ", count);
    if (count > 0) {
        va_start(ap, count);

        for (i = 0; i < count; i++) {
             return add(count -1 ,ret += va_arg(ap, int));
        }

        va_end(ap);
    }
    return ret;
}

int main() {
    printf("ljx:%d\n",add(3, 3, 0, 0));
    return 0;
}

