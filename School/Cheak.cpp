#include "stdafx.h"
#include "mylib.h"
#include <iostream>

//整数判断结果
#define NotInt 0
#define IsInt 1

int IntCheak(double x);				//检测目标浮点数是否为整数

int AnsCheak(char * Expression, int ResultMin, int ResultMax) {	//检测表达式是否合理（无法整除、为负数、超出范围）
	double Sum = ExpSum(Expression);	//计算
	if (Sum == FERROR)
		return ERROR;	//除以0
	if (Sum  < ResultMin || Sum > ResultMax)
		return ERROR;	//结果超出范围
	if (Sum == 0 && MyRnd(1, 10) != 2)
		return ERROR;	//减少值为0的式子
	if (IntCheak(Sum) == NotInt)
		return ERROR;	//结果不为整数
	return OK;
}

int KuoHaoCheak(int NumLeft, int NumRight, int OpCount, NumNode * num) {	//检测括号是否合理
	if (NumLeft >= NumRight)
		return ERROR;	//右括号必须出现在左括号右侧
	if ((num[NumLeft].KuoHao > 0) || (num[NumRight].KuoHao < 0))
		return ERROR;	//同一个数字不能同时被左右括号包围
	if (NumLeft == 0 && NumRight == OpCount)
		return ERROR;	//括号不能直接包含整个算式
	if (num[NumLeft].KuoHao == -num[NumRight].KuoHao && num[NumLeft].KuoHao < 0)	//无括号无此问题
		return ERROR;	//不能出现双重括号
	return OK;
}

int RepeatCheak(ExpressionNode * Address, int Num) {	//检测算式是否重复
	for (int i = 0; i < Num; i++) {
		if (strcmp(Address[i].Expression, Address[Num].Expression) == 0) {
			return ERROR;	//字符串相等，出现重复
		}
	}
	return OK;
}

int IntCheak(double x) {	//检测目标浮点数是否为整数
	if ((x - (int)x) == 0)
		return IsInt;		//二者相等即为整数
	return NotInt;
}