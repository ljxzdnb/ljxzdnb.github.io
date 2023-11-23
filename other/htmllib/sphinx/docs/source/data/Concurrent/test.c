#include "con_segmap.h"
#define MAXN 10000000
int c[MAXN];
int v[MAXN];
int a[MAXN];
int ptr, p;
uint32_t hash_func(void* x) {
    return *(int*)x;
}
bool equals(void* a, void* b) {
    return *(int*)a == *(int*)b;
}
int main()
{
    con_segmap* map = create_segmap(hash_func, equals, 4);
    for(int i = 0; i < MAXN; i ++) {
        v[i] = rand()%MAXN;
        int* cnt = sget(map, &v[i]);
        if (cnt == NULL) {
            c[ptr++] = 1;
            sput(map, &v[i], &c[ptr - 1]);
        } else {
            *cnt += 1;
        }
    }
    for(int i = 0; i < MAXN; i ++) {
        int* cnt = sget(map, &i);
        if(cnt == NULL) {
            continue;
        } else {
            while((*cnt) --) {
                a[p++] = i;
            }
        }
    }
}

