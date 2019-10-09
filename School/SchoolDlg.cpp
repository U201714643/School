
// SchoolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Support.h"
#include "School.h"
#include "SchoolDlg.h"
#include "Students.h"
#include "admin.h"
#include "NewUser.h"
#include "Teacher.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int GetMySQLsta(void);	//������MySQL���ݿ�
int GetName(char * Name,int len);	//�������û���
int SaveName(char * Name);	//�����û���
int VerifyOperator(char * name,char * pwd);	//У���û�


global gs;	//�����ҽ�����һ��ȫ�ֱ�����


// CSchoolDlg �Ի���




CSchoolDlg::CSchoolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSchoolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSchoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditPassword, TxtPwd);
	DDX_Control(pDX, IDC_EditUser, TxtName);
	DDX_Control(pDX, IDCmdLogin, CmdLogin);
}

BEGIN_MESSAGE_MAP(CSchoolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSchoolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSchoolDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDCmdQuit, &CSchoolDlg::OnBnClickedCmdquit)
	ON_BN_CLICKED(IDCmdLogin, &CSchoolDlg::OnBnClickedCmdlogin)
	ON_BN_CLICKED(IDCmdNewUser, &CSchoolDlg::OnBnClickedCmdnewuser)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSchoolDlg ��Ϣ�������

BOOL CSchoolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	char name[72];
	GetName(name,sizeof(name));	//�������û���
	TxtName.SetWindowTextA(name);
	int sta=GetMySQLsta();
	if(sta!=TRUE){
		MessageBoxA("���ݿ����Ӳ���ʧ�ܣ���������","����",MB_TOPMOST);
		CDialogEx::OnOK();
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSchoolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSchoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSchoolDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void CSchoolDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void CSchoolDlg::OnBnClickedCmdquit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CSchoolDlg::OnBnClickedCmdlogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sta;	//״̬��־
	char UserBuf[72];
	char PwdBuf[72];
	int NameLength,PwdLength;
	//--------���������û���������--------
	TxtName.GetWindowTextA(UserBuf,sizeof(UserBuf));
	TxtPwd.GetWindowTextA(PwdBuf,sizeof(PwdBuf));
	//--------������ǵĳ���--------
	NameLength=strlen(UserBuf);
	PwdLength=strlen(PwdBuf);
	//--------���鳤��--------
	if(NameLength<4||PwdLength<4){	//���Ⱦ�����Ϊ4
		MessageBoxA("�û�������������ٺ���2�����ֻ�4��Ӣ����ĸ�������֣�","��¼ʧ��");
		return;		//���ٷ������ݿ�
	}
	//--------�����ݿ��в����û�--------
	sta=VerifyOperator(UserBuf,PwdBuf);
	if(sta==TRUE){
		CDialogEx::OnOK();
		if(gs.op.right==0)			//δ��֤
			MessageBoxA("ѧ����ע�ᣬ��δ����\n����������ٵ�¼","δ����ѧ��");
		else if(gs.op.right==2)		//δ��֤
			MessageBoxA("��ʦ��ע�ᣬ��δ����\n����������ٵ�¼","δ������ʦ");
		else if(gs.op.right==1){	//ѧ��
			Students frm;	
			frm.DoModal();
		}
		else if(gs.op.right==3){	//��ʦ
			Teacher frm;	
			frm.DoModal();
		}
		else if(gs.op.right==9){	//����Ա
			Admin	FrmAdmin;
			FrmAdmin.DoModal();		
		}
		else
			MessageBoxA("δ�������Ա����","��¼��Ч");
	}
	else
		MessageBoxA("��¼ʧ�ܣ���˶��û���������","��¼ʧ��");
}


int GetMySQLsta(void){	//������MySQL���ݿ�
	MySQLHostVariable host;
	MYSQL_RES *result;
	int i,pwd,sta;
	char cmd[256];
	for(i=0;i<4;i++)
	{
		sta=InitMySQL(&host);//����MySQL���ݿ�
		if(sta==TRUE)
			break;
	}
	if(sta==TRUE){
		//����Ƿ����admin�û�
		mysql_query(&host.mysql,"Select `ID` From `Operator` Where `No`='admin' and `Right`='9'");
		result = mysql_store_result(&host.mysql);
		if(result!=NULL){
			i=(long)result->row_count;//����
			mysql_free_result(result);	
		}
		else
			i=0;
		if(i!=1){//���admin�û������ڣ�������һ��admin�û�
			pwd=PwdCode("admin","admin");
			sprintf_s(cmd,sizeof(cmd),"Insert Into `Operator` Set `No`='admin',`user`='admin',`Grade`=0,"
			"`Password`='%d',`Right`='9';",pwd);
			mysql_query(&host.mysql,cmd);
		}
		CloseMySQL(&host);	//�ر�MySQL����	
	}
	return sta;
}

int GetName(char * Name,int len){	//�������û���
	char path[128]; //���·���ı���	
	GetModuleFileName(NULL,path,sizeof(path));     //��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	memcpy(path+strlen(path)-4,".INI",5);
	GetPrivateProfileString("School","UserName","",Name,len,path);
	return TRUE;
}

int SaveName(char * Name){	//�����û���
	char path[128]; //���·���ı���	
	GetModuleFileName(NULL,path,sizeof(path));     //��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	memcpy(path+strlen(path)-4,".INI",5);
	WritePrivateProfileStringA("School","UserName",Name,path);
	return TRUE;
}


int VerifyOperator(char * name,char * password){	//У���û�
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	long pwd;
	int sta;	//״̬��־
	char buf[200];
	int i;
	sta=InitMySQL(&host);//����MySQL���ݿ�
	if(sta==TRUE){
		pwd=PwdCode(name,password);
		sprintf_s(buf,sizeof(buf),"Select operator.`ID`,`Grade`,`No`,`Right`, grade.`Name` From `operator` "
		"Left Join Grade ON operator.grade = grade.ID Where `User`='%s' And `Password`='%ld';",name,pwd);
		mysql_query(&host.mysql,buf);
		result = mysql_store_result(&host.mysql);
		if(result!=NULL)
			i=(long)result->row_count;//����
		else
			i=0;
		if(i==1){	
			//У��ͨ�������û����ݶ���ȫ�ֱ���gs.op��
			row = mysql_fetch_row(result);
			gs.op.ID=atoi(row[0]);
			gs.op.grade=atoi(row[1]);
			strcpy_s(gs.op.No,sizeof(gs.op.No),row[2]);
			gs.op.right=atoi(row[3]);
			strcpy_s(gs.op.Name,sizeof(gs.op.Name),name);
			if(row[4]==NULL)	//�༶���ƿ�����Ч�������ʦû�������༶
				gs.op.GradeName[0]=0;	//��ʱ�򽫰༶��������Ϊ""�ַ���
			else
				strcpy_s(gs.op.GradeName,sizeof(gs.op.GradeName),row[4]);
			gs.op.pwd=pwd;
			SaveName(name);	//�����û���
		}
		else
			sta=FALSE;
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
	};
	return sta;
}

void CSchoolDlg::OnBnClickedCmdnewuser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NewUser f;
	f.DoModal();
}


void CSchoolDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnCancel();
}
