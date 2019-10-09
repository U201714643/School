// Combo.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolMod.h"
#include "School.h"
#include "Combo.h"
#include "afxdialogex.h"

ComobRet * ret;

// Combo 对话框

IMPLEMENT_DYNAMIC(Combo, CDialogEx)

Combo::Combo(CWnd* pParent /*=NULL*/)
	: CDialogEx(Combo::IDD, pParent)
{

}

Combo::~Combo()
{
}

void Combo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Cmb, ComboBox);
	DDX_Control(pDX, IDC_CmdOK, CmdOk);
}


BEGIN_MESSAGE_MAP(Combo, CDialogEx)
	ON_BN_CLICKED(IDOK, &Combo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Combo::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CmdExit, &Combo::OnBnClickedCmdexit)
	ON_BN_CLICKED(IDC_CmdOK, &Combo::OnBnClickedCmdok)
	ON_CBN_SELCHANGE(IDC_Cmb, &Combo::OnCbnSelchangeCmb)
END_MESSAGE_MAP()


// Combo 消息处理程序


void Combo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void Combo::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void Combo::OnBnClickedCmdexit()
{
	// TODO: 在此添加控件通知处理程序代码
	ret->sta=FALSE;
	CDialogEx::OnOK();
}


void Combo::OnBnClickedCmdok()
{
	// TODO: 在此添加控件通知处理程序代码
	ret->sta=TRUE;
	ComboBox.GetWindowTextA(ret->Sel,sizeof(ret->Sel));
	CDialogEx::OnOK();
}

int Combo::SetRet(ComobRet * lpret){
	ret=lpret;
	return TRUE;
}

BOOL Combo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if(ret->type==0){
		MySQLHostVariable host;
		MYSQL_RES *result;
		MYSQL_ROW row;
		int sta;	//状态标志
		int i,j;
		sta=InitMySQL(&host);//连接MySQL数据库
		if(sta==TRUE){
			mysql_query(&host.mysql,"Select `Course`,`CourseName`  From `Course` order by `Course`");
			result = mysql_store_result(&host.mysql);
			if(result!=NULL)
				j=(long)result->row_count;//总数
			else
				j=0;
			for(i=0;i<j;i++){
				row = mysql_fetch_row(result);	
				ComboBox.InsertString(i,row[1]);
			}
			mysql_free_result(result);
			CloseMySQL(&host);	//关闭MySQL连接	
		};	
		Combo::SetWindowTextA("请选择课程");
	}
	return TRUE;
}

void Combo::OnCbnSelchangeCmb()
{
	// TODO: 在此添加控件通知处理程序代码
	CmdOk.EnableWindow(TRUE);
}
