
// SchoolDlg.cpp : 实现文件
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

int GetMySQLsta(void);		//试连接MySQL数据库
int SaveName(char * Name);	//缓存用户名
int SavePwd(char * Pwd);	//缓存密码
int GetName(char * Name, int len);	//读缓存用户名
int GetPwd(char * Pwd, int len);	//读缓存密码

global gs;	//定义且仅定义一个全局变量。：登录用户信息

// CSchoolDlg 对话框

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


// CSchoolDlg 消息处理程序

BOOL CSchoolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//-------读之前保存的用户名密码-------
	char buf[72];
	GetName(buf, sizeof(buf));	//读缓存用户名
	TxtName.SetWindowTextA(buf);
	if (strlen(buf) != 0) {		//上次保存了用户名这次依然保存
		RemID.SetCheck(true);
	}
	GetPwd(buf, sizeof(buf));	//读缓存用户名
	TxtPwd.SetWindowTextA(buf);
	if (strlen(buf) != 0) {		//上次保存了用户名这次依然保存
		RemPwd.SetCheck(true);
	}
	//-------连接至数据库-------
	int sta = GetMySQLsta();
	if (sta != TRUE) {
		MessageBoxA("数据库连接测试失败，请检查配置", "故障", MB_TOPMOST);
		CDialogEx::OnOK();
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSchoolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSchoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSchoolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CSchoolDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void CSchoolDlg::OnBnClickedCmdquit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CSchoolDlg::OnBnClickedCmdlogin()
{
	//点击登录时
	// TODO: 在此添加控件通知处理程序代码
	int sta;	//状态标志
	char UserBuf[72];
	char PwdBuf[72];
	int NameLength, PwdLength;
	//--------获得输入的用户名、密码--------
	TxtName.GetWindowTextA(UserBuf, sizeof(UserBuf));
	TxtPwd.GetWindowTextA(PwdBuf, sizeof(PwdBuf));
	//--------获得它们的长度--------
	NameLength = strlen(UserBuf);
	PwdLength = strlen(PwdBuf);
	//--------检验长度--------
	if (NameLength < 4 || PwdLength < 4) {	//长度均至少为4
		MessageBoxA("用户名、密码均至少含有2个汉字或4个英文字母（含数字）。", "登录失败");
		return;		//不再访问数据库
	}
	else if (NameLength > 20 || PwdLength > 20) {	//长度均不超过20
		MessageBoxA("用户名、密码长度均不超过10个汉字或20个英文字母（含数字）。", "登录失败");
		return;		//不再访问数据库
	}
	//--------在数据库中查找用户--------
	sta = VerifyOperator(UserBuf, PwdBuf);
	if (sta == TRUE) {
		CDialogEx::OnOK();		//关掉当前窗口
		//--------保存用户名、密码--------
		//登陆成功才保存
		if (RemID.GetCheck() == true) {	//选了才保存
			SaveName(UserBuf);
		}
		else {	//没选则清空
			SaveName("");
		}
		if (RemPwd.GetCheck() == true) {//选了才保存
			SavePwd(PwdBuf);
		}
		else {	//没选则清空
			SavePwd("");
		}
		//--------权限认证--------
		if (gs.op.Right == 0)			//未认证
			MessageBoxA("学生已注册，尚未审批\n请待审批后再登录", "未审批学生");
		else if (gs.op.Right == 2)		//未认证
			MessageBoxA("教师已注册，尚未审批\n请待审批后再登录", "未审批教师");
		else if (gs.op.Right == 1) {	//学生
			Students frm;
			frm.DoModal();
		}
		else if (gs.op.Right == 3) {	//教师
			Teacher frm;
			frm.DoModal();
		}
		else if (gs.op.Right == 9) {	//管理员
			Admin	FrmAdmin;
			FrmAdmin.DoModal();
		}
		else
			MessageBoxA("未定义操作员类型", "登录无效");
	}
	else   //密码不对
		MessageBoxA("登录失败，请核对用户名、密码。\n如遗忘密码请与管理员联系。", "登录失败");
}


int GetMySQLsta(void) {	//试连接MySQL数据库
	MySQLHostVariable host;
	MYSQL_RES *result;
	int i, pwd, sta;
	char cmd[256];
	for (i = 0; i < 4; i++)
	{	//至多重连4次
		sta = InitMySQL(&host);//连接MySQL数据库
		if (sta == TRUE)
			break;
	}
	if (sta == TRUE) {
		//检查是否存在admin用户
		mysql_query(&host.mysql, "Select `ID` From `Operator` Where `No`='admin' and `Right`='9'");
		result = mysql_store_result(&host.mysql);
		if (result != NULL) {
			i = (long)result->row_count;//总数
			mysql_free_result(result);
		}
		else
			i = 0;
		if (i != 1) {	//如果admin用户不存在，则增加一个admin用户
			pwd = PwdCode("admin", "admin");
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Operator` Set `No`='admin',`user`='admin',`Grade`=0,"
				"`Password`='%d',`Right`='9';", pwd);
			mysql_query(&host.mysql, cmd);
		}
		CloseMySQL(&host);	//关闭MySQL连接	
	}
	return sta;
}

int GetName(char * Name, int len) {	//读缓存用户名
	char path[128]; //存放路径的变量	
	GetModuleFileName(NULL, path, sizeof(path));     //获取当前进程已加载模块的文件的完整路径
	memcpy(path + strlen(path) - 4, ".ini", 5);
	GetPrivateProfileString("School", "UserName", "", Name, len, path);
	return TRUE;
}

int GetPwd(char * Pwd, int len) {	//读缓存用密码
	char path[128]; //存放路径的变量	
	GetModuleFileName(NULL, path, sizeof(path));     //获取当前进程已加载模块的文件的完整路径
	memcpy(path + strlen(path) - 4, ".ini", 5);
	GetPrivateProfileString("School", "UserPwd", "", Pwd, len, path);
	return TRUE;
}

int SaveName(char * Name) {	//缓存用户名
	char path[128]; //存放路径的变量	
	GetModuleFileName(NULL, path, sizeof(path));     //获取当前进程已加载模块的文件的完整路径
	memcpy(path + strlen(path) - 4, ".ini", 5);
	WritePrivateProfileStringA("School", "UserName", Name, path);
	return TRUE;
}

int SavePwd(char * Pwd) {	//缓存密码
	char path[128]; //存放路径的变量	
	GetModuleFileName(NULL, path, sizeof(path));     //获取当前进程已加载模块的文件的完整路径
	memcpy(path + strlen(path) - 4, ".ini", 5);
	WritePrivateProfileStringA("School", "UserPwd", Pwd, path);
	return TRUE;
}




void CSchoolDlg::OnBnClickedCmdnewuser()
{
	//创建新用户
	// TODO: 在此添加控件通知处理程序代码
	NewUser f;
	f.DoModal();
}


void CSchoolDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnCancel();
}


void CSchoolDlg::OnBnClickedRemid()
{
	//点击了保存用户名按钮
	if (RemID.GetCheck() == false) {
		RemPwd.SetCheck(false);		//保存密码必须保存用户名
	}
}


void CSchoolDlg::OnBnClickedRmepwd()
{
	//点击了保存密码按钮
	if (RemPwd.GetCheck() == true) {
		RemID.SetCheck(true);		//保存密码必须保存用户名
	}
}
