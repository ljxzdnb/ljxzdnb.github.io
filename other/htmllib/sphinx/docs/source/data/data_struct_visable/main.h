#ifndef _LOG_h_
#define _LOG_h_

#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>

#define COL(x)  "\033[;" #x "m"

#define RED(x)     COL(31) x "\033[m"
#define GREEN(x)   COL(32) x "\033[m"
#define YELLOW(x)  COL(33) x "\033[m"
#define BLUE(x)    COL(34) x "\033[m"
#define MAGENTA(x) COL(35) x "\033[m"
#define CYAN(x)    COL(36) x "\033[m"
#define WHITE(x)   COL(37) x "\033[m"
#define SRED(x)    COL(31) x "\034[m"
#endif

typedef int type;
typedef type (*fun_t)(int,type,...);
#define STR(x) #x

#define TYPE(var) _Generic((var),\
    int:"int", \
    char:"char", \
    float:"float", \
    double:"double", \
    int *:"int *", \
    char *:"char *", \
    default:"other type")

#define TYPEDEF(var) _Generic((var),\
    int:typedef int type)\

#define COLOR_TYPE(var) _Generic((var),\
    int:GREEN("int"), \
    char:WHITE("char"), \
    float:CYAN("float"), \
    double:MAGENTA("double"), \
    int *:YELLOW("int *"), \
    char *:BLUE("char *"), \
    default:"other type")

#define CALL_BACK(var,ops,x,...) _Generic((var),\
    int(*)(int,int,...):var(ops,x,##__VA_ARGS__,0),\
    float(*)(float,int,...):var(ops,x,##__VA_ARGS__,0),\
    double(*)(double,int,...):var(ops,x,##__VA_ARGS__,0),\
    default:printf("other type"))

#define FORM(var) _Generic((var),\
    int:"%d", \
    char:"%c", \
    float:"%f", \
    double:"%lf", \
    int *:"%p", \
    char *:"%s", \
    default:"%p")

//宏列表
#define TAG_LIST(tag,mark) \
tag(tim,mark)\
tag(div,mark)\
tag(odd,mark)\
tag(add,mark)\
tag(mil,mark)\
tag(and,mark)

//枚举处理
#define DEFINE_TAG(_tag,nil) _tag,
enum Flag{
None=0,
TAG_LIST(DEFINE_TAG,0)
EmMAX
};
#undef DEFINE_TAG

//通用方法定义
int GetFlags(int* flag,int mask)
{
	return((*flag&mask) != 0)?1:0;
}

void SetFlags(int* flag,int mask)
{
	*flag |= mask;
}

void ClrFlags(int* flag,int mask)
{
	*flag &= ~mask;
}

#define OPS_SELECT(ops,a,b) (ops&(1<<tim)? (a*b):(ops&(1<<div)? (a/b):(ops&(1<<odd)? (a%b):(ops&(1<<add)? (a+b):(ops&(1<<mil)? (a-b):(ops&(1<<and)? (a&b):(0)))))))

//自动生成三类函数定义
#define FLAG_Operater(flag,mark)\
int flag##_get(int* mark){\
return GetFlags(mark,1<<flag);\
}\
void flag##_set(int* mark){\
	SetFlags(mark,1<<flag);\
}\
void flag##_clr(int* mark){\
	ClrFlags(mark,1<<flag);\
}

//反向函数关联
TAG_LIST(FLAG_Operater,mark)

#define SUPER_PRINT(x) \
{\
	char type[60] = {0}; \
	sprintf(type,"[%s] value: %s; type: %s\n",STR(x),FORM(x),COLOR_TYPE(x)); \
	printf(type,x,TYPE(x)); \
}
extern type
type sum(int ops,type _,...){
        va_list var;
	SUPER_PRINT(ops);
        type tot = _;
        va_start(var, _);
        for (type i = va_arg(var,type); (int)i !=0;){
		//SUPER_PRINT(i);
		SUPER_PRINT(tot);
                tot = OPS_SELECT(ops,tot,i);
		i = va_arg(var,type);
		//SUPER_PRINT(i);
		//SUPER_PRINT(tot);
	}
        va_end(var);
        return(tot);
};


struct stu{
    char *name;  //姓名
    int num;  //学号
    int age;  //年龄
    char group;  //所在学习小组
    float score;  //成绩
};
