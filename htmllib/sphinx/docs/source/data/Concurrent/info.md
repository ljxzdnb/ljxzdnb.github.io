## [c语言实现 Concurrent HashMap](https://blog.csdn.net/GreyKa/article/details/126326602)

基本原理介绍
扩容：当 hash 桶超过 4 / 5 中有内容时候，重新开辟两倍大小的桶空间，并且把元素移动到新的空间中
分段加锁：将整个哈希表划分成多个子表，对每个子表进行加锁，保证子表之间访问互不干扰，进行两段式哈希，第一次定位到子表，第二次定位到所在子表的桶中。
update1
实测随机数据下面进行桶排序的速度是 C++ STL unordered_map 的四倍，但是在完全无哈希冲突的数据下面速度会慢不少，推测是因为扩容的逻辑比较耗时。后续可以改进扩容逻辑。

update2
扩容逻辑优化，实测下来随机情况下效率提升10%，完全无哈希冲突时效率提升250%。保持了各种情况下面效率为 C++ STL unordered_map 效率的三倍以上。接下来的任务是研究上锁，做成线程安全的。

update3
对串行 hashmap 加锁， 外层包装一个 分段加锁的 hashmap， 完成了类似 JDK 1.7 之前的 Concurrent HashMap 机制的分段式加锁 并发哈希表。

C++ STL unordered_map 和 con_segmap 效率对比 （执行代码见最下方）



hash_entry.h

```c
#pragma once
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define INIT_SIZE 4

typedef struct entry {
    void* key;
    void* val;
    struct entry* next;
}entry;

entry* create_entry(void* key, void* val, entry* next) {
    entry* ret = malloc(sizeof(entry));
    ret->key = key;
    ret->val = val;
    ret->next = next;
    return ret;
}

entry* create_entries(int n) {
    return calloc(n, sizeof(entry));
}

void lput(entry* head, entry* item, bool (*equals)(void* a, void* b)) {
    while(head->next != NULL) {
        entry* now = head->next;
        if((*equals)(now->key, item->key)) {
            now->val = item->val;
            return;
        }
        head = head->next;
    }
    head->next = item;
    item->next = NULL;
}

void* lget(entry* head, void* key, bool (*equals)(void* a, void* b)) {
    void* ret = NULL;
    while(head->next != NULL) {
        entry* now = head->next;
        if((*equals)(now->key, key)) {
            ret = now->val;
            break;
        }
        head = head->next;
    }
    return ret;
}

void* ldel(entry* head, void* key, bool (*equals)(void* a, void* b)) {
    while(head->next != NULL) {
        entry* now = head->next;
        if((*equals)(now->key, key)) {
            head->next = now->next;
            return now;
        }
        head = head->next;
    }
    return NULL;
}
```

con_hashmap.h

```c
#pragma once
#include "hash_entry.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define INIT_SIZE 4


typedef struct con_hashmap {
    size_t cap;
    size_t load;
    size_t size;
    entry** data;
    size_t hash_offset;
    uint32_t (*hash_func)(void* v);
    bool (*equals)(void* a, void* b);
    pthread_spinlock_t lock;
}con_hashmap;

con_hashmap* create_hashmap(uint32_t (*hash_func)(void* v), bool (*equals)(void* a, void* b)) {
    con_hashmap* ret = malloc(sizeof(con_hashmap));
    ret->cap = (1 << INIT_SIZE);
    ret->load = ret->size = ret->hash_offset = 0;
    ret->data = malloc(sizeof(entry*) * (1 << INIT_SIZE));
    entry* heads = create_entries(1 << INIT_SIZE);
    for(int i = 0; i < (1 << INIT_SIZE); i ++) {
        ret->data[i] = &heads[i];
    }
    ret->hash_func = hash_func;
    ret->equals = equals;
    pthread_spin_init(&ret->lock, PTHREAD_PROCESS_PRIVATE);
    return ret;
}

uint32_t _hv(con_hashmap* s, void* key) {
    return ((*s->hash_func)(key)) >> s->hash_offset;
}

void* _get_bucket(con_hashmap* s, uint32_t hv) {
    uint32_t bucket_id = hv & (s->cap - 1);
    return s->data[bucket_id];
}

void* _get_bucket_raw(entry** data, size_t cap, uint32_t hv) {
    uint32_t bucket_id = hv & (cap - 1);
    return data[bucket_id];
}

void _resize(con_hashmap* s) {
    size_t new_cap = s->cap << 1;
    size_t new_load = 0;
    entry* heads = create_entries(new_cap);
    entry** new_data = malloc(sizeof(entry*) * new_cap);
    for(int i = 0; i < new_cap; i ++) {
        new_data[i] = &heads[i];
    }

    for(int i = 0; i < s->cap; i ++) {
        entry* h = s->data[i];
        while(h->next != NULL) {
            entry* now = h->next;
            h->next = now->next;
            entry* bucket = _get_bucket_raw(new_data, new_cap, _hv(s, now->key));
            if(bucket->next == NULL){
                new_load ++;
            }
            lput(bucket, now, s->equals);
        }
    }

    entry* old_heads = s->data[0];
    entry** old_data = s->data;
    s->data = new_data;
    s->cap = new_cap;
    s->load = new_load;

    free(old_heads);
    free(old_data);
}

void* _get(con_hashmap* s, uint32_t hv, void* key) {
    entry* bucket = _get_bucket(s, hv);
    return lget(bucket, key, s->equals);
}

void* get(con_hashmap* s, void* key) {
    pthread_spin_lock(&s->lock);
    void* ret = _get(s, _hv(s, key), key);
    pthread_spin_unlock(&s->lock);
    return ret;
}

void _put(con_hashmap*s, uint32_t hv, void* key, void* val) {
    entry* bucket = _get_bucket(s, hv);
    if(bucket->next == NULL) {
        s->load ++;
    }
    if(s->cap * 4 <= s->load * 5) {
        _resize(s);
        bucket = _get_bucket(s, hv);
        if(bucket->next == NULL) {
            s->load ++;
        }
    }
    entry* item = create_entry(key, val, NULL);
    lput(bucket, item, s->equals);
    s->size ++;
}

void put(con_hashmap* s, void* key, void* val) {
    pthread_spin_lock(&s->lock);
    _put(s, _hv(s, key), key, val);
    pthread_spin_unlock(&s->lock);
}

void* _del(con_hashmap* s, uint32_t hv, void* key) {
    entry* bucket = _get_bucket(s, hv);
    void* ret = ldel(bucket, key, s->equals);
    if(bucket->next == NULL) {
        s->load --;
    }
    s->size --;
    return ret;
}

void* del(con_hashmap* s, void* key) {
    pthread_spin_lock(&s->lock);
    void* ret = _del(s, _hv(s, key), key);
    pthread_spin_unlock(&s->lock);
    return ret;
}

size_t size(con_hashmap* s) {
    return s->size;
}
```

con_segmap.h

```c
#pragma once
#include "con_hashmap.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define INIT_SIZE 4


typedef struct con_segmap {
    con_hashmap** submaps;
    uint32_t (*hash_func)(void* v);
    size_t con_level;
}con_segmap;

con_segmap* create_segmap(uint32_t (*hash_func)(void* v), bool (*equals)(void* a, void* b), size_t con_level) {
    con_segmap* ret = malloc(sizeof(con_segmap));
    ret->con_level = con_level;
    ret->hash_func = hash_func;
    ret->submaps = malloc(sizeof(con_hashmap*) * (1 << con_level));
    for(int i = 0; i < (1 << con_level); i ++) {
        ret->submaps[i] = create_hashmap(hash_func, equals);
        ret->submaps[i]->hash_offset = con_level;
    }
    return ret;
}

uint32_t __hv(con_segmap* s, void* x) {
    return (*s->hash_func)(x);
}

void sput(con_segmap* s, void* key, void* val) {
    uint32_t hv = __hv(s, key);
    uint32_t sub_id = hv & ((1 << s->con_level) - 1);
    pthread_spin_lock(&s->submaps[sub_id]->lock);
    _put(s->submaps[sub_id], hv >> s->con_level, key, val);
    pthread_spin_unlock(&s->submaps[sub_id]->lock);
}

void* sget(con_segmap* s, void* key) {
    uint32_t hv = __hv(s, key);
    uint32_t sub_id = hv & ((1 << s->con_level) - 1);
    pthread_spin_lock(&s->submaps[sub_id]->lock);
    void* ret = _get(s->submaps[sub_id], hv >> s->con_level, key);
    pthread_spin_unlock(&s->submaps[sub_id]->lock);
    return ret;
}

void* sdel(con_segmap* s, void* key) {
    uint32_t hv = __hv(s, key);
    uint32_t sub_id = hv & ((1 << s->con_level) - 1);
    pthread_spin_lock(&s->submaps[sub_id]->lock);
    void* ret = _del(s->submaps[sub_id], hv >> s->con_level, key);
    pthread_spin_unlock(&s->submaps[sub_id]->lock);
    return ret;
}

size_t ssize(con_segmap* s) {
    size_t ret = 0;
    for(int i = 0; i < (1 << s->con_level); i ++) {
        ret += s->submaps[i]->size;
    }
    return ret;
}
```

test.c

```c
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
```

