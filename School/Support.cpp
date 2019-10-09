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

int CloseMySQL(MySQLHostVariable * host)//�ر�MySQL����
{
	mysql_close(&host->mysql);
	return 0;
}

int InitMySQL(MySQLHostVariable * host)	//��ʼ��MySQL����
{	//��ʼMySQL����
	int nConnectTimeout = 5, nWriteReadTimeout = 5;
	char hostip[128];
	int i;
	MYSQL* sap;
	char path[128]; //���·���ı���
	mysql_init(&host->mysql);
	mysql_options(&host->mysql, MYSQL_SET_CHARSET_NAME, "gb2312");//����Ҫ�ڱ�����ȫ�ֶ��� 
	mysql_options(&host->mysql, MYSQL_OPT_CONNECT_TIMEOUT, &nConnectTimeout);
	mysql_options(&host->mysql, MYSQL_OPT_READ_TIMEOUT, &nWriteReadTimeout);
	mysql_options(&host->mysql, MYSQL_OPT_WRITE_TIMEOUT, &nWriteReadTimeout);
	GetModuleFileName(NULL, path, sizeof(path));     //��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	memcpy(path + strlen(path) - 4, ".INI", 5);
	GetPrivateProfileString("MySQL", "MySQL", "", hostip, sizeof(hostip), path);
	if (ValidIP(hostip) != TRUE) {	//IP��ַ�Ƿ�����127.0.0.1���沢дINI�ļ�
		strcpy_s(hostip, sizeof(hostip), "127.0.0.1");
		WritePrivateProfileStringA("MySQL", "MySQL", hostip, path);
	}
	sap = mysql_real_connect(&host->mysql, hostip, MYSQLUSER, MYSQLPWD, MYSQLDBS, 3306, NULL, 0);

	if (sap == NULL)
		return FALSE;
	i = sizeof(host->host);
	strcpy_s(host->host, i, hostip);
	return TRUE;
}

int PwdCode(char *s1, char *s2) {	//�����ַ���������ת����long
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