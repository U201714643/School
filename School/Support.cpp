#include "stdafx.h"
#include "Support.h"
#include "include\mysql.h"

char * MsToTime(char * ms, char * Src) {		//ת�����뵥λ��ʱ��Ϊ�֣�����
	int s = atoi(ms) / 1000;	//�����ʾ
	if (s == 0) {	//��ʱ���̣�������ĳ������
		sprintf_s(Src, 32, "��������");
	}
	else {	//s/60�õ���������s%60�õ�����
		sprintf_s(Src, 32, "%d:%d", s / 60, s % 60);
	}
	return Src;
}

int PwdCode(char *s1, char *s2) {	//�û���+�������
	int i, j;
	long l;
	for (l = 0, j = 0; s1[j] != 0; j++) {
		for (i = 0; i < 3; i++)
			l = l ^ (l << 5);		//��������32λ
		l = l ^ s1[j];
	}
	for (j = 0; s2[j] != 0; j++) {
		for (i = 0; i < 3; i++)
			l = l ^ (l << 5);
		l = l ^ s2[j];
	}
	return l;
}

int ValidIP(char * str)	//У��IP��ַ�Ϸ���
{
	int a, b, c, d;
	int ret = TRUE;
	ret = sscanf_s(str, "%d.%d.%d.%d", &a, &b, &c, &d);
	if (ret == 4 && (a >= 1 && a <= 255) && (b >= 0 && b <= 255) && (c >= 0 && c <= 255) && (d >= 1 && d <= 254))
		return TRUE;
	else
		return FALSE;
}

char * RealChar(char * str) {		//ת��*��/Ϊ�����±�����ʾ
	char t[EXPLEN] = { 0 };
	char * p = str;
	while (*p) {	//����ԱȲ�д�����ַ���
		if (*p == '*') {
			strcat_s(t, "��");
		}
		else if (*p == '/') {
			strcat_s(t, "��");
		}
		else {
			//�ǡ������ַ�����ԭ��
			int length = strlen(t);	//�ַ���t����
			t[length] = *p;		//�����ַ���ĩβ���
			t[length + 1] = 0;	//��ֹ�ַ���β����0���ƻ�
		}
		p++;
	}
	strcpy_s(str, EXPLEN, t);	//���ַ���tΪ�ֲ��������ڷ��غ�ᶪʧ���ʸ��Ƶ��ַ���str��
	return str;	//���ظ��ƺ���ַ����׵�ַstr
}

int IntFormEidt(CEdit * Edit) {	//���ı����л������
	char buf[128];	//�����ַ�����ʽ������
	Edit->GetWindowTextA(buf, sizeof(buf));	//����ַ�����ʽ������
	if (strlen(buf) == 0)
		return -1;	//���ǵĳ�������������޸���
	return atoi(buf);
}