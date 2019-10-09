#include "stdafx.h"
#include "Support.h"
#include "include\mysql.h"


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
	memcpy(path + strlen(path) - 4, ".ini", 5);
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