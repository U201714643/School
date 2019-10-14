// Teacher.cpp : 实现文件
//

#include "stdafx.h"
#include "include\mysql.h"
#include "Support.h"
#include "School.h"
#include "Teacher.h"
#include "NewPwd.h"
#include "afxdialogex.h"

typedef struct teacherlocal {	//admin窗体共用变量
	HTREEITEM hTreeItemCourse, hTreeItemGrade;
	int Manage;	 //班级管理界面标志
	int	WorkMode;//0-无,1-考题分析,2-习题分析,3-学生考分,4-学生练习分
	char ItemText[72];//工作参数
	int	 Course;
}teacherlocal;

extern global gs;
static teacherlocal ts;

// Teacher 对话框

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
	ON_NOTIFY(HDN_ITEMCLICK, 0, &Teacher::OnHdnItemclickList)
END_MESSAGE_MAP()


// Teacher 消息处理程序


void Teacher::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void Teacher::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void Teacher::OnBnClickedCmdok()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

BOOL Teacher::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	char buf[72];
	sprintf_s(buf, sizeof(buf), "欢迎 %s 老师", gs.op.Name);
	SetWindowTextA(buf);

	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	HTREEITEM hSubItem1;
	int sta;	//状态标志
	int i, j;
	sta = InitMySQL(&host);//连接MySQL数据库
	ts.hTreeItemCourse = cTree.InsertItem("课程", TVI_ROOT);//在根结点上添加Parent
	if (sta == TRUE) {
		mysql_query(&host.mysql, "Select `Course`,`CourseName`  From `Course` order by `Course`");
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		for (hSubItem1 = NULL, i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			hSubItem1 = cTree.InsertItem(row[1], ts.hTreeItemCourse, hSubItem1);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
	};
	ts.hTreeItemGrade = cTree.InsertItem("分管班级", TVI_ROOT);//在根结点上添加Parent
	cTabMod.InsertItem(0, "  考题分析  ");
	cTabMod.InsertItem(1, "  习题分析  ");
	cTabMod.InsertItem(2, "  学生考分  ");
	cTabMod.InsertItem(3, "  学生练习分  ");
	cTabMod.SetCurSel(0);
	ts.WorkMode = 1;
	cCmdManage.ShowWindow(SW_HIDE);
	ts.Manage = FALSE;
	return TRUE;
}

int Teacher::ChManageMode(void) {	//分管班级
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志	
	int i, j, k, len;
	int ListLine;
	char cmd[128];
	int * GradeID;	//保存班级ID用于与授权表比对。
	ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //设置list风格  
	ListCtrl.DeleteAllItems();
	for (; ListCtrl.DeleteColumn(0) == TRUE;);
	sta = InitMySQL(&host);//连接MySQL数据库
	//确定当前选中的课程，查课程ID
	if (sta != TRUE)	
		return FALSE;
	mysql_query(&host.mysql, "Select `ID`,`Name`  From `Grade` Order By `ID`;");
	result = mysql_store_result(&host.mysql);
	if (result != NULL)
		ListLine = (long)result->row_count;//总数
	else
		ListLine = 0;
	GradeID = (int *)malloc(ListLine * sizeof(int));
	ListCtrl.InsertColumn(0, "ID", LVCFMT_CENTER, -1, -1);
	ListCtrl.InsertColumn(1, "班  级", LVCFMT_CENTER, -1, -1);
	ListCtrl.InsertColumn(2, "状  态", LVCFMT_CENTER, -1, -1);
	ListCtrl.SetColumnWidth(0, 0);
	ListCtrl.SetColumnWidth(1, 100);
	ListCtrl.SetColumnWidth(2, 75);
	for (i = 0; i < ListLine; i++) {
		row = mysql_fetch_row(result);
		GradeID[i] = atoi(row[0]);//每行对应的班级ID
		ListCtrl.InsertItem(i, row[0]);
		ListCtrl.SetItemText(i, 1, row[1]);
		ListCtrl.SetItemText(i, 2, "");
	}
	mysql_free_result(result);
	sprintf_s(cmd, sizeof(cmd), "Select `GradeID`,`Right`  From `Manage` Where `OperatorID`='%d'", gs.op.ID);
	mysql_query(&host.mysql, cmd);
	result = mysql_store_result(&host.mysql);
	if (result != NULL)
		len = (long)result->row_count;//总数
	else
		len = 0;
	for (i = 0; i < len; i++) {
		row = mysql_fetch_row(result);
		k = atoi(row[0]);//为效率起见放循环外
		for (j = 0; j < ListLine; j++) {
			if (GradeID[j] == k) {
				if (atoi(row[1]) == 1)
					ListCtrl.SetItemText(j, 2, "执教中");
				else
					ListCtrl.SetItemText(j, 2, "申请中");
				break;
			}
		}
	}
	mysql_free_result(result);
	free(GradeID);
	CloseMySQL(&host);	//关闭MySQL连接
	return TRUE;
}

int Teacher::ChMode(int Mode, int CmdNum, int CilckTime) {
	//0-无,1-试题,2-练习,3-学生
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	int i, j, k;
	int len;
	char *BufNow;
	char buf[512];
	char cmd[768];
	ListCtrl.DeleteAllItems();
	for (; ListCtrl.DeleteColumn(0) == TRUE;);
	sta = InitMySQL(&host);//连接MySQL数据库
	//确定当前选中的课程，查课程ID
	if (sta != TRUE)	return FALSE;
	sprintf_s(cmd, sizeof(cmd), "Select `Course`,`CourseName`  From `Course` Where `CourseName`='%s';", ts.ItemText);
	mysql_query(&host.mysql, cmd);
	result = mysql_store_result(&host.mysql);
	if (result != NULL)
		j = (long)result->row_count;//总数
	else
		j = 0;
	if (j == 1) {
		row = mysql_fetch_row(result);
		ts.Course = atoi(row[0]);
	}
	mysql_free_result(result);
	if (ts.Course == 0) {
		CloseMySQL(&host);	//关闭MySQL连接
		return FALSE;
	}
	//确定有权管理的班级		
	sprintf_s(cmd, sizeof(cmd), "Select GradeID From `Manage` Where (`OperatorID`='%d' And `Right`='1') ;", gs.op.ID);
	mysql_query(&host.mysql, cmd);
	result = mysql_store_result(&host.mysql);
	if (result != NULL)
		j = (long)result->row_count;//总数
	else
		j = 0;
	if (j == 0) {	//未授权管理任何班级
		MessageBoxA("未授权管理任何班级");
		CloseMySQL(&host);	//关闭MySQL连接
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
		//--------仅用于题目查看--------
		static char *RankType[7] = { "questions.ID","","","" ,"SUM(examdetails.Correct)" ,"SUM(examdetails.Error)" ,"SUM(examdetails.Correct)/(SUM(examdetails.Error)+SUM(examdetails.Correct))" };
		//前四个均为无效操作（默认为问题内部编号）,正确数，错误数，正确率
		//绘制表格标题
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //设置list风格  
		ListCtrl.InsertColumn(0, "ID", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(1, "序  号", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "题  干", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(3, "答  案", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(4, "正确数", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(5, "错误数", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(6, "正确率(%)", LVCFMT_CENTER, -1, -1);
		ListCtrl.SetColumnWidth(0, 0);
		ListCtrl.SetColumnWidth(1, 0);
		ListCtrl.SetColumnWidth(2, 100);
		ListCtrl.SetColumnWidth(3, 60);
		ListCtrl.SetColumnWidth(4, 60);
		ListCtrl.SetColumnWidth(5, 60);
		ListCtrl.SetColumnWidth(6, 80);
		if (CilckTime == 0) {	//默认为降序
			sprintf_s(cmd, sizeof(cmd), "SELECT questions.ID,questions.Text,questions.Answer,"
				"SUM(examdetails.Correct),SUM(examdetails.Error)FROM examdetails INNER JOIN exam ON "
				"examdetails.ExamID = exam.ID INNER JOIN operator ON exam.OperatorID = operator.ID INNER JOIN "
				"grade ON operator.grade = grade.ID INNER JOIN questions ON examdetails.QuestID = questions.ID "
				"WHERE ((%s)&&(ExamType='%d')&&(exam.CourseID='%d')) "
				"GROUP BY questions.ID ORDER BY %s DESC;", buf, Mode - 1, ts.Course, RankType[CmdNum]);
		}
		else{	//再点为升序
			sprintf_s(cmd, sizeof(cmd), "SELECT questions.ID,questions.Text,questions.Answer,"
				"SUM(examdetails.Correct),SUM(examdetails.Error)FROM examdetails INNER JOIN exam ON "
				"examdetails.ExamID = exam.ID INNER JOIN operator ON exam.OperatorID = operator.ID INNER JOIN "
				"grade ON operator.grade = grade.ID INNER JOIN questions ON examdetails.QuestID = questions.ID "
				"WHERE ((%s)&&(ExamType='%d')&&(exam.CourseID='%d')) "
				"GROUP BY questions.ID ORDER BY %s ASC;", buf, Mode - 1, ts.Course, RankType[CmdNum]);
		}
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		double e, c;
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);
			sprintf_s(buf, sizeof(buf), "%d", i + 1);	//row中内容不能修改
			ListCtrl.SetItemText(i, 1, buf);
			sprintf_s(buf, sizeof(buf), "%s", row[1]);	//row中内容不能修改
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
		//--------仅用于学生信息查看--------
		static char *RankType[7] = { "grade.ID,operator.`No`","grade.ID,SUM(exam.Correct)/(SUM(exam.Error)+SUM(exam.Correct))","","" ,"SUM(exam.Correct)" ,"SUM(exam.Error)" ,"SUM(exam.Correct)/(SUM(exam.Error)+SUM(exam.Correct))" };
		//默认为学生内部编号，按班级（+正确率）排序，2、3均为无效操作,正确数，错误数，正确率
		//绘制表格标题
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //设置list风格  
		ListCtrl.InsertColumn(0, "班级ID", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(1, "班  级", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "学  号", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(3, "姓  名", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(4, "正确数", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(5, "错误数", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(6, "正确率(%)", LVCFMT_CENTER, -1, -1);
		ListCtrl.SetColumnWidth(0, 0);
		ListCtrl.SetColumnWidth(1, 100);
		ListCtrl.SetColumnWidth(2, 60);
		ListCtrl.SetColumnWidth(3, 60);
		ListCtrl.SetColumnWidth(4, 60);
		ListCtrl.SetColumnWidth(5, 60);
		ListCtrl.SetColumnWidth(6, 80);
		if (CilckTime == 0) {	//默认为降序
			sprintf_s(cmd, sizeof(cmd), "SELECT grade.ID,grade.`Name`,operator.`No`,operator.`user`,"
				"Sum(exam.Correct),Sum(exam.Error) FROM exam INNER JOIN operator ON exam.OperatorID = operator.ID "
				"INNER JOIN grade ON operator.grade = grade.ID WHERE ((%s)&&(exam.CourseID='%d')&&(exam.ExamType='%d')) "
				"GROUP BY grade.ID,operator.`No` ORDER BY %s DESC", buf, ts.Course, Mode - 3, RankType[CmdNum]);
		}
		else {	//再点为升序
			sprintf_s(cmd, sizeof(cmd), "SELECT grade.ID,grade.`Name`,operator.`No`,operator.`user`,"
				"Sum(exam.Correct),Sum(exam.Error) FROM exam INNER JOIN operator ON exam.OperatorID = operator.ID "
				"INNER JOIN grade ON operator.grade = grade.ID WHERE ((%s)&&(exam.CourseID='%d')&&(exam.ExamType='%d')) "
				"GROUP BY grade.ID,operator.`No` ORDER BY %s ASC", buf, ts.Course, Mode - 3, RankType[CmdNum]);
		}
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
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
	CloseMySQL(&host);	//关闭MySQL连接
	return TRUE;
}

void Teacher::OnTcnSelchangeTabmod(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ts.WorkMode = cTabMod.GetCurSel() + 1;
	ChMode(ts.WorkMode, 0, 0);
	*pResult = 0;
}

void Teacher::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

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
		if (ItemP == ts.hTreeItemCourse) {		//用户点击选择某课程
			ItemText = cTree.GetItemText(hItem);
			if (strcmp(ts.ItemText, ItemText) != 0) {
				strcpy_s(ts.ItemText, sizeof(ts.ItemText), ItemText);
				cTabMod.ShowWindow(SW_SHOW);
				cCmdManage.ShowWindow(SW_HIDE);
				ts.Manage = FALSE;
				ChMode(ts.WorkMode, 0, 0);
			}
		}
	}
	*pResult = 0;
}


void Teacher::OnBnClickedCmdsave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Fn;
	char * Fndef;
	if (ts.Manage == TRUE)
		Fndef = "分管班级.xls";
	else if (ts.WorkMode == 1)
		Fndef = "考题分析.xls";
	else if (ts.WorkMode == 2)
		Fndef = "习题分析.xls";
	else if (ts.WorkMode == 3)
		Fndef = "学生考分.xls";
	else if (ts.WorkMode == 4)
		Fndef = "学生练习分.xls";
	else
		return;
	CFileDialog dlg(FALSE, NULL, Fndef, OFN_HIDEREADONLY, "Excel文件(*.XLS)|*.XLS|");//选择导出文本文件
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return;
	if (WriteListCtrl(&ListCtrl, Fn.GetBuffer()) == TRUE)
		MessageBoxA("文件导出成功");
	else
		MessageBoxA("导出失败，请关闭目标文件后再试");
}


void Teacher::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnCancel();
}


void Teacher::OnmnuExit()
{
	// TODO: 在此添加命令处理程序代码
	CDialogEx::OnOK();
}


void Teacher::OnmnuNewPwd()
{
	// TODO: 在此添加命令处理程序代码
	NewPwd f;
	f.DoModal();
}


void Teacher::OnmnuExport()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedCmdsave();
}


void Teacher::OnBnClickedCmdmanage()
{
	// TODO: 在此添加控件通知处理程序代码
	char cmd[200];
	POSITION pos;
	int i, j;
	pos = ListCtrl.GetFirstSelectedItemPosition();
	for (i = 0; ListCtrl.GetNextSelectedItem(pos) >= 0; i++);
	if (i < 1) {
		MessageBoxA("请先选择班级", "提示");
		return;
	}
	sprintf_s(cmd, sizeof(cmd), "确认要申请管理%d个班级？", i);
	i = MessageBoxA(cmd, "确认", MB_YESNO);
	if (i != IDYES)
		return;

	MySQLHostVariable host;
	int sta;	//状态标志
	char Note[72], ID[72];
	sta = InitMySQL(&host);//连接MySQL数据库
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
		sprintf_s(cmd, sizeof(cmd), "已申请管理%d个班级", j);
		MessageBoxA(cmd, "提示");
		CloseMySQL(&host);	//关闭MySQL连接	
		ChManageMode();
	}
}


void Teacher::OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	HTREEITEM hItem, ItemP;
	hItem = cTree.GetSelectedItem();
	static int ClickTime = 0;		//防止执行后ClickTime没了
	if (hItem == ts.hTreeItemGrade) {		//进入管班变更模式
		MessageBoxA("班级教学权限管理模式下排序无效。");
		return;		//管班变更模式中排序无意义
	}
	else {
		ItemP = cTree.GetParentItem(hItem);
		if (ItemP == ts.hTreeItemCourse) {	//用户点击选择某课程
			if (phdr->iItem < 4) {
				if ((ts.WorkMode == 1) || (ts.WorkMode == 2)) {
					MessageBoxA("不能对题目和答案排序。");
					return;		//查看题目模式下，不能对题目和答案排序
				}
				else if (phdr->iItem != 1) {
					MessageBoxA("不能对学号和姓名排序。");	
					return;		//查看学生答题情况模式下，不能对学号和姓名排序
				}
				//否则正常进行查询
			}
			ChMode(ts.WorkMode, phdr->iItem, ClickTime);	//进入对应课程
		}
	}
	ClickTime = 1 - ClickTime;		//保证ClickTime在01之间循环*/
	*pResult = 0;
}
