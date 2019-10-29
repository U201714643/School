// NewUser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "include\mysql.h"
#include "School.h"
#include "NewUser.h"
#include "Support.h"
#include "afxdialogex.h"


// NewUser �Ի���

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


// NewUser ��Ϣ�������


void NewUser::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void NewUser::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void NewUser::OnBnClickedCmdquit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

BOOL NewUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int  i, j;
	//---------�޸ĳ�ʼ����---------
	for (; cCmbType.DeleteString(0) == 1;);
	cCmbType.InsertString(0, "ѧ��");
	cCmbType.InsertString(1, "��ʦ");
	cCmbType.SetCurSel(0);	//��ʼ��ѡ��
	//---------���������ݿ�---------
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		//---------��ð༶��Ϣ---------
		mysql_query(&host.mysql, "Select `Name` from `Grade` order by `ID`;");
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		for (; cCmbGrade.DeleteString(0) == 1;);//ɾ����ʼ����
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);	//��ʾ�༶��Ϣ
			cCmbGrade.InsertString(i, row[0]);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
		cCmbGrade.SetWindowTextA("a");
	}
	return TRUE;
}

void NewUser::OnBnClickedCmdok()
{
	//ע�����û�
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char Type[72];	//ѧ��/��ʦ
	char Grade[72];	//�༶
	char No[72];	//ѧ��
	char Name[72];	//����
	char Pwd1[72];	//����
	char Pwd2[72];	//�ظ�����
	int  right;		//Ȩ��
	int  opID;		//�ڲ�ID
	//---------�����������---------
	cCmbType.GetWindowTextA(Type, sizeof(Type));
	if (strcmp(Type, "ѧ��") == 0)
		right = 0;	//0--ѧ������
	else
		right = 2;	//2--��ʦ����
	//---------�������༶--------
	cCmbGrade.GetWindowTextA(Grade, sizeof(Grade));
	if (strlen(Grade) < 1) {
		MessageBoxA("��ѡ��༶", "��ʾ");
		return;
	}
	//---------������Ϣ������--------
	cTxtNo.GetWindowTextA(No, sizeof(No));
	if (strlen(No) < 4) {
		MessageBoxA("ѧ������4λ", "��ʾ");
		return;
	}
	if (strlen(No) > 14) {
		MessageBoxA("ѧ������14λ", "��ʾ");
		return;
	}
	cTxtName.GetWindowTextA(Name, sizeof(Name));
	if (strlen(Name) < 4) {
		MessageBoxA("��������2������", "��ʾ");
		return;
	}
	if (strlen(Name) > 20) {
		MessageBoxA("��������10������", "��ʾ");
		return;
	}
	cTxtPwd1.GetWindowTextA(Pwd1, sizeof(Pwd1));
	if (strlen(Pwd1) < 4) {
		MessageBoxA("���볤������4λ", "��ʾ");
		return;
	}
	if (strlen(Pwd1) > 20) {
		MessageBoxA("���볤������20λ", "��ʾ");
		return;
	}
	cTxtPwd2.GetWindowTextA(Pwd2, sizeof(Pwd2));
	if (strcmp(Pwd1, Pwd2) != 0) {
		MessageBoxA("�������������Ӧһ��", "��ʾ");
		return;
	}
	//---------���������ݿ�---------
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int j;
	int GradeID;
	long pwd;
	char cmd[256];
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		//---------�����Ƿ���ڸ��û�---------
		if (VerifyOperator(Name, Pwd1) == TRUE) {	//����֤pwd1��pwd2һ�£�ֱ��ʹ������һ������
			CloseMySQL(&host);	//�ر�MySQL����	
			MessageBoxA("ʹ�����Ѵ��ڵ��û�����������ϡ�\n�����Լ���ʹ�ø��û���������Ҫʹ����һ�����롣", "��ʾ");
			return;
		}
		//---------�԰༶Ϊ����---------
		sprintf_s(cmd, sizeof(cmd), "Select `ID` from `Grade` Where `Name`='%s'", Grade);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result == NULL) {
			CloseMySQL(&host);	//�ر�MySQL����	
			MessageBoxA("��ѡ�༶��Ч,ע��ʧ��", "��ʾ");
			return;
		}
		j = (long)result->row_count;//����
		if (j != 1) {
			mysql_free_result(result);
			CloseMySQL(&host);	//�ر�MySQL����	
			MessageBoxA("ѡ��༶��Ч,ע��ʧ��", "��ʾ");
			return;
		}
		row = mysql_fetch_row(result);
		GradeID = atoi(row[0]);
		mysql_free_result(result);
		//---------��ѧ��Ϊ����---------
		sprintf_s(cmd, sizeof(cmd), "Select `ID` from `Operator` Where `No`='%s'", No);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result == NULL) {
			CloseMySQL(&host);	//�ر�MySQL����	
			MessageBoxA("ѧ��/������Ч,ע��ʧ��", "��ʾ");
			return;
		}
		j = (long)result->row_count;//����
		if (j > 0) {
			mysql_free_result(result);
			CloseMySQL(&host);	//�ر�MySQL����	
			MessageBoxA("ѧ��/�����Ѵ���,ע��ʧ��", "��ʾ");
			return;
		}
		mysql_free_result(result);
		//---------�ϴ������ݿ�---------
		pwd = PwdCode(Name, Pwd1);
		if (right == 0) {	//����ѧ���û�
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Operator` (`Grade`,`No`,`User`,`Password`,`right`)"
				" Values ('%d','%s','%s','%ld','0');", GradeID, No, Name, pwd);
			mysql_query(&host.mysql, cmd);
			j = (int)host.mysql.affected_rows;
		}
		else {	//�����ʦ�û�
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Operator` (`Grade`,`No`,`User`,`Password`,`right`)"
				" Values ('%d','%s','%s','%ld','2');", 0, No, Name, pwd);
			mysql_query(&host.mysql, cmd);//������Ա��
			j = (int)host.mysql.affected_rows;
			if (j == 1) {
				opID = (int)host.mysql.insert_id;
				sprintf_s(cmd, sizeof(cmd), "Insert Into `Manage` (`OperatorID`,`GradeID`,`Right`)"
					" Values ('%d','%d','0');", opID, GradeID);
				mysql_query(&host.mysql, cmd);//����ܰ��
			}
		}
		if (j == 1)
			MessageBoxA("ע��ɹ�", "��ʾ");
		else
			MessageBoxA("ע��ʧ��", "��ʾ");
		CloseMySQL(&host);	//�ر�MySQL����	
	}
	CDialogEx::OnOK();	//�رմ���
}
