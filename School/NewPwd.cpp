// NewPwd.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "include\mysql.h"
#include "School.h"
#include "NewPwd.h"
#include "afxdialogex.h"
#include "Support.h"

extern global gs;

// NewPwd �Ի���

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


// NewPwd ��Ϣ�������


void NewPwd::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void NewPwd::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void NewPwd::OnBnClickedCmdexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void NewPwd::OnBnClickedCmdok()
{
	//�޸�����
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//---------�������---------
	char bufPwd[72];
	char bufNewPwd1[72], bufNewPwd2[72];
	TxtOldPwd.GetWindowTextA(bufPwd, sizeof(bufPwd));
	TxtNewPwd1.GetWindowTextA(bufNewPwd1, sizeof(bufNewPwd1));
	TxtNewPwd2.GetWindowTextA(bufNewPwd2, sizeof(bufNewPwd2));
	//---------������������---------
	if (strcmp(bufNewPwd1, bufNewPwd2) != 0) {
		MessageBoxA("�����������������һ��");
		return;
	}
	if (strcmp(bufPwd, bufNewPwd1) == 0) {
		MessageBoxA("�������¾�����һ��");
		return;
	}
	if (strlen(bufNewPwd1) < 4) {
		MessageBoxA("����������4λ");
		return;
	}
	if (strlen(bufNewPwd1) > 20) {
		MessageBoxA("����������20λ");
		return;
	}
	//---------���������ݿ�---------
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	char cmd[200];
	int  i;
	long pwd;
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		//---------���������---------
		pwd = PwdCode(gs.op.Name, bufPwd);
		sprintf_s(cmd, sizeof(cmd), "Select `ID` from `Operator` Where `User`='%s' And `Password`='%ld';", gs.op.Name, pwd);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			i = (long)result->row_count;//����
		else
			i = 0;
		if (i != 1) {	//�����벻��
			MessageBoxA("�����벻��");
			CloseMySQL(&host);	//�ر�MySQL����	
			return;
		}
		row = mysql_fetch_row(result);
		ASSERT(gs.op.ID == atoi(row[0]));
		mysql_free_result(result);
		//У��ͨ�������û����ݶ���ȫ�ֱ���gs.op��
		//---------�ϴ�������---------
		pwd = PwdCode(gs.op.Name, bufNewPwd1);
		sprintf_s(cmd, sizeof(cmd), "Update `operator` Set `password`='%ld' Where `ID`='%d';", pwd, gs.op.ID);
		mysql_query(&host.mysql, cmd);
		MessageBoxA("�����Ѹ��£����ס������", "��ʾ");
		CloseMySQL(&host);	//�ر�MySQL����	
	}
	CDialogEx::OnOK();	//�رմ���
}
