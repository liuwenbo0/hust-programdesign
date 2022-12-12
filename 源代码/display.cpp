#include"define.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 6031)

int inf = 1061109567;
int boolCnt, clauseCnt, sumOfAns, branchRule;//变元数量 子句数量  选择策略
char fileName[200];  //cnf文件文件名
int *ans=NULL; //结果数组
sud S; //数独问题
clause* CNF=NULL; //SAT问题
int main()
{
    
    int op = 1, op1 = 1, op2 = 1, flag_cnf, difficulty;
    clock_t start, finish;
    double time;
    bool res;
    while (op)
    {
        system("cls");
        printf("                               菜单                                  \n");
        printf("------------------------------------------------------------------------\n");
        printf("                     1.DPLL     2.双数独    0.退出                       \n");
        printf("------------------------------------------------------------------------\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            flag_cnf = 1, op1 = 1;
            while (op1)
            {
                system("cls");
                printf("-------------------------------------------------------------------------\n");
                printf("1.读文件  2.打印文件  3.DPLL  4.输出并保存结果   0.返回\n");
                printf("-------------------------------------------------------------------------\n");
                scanf("%d", &op1);
                switch (op1)
                {
                case 1:
                    printf("请输入测试文件名:");
                    scanf("%s", fileName);
                    if (ReadCNF(CNF, SAT) == OK)
                        flag_cnf = 0, printf("cnf文件读取完毕！\n");
                    else
                        printf("cnf文件读取失败!\n");
                    getchar(), getchar();
                    break;
                case 2:
                    if (flag_cnf)
                        printf("尚未未读取cnf文件!\n ");
                    else
                    {
                        printf("文件中有如下子句：\n");
                        printf("___________________________\n");
                        for (clause* lp = CNF; lp; lp = lp->next)
                        {
                            for (node* tp = lp->head; tp; tp = tp->next)
                                printf("%d ", tp->value);
                            printf("\n");
                        }
                    }
                    getchar(), getchar();
                    break;
                case 3:
                    if (flag_cnf)
                        printf("尚未未读取cnf文件!\n ");
                    else
                    {   branchRule = 1;
                        ans = (int*)malloc(sizeof(int) * (boolCnt + 1));
                        for (int i = 0; i <= boolCnt; i++)
                            ans[i] = 1;
                        start = clock();
                        res = DPLL(CNF, ans, SAT);
                        finish = clock();
                        if (!res)
                            printf("不可满足！\n");
                        else
                        {
                            printf("可满足！\n");
                        }
                        time = (double)(finish - start);
                        printf("DPLL过程优化后用时%lfms\n", time );
                        destroyClause(CNF);
                        ReadCNF(CNF, SAT);
                        branchRule = 4; 
                        for (int i = 0; i <= boolCnt; i++)
                            ans[i] = 1;
                        start = clock();
                        res = DPLL(CNF, ans, SAT);
                        finish = clock();
                        time = (double)(finish - start);
                        printf("DPLL过程优化前用时%lfms\n", time );
                    }
                    getchar(), getchar();
                    break;
                case 4:
                    for (int i = 1; i <= boolCnt; i++)
                    {
                        if (ans[i])
                            printf("%d ", i);
                        else
                            printf("%d ", -i);
                    }
                case 5:
                    if (flag_cnf)
                        printf("尚未未读取cnf文件!\n ");
                    else
                    {
                        if (printCNF(res, ans, time))
                            printf("结果已保存\n");
                        else
                            printf("结果保存失败!\n");
                    }
                    getchar(), getchar();
                    break;
                case 0:
                    break;
                }
            }
            break;
        case 2:
             op2 = 1;
            while (op2)
            {
                system("cls");
                printf("------------------------------------------------------------------------\n");
                printf("                   1.数独游戏   0.返回                            \n");
                printf("------------------------------------------------------------------------\n");
                scanf("%d", &op2);
                switch (op2)
                {
                case 1:
                    SudPlay();
                    getchar();
                    break;
                default:
                    break;
                }
            }
            break;
        case 0:
            break;
        }
    }
    return 0;
}