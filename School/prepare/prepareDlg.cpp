
// prepareDlg.cpp : ʵ���ļ�
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
	char host[31];	//MySQ������IP
}MySQLHostVariable;


typedef struct PrepareCtrl{
	int run;
	long operNo;	//����Ա������
	int * operators;//����Ա�б�
	long CourseNo;	//�γ�����
	int * Course;	//�γ��б�
	int OperNow;	//��ǰģ��Ĳ���Ա
	int CourNow;	//��ǰģ��Ŀγ�
}PrepareCtrl;

int InitMySQL(MySQLHostVariable * host);	//��ʼ��MySQL����
int ValidIP(char * str);	//У��IP��ַ�Ϸ���


PrepareCtrl ls;

// CprepareDlg �Ի���




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


// CprepareDlg ��Ϣ�������

BOOL CprepareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	memset(&ls,0,sizeof(ls));
	ls.run=0;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CprepareDlg::OnPaint()
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
HCURSOR CprepareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CprepareDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void CprepareDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void CprepareDlg::OnBnClickedCmdquit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CprepareDlg::OnBnClickedCmdstart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ls.run=1;
	SetTimer(1,100,NULL);
	cmdQuit.EnableWindow(FALSE);

}


int CloseMySQL(MySQLHostVariable * host)//�ر�MySQL����
{	
	mysql_close(&host->mysql);
	return 0;
}

int InitMySQL(MySQLHostVariable * host)	//��ʼ��MySQL����
{	//��ʼMySQL����
	int nConnectTimeout = 5, nWriteReadTimeout = 5;
	char hostip[128];
	int i;
	MYSQL* sap;
	char path[128]; //���·���ı���
	mysql_init(&host->mysql);
	mysql_options(&host->mysql, MYSQL_SET_CHARSET_NAME, "gb2312");//����Ҫ�ڱ�����ȫ�ֶ��� 
	mysql_options(&host->mysql, MYSQL_OPT_CONNECT_TIMEOUT, &nConnectTimeout);
	mysql_options(&host->mysql, MYSQL_OPT_READ_TIMEOUT, &nWriteReadTimeout);
	mysql_options(&host->mysql, MYSQL_OPT_WRITE_TIMEOUT, &nWriteReadTimeout);
	GetModuleFileName(NULL,path,sizeof(path));     //��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	memcpy(path+strlen(path)-4,".INI",5);
	GetPrivateProfileString("MySQL","MySQL","",hostip,sizeof(hostip),path);
	if (ValidIP(hostip)!=TRUE){	//IP��ַ�Ƿ�����127.0.0.1���沢дINI�ļ�
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

int ValidIP(char * str)	//У��IP��ַ�Ϸ���
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
	int sta;	//״̬��־
	char cmd[200];
	int i,j;
	int e,c;
	long examID;


	sta=InitMySQL(&host);//����MySQL���ݿ�
	if(sta!=TRUE) 
		return FALSE;
	if(ls.CourseNo==0){//�״ε���ʱ����ʼ���γ��б�
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
		ls.CourNow=(int)(GetTickCount()%ls.CourseNo);//���ָ����ǰģ��γ�
	}
	if(ls.operNo==0){//�״ε���ʱ����ʼ��ѧ���б�
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
		ls.OperNow=(int)((GetTickCount()^ls.CourNow)%ls.operNo);//���ָ����ǰģ����Ա
	}
	ls.CourNow=(ls.CourNow+1)%ls.CourseNo;
	ls.OperNow=(ls.OperNow+1)%ls.operNo;
	sprintf_s(cmd,sizeof(cmd),"Select `ID`,`Answer` From `Questions` Where `Course`='%d';",ls.Course[ls.CourNow]);
	mysql_query(&host.mysql,cmd);
	result = mysql_store_result(&host.mysql);
	if(result!=NULL)
		j=(long)result->row_count;//����
	else
		j=0;
	j=min(j,30);	
	if(j!=30){		//�ÿγ����㹻��Ŀ
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����
		return FALSE;		
	}
	sprintf_s(cmd,sizeof(cmd),"Insert Into `Exam` (`TimesTamp`,`Examms`,`CourseID`,`OperatorID`,`Correct`,`Error`,`Score`)"
		" Values (now(),'1','%d','%d','0','0','0');",ls.Course[ls.CourNow],ls.operators[ls.OperNow]);
	mysql_query(&host.mysql,cmd);	//��¼����ſ�
	examID=(long)host.mysql.insert_id;
	for(e=0,c=0,i=0;i<30;i++){
		row = mysql_fetch_row(result);
		for(j=0;row[0][j]!=0;j++)
			check=(check<<1)^row[0][j];
		for(j=0;row[1][j]!=0;j++)
			check=(check<<1)^row[1][j];//���������ǰ��Ŀ�Ƿ���ȷ
		if(check<0) check=-check;
		if((check%10)>8){	//20%���ʴ��
			sprintf_s(cmd,sizeof(cmd),"Insert Into `ExamDetails`(`ExamID`,`QuestID`,`UserAnswer`,`Correct`,`Error`)"
			"Values ('%ld','%s','%s','0','1');",examID,row[0],"00");
			e++;
		}
		else{	//90%���ʴ��
			sprintf_s(cmd,sizeof(cmd),"Insert Into `ExamDetails`(`ExamID`,`QuestID`,`UserAnswer`,`Correct`,`Error`)"
			"Values ('%ld','%s','%s','1','0');",examID,row[0],row[1]);
			c++;
		}
		mysql_query(&host.mysql,cmd);	//��¼��������	
	}
	sprintf_s(cmd,sizeof(cmd),"Update `Exam` Set `Correct`='%d',`Error`='%d',`Score`='%5.3f' "
	" Where `ID`='%d';",c,e,double(100.0*c/(c+e+0.0)),examID);
	mysql_query(&host.mysql,cmd);	//�������ſ�
	CloseMySQL(&host);	//�ر�MySQL����	
	return TRUE;
}

void CprepareDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(ls.run==1)
		Prepare();
	if(ls.run==0){	//�����ֳ�׼���˳�
		if(ls.Course!=NULL)
			free(ls.Course);
		ls.Course=NULL;
		ls.CourseNo=0;
		if(ls.operators!=NULL)
			free(ls.operators);
		ls.operators=NULL;
		ls.operNo=0;
		cmdQuit.EnableWindow(TRUE);	//ʹ���˳���ť
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CprepareDlg::OnBnClickedCmdend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ls.run=0;
}
