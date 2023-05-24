//void name(struct _signal_slot_node *sender, ##__VA_ARGS__); \

#include <stdio.h>
#define CLASS(...) _CLASS(__VA_ARGS__)
#define _CLASS(...)({ \
    struct class_t _class = {__VA_ARGS__};\
   _class;\
})\

struct class_t{
    char* name;
    int fd;
};

int main(){
    struct class_t class = CLASS(.name="ljx",.fd=1);
    printf("%d\n",class.fd);
}
