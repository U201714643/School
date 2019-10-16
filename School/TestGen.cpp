#include "stdafx.h"
#include "mylib.h"
#include <iostream>

int LengthOfInt(int x);		//整数的十进制长度
int StringGen(char * Expression, int OpCount, int SprintfLengthMax, NumNode * num, int * op);	//生成字符串形式的表达式

int TestGen(ExpressionNode * Address, ExpressionList * Arguements)	//地址调用、提高效率
{	//生成表达式
	int TotalTry = 0;	//总尝试次数，用于防止生成超过理论数目的算式
	int SprintfLengthMax = LengthOfInt(Arguements->NumMax) + 1;	//会生成的最大整数长度
	for (int i = 0; i < Arguements->EqualsNum; i++) {	//生成算式的数目
		if (++TotalTry > TOTALTRYMAX) {
			return ERROR;	//尝试生成超过理论数目的表达式
		}
		NumNode * num;	//用于存放数字
		int * op;		//用于存放操作符
		//---------空间分配---------
		if ((num = (NumNode *)malloc(sizeof(NumNode)*(Arguements->OpCount + 2))) == NULL) {
			//数字数应比操作符数（不含括号）多1
			exit(OVERFLOW);		//空间不足，退出
		}
		if ((op = (int *)malloc(sizeof(int)*(Arguements->OpCount + 1))) == NULL) {
			exit(OVERFLOW);		//空间不足，退出
		}
		//---------生成操作符和操作数---------
		for (int j = 0; j < Arguements->OpCount + 1; j++) {
			num[j].Num = MyRnd(Arguements->NumMin, Arguements->NumMax);	//随机生成[NumMin,NumMax]内整数
			num[j].KuoHao = 0;	//最初显然无括号
		}
		for (int j = 0; j < Arguements->OpCount; j++) {
			op[j] = MyRnd(Arguements->OpTypeMin, Arguements->OpTypeMax);//随机生成运算符
		}
		//---------生成括号---------
		if (Arguements->KuoHao == TRUE) {
			int KuoHaoNum = MyRnd(1, Arguements->OpCount / 2 + 1);	//括号数目
			for (int j = 0; j < KuoHaoNum; j++) {
				int NumLeft = MyRnd(0, Arguements->OpCount);	//添加左括号的位置
				int NumRight = MyRnd(0, Arguements->OpCount);	//添加右括号的位置
				if (KuoHaoCheak(NumLeft, NumRight, Arguements->OpCount, num) == ERROR) {
					j--;	//不能为同一个数同时添加左、右括号,左括号还必须位于右括号的左侧（一次生成中）,也不能给最外层添加括号
					continue;
				}
				num[NumLeft].KuoHao--;	//添加左括号
				num[NumRight].KuoHao++;	//添加右括号
			}
		}
		//---------生成字符串形式的表达式---------
		StringGen(Address[i].Expression, Arguements->OpCount, SprintfLengthMax, num, op);
		//---------检测算式合理性（是否为整数、有无除0、越界、重复现象）---------
		int TryTime = 0;
		for (; TryTime < TRYMAX; TryTime++) {	//设置最大尝试次数，防止生成时间过长
			int Reset = 0;	//是否重新生成
			if (Arguements->Repeat == FLASE && RepeatCheak(Address, i) == ERROR)
				Reset++;
			if (AnsCheak(Address[i].Expression, Arguements->ResultMin, Arguements->ResultMax) == ERROR)
				Reset++;
			if(Reset) {	//为使算式结构尽可能随机分布，再次随机只改变数字
				for (int j = 0; j < Arguements->OpCount + 1; j++) {
					num[j].Num = MyRnd(Arguements->NumMin, Arguements->NumMax);	//随机生成[NumMin,NumMax]内整数
				}
				//---------生成字符串形式的表达式---------
				StringGen(Address[i].Expression, Arguements->OpCount, SprintfLengthMax, num, op);
			}	
			else break;	//无需重新生成即可结束生成
		}
		if (TryTime == TRYMAX) {
			i--;//如仅尝试修改数字无法使算式合理，则本次生成的算式结构作废
		}
		else {	//对合理的算式，则为它赋值
			Address[i].Value = ExpSum(Address[i].Expression);
		}
		//---------释放空间---------
		free(op);
		free(num);
	}
	return OK;
}

int LengthOfInt(int x) {		//整数的十进制长度
	int l = 0;
	while (x) {
		//每次减少一个十进制位
		x = x / 10;
		l++;
	}
	return l;
}

int StringGen(char * Expression, int OpCount, int SprintfLengthMax, NumNode * num, int * op) {	//生成字符串形式的表达式
	char *t = Expression;	//获得字符串首地址
	for (int j = 0; j < OpCount + 1; j++) {
		int step = 0;	//记录sprinf函数造成的指针移动
		int KuoHaoNum = num[j].KuoHao;	//括号数，为保证再次生成时结构不被改变，不能直接修改num中信息
		//---------生成左侧括号---------
		while (KuoHaoNum < 0) {
			*t = '(';
			KuoHaoNum++;//更新标记
			t++;	//移动指针	
		}
		//---------生成数字---------
		step = sprintf_s(t, SprintfLengthMax, "%d", num[j].Num);
		t += step;	//移动指针	
		//---------生成右侧括号---------
		while (KuoHaoNum > 0) {
			*t = ')';
			KuoHaoNum--;//更新标记
			t++;	//移动指针	
		}
		//---------生成算符---------
		if (j != OpCount) {	//最后无需生成算符
			*t = OpTxet[op[j]];
			t++;	//移动指针
		}
		*t = 0;	//封闭字符串
	}
	return OK;
}