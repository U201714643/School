#include "include\mysql.h"

#define  MYSQLUSER "root"
#define  MYSQLPWD "19990220"
#define  MYSQLDBS "school"

typedef struct ComobRet {	//ComobBox选择窗口交互变量
	int type;
	int sta;
	char Sel[72];
}ComobRet;

typedef struct MySQLHostVariable
{
	MYSQL mysql;
	char host[31];	//MySQ服务器IP
}MySQLHostVariable;


typedef struct operators {
	int  ID;			//ID
	int  grade;			//班级
	char GradeName[32];	//班级名称
	char No[16];		//学号或员工号
	char Name[32];		//姓名
	long pwd;			//变形后密码
	int  right;			//权限
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

int ValidIP(char * str);		//校验IP地址合法性
int PwdCode(char *s1, char *s2);	//用户名+密码编码
int CloseMySQL(MySQLHostVariable * host);	//关闭MySQL连接
int InitMySQL(MySQLHostVariable * host);	//初始化MySQL连接
char * MsToTime(char * ms, char * Src);		//转换毫秒单位的时间为分：秒制
int WriteListCtrl(CListCtrl *ListCtrl, char * Fn);//表格数据写文本文件