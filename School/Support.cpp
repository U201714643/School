#include "stdafx.h"
#include "Support.h"
#include "include\mysql.h"

char * MsToTime(char * ms, char * Src) {		//转换毫秒单位的时间为分：秒制
	int s = atoi(ms) / 1000;	//用秒表示
	if (s == 0) {	//用时过短，可能是某处出错
		sprintf_s(Src, 32, "采样错误");
	}
	else {	//s/60得到分钟数，s%60得到秒数
		sprintf_s(Src, 32, "%d:%d", s / 60, s % 60);
	}
	return Src;
}

int PwdCode(char *s1, char *s2) {	//用户名+密码编码
	int i, j;
	long l;
	for (l = 0, j = 0; s1[j] != 0; j++) {
		for (i = 0; i < 3; i++)
			l = l ^ (l << 5);
		l = l ^ s1[j];
	}
	for (j = 0; s2[j] != 0; j++) {
		for (i = 0; i < 3; i++)
			l = l ^ (l << 5);
		l = l ^ s2[j];
	}
	return l;
}

int ValidIP(char * str)	//校验IP地址合法性
{
	int a, b, c, d;
	int ret = TRUE;
	ret = sscanf_s(str, "%d.%d.%d.%d", &a, &b, &c, &d);
	if (ret == 4 && (a >= 1 && a <= 255) && (b >= 0 && b <= 255) && (c >= 0 && c <= 255) && (d >= 1 && d <= 254))
		return TRUE;
	else
		return FALSE;
}

char * RealChar(char * str) {		//转换*、/符号为×、÷
	char * p = str;
	while (*p) {	//逐个查找、替换
		if (*p == '*') {
			char * t = p;
			return 0;
		}
	}
}