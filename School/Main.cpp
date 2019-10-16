#include "pch.h"
#include "mylib.h"
#include <iostream>

char * RealChar(char * str) {		//转换*、/符号为×、÷
	char t[EXPLEN] = { 0 };
	char * p = str;
	while (*p) {	//逐个对比并写入新字符串
		if (*p == '*') {
			strcat_s(t, "×");
		}
		else if (*p == '/') {
			strcat_s(t, "÷");
		}
		else {
			//非×、÷字符保持原样
			int length = strlen(t);
			t[length] = *p;		//即在字符串末尾添加
			t[length + 1] = 0;	//防止字符串尾部的0被破坏
		}
		p++;
	}
	strcpy_s(str, 888, t);	//因字符串t为局部变量，在返回后会丢失，故复制到字符串str中
	return str;	//返回复制后的字符串首地址str
}

int main()
{
	Initial();	//初始化包括重置种子等
	
	/*ExpressionNode Exp[EXPNUM] = { 0 };	//用于存放将要生成的表达式
	LeftGen(Exp, 20);*/
	ExpressionNode Exp[EXPNUM] = { 0 };	//用于存放将要生成的表达式
	ExpressionList Arguement;	//生成参数
	Arguement.EqualsNum = 300;
	Arguement.KuoHao = FLASE;
	Arguement.Repeat = FLASE;
	Arguement.NumMax = 5;
	Arguement.NumMin = 1;
	Arguement.OpCount = 1;
	Arguement.OpTypeMax = 1;
	Arguement.OpTypeMin = 0;
	Arguement.ResultMax = 5;
	Arguement.ResultMin = 1;
	TestGen(Exp, &Arguement);
	printf("输入文件名:");
	char p1[200] = "C:\\Users\\DEAD\\Desktop\\题库\\";
	char p2[200];
	gets_s(p2,200);
	strcat_s(p1, p2);
	strcat_s(p1, ".txt");
	puts(p1);
	FILE * fp = 0;
	fopen_s(&fp, p1, "w");
	for (int i = 0; i < Arguement.EqualsNum; i++) {
		fprintf(fp,"%s=%d\n", Exp[i].Expression, Exp[i].Value);
	}
	/*char t[EXPLEN] = "1*2=";
	RealChar(t);*/
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
