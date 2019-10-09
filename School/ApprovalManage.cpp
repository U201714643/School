// ApprovalManage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "include\mysql.h"
#include "School.h"
#include "SchoolMod.h"
#include "ApprovalManage.h"
#include "afxdialogex.h"


// ApprovalManage �Ի���

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


// ApprovalManage ��Ϣ�������


void ApprovalManage::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void ApprovalManage::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void ApprovalManage::OnBnClickedCmdok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


BOOL ApprovalManage::OnInitDialog()
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

int  ApprovalManage::Init(void){
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int  i;
	char buf[256];
	sta=InitMySQL(&host);//����MySQL���ݿ�
	if(sta==TRUE){
		mysql_query(&host.mysql,"SELECT operator.ID,`Name`,`No`,`user`,`right` "
		" FROM operator INNER JOIN grade ON operator.grade = grade.ID Where (`right`='0' Or `right`='2')Limit 1;");
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
				"���������룺\r\n��Ա��𣺽�ʦ\r\n�༶��%s\r\nѧ�ţ�%s\r\n������%s\r\n",row[1],row[2],row[3]);
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
