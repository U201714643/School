
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

int GetMySQLsta(void);		//������MySQL���ݿ�
int SaveName(char * Name);	//�����û���
int SavePwd(char * Pwd);	//��������
int GetName(char * Name, int len);	//�������û���
int GetPwd(char * Pwd, int len);	//����������

global gs;	//�����ҽ�����һ��ȫ�ֱ���������¼�û���Ϣ

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
	DDX_Control(pDX, IDC_REMID, RemID);
	DDX_Control(pDX, IDC_RMEPWD, RemPwd);
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
	ON_BN_CLICKED(IDC_REMID, &CSchoolDlg::OnBnClickedRemid)
	ON_BN_CLICKED(IDC_RMEPWD, &CSchoolDlg::OnBnClickedRmepwd)
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
	//-------��֮ǰ������û�������-------
	char buf[72];
	GetName(buf, sizeof(buf));	//�������û���
	TxtName.SetWindowTextA(buf);
	if (strlen(buf) != 0) {		//�ϴα������û��������Ȼ����
		RemID.SetCheck(true);
	}
	GetPwd(buf, sizeof(buf));	//�������û���
	TxtPwd.SetWindowTextA(buf);
	if (strlen(buf) != 0) {		//�ϴα������û��������Ȼ����
		RemPwd.SetCheck(true);
	}
	//-------���������ݿ�-------
	int sta = GetMySQLsta();
	if (sta != TRUE) {
		MessageBoxA("���ݿ����Ӳ���ʧ�ܣ���������", "����", MB_TOPMOST);
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
	//�����¼ʱ
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sta;	//״̬��־
	char UserBuf[72];
	char PwdBuf[72];
	int NameLength, PwdLength;
	//--------���������û���������--------
	TxtName.GetWindowTextA(UserBuf, sizeof(UserBuf));
	TxtPwd.GetWindowTextA(PwdBuf, sizeof(PwdBuf));
	//--------������ǵĳ���--------
	NameLength = strlen(UserBuf);
	PwdLength = strlen(PwdBuf);
	//--------���鳤��--------
	if (NameLength < 4 || PwdLength < 4) {	//���Ⱦ�����Ϊ4
		MessageBoxA("�û�������������ٺ���2�����ֻ�4��Ӣ����ĸ�������֣���", "��¼ʧ��");
		return;		//���ٷ������ݿ�
	}
	else if (NameLength > 20 || PwdLength > 20) {	//���Ⱦ�������20
		MessageBoxA("�û��������볤�Ⱦ�������10�����ֻ�20��Ӣ����ĸ�������֣���", "��¼ʧ��");
		return;		//���ٷ������ݿ�
	}
	//--------�����ݿ��в����û�--------
	sta = VerifyOperator(UserBuf, PwdBuf);
	if (sta == TRUE) {
		CDialogEx::OnOK();		//�ص���ǰ����
		//--------�����û���������--------
		//��½�ɹ��ű���
		if (RemID.GetCheck() == true) {	//ѡ�˲ű���
			SaveName(UserBuf);
		}
		else {	//ûѡ�����
			SaveName("");
		}
		if (RemPwd.GetCheck() == true) {//ѡ�˲ű���
			SavePwd(PwdBuf);
		}
		else {	//ûѡ�����
			SavePwd("");
		}
		//--------Ȩ����֤--------
		if (gs.op.Right == 0)			//δ��֤
			MessageBoxA("ѧ����ע�ᣬ��δ����\n����������ٵ�¼", "δ����ѧ��");
		else if (gs.op.Right == 2)		//δ��֤
			MessageBoxA("��ʦ��ע�ᣬ��δ����\n����������ٵ�¼", "δ������ʦ");
		else if (gs.op.Right == 1) {	//ѧ��
			Students frm;
			frm.DoModal();
		}
		else if (gs.op.Right == 3) {	//��ʦ
			Teacher frm;
			frm.DoModal();
		}
		else if (gs.op.Right == 9) {	//����Ա
			Admin	FrmAdmin;
			FrmAdmin.DoModal();
		}
		else
			MessageBoxA("δ�������Ա����", "��¼��Ч");
	}
	else   //���벻��
		MessageBoxA("��¼ʧ�ܣ���˶��û��������롣\n�����������������Ա��ϵ��", "��¼ʧ��");
}


int GetMySQLsta(void) {	//������MySQL���ݿ�
	MySQLHostVariable host;
	MYSQL_RES *result;
	int i, pwd, sta;
	char cmd[256];
	for (i = 0; i < 4; i++)
	{	//��������4��
		sta = InitMySQL(&host);//����MySQL���ݿ�
		if (sta == TRUE)
			break;
	}
	if (sta == TRUE) {
		//����Ƿ����admin�û�
		mysql_query(&host.mysql, "Select `ID` From `Operator` Where `No`='admin' and `Right`='9'");
		result = mysql_store_result(&host.mysql);
		if (result != NULL) {
			i = (long)result->row_count;//����
			mysql_free_result(result);
		}
		else
			i = 0;
		if (i != 1) {	//���admin�û������ڣ�������һ��admin�û�
			pwd = PwdCode("admin", "admin");
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Operator` Set `No`='admin',`user`='admin',`Grade`=0,"
				"`Password`='%d',`Right`='9';", pwd);
			mysql_query(&host.mysql, cmd);
		}
		CloseMySQL(&host);	//�ر�MySQL����	
	}
	return sta;
}

int GetName(char * Name, int len) {	//�������û���
	char path[128]; //���·���ı���	
	GetModuleFileName(NULL, path, sizeof(path));     //��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	memcpy(path + strlen(path) - 4, ".ini", 5);
	GetPrivateProfileString("School", "UserName", "", Name, len, path);
	return TRUE;
}

int GetPwd(char * Pwd, int len) {	//������������
	char path[128]; //���·���ı���	
	GetModuleFileName(NULL, path, sizeof(path));     //��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	memcpy(path + strlen(path) - 4, ".ini", 5);
	GetPrivateProfileString("School", "UserPwd", "", Pwd, len, path);
	return TRUE;
}

int SaveName(char * Name) {	//�����û���
	char path[128]; //���·���ı���	
	GetModuleFileName(NULL, path, sizeof(path));     //��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	memcpy(path + strlen(path) - 4, ".ini", 5);
	WritePrivateProfileStringA("School", "UserName", Name, path);
	return TRUE;
}

int SavePwd(char * Pwd) {	//��������
	char path[128]; //���·���ı���	
	GetModuleFileName(NULL, path, sizeof(path));     //��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	memcpy(path + strlen(path) - 4, ".ini", 5);
	WritePrivateProfileStringA("School", "UserPwd", Pwd, path);
	return TRUE;
}




void CSchoolDlg::OnBnClickedCmdnewuser()
{
	//�������û�
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NewUser f;
	f.DoModal();
}


void CSchoolDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnCancel();
}


void CSchoolDlg::OnBnClickedRemid()
{
	//����˱����û�����ť
	if (RemID.GetCheck() == false) {
		RemPwd.SetCheck(false);		//����������뱣���û���
	}
}


void CSchoolDlg::OnBnClickedRmepwd()
{
	//����˱������밴ť
	if (RemPwd.GetCheck() == true) {
		RemID.SetCheck(true);		//����������뱣���û���
	}
}
