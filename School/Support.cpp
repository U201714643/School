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
			l = l ^ (l << 5);		//尽量填满32位
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

char * RealChar(char * str) {		//转换*、/为×、÷便于显示
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
			int length = strlen(t);	//字符串t长度
			t[length] = *p;		//即在字符串末尾添加
			t[length + 1] = 0;	//防止字符串尾部的0被破坏
		}
		p++;
	}
	strcpy_s(str, EXPLEN, t);	//因字符串t为局部变量，在返回后会丢失，故复制到字符串str中
	return str;	//返回复制后的字符串首地址str
}

int IntFormEidt(CEdit * Edit) {	//从文本框中获得数字
	char buf[128];	//保存字符串形式的数字
	Edit->GetWindowTextA(buf, sizeof(buf));	//获得字符串形式的数字
	if (strlen(buf) == 0)
		return -1;	//我们的程序正常情况下无负数
	return atoi(buf);
}