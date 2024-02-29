#include <stdio.h>
#include <string.h>

struct recordline{
    char name[128];
    int line;
    int num;
};

int main() {
    int i=0,index = 0;
    char name[128] = {0};
    struct recordline tmp ={{0},0,1};
    struct recordline record[128] = {0};
    while (scanf("%s %d", name, &tmp.line) == 2) { // 注意 while 处理多个 case
        char* sec = strtok(name, "\\"),*old=NULL;
        while (sec) {
            old = sec;
            sec = strtok(NULL, "\\");
        }
        strcpy(tmp.name, old);
        for(i = 0;i<index+1;i++){
            if(tmp.line == record[i].line && strcmp(tmp.name,record[i].name)==0){
                record[i].num++;
                index--;
                break;
            }else if(i == index)
                record[i] = tmp;
        }
        index++;
    }
    for(i = index - 8 > 0 ? index - 8 : 0; i < index; i++)
        printf("%s %d %d\n",&record[i].name[(int)strlen(record[i].name) - 16 >0?strlen(record[i].name)-16:0],record[i].line,record[i].num);
    
    return 0;
}
