// ApprovalManage.cpp : 实现文件
//

#include "stdafx.h"
#include "include\mysql.h"
#include "School.h"
#include "SchoolMod.h"
#include "ApprovalManage.h"
#include "afxdialogex.h"


// ApprovalManage 对话框

IMPLEMENT_DYNAMIC(ApprovalManage, CDialogEx)

ApprovalManage::ApprovalManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(ApprovalManage::IDD, pParent)
{

}

ApprovalManage::~ApprovalManage()
{
}

void ApprovalManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ApprovalManage, CDialogEx)
	ON_BN_CLICKED(IDOK, &ApprovalManage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ApprovalManage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CmdOK, &ApprovalManage::OnBnClickedCmdok)
END_MESSAGE_MAP()


// ApprovalManage 消息处理程序


void ApprovalManage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void ApprovalManage::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void ApprovalManage::OnBnClickedCmdok()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


BOOL ApprovalManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Init();
	return TRUE;
}

int  ApprovalManage::Init(void){
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	int  i;
	char buf[256];
	sta=InitMySQL(&host);//连接MySQL数据库
	if(sta==TRUE){
		mysql_query(&host.mysql,"SELECT operator.ID,`Name`,`No`,`user`,`right` "
		" FROM operator INNER JOIN grade ON operator.grade = grade.ID Where (`right`='0' Or `right`='2')Limit 1;");
		result = mysql_store_result(&host.mysql);
		if(result==NULL){
			CloseMySQL(&host);	//关闭MySQL连接	
			return FALSE;
		}
		i=(long)result->row_count;//计数
		if(i>0){
			row = mysql_fetch_row(result);
			ID=atoi(row[0]);
			right=atoi(row[4]);
			if(right==0)
				sprintf_s(buf,sizeof(buf),
				"待审批申请：\r\n人员类别：学生\r\n班级：%s\r\n学号：%s\r\n姓名：%s\r\n",row[1],row[2],row[3]);
			else
				sprintf_s(buf,sizeof(buf),
				"待审批申请：\r\n人员类别：教师\r\n班级：%s\r\n学号：%s\r\n姓名：%s\r\n",row[1],row[2],row[3]);
			cEditNote.SetWindowTextA(buf);
			cEditNote.SetSel(-1);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
		if(i<=0){
			MessageBoxA("无待审批任务");
			CDialogEx::OnOK();
		}
	}
	return TRUE;
}
