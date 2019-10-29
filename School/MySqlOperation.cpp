#include "stdafx.h"
#include "Support.h"
#include "include\mysql.h"

extern global gs;	//登录者信息

int CloseMySQL(MySQLHostVariable * host)//关闭MySQL连接
{
	mysql_close(&host->mysql);
	return 0;
}

int InitMySQL(MySQLHostVariable * host)	//初始化MySQL连接
{	//初始MySQL连接
	int nConnectTimeout = 5, nWriteReadTimeout = 5;
	char hostip[128];
	int i;
	MYSQL* sap;
	char path[128]; //存放路径的变量
	mysql_init(&host->mysql);
	mysql_options(&host->mysql, MYSQL_SET_CHARSET_NAME, "gb2312");//必须要在本函数全局定义 
	mysql_options(&host->mysql, MYSQL_OPT_CONNECT_TIMEOUT, &nConnectTimeout);
	mysql_options(&host->mysql, MYSQL_OPT_READ_TIMEOUT, &nWriteReadTimeout);
	mysql_options(&host->mysql, MYSQL_OPT_WRITE_TIMEOUT, &nWriteReadTimeout);
	GetModuleFileName(NULL, path, sizeof(path));     //获取当前进程已加载模块的文件的完整路径
	memcpy(path + strlen(path) - 4, ".ini", 5);
	GetPrivateProfileString("MySQL", "MySQL", "", hostip, sizeof(hostip), path);
	if (ValidIP(hostip) != TRUE) {	//IP地址非法，用127.0.0.1代替并写INI文件
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

int VerifyOperator(char * name, char * password) {	//校验用户
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	long pwd;
	int sta;	//状态标志
	char buf[200];
	int i;
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		pwd = PwdCode(name, password);	//编码
		sprintf_s(buf, sizeof(buf), "Select operator.`ID`,`Grade`,`No`,`Right`, grade.`Name` From `operator` "
			"Left Join Grade ON operator.grade = grade.ID Where `User`='%s' And `Password`='%ld';", name, pwd);
		mysql_query(&host.mysql, buf);	//进入数据库查询
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			i = (long)result->row_count;//计数
		else
			i = 0;
		if (i == 1) {
			//校验通过，将用户数据读入全局变量gs.op中
			row = mysql_fetch_row(result);
			gs.op.ID = atoi(row[0]);	//内部ID
			gs.op.GradeID = atoi(row[1]);	//班级
			strcpy_s(gs.op.No, sizeof(gs.op.No), row[2]);	//row不能修改
			gs.op.Right = atoi(row[3]);	//权限
			strcpy_s(gs.op.Name, sizeof(gs.op.Name), name);
			if (row[4] == NULL)	//班级名称可能无效。例如教师没有所属班级
				gs.op.GradeName[0] = 0;	//这时候将班级名称设置为""字符串
			else
				strcpy_s(gs.op.GradeName, sizeof(gs.op.GradeName), row[4]);
			gs.op.Pwd = pwd;	//密码
		}
		else
			sta = FALSE;
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
	};
	return sta;
}