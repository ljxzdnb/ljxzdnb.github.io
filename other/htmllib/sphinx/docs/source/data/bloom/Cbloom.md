# [布隆过滤器(Bloom Filter)的C语言实现](https://blog.csdn.net/weixin_33720956/article/details/93762260)

前文《[布隆过滤器(Bloom Filter)详解](http://www.cnblogs.com/haippy/archive/2012/07/13/2590351.html)》主要讲了[布隆过滤器](https://so.csdn.net/so/search?q=布隆过滤器&spm=1001.2101.3001.7020)(Bloom Filter)的基本原理和相关应用，本文将使用 C 语言实现一个简单的布隆过滤器(Bloom Filter)。

http://en.literateprograms.org/Bloom_filter_(C)?oldid=17386

```c
#ifndef __BLOOM_FILTER_H__
#define __BLOOM_FILTER_H__

#include<stdlib.h>

typedef unsigned int (*hashfunc_t)(const char *);
typedef struct {
    size_t asize;
    unsigned char *a;
    size_t nfuncs;
    hashfunc_t *funcs;
} BLOOM;

BLOOM *bloom_create(size_t size, size_t nfuncs, ...);
int bloom_destroy(BLOOM *bloom);
int bloom_add(BLOOM *bloom, const char *s);
int bloom_check(BLOOM *bloom, const char *s);

#endif
```

```c
#include<limits.h>
#include<stdarg.h>

#include"bloom.h"

#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

BLOOM *bloom_create(size_t size, size_t nfuncs, ...)
{
    BLOOM *bloom;
    va_list l;
    int n;
    
    if(!(bloom=malloc(sizeof(BLOOM)))) return NULL;
    if(!(bloom->a=calloc((size+CHAR_BIT-1)/CHAR_BIT, sizeof(char)))) {
        free(bloom);
        return NULL;
    }
    if(!(bloom->funcs=(hashfunc_t*)malloc(nfuncs*sizeof(hashfunc_t)))) {
        free(bloom->a);
        free(bloom);
        return NULL;
    }

    va_start(l, nfuncs);
    for(n=0; n<nfuncs; ++n) {
        bloom->funcs[n]=va_arg(l, hashfunc_t);
    }
    va_end(l);

    bloom->nfuncs=nfuncs;
    bloom->asize=size;

    return bloom;
}

int bloom_destroy(BLOOM *bloom)
{
    free(bloom->a);
    free(bloom->funcs);
    free(bloom);

    return 0;
}

int bloom_add(BLOOM *bloom, const char *s)
{
    size_t n;

    for(n=0; n<bloom->nfuncs; ++n) {
        SETBIT(bloom->a, bloom->funcs[n](s)%bloom->asize);
    }

    return 0;
}

int bloom_check(BLOOM *bloom, const char *s)
{
    size_t n;

    for(n=0; n<bloom->nfuncs; ++n) {
        if(!(GETBIT(bloom->a, bloom->funcs[n](s)%bloom->asize))) return 0;
    }

    return 1;
}
```

测试文件：

```c
#include<stdio.h>
#include<string.h>

#include"bloom.h"

unsigned int sax_hash(const char *key)
{
    unsigned int h=0;

    while(*key) h^=(h<<5)+(h>>2)+(unsigned char)*key++;

    return h;
}

unsigned int sdbm_hash(const char *key)
{
    unsigned int h=0;
    while(*key) h=(unsigned char)*key++ + (h<<6) + (h<<16) - h;
    return h;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char line[1024];
    char *p;
    BLOOM *bloom;
    
    if(argc<2) {
        fprintf(stderr, "ERROR: No word file specified\n");
        return EXIT_FAILURE;
    }

    if(!(bloom=bloom_create(2500000, 2, sax_hash, sdbm_hash))) {
        fprintf(stderr, "ERROR: Could not create bloom filter\n");
        return EXIT_FAILURE;
    }

    if(!(fp=fopen(argv[1], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    while(fgets(line, 1024, fp)) {
        if((p=strchr(line, '\r'))) *p='\0';
        if((p=strchr(line, '\n'))) *p='\0';

        bloom_add(bloom, line);
    }

    fclose(fp);

    while(fgets(line, 1024, stdin)) {
        if((p=strchr(line, '\r'))) *p='\0';
        if((p=strchr(line, '\n'))) *p='\0';

        p=strtok(line, " \t,.;:\r\n?!-/()");
        while(p) {
            if(!bloom_check(bloom, p)) {
                printf("No match for ford \"%s\"\n", p);
            }
            p=strtok(NULL, " \t,.;:\r\n?!-/()");
        }
    }

    bloom_destroy(bloom);

    return EXIT_SUCCESS;
}

```

