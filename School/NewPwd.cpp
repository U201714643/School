// NewPwd.cpp : 实现文件
//
#include "stdafx.h"
#include "include\mysql.h"
#include "School.h"
#include "NewPwd.h"
#include "afxdialogex.h"
#include "Support.h"

extern global gs;

// NewPwd 对话框

IMPLEMENT_DYNAMIC(NewPwd, CDialogEx)

NewPwd::NewPwd(CWnd* pParent /*=NULL*/)
	: CDialogEx(NewPwd::IDD, pParent)
{

}

NewPwd::~NewPwd()
{
}

void NewPwd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TxtOldPwd, TxtOldPwd);
	DDX_Control(pDX, IDC_TxtNewPwd1, TxtNewPwd1);
	DDX_Control(pDX, IDC_TxtNewPwd2, TxtNewPwd2);
}


BEGIN_MESSAGE_MAP(NewPwd, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewPwd::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &NewPwd::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CmdExit, &NewPwd::OnBnClickedCmdexit)
	ON_BN_CLICKED(IDC_CmdOK, &NewPwd::OnBnClickedCmdok)
END_MESSAGE_MAP()


// NewPwd 消息处理程序


void NewPwd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void NewPwd::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void NewPwd::OnBnClickedCmdexit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void NewPwd::OnBnClickedCmdok()
{
	//修改密码
	// TODO: 在此添加控件通知处理程序代码
	//---------获得输入---------
	char bufPwd[72];
	char bufNewPwd1[72], bufNewPwd2[72];
	TxtOldPwd.GetWindowTextA(bufPwd, sizeof(bufPwd));
	TxtNewPwd1.GetWindowTextA(bufNewPwd1, sizeof(bufNewPwd1));
	TxtNewPwd2.GetWindowTextA(bufNewPwd2, sizeof(bufNewPwd2));
	//---------检查输入合理性---------
	if (strcmp(bufNewPwd1, bufNewPwd2) != 0) {
		MessageBoxA("两次输入新密码必须一致");
		return;
	}
	if (strcmp(bufPwd, bufNewPwd1) == 0) {
		MessageBoxA("不允许新旧密码一致");
		return;
	}
	if (strlen(bufNewPwd1) < 4) {
		MessageBoxA("新密码至少4位");
		return;
	}
	if (strlen(bufNewPwd1) > 20) {
		MessageBoxA("新密码至多20位");
		return;
	}
	//---------连接至数据库---------
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	char cmd[200];
	int  i;
	long pwd;
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		//---------检验旧密码---------
		pwd = PwdCode(gs.op.Name, bufPwd);
		sprintf_s(cmd, sizeof(cmd), "Select `ID` from `Operator` Where `User`='%s' And `Password`='%ld';", gs.op.Name, pwd);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			i = (long)result->row_count;//计数
		else
			i = 0;
		if (i != 1) {	//旧密码不符
			MessageBoxA("旧密码不符");
			CloseMySQL(&host);	//关闭MySQL连接	
			return;
		}
		row = mysql_fetch_row(result);
		ASSERT(gs.op.ID == atoi(row[0]));
		mysql_free_result(result);
		//校验通过，将用户数据读入全局变量gs.op中
		//---------上传新密码---------
		pwd = PwdCode(gs.op.Name, bufNewPwd1);
		sprintf_s(cmd, sizeof(cmd), "Update `operator` Set `password`='%ld' Where `ID`='%d';", pwd, gs.op.ID);
		mysql_query(&host.mysql, cmd);
		MessageBoxA("密码已更新，请记住新密码", "提示");
		CloseMySQL(&host);	//关闭MySQL连接	
	}
	CDialogEx::OnOK();	//关闭窗口
}
