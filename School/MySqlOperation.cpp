#include "stdafx.h"
#include "Support.h"
#include "include\mysql.h"

extern global gs;	//��¼����Ϣ

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

int VerifyOperator(char * name, char * password) {	//У���û�
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	long pwd;
	int sta;	//״̬��־
	char buf[200];
	int i;
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		pwd = PwdCode(name, password);	//����
		sprintf_s(buf, sizeof(buf), "Select operator.`ID`,`Grade`,`No`,`Right`, grade.`Name` From `operator` "
			"Left Join Grade ON operator.grade = grade.ID Where `User`='%s' And `Password`='%ld';", name, pwd);
		mysql_query(&host.mysql, buf);	//�������ݿ��ѯ
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			i = (long)result->row_count;//����
		else
			i = 0;
		if (i == 1) {
			//У��ͨ�������û����ݶ���ȫ�ֱ���gs.op��
			row = mysql_fetch_row(result);
			gs.op.ID = atoi(row[0]);	//�ڲ�ID
			gs.op.GradeID = atoi(row[1]);	//�༶
			strcpy_s(gs.op.No, sizeof(gs.op.No), row[2]);	//row�����޸�
			gs.op.Right = atoi(row[3]);	//Ȩ��
			strcpy_s(gs.op.Name, sizeof(gs.op.Name), name);
			if (row[4] == NULL)	//�༶���ƿ�����Ч�������ʦû�������༶
				gs.op.GradeName[0] = 0;	//��ʱ�򽫰༶��������Ϊ""�ַ���
			else
				strcpy_s(gs.op.GradeName, sizeof(gs.op.GradeName), row[4]);
			gs.op.Pwd = pwd;	//����
		}
		else
			sta = FALSE;
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
	};
	return sta;
}