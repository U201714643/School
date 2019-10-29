#include "include\mysql.h"

#define  MYSQLUSER "root"
#define  MYSQLPWD "19990220"
#define  MYSQLDBS "school"
#define  EXPLEN 128

typedef struct ComobRet {	//ComobBoxѡ�񴰿ڽ�������
	int type;
	int sta;
	char Sel[72];
}ComobRet;

typedef struct MySQLHostVariable
{	//����MySQL���ݿ���
	MYSQL mysql;
	char host[31];	//MySQ������IP
}MySQLHostVariable;


typedef struct operators {	//�û���Ϣ
	int  ID;			//�û������ݿ��е�ΨһID
	int  GradeID;		//�༶
	char GradeName[32];	//�༶����
	char No[16];		//ѧ�Ż�Ա����
	char Name[32];		//����
	int  Pwd;			//���κ�����
	int  Right;			//Ȩ��
}operators;

typedef struct question {	//��Ŀ��Ϣ
	int ID;			//��Ŀ�����ݿ��е�ΨһID
	int CourseID;		//�γ������ݿ��е�ΨһID
	char Text[EXPLEN];	//��Ŀ�ı�
	int  Answer;		//��
	int  AnswerLeft;	//���е���������
	int  UserAnswer;	//�û�����
	int  UserAnswerLeft;//�û������е���������
	int  UserOldAnswer;	//�ϴ�����
	int  UserOldAnswerLeft;	//�ϴ������е���������
	int  Correct;		//��ȷ��Ϊ1������ȷ
	int  Error;			//����Ϊ1������󣬷ֿ���Ϊ�˷���ͳ��
}question;


typedef struct global {
	operators op;	//ʹ������Ϣ
}global;

#define Clear(x) memset(x, 0, sizeof(x))	//�����ռ���0

int ValidIP(char * str);		//У��IP��ַ�Ϸ���
int IntFormEidt(CEdit * Edit);	//���ı����л������
char * RealChar(char * str);		//ת��*��/Ϊ�����±�����ʾ
int PwdCode(char *s1, char *s2);	//�û���+�������
int VerifyOperator(char * name, char * pwd);//У���û�
int CloseMySQL(MySQLHostVariable * host);	//�ر�MySQL����
int InitMySQL(MySQLHostVariable * host);	//��ʼ��MySQL����
char * MsToTime(char * ms, char * Src);		//ת�����뵥λ��ʱ��Ϊ�֣�����
int WriteListCtrl(CListCtrl *ListCtrl, char * Fn);//�������д�ı��ļ�