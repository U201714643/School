#include "pch.h"
#include "mylib.h"
#include <iostream>

//由于教材内容限制，这些参数都是固定的，故直接设计为常数
#define ChuShuMax 9
#define ChuShuMin 2

const int StrLenMax = strlen("89/9") + 1;	//生成的字符串的最大长度

int LeftGen(ExpressionNode * Address, int Num) {	//生成带余数的除法算式
	for (int i = 0; i < Num; i++) {
		//-------生成算式数字-------
		int ChuShu = MyRnd(ChuShuMin, ChuShuMax);	//先生成除数，这里就避免了除以0的现象
		int BeiChuShu = MyRnd(ChuShu + 1, ChuShu * 9);	//根据除数产生被除数，被除数至少比除数大1，并保证结果在1~9之间
		//-------生成算式字符串-------
		sprintf_s(Address[i].Expression, StrLenMax, "%d/%d", BeiChuShu, ChuShu);
		//-------检验算式是否合理（有余数）、是否重复-------
		while (BeiChuShu % ChuShu == 0 || RepeatCheak(Address, i) == ERROR) {
			//-------先重新生成数字-------
			ChuShu = MyRnd(ChuShuMin, ChuShuMax);	
			BeiChuShu = MyRnd(ChuShu + 1, ChuShu * 9);
			//-------才能再次生成字符串-------
			sprintf_s(Address[i].Expression, StrLenMax, "%d/%d", BeiChuShu, ChuShu);
		}
		//-------生成结果-------
		Address[i].Left = BeiChuShu % ChuShu;	//余数
		Address[i].Value = BeiChuShu / ChuShu;	//商
	}
	return 0;
}