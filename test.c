#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CH Ch;
struct CH {
    char* ch;
    Ch* next;
    /* data */
}* header, *ptr;

void nodelink(Ch* sec, int index, char* str) {
    ptr = &sec[index];
    *ptr = (Ch) {
        str, header
    };
    header = ptr;
}

int main() {
    char number[][4] = {"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};
    char unit[][4] = {"分", "角", "元", "拾", "佰", "仟", "万", "拾", "佰", "仟", "亿"};
    int num = 0;
    double value;
    int zero = 0;
    int posnum = 0;
    while (scanf("%lf", &value) != EOF) {
        Ch sec[128] = {0};
        int index = 0;
        int position = 0;
        num = (value + 0.005) * 100;
        zero = 0x02;
        while (num) {
            if (!header && position > 1)
                nodelink(sec, index++, "整");
            posnum = num % 10;
            if ((0x01 & zero) || posnum)
                nodelink(sec, index++, unit[position]);
            switch (posnum) {
                case 0:
                    if (!zero && position > 1)
                        nodelink(sec, index++, number[posnum]);
                    break;
                case 1:
                    if (position != 3 && position != 7)
                        nodelink(sec, index++, number[posnum]);
                    break;
                default:
                    nodelink(sec, index++, number[posnum]);
            }
            num /= 10;
            position++;
            if (posnum)
                zero &= 0xfd;
            else
                zero |= 0x02;
            if ((position + 2) % 4 == 0)
                zero |= 0x01;
            else
                zero &= 0xfe;
        }
        printf("人民币");
        while (header) {
            ptr = header->next;
            printf("%s", header->ch);
            header = ptr;
        }
        printf("\n");
    }
    return 0;
}
