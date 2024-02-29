#include <stdio.h>
#include <string.h>



struct poker{
    char cards[16];
    char card[6][6];
    int type;
    int value;
}cardA,cardB;

char *types[15] = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "joker", "JOKER"};


void valuedec(struct poker* tmp){
    if(strstr(tmp->cards,"joker JOKER") || strstr(tmp->cards,"JOKER joker")){
        tmp->type = 0;
        return;
    }
    for(int i =0;i<tmp->type;i++){
        for (int j= 0; j < 15; j++) {
            if (strcmp(tmp->card[i], types[j]) == 0) 
                tmp->value+=j;
        }
    }
}

int main() {

    // 输入两手牌
    scanf("%[^-]", cardA.cards);
    getchar(); // 消耗换行符
    scanf("%[^\n]", cardB.cards);

    cardA.type = sscanf(cardA.cards,"%s%s%s%s%s", cardA.card[0], cardA.card[1], cardA.card[2], cardA.card[3], cardA.card[4]);
    cardB.type = sscanf(cardB.cards,"%s%s%s%s%s", cardB.card[0], cardB.card[1], cardB.card[2], cardB.card[3], cardB.card[4]);
    valuedec(&cardA);
    valuedec(&cardB);
    if(cardA.type == cardB.type)
        printf("%s\n",cardA.value > cardB.value?cardA.cards:cardB.cards);
    else if(cardA.type == 0 || cardB.type == 0 )
       printf("%s\n",cardA.type == 0?cardA.cards:cardB.cards);
    else if(cardA.type == 4 || cardB.type == 4 )
        printf("%s\n",cardA.type == 4?cardA.cards:cardB.cards);
    else
        printf("ERROR");
    return 0;
}
