#ifndef _LOG_h_
#define _LOG_h_
#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>

#define COL(x)  "\033[;" #x "m"
#define LOC    "\033[m"
#define RED(x)     COL(31) x LOC
#define GREEN(x)   COL(32) x LOC
#define YELLOW(x)  COL(33) x LOC
#define BLUE(x)    COL(34) x LOC
#define MAGENTA(x) COL(35) x LOC
#define CYAN(x)    COL(36) x LOC
#define WHITE(x)   COL(37) x LOC
#endif

#define STR(x) #x
typedef int type;
typedef type (*fun_t)(type,...);

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

#define CALL_BACK(var,x,...) _Generic((var),\
    int(*)(int,...):var(x,##__VA_ARGS__,0),\
    float(*)(float,...):var(x,##__VA_ARGS__,0),\
    double(*)(double,...):var(x,##__VA_ARGS__,0),\
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

#define OPS_SELECT(ops,a,b) ((ops)&tim?((ops)&div?((ops)&odd?((ops)&add?((ops)&mil?((ops)&and?(0):(a&b)):(a-b)):(a+b)):(a%b)):(a/b)):(a*b))



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
	sprintf(type,"%s value: %s ; %s type: %s\n",STR(x),FORM(x),STR(x),COLOR_TYPE(x)); \
	printf(type,x,TYPE(x)); \
}


type sum(type _,...){
        va_list var;
	//TYPEDEF(_);
        type tot = _;
        va_start(var, _);
        for (type i = va_arg(var,type); (int)i !=0;){
                tot += i;
		i = va_arg(var,type);
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

int main(int argc, char const *argv[])
{
    int x = 9;
    fun_t fuc = sum;
    char s[10] = "ljxnb";
    char *p = s;
    //位定义变量
    int OpsFlag=0x00000000;
    div_set(&OpsFlag);
    printf("set:%X\r\n",OpsFlag);
    struct stu stu1 = {"ljx",23,18,2,100}; 
    //SUPER_PRINT(0,"ljx:%d",x);
    type num = CALL_BACK(fuc,23,334,23);
    printf(FORM(num),num);
    //SUPER_PRINT(stu1.name);
    //SUPER_PRINT(fuc);
    //SUPER_PRINT(x1);
    //debug(x,s);
    return 0;
}
