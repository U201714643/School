
// prepareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "prepare.h"
#include "prepareDlg.h"
#include "afxdialogex.h"
#include "include\mysql.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  MYSQLUSER "root"
#define  MYSQLPWD "19990220"
#define  MYSQLDBS "school"



typedef struct MySQLHostVariable
{
	MYSQL mysql;
	char host[31];	//MySQ服务器IP
}MySQLHostVariable;


typedef struct PrepareCtrl{
	int run;
	long operNo;	//操作员数数量
	int * operators;//操作员列表
	long CourseNo;	//课程数量
	int * Course;	//课程列表
	int OperNow;	//当前模拟的操作员
	int CourNow;	//当前模拟的课程
}PrepareCtrl;

int InitMySQL(MySQLHostVariable * host);	//初始化MySQL连接
int ValidIP(char * str);	//校验IP地址合法性


PrepareCtrl ls;

// CprepareDlg 对话框




CprepareDlg::CprepareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CprepareDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CprepareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_CmdQuit, cmdQuit);
	DDX_Control(pDX, IDC_TEXT, txtNote);
}

BEGIN_MESSAGE_MAP(CprepareDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CprepareDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CprepareDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_CmdQuit, &CprepareDlg::OnBnClickedCmdquit)
	ON_BN_CLICKED(ID_CmdStart, &CprepareDlg::OnBnClickedCmdstart)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_CmdEnd, &CprepareDlg::OnBnClickedCmdend)
END_MESSAGE_MAP()


// CprepareDlg 消息处理程序

BOOL CprepareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	memset(&ls,0,sizeof(ls));
	ls.run=0;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CprepareDlg::OnPaint()
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
HCURSOR CprepareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CprepareDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CprepareDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void CprepareDlg::OnBnClickedCmdquit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CprepareDlg::OnBnClickedCmdstart()
{
	// TODO: 在此添加控件通知处理程序代码
	ls.run=1;
	SetTimer(1,100,NULL);
	cmdQuit.EnableWindow(FALSE);

}


int CloseMySQL(MySQLHostVariable * host)//关闭MySQL连接
{	
	mysql_close(&host->mysql);
	return 0;
}

int InitMySQL(MySQLHostVariable * host)	//初始化MySQL连接
{	//初始MySQL连接
	int nConnectTimeout = 5, nWriteReadTimeout = 5;
	char hostip[128];
	int i;
	MYSQL* sap;
	char path[128]; //存放路径的变量
	mysql_init(&host->mysql);
	mysql_options(&host->mysql, MYSQL_SET_CHARSET_NAME, "gb2312");//必须要在本函数全局定义 
	mysql_options(&host->mysql, MYSQL_OPT_CONNECT_TIMEOUT, &nConnectTimeout);
	mysql_options(&host->mysql, MYSQL_OPT_READ_TIMEOUT, &nWriteReadTimeout);
	mysql_options(&host->mysql, MYSQL_OPT_WRITE_TIMEOUT, &nWriteReadTimeout);
	GetModuleFileName(NULL,path,sizeof(path));     //获取当前进程已加载模块的文件的完整路径
	memcpy(path+strlen(path)-4,".INI",5);
	GetPrivateProfileString("MySQL","MySQL","",hostip,sizeof(hostip),path);
	if (ValidIP(hostip)!=TRUE){	//IP地址非法，用127.0.0.1代替并写INI文件
		strcpy_s(hostip,sizeof(hostip),"127.0.0.1");
		WritePrivateProfileStringA("MySQL","MySQL",hostip,path);	
	}
	sap=mysql_real_connect(&host->mysql, hostip, MYSQLUSER, MYSQLPWD, MYSQLDBS, 3306, NULL, 0);

	if(sap==NULL)
			return FALSE;
	i=sizeof(host->host);
	strcpy_s(host->host,i,hostip);
	return TRUE;
}

int ValidIP(char * str)	//校验IP地址合法性
{
	int a,b,c,d; 
	int ret=TRUE; 
    ret=sscanf_s(str,"%d.%d.%d.%d",&a,&b,&c,&d); 
	if(ret==4&&(a>=1&&a<=255)&&(b>=0&&b<=255)&&(c>=0&&c<=255)&&(d>=1&&d<=254)) 
		return TRUE;
	else 
		return FALSE;      
}


int Prepare(void){
	static int check=0;
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	char cmd[200];
	int i,j;
	int e,c;
	long examID;


	sta=InitMySQL(&host);//连接MySQL数据库
	if(sta!=TRUE) 
		return FALSE;
	if(ls.CourseNo==0){//首次调用时，初始化课程列表
		mysql_query(&host.mysql,"Select `Course` from `Course`");
		result = mysql_store_result(&host.mysql);
		ls.CourseNo=(long)result->row_count;
		if(ls.Course!=NULL)	free(ls.Course);
		ls.Course=(int *)malloc(ls.CourseNo*sizeof(int));
		for(i=0;i<ls.CourseNo;i++)
		{
			row = mysql_fetch_row(result);
			ls.Course[i]=atoi(row[0]);
		}
		mysql_free_result(result);
		ls.CourNow=(int)(GetTickCount()%ls.CourseNo);//随机指定当前模拟课程
	}
	if(ls.operNo==0){//首次调用时，初始化学生列表
		mysql_query(&host.mysql,"Select `ID` from `operator` Where `Right`='1';");
		result = mysql_store_result(&host.mysql);
		ls.operNo=(long)result->row_count;
		if(ls.operators!=NULL)	free(ls.operators);
		ls.operators=(int *)malloc(ls.operNo*sizeof(int));
		for(i=0;i<ls.operNo;i++)
		{
			row = mysql_fetch_row(result);
			ls.operators[i]=atoi(row[0]);
		}
		mysql_free_result(result);
		ls.OperNow=(int)((GetTickCount()^ls.CourNow)%ls.operNo);//随机指定当前模拟人员
	}
	ls.CourNow=(ls.CourNow+1)%ls.CourseNo;
	ls.OperNow=(ls.OperNow+1)%ls.operNo;
	sprintf_s(cmd,sizeof(cmd),"Select `ID`,`Answer` From `Questions` Where `Course`='%d';",ls.Course[ls.CourNow]);
	mysql_query(&host.mysql,cmd);
	result = mysql_store_result(&host.mysql);
	if(result!=NULL)
		j=(long)result->row_count;//计数
	else
		j=0;
	j=min(j,30);	
	if(j!=30){		//该课程无足够题目
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接
		return FALSE;		
	}
	sprintf_s(cmd,sizeof(cmd),"Insert Into `Exam` (`TimesTamp`,`Examms`,`CourseID`,`OperatorID`,`Correct`,`Error`,`Score`)"
		" Values (now(),'1','%d','%d','0','0','0');",ls.Course[ls.CourNow],ls.operators[ls.OperNow]);
	mysql_query(&host.mysql,cmd);	//记录测验概况
	examID=(long)host.mysql.insert_id;
	for(e=0,c=0,i=0;i<30;i++){
		row = mysql_fetch_row(result);
		for(j=0;row[0][j]!=0;j++)
			check=(check<<1)^row[0][j];
		for(j=0;row[1][j]!=0;j++)
			check=(check<<1)^row[1][j];//随机决定当前题目是否正确
		if(check<0) check=-check;
		if((check%10)>8){	//20%几率答错
			sprintf_s(cmd,sizeof(cmd),"Insert Into `ExamDetails`(`ExamID`,`QuestID`,`UserAnswer`,`Correct`,`Error`)"
			"Values ('%ld','%s','%s','0','1');",examID,row[0],"00");
			e++;
		}
		else{	//90%几率答对
			sprintf_s(cmd,sizeof(cmd),"Insert Into `ExamDetails`(`ExamID`,`QuestID`,`UserAnswer`,`Correct`,`Error`)"
			"Values ('%ld','%s','%s','1','0');",examID,row[0],row[1]);
			c++;
		}
		mysql_query(&host.mysql,cmd);	//记录测验详情	
	}
	sprintf_s(cmd,sizeof(cmd),"Update `Exam` Set `Correct`='%d',`Error`='%d',`Score`='%5.3f' "
	" Where `ID`='%d';",c,e,double(100.0*c/(c+e+0.0)),examID);
	mysql_query(&host.mysql,cmd);	//补充测验概况
	CloseMySQL(&host);	//关闭MySQL连接	
	return TRUE;
}

void CprepareDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(ls.run==1)
		Prepare();
	if(ls.run==0){	//清理现场准备退出
		if(ls.Course!=NULL)
			free(ls.Course);
		ls.Course=NULL;
		ls.CourseNo=0;
		if(ls.operators!=NULL)
			free(ls.operators);
		ls.operators=NULL;
		ls.operNo=0;
		cmdQuit.EnableWindow(TRUE);	//使能退出按钮
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CprepareDlg::OnBnClickedCmdend()
{
	// TODO: 在此添加控件通知处理程序代码
	ls.run=0;
}
