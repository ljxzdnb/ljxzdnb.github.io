#include<stdio.h>
#include<stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef signed char int8_t;
//typedef int int16_t;

#define true 1
#define false 0


//宏列表
#define TAG_LIST(tag) \
tag(Run)\
tag(Alarm)\
tag(Online)\
tag(TimerOver)


//枚举处理
#define DEFINE_TAG(_tag) _tag,
enum Flag{
None=0,
TAG_LIST(DEFINE_TAG)
EmMAX
};
#undef DEFINE_TAG

//位定义变量
uint16_t SysFlag=0x0000;


//通用方法定义
uint8_t GetFlags(uint16_t mask)
{
	return((SysFlag&mask) != 0)?true:false;
}

void SetFlags(uint16_t mask)
{
	SysFlag |= mask;
}

void ClrFlags(uint16_t mask)
{
	SysFlag &= ~mask;
}


//自动生成三类函数定义
#define FLAG_Operater(flag)\
uint8_t get##flag(){\
return GetFlags(1<<flag);\
}\
void set##flag(){\
	SetFlags(1<<flag);\
}\
void clr##flag(){\
	ClrFlags(1<<flag);\
}

//反向函数关联
TAG_LIST(FLAG_Operater)

int main(int argc,char*argv[]){

	setAlarm();
//#setOnline();
	printf("set:%2x\r\n",SysFlag);

	if(getAlarm()==true)
	{
		printf("set\r\n");
	}
	else
	{
		printf("clr\r\n");
	}
		return 0;
}
