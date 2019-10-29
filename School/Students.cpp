// Students.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "School.h"
#include "Support.h"
#include "Students.h"
#include "NewPwd.h"
#include "afxdialogex.h"

CFont InsFont;		//����ҳ������
extern global gs;	//��¼����Ϣ
studentslocal WinInf;	//������ͨ����Ϣ

// Students �Ի���

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
	//DDX_Control(pDX, IDC_INSTRUCTION, InstructorTxt);
	DDX_Control(pDX, IDC_EDIT1, InsTxt);
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


// Students ��Ϣ�������


void Students::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void Students::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


BOOL Students::OnInitDialog()
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

BOOL Students::Init() {	//��ʼ��
	char buf[128], AIRec[128];	//AIRec�˹������Ƽ��γ���
	srand((unsigned int)GetTickCount());	//��ʼ�����������
	//--------��ʾ���ڱ���--------
	WinInf.OpertorID = gs.op.ID;
	sprintf_s(buf, sizeof(buf), "��ӭ %s-%s ͬѧ", gs.op.GradeName, gs.op.Name);
	SetWindowTextA(buf);
	//--------���ø�����Ϣ--------
	sprintf_s(buf, sizeof(buf), "������%s\r\n�༶��%s\r\nѧ�ţ�%s\r\n%s", gs.op.Name, gs.op.GradeName, gs.op.No, RecCourse(AIRec));//���б���ʹ��\r\n
	cTxtStats.SetWindowTextA(buf);
	//--------����������Ϣ--------
	//��������
	InsFont.CreateFont(26,0,0,0, FW_BOLD, FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("Arial"));
	InsTxt.SetFont(&InsFont);
	InsTxt.SetWindowTextA("�����Ϸ��˵�����ѡ��һ�ʼ����\r\n������ϰ��Ŀ��ѡ�񡰿�ʼ��\r\n����鿴��ʷ��¼��ѡ��ͳ�ơ�\r\n�����޸�������˳���ѡ��ϵͳ��");
	//--------��ʼ������--------
	CmbCourse.SetWindowTextA("");
	CmbCourse.EnableWindow(TRUE);
	WinInf.WorkMode = ModNone;
	CreateCtrl();//�趨�ؼ�λ��
	ShowMode();
	return TRUE;
}

int Students::InitCmbCourse(void) {	//��ʼ���γ�������
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int i, j;
	int Mode;	//��ʼ��ģʽ
	char cmd[512];
	//---------����ϴ�ѡ���ģʽ---------
	static int LastMode = -9999; //��ס�ϴγ�ʼ��ģʽ
	if (WinInf.ExamMode == ExamRetry)
		Mode = ExamRetry;	//������ϰ
	else
		Mode = ExamTry;		//ȫ���γ�
	if (Mode == LastMode)
		return TRUE;
	LastMode = Mode;
	for (; ((sta = CmbCourse.DeleteString(0)) > 0););
	//i=CmbCourse.GetCount();
	//---------���������ݿ�---------
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		if (Mode != ExamRetry)	//��������²�ѯ���пγ�
			mysql_query(&host.mysql, "Select `Course`,`CourseName`  From `Course` order by `Course`");
		else {	//����������ֻ��ѯ�д���Ŀγ�
			sprintf_s(cmd, sizeof(cmd), "SELECT course.Course,course.CourseName,sum(examdetails.Error),"
				"exam.OperatorID FROM examdetails INNER JOIN exam ON examdetails.ExamID = exam.ID "
				"INNER JOIN course ON exam.CourseID = course.Course WHERE exam.OperatorID='%d' and "
				"examdetails.Error>0 GROUP BY course.Course ORDER BY course.Course;", gs.op.ID);
			mysql_query(&host.mysql, cmd);
		}
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		//---------��ʾ�γ���Ϣ---------
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			CmbCourse.InsertString(i, row[1]);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
	};
	return sta;
}

void Students::OnCbnSelchangeCmbcourse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CmdStartTest.EnableWindow(TRUE);
}


void Students::CreateCtrl(void) {	////�趨�ؼ�λ��
	RECT RectItem;//�ؼ�λ��
	RECT RectArea;//�����ͼ����
	RECT RectCtrl;//�ؼ���ͼ����
	int i, x, y;
	//---------���ð�ť---------
	CmdSubmit.GetWindowRect(&RectItem);
	ScreenToClient(&RectItem);		//�������������������
	CmdReview.MoveWindow(&RectItem);//�ѡ����顢���ء���ť�����ڡ��ύ��ťͬһλ�á�
	//---------������Ŀ�ʹ����---------
	cList.GetWindowRect(&RectArea);	//��ͳ�Ʊ��ͬһ�����ô�������4��*30���ؼ�
	ScreenToClient(&RectArea);		//�������������������
	for (i = 0; i < 30; i++) {
		x = i % 3; y = i / 3;	//����ѭ�����������
		//---------��Ŀ---------
		RectCtrl.left = RectArea.left + (RectArea.right - RectArea.left) / 3 * x;
		RectCtrl.right = RectCtrl.left + (long)((RectArea.right - RectArea.left) / 3 * 0.60);
		RectCtrl.top = RectArea.top + (RectArea.bottom - RectArea.top) / 10 * y;
		RectCtrl.bottom = RectCtrl.top + (long)((RectArea.bottom - RectArea.top) / 10 * 0.4);
		if (Question[i].m_hWnd == NULL)
			Question[i].Create("���", ES_RIGHT, RectCtrl, this, 15000 + i);
		//---------�����---------
		RectCtrl.left = RectCtrl.right;
		RectCtrl.right = RectCtrl.left + (long)((RectArea.right - RectArea.left) / 3 * 0.18);	//��Ҫ����4����
		if (Answer[i].m_hWnd == NULL)
			Answer[i].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, RectCtrl, this, 15100 + i);
		else
			Answer[i].SetWindowTextA("");
		//---------����---------
		RectCtrl.left = RectCtrl.right;
		RectCtrl.right = RectCtrl.left + (long)((RectArea.right - RectArea.left) / 3 * 0.20);
		if (Flag[i].m_hWnd == NULL)
			Flag[i].Create("��ȷ", NULL, RectCtrl, this, 15200 + i);
		//---------����ʾ---------
		RectCtrl.top = RectCtrl.bottom;
		RectCtrl.bottom = RectCtrl.top + (long)((RectArea.bottom - RectArea.top) / 10 * 0.4);
		RectCtrl.right = RectCtrl.left + (long)((RectArea.right - RectArea.left) / 3 * 0.08);
		if (Prompt[i].m_hWnd == NULL)
			Prompt[i].Create("?", NULL, RectCtrl, this, 15300 + i);
	}
}

void Students::OnBnClickedCmdsubmit()
{
	//����ύʱ
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i, CorrectCount, ErrorCount;
	int mm, ss;		//ʱ��
	char buf[72];
	KillTimer(1);	//�رռ�ʱ��
	WinInf.WorkMode = ModExamC;
	ShowMode();		//ˢ�´���
	for (i = 0, CorrectCount = 0, ErrorCount = 0; i < 30; i++) {
		if (WinInf.ques[i].ID == 0) {
			//---------�ύ����Ȼ�����ٴ���---------
			Flag[i].ShowWindow(SW_HIDE);
			Prompt[i].ShowWindow(SW_HIDE);
			Answer[i].EnableWindow(FALSE);
			continue;
		}
		//---------���ж�---------
		Answer[i].GetWindowTextA(buf, sizeof(buf));
		//---------δ��---------
		if ((buf[0] == 0) && ((WinInf.ExamMode == ExamTry) || (WinInf.ExamMode == ExamRetry))) {	//ˢ������ģʽ�µ�δ����Ŀ
			Flag[i].SetWindowTextA("δ��");
			Prompt[i].ShowWindow(SW_SHOW);
			WinInf.ques[i].Correct = 0;
			WinInf.ques[i].Error = 0;
		}
		else {	//---------��ȷ---------
			WinInf.ques[i].UserAnswer = atoi(buf);
			if (WinInf.ques[i].UserAnswer == WinInf.ques[i].Answer) {
				Flag[i].SetWindowTextA("��ȷ");
				Prompt[i].ShowWindow(SW_HIDE);
				WinInf.ques[i].Correct = 1;
				WinInf.ques[i].Error = 0;
				CorrectCount++;
			}
			else {	//---------���������δ��---------
				Flag[i].SetWindowTextA("����");
				Prompt[i].ShowWindow(SW_SHOW);
				WinInf.ques[i].Correct = 0;
				WinInf.ques[i].Error = 1;
				ErrorCount++;
			}
		}
		//---------�ύ����Ȼ�����ٴ���---------
		Flag[i].ShowWindow(SW_SHOW);
		Answer[i].EnableWindow(FALSE);
	}
	WinInf.Error = ErrorCount;		//�������
	WinInf.Correct = CorrectCount;	//��ȷ����
	WinInf.ExamMs = (int)(GetTickCount() - WinInf.ExamStart);	//ʱ��
	if ((CorrectCount + ErrorCount) > 0)
		WinInf.Score = 100.0*CorrectCount / (CorrectCount + ErrorCount);
	else  //��ֹ����0
		WinInf.Score = 0;
	//---------������֣���---------
	ss = WinInf.ExamMs / 1000;
	mm = ss / 60;
	ss = ss % 60;
	sprintf_s(buf, sizeof(buf), "������ʱ%d��%d�롣��ȷ%d��������%d�����÷�%5.2f", mm, ss, CorrectCount, ErrorCount, WinInf.Score);
	MessageBoxA(buf, "���Խ��");
}

int  Students::WriteExam() {	//���Խ��дMySQL���ݿ�
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MySQLHostVariable host;
	int sta;	//״̬��־
	char cmd[200];
	int i;
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		sprintf_s(cmd, sizeof(cmd), "Insert Into `Exam` (`TimesTamp`,`ExamType`,`Examms`,`CourseID`,`OperatorID`,`Correct`,`Error`,`Score`)"
			" Values (now(),'%d','%d','%d','%d','%d','%d','%lf');",
			WinInf.ExamMode, WinInf.ExamMs, WinInf.CourseID, WinInf.OpertorID, WinInf.Correct, WinInf.Error, WinInf.Score);
		mysql_query(&host.mysql, cmd);	//��¼����ſ�
		WinInf.ExamID = (long)host.mysql.insert_id;
		for (i = 0; i < 30; i++) {
			if (WinInf.ques[i].Error + WinInf.ques[i].Correct > 0) {
				sprintf_s(cmd, sizeof(cmd), "Insert Into `ExamDetails`(`ExamID`,`QuestID`,`UserAnswer`,`Correct`,`Error`)"
					"Values ('%d','%d','%d','%d','%d');",
					WinInf.ExamID, WinInf.ques[i].ID, WinInf.ques[i].UserAnswer, WinInf.ques[i].Correct, WinInf.ques[i].Error);
				mysql_query(&host.mysql, cmd);	//��¼��������
			}
		}
		CloseMySQL(&host);	//�ر�MySQL����	
	};
	return TRUE;
}

void Students::OnBnClickedCmdquit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WriteExam();	//���Խ��дMySQL���ݿ�
	WinInf.WorkMode = ModExamA;
	ShowMode();
}

void Students::OnBnClickedCmdtry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			char buf[128];
			if (WinInf.ques[wID].Answer != -1)		//��ֹ��Ŀ��ɾ��
				sprintf_s(buf, sizeof(buf), "%s%d", WinInf.ques[wID].Text, WinInf.ques[wID].Answer);
			else sprintf_s(buf, sizeof(buf), "��ɾ������Ŀ��\n������������ϵ���Ľ�ʦ�����Ա��");
			MessageBoxA(buf, "��ʾ", MB_TOPMOST);
		}
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void Students::OnMnuExamTestStu()
{
	// TODO: �ڴ���������������
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("������ɵ�ǰ����");
		return;
	}
	cTxtCourse.SetWindowTextA("��ѡ����Կγ�");
	WinInf.WorkMode = ModExamA;
	WinInf.ExamMode = ExamTest;
	ShowMode();
}

void Students::OnmnuExamTryStu()
{
	// TODO: �ڴ���������������
	//ModNone-��,ModStats-ͳ��,ModExamA-����ǰ,ModExamB-������,ModExamC-�����,ModReview-�ع�
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("������ɵ�ǰ����");
		return;
	}
	cTxtCourse.SetWindowTextA("��ѡ��������ϰ�γ�");
	WinInf.WorkMode = ModExamA;
	WinInf.ExamMode = ExamTry;
	ShowMode();
}

void Students::OnmnuExamRetry() {
	// TODO: �ڴ���������������
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("������ɵ�ǰ����");
		return;
	}
	cTxtCourse.SetWindowTextA("��ѡ�������ϰ�γ�");
	WinInf.WorkMode = ModExamA;
	WinInf.ExamMode = ExamRetry;
	ShowMode();
}

void Students::OnmnuStatsTryStu()
{
	// TODO: �ڴ���������������
	//ModNone-��,ModStats-ͳ��,ModExamA-����ǰ,ModExamB-������,ModExamC-�����,ModReview-�ع�
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("����ɵ�ǰ�������ٲ�ѯ");
		return;
	}
	//cTxtCourse.SetWindowTextA("��ѡ����ϰ�γ�");
	WinInf.ExamMode = ExamTry;
	StatsStu(RankNum, 0);	//Ĭ�ϰ�ʱ���������
}

void Students::OnmnuStatsTestStu()
{
	// TODO: �ڴ���������������
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("����ɵ�ǰ�������ٲ�ѯ");
		return;
	}
	//cTxtCourse.SetWindowTextA("��ѡ�����γ�");
	WinInf.ExamMode = ExamTest;
	StatsStu(RankNum, 0);	//Ĭ�ϰ�ʱ���������
}

void Students::OnMnuExitStu()
{
	// TODO: �ڴ���������������
	CDialogEx::OnOK();
}


int  Students::StatsStu(int CmdNum, int ClickTime) {	//ͳ�Ʋ���ɼ�
	//RankNum 0		//����ʽ-ʱ��   RankTime 1	//����ʽ-��ʱ
	//RankCourse 2	//����ʽ-�γ�   RankOk 3		//����ʽ-��ȷ��
	//RankError	4	//����ʽ-������ RankScore 5	//����ʽ-��ȷ��
	static char *RankType[6] = { "exam.ID","`Examms`","`CourseID`","`Correct`","`Error`","`Score`" };	//��������6��
	InsTxt.ShowWindow(SW_HIDE);		//������ʾ��Ϣ
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int i, j, n;
	char cmd[512];
	//--------��ձ��--------
	cList.DeleteAllItems();
	for (; cList.DeleteColumn(0) == TRUE;);
	//--------���ƿ��--------
	WinInf.WorkMode = ModStats;
	ShowMode();
	//���Ʊ�����
	cList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //����list���  
	cList.InsertColumn(0, "���", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(1, "ʱ��", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(2, "��ʱ", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(3, "�γ̺�", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(4, "�γ�", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(5, "ѧ��", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(6, "��ȷ", LVCFMT_CENTER, -1, -1);
	cList.InsertColumn(7, "����", LVCFMT_CENTER, -1, -1);
	if (WinInf.ExamMode == ExamTest)
		cList.InsertColumn(8, "�÷�", LVCFMT_CENTER, -1, -1);
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
	//--------��д����--------
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		if (ClickTime == 0) {	//��ʼΪ����
			sprintf_s(cmd, sizeof(cmd), "SELECT exam.ID , `TimesTamp` , `Examms` , `CourseID` , `CourseName` ,"
				" `OperatorID`,`Correct`,`Error`,`Score` FROM `exam` INNER JOIN `course` ON exam.CourseID = course.Course "
				" Where OperatorID='%d' And `ExamType`='%d' ORDER BY %s DESC;", gs.op.ID, WinInf.ExamMode, RankType[CmdNum]);
		}
		else {	//�ٵ�һ��Ϊ����
			sprintf_s(cmd, sizeof(cmd), "SELECT exam.ID , `TimesTamp` , `Examms` , `CourseID` , `CourseName` ,"
				" `OperatorID`,`Correct`,`Error`,`Score` FROM `exam` INNER JOIN `course` ON exam.CourseID = course.Course "
				" Where OperatorID='%d' And `ExamType`='%d' ORDER BY %s ASC;", gs.op.ID, WinInf.ExamMode, RankType[CmdNum]);
		}
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			n = (long)result->row_count;//����
		else
			n = 0;
		for (i = 0; i < n; i++) {
			row = mysql_fetch_row(result);
			char Time[32];	//װ�֣����ʽ��ʱ��
			cList.InsertItem(i, row[0]);
			cList.SetItemText(i, 1, row[1]);
			//����ʱ��ĸ�ʽ��Ҫת��
			for (j = 3; j <= 7; j++) {
				cList.SetItemText(i, j, row[j]);
			}
			cList.SetItemText(i, 2, MsToTime(row[2], Time));	//���ĵ�ʱ��
			if (WinInf.ExamMode == ExamTest)		//ֻ�п��Բ��з���
				cList.SetItemText(i, 8, row[8]);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
	};
	return TRUE;
}

int	Students::ShowMode() {		//�л���ʾģʽ
	//ModNone-��,ModStats-ͳ��,ModExamA-����ǰ,ModExamB-������,ModExamC-�����,ModReview-�ع�
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
			CmdReview.SetWindowTextA("���Ծ�");
		else
			CmdReview.SetWindowTextA("�� ��");
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	char buf[256];
	DWORD ExamNow = GetTickCount();
	int ExamSec = (int)((ExamNow - WinInf.ExamStart) / 1000);
	WinInf.RemainingSec = 1200 - ExamSec;
	int mme = ExamSec / 60;
	int sse = ExamSec % 60;
	int mmr = WinInf.RemainingSec / 60;
	int ssr = WinInf.RemainingSec % 60;
	sprintf_s(buf, sizeof(buf), "����ʱ��:%02d��%02d��", mme, sse);
	TxtNote2.SetWindowTextA(buf);
	sprintf_s(buf, sizeof(buf), "ʣ��ʱ��:%02d��%02d��", mmr, ssr);
	TxtNote.SetWindowTextA(buf);
	if (WinInf.RemainingSec <= 0)
		OnBnClickedCmdsubmit();//ʱ�䵽ǿ���ύ
	CDialogEx::OnTimer(nIDEvent);
}

void Students::OnBnClickedCmdreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ModNone-��,ModStats-ͳ��,ModExamA-����ǰ,ModExamB-������,ModExamC-�����,ModReview-�ع�
	if (WinInf.WorkMode == ModReview) {	//��ť˫�ض��壬�ع�ģʽ���ٴε��Ϊ����ͳ��ģʽ
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
		MessageBoxA("����ѡ�����", "��ʾ");
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
		MessageBoxA("����ѡ�����", "��ʾ");
		return;
	}

	WinInf.WorkMode = ModReview;
	ShowMode();

	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char cmd[128];
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		sprintf_s(cmd, sizeof(cmd), "SELECT `ExamID`,`QuestID`,`UserAnswer` FROM `ExamDetails` "
			" Where `ExamID`='%d' ORDER BY QuestID ASC;", ExamID);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		if ((j < 1) || (j > 30)) {	//Ӧ�ö���1-30����Ŀ
			mysql_free_result(result);
			CloseMySQL(&host);	//�ر�MySQL����	
			MessageBoxA("����ѡ�����", "��ʾ");
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
			if (row == NULL) {	//��Ŀ��ɾ����
				strcpy_s(WinInf.ques[i].Text, sizeof(WinInf.ques[i].Text), "��ɾ������Ŀ");
				WinInf.ques[i].Answer = -1;
			}
			else {	//����������ʾ
				strcpy_s(WinInf.ques[i].Text, sizeof(WinInf.ques[i].Text), row[1]);
				WinInf.ques[i].Answer = atoi(row[2]);
			}
			for (int k = 0; k < sizeof(WinInf.ques[i].Text) - 1; k++) {
				if (WinInf.ques[i].Text[k] == 0) {//��ҪʱΪ�����ַ���β����'='�ַ�
					if (WinInf.ques[i].Text[k - 1] == '=')
						break;
					else {
						WinInf.ques[i].Text[k] = '=';
						WinInf.ques[i].Text[k + 1] = 0;
					}
				}
			}
			Question[i].SetWindowTextA(RealChar(WinInf.ques[i].Text));//��ʾ�����ַ���
			if (WinInf.ques[i].Answer == WinInf.ques[i].UserAnswer) {
				Prompt[i].ShowWindow(SW_HIDE);
			}
			else {
				Flag[i].SetWindowTextA("����");
				Flag[i].ShowWindow(SW_SHOW);
				Prompt[i].ShowWindow(SW_SHOW);
			}
			mysql_free_result(result);
		}
		CloseMySQL(&host);	//�ر�MySQL����	
	};
	return;
}

void Students::OnmnuNewPwd()
{
	// TODO: �ڴ���������������
	NewPwd f;
	f.DoModal();
}


void Students::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnCancel();
}

void Students::OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static int ClickTime = 0;			//��ִֹ�к�ClickTimeû��
	static int Type[10] = { RankNum,RankNum,RankTime,RankCourse,RankCourse,0,RankOk,RankError,RankScore,RankScore };	//�����ѯ�������ζ�Ӧ
	StatsStu(Type[phdr->iItem], ClickTime);
	ClickTime = 1 - ClickTime;		//��֤ClickTime��01֮��ѭ��
	*pResult = 0;
}

static int ReadOrSkip(int now, int max, int need, int needmax, int mode) {//�����Ƿ��ȡ��ǰ��¼
	//now---��ǰλ��,max���λ��,need--����Ҫ��ȡ���ٸ�,needmax--�����Ҫ����
	//mode��ʽ�����Ի���ϰ(ExamTest����,ExamTryˢ��)
	//����ֵ��TRUE---Read,FALSE---Skip
	int r, i;
	if (mode == ExamTest) {//����ģʽ�¶����need��
		if (now + need >= max)
			return TRUE;
		else
			return FALSE;
	}
	if (mode = ExamRetry)
		return TRUE;	//����ģʽ�£�ֱ�Ӷ�ǰ���
	if (now > max - needmax)
		return FALSE;//���needmax�������Բ��ṩ��ϰ
	if (now + need >= max - needmax)
		return TRUE;
	r = abs(rand());
	i = (max - needmax - now);//�ɶ�ȡλ������
	ASSERT(i > 0);
	i = r % i;
	if (i < need)
		return TRUE;
	else
		return FALSE;
	return FALSE;
}

static int RndExchQues(int size, int tim) {//��������������
	int i, x, y;
	question qu;
	if (size <= 3)
		return FALSE;	//����̫�����������������size=1����A�����γ���ѭ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	char buf[72];
	char cmd[1024];
	int i, j, k, m;
	memset(WinInf.ques, 0, sizeof(WinInf.ques));
	InsTxt.ShowWindow(SW_HIDE);	//������ʾ��Ϣ
	sta = InitMySQL(&host);		//����MySQL���ݿ�
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
			j = (long)result->row_count;//����
		else
			j = 0;
		for (i = 0, m = 0; ((i < j) && (m < 30)); i++) {	//i��ǰ��¼��,j�ܼ�¼��,m�Ѷ�ȡ��
			//�������ques��
			row = mysql_fetch_row(result);
			if (WinInf.ExamMode == ExamRetry) {	//����ģʽ�²���ȡ��ȷ��Ŀ
				if (atoi(row[6]) == 0)
					break;
			}
			if (ReadOrSkip(i, j, 30 - m, 30, WinInf.ExamMode) == TRUE) {//�����Ƿ��ȡ��ǰ��¼
				WinInf.ques[m].ID = atoi(row[0]);
				WinInf.ques[m].CourseID = atoi(row[1]);
				strcpy_s(WinInf.ques[m].Text, sizeof(WinInf.ques[m].Text), row[2]);
				WinInf.ques[m].Answer = atoi(row[3]);
				WinInf.ques[m].AnswerLeft = atoi(row[4]);
				m++;
			}
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
	};//��������
	if (m == 0) {
		MessageBoxA("�ÿγ����ⲻ��", "��ʾ");
		WinInf.WorkMode = ModExamA;
		ShowMode();
		return;
	}
	if (WinInf.ExamMode != ExamRetry)	//��ϰģʽ�Ϳ���ģʽ��������������
		RndExchQues(m, 30);		//����ģʽ��������ά�ִ����ʸߵ���Ŀ��ǰ��
	for (i = 0; i < m; i++) {//��ʾ��������
		for (k = 0; k < sizeof(WinInf.ques[i].Text) - 1; k++) {
			if (WinInf.ques[i].Text[k] == 0) {//��ҪʱΪ�����ַ���β����'='�ַ�
				if (WinInf.ques[i].Text[k - 1] == '=')
					break;
				else {
					WinInf.ques[i].Text[k] = '=';
					WinInf.ques[i].Text[k + 1] = 0;
				}
			}
		}
		Question[i].SetWindowTextA(RealChar(WinInf.ques[i].Text));//��ʾ�����ַ���
		Answer[i].EnableWindow(TRUE);
		Answer[i].SetWindowTextA("");
	}
	for (i = m; i < 30; i++) {//��ʾ�հ�����
		Question[i].SetWindowTextA("������");//��ʾ"������"�ַ���
		Answer[i].SetWindowTextA("");
		Answer[i].EnableWindow(FALSE);
	}
	sprintf_s(cmd, sizeof(cmd), "������%d��    ����100��     ����ʱ��20����", m);
	TxtNote1.SetWindowTextA(cmd);
	WinInf.ExamStart = GetTickCount();//��¼���鿪ʼʱ��
	SetTimer(1, 1000, NULL);
	WinInf.WorkMode = ModExamB;
	ShowMode();
}


void Students::OnmnuStatsRetryStu()
{
	// TODO: �ڴ���������������
	if ((WinInf.WorkMode == ModExamB) || (WinInf.WorkMode == ModExamC) || (WinInf.WorkMode == ModReview)) {
		MessageBoxA("����ɵ�ǰ�������ٲ�ѯ");
		return;
	}
	//cTxtCourse.SetWindowTextA("��ѡ����ϰ�γ�");
	WinInf.ExamMode = ExamRetry;
	StatsStu(RankNum, 0);	//Ĭ�ϰ�ʱ���������
}

char * RecCourse(char * resource) {		//�Ƽ���ѧϰ�γ�.
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int i, j, n;
	char cmd[512];
	//--------����MySQL���ݿ�--------
	sta = InitMySQL(&host);
	if (sta == TRUE) {
		sprintf_s(cmd, sizeof(cmd), "SELECT exam.ID , `TimesTamp` , `Examms` , `CourseID` , `CourseName` ,"
			" `OperatorID`,`Correct`,`Error`,`Score` FROM `exam` INNER JOIN `course` ON exam.CourseID = course.Course "
			" Where OperatorID='%d' And `ExamType`='%d' ORDER BY `CourseID` DESC;", gs.op.ID, ExamTry);
		//��������ȷ������
	}
	else {
		sprintf_s(resource, 128, "���ݿ�����ʧ�ܡ�");
		return resource;
	}
	mysql_query(&host.mysql, cmd);
	result = mysql_store_result(&host.mysql);
	//--------ͳ��ƽ���ɼ�--------
	if (result != NULL)
		n = (long)result->row_count;//����
	else n = 0;
	if (n == 0) {	//һ����¼��û�У��޷�ͳ��
		sprintf_s(resource, 128, "����û����ϰ��¼");
		return resource;
	}
	row = mysql_fetch_row(result);
	int CourseNum = atoi(row[3]);	//�γ�����
	int * Correct = (int *) malloc(sizeof(int) * (CourseNum + 1));	//��Ÿ���ȷ��
	int * Error = (int *)malloc(sizeof(int) * (CourseNum + 1));	//��Ÿ���ȷ��
	memset(Correct, 0, sizeof(int) * (CourseNum + 1));	//��0
	memset(Error, 0, sizeof(int) * (CourseNum + 1));
	for (i = 0; i < n; i++) {
		int CourseID = atoi(row[3]);	//��ÿγ̺�
		Correct[CourseID] += atoi(row[6]);	//ͳ����ȷ������
		Error[CourseID] += atoi(row[7]);
		row = mysql_fetch_row(result);	//�ڽ���ѭ��֮ǰ��ù�һ����Ϣ
	}
	int AvgMin = 0x7fffffff, MinID = 0;
	for (i = 1; i <= CourseNum; i++) {
		if (Correct[i] + Error[i] == 0)
			continue;	//��ֹ����0��������������ϰ
		int AvgLocal = Correct[i] * 100 / (Correct[i] + Error[i]);
		if (AvgLocal < AvgMin) {
			AvgMin = AvgLocal;
			MinID = i;
		}
	}
	//--------��ÿγ�����--------
	sprintf_s(cmd, sizeof(cmd), "SELECT exam.ID , `TimesTamp` , `Examms` , `CourseID` , `CourseName` ,"
		" `OperatorID`,`Correct`,`Error`,`Score` FROM `exam` INNER JOIN `course` ON exam.CourseID = course.Course "
		" Where CourseID='%d';", MinID, ExamTry);	//MinID��ƽ������͵Ŀγ̱��
	mysql_query(&host.mysql, cmd);
	result = mysql_store_result(&host.mysql);
	row = mysql_fetch_row(result);
	sprintf_s(resource, 128, "�����˹����ܼ��㣬������Ҫ��ϰ�Ŀγ�Ϊ��%s", row[4]);
	//--------�ͷſռ�--------
	free(Correct);
	free(Error);
	mysql_free_result(result);
	CloseMySQL(&host);	//�ر�MySQL����
	return resource;	//����ԭ��ַ
}