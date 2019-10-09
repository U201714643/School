// Approval.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "include\mysql.h"
#include "School.h"
#include "SchoolMod.h"
#include "Approval.h"
#include "afxdialogex.h"


// Approval �Ի���

IMPLEMENT_DYNAMIC(Approval, CDialogEx)

Approval::Approval(CWnd* pParent /*=NULL*/)
	: CDialogEx(Approval::IDD, pParent)
{

}

Approval::~Approval()
{
}

void Approval::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, cEditNote);
}


BEGIN_MESSAGE_MAP(Approval, CDialogEx)
	ON_BN_CLICKED(IDOK, &Approval::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Approval::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CmdOK, &Approval::OnBnClickedCmdok)
	ON_BN_CLICKED(IDC_CmdEnable, &Approval::OnBnClickedCmdenable)
	ON_BN_CLICKED(IDC_CmdDisable, &Approval::OnBnClickedCmddisable)
END_MESSAGE_MAP()


// Approval ��Ϣ�������


void Approval::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void Approval::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}

BOOL Approval::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	Init();
	return TRUE;
}

int  Approval::Init(void){
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int  i;
	char buf[256];
	sta=InitMySQL(&host);//����MySQL���ݿ�
	if(sta==TRUE){
		mysql_query(&host.mysql,"SELECT operator.ID,`Name`,`No`,`user`,`right` "
		" FROM operator Left JOIN grade ON operator.grade = grade.ID Where (`right`='0' Or `right`='2')Limit 1;");
		result = mysql_store_result(&host.mysql);
		if(result==NULL){
			CloseMySQL(&host);	//�ر�MySQL����	
			return FALSE;
		}
		i=(long)result->row_count;//����
		if(i>0){
			row = mysql_fetch_row(result);
			ID=atoi(row[0]);
			right=atoi(row[4]);
			if(right==0)
				sprintf_s(buf,sizeof(buf),
				"���������룺\r\n��Ա���ѧ��\r\n�༶��%s\r\nѧ�ţ�%s\r\n������%s\r\n",row[1],row[2],row[3]);
			else
				sprintf_s(buf,sizeof(buf),
				"���������룺\r\n��Ա��𣺽�ʦ\r\nԱ���ţ�%s\r\n������%s\r\n",row[2],row[3]);
			cEditNote.SetWindowTextA(buf);
			cEditNote.SetSel(-1);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
		if(i<=0){
			MessageBoxA("�޴���������");
			CDialogEx::OnOK();
		}
	}
	return TRUE;
}

void Approval::OnBnClickedCmdok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void Approval::OnBnClickedCmdenable()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MySQLHostVariable host;
	int sta;	//״̬��־
	char cmd[256];
	sta=InitMySQL(&host);//����MySQL���ݿ�
	if(sta==TRUE){
		if(right==2){
			sprintf_s(cmd,sizeof(cmd),"Update `operator` SET `Right`='3',`grade`='0' Where `ID`='%d';",ID);
		}
		else
			sprintf_s(cmd,sizeof(cmd),"Update `operator` SET `Right`='1' Where `ID`='%d';",ID);
		mysql_query(&host.mysql,cmd);
		CloseMySQL(&host);	//�ر�MySQL����	
		Init();
	}
	return ;
}


void Approval::OnBnClickedCmddisable()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MySQLHostVariable host;
	int sta;	//״̬��־
	char cmd[256];
	sta=InitMySQL(&host);//����MySQL���ݿ�
	if(sta==TRUE){
		sprintf_s(cmd,sizeof(cmd),"Delete From `operator` Where `ID`='%d';",ID);
		mysql_query(&host.mysql,cmd);
		CloseMySQL(&host);	//�ر�MySQL����	
		Init();
	}
	return ;
}

