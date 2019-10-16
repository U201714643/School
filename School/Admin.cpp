// admin.cpp : 实现文件
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


#define WMNone 0	//工作模式-无
#define WMOper 1	//工作模式-学员管理
#define WMQust 2	//工作模式-题库管理
#define WMManage 3	//工作模式-教学权分配

typedef struct adminlocal {	//admin窗体共用变量
	HTREEITEM hTreeItemSchool;	//"学校"
	HTREEITEM hTreeItemGroup;	//"教研组"
	HTREEITEM hTreeItemCourse;	//"课程"
	HTREEITEM hTreeItemManage;	//"教师分班"
	int	WorkMode;//0-无,1-operator,2-questions
	char ItemText[72];//工作参数
	int	 Course, GradeID;
}adminlocal;


extern global gs;
adminlocal AWinInf;


// admin 对话框

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


// admin 消息处理程序


void Admin::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void Admin::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void Admin::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	CDialogEx::OnOK();
}

BOOL Admin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//--------显示姓名---------
	char buf[72];
	sprintf_s(buf, sizeof(buf), "欢迎%s管理员", gs.op.Name);
	SetWindowTextA(buf);
	//---------连接数据库---------
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	HTREEITEM hSubItem1;
	int sta;	//状态标志
	int i, j;
	sta = InitMySQL(&host);//连接MySQL数据库
	//---------设置标题---------
	AWinInf.hTreeItemSchool = cTree.InsertItem("人员管理", TVI_ROOT);//在根结点上添加父节点“学校”
	if (sta == TRUE) {
		AWinInf.hTreeItemGroup = cTree.InsertItem("教研组", AWinInf.hTreeItemSchool, NULL);//在“学校”结点小添加“教研组”
		mysql_query(&host.mysql, "Select `ID`,`Name`  From `Grade` order by `ID`");
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		for (hSubItem1 = AWinInf.hTreeItemGroup, i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			hSubItem1 = cTree.InsertItem(row[1], AWinInf.hTreeItemSchool, hSubItem1);
		}
		mysql_free_result(result);
	};
	//---------设置标题---------
	AWinInf.hTreeItemCourse = cTree.InsertItem("题库管理", TVI_ROOT);//在根结点上添加父节点“课程”
	if (sta == TRUE) {
		mysql_query(&host.mysql, "Select `Course`,`CourseName`  From `Course` order by `Course`");
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		for (hSubItem1 = NULL, i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			hSubItem1 = cTree.InsertItem(row[1], AWinInf.hTreeItemCourse, hSubItem1);
		}
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
	};
	//---------设置标题---------
	AWinInf.hTreeItemManage = cTree.InsertItem("教学任务分配", TVI_ROOT);//在根结点上添加“教学任务分配”
	//---------绘制窗体---------
	ChMode(WMNone, 0, 0);
	return TRUE;
}

void Admin::OnBatchQuestions()
{
	// TODO: 在此添加命令处理程序代码
	ComobRet CmbRet;
	Combo SelCourse;
	CString Fn;
	CmbRet.type = 0;//0==>Course
	SelCourse.SetRet(&CmbRet);
	SelCourse.DoModal();	//选择课程
	//---------选择文件---------
	if (CmbRet.sta != TRUE)	return;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "文本文档(*.txt)|*.txt|");//选择题目文本
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return;
	//---------连接数据库---------
	char cmd[128];
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	int i, j;
	sta = InitMySQL(&host);//连接MySQL数据库
	//---------获得课程名称---------
	if (sta == TRUE) {
		sprintf_s(cmd, sizeof(cmd), "Select `Course`,`CourseName`  From `Course` Where `CourseName`='%s';", CmbRet.Sel);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
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
			CloseMySQL(&host);	//关闭MySQL连接	
			return;
		}
	};
	//---------读取文件中题目---------
	FILE *fp = NULL;
	char buf[1024];
	int eq;//等号位置
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
		//---------数据库操作---------
		if ((eq > 0) && (eq < 70) && (i - eq > 1) && (i - eq < 10)) {
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Questions` (`Course`,`Text`,`Answer`) "
				" Values('%s','%s','%s');", CmbRet.Sel, buf, buf + eq + 1);
			mysql_query(&host.mysql, cmd);
		}
	}
	//---------收尾工作---------
	fclose(fp);
	CloseMySQL(&host);	//关闭MySQL连接	
}


void Admin::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem, ItemP;
	CString ItemText;
	hItem = cTree.GetSelectedItem();
	//---------选择教师分工---------
	if (hItem == AWinInf.hTreeItemManage) {
		TeacherManage();	//用户点击选择教师分工
	}
	//---------选择题库管理---------
	else {
		ItemP = cTree.GetParentItem(hItem);
		if (ItemP == AWinInf.hTreeItemCourse) {		//用户点击选择某课程
			ItemText = cTree.GetItemText(hItem);
			strcpy_s(AWinInf.ItemText, sizeof(AWinInf.ItemText), ItemText);
			ChMode(WMQust, 0, 0);
		}
		else if (ItemP == AWinInf.hTreeItemSchool) {//用户点击选择某班级
			ItemText = cTree.GetItemText(hItem);
			strcpy_s(AWinInf.ItemText, sizeof(AWinInf.ItemText), ItemText);
			ChMode(WMOper, 0, 0);
		}
	}
	*pResult = 0;
}

int Admin::TeacherManage() {//教师分工管理模式
	ChMode(WMManage, 0, 0);
	return TRUE;
}

int Admin::ChMode(int Mode,int CmdNum,int CilckTime) {//改变工作模式
	//#define WMNone 0		//工作模式-无
	//#define WMOper 1		//工作模式-学员管理
	//#define WMQust 2		//工作模式-题库管理
	//#define WMManage 3	//工作模式-教学权分配
	//--------仅用于教学权分配--------
	static char *RankType[4] = { "`OperatorID`","`OperatorID`","grade.`ID`","manage.`Right`"};	//默认（内部序号），姓名（内部序号），执教班级，权限
	//--------其余情况无需分类处理--------
	MySQLHostVariable host;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int sta;	//状态标志
	int i, j, n;
	char buf[EXPLEN];
	char cmd[512];
	AWinInf.WorkMode = Mode;
	ListCtrl.DeleteAllItems();
	for (; ListCtrl.DeleteColumn(0) == TRUE;);
	//---------空模式---------
	if (AWinInf.WorkMode == WMNone) {
		//仅仅隐藏、禁用按钮即可
		TxtNote.SetWindowTextA("请选择");
		TestGenorArg.ShowWindow(SW_HIDE);
		CmdClrPwd.ShowWindow(SW_HIDE);
		CmdApprovalTask.ShowWindow(SW_HIDE);
		CmdBatch.EnableWindow(FALSE);
		CmdSave.EnableWindow(FALSE);
		cTree.EnableWindow(TRUE);
	}
	//---------人员管理模式---------
	else if (AWinInf.WorkMode == WMOper) {
		//---------控件标题---------
		sprintf_s(buf, sizeof(buf), "%s 人员管理", AWinInf.ItemText);
		//---------隐藏、显示按钮---------
		TxtNote.SetWindowTextA(buf);
		TestGenorArg.ShowWindow(SW_HIDE);
		CmdClrPwd.ShowWindow(SW_SHOW);
		CmdApprovalTask.ShowWindow(SW_HIDE);
		CmdBatch.EnableWindow(TRUE);
		CmdSave.EnableWindow(TRUE);
		CmdDel.SetWindowTextA("删除人员");
		AWinInf.GradeID = 0;
		//----------连接MySQL数据库----------
		sta = InitMySQL(&host);
		if (sta == TRUE) {
			sprintf_s(cmd, sizeof(cmd), "Select `ID`,`Name`  From `Grade` Where `Name`='%s';", AWinInf.ItemText);
			mysql_query(&host.mysql, cmd);
			result = mysql_store_result(&host.mysql);
			if (result != NULL)
				j = (long)result->row_count;//总数
			else
				j = 0;
			if (j == 1) {
				row = mysql_fetch_row(result);
				AWinInf.GradeID = atoi(row[0]);
			}
			mysql_free_result(result);
		};
		//----------绘制表格标题----------
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //设置list风格  
		ListCtrl.InsertColumn(0, "序  号", LVCFMT_CENTER, -1, -1);
		if (AWinInf.GradeID != 0)	//学生显示学号，教师显示工号
			ListCtrl.InsertColumn(1, "学  号", LVCFMT_CENTER, -1, -1);
		else ListCtrl.InsertColumn(1, "工  号", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "姓  名", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(3, "权  限", LVCFMT_CENTER, -1, -1);
		ListCtrl.SetColumnWidth(0, 0);
		ListCtrl.SetColumnWidth(1, 80);
		ListCtrl.SetColumnWidth(2, 80);
		ListCtrl.SetColumnWidth(3, 80);
		//----------获得表格内容----------
		if (CilckTime == 0) {	//初始为降序，且只按学号排序
			sprintf_s(cmd, sizeof(cmd), "Select `ID`,`No`,`user`,`right`  From `operator` "
				" Where `Grade`='%d' order by `right` DESC,`No` ASC;", AWinInf.GradeID);
		}
		else {	//再点一次为升序
			sprintf_s(cmd, sizeof(cmd), "Select `ID`,`No`,`user`,`right`  From `operator` "
				" Where `Grade`='%d' order by `right` ASC,`No` ASC;", AWinInf.GradeID);
		}
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			n = (long)result->row_count;//总数
		else
			n = 0;
		//---------绘制---------
		for (i = 0; i < n; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);		//内部序号
			ListCtrl.SetItemText(i, 1, row[1]);	//学号
			ListCtrl.SetItemText(i, 2, row[2]);	//姓名
			//----------权限----------
			if (row[3][0] == '0') {
				ListCtrl.SetItemText(i, 3, "待审核学生");
			}
			else if (row[3][0] == '1') {
				ListCtrl.SetItemText(i, 3, "学生");
			}
			else if (row[3][0] == '2') {
				ListCtrl.SetItemText(i, 3, "待审核教师");
			}
			else if (row[3][0] == '3') {
				ListCtrl.SetItemText(i, 3, "教师");
			}
			else if (row[3][0] == '9') {
				ListCtrl.SetItemText(i, 3, "管理员");
			}
			else {
				ListCtrl.SetItemText(i, 3, "错误");
			}
		}
		//---------释放空间、关闭连接---------
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接	
	}
	else if (AWinInf.WorkMode == WMQust) {
		//---------按钮可用性设置---------
		sprintf_s(buf, sizeof(buf), "%s课程 题库管理", AWinInf.ItemText);
		TxtNote.SetWindowTextA(buf);
		TestGenorArg.ShowWindow(SW_SHOW);
		CmdClrPwd.ShowWindow(SW_HIDE);
		CmdApprovalTask.ShowWindow(SW_HIDE);
		CmdBatch.EnableWindow(TRUE);
		CmdSave.EnableWindow(TRUE);
		CmdDel.SetWindowTextA("删除题目");
		AWinInf.Course = 0;		//清理上次的课程信息
		sta = InitMySQL(&host);	//连接MySQL数据库
		//---------连接数据库---------
		if (sta != TRUE)	return FALSE;
		sprintf_s(cmd, sizeof(cmd), "Select `Course`,`CourseName`  From `Course` Where `CourseName`='%s';", AWinInf.ItemText);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		if (j == 1) {	//课程号发生变更
			row = mysql_fetch_row(result);
			AWinInf.Course = atoi(row[0]);
		}
		mysql_free_result(result);
		if (AWinInf.Course == 0) {
			CloseMySQL(&host);	//关闭MySQL连接
			return FALSE;
		}
		//---------绘制表格标题---------
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //设置list风格  
		ListCtrl.InsertColumn(0, "  ID  ", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(1, "序  号", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "题  目", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(3, "答  案", LVCFMT_CENTER, -1, -1);
		ListCtrl.SetColumnWidth(0, 0);
		ListCtrl.SetColumnWidth(1, 0);
		ListCtrl.SetColumnWidth(2, 100);
		ListCtrl.SetColumnWidth(3, 60);
		//---------获得表格内容---------
		sprintf_s(cmd, sizeof(cmd), "Select `ID`,`Text`,`Answer`  From `questions` "
			" Where `course`='%d' order by `ID` ;", AWinInf.Course);
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		//---------填入表格内容---------
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);
			sprintf_s(buf, sizeof(buf), "%d", i + 1);	//row中内容不能修改
			ListCtrl.SetItemText(i, 1, buf);
			sprintf_s(buf, sizeof(buf), "%s", row[1]);	//row中内容不能修改
			ListCtrl.SetItemText(i, 2, RealChar(buf));
			ListCtrl.SetItemText(i, 3, row[2]);
		}
		//---------释放空间、关闭连接---------
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接
	}
	else if (AWinInf.WorkMode == WMManage) {
		//---------显示、隐藏按钮---------
		sprintf_s(buf, sizeof(buf), "教学任务分配", AWinInf.ItemText);
		TxtNote.SetWindowTextA(buf);
		TestGenorArg.ShowWindow(SW_HIDE);
		CmdClrPwd.ShowWindow(SW_HIDE);
		CmdApprovalTask.ShowWindow(SW_SHOW);
		CmdBatch.EnableWindow(FALSE);
		CmdSave.EnableWindow(TRUE);
		CmdDel.SetWindowTextA("取消任务");
		AWinInf.Course = 0;
		AWinInf.GradeID = 0;	//只有老师才有执教权限一说
		//---------连接数据库---------
		sta = InitMySQL(&host);	//连接MySQL数据库
		if (sta != TRUE)	return FALSE;
		//---------绘制表格标题---------
		ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); //设置list风格  
		ListCtrl.InsertColumn(0, "序  号", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(1, "姓  名", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(2, "班  级", LVCFMT_CENTER, -1, -1);
		ListCtrl.InsertColumn(3, "状  态", LVCFMT_CENTER, -1, -1);
		ListCtrl.SetColumnWidth(0, 0);
		ListCtrl.SetColumnWidth(1, 80);
		ListCtrl.SetColumnWidth(2, 120);
		ListCtrl.SetColumnWidth(3, 80);
		//---------获得表格内容---------
		if (CilckTime == 0) {	//初始为升序
			sprintf_s(cmd, sizeof(cmd), "SELECT manage.ID,`user`,`Name`,manage.`Right` FROM manage "
				" INNER JOIN operator ON manage.OperatorID = operator.ID "
				" INNER JOIN grade ON manage.GradeID = grade.ID"
				" Order By %s ASC;", RankType[CmdNum]);
		}
		else {	//再点一次为降序
			sprintf_s(cmd, sizeof(cmd), "SELECT manage.ID,`user`,`Name`,manage.`Right` FROM manage "
				" INNER JOIN operator ON manage.OperatorID = operator.ID "
				" INNER JOIN grade ON manage.GradeID = grade.ID"
				" Order By %s DESC;", RankType[CmdNum]);
		}
		//---------填写表格---------
		mysql_query(&host.mysql, cmd);
		result = mysql_store_result(&host.mysql);
		if (result != NULL)
			j = (long)result->row_count;//总数
		else
			j = 0;
		for (i = 0; i < j; i++) {
			row = mysql_fetch_row(result);
			ListCtrl.InsertItem(i, row[0]);
			ListCtrl.SetItemText(i, 1, row[1]);
			ListCtrl.SetItemText(i, 2, row[2]);
			if (row[3][0] == '0')
				ListCtrl.SetItemText(i, 3, "申请中");
			else
				ListCtrl.SetItemText(i, 3, "执教中");
		}
		//---------获得表格内容---------
		mysql_free_result(result);
		CloseMySQL(&host);	//关闭MySQL连接
	}
	return TRUE;
}

void Admin::OnBnClickedCmdok()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

int BatchAddQuestions(void) {
	MySQLHostVariable host;
	FILE *fp = NULL;
	char cmd[256];
	char buf[1024];
	int sta;	//状态标志
	int i;
	int eq;//等号(或逗号或Tab)位置
	long pwd;
	//---------获得文件路径---------
	CString Fn;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "文本文档(*.txt)|*.txt|");//选择题目文本
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return FALSE;
	//---------连接至数据库---------
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta != TRUE)
		return FALSE;
	//---------打开文件---------
	fopen_s(&fp, Fn, "rt");
	for (; fgets(buf, sizeof(buf), fp) != NULL;) {
		for (i = 0, eq = 0; buf[i] != 0; i++) {
			//---------读至,、=、空白符---------
			if ((buf[i] == '\t') || (buf[i] == ',') || (buf[i] == '=')) {
				eq = i;
				buf[i] = 0;
			}
			//---------读至换行---------
			if (buf[i] == 0x0A)
				buf[i] = 0;
		}
		//---------上传至数据库---------
		if ((eq > 0) && (eq < 70) && (i - eq > 1) && (i - eq < 30)) {
			pwd = PwdCode(buf + eq + 1, buf);		//密码需进行一次编码
			sprintf_s(cmd, sizeof(cmd), "Insert Into `Questions` (`Course`,`Text`,`Answer`) "
				" Values('%d','%s','%s');", AWinInf.Course, buf, buf + eq + 1, pwd);
			mysql_query(&host.mysql, cmd);
		}
	}
	//---------获得表格内容---------
	fclose(fp);
	CloseMySQL(&host);	//关闭MySQL连接	
	return TRUE;
}



int BatchAddUser(void) {
	MySQLHostVariable host;
	FILE *fp = NULL;
	char cmd[256];
	char buf[1024];
	int sta;	//状态标志
	int i;
	int eq;//等号(或逗号或Tab)位置
	long pwd;
	//---------获得文件路径---------
	CString Fn;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "学生名单文本文档(*名单.txt)|*名单.txt|");//选择学生名单文本
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return FALSE;
	//---------连接至数据库---------
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta != TRUE)
		return FALSE;
	//---------打开文件---------
	fopen_s(&fp, Fn, "rt");
	for (; fgets(buf, sizeof(buf), fp) != NULL;) {
		for (i = 0, eq = 0; buf[i] != 0; i++) {
			//---------读至,、=、空白符---------
			if ((buf[i] == '\t') || (buf[i] == ',') || (buf[i] == '=')) {
				eq = i;
				buf[i] = 0;
			}
			//---------读至换行---------
			if (buf[i] == 0x0A)
				buf[i] = 0;
		}
		//---------上传至数据库---------
		if ((eq > 0) && (eq < 70) && (i - eq > 1) && (i - eq < 30)) {
			pwd = PwdCode(buf + eq + 1, buf);	//密码需进行一次编码
			sprintf_s(cmd, sizeof(cmd), "Insert Into `operator` (`Grade`,`No`,`User`,`Password`,`right`) "
				" Values('%d','%s','%s','%ld','1');", AWinInf.GradeID, buf, buf + eq + 1, pwd);
			mysql_query(&host.mysql, cmd);
		}
	}
	//---------获得表格内容---------
	fclose(fp);
	CloseMySQL(&host);	//关闭MySQL连接	
	return TRUE;
}

void Admin::OnBnClickedCmdbatch()
{
	// TODO: 在此添加控件通知处理程序代码
	//---------决定上方按钮的功能---------
	if (AWinInf.WorkMode == WMOper)
		BatchAddUser();		//人员管理下为添加人员
	else if (AWinInf.WorkMode == WMQust)
		BatchAddQuestions();//题库管理下为添加题目
	else
		MessageBoxA("无效命令");
	ChMode(AWinInf.WorkMode, 0, 0);		//刷新
}


void Admin::OnNMKillfocusTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//cTree.EnableWindow(FALSE);
	*pResult = 0;
}


void Admin::OnBnClickedCmdclrpwd()
{
	// TODO: 在此添加控件通知处理程序代码
	char cmd[200];
	POSITION pos;
	int i;
	//---------获得选择人数---------
	pos = ListCtrl.GetFirstSelectedItemPosition();
	for (i = 0; ListCtrl.GetNextSelectedItem(pos) >= 0; i++);
	if (i < 1) {
		MessageBoxA("请先选择人员", "提示");
		return;
	}
	sprintf_s(cmd, sizeof(cmd), "确认要恢复%d人的默认密码？\n(默认密码为学号)", i);
	i = MessageBoxA(cmd, "确认", MB_YESNO);
	if (i != IDYES)
		return;		//取消
	//---------连接至数据库---------
	MySQLHostVariable host;
	long pwd;
	int sta;	//状态标志
	char user[72], no[72], ID[72];
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		pos = ListCtrl.GetFirstSelectedItemPosition();
		for (; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
			//---------恢复密码为学号---------
			ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
			ListCtrl.GetItemText(i, 1, no, sizeof(no));
			ListCtrl.GetItemText(i, 2, user, sizeof(user));
			pwd = PwdCode(user, no);
			//---------上传至数据库---------
			sprintf_s(cmd, sizeof(cmd), "Update `operator` Set `password`='%ld' Where `ID`='%s';", pwd, ID);
			mysql_query(&host.mysql, cmd);
		}
		MessageBoxA("已恢复默认密码", "提示");
		CloseMySQL(&host);	//关闭MySQL连接	
	}
}


void Admin::OnBnClickedCmddel()
{
	// TODO: 在此添加控件通知处理程序代码
	char cmd[200];
	POSITION pos;
	int i;
	//---------获得选择人数---------
	pos = ListCtrl.GetFirstSelectedItemPosition();
	for (i = 0; ListCtrl.GetNextSelectedItem(pos) >= 0; i++);
	if (i < 1) {
		MessageBoxA("请先选择", "提示");
		return;
	}
	//#define WMOper 1	//工作模式-学员管理
	//#define WMQust 2	//工作模式-题库管理
	//---------根据模式选择删除什么---------
	if (AWinInf.WorkMode == WMOper)
		sprintf_s(cmd, sizeof(cmd), "确认要删除%d人信息吗？", i);
	else if (AWinInf.WorkMode == WMQust)
		sprintf_s(cmd, sizeof(cmd), "确认要删除%d道题目吗？", i);
	else if (AWinInf.WorkMode == WMManage)
		sprintf_s(cmd, sizeof(cmd), "确认要取消%d项分班授权？", i);
	else
		return;
	i = MessageBoxA(cmd, "确认", MB_YESNO);
	if (i != IDYES)
		return;	//取消
	//---------连接至数据库---------
	MySQLHostVariable host;
	int sta;	//状态标志
	char ID[72];
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		pos = ListCtrl.GetFirstSelectedItemPosition();
		if (AWinInf.WorkMode == WMOper) {
			//---------删除人员---------
			for (; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
				ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
				sprintf_s(cmd, sizeof(cmd), "Delete From `operator` Where `ID`='%s';", ID);
				mysql_query(&host.mysql, cmd);

			}
			MessageBoxA("已删除", "提示");
		}
		else if (AWinInf.WorkMode == WMQust) {
			//---------删除题目---------
			for (; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
				ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
				sprintf_s(cmd, sizeof(cmd), "Delete From `questions` Where `ID`='%s';", ID);
				mysql_query(&host.mysql, cmd);
			}
			MessageBoxA("已删除", "提示");
		}
		else if (AWinInf.WorkMode == WMManage) {
			//---------删除管理关系---------
			for (; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
				ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
				sprintf_s(cmd, sizeof(cmd), "Delete From `Manage` Where `ID`='%s';", ID);
				mysql_query(&host.mysql, cmd);
			}
			MessageBoxA("已删除", "提示");
		}
		else
			MessageBoxA("无效命令未执行", "提示");
		CloseMySQL(&host);	//关闭MySQL连接	
	}
	ChMode(AWinInf.WorkMode, 0, 0);	//刷新页面
}


void Admin::OnmnuNewPwd()
{
	// TODO: 在此添加命令处理程序代码
	NewPwd f;	//修改密码窗口
	f.DoModal();
}


void Admin::OnApprovalUser()
{
	// TODO: 在此添加命令处理程序代码
	Approval f;	//认证窗口
	f.DoModal();
}


void Admin::OnmnuAutoApproval()
{
	// TODO: 在此添加命令处理程序代码
	CMenu *mnu = GetMenu();
	UINT sta = GetMenuState(mnu->m_hMenu, ID_mnuAutoApproval, MF_BYCOMMAND);
	if (sta == MF_CHECKED) {	//没选
		sta = 0;
		KillTimer(1);	//停止计时器
	}
	else {	//选上了
		sta = MF_CHECKED;
		SetTimer(1, 10000, NULL);	//10秒定时器。每十秒自动批准一次
	}
	CheckMenuItem(mnu->m_hMenu, ID_mnuAutoApproval, sta);
}


void Admin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MySQLHostVariable host;
	int sta;	//状态标志
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		mysql_query(&host.mysql, "Update `operator` SET `Right`='1' Where `right`='0';");//批准所有学生申请
		mysql_query(&host.mysql, "Update `operator` SET `Right`='3' Where `right`='2';");//批准所有教师申请
		CloseMySQL(&host);	//关闭MySQL连接	
	}
	CDialogEx::OnTimer(nIDEvent);
}


//void Admin::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	static int ClickTime = 0;			//防止执行后ClickTime没了
//	if (pNMItemActivate->iItem == -1)	//未点击到操作栏则直接忽略
//		return;	
//	ChMode(AWinInf.WorkMode, ClickTime);//须保持点击时的工作状态
//	ClickTime = 1 - ClickTime;		//保证ClickTime在01之间循环
//	*pResult = 0;
//}


void Admin::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnCancel();
}


void Admin::Onmnuteacher()
{
	// TODO: 在此添加命令处理程序代码
	TeacherManage();
}


void Admin::OnBnClickedCmdapprovaltask()
{
	// TODO: 在此添加控件通知处理程序代码
	char cmd[200];
	POSITION pos;
	int i, j;
	//---------获得选择人数---------
	pos = ListCtrl.GetFirstSelectedItemPosition();
	for (i = 0; ListCtrl.GetNextSelectedItem(pos) >= 0; i++);
	if (i < 1) {
		MessageBoxA("请先选择分工", "提示");
		return;
	}
	sprintf_s(cmd, sizeof(cmd), "确认要批准%d条教学任务？", i);
	i = MessageBoxA(cmd, "确认", MB_YESNO);
	if (i != IDYES)
		return;	//取消
	//---------连接至数据库---------
	MySQLHostVariable host;
	int sta;	//状态标志
	char ID[72];
	sta = InitMySQL(&host);//连接MySQL数据库
	if (sta == TRUE) {
		pos = ListCtrl.GetFirstSelectedItemPosition();
		for (j = 0; (i = ListCtrl.GetNextSelectedItem(pos)) >= 0;) {
			//---------上传至数据库---------
			ListCtrl.GetItemText(i, 0, ID, sizeof(ID));
			sprintf_s(cmd, sizeof(cmd), "Update `Manage` Set `Right`='1' Where `ID`='%s';", ID);
			mysql_query(&host.mysql, cmd);
			j = j + (int)host.mysql.affected_rows;
		}
		if (j > 0)
			sprintf_s(cmd, sizeof(cmd), "已批准%d条教学任务", j);
		else
			strcpy_s(cmd, sizeof(cmd), "无需审批的教学任务");
		MessageBoxA(cmd, "提示");
		CloseMySQL(&host);	//关闭MySQL连接	
	}
	ChMode(WMManage, 0, 0);
}


void Admin::OnBnClickedCmdsave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Fn;
	CString Fndef;
	//---------自动设置文件名---------
	if (AWinInf.WorkMode == WMOper)
		Fndef = "学生名单.xls";
	else if (AWinInf.WorkMode == WMQust)
		Fndef = "题库.xls";
	else if (AWinInf.WorkMode == WMManage)
		Fndef = "教学分工.xls";
	else {
		MessageBoxA("无效命令");
		return;
	}
	//---------获得文件路径---------
	CFileDialog dlg(FALSE, NULL, Fndef, OFN_HIDEREADONLY, "Excel文件(*.XLS)|*.XLS|");//选择导出文本文件
	if (dlg.DoModal() == IDOK)
		Fn = dlg.GetPathName();
	else
		return;
	if (WriteListCtrl(&ListCtrl, Fn.GetBuffer()) == TRUE)
		MessageBoxA("文件导出成功");
	else  //文件打开失败
		MessageBoxA("导出失败，请关闭目标文件后再试");
}


int WriteListCtrl(CListCtrl *ListCtrl, char * Fn) {//表格数据写文本文件
	int x, y;
	int i, j;
	char buf[72];
	CHeaderCtrl*   pHeaderCtrl;
	HDITEM Item;
	FILE *fp = NULL;
	//---------打开文件---------
	if (fopen_s(&fp, Fn, "wt") != 0)
		return FALSE;
	pHeaderCtrl = ListCtrl->GetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return FALSE;
	x = pHeaderCtrl->GetItemCount();	//计数
	//---------写文件---------
	for (i = 0; i < x; i++) {
		memset(&Item, 0, sizeof(Item));
		Item.mask = HDI_TEXT;
		Item.pszText = buf;
		Item.cchTextMax = 512;
		pHeaderCtrl->GetItem(i, &Item);
		if (i == 0)
			fprintf(fp, "\"%s\"", buf);
		else
			fprintf(fp, "\t\"%s\"", buf);	//表格可以用制表符来分隔
	}
	fprintf(fp, "\n", buf);	//换行
	//---------写文件---------
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
	fclose(fp);		//关闭文件
	return TRUE;
}


void Admin::OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	if (AWinInf.WorkMode == WMOper && phdr->iItem != 3) {
		MessageBoxA("人员管理下只能按权限排序。");
		return;		//人员员管理下只能按权限排序
	}
	if (AWinInf.WorkMode == WMQust) {
		MessageBoxA("课程管理下排序无效。");
		return;		//题库管理下无意义
	}
	//教学任务管理下排序均有意义
	static int ClickTime = 0;		//防止执行后ClickTime没了
	ChMode(AWinInf.WorkMode, phdr->iItem, ClickTime);//须保持点击时的工作状态
	ClickTime = 1 - ClickTime;		//保证ClickTime在01之间循环
	*pResult = 0;
}


void Admin::OnBnClickedTestgen()
{
	TestGenor t;
	t.CourseID = AWinInf.Course;	//需根据选择的位置生成题目
	t.DoModal();
	ChMode(AWinInf.WorkMode, 0, 0);	//生成后刷新页面
}
