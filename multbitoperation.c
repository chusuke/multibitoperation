#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*組込型の再定義*/
typedef unsigned char           U8;
typedef signed char             S8;
typedef unsigned short          U16;
typedef signed short            S16;
typedef signed int              S32;
typedef unsigned int            U32;
typedef signed long             S64;
typedef unsigned long           U64;

#define     SHORTSIZE           16
#define     INTSIZE             32
#define     LONGSIZE            64
#define     EVERY2CHAR          (2)               /* every 2 characters */
#define     FIRSTPOS            (0)

#define     ERRORMSG1           "Area reservation failure!"

/*マルチビットデータ型の定義*/
typedef struct multibit {
    U16*                pWord;
    U16                 ByteSize;
    struct multibit*    this;
}MULTIBIT;

/*プロトタイプ宣言*/
MULTIBIT* CreateModule(const S8*);
void setnumber(const S8*, const MULTIBIT*);
void add_multibit(MULTIBIT *, MULTIBIT *, MULTIBIT *);
void sub_multibit(MULTIBIT *, MULTIBIT *, MULTIBIT *);

/* 型のデータ生成*/
MULTIBIT* CreateModule(const S8* numstr)
{
    U16     ByteSize;
    U16*    pWord;
    MULTIBIT* multiword;

    ByteSize = strlen(numstr)/EVERY2CHAR;
    pWord = malloc(ByteSize);
    multiword = malloc(sizeof(MULTIBIT));
    if(multiword != NULL && pWord != NULL) {
        multiword->pWord = pWord;
        multiword->ByteSize = ByteSize;
        multiword->this = multiword;
        memset(pWord, 0x00, ByteSize);
        setnumber(numstr, multiword);
    } else {
        puts(ERRORMSG1);
    }
    return multiword;
}

void setnumber(const S8* numstr, const MULTIBIT* multiword)
{
    S16 pos;
    U16 *pWord;
    S8  temp[5];

    pos = 0;
    pWord = multiword->pWord;
    while(pos < multiword->ByteSize*2)
    {
        strncpy(temp,&numstr[pos], 4);
        temp[4] = '\0';
        sscanf(temp, "%X", pWord);
        pWord++;
        pos += 4;
    }

}

void add_multibit(MULTIBIT *ans, MULTIBIT *ax1,MULTIBIT *ax2)
{

}

void sub_multibit(MULTIBIT *ans, MULTIBIT *ax1, MULTIBIT *ax2)
{
    S16 index;
    U16 wordsize;
    U16 *t_array;
    U32 carry;
    U32 t_ans;

    wordsize  = ax2->ByteSize / 2;
    t_array = malloc(ax2->ByteSize);
    memset(t_array,0x00,ax2->ByteSize);
    for(index = wordsize-1; index >= FIRSTPOS; index--)
    {
        t_array[index] = ~ax2->pWord[index];
    }
    carry = 1;
    for(index = wordsize-1; index >= FIRSTPOS; index--)
    {
        t_ans = t_array[index] + carry;
        t_array[index] = (U16)t_ans & 0xFFFF;
        carry = t_ans >> SHORTSIZE;
    }
    carry = 0;
    for(index = wordsize-1; index >= FIRSTPOS; index--)
    {
        t_ans = ax1->pWord[index] + t_array[index] + carry;
        ans->pWord[index] = (U16)t_ans & 0xFFFF;
        carry = t_ans >> SHORTSIZE;
    }
    free(t_array);
}

/*メインループ*/
int main(void) {
    int i;
    const MULTIBIT *ans = CreateModule("0000000000000000000000000000");
    const MULTIBIT *ax1 = CreateModule("8000000000000000000000000000");
    const MULTIBIT *ax2 = CreateModule("7FFFFFFFFFFFFFFFFFFFFFFFFFFF");

    sub_multibit(ans, ax1, ax2);
    for(i = 0; i <= (ans->ByteSize/2-1); i++)
    {
        printf("%04X", ans->pWord[i]);
    }
    puts("");
    return 0;
}
