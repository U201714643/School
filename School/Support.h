#include "include\mysql.h"

#define  MYSQLUSER "root"
#define  MYSQLPWD "19990220"
#define  MYSQLDBS "school"
#define  EXPLEN 128

typedef struct ComobRet {	//ComobBox选择窗口交互变量
	int type;
	int sta;
	char Sel[72];
}ComobRet;

typedef struct MySQLHostVariable
{	//连接MySQL数据库用
	MYSQL mysql;
	char host[31];	//MySQ服务器IP
}MySQLHostVariable;


typedef struct operators {	//用户信息
	int  ID;			//用户在数据库中的唯一ID
	int  GradeID;		//班级
	char GradeName[32];	//班级名称
	char No[16];		//学号或员工号
	char Name[32];		//姓名
	int  Pwd;			//变形后密码
	int  Right;			//权限
}operators;

typedef struct question {	//题目信息
	int ID;			//题目在数据库中的唯一ID
	int CourseID;		//课程在数据库中的唯一ID
	char Text[EXPLEN];	//题目文本
	int  Answer;		//答案
	int  AnswerLeft;	//答案中的余数部分
	int  UserAnswer;	//用户作答
	int  UserAnswerLeft;//用户作答中的余数部分
	int  UserOldAnswer;	//上次作答
	int  UserOldAnswerLeft;	//上次作答中的余数部分
	int  Correct;		//正确，为1代表正确
	int  Error;			//错误，为1代表错误，分开是为了方便统计
}question;


typedef struct global {
	operators op;	//使用者信息
}global;

#define Clear(x) memset(x, 0, sizeof(x))	//连续空间清0

int ValidIP(char * str);		//校验IP地址合法性
int IntFormEidt(CEdit * Edit);	//从文本框中获得数字
char * RealChar(char * str);		//转换*、/为×、÷便于显示
int PwdCode(char *s1, char *s2);	//用户名+密码编码
int VerifyOperator(char * name, char * pwd);//校验用户
int CloseMySQL(MySQLHostVariable * host);	//关闭MySQL连接
int InitMySQL(MySQLHostVariable * host);	//初始化MySQL连接
char * MsToTime(char * ms, char * Src);		//转换毫秒单位的时间为分：秒制
int WriteListCtrl(CListCtrl *ListCtrl, char * Fn);//表格数据写文本文件