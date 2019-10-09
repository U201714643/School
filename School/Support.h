#include "include\mysql.h"

#define  MYSQLUSER "root"
#define  MYSQLPWD "19990220"
#define  MYSQLDBS "school"

typedef struct ComobRet {	//ComobBoxѡ�񴰿ڽ�������
	int type;
	int sta;
	char Sel[72];
}ComobRet;

typedef struct MySQLHostVariable
{
	MYSQL mysql;
	char host[31];	//MySQ������IP
}MySQLHostVariable;


typedef struct operators {
	int  ID;			//ID
	int  grade;			//�༶
	char GradeName[32];	//�༶����
	char No[16];		//ѧ�Ż�Ա����
	char Name[32];		//����
	long pwd;			//���κ�����
	int  right;			//Ȩ��
}operators;

typedef struct question {
	int ID;
	int course;
	char Text[72];
	int  Answer;
	int  Answer2;
	int  UserAnswer;
	int  UserAnswer2;
	int  UserOldAnswer;
	int  UserOldAnswer2;
	int  Correct;
	int  Error;
}question;


typedef struct global {
	operators op;
}global;

int ValidIP(char * str);		//У��IP��ַ�Ϸ���
int PwdCode(char *s1, char *s2);	//�û���+�������
int CloseMySQL(MySQLHostVariable * host);	//�ر�MySQL����
int InitMySQL(MySQLHostVariable * host);	//��ʼ��MySQL����
char * MsToTime(char * ms, char * Src);		//ת�����뵥λ��ʱ��Ϊ�֣�����
int WriteListCtrl(CListCtrl *ListCtrl, char * Fn);//�������д�ı��ļ�