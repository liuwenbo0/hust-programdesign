#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define OK 1
#define ERROR 0
#define SUDOKU 1
#define SAT 0

typedef struct node           //保存文字
{
    int value;                //文字名，小于0表示其在句子中的带有非
    node* next;               //下一变量
} node;
typedef struct clause           //保存子句
{
    int num;                    //标记子句中文字的个数
    node* head;                 //指向下一个文字
    clause* next;              //指向下一个句子
} clause;
typedef struct sudproblem {     //数独问题
    int original[3][10][10];       //记录原始数值
    int pattern[3][10][10];        //0表示挖出的洞，其他情况同ori
    int num;                    //非0个数
}sud;
typedef int status;

int ABS(int a);                                                   //辅助计算函数
int read();                                                       //快读
status ReadCNF(clause*& cnf, int satORsudoku);                    //读取cnf文件
void destroyClause(clause*& cnf);                                 //删除SAT问题
void removeClause(clause*& cnf, clause*& cl);                     //删除子句
void removeNode(clause* cl, node*& nd);                           //删除文字
status addClause(clause* cl, clause*& cnf);                       //加入单子句
bool isUnitClause(clause* cl);                                    //子句判断是否为单子句
void delelteSingleClause(clause* s, clause*& cnf);                //单子句规则
bool emptyClause(clause* cnf);                                    //子句判空
void getForgery(clause*& forgery, clause* cnf);                   //获取副本
int getMaxBool(clause* cnf);                                      //选取出现次数最多的变元
int getMaxBool_2(clause* cnf);                   //选取总出现次数最多的变元
int getMaxBool_Sudoku(clause* cnf);              //针对数独的选择出现次数最多的变元的算法
int getNextBool_3(clause* cnf);                  //选取在短子句总出现次数最多的变元
int getNextBool_4(clause* cnf);                  //选取在短子句出现次数最多的变元
bool DPLL(clause*& cnf, int* v, int satORsudoku);                 //DPLL算法
status printCNF(int s, int* v, double t);                         //创建res文件

int VarTrans(int f, int i, int j, int n);      //变量转换函数，将语义编码转换为自然顺序编码，
                                                       //输入变量名i行j列填入数字n
void SudPlay();                                        //可玩数独
void Sudcreate();                                      //生成一个可以玩的数独
status Candig( int x, int y, int z);                   //判断是否为可以挖洞
void hint();                                           //给出提示
void CreateCnf();                                      //生成cnf文件
