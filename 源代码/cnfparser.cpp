#include"define.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:6031)

const int inf = 1061109567;
extern int boolCnt, clauseCnt, sumOfAns, branchRule;
extern char fileName[100];
extern clause* CNF ;
int ABS(int a)
{
    return a > 0 ? a : -a;
}
int read(FILE* fp)
{
    int flag = 0;
    int x = 0, f = 1;
    char c;
    c = getc(fp);        //读入字符
    while (c < '0' || c > '9')     //非数字
    {
        if (c == '-')
            f = -1, c = getc(fp);
        else
            return inf;           
    }
    while (c >= '0' && c <= '9')    //读入数字
    {
        x = x * 10 + c - '0';
        c = getc(fp);
        if (c == -1)    //文件结束
            flag = 1;
    }
    //跳出循环时c应为空格 换行 或者EOF
    if (!flag && c != ' ' && c != '\n')  
        return inf;
    return x * f;    //返回读入数字的值
}
status ReadCNF(clause*& cnf, int satORsudoku)
{
    FILE* fp;
    char ch;
    node* pn;
    clause* pc;
    char check[5] = { ' ', 'c', 'n', 'f', ' ' };
    if (satORsudoku == SAT)
        fp = fopen(fileName, "r");
    else
        fp = fopen("sud.cnf", "r");
    if (!fp)
        return ERROR;
    while ((ch = getc(fp)) == 'c')  
        while ((ch = getc(fp)) != '\n')  //当行首为c时读到换行符
            continue;
    //跳出循环时ch应为‘p’
    if (ch != 'p')
        return ERROR;   
    for (int i = 0; i < 5; i++)
        if ((ch = getc(fp)) != check[i])  //读取‘p cnf ’部分
            return ERROR;
    if ((boolCnt = read(fp)) == inf || (clauseCnt = read(fp)) == inf)//读入变元数和子句数
        return ERROR;
    cnf = (clause*)malloc(sizeof(clause));
    cnf->next = NULL;
    cnf->head = (node*)malloc(sizeof(node));
    cnf->head->next = NULL;
    cnf->num = 0;
    pc = cnf;  //pc是一个指向子句的指针
    pn = cnf->head;   //pn是一个指向文字的指针
    //初始化问题
    for (int i = 1; i <= clauseCnt; i++)
    {
        int dat;
        if ((dat = read(fp)) == inf)
            return ERROR;
        while (dat)//dat不是终止符0时，循环读取后面的数据
        {
            pc->num++;   
            pn->value = dat;
            pn->next = (node*)malloc(sizeof(node));
            if ((dat = read(fp)) == inf)
                return ERROR;
            if (!dat)
                pn->next = NULL;
            pn = pn->next;
        }
        pc->next = (clause*)malloc(sizeof(clause));
        pc->next->num = 0;
        pc->next->head = (node*)malloc(sizeof(node));
        if (i == clauseCnt)
        {
            pc->next = NULL;
            break;
        }
        pc = pc->next;
        pn = pc->head;
    }
    //读取cnf文件
    fclose(fp);
    return OK;
}
status printCNF(int s, int* v, double t)
{
    FILE* fp;
    for (int i = 0; fileName[i] != '\0'; i++)
        if (fileName[i] == '.' && fileName[i + 4] == '\0')
        {
            fileName[i + 1] = 'r', fileName[i + 2] = 'e', fileName[i + 3] = 's';
            break;
        }
    if (!(fp = fopen(fileName, "w+")))//创建res文件
    {
        printf("文件创建失败!\n");
        return ERROR;
    }
    fprintf(fp, "s %d\nv ", s);
    if (s)//如果可满足
        for (int i = 1; i <= boolCnt; i++)//写入结果
        {
            if (v[i])
                fprintf(fp, "%d ", i);
            else
                fprintf(fp, "%d ", -i);
        }
    fprintf(fp, "\nt %lf", t * 1000);
    fclose(fp);
    return OK;
}