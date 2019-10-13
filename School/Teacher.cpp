// Teacher.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "include\mysql.h"
#include "Support.h"
#include "School.h"
#include "Teacher.h"
#include "NewPwd.h"
#include "afxdialogex.h"

typedef struct teacherlocal {	//admin���干�ñ���
	HTREEITEM hTreeItemCourse, hTreeItemGrade;
	int Manage;	 //�༶��������־
	int	WorkMode;//0-��,1-�������,2-ϰ�����,3-ѧ������,4-ѧ����ϰ��
	char ItemText[72];//��������
	int	 Course;
}teacherlocal;

extern global gs;
static teacherlocal ts;

// Teacher �Ի���

IMPLEMENT_DYNAMIC(Teacher, CDialogEx)

Teacher::Teacher(CWnd* pParent /*=NULL*/)
	: CDialogEx(Teacher::IDD, pParent)
{

}

Teacher::~Teacher()
{
}

void Teacher::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, cTree);
	DDX_Control(pDX, IDC_TabMod, cTabMod);
	DDX_Control(pDX, IDC_LIST, ListCtrl);
	DDX_Control(pDX, IDC_CmdManage, cCmdManage);
}


BEGIN_MESSAGE_MAP(Teacher, CDialogEx)
	ON_BN_CLICKED(IDOK, &Teacher::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Teacher::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CmdOK, &Teacher::OnBnClickedCmdok)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TabMod, &Teacher::OnTcnSelchangeTabmod)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &Teacher::OnTvnSelchangedTree)
	ON_BN_CLICKED(IDC_CmdSave, &Teacher::OnBnClickedCmdsave)
	ON_WM_CLOSE()
	ON_COMMAND(ID_mnuExit, &Teacher::OnmnuExit)
	ON_COMMAND(ID_mnuNewPwd, &Teacher::OnmnuNewPwd)
	ON_COMMAND(ID_mnuExport, &Teacher::OnmnuExport)
	ON_BN_CLICKED(IDC_CmdManage, &Teacher::OnBnClickedCmdmanage)
END_MESSAGE_MAP()


// Teacher ��Ϣ�������


void Teacher::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void Teacher::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void Teacher::OnBnClickedCmdok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}

BOOL Teacher::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	char buf[72];
	sprintf_s(buf, sizeof(buf), "��ӭ %s ��ʦ", gs.op.Name);
	SetWindowTextA(buf);

	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	HTREEITEM hSubItem1;
	int sta;	//״̬��־
	int i, j;
	sta = InitMySQL(&host);//����MySQL���ݿ�
	ts.hTreeItemCourse = cTree.InsertItem("�γ�", TVI_ROOT);//�ڸ���������Parent
	if (sta == TRUE) {
		mysql_query(&host.mysql, "Select `Course`,`CourseName`  From `Course` order by `Course`");
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		for (hSubItem1 = NULL, i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			hSubItem1 = cTree.InsertItem(row[1], ts.hTreeItemCourse, hSubItem1);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//�ر�MySQL����	
	};
	ts.hTreeItemGrade = cTree.InsertItem("�ֹܰ༶", TVI_ROOT);//�ڸ���������Parent
	cTabMod.InsertItem(0, "  �������  ");
	cTabMod.InsertItem(1, "  ϰ�����  ");
	cTabMod.InsertItem(2, "  ѧ������  ");
	cTabMod.InsertItem(3, "  ѧ����ϰ��  ");
	cTabMod.SetCurSel(0);
	ts.WorkMode = 1;
	cCmdManage.ShowWindow(SW_HIDE);
	ts.Manage = FALSE;
	return TRUE;
}

int Teacher::ChManageMode(void) {	//�ֹܰ༶
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־	
	int i, j, k, len;
	int ListLine;
	char cmd[128];
	int * GradeID;	//����༶ID��������Ȩ��ȶԡ�
	ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //����list���  
	ListCtrl.DeleteAllItems();
	for (; ListCtrl.DeleteColumn(0) == TRUE;);
	sta = InitMySQL(&host);//����MySQL���ݿ�
	//ȷ����ǰѡ�еĿγ̣���γ�ID
	if (sta != TRUE)	return FALSE;

	mysql_query(&host.mysql, "Select `ID`,`Name`  From `Grade` Order By `ID`;");
	result = mysql_store_result(&host.mysql);
	if (result != NULL)
		ListLine = (long)result->row_count;//����
	else
		ListLine = 0;
	GradeID = (int *)malloc(ListLine * sizeof(int));
	ListCtrl.InsertColumn(0, "ID", LVCFMT_CENTER, -1, -1);
	ListCtrl.InsertColumn(1, "��  ��", LVCFMT_CENTER, -1, -1);
	ListCtrl.InsertColumn(2, "��  ��", LVCFMT_CENTER, -1, -1);
	ListCtrl.SetColumnWidth(0, 50);
	ListCtrl.SetColumnWidth(1, 100);
	ListCtrl.SetColumnWidth(2, 50);
	for (i = 0; i < ListLine; i++) {
		row = mysql_fetch_row(result);
		GradeID[i] = atoi(row[0]);//ÿ�ж�Ӧ�İ༶ID
		ListCtrl.InsertItem(i, row[0]);
		ListCtrl.SetItemText(i, 1, row[1]);
		ListCtrl.SetItemText(i, 2, "");
	}
	mysql_free_result(result);

	sprintf_s(cmd, sizeof(cmd), "Select `GradeID`,`Right`  From `Manage` Where `OperatorID`='%d';", gs.op.ID);
	mysql_query(&host.mysql, cmd);
	result = mysql_store_result(&host.mysql);
	if (result != NULL)
		len = (long)result->row_count;//����
	else
		len = 0;
	for (i = 0; i < len; i++) {
		row = mysql_fetch_row(result);
		k = atoi(row[0]);//ΪЧ�������ѭ����
		for (j = 0; j < ListLine; j++) {
			if (GradeID[j] == k) {
				if (atoi(row[1]) == 1)
					ListCtrl.SetItemText(j, 2, "�ܰ�");
				else
					ListCtrl.SetItemText(j, 2, "����");
				break;
			}
		}
	}
	mysql_free_result(result);
	free(GradeID);
	CloseMySQL(&host);	//�ر�MySQL����
	return TRUE;
}

int Teacher::ChMode(int Mode) {
	//0-��,1-����,2-��ϰ,3-ѧ��
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//״̬��־
	int i, j, k;
	int len;
	char *BufNow;
	char buf[512];
	char cmd[768];
	ListCtrl.DeleteAllItems();
	for (; ListCtrl.DeleteColumn(0) == TRUE;);
	sta = InitMySQL(&host);//����MySQL���ݿ�
	//ȷ����ǰѡ�еĿγ̣���γ�ID
	if (sta != TRUE)	return FALSE;
	sprintf_s(cmd, sizeof(cmd), "Select `Course`,`CourseName`  From `Course` Where `CourseName`='%s';", ts.ItemText);
	mysql_query(&host.mysql, cmd);
	result = mysql_store_result(&host.mysql);
	if (result != NULL)
		j = (long)result->row_count;//����
	else
		j = 0;
	if (j == 1) {
		row = mysql_fetch_row(result);
		ts.Course = atoi(row[0]);
	}
	mysql_free_result(result);
	if (ts.Course == 0) {
		CloseMySQL(&host);	//�ر�MySQL����
		return FALSE;
	}
	//ȷ����Ȩ����İ༶		
	sprintf_s(cmd, sizeof(cmd), "Select GradeID From `Manage` Where (`OperatorID`='%d' And `Right`='1') ;", gs.op.ID);
	mysql_query(&host.mysql, cmd);
	result = mysql_store_result(&host.mysql);
	if (result != NULL)
		j = (long)result->row_count;//����
	else
		j = 0;
	if (j == 0) {	//δ��Ȩ�����κΰ༶
		MessageBoxA("δ��Ȩ�����κΰ༶");
		CloseMySQL(&host);	//�ر�MySQL����
		return FALSE;
	}
	for (BufNow = buf, BufNow[0] = 0, len = sizeof(buf), i = 0; i < j; i++) {
		row = mysql_fetch_row(result);
		k = sprintf_s(BufNow, len, "grade.ID='%s' or ", row[0]);
		len = len - k;
		BufNow = BufNow + k;
	}
	if (BufNow - buf > 14)
		BufNow[-3] = 0;
	mysql_free_result(result);
	if ((Mode == 1) || (Mode == 2)) {
		//���Ʊ�����
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //����list���  
		ListCtrl.InsertColumn(0, "ID", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(1, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "��  ��", LVCFMT_RIGHT, -1, -1);
		ListCtrl.InsertColumn(3, "��  ��", LVCFMT_LEFT, -1, -1);
		ListCtrl.InsertColumn(4, "��ȷ��", LVCFMT_RIGHT, -1, -1);
		ListCtrl.InsertColumn(5, "������", LVCFMT_RIGHT, -1, -1);
		ListCtrl.InsertColumn(6, "��ȷ��(%)", LVCFMT_RIGHT, -1, -1);
		ListCtrl.SetColumnWidth(0, 50);
		ListCtrl.SetColumnWidth(1, 50);
		ListCtrl.SetColumnWidth(2, 100);
		ListCtrl.SetColumnWidth(3, 80);
		ListCtrl.SetColumnWidth(4, 80);
		ListCtrl.SetColumnWidth(5, 80);
		ListCtrl.SetColumnWidth(6, 80);
		sprintf_s(cmd, sizeof(cmd), "SELECT questions.ID,questions.Text,questions.Answer,"
			"SUM(examdetails.Correct),SUM(examdetails.Error)FROM examdetails INNER JOIN exam ON "
			"examdetails.ExamID = exam.ID INNER JOIN operator ON exam.OperatorID = operator.ID INNER JOIN "
			"grade ON operator.grade = grade.ID INNER JOIN questions ON examdetails.QuestID = questions.ID "
			"WHERE ((%s)&&(ExamType='%d')&&(exam.CourseID='%d')) "
			"GROUP BY questions.ID ORDER BY questions.ID;", buf, Mode - 1, ts.Course);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		double e, c;
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);
			sprintf_s(buf, sizeof(buf), "%d", i + 1);	//row�����ݲ����޸�
			ListCtrl.SetItemText(i, 1, buf);
			sprintf_s(buf, sizeof(buf), "%s", row[1]);	//row�����ݲ����޸�
			ListCtrl.SetItemText(i, 2, RealChar(buf));
			ListCtrl.SetItemText(i, 3, row[2]);
			ListCtrl.SetItemText(i, 4, row[3]);
			ListCtrl.SetItemText(i, 5, row[4]);
			c = atoi(row[3]);
			e = atoi(row[4]);
			if (c + e > 0) {
				sprintf_s(buf, sizeof(buf), "%5.2f", 100.0*c / (c + e));
				ListCtrl.SetItemText(i, 6, buf);
			}
		}
		mysql_free_result(result);
	}
	else if ((Mode == 3) || (Mode == 4)) {
		//���Ʊ�����
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //����list���  
		ListCtrl.InsertColumn(0, "�༶ID", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(1, "��  ��", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "ѧ  ��", LVCFMT_RIGHT, -1, -1);
		ListCtrl.InsertColumn(3, "��  ��", LVCFMT_LEFT, -1, -1);
		ListCtrl.InsertColumn(4, "��ȷ��", LVCFMT_RIGHT, -1, -1);
		ListCtrl.InsertColumn(5, "������", LVCFMT_RIGHT, -1, -1);
		ListCtrl.InsertColumn(6, "��ȷ��(%)", LVCFMT_RIGHT, -1, -1);
		ListCtrl.SetColumnWidth(0, 50);
		ListCtrl.SetColumnWidth(1, 120);
		ListCtrl.SetColumnWidth(2, 50);
		ListCtrl.SetColumnWidth(3, 80);
		ListCtrl.SetColumnWidth(4, 80);
		ListCtrl.SetColumnWidth(5, 80);
		ListCtrl.SetColumnWidth(6, 80);
		sprintf_s(cmd, sizeof(cmd), "SELECT grade.ID,grade.`Name`,operator.`No`,operator.`user`,"
			"Sum(exam.Correct),Sum(exam.Error) FROM exam INNER JOIN operator ON exam.OperatorID = operator.ID "
			"INNER JOIN grade ON operator.grade = grade.ID WHERE ((%s)&&(exam.CourseID='%d')&&(exam.ExamType='%d')) "
			"GROUP BY grade.ID,operator.`No` ORDER BY grade.ID,operator.`No`", buf, ts.Course, Mode - 3);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//����
		else
			j = 0;
		double c, e;
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);
			ListCtrl.SetItemText(i, 1, row[1]);
			ListCtrl.SetItemText(i, 2, row[2]);
			ListCtrl.SetItemText(i, 3, row[3]);
			ListCtrl.SetItemText(i, 4, row[4]);
			ListCtrl.SetItemText(i, 5, row[5]);
			c = atoi(row[4]);
			e = atoi(row[5]);
			if (c + e > 0) {
				sprintf_s(buf, sizeof(buf), "%5.2f", 100.0*c / (c + e));
				ListCtrl.SetItemText(i, 6, buf);
			}
		}
		mysql_free_result(result);
	}
	else {
		;
	}
	CloseMySQL(&host);	//�ر�MySQL����
	return TRUE;
}

void Teacher::OnTcnSelchangeTabmod(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ts.WorkMode = cTabMod.GetCurSel() + 1;
	ChMode(ts.WorkMode);
	*pResult = 0;
}

void Teacher::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HTREEITEM hItem, ItemP;
	CString ItemText;
	hItem = cTree.GetSelectedItem();
	if (hItem == ts.hTreeItemGrade) {
		cTabMod.ShowWindow(SW_HIDE);
		cCmdManage.ShowWindow(SW_SHOW);
		ts.Manage = TRUE;
		ChManageMode();
	}
	else {
		ItemP = cTree.GetParentItem(hItem);
		if (ItemP == ts.hTreeItemCourse) {		//�û����ѡ��ĳ�γ�
			ItemText = cTree.GetItemText(hItem);
			if (strcmp(ts.ItemText, ItemText) != 0) {
				strcpy_s(ts.ItemText, sizeof(ts.ItemText), ItemText);
				cTabMod.ShowWindow(SW_SHOW);
				cCmdManage.ShowWindow(SW_HIDE);
				ts.Manage = FALSE;
				ChMode(ts.WorkMode);
			}
		}
	}
	*pResult = 0;
}


void Teacher::OnBnClickedCmdsave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString Fn;
	char * Fndef;
	if (ts.Manage == TRUE)
		Fndef = "�ֹܰ༶.xls";
	else if (ts.WorkMode == 1)
		Fndef = "�������.xls";
	else if (ts.WorkMode == 2)
		Fndef = "ϰ�����.xls";
	else if (ts.WorkMode == 3)
		Fndef = "ѧ������.xls";
	else if (ts.WorkMode == 4)
		Fndef = "ѧ����ϰ��.xls";
	else
		return;
	CFileDialog dlg(FALSE, NULL, Fndef, OFN_HIDEREADONLY, "Excel�ļ�(*.XLS)|*.XLS|");//ѡ�񵼳��ı��ļ�
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return;
	if (WriteListCtrl(&ListCtrl, Fn.GetBuffer()) == TRUE)
		MessageBoxA("�ļ������ɹ�");
	else
		MessageBoxA("����ʧ�ܣ���ر�Ŀ���ļ�������");
}


void Teacher::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnCancel();
}


void Teacher::OnmnuExit()
{
	// TODO: �ڴ���������������
	CDialogEx::OnOK();
}


void Teacher::OnmnuNewPwd()
{
	// TODO: �ڴ���������������
	NewPwd f;
	f.DoModal();
}


void Teacher::OnmnuExport()
{
	// TODO: �ڴ���������������
	OnBnClickedCmdsave();
}


void Teacher::OnBnClickedCmdmanage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char cmd[200];
	POSITION pos;
	int i, j;
	pos = ListCtrl.GetFirstSelectedItemPosition();
	for (i = 0; ListCtrl.GetNextSelectedItem(pos) >= 0; i++);
	if (i < 1) {
		MessageBoxA("����ѡ��༶", "��ʾ");
		return;
	}
	sprintf_s(cmd, sizeof(cmd), "ȷ��Ҫ�������%d���༶��", i);
	i = MessageBoxA(cmd, "ȷ��", MB_YESNO);
	if (i != IDYES)
		return;

	MySQLHostVariable host;
	int sta;	//״̬��־
	char Note[72], ID[72];
	sta = InitMySQL(&host);//����MySQL���ݿ�
	if (sta == TRUE) {
		pos = ListCtrl.GetFirstSelectedItemPosition();
		for (j = 0; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
			ListCtrl.GetItemText(i, 2, Note, sizeof(Note));
			if (Note[0] == 0) {
				ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
				sprintf_s(cmd, sizeof(cmd), "Insert Into `Manage` Set `OperatorID`='%d',`GradeID`='%s',`Right`=0;", gs.op.ID, ID);
				mysql_query(&host.mysql, cmd);
				j++;
			}
		}
		sprintf_s(cmd, sizeof(cmd), "���������%d���༶", j);
		MessageBoxA(cmd, "��ʾ");
		CloseMySQL(&host);	//�ر�MySQL����	
		ChManageMode();
	}
}
