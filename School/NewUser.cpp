// NewUser.cpp : 实现文件
//

#include "stdafx.h"
#include "include\mysql.h"
#include "School.h"
#include "NewUser.h"
#include "Support.h"
#include "afxdialogex.h"


// NewUser 对话框

IMPLEMENT_DYNAMIC(NewUser, CDialogEx)

NewUser::NewUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(NewUser::IDD, pParent)
{

}

NewUser::~NewUser()
{
}

void NewUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CmbGrade, cCmbGrade);
	DDX_Control(pDX, IDC_TxtNo, cTxtNo);
	DDX_Control(pDX, IDC_TxtName, cTxtName);
	DDX_Control(pDX, IDC_TxtPwd1, cTxtPwd1);
	DDX_Control(pDX, IDC_TxtPwd2, cTxtPwd2);
	DDX_Control(pDX, IDC_CmbType, cCmbType);
}


BEGIN_MESSAGE_MAP(NewUser, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewUser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &NewUser::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CmdQuit, &NewUser::OnBnClickedCmdquit)
	ON_BN_CLICKED(IDC_CmdOK, &NewUser::OnBnClickedCmdok)
	//	ON_CBN_SELCHANGE(IDC_CmbGrade, &NewUser::OnCbnSelchangeCmbgrade)
END_MESSAGE_MAP()


// NewUser 消息处理程序


void NewUser::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void NewUser::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void NewUser::OnBnClickedCmdquit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

BOOL NewUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	int  i, j;
	//---------修改初始内容---------
	for (; cCmbType.DeleteString(0) == 1;);
	cCmbType.InsertString(0, "学生");
	cCmbType.InsertString(1, "教师");
	cCmbType.SetCurSel(0);	//初始无选择
	//---------连接至数据库---------
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		//---------获得班级信息---------
		mysql_query(&host.mysql, "Select `Name` from `Grade` order by `ID`;");
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//计数
		else
			j = 0;
		for (; cCmbGrade.DeleteString(0) == 1;);//删除初始内容
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);	//显示班级信息
			cCmbGrade.InsertString(i, row[0]);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
		cCmbGrade.SetWindowTextA("a");
	}
	return TRUE;
}

void NewUser::OnBnClickedCmdok()
{
	//注册新用户
	// TODO: 在此添加控件通知处理程序代码
	char Type[72];	//学生/教师
	char Grade[72];	//班级
	char No[72];	//学号
	char Name[72];	//姓名
	char Pwd1[72];	//密码
	char Pwd2[72];	//重复密码
	int  right;		//权限
	int  opID;		//内部ID
	//---------获得申请种类---------
	cCmbType.GetWindowTextA(Type, sizeof(Type));
	if (strcmp(Type, "学生") == 0)
		right = 0;	//0--学生申请
	else
		right = 2;	//2--教师申请
	//---------获得申请班级--------
	cCmbGrade.GetWindowTextA(Grade, sizeof(Grade));
	if (strlen(Grade) < 1) {
		MessageBoxA("请选择班级", "提示");
		return;
	}
	//---------检验信息合理性--------
	cTxtNo.GetWindowTextA(No, sizeof(No));
	if (strlen(No) < 4) {
		MessageBoxA("学号至少4位", "提示");
		return;
	}
	if (strlen(No) > 14) {
		MessageBoxA("学号至多14位", "提示");
		return;
	}
	cTxtName.GetWindowTextA(Name, sizeof(Name));
	if (strlen(Name) < 4) {
		MessageBoxA("姓名至少2个汉字", "提示");
		return;
	}
	if (strlen(Name) > 20) {
		MessageBoxA("姓名至多10个汉字", "提示");
		return;
	}
	cTxtPwd1.GetWindowTextA(Pwd1, sizeof(Pwd1));
	if (strlen(Pwd1) < 4) {
		MessageBoxA("密码长度至少4位", "提示");
		return;
	}
	if (strlen(Pwd1) > 20) {
		MessageBoxA("密码长度至多20位", "提示");
		return;
	}
	cTxtPwd2.GetWindowTextA(Pwd2, sizeof(Pwd2));
	if (strcmp(Pwd1, Pwd2) != 0) {
		MessageBoxA("两次输入的密码应一致", "提示");
		return;
	}
	//---------连接至数据库---------
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	int j;
	int GradeID;
	long pwd;
	char cmd[256];
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		//---------检验是否存在该用户---------
		if (VerifyOperator(Name, Pwd1) == TRUE) {	//已验证pwd1和pwd2一致，直接使用其中一个即可
			CloseMySQL(&host);	//关闭MySQL连接	
			MessageBoxA("使用了已存在的用户名和密码组合。\n您可以继续使用该用户名，但需要使用另一个密码。", "提示");
			return;
		}
		//---------以班级为索引---------
		sprintf_s(cmd, sizeof(cmd), "Select `ID` from `Grade` Where `Name`='%s'", Grade);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result == NULL) {
			CloseMySQL(&host);	//关闭MySQL连接	
			MessageBoxA("所选班级无效,注册失败", "提示");
			return;
		}
		j = (long)result->row_count;//计数
		if (j != 1) {
			mysql_free_result(result);
			CloseMySQL(&host);	//关闭MySQL连接	
			MessageBoxA("选择班级无效,注册失败", "提示");
			return;
		}
		row = mysql_fetch_row(result);
		GradeID = atoi(row[0]);
		mysql_free_result(result);
		//---------以学号为索引---------
		sprintf_s(cmd, sizeof(cmd), "Select `ID` from `Operator` Where `No`='%s'", No);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result == NULL) {
			CloseMySQL(&host);	//关闭MySQL连接	
			MessageBoxA("学号/工号无效,注册失败", "提示");
			return;
		}
		j = (long)result->row_count;//计数
		if (j > 0) {
			mysql_free_result(result);
			CloseMySQL(&host);	//关闭MySQL连接	
			MessageBoxA("学号/工号已存在,注册失败", "提示");
			return;
		}
		mysql_free_result(result);
		//---------上传至数据库---------
		pwd = PwdCode(Name, Pwd1);
		if (right == 0) {	//申请学生用户
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Operator` (`Grade`,`No`,`User`,`Password`,`right`)"
				" Values ('%d','%s','%s','%ld','0');", GradeID, No, Name, pwd);
			mysql_query(&host.mysql, cmd);
			j = (int)host.mysql.affected_rows;
		}
		else {	//申请教师用户
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Operator` (`Grade`,`No`,`User`,`Password`,`right`)"
				" Values ('%d','%s','%s','%ld','2');", 0, No, Name, pwd);
			mysql_query(&host.mysql, cmd);//插入人员表
			j = (int)host.mysql.affected_rows;
			if (j == 1) {
				opID = (int)host.mysql.insert_id;
				sprintf_s(cmd, sizeof(cmd), "Insert Into `Manage` (`OperatorID`,`GradeID`,`Right`)"
					" Values ('%d','%d','0');", opID, GradeID);
				mysql_query(&host.mysql, cmd);//插入管班表
			}
		}
		if (j == 1)
			MessageBoxA("注册成功", "提示");
		else
			MessageBoxA("注册失败", "提示");
		CloseMySQL(&host);	//关闭MySQL连接	
	}
	CDialogEx::OnOK();	//关闭窗口
}
