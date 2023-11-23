# [C语言布隆过滤器BloomFilter](https://blog.csdn.net/qq_35402412/article/details/79422775)

在实现BloomFilter，首先实现一个位图；

### **BitMap**

在位图中，每个元素为“0”或“1”，表示其对应的元素不存在或者存在。

![img](https://img-blog.csdn.net/20180302142441169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzU0MDI0MTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```c
typedef struct BitMap 
{ 
	size_t* _bits; 
	size_t _range; 
}BitMap; 
 
void BitMapInit(BitMap* bm, size_t range) ;
void BitMapSet(BitMap* bm, size_t x) ;
void BitMapReset(BitMap* bm, size_t x) ;
int BitMapTest(BitMap* bm, size_t x) ;
 
void BitMapInit(BitMap* bm, size_t range) {
	assert(bm);
	bm->_bits=(size_t*)malloc(sizeof(size_t)*((range>>5)+1));
	bm->_range=(range>>5)+1;
	memset(bm->_bits,0,sizeof(size_t)*((range>>5)+1));
}
 
void BitMapSet(BitMap* bm, size_t x) {
	size_t index,num;
	index=x>>5;
	num=x%32;
	bm->_bits[index]|=(1<<num);
}
 
void BitMapReset(BitMap* bm, size_t x) {
	size_t index,num;
	index=x>>5;
	num=x%32;
	bm->_bits[index]&=(~(1<<num));
}
void BitMapDestroy(BitMap* bm){
 	free(bm->_bits); 
}
 
// x存在返回0，不存在返回-1 
int BitMapTest(BitMap* bm, size_t x){
	size_t index,num;
	index=x>>5;
	num=x%32;
	if(bm->_bits[index]&(1<<num))
		return 0;
	return -1;
}
 
	free(bm->_bits); 
}

// x存在返回0，不存在返回-1 
int BitMapTest(BitMap* bm, size_t x){
	size_t index,num;
	index=x>>5;
	num=x%32;
	if(bm->_bits[index]&(1<<num))
		return 0;
	return -1;
}
```

写好位图后，可以快速的在海量数据中查找一个数据是否在其中

笔试题： 
给40亿个不重复的无符号整数，没排过序。给一个无符号整数，如何快速判断一个数是否在这40亿个数中。 【腾讯】

```c
#include"BitMap.h"
 
int main(){
	BitMap bm;
	BitMapInit(&bm,-1);//-1 无符号整型中为最大值
	BitMapSet(&bm, 2);
	BitMapSet(&bm, 33);
	BitMapSet(&bm, 120);
	BitMapSet(&bm, 9);
 
	printf("1?   %d \n",BitMapTest(&bm,1));
	printf("2?   %d \n",BitMapTest(&bm,2));
	printf("33?  %d \n",BitMapTest(&bm,33));
	printf("120? %d \n",BitMapTest(&bm,120));
	printf("9?   %d \n",BitMapTest(&bm,9));
 
	BitMapReset(&bm, 33) ;
	BitMapReset(&bm, 120) ;
	printf("33?  %d \n",BitMapTest(&bm,33));
	printf("120? %d \n",BitMapTest(&bm,120));
	system("pause");
	return 0;
}
```

![img](https://img-blog.csdn.net/20180302143024485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzU0MDI0MTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

BloomFilter
如果我们要存入的不是一个整数，而是字符串，那么引起冲突的可能性将大大增加，为了解决这个问题，就需要使用的布隆过滤器，但布隆过滤器并不是完全准确的，他只能只能保证不在是准确的，在则有可能是误判；

为降低存在的误判可能，采取多个HashFunc，这样，只要查找多次找到都为1，那么可以近似认为他就是存在；

反之，只要有一个为0，那么他必定不存在；


```c
#include"BitMap.h"
 
typedef const char* KeyValue;
typedef size_t(*HASH_FUNC)(const char* str);
 
typedef struct{
	BitMap _bm;
	HASH_FUNC hashfunc1;
	HASH_FUNC hashfunc2;
	HASH_FUNC hashfunc3;
}BloomFilter;
 
void BloomFilterInit(BloomFilter* bf,size_t range);
void BloomFilterSet(BloomFilter* bf, KeyValue x) ;
void BloomFilterReset(BloomFilter* bf, KeyValue x) ;
void BloomFilterTest() ;
void BloomFilterDestory(BloomFilter* bf); 
 
static size_t BKDRHash(KeyValue str)
{
	size_t seed = 131;  // 31 131 1313 13131 131313
	size_t hash = 0;
	while (*str )
	{
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}
 
size_t SDBMHash(KeyValue str)  
{  
	size_t ch;
	size_t hash = 0;  
	while (ch = (size_t)*str++)  
	{  
		hash = 65599 * hash + ch;         
		//hash = (size_t)ch + (hash << 6) + (hash << 16) - hash;  
	}  
	return hash;  
}
 
size_t RSHash(KeyValue str)  
{  
	size_t ch;
	size_t hash = 0;  
	size_t magic = 63689;     
	while (ch = (size_t)*str++)  
	{  
		hash = hash * magic + ch;  
		magic *= 378551;  
	}
	return hash;  
}
 
void BloomFilterInit(BloomFilter* bf,size_t range){
	assert(bf);
	BitMapInit(&bf->_bm,range);
	bf->hashfunc1=BKDRHash;
	bf->hashfunc2=SDBMHash;
	bf->hashfunc3=RSHash;
}
 
void BloomFilterSet(BloomFilter* bf, KeyValue x){
	size_t hash1,hash2,hash3;
	hash1=bf->hashfunc1(x)%bf->_bm._range;
	hash2=bf->hashfunc2(x)%bf->_bm._range;
	hash3=bf->hashfunc3(x)%bf->_bm._range;
	BitMapSet(&bf->_bm,hash1);
	BitMapSet(&bf->_bm,hash2);
	BitMapSet(&bf->_bm,hash3);
}
 
void BloomFilterReset(BitMap* bm, KeyValue x) ;
 
int BloomFilterFind(BloomFilter* bf,KeyValue x){
 
	size_t hash1,hash2,hash3;
	hash1=bf->hashfunc1(x)%bf->_bm._range;
	hash2=bf->hashfunc2(x)%bf->_bm._range;
	hash3=bf->hashfunc3(x)%bf->_bm._range;
 
	if(BitMapTest(&bf->_bm,hash1)==-1)
		return -1;
	if(BitMapTest(&bf->_bm,hash2)==-1)
		return -1;
	if(BitMapTest(&bf->_bm,hash3)==-1)
		return -1;
 
	return 0;
}
 
void BloomFilterDestory(BloomFilter* bf){
	assert(bf);
	BitMapDestroy(&bf->_bm);
}
 
 
void BloomFilterTest(){
	BloomFilter bf;
	BloomFilterInit(&bf,10000);
	BloomFilterSet(&bf,"sort");
	BloomFilterSet(&bf,"srot");
	BloomFilterSet(&bf,"return");
	BloomFilterSet(&bf,"find");
	BloomFilterSet(&bf,"sort123456789");
 
	printf("sort?  %d\n",BloomFilterFind(&bf,"sort"));
	printf("srot?  %d\n",BloomFilterFind(&bf,"srot"));
	printf("return?  %d\n",BloomFilterFind(&bf,"return"));
	printf("sort123456789?  %d\n",BloomFilterFind(&bf,"sort123456789"));
	printf("find?  %d\n",BloomFilterFind(&bf,"find"));
	printf("find1?  %d\n",BloomFilterFind(&bf,"find1"));
	printf("asd?  %d\n",BloomFilterFind(&bf,"asd"));
	BloomFilterDestory(&bf);
}
```

![img](https://img-blog.csdn.net/20180302144335940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzU0MDI0MTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)