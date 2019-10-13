// Students.cpp : 实现文件
//

#include "stdafx.h"
#include "School.h"
#include "Support.h"
#include "Students.h"
#include "NewPwd.h"
#include "afxdialogex.h"

#define  ModNone  0	 //模式-无
#define  ModStats 1	 //模式-统计
#define  ModExamA 2	 //模式-测验前
#define  ModExamB 3	 //模式-测验中
#define  ModExamC 4	 //模式-测验后
#define  ModReview 5 //模式-回顾

#define  ExamTest    0	 //答题方式-考试
#define  ExamTry     1   //答题方式-刷题
#define  ExamRetry  2	 //答题方式-错题本

#define  RankNum	0	 //排序方式-时间
#define  RankTime	1	 //排序方式-用时
#define  RankCourse	2	 //排序方式-课程
#define  RankOk		3	 //排序方式-正确数
#define  RankError	4	 //排序方式-错误数
#define  RankScore	5	 //排序方式-正确率


typedef struct studentslocal {	//student窗体共用变量
	int   WorkMode;	//ModNone-无,ModStats-统计,ModExamA-测验前,ModExamB-测验中,ModExamC-测验后,ModReview-回顾
	DWORD ExamStart;//测验开始时间(ms)
	int   RemainingSec;//测验剩余时间(S)
	long  ExamID;//测验记录ID
	int   ExamMs;//测验用时
	int   ExamMode;//答题方式
	int   CourseID;	//课程ID
	int   OpertorID;//学员ID
	int   Correct;//正解题目数
	int   Error;  //错题数
	double	Score;//得分
	question ques[30];	//当前测试题
}studentslocal;

static int ReadOrSkip(int now, int max, int need, int needmax, int mode);//决定是否读取当前记录
static int RndExchQues(int size, int tim);//随机交换试题次序

extern global gs;
studentslocal WinInf;


// Students 对话框

IMPLEMENT_DYNAMIC(Students, CDialogEx)

Students::Students(CWnd* pParent /*=NULL*/)
	: CDialogEx(Students::IDD, pParent)
{

}

Students::~Students()
{
}

void Students::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CmbCourse, CmbCourse);
	DDX_Control(pDX, IDC_TxtNote, TxtNote);
	DDX_Control(pDX, IDC_CmdTry, CmdTry);
	DDX_Control(pDX, IDC_CmdSubmit, CmdSubmit);
	DDX_Control(pDX, IDC_CmdQuit, CmdQuit);
	DDX_Control(pDX, IDC_LIST, cList);
	DDX_Control(pDX, IDC_TxtStats, cTxtStats);
	DDX_Control(pDX, IDC_TxtCourse, cTxtCourse);
	DDX_Control(pDX, IDC_CmdReview, CmdReview);
	DDX_Control(pDX, IDCmdStartTest, CmdStartTest);
	DDX_Control(pDX, IDC_TxtNote1, TxtNote1);
	DDX_Control(pDX, IDC_TxtNote2, TxtNote2);
}


BEGIN_MESSAGE_MAP(Students, CDialogEx)
	ON_BN_CLICKED(IDOK, &Students::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Students::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CmbCourse, &Students::OnCbnSelchangeCmbcourse)
	ON_BN_CLICKED(IDC_CmdSubmit, &Students::OnBnClickedCmdsubmit)
	ON_BN_CLICKED(IDC_CmdQuit, &Students::OnBnClickedCmdquit)
	ON_BN_CLICKED(IDC_CmdTry, &Students::OnBnClickedCmdtry)
	ON_COMMAND(ID_mnuExitStu, &Students::OnMnuExitStu)
	ON_COMMAND(ID_mnuExamStu, &Students::OnMnuExamTestStu)
	ON_COMMAND(ID_mnuExamTryStu, &Students::OnmnuExamTryStu)
	ON_COMMAND(ID_mnuStatsTryStu, &Students::OnmnuStatsTryStu)
	ON_COMMAND(ID_mnuStatsTestStu, &Students::OnmnuStatsTestStu)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CmdReview, &Students::OnBnClickedCmdreview)
	ON_COMMAND(ID_mnuNewPwd, &Students::OnmnuNewPwd)
	ON_WM_CLOSE()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &Students::OnHdnItemclickList)
	ON_BN_CLICKED(IDCmdStartTest, &Students::OnBnClickedCmdstarttest)
	ON_COMMAND(ID_mnuExamRetry, &Students::OnmnuExamRetry)
	ON_COMMAND(ID_mnuStatsRetryStu, &Students::OnmnuStatsRetryStu)
END_MESSAGE_MAP()


// Students 消息处理程序


void Students::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void Students::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


BOOL Students::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Init();
	return TRUE;
}
BOOL Students::Init() {	//初始化
	char buf[72];
	srand((unsigned int)GetTickCount());
	WinInf.OpertorID = gs.op.ID;
	sprintf_s(buf, sizeof(buf), "欢迎 %s-%s 同学", gs.op.GradeName, gs.op.Name);
	SetWindowTextA(buf);
	CmbCourse.SetWindowTextA("");
	CmbCourse.EnableWindow(TRUE);
	WinInf.WorkMode = ModNone;
	CreateCtrl();//设定控件位置
	ShowMode();
	return TRUE;
}

int Students::InitCmbCourse(void) {	//初始化课程下拉框
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	int i, j;
	int Mode;	//初始化模式
	char cmd[512];
	static int LastMode = -9999; //记住上次初始化模式
	if (WinInf.ExamMode == ExamRetry)
		Mode = 1;	//错题练习
	else
		Mode = 2;//全部课程
	if (Mode == LastMode)
		return TRUE;
	LastMode = Mode;
	for (; ((sta = CmbCourse.DeleteString(0)) > 0););
	//i=CmbCourse.GetCount();
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		if (Mode == 2)
			mysql_query(&host.mysql, "Select `Course`,`CourseName`  From `Course` order by `Course`");
		else {
			sprintf_s(cmd, sizeof(cmd), "SELECT course.Course,course.CourseName,sum(examdetails.Error),"
				"exam.OperatorID FROM examdetails INNER JOIN exam ON examdetails.ExamID = exam.ID "
				"INNER JOIN course ON exam.CourseID = course.Course WHERE exam.OperatorID='%d' and "
				"examdetails.Error>0 GROUP BY course.Course ORDER BY course.Course;", gs.op.ID);
			mysql_query(&host.mysql, cmd);
		}
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			CmbCourse.InsertString(i, row[1]);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
	};
	return sta;
}

void Students::OnCbnSelchangeCmbcourse()
{
	// TODO: 在此添加控件通知处理程序代码
	CmdStartTest.EnableWindow(TRUE);
}


void Students::CreateCtrl(void) {	////设定控件位置
	RECT RectItem;//控件位置
	RECT RectArea;//代码绘图区域
	RECT RectCtrl;//控件绘图区域
	int i, x, y;

	CmdSubmit.GetWindowRect(&RectItem);
	ScreenToClient(&RectItem);//获得相对于主窗体的坐标
	CmdReview.MoveWindow(&RectItem);//把“详情、返回”按钮放置在“提交按钮同一位置”

	cList.GetWindowRect(&RectArea);//在统计表格同一区域布置答题所用4种*30个控件
	ScreenToClient(&RectArea);//获得相对于主窗体的坐标

	for (i = 0; i < 30; i++) {
		x = i % 3; y = i / 3;
		RectCtrl.left = RectArea.left + (RectArea.right - RectArea.left) / 3 * x;
		RectCtrl.right = RectCtrl.left + (long)((RectArea.right - RectArea.left) / 3 * 0.60);
		RectCtrl.top = RectArea.top + (RectArea.bottom - RectArea.top) / 10 * y;
		RectCtrl.bottom = RectCtrl.top + (long)((RectArea.bottom - RectArea.top) / 10 * 0.4);
		if (Question[i].m_hWnd == NULL)
			Question[i].Create("题干", ES_RIGHT, RectCtrl, this, 15000 + i);

		RectCtrl.left = RectCtrl.right;
		RectCtrl.right = RectCtrl.left + (long)((RectArea.right - RectArea.left) / 3 * 0.15);
		if (Answer[i].m_hWnd == NULL)
			Answer[i].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, RectCtrl, this, 15100 + i);
		else
			Answer[i].SetWindowTextA("");

		RectCtrl.left = RectCtrl.right;
		RectCtrl.right = RectCtrl.left + (long)((RectArea.right - RectArea.left) / 3 * 0.20);
		if (Flag[i].m_hWnd == NULL)
			Flag[i].Create("正确", NULL, RectCtrl, this, 15200 + i);

		RectCtrl.top = RectCtrl.bottom;
		RectCtrl.bottom = RectCtrl.top + (long)((RectArea.bottom - RectArea.top) / 10 * 0.4);
		RectCtrl.right = RectCtrl.left + (long)((RectArea.right - RectArea.left) / 3 * 0.08);
		if (Prompt[i].m_hWnd == NULL)
			Prompt[i].Create("?", NULL, RectCtrl, this, 15300 + i);
	}
}

void Students::OnBnClickedCmdsubmit()
{
	// TODO: 在此添加控件通知处理程序代码
	int i, c, e;
	int mm, ss;
	char buf[72];
	KillTimer(1);
	WinInf.WorkMode = ModExamC;
	ShowMode();
	for (i = 0, c = 0, e = 0; i < 30; i++) {
		if (WinInf.ques[i].ID == 0) {
			Flag[i].ShowWindow(SW_HIDE);
			Prompt[i].ShowWindow(SW_HIDE);
			Answer[i].EnableWindow(FALSE);
			continue;
		}
		Answer[i].GetWindowTextA(buf, sizeof(buf));
		if ((buf[0] == 0) && ((WinInf.ExamMode == ExamTry) || (WinInf.ExamMode == ExamRetry))) {	//刷题或错题模式下的未答题目
			Flag[i].SetWindowTextA("未答");
			Prompt[i].ShowWindow(SW_SHOW);
			WinInf.ques[i].Correct = 0;
			WinInf.ques[i].Error = 0;

		}
		else {
			WinInf.ques[i].UserAnswer = atoi(buf);
			if (WinInf.ques[i].UserAnswer == WinInf.ques[i].Answer) {
				Flag[i].SetWindowTextA("正确");
				Prompt[i].ShowWindow(SW_HIDE);
				WinInf.ques[i].Correct = 1;
				WinInf.ques[i].Error = 0;
				c++;
			}
			else {
				Flag[i].SetWindowTextA("错误");
				Prompt[i].ShowWindow(SW_SHOW);
				WinInf.ques[i].Correct = 0;
				WinInf.ques[i].Error = 1;
				e++;
			}
		}
		Flag[i].ShowWindow(SW_SHOW);
		Answer[i].EnableWindow(FALSE);
	}
	WinInf.Error = e;
	WinInf.Correct = c;
	WinInf.ExamMs = (int)(GetTickCount() - WinInf.ExamStart);
	if ((c + e) > 0)
		WinInf.Score = 100.0*c / (c + e);
	else
		WinInf.Score = 0;
	ss = WinInf.ExamMs / 1000;
	mm = ss / 60;
	ss = ss % 60;
	sprintf_s(buf, sizeof(buf), "测试用时%d分%d秒。正确%d道，错误%d道，得分%5.2f", mm, ss, c, e, WinInf.Score);
	MessageBoxA(buf, "测试结果");
}

int  Students::WriteExam() {	//测试结果写MySQL数据库
	// TODO: 在此添加控件通知处理程序代码
	MySQLHostVariable host;
	int sta;	//状态标志
	char cmd[200];
	int i;
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		sprintf_s(cmd, sizeof(cmd), "Insert Into `Exam` (`TimesTamp`,`ExamType`,`Examms`,`CourseID`,`OperatorID`,`Correct`,`Error`,`Score`)"
			" Values (now(),'%d','%d','%d','%d','%d','%d','%lf');",
			WinInf.ExamMode, WinInf.ExamMs, WinInf.CourseID, WinInf.OpertorID, WinInf.Correct, WinInf.Error, WinInf.Score);
		mysql_query(&host.mysql, cmd);	//记录测验概况
		WinInf.ExamID = (long)host.mysql.insert_id;
		for (i = 0; i < 30; i++) {
			if (WinInf.ques[i].Error + WinInf.ques[i].Correct > 0) {
				sprintf_s(cmd, sizeof(cmd), "Insert Into `ExamDetails`(`ExamID`,`QuestID`,`UserAnswer`,`Correct`,`Error`)"
					"Values ('%d','%d','%d','%d','%d');",
					WinInf.ExamID, WinInf.ques[i].ID, WinInf.ques[i].UserAnswer, WinInf.ques[i].Correct, WinInf.ques[i].Error);
				mysql_query(&host.mysql, cmd);	//记录测验详情
			}
		}
		CloseMySQL(&host);	//关闭MySQL连接	
	};
	return TRUE;
}

void Students::OnBnClickedCmdquit()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteExam();	//测试结果写MySQL数据库
	WinInf.WorkMode = ModExamA;
	ShowMode();
}

void Students::OnBnClickedCmdtry()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	SetTimer(1, 1000, NULL);
	WinInf.WorkMode = ModExamB;
	ShowMode();
	for (i = 0; i < 30; i++) {
		Answer[i].EnableWindow(TRUE);
	}

}

LRESULT Students::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	if (WM_COMMAND == message) {
		WORD wID = LOWORD(wParam) - 15300;
		if ((wID >= 0) && (wID <= 29)) {
			char buf[72];
			sprintf_s(buf, sizeof(buf), "%s%d", WinInf.ques[wID].Text, WinInf.ques[wID].Answer);
			MessageBoxA(buf, "提示", MB_TOPMOST);
		}
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void Students::OnMnuExamTestStu()
{
	// TODO: 在此添加命令处理程序代码
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("请先完成当前操作");
		return;
	}
	cTxtCourse.SetWindowTextA("请选择测试课程");
	WinInf.WorkMode = ModExamA;
	WinInf.ExamMode = ExamTest;
	ShowMode();
}

void Students::OnmnuExamTryStu()
{
	// TODO: 在此添加命令处理程序代码
	//ModNone-无,ModStats-统计,ModExamA-测验前,ModExamB-测验中,ModExamC-测验后,ModReview-回顾
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("请先完成当前操作");
		return;
	}
	cTxtCourse.SetWindowTextA("请选择自由练习课程");
	WinInf.WorkMode = ModExamA;
	WinInf.ExamMode = ExamTry;
	ShowMode();
}

void Students::OnmnuExamRetry() {
	// TODO: 在此添加命令处理程序代码
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("请先完成当前操作");
		return;
	}
	cTxtCourse.SetWindowTextA("请选择错题练习课程");
	WinInf.WorkMode = ModExamA;
	WinInf.ExamMode = ExamRetry;
	ShowMode();
}

void Students::OnmnuStatsTryStu()
{
	// TODO: 在此添加命令处理程序代码
	//ModNone-无,ModStats-统计,ModExamA-测验前,ModExamB-测验中,ModExamC-测验后,ModReview-回顾
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("请完成当前操作后再查询");
		return;
	}
	//cTxtCourse.SetWindowTextA("请选择练习课程");
	WinInf.ExamMode = ExamTry;
	StatsStu(RankNum, 0);	//默认按时间序号排序
}

void Students::OnmnuStatsTestStu()
{
	// TODO: 在此添加命令处理程序代码
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("请完成当前操作后再查询");
		return;
	}
	//cTxtCourse.SetWindowTextA("请选择测验课程");
	WinInf.ExamMode = ExamTest;
	StatsStu(RankNum, 0);	//默认按时间序号排序
}

void Students::OnMnuExitStu()
{
	// TODO: 在此添加命令处理程序代码
	CDialogEx::OnOK();
}


int  Students::StatsStu(int CmdNum, int ClickTime) {	//统计测验成绩
	//RankNum 0		//排序方式-时间   RankTime 1	//排序方式-用时
	//RankCourse 2	//排序方式-课程   RankOk 3		//排序方式-正确数
	//RankError	4	//排序方式-错误数 RankScore 5	//排序方式-正确率
	static char *RankType[6] = { "exam.ID","`Examms`","`CourseID`","`Correct`","`Error`","`Score`" };	//就是上面6种
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	int i, j, n;
	char cmd[512];
	//--------清空表格--------
	cList.DeleteAllItems();
	for (; cList.DeleteColumn(0) == TRUE;);
	//--------绘制框架--------
	WinInf.WorkMode = ModStats;
	ShowMode();
	//绘制表格标题
	cList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //设置list风格  
	cList.InsertColumn(0, "序号", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(1, "时间", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(2, "用时", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(3, "课程号", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(4, "课程", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(5, "学号", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(6, "正确", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(7, "错误", LVCFMT_CENTER, -1, -1);
	if (WinInf.ExamMode == ExamTest)
		cList.InsertColumn(8, "得分", LVCFMT_CENTER, -1, -1);
	cList.SetColumnWidth(0, 0);
	cList.SetColumnWidth(1, 120);
	cList.SetColumnWidth(2, 70);
	cList.SetColumnWidth(3, 0);
	cList.SetColumnWidth(4, 170);
	cList.SetColumnWidth(5, 0);
	cList.SetColumnWidth(6, 40);
	cList.SetColumnWidth(7, 40);
	if (WinInf.ExamMode == ExamTest)
		cList.SetColumnWidth(8, 60);
	//--------填写内容--------
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		if (ClickTime == 0) {	//初始为降序
			sprintf_s(cmd, sizeof(cmd), "SELECT exam.ID , `TimesTamp` , `Examms` , `CourseID` , `CourseName` ,"
				" `OperatorID`,`Correct`,`Error`,`Score` FROM `exam` INNER JOIN `course` ON exam.CourseID = course.Course "
				" Where OperatorID='%d' And `ExamType`='%d' ORDER BY %s DESC;", gs.op.ID, WinInf.ExamMode, RankType[CmdNum]);
		}
		else {	//再点一次为升序
			sprintf_s(cmd, sizeof(cmd), "SELECT exam.ID , `TimesTamp` , `Examms` , `CourseID` , `CourseName` ,"
				" `OperatorID`,`Correct`,`Error`,`Score` FROM `exam` INNER JOIN `course` ON exam.CourseID = course.Course "
				" Where OperatorID='%d' And `ExamType`='%d' ORDER BY %s ASC;", gs.op.ID, WinInf.ExamMode, RankType[CmdNum]);
		}
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			n = (long)result->row_count;//总数
		else
			n = 0;
		for (i = 0; i < n; i++) {
			row = mysql_fetch_row(result);
			char Time[32];	//装分：秒格式的时间
			cList.InsertItem(i, row[0]);
			cList.SetItemText(i, 1, row[1]);
			//消耗时间的格式需要转换
			for (j = 3; j <= 7; j++) {
				cList.SetItemText(i, j, row[j]);
			}
			cList.SetItemText(i, 2, MsToTime(row[2], Time));	//消耗的时间
			if (WinInf.ExamMode == ExamTest)		//只有考试才有分数
				cList.SetItemText(i, 8, row[8]);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
	};
	return TRUE;
}

int	Students::ShowMode() {		//切换显示模式
	//ModNone-无,ModStats-统计,ModExamA-测验前,ModExamB-测验中,ModExamC-测验后,ModReview-回顾
	int i;
	if (WinInf.WorkMode == ModStats) {
		cList.ShowWindow(SW_SHOW);
		cTxtStats.ShowWindow(SW_SHOW);
	}
	else {
		cList.ShowWindow(SW_HIDE);
		cTxtStats.ShowWindow(SW_HIDE);
	}

	if ((WinInf.WorkMode == ModStats) || (WinInf.WorkMode == ModReview)) {
		if (WinInf.WorkMode == ModStats)
			CmdReview.SetWindowTextA("查试卷");
		else
			CmdReview.SetWindowTextA("返 回");
		CmdReview.ShowWindow(SW_SHOW);
	}
	else
		CmdReview.ShowWindow(SW_HIDE);

	if ((WinInf.WorkMode == ModExamA)) {
		CmdStartTest.ShowWindow(SW_SHOW);
	}
	else {
		CmdStartTest.ShowWindow(SW_HIDE);
	}

	if ((WinInf.WorkMode == ModExamA) || (WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		cTxtCourse.ShowWindow(SW_SHOW);
		InitCmbCourse();
		CmbCourse.ShowWindow(SW_SHOW);
	}
	else {
		cTxtCourse.ShowWindow(SW_HIDE);
		CmbCourse.ShowWindow(SW_HIDE);
	}

	if (WinInf.WorkMode == ModExamA)
		CmbCourse.EnableWindow(TRUE);
	else
		CmbCourse.EnableWindow(FALSE);

	if (WinInf.WorkMode == ModExamB)
		CmdSubmit.ShowWindow(SW_SHOW);
	else
		CmdSubmit.ShowWindow(SW_HIDE);

	if (WinInf.WorkMode == ModExamC) {
		CmdTry.ShowWindow(SW_SHOW);
		if (WinInf.RemainingSec >= 0)
			CmdTry.EnableWindow(TRUE);
		else
			CmdTry.EnableWindow(FALSE);
		CmdQuit.ShowWindow(SW_SHOW);
		for (i = 0; i < 30; i++) {
			if (Flag[i].m_hWnd != NULL) Flag[i].ShowWindow(SW_SHOW);
			if (Prompt[i].m_hWnd != NULL) Prompt[i].ShowWindow(SW_SHOW);
		}
	}
	else {
		CmdTry.ShowWindow(SW_HIDE);
		CmdQuit.ShowWindow(SW_HIDE);
		for (i = 0; i < 30; i++) {
			if (Flag[i].m_hWnd != NULL) Flag[i].ShowWindow(SW_HIDE);
			if (Prompt[i].m_hWnd != NULL) Prompt[i].ShowWindow(SW_HIDE);
		}
	}

	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		for (i = 0; i < 30; i++) {
			if (Question[i].m_hWnd != NULL) Question[i].ShowWindow(SW_SHOW);
			if (Answer[i].m_hWnd != NULL) Answer[i].ShowWindow(SW_SHOW);
		}
		TxtNote1.ShowWindow(SW_SHOW);
		TxtNote2.ShowWindow(SW_SHOW);
		TxtNote.ShowWindow(SW_SHOW);
	}
	else {
		for (i = 0; i < 30; i++) {
			if (Question[i].m_hWnd != NULL) Question[i].ShowWindow(SW_HIDE);
			if (Answer[i].m_hWnd != NULL) Answer[i].ShowWindow(SW_HIDE);

		}
		TxtNote1.ShowWindow(SW_HIDE);
		TxtNote2.ShowWindow(SW_HIDE);
		TxtNote.ShowWindow(SW_HIDE);
	}
	return TRUE;
}

void Students::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	char buf[256];
	DWORD ExamNow = GetTickCount();
	int ExamSec = (int)((ExamNow - WinInf.ExamStart) / 1000);
	WinInf.RemainingSec = 1200 - ExamSec;
	int mme = ExamSec / 60;
	int sse = ExamSec % 60;
	int mmr = WinInf.RemainingSec / 60;
	int ssr = WinInf.RemainingSec % 60;
	sprintf_s(buf, sizeof(buf), "已用时间:%02d分%02d秒", mme, sse);
	TxtNote2.SetWindowTextA(buf);
	sprintf_s(buf, sizeof(buf), "剩余时间:%02d分%02d秒", mmr, ssr);
	TxtNote.SetWindowTextA(buf);
	if (WinInf.RemainingSec <= 0)
		OnBnClickedCmdsubmit();//时间到强制提交
	CDialogEx::OnTimer(nIDEvent);
}

void Students::OnBnClickedCmdreview()
{
	// TODO: 在此添加控件通知处理程序代码
	//ModNone-无,ModStats-统计,ModExamA-测验前,ModExamB-测验中,ModExamC-测验后,ModReview-回顾
	if (WinInf.WorkMode == ModReview) {	//按钮双重定义，回顾模式下再次点击为返回统计模式
		WinInf.WorkMode = ModStats;
		ShowMode();
		return;
	}
	POSITION pos;
	int i, j, sta;
	char buf[72], txt[72];
	int ExamID;
	pos = cList.GetFirstSelectedItemPosition();
	i = cList.GetNextSelectedItem(pos);
	if (i < 0) {
		MessageBoxA("请先选择测验", "提示");
		return;
	}
	cList.GetItemText(i, 0, buf, sizeof(buf));
	ExamID = atoi(buf);
	cList.GetItemText(i, 4, buf, sizeof(buf));
	InitCmbCourse();
	for (i = 0; (sta = CmbCourse.SetCurSel(i)) == i; i++)
	{
		CmbCourse.GetWindowTextA(txt, sizeof(txt));
		if (strcmp(txt, buf) == 0)
			break;
	}
	if (sta != i)
	{
		MessageBoxA("请先选择测验", "提示");
		return;
	}

	WinInf.WorkMode = ModReview;
	ShowMode();

	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char cmd[128];
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		sprintf_s(cmd, sizeof(cmd), "SELECT `ExamID`,`QuestID`,`UserAnswer` FROM `ExamDetails` "
			" Where `ExamID`='%d' ORDER BY QuestID ASC;", ExamID);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		if ((j < 1) || (j > 30)) {	//应该都是1-30道题目
			mysql_free_result(result);
			CloseMySQL(&host);	//关闭MySQL连接	
			MessageBoxA("请先选择测验", "提示");
			return;
		}
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			WinInf.ques[i].ID = atoi(row[1]);
			WinInf.ques[i].UserAnswer = atoi(row[2]);
			Answer[i].SetWindowTextA(row[2]);
			Answer[i].EnableWindow(FALSE);
		}
		mysql_free_result(result);
		for (i = j; i < 30; i++) {
			Question[i].SetWindowTextA("");
			Answer[i].SetWindowTextA("");
			Answer[i].EnableWindow(FALSE);
		}
		for (i = 0; i < j; i++) {
			sprintf_s(cmd, sizeof(cmd), "Select `ID`,`Text`,`Answer` From `Questions` Where `ID`='%d';", WinInf.ques[i].ID);
			mysql_query(&host.mysql, cmd);
			result = mysql_store_result(&host.mysql);
			ASSERT(result != NULL);
			row = mysql_fetch_row(result);
			ASSERT(row != NULL);
			strcpy_s(WinInf.ques[i].Text, sizeof(WinInf.ques[i].Text), row[1]);
			WinInf.ques[i].Answer = atoi(row[2]);
			for (int k = 0; k < sizeof(WinInf.ques[i].Text) - 1; k++) {
				if (WinInf.ques[i].Text[k] == 0) {//必要时为试题字符串尾部加'='字符
					if (WinInf.ques[i].Text[k - 1] == '=')
						break;
					else {
						WinInf.ques[i].Text[k] = '=';
						WinInf.ques[i].Text[k + 1] = 0;
					}
				}
			}
			Question[i].SetWindowTextA(RealChar(WinInf.ques[i].Text));//显示试题字符串
			if (WinInf.ques[i].Answer == WinInf.ques[i].UserAnswer) {
				Prompt[i].ShowWindow(SW_HIDE);
			}
			else {
				Flag[i].SetWindowTextA("错误");
				Flag[i].ShowWindow(SW_SHOW);
				Prompt[i].ShowWindow(SW_SHOW);
			}
			mysql_free_result(result);
		}
		CloseMySQL(&host);	//关闭MySQL连接	
	};
	return;
}

void Students::OnmnuNewPwd()
{
	// TODO: 在此添加命令处理程序代码
	NewPwd f;
	f.DoModal();
}


void Students::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnCancel();
}

void Students::OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	static int x = 0;			//防止执行后x没了
	static int Type[10] = { RankNum,RankNum,RankTime,RankCourse,RankCourse,0,RankOk,RankError,RankScore,RankScore };	//各类查询操作依次对应
	StatsStu(Type[phdr->iItem], x);
	x = 1 - x;		//保证x在01之间循环
	*pResult = 0;
}

static int ReadOrSkip(int now, int max, int need, int needmax, int mode) {//决定是否读取当前记录
	//now---当前位置,max最大位置,need--还需要读取多少个,needmax--最多需要多少
	//mode方式：考试或练习(ExamTest考试,ExamTry刷题)
	//返回值：TRUE---Read,FALSE---Skip
	int r, i;
	if (mode == ExamTest) {//考试模式下读最后need个
		if (now + need >= max)
			return TRUE;
		else
			return FALSE;
	}
	if (mode = ExamRetry)
		return TRUE;	//错题模式下，直接读前面的
	if (now > max - needmax)
		return FALSE;//最后needmax仅供考试不提供练习
	if (now + need >= max - needmax)
		return TRUE;
	r = abs(rand());
	i = (max - needmax - now);//可读取位置数量
	ASSERT(i > 0);
	i = r % i;
	if (i < need)
		return TRUE;
	else
		return FALSE;
	return FALSE;
}

static int RndExchQues(int size, int tim) {//随机交换试题次序
	int i, x, y;
	question qu;
	if (size <= 3)
		return FALSE;	//数量太少无需随机交换。且size=1是在A处会形成死循环
	for (i = 0; i < tim; i++) {
		x = abs(rand()) % size;
		for (y = x; y == x; y = abs(rand()) % size);//.....A
		qu = WinInf.ques[x];
		WinInf.ques[x] = WinInf.ques[y];
		WinInf.ques[y] = qu;
	}
	return TRUE;
}

void Students::OnBnClickedCmdstarttest()
{
	// TODO: 在此添加控件通知处理程序代码
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	char buf[72];
	char cmd[1024];
	int i, j, k, m;
	memset(WinInf.ques, 0, sizeof(WinInf.ques));
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		CmbCourse.GetWindowTextA(buf, sizeof(buf));
		sprintf_s(cmd, sizeof(cmd), "Select `Course`,`CourseName` from `Course` Where `CourseName`='%s';", buf);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		row = mysql_fetch_row(result);
		WinInf.CourseID = atoi(row[0]);
		mysql_free_result(result);
		if (WinInf.ExamMode == ExamRetry)
			sprintf_s(cmd, sizeof(cmd), "SELECT questions.ID,course.Course,questions.Text,"
				"questions.Answer,questions.Answer2,sum(examdetails.Correct),sum(examdetails.Error)"
				"FROM examdetails INNER JOIN exam ON examdetails.ExamID = exam.ID "
				"INNER JOIN questions ON examdetails.QuestID = questions.ID "
				"INNER JOIN course ON questions.course = course.Course "
				"Where exam.OperatorID='%d' and course.Course='%d' "
				"GROUP BY questions.ID ORDER BY sum(examdetails.Error)/(sum(examdetails.Correct)+"
				"sum(examdetails.Error)) DESC,sum(examdetails.Error) DESC;", gs.op.ID, WinInf.CourseID);
		else
			sprintf_s(cmd, sizeof(cmd), "Select `ID`,`Course`,`Text`,`Answer`,`Answer2` "
				" From `Questions` Where `Course`='%d' order by `ID`;", WinInf.CourseID);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//计数
		else
			j = 0;
		for (i = 0, m = 0; ((i < j) && (m < 30)); i++) {	//i当前记录号,j总记录数,m已读取数
			//读入变量ques中
			row = mysql_fetch_row(result);
			if (WinInf.ExamMode == ExamRetry) {	//错题模式下不读取正确题目
				if (atoi(row[6]) == 0)
					break;
			}
			if (ReadOrSkip(i, j, 30 - m, 30, WinInf.ExamMode) == TRUE) {//决定是否读取当前记录
				WinInf.ques[m].ID = atoi(row[0]);
				WinInf.ques[m].course = atoi(row[1]);
				strcpy_s(WinInf.ques[m].Text, sizeof(WinInf.ques[m].Text), row[2]);
				WinInf.ques[m].Answer = atoi(row[3]);
				WinInf.ques[m].Answer2 = atoi(row[4]);
				m++;
			}
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
	};//读入试题
	if (m == 0) {
		MessageBoxA("该课程试题不足", "提示");
		WinInf.WorkMode = ModExamA;
		ShowMode();
		return;
	}
	if (WinInf.ExamMode != ExamRetry)	//练习模式和考试模式随机交换试题次序
		RndExchQues(m, 30);		//错题模式不交换，维持错误率高的题目在前面
	for (i = 0; i < m; i++) {//显示正常试题
		for (k = 0; k < sizeof(WinInf.ques[i].Text) - 1; k++) {
			if (WinInf.ques[i].Text[k] == 0) {//必要时为试题字符串尾部加'='字符
				if (WinInf.ques[i].Text[k - 1] == '=')
					break;
				else {
					WinInf.ques[i].Text[k] = '=';
					WinInf.ques[i].Text[k + 1] = 0;
				}
			}
		}
		Question[i].SetWindowTextA(RealChar(WinInf.ques[i].Text));//显示试题字符串
		Answer[i].EnableWindow(TRUE);
		Answer[i].SetWindowTextA("");
	}
	for (i = m; i < 30; i++) {//显示空白试题
		Question[i].SetWindowTextA("无试题");//显示"无试题"字符串
		Answer[i].SetWindowTextA("");
		Answer[i].EnableWindow(FALSE);
	}
	sprintf_s(cmd, sizeof(cmd), "题量：%d道    满分100分     测验时间20分钟", m);
	TxtNote1.SetWindowTextA(cmd);
	WinInf.ExamStart = GetTickCount();//记录测验开始时间
	SetTimer(1, 1000, NULL);
	WinInf.WorkMode = ModExamB;
	ShowMode();
}


void Students::OnmnuStatsRetryStu()
{
	// TODO: 在此添加命令处理程序代码
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("请完成当前操作后再查询");
		return;
	}
	//cTxtCourse.SetWindowTextA("请选择练习课程");
	WinInf.ExamMode = ExamRetry;
	StatsStu(RankNum, 0);	//默认按时间序号排序
}
