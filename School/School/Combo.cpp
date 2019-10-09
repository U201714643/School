// Combo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SchoolMod.h"
#include "School.h"
#include "Combo.h"
#include "afxdialogex.h"

ComobRet * ret;

// Combo �Ի���

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


// Combo ��Ϣ�������


void Combo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void Combo::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void Combo::OnBnClickedCmdexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ret->sta=FALSE;
	CDialogEx::OnOK();
}


void Combo::OnBnClickedCmdok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if(ret->type==0){
		MySQLHostVariable host;
		MYSQL_RES *result;
		MYSQL_ROW row;
		int sta;	//״̬��־
		int i,j;
		sta=InitMySQL(&host);//����MySQL���ݿ�
		if(sta==TRUE){
			mysql_query(&host.mysql,"Select `Course`,`CourseName`  From `Course` order by `Course`");
			result = mysql_store_result(&host.mysql);
			if(result!=NULL)
				j=(long)result->row_count;//����
			else
				j=0;
			for(i=0;i<j;i++){
				row = mysql_fetch_row(result);	
				ComboBox.InsertString(i,row[1]);
			}
			mysql_free_result(result);
			CloseMySQL(&host);	//�ر�MySQL����	
		};	
		Combo::SetWindowTextA("��ѡ��γ�");
	}
	return TRUE;
}

void Combo::OnCbnSelchangeCmb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CmdOk.EnableWindow(TRUE);
}
