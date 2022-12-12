#include"define.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:6031)

extern int boolCnt, clauseCnt, sumOfAns, branchRule;
extern int * ans ;
extern char fileName[100];
extern clause* CNF ;
extern sud S;
int VarTrans(int f, int i, int j, int n) {
	return (f - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + n;
}

status Candig( int x, int y, int z) //修改都在original的基础上
{
	if (S.original[x][y][z] == 0) return ERROR;
	for (int i = 1; i <= 9; i++) //试填除原来数字之外的8个数字，若都不能满足说明可以挖洞
	{
		if (i == S.pattern[x][y][z]) continue;//和原数字相同，则比较下一个数字
		S.original[x][y][z] = i;//给空格处填入一个数字，检查其是否有解
		CreateCnf();
		ReadCNF(CNF, SUDOKU);
		ans = (int*)malloc(sizeof(int) * (boolCnt + 1));
		for (int i = 0; i <= boolCnt; i++)
			ans[i] = 1;
		int res=DPLL(CNF, ans, SUDOKU);
		if (!res) continue;
		else
		{//若能满足，说明不能挖洞，给数独恢复其值
			S.original[x][y][z] = S.pattern[x][y][z];
			return ERROR;
		}
	}
	S.original[x][y][z] = 0;//可以挖洞
	S.num--;
	return OK;
}
void hint() {
	int x, y, z, flag;
	flag = 1;
	srand(time(NULL));
	while (flag) {
		x = rand() % 2 + 1;
		y = rand() % 9 + 1;
		z = rand() % 9 + 1;
		if (!S.original[x][y][z]) {
			S.original[x][y][z] = S.pattern[x][y][z];
			flag = 0;
		}
	}
}
void CreateCnf()
{
	FILE* fp = fopen("sud.cnf", "w");
	int d[9][2] = { {0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2} };
	fprintf(fp, "p cnf %d %d\n", 1458, 24138 + S.num * 9);//双数独 fijn==2*9*9*9=1458   基本要求 162个格子*（1+36）  行约束 18行*（9+36*9） 列约束同理  盒子约束 18个盒子*（9+36*9）等价约束 18*9=162
	int x, y, z, n;
	int f, i, j, count;
	{//将数独游戏所给提示数对应的单子句集录入cnf文件
		for (f = 1; f <= 2; f++)
		{
			for (i = 1; i <= 9; i++) {

				for (j = 1; j <= 9; j++)
				{
					if (S.original[f][i][j]) {
						fprintf(fp, "%d 0\n", VarTrans(f, i, j, S.original[f][i][j]));
						for (x = 1; x <= 9; x++)
						{
							if (x == S.original[f][i][j]) continue;
							fprintf(fp, "%d 0\n", -VarTrans(f, i, j, x));
						}
					}
				}
			}

		}

	}
	{//将基础规则录入cnf文件
		for (f = 1; f <= 2; f++) {
			for (i = 1; i <= 9; i++)//对于9行，每一行有(9*C92+9)个子句
			{
				for (n = 1; n <= 9; n++)//111或121或131……n为填入的数字，9次循环，每循环一次填入一个句子
				{
					for (j = 1; j <= 9; j++)//j为列号，
						fprintf(fp, "%d ", VarTrans(f, i, j, n));
					fprintf(fp, "0\n");
				}
				for (n = 1; n <= 9; n++)//n为填入的数字，9次循环
					for (count = 1; count <= 9; count++)//这里循环输入非111或非121，等，共C92个句子
						for (j = count + 1; j <= 9; j++)
							fprintf(fp, "%d %d 0\n", -VarTrans(f, i, count, n), -VarTrans(f, i, j, n));
			}
		}

		for (f = 1; f <= 2; f++)
		{
			for (j = 1; j <= 9; j++)//对于9列，每一列有(9*C92+9)个子句
			{
				for (n = 1; n <= 9; n++)//111或211或311……n为填入的数字，9次循环，每循环一次填入一个句子
				{
					for (i = 1; i <= 9; i++)//i为行号
						fprintf(fp, "%d ", VarTrans(f, i, j, n));
					fprintf(fp, "0\n");
				}
				for (n = 1; n <= 9; n++)//n为填入的数字，9次循环
					for (count = 1; count <= 9; count++)//这里循环输入非111或非211，等，共C92个句子
						for (i = count + 1; i <= 9; i++)
							fprintf(fp, "%d %d 0\n", -VarTrans(f, count, j, n), -VarTrans(f, i, j, n));
			}
		}
		for (f = 1; f <= 2; f++) {
			for (count = 0; count < 9; count++)//对于9个九宫格，每个格子有(9*C92+9)个子句
			{
				x = 1 + d[count][0] * 3;
				y = 1 + d[count][1] * 3;//每个宫格左上角格子的位置
				for (n = 1; n <= 9; n++)//n为填入的数字
				{
					for (int m = 0; m < 9; m++)
						fprintf(fp, "%d ", VarTrans(f, x + d[m][0], y + d[m][1], n));
					fprintf(fp, "0\n");
				}


				for (n = 1; n <= 9; n++)//n为填入的数字，9次循环
					for (int m1 = 0; m1 < 9; m1++)//控制格子中的9个位置，共C92个句子
						for (int m2 = m1 + 1; m2 < 9; m2++)
							fprintf(fp, "%d %d 0\n", -VarTrans(f, x + d[m1][0], y + d[m1][1], n), -VarTrans(f, x + d[m2][0], y + d[m2][1], n));
			}
		}
		for (f = 1; f <= 2; f++)
		{
			for (i = 1; i <= 9; i++) {
				for (j = 1; j <= 9; j++)//对于81个格子，每个格子有(C92+1)个句子
				{
					for (n = 1; n <= 9; n++)//n为填入的数字
						fprintf(fp, "%d ", VarTrans(f, i, j, n));
					fprintf(fp, "0\n");
					for (n = 1; n <= 9; n++)
						for (count = n + 1; count <= 9; count++)
							fprintf(fp, "%d %d 0\n", -VarTrans(f, i, j, n), -VarTrans(f, i, j, count));
				}
			}
		}

		for (n = 1; n <= 9; n++)
		{
			for (i = 7; i <= 9; i++)
			{
				for (j = 7; j <= 9; j++)
				{
					fprintf(fp, "%d %d 0\n", -VarTrans(1, i, j, n), VarTrans(2, i - 6, j - 6, n));
					fprintf(fp, "%d %d 0\n", VarTrans(1, i, j, n), -VarTrans(2, i - 6, j - 6, n));
				}
			}
		}
	}

	//将规则录入cnf文件
	fclose(fp);
}
void Sudcreate() {
	int x, y, z, v;
	int f, i, j, n;
	srand(time(NULL));
	x = rand() % 2 + 1;
	y = rand() % 9 + 1;//随机确定数独一个位置的数值
	z = rand() % 9 + 1;
	v = rand() % 9 + 1;
	for (int f = 0; f <= 2; f++)
		for (int i = 0; i <= 9; i++)
			for (int j = 0; j <= 9; j++)
				S.original[f][i][j] = S.pattern[f][i][j] = 0;

	S.num = 0;//初始化数独
	S.original[x][y][z] = v;
	S.num = 1;
	CreateCnf();//生成cnf文件
	ReadCNF(CNF, SUDOKU);
	ans = (int*)malloc(sizeof(int) * (boolCnt + 1));
	for (int i = 0; i <= boolCnt; i++)
		ans[i] = 1;
	DPLL(CNF, ans, SUDOKU);//求解双数独
	for (f = 1; f <= 2; f++)
		for (i = 1; i <= 9; i++)
			for (j = 1; j <= 9; j++)
				for (n = 1; n <= 9; n++)
					if (ans[VarTrans(f, i, j, n)]) S.pattern[f][i][j] = n;
	S.num = 162;
}
void SudPlay()
{
	int choice = 1, diff, op;
	int x, y, z, v;
	while (choice) {
		system("cls");
		printf("请选择难度：\n");
		printf("   1.简单     2.中等     3.困难 \n");
		scanf("%d", &diff);
		printf("正在生成数独，请稍等……");
		Sudcreate();//随机形成一个数独
		for (int f = 1; f <= 2; f++)
			for (int i = 1; i <= 9; i++)
				for (int j = 1; j <= 9; j++)
					S.original[f][i][j] = S.pattern[f][i][j];
		int dignum = 0,xx,yy,zz;//给数独挖洞
		do
		{
			xx = rand() % 2 + 1;
			yy = rand() % 9 + 1;//随机确定数独一个位置的数值
			zz = rand() % 9 + 1;
			Candig(xx, yy, zz); 
			dignum++;
		} while (dignum < diff * 40);
		op = 1;
		while (op)
		{
			system("cls");
			printf("------------sudoku------------\n\n");
			for (int i = 1; i <= 6; i++)
			{
				for (int j = 1; j <= 9; j++)
					printf("%3d", S.original[1][i][j]);
				printf("\n");
			}
			for (int i = 7; i <= 9; i++)
			{
				for (int j = 1; j <= 9; j++)
					printf("%3d", S.original[1][i][j]);
				for (int j = 4; j <= 9; j++)
					printf("%3d", S.original[2][i - 6][j]);
				printf("\n");
			}
			for (int i = 4; i <= 9; i++)
			{
				printf("                  ");
				for (int j = 1; j <= 9; j++)
					printf("%3d", S.original[2][i][j]);
				printf("\n");
			}
			printf("------------------------------\n");
			printf("\n1.填写        2.更多提示\n0.显示答案/退出\n");
			scanf("%d", &op);
			if (op == 2) hint();
			if (op == 1)
			{
				printf("\n输入填写的坐标与待填值:");
				scanf("%d%d%d%d", &x, &y, &z, &v);
				if (!S.original[x][y][z] && v == S.pattern[x][y][z])
				{
					printf("\n填写正确!\n");
					S.original[x][y][z] = S.pattern[x][y][z];
					getchar(); getchar();
				}
				else {
					printf("\n答案错误\n");
					getchar(); getchar();
				}
			}
		}
		for (int i = 1; i <= 6; i++)
		{
			for (int j = 1; j <= 9; j++)
				printf("%3d", S.pattern[1][i][j]);
			printf("\n");
		}
		for (int i = 7; i <= 9; i++)
		{
			for (int j = 1; j <= 9; j++)
				printf("%3d", S.pattern[1][i][j]);
			for (int j = 4; j <= 9; j++)
				printf("%3d", S.pattern[2][i - 6][j]);
			printf("\n");
		}
		for (int i = 4; i <= 9; i++)
		{
			printf("                  ");
			for (int j = 1; j <= 9; j++)
				printf("%3d", S.pattern[2][i][j]);
			printf("\n");
		}
		printf("1.再来一局       0.退出\n");
		scanf("%d", &choice);
	}
}