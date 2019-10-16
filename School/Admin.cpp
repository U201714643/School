// admin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "include\mysql.h"
#include "School.h"
#include "Support.h"
#include "Combo.h"
#include "admin.h"
#include "NewPwd.h"
#include "TestGenor.h"
#include "Approval.h"


#define WMNone 0	//����ģʽ-��
#define WMOper 1	//����ģʽ-ѧԱ����
#define WMQust 2	//����ģʽ-������
#define WMManage 3	//����ģʽ-��ѧȨ����

typedef struct adminlocal {	//admin���干�ñ���
	HTREEITEM hTreeItemSchool;	//"ѧУ"
	HTREEITEM hTreeItemGroup;	//"������"
	HTREEITEM hTreeItemCourse;	//"�γ�"
	HTREEITEM hTreeItemManage;	//"��ʦ�ְ�"
	int	WorkMode;//0-��,1-operator,2-questions
	char ItemText[72];//��������
	int	 Course, GradeID;
}adminlocal;


extern global gs;
adminlocal AWinInf;


// admin �Ի���

IMPLEMENT_DYNAMIC(Admin, CDialogEx)

Admin::Admin(CWnd* pParent /*=NULL*/)
	: CDialogEx(Admin::IDD, pParent)
{
}

Admin::~Admin()
{
}

void Admin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, cTree);
	DDX_Control(pDX, IDC_TxtNote, TxtNote);
	DDX_Control(pDX, IDC_CmdClrPwd, CmdClrPwd);
	DDX_Control(pDX, IDC_LIST, ListCtrl);
	DDX_Control(pDX, IDC_CmdApprovalTask, CmdApprovalTask);
	DDX_Control(pDX, IDC_CmdDel, CmdDel);
	DDX_Control(pDX, IDC_CmdBatch, CmdBatch);
	DDX_Control(pDX, IDC_CmdSave, CmdSave);
	DDX_Control(pDX, IDC_TESTGEN, TestGenorArg);
}


BEGIN_MESSAGE_MAP(Admin, CDialogEx)
	ON_BN_CLICKED(IDOK, &Admin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Admin::OnBnClickedCancel)
	ON_COMMAND(ID_mnuExit, &Admin::OnExit)
	ON_COMMAND(ID_32774, &Admin::OnBatchQuestions)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &Admin::OnTvnSelchangedTree)
	ON_BN_CLICKED(IDC_CmdOK, &Admin::OnBnClickedCmdok)
	ON_BN_CLICKED(IDC_CmdBatch, &Admin::OnBnClickedCmdbatch)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TREE, &Admin::OnNMKillfocusTree)
	ON_BN_CLICKED(IDC_CmdClrPwd, &Admin::OnBnClickedCmdclrpwd)
	ON_BN_CLICKED(IDC_CmdDel, &Admin::OnBnClickedCmddel)
	ON_COMMAND(ID_mnuNewPwd, &Admin::OnmnuNewPwd)
	ON_COMMAND(ID_ApprovalUser, &Admin::OnApprovalUser)
	ON_COMMAND(ID_mnuAutoApproval, &Admin::OnmnuAutoApproval)
	ON_WM_TIMER()
//	ON_NOTIFY(NM_CLICK, IDC_LIST, &Admin::OnNMClickList)
	ON_WM_CLOSE()
	ON_COMMAND(ID_mnuTeacher, &Admin::Onmnuteacher)
	ON_BN_CLICKED(IDC_CmdApprovalTask, &Admin::OnBnClickedCmdapprovaltask)
	ON_BN_CLICKED(IDC_CmdSave, &Admin::OnBnClickedCmdsave)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &Admin::OnHdnItemclickList)
	ON_BN_CLICKED(IDC_TESTGEN, &Admin::OnBnClickedTestgen)
END_MESSAGE_MAP()


// admin ��Ϣ�������


void Admin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void Admin::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void Admin::OnExit()
{
	// TODO: �ڴ���������������
	CDialogEx::OnOK();
}

BOOL Admin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	//--------��ʾ����---------
	char buf[72];
	sprintf_s(buf, sizeof(buf), "��ӭ%s����Ա", gs.op.Name);
	SetWindowTextA(buf);
	//---------�������ݿ�---------
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	HTREEITEM hSubItem1;
	int sta;	//״̬��־
	int i, j;
	sta = InitMySQL(&host);//����MySQL���ݿ�
	//---------���ñ���---------
	AWinInf.hTreeItemSchool = cTree.InsertItem("��Ա����", TVI_ROOT);//�ڸ��������Ӹ��ڵ㡰ѧУ��
	if (sta == TRUE) {
		AWinInf.hTreeItemGroup = cTree.InsertItem("������", AWinInf.hTreeItemSchool, NULL);//�ڡ�ѧУ�����С��ӡ������顱
		mysql_query(&host.mysql, "Select `ID`,`Name`  From `Grade` order by `ID`");
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		for (hSubItem1 = AWinInf.hTreeItemGroup, i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			hSubItem1 = cTree.InsertItem(row[1], AWinInf.hTreeItemSchool, hSubItem1);
		}
		mysql_free_result(result);
	};
	//---------���ñ���---------
	AWinInf.hTreeItemCourse = cTree.InsertItem("������", TVI_ROOT);//�ڸ��������Ӹ��ڵ㡰�γ̡�
	if (sta == TRUE) {
		mysql_query(&host.mysql, "Select `Course`,`CourseName`  From `Course` order by `Course`");
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		for (hSubItem1 = NULL, i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			hSubItem1 = cTree.InsertItem(row[1], AWinInf.hTreeItemCourse, hSubItem1);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
	};
	//---------���ñ���---------
	AWinInf.hTreeItemManage = cTree.InsertItem("��ѧ�������", TVI_ROOT);//�ڸ��������ӡ���ѧ������䡱
	//---------���ƴ���---------
	ChMode(WMNone, 0, 0);
	return TRUE;
}

void Admin::OnBatchQuestions()
{
	// TODO: �ڴ���������������
	ComobRet CmbRet;
	Combo SelCourse;
	CString Fn;
	CmbRet.type = 0;//0==>Course
	SelCourse.SetRet(&CmbRet);
	SelCourse.DoModal();	//ѡ��γ�
	//---------ѡ���ļ�---------
	if (CmbRet.sta != TRUE)	return;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "�ı��ĵ�(*.txt)|*.txt|");//ѡ����Ŀ�ı�
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return;
	//---------�������ݿ�---------
	char cmd[128];
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int i, j;
	sta = InitMySQL(&host);//����MySQL���ݿ�
	//---------��ÿγ�����---------
	if (sta == TRUE) {
		sprintf_s(cmd, sizeof(cmd), "Select `Course`,`CourseName`  From `Course` Where `CourseName`='%s';", CmbRet.Sel);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		if (j == 1) {
			row = mysql_fetch_row(result);
			strcpy_s(CmbRet.Sel, sizeof(CmbRet.Sel), row[0]);
		}
		else
			sta = FALSE;
		mysql_free_result(result);
		if (sta == FALSE) {
			CloseMySQL(&host);	//�ر�MySQL����	
			return;
		}
	};
	//---------��ȡ�ļ�����Ŀ---------
	FILE *fp = NULL;
	char buf[1024];
	int eq;//�Ⱥ�λ��
	fopen_s(&fp, Fn, "rt");
	for (; fgets(buf, sizeof(buf), fp) != NULL;) {
		for (i = 0, eq = 0; buf[i] != 0; i++) {
			if (buf[i] == '=') {
				eq = i;
				buf[i] = 0;
			}
			if (buf[i] == 0x0A)
				buf[i] = 0;
		}
		//---------���ݿ����---------
		if ((eq > 0) && (eq < 70) && (i - eq > 1) && (i - eq < 10)) {
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Questions` (`Course`,`Text`,`Answer`) "
				" Values('%s','%s','%s');", CmbRet.Sel, buf, buf + eq + 1);
			mysql_query(&host.mysql, cmd);
		}
	}
	//---------��β����---------
	fclose(fp);
	CloseMySQL(&host);	//�ر�MySQL����	
}


void Admin::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem, ItemP;
	CString ItemText;
	hItem = cTree.GetSelectedItem();
	//---------ѡ���ʦ�ֹ�---------
	if (hItem == AWinInf.hTreeItemManage) {
		TeacherManage();	//�û����ѡ���ʦ�ֹ�
	}
	//---------ѡ��������---------
	else {
		ItemP = cTree.GetParentItem(hItem);
		if (ItemP == AWinInf.hTreeItemCourse) {		//�û����ѡ��ĳ�γ�
			ItemText = cTree.GetItemText(hItem);
			strcpy_s(AWinInf.ItemText, sizeof(AWinInf.ItemText), ItemText);
			ChMode(WMQust, 0, 0);
		}
		else if (ItemP == AWinInf.hTreeItemSchool) {//�û����ѡ��ĳ�༶
			ItemText = cTree.GetItemText(hItem);
			strcpy_s(AWinInf.ItemText, sizeof(AWinInf.ItemText), ItemText);
			ChMode(WMOper, 0, 0);
		}
	}
	*pResult = 0;
}

int Admin::TeacherManage() {//��ʦ�ֹ�����ģʽ
	ChMode(WMManage, 0, 0);
	return TRUE;
}

int Admin::ChMode(int Mode,int CmdNum,int CilckTime) {//�ı乤��ģʽ
	//#define WMNone 0		//����ģʽ-��
	//#define WMOper 1		//����ģʽ-ѧԱ����
	//#define WMQust 2		//����ģʽ-������
	//#define WMManage 3	//����ģʽ-��ѧȨ����
	//--------�����ڽ�ѧȨ����--------
	static char *RankType[4] = { "`OperatorID`","`OperatorID`","grade.`ID`","manage.`Right`"};	//Ĭ�ϣ��ڲ���ţ����������ڲ���ţ���ִ�̰༶��Ȩ��
	//--------�������������ദ��--------
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int i, j, n;
	char buf[EXPLEN];
	char cmd[512];
	AWinInf.WorkMode = Mode;
	ListCtrl.DeleteAllItems();
	for (; ListCtrl.DeleteColumn(0) == TRUE;);
	//---------��ģʽ---------
	if (AWinInf.WorkMode == WMNone) {
		//�������ء����ð�ť����
		TxtNote.SetWindowTextA("��ѡ��");
		TestGenorArg.ShowWindow(SW_HIDE);
		CmdClrPwd.ShowWindow(SW_HIDE);
		CmdApprovalTask.ShowWindow(SW_HIDE);
		CmdBatch.EnableWindow(FALSE);
		CmdSave.EnableWindow(FALSE);
		cTree.EnableWindow(TRUE);
	}
	//---------��Ա����ģʽ---------
	else if (AWinInf.WorkMode == WMOper) {
		//---------�ؼ�����---------
		sprintf_s(buf, sizeof(buf), "%s ��Ա����", AWinInf.ItemText);
		//---------���ء���ʾ��ť---------
		TxtNote.SetWindowTextA(buf);
		TestGenorArg.ShowWindow(SW_HIDE);
		CmdClrPwd.ShowWindow(SW_SHOW);
		CmdApprovalTask.ShowWindow(SW_HIDE);
		CmdBatch.EnableWindow(TRUE);
		CmdSave.EnableWindow(TRUE);
		CmdDel.SetWindowTextA("ɾ����Ա");
		AWinInf.GradeID = 0;
		//----------����MySQL���ݿ�----------
		sta = InitMySQL(&host);
		if (sta == TRUE) {
			sprintf_s(cmd, sizeof(cmd), "Select `ID`,`Name`  From `Grade` Where `Name`='%s';", AWinInf.ItemText);
			mysql_query(&host.mysql, cmd);
			result = mysql_store_result(&host.mysql);
			if (result != NULL)
				j = (long)result->row_count;//����
			else
				j = 0;
			if (j == 1) {
				row = mysql_fetch_row(result);
				AWinInf.GradeID = atoi(row[0]);
			}
			mysql_free_result(result);
		};
		//----------���Ʊ�����----------
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //����list���  
		ListCtrl.InsertColumn(0, "��  ��", LVCFMT_CENTER, -1, -1);
		if (AWinInf.GradeID != 0)	//ѧ����ʾѧ�ţ���ʦ��ʾ����
			ListCtrl.InsertColumn(1, "ѧ  ��", LVCFMT_CENTER, -1, -1);
		else ListCtrl.InsertColumn(1, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(3, "Ȩ  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.SetColumnWidth(0, 0);
		ListCtrl.SetColumnWidth(1, 80);
		ListCtrl.SetColumnWidth(2, 80);
		ListCtrl.SetColumnWidth(3, 80);
		//----------��ñ������----------
		if (CilckTime == 0) {	//��ʼΪ������ֻ��ѧ������
			sprintf_s(cmd, sizeof(cmd), "Select `ID`,`No`,`user`,`right`  From `operator` "
				" Where `Grade`='%d' order by `right` DESC,`No` ASC;", AWinInf.GradeID);
		}
		else {	//�ٵ�һ��Ϊ����
			sprintf_s(cmd, sizeof(cmd), "Select `ID`,`No`,`user`,`right`  From `operator` "
				" Where `Grade`='%d' order by `right` ASC,`No` ASC;", AWinInf.GradeID);
		}
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			n = (long)result->row_count;//����
		else
			n = 0;
		//---------����---------
		for (i = 0; i < n; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);		//�ڲ����
			ListCtrl.SetItemText(i, 1, row[1]);	//ѧ��
			ListCtrl.SetItemText(i, 2, row[2]);	//����
			//----------Ȩ��----------
			if (row[3][0] == '0') {
				ListCtrl.SetItemText(i, 3, "�����ѧ��");
			}
			else if (row[3][0] == '1') {
				ListCtrl.SetItemText(i, 3, "ѧ��");
			}
			else if (row[3][0] == '2') {
				ListCtrl.SetItemText(i, 3, "����˽�ʦ");
			}
			else if (row[3][0] == '3') {
				ListCtrl.SetItemText(i, 3, "��ʦ");
			}
			else if (row[3][0] == '9') {
				ListCtrl.SetItemText(i, 3, "����Ա");
			}
			else {
				ListCtrl.SetItemText(i, 3, "����");
			}
		}
		//---------�ͷſռ䡢�ر�����---------
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
	}
	else if (AWinInf.WorkMode == WMQust) {
		//---------��ť����������---------
		sprintf_s(buf, sizeof(buf), "%s�γ� ������", AWinInf.ItemText);
		TxtNote.SetWindowTextA(buf);
		TestGenorArg.ShowWindow(SW_SHOW);
		CmdClrPwd.ShowWindow(SW_HIDE);
		CmdApprovalTask.ShowWindow(SW_HIDE);
		CmdBatch.EnableWindow(TRUE);
		CmdSave.EnableWindow(TRUE);
		CmdDel.SetWindowTextA("ɾ����Ŀ");
		AWinInf.Course = 0;		//�����ϴεĿγ���Ϣ
		sta = InitMySQL(&host);	//����MySQL���ݿ�
		//---------�������ݿ�---------
		if (sta != TRUE)	return FALSE;
		sprintf_s(cmd, sizeof(cmd), "Select `Course`,`CourseName`  From `Course` Where `CourseName`='%s';", AWinInf.ItemText);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		if (j == 1) {	//�γ̺ŷ������
			row = mysql_fetch_row(result);
			AWinInf.Course = atoi(row[0]);
		}
		mysql_free_result(result);
		if (AWinInf.Course == 0) {
			CloseMySQL(&host);	//�ر�MySQL����
			return FALSE;
		}
		//---------���Ʊ�����---------
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //����list���  
		ListCtrl.InsertColumn(0, "  ID  ", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(1, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "��  Ŀ", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(3, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.SetColumnWidth(0, 0);
		ListCtrl.SetColumnWidth(1, 0);
		ListCtrl.SetColumnWidth(2, 100);
		ListCtrl.SetColumnWidth(3, 60);
		//---------��ñ������---------
		sprintf_s(cmd, sizeof(cmd), "Select `ID`,`Text`,`Answer`  From `questions` "
			" Where `course`='%d' order by `ID` ;", AWinInf.Course);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		//---------����������---------
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);
			sprintf_s(buf, sizeof(buf), "%d", i + 1);	//row�����ݲ����޸�
			ListCtrl.SetItemText(i, 1, buf);
			sprintf_s(buf, sizeof(buf), "%s", row[1]);	//row�����ݲ����޸�
			ListCtrl.SetItemText(i, 2, RealChar(buf));
			ListCtrl.SetItemText(i, 3, row[2]);
		}
		//---------�ͷſռ䡢�ر�����---------
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����
	}
	else if (AWinInf.WorkMode == WMManage) {
		//---------��ʾ�����ذ�ť---------
		sprintf_s(buf, sizeof(buf), "��ѧ�������", AWinInf.ItemText);
		TxtNote.SetWindowTextA(buf);
		TestGenorArg.ShowWindow(SW_HIDE);
		CmdClrPwd.ShowWindow(SW_HIDE);
		CmdApprovalTask.ShowWindow(SW_SHOW);
		CmdBatch.EnableWindow(FALSE);
		CmdSave.EnableWindow(TRUE);
		CmdDel.SetWindowTextA("ȡ������");
		AWinInf.Course = 0;
		AWinInf.GradeID = 0;	//ֻ����ʦ����ִ��Ȩ��һ˵
		//---------�������ݿ�---------
		sta = InitMySQL(&host);	//����MySQL���ݿ�
		if (sta != TRUE)	return FALSE;
		//---------���Ʊ�����---------
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //����list���  
		ListCtrl.InsertColumn(0, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(1, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(3, "״  ̬", LVCFMT_CENTER, -1, -1);
		ListCtrl.SetColumnWidth(0, 0);
		ListCtrl.SetColumnWidth(1, 80);
		ListCtrl.SetColumnWidth(2, 120);
		ListCtrl.SetColumnWidth(3, 80);
		//---------��ñ������---------
		if (CilckTime == 0) {	//��ʼΪ����
			sprintf_s(cmd, sizeof(cmd), "SELECT manage.ID,`user`,`Name`,manage.`Right` FROM manage "
				" INNER JOIN operator ON manage.OperatorID = operator.ID "
				" INNER JOIN grade ON manage.GradeID = grade.ID"
				" Order By %s ASC;", RankType[CmdNum]);
		}
		else {	//�ٵ�һ��Ϊ����
			sprintf_s(cmd, sizeof(cmd), "SELECT manage.ID,`user`,`Name`,manage.`Right` FROM manage "
				" INNER JOIN operator ON manage.OperatorID = operator.ID "
				" INNER JOIN grade ON manage.GradeID = grade.ID"
				" Order By %s DESC;", RankType[CmdNum]);
		}
		//---------��д���---------
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);
			ListCtrl.SetItemText(i, 1, row[1]);
			ListCtrl.SetItemText(i, 2, row[2]);
			if (row[3][0] == '0')
				ListCtrl.SetItemText(i, 3, "������");
			else
				ListCtrl.SetItemText(i, 3, "ִ����");
		}
		//---------��ñ������---------
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����
	}
	return TRUE;
}

void Admin::OnBnClickedCmdok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}

int BatchAddQuestions(void) {
	MySQLHostVariable host;
	FILE *fp = NULL;
	char cmd[256];
	char buf[1024];
	int sta;	//״̬��־
	int i;
	int eq;//�Ⱥ�(�򶺺Ż�Tab)λ��
	long pwd;
	//---------����ļ�·��---------
	CString Fn;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "�ı��ĵ�(*.txt)|*.txt|");//ѡ����Ŀ�ı�
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return FALSE;
	//---------���������ݿ�---------
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta != TRUE)
		return FALSE;
	//---------���ļ�---------
	fopen_s(&fp, Fn, "rt");
	for (; fgets(buf, sizeof(buf), fp) != NULL;) {
		for (i = 0, eq = 0; buf[i] != 0; i++) {
			//---------����,��=���հ׷�---------
			if ((buf[i] == '\t') || (buf[i] == ',') || (buf[i] == '=')) {
				eq = i;
				buf[i] = 0;
			}
			//---------��������---------
			if (buf[i] == 0x0A)
				buf[i] = 0;
		}
		//---------�ϴ������ݿ�---------
		if ((eq > 0) && (eq < 70) && (i - eq > 1) && (i - eq < 30)) {
			pwd = PwdCode(buf + eq + 1, buf);		//���������һ�α���
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Questions` (`Course`,`Text`,`Answer`) "
				" Values('%d','%s','%s');", AWinInf.Course, buf, buf + eq + 1, pwd);
			mysql_query(&host.mysql, cmd);
		}
	}
	//---------��ñ������---------
	fclose(fp);
	CloseMySQL(&host);	//�ر�MySQL����	
	return TRUE;
}



int BatchAddUser(void) {
	MySQLHostVariable host;
	FILE *fp = NULL;
	char cmd[256];
	char buf[1024];
	int sta;	//״̬��־
	int i;
	int eq;//�Ⱥ�(�򶺺Ż�Tab)λ��
	long pwd;
	//---------����ļ�·��---------
	CString Fn;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "ѧ�������ı��ĵ�(*����.txt)|*����.txt|");//ѡ��ѧ�������ı�
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return FALSE;
	//---------���������ݿ�---------
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta != TRUE)
		return FALSE;
	//---------���ļ�---------
	fopen_s(&fp, Fn, "rt");
	for (; fgets(buf, sizeof(buf), fp) != NULL;) {
		for (i = 0, eq = 0; buf[i] != 0; i++) {
			//---------����,��=���հ׷�---------
			if ((buf[i] == '\t') || (buf[i] == ',') || (buf[i] == '=')) {
				eq = i;
				buf[i] = 0;
			}
			//---------��������---------
			if (buf[i] == 0x0A)
				buf[i] = 0;
		}
		//---------�ϴ������ݿ�---------
		if ((eq > 0) && (eq < 70) && (i - eq > 1) && (i - eq < 30)) {
			pwd = PwdCode(buf + eq + 1, buf);	//���������һ�α���
			sprintf_s(cmd, sizeof(cmd), "Insert Into `operator` (`Grade`,`No`,`User`,`Password`,`right`) "
				" Values('%d','%s','%s','%ld','1');", AWinInf.GradeID, buf, buf + eq + 1, pwd);
			mysql_query(&host.mysql, cmd);
		}
	}
	//---------��ñ������---------
	fclose(fp);
	CloseMySQL(&host);	//�ر�MySQL����	
	return TRUE;
}

void Admin::OnBnClickedCmdbatch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//---------�����Ϸ���ť�Ĺ���---------
	if (AWinInf.WorkMode == WMOper)
		BatchAddUser();		//��Ա������Ϊ�����Ա
	else if (AWinInf.WorkMode == WMQust)
		BatchAddQuestions();//��������Ϊ�����Ŀ
	else
		MessageBoxA("��Ч����");
	ChMode(AWinInf.WorkMode, 0, 0);		//ˢ��
}


void Admin::OnNMKillfocusTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//cTree.EnableWindow(FALSE);
	*pResult = 0;
}


void Admin::OnBnClickedCmdclrpwd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char cmd[200];
	POSITION pos;
	int i;
	//---------���ѡ������---------
	pos = ListCtrl.GetFirstSelectedItemPosition();
	for (i = 0; ListCtrl.GetNextSelectedItem(pos) >= 0; i++);
	if (i < 1) {
		MessageBoxA("����ѡ����Ա", "��ʾ");
		return;
	}
	sprintf_s(cmd, sizeof(cmd), "ȷ��Ҫ�ָ�%d�˵�Ĭ�����룿\n(Ĭ������Ϊѧ��)", i);
	i = MessageBoxA(cmd, "ȷ��", MB_YESNO);
	if (i != IDYES)
		return;		//ȡ��
	//---------���������ݿ�---------
	MySQLHostVariable host;
	long pwd;
	int sta;	//״̬��־
	char user[72], no[72], ID[72];
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		pos = ListCtrl.GetFirstSelectedItemPosition();
		for (; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
			//---------�ָ�����Ϊѧ��---------
			ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
			ListCtrl.GetItemText(i, 1, no, sizeof(no));
			ListCtrl.GetItemText(i, 2, user, sizeof(user));
			pwd = PwdCode(user, no);
			//---------�ϴ������ݿ�---------
			sprintf_s(cmd, sizeof(cmd), "Update `operator` Set `password`='%ld' Where `ID`='%s';", pwd, ID);
			mysql_query(&host.mysql, cmd);
		}
		MessageBoxA("�ѻָ�Ĭ������", "��ʾ");
		CloseMySQL(&host);	//�ر�MySQL����	
	}
}


void Admin::OnBnClickedCmddel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char cmd[200];
	POSITION pos;
	int i;
	//---------���ѡ������---------
	pos = ListCtrl.GetFirstSelectedItemPosition();
	for (i = 0; ListCtrl.GetNextSelectedItem(pos) >= 0; i++);
	if (i < 1) {
		MessageBoxA("����ѡ��", "��ʾ");
		return;
	}
	//#define WMOper 1	//����ģʽ-ѧԱ����
	//#define WMQust 2	//����ģʽ-������
	//---------����ģʽѡ��ɾ��ʲô---------
	if (AWinInf.WorkMode == WMOper)
		sprintf_s(cmd, sizeof(cmd), "ȷ��Ҫɾ��%d����Ϣ��", i);
	else if (AWinInf.WorkMode == WMQust)
		sprintf_s(cmd, sizeof(cmd), "ȷ��Ҫɾ��%d����Ŀ��", i);
	else if (AWinInf.WorkMode == WMManage)
		sprintf_s(cmd, sizeof(cmd), "ȷ��Ҫȡ��%d��ְ���Ȩ��", i);
	else
		return;
	i = MessageBoxA(cmd, "ȷ��", MB_YESNO);
	if (i != IDYES)
		return;	//ȡ��
	//---------���������ݿ�---------
	MySQLHostVariable host;
	int sta;	//״̬��־
	char ID[72];
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		pos = ListCtrl.GetFirstSelectedItemPosition();
		if (AWinInf.WorkMode == WMOper) {
			//---------ɾ����Ա---------
			for (; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
				ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
				sprintf_s(cmd, sizeof(cmd), "Delete From `operator` Where `ID`='%s';", ID);
				mysql_query(&host.mysql, cmd);

			}
			MessageBoxA("��ɾ��", "��ʾ");
		}
		else if (AWinInf.WorkMode == WMQust) {
			//---------ɾ����Ŀ---------
			for (; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
				ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
				sprintf_s(cmd, sizeof(cmd), "Delete From `questions` Where `ID`='%s';", ID);
				mysql_query(&host.mysql, cmd);
			}
			MessageBoxA("��ɾ��", "��ʾ");
		}
		else if (AWinInf.WorkMode == WMManage) {
			//---------ɾ�������ϵ---------
			for (; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
				ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
				sprintf_s(cmd, sizeof(cmd), "Delete From `Manage` Where `ID`='%s';", ID);
				mysql_query(&host.mysql, cmd);
			}
			MessageBoxA("��ɾ��", "��ʾ");
		}
		else
			MessageBoxA("��Ч����δִ��", "��ʾ");
		CloseMySQL(&host);	//�ر�MySQL����	
	}
	ChMode(AWinInf.WorkMode, 0, 0);	//ˢ��ҳ��
}


void Admin::OnmnuNewPwd()
{
	// TODO: �ڴ���������������
	NewPwd f;	//�޸����봰��
	f.DoModal();
}


void Admin::OnApprovalUser()
{
	// TODO: �ڴ���������������
	Approval f;	//��֤����
	f.DoModal();
}


void Admin::OnmnuAutoApproval()
{
	// TODO: �ڴ���������������
	CMenu *mnu = GetMenu();
	UINT sta = GetMenuState(mnu->m_hMenu, ID_mnuAutoApproval, MF_BYCOMMAND);
	if (sta == MF_CHECKED) {	//ûѡ
		sta = 0;
		KillTimer(1);	//ֹͣ��ʱ��
	}
	else {	//ѡ����
		sta = MF_CHECKED;
		SetTimer(1, 10000, NULL);	//10�붨ʱ����ÿʮ���Զ���׼һ��
	}
	CheckMenuItem(mnu->m_hMenu, ID_mnuAutoApproval, sta);
}


void Admin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MySQLHostVariable host;
	int sta;	//״̬��־
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		mysql_query(&host.mysql, "Update `operator` SET `Right`='1' Where `right`='0';");//��׼����ѧ������
		mysql_query(&host.mysql, "Update `operator` SET `Right`='3' Where `right`='2';");//��׼���н�ʦ����
		CloseMySQL(&host);	//�ر�MySQL����	
	}
	CDialogEx::OnTimer(nIDEvent);
}


//void Admin::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	static int ClickTime = 0;			//��ִֹ�к�ClickTimeû��
//	if (pNMItemActivate->iItem == -1)	//δ�������������ֱ�Ӻ���
//		return;	
//	ChMode(AWinInf.WorkMode, ClickTime);//�뱣�ֵ��ʱ�Ĺ���״̬
//	ClickTime = 1 - ClickTime;		//��֤ClickTime��01֮��ѭ��
//	*pResult = 0;
//}


void Admin::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnCancel();
}


void Admin::Onmnuteacher()
{
	// TODO: �ڴ���������������
	TeacherManage();
}


void Admin::OnBnClickedCmdapprovaltask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char cmd[200];
	POSITION pos;
	int i, j;
	//---------���ѡ������---------
	pos = ListCtrl.GetFirstSelectedItemPosition();
	for (i = 0; ListCtrl.GetNextSelectedItem(pos) >= 0; i++);
	if (i < 1) {
		MessageBoxA("����ѡ��ֹ�", "��ʾ");
		return;
	}
	sprintf_s(cmd, sizeof(cmd), "ȷ��Ҫ��׼%d����ѧ����", i);
	i = MessageBoxA(cmd, "ȷ��", MB_YESNO);
	if (i != IDYES)
		return;	//ȡ��
	//---------���������ݿ�---------
	MySQLHostVariable host;
	int sta;	//״̬��־
	char ID[72];
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		pos = ListCtrl.GetFirstSelectedItemPosition();
		for (j = 0; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
			//---------�ϴ������ݿ�---------
			ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
			sprintf_s(cmd, sizeof(cmd), "Update `Manage` Set `Right`='1' Where `ID`='%s';", ID);
			mysql_query(&host.mysql, cmd);
			j = j + (int)host.mysql.affected_rows;
		}
		if (j > 0)
			sprintf_s(cmd, sizeof(cmd), "����׼%d����ѧ����", j);
		else
			strcpy_s(cmd, sizeof(cmd), "���������Ľ�ѧ����");
		MessageBoxA(cmd, "��ʾ");
		CloseMySQL(&host);	//�ر�MySQL����	
	}
	ChMode(WMManage, 0, 0);
}


void Admin::OnBnClickedCmdsave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString Fn;
	CString Fndef;
	//---------�Զ������ļ���---------
	if (AWinInf.WorkMode == WMOper)
		Fndef = "ѧ������.xls";
	else if (AWinInf.WorkMode == WMQust)
		Fndef = "���.xls";
	else if (AWinInf.WorkMode == WMManage)
		Fndef = "��ѧ�ֹ�.xls";
	else {
		MessageBoxA("��Ч����");
		return;
	}
	//---------����ļ�·��---------
	CFileDialog dlg(FALSE, NULL, Fndef, OFN_HIDEREADONLY, "Excel�ļ�(*.XLS)|*.XLS|");//ѡ�񵼳��ı��ļ�
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return;
	if (WriteListCtrl(&ListCtrl, Fn.GetBuffer()) == TRUE)
		MessageBoxA("�ļ������ɹ�");
	else  //�ļ���ʧ��
		MessageBoxA("����ʧ�ܣ���ر�Ŀ���ļ�������");
}


int WriteListCtrl(CListCtrl *ListCtrl, char * Fn) {//�������д�ı��ļ�
	int x, y;
	int i, j;
	char buf[72];
	CHeaderCtrl*   pHeaderCtrl;
	HDITEM Item;
	FILE *fp = NULL;
	//---------���ļ�---------
	if (fopen_s(&fp, Fn, "wt") != 0)
		return FALSE;
	pHeaderCtrl = ListCtrl->GetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return FALSE;
	x = pHeaderCtrl->GetItemCount();	//����
	//---------д�ļ�---------
	for (i = 0; i < x; i++) {
		memset(&Item, 0, sizeof(Item));
		Item.mask = HDI_TEXT;
		Item.pszText = buf;
		Item.cchTextMax = 512;
		pHeaderCtrl->GetItem(i, &Item);
		if (i == 0)
			fprintf(fp, "\"%s\"", buf);
		else
			fprintf(fp, "\t\"%s\"", buf);	//���������Ʊ�����ָ�
	}
	fprintf(fp, "\n", buf);	//����
	//---------д�ļ�---------
	y = ListCtrl->GetItemCount();
	for (i = 0; i < y; i++) {
		for (j = 0; j < x; j++) {
			ListCtrl->GetItemText(i, j, buf, sizeof(buf));
			if (j == 0)
				fprintf(fp, "=\"%s\"", buf);
			else
				fprintf(fp, "\t=\"%s\"", buf);
		}
		fprintf(fp, "\n", buf);
	}
	fclose(fp);		//�ر��ļ�
	return TRUE;
}


void Admin::OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	if (AWinInf.WorkMode == WMOper && phdr->iItem != 3) {
		MessageBoxA("��Ա������ֻ�ܰ�Ȩ������");
		return;		//��ԱԱ������ֻ�ܰ�Ȩ������
	}
	if (AWinInf.WorkMode == WMQust) {
		MessageBoxA("�γ̹�����������Ч��");
		return;		//��������������
	}
	//��ѧ��������������������
	static int ClickTime = 0;		//��ִֹ�к�ClickTimeû��
	ChMode(AWinInf.WorkMode, phdr->iItem, ClickTime);//�뱣�ֵ��ʱ�Ĺ���״̬
	ClickTime = 1 - ClickTime;		//��֤ClickTime��01֮��ѭ��
	*pResult = 0;
}


void Admin::OnBnClickedTestgen()
{
	TestGenor t;
	t.CourseID = AWinInf.Course;	//�����ѡ���λ��������Ŀ
	t.DoModal();
	ChMode(AWinInf.WorkMode, 0, 0);	//���ɺ�ˢ��ҳ��
}
