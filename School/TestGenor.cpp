// TestGenor.cpp: 实现文件
//

#include "stdafx.h"
#include "School.h"
#include "Support.h"
#include "TestGenor.h"
#include "mylib.h"
#include "afxdialogex.h"

int IsOp(CButton * Edit);	//是否存在此类操作符
int UpLoadQuestion(int CourseID ,ExpressionNode * ExpAddr);	//上传题目

// TestGenor 对话框

IMPLEMENT_DYNAMIC(TestGenor, CDialogEx)

TestGenor::TestGenor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NewTest, pParent)
{

}

TestGenor::~TestGenor()
{
}

void TestGenor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Count);
	DDX_Control(pDX, IDC_EDIT2, NumMin);
	DDX_Control(pDX, IDC_EDIT3, NumMax);
	DDX_Control(pDX, IDC_EDIT4, Opcount);
	DDX_Control(pDX, IDC_EDIT5, ResultMin);
	DDX_Control(pDX, IDC_EDIT6, ResultMax);
	DDX_Control(pDX, IDC_CHECK1, IsKuoHao);
	DDX_Control(pDX, IDC_CHECK2, IsJia);
	DDX_Control(pDX, IDC_CHECK3, IsJian);
	DDX_Control(pDX, IDC_CHECK4, IsCheng);
	DDX_Control(pDX, IDC_CHECK5, IsChu);
}


BEGIN_MESSAGE_MAP(TestGenor, CDialogEx)
	ON_BN_CLICKED(IDOK, &TestGenor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &TestGenor::OnBnClickedCancel)
END_MESSAGE_MAP()


// TestGenor 消息处理程序


void TestGenor::OnBnClickedOk()
{
	ExpressionList Arguement;	//参数表
	ExpressionNode EXP[EXPNUM] = { 0 };	//存放生成结果
	Arguement.Repeat = FLASE;	//不允许重复
	Arguement.EqualsNum = IntFormEidt(&Count);	//生成的表达式数目
	Arguement.NumMax = IntFormEidt(&NumMax);	//操作数范围
	Arguement.NumMin = IntFormEidt(&NumMin);	//操作数范围
	Arguement.OpCount = IntFormEidt(&Opcount);	//操作符数目
	Arguement.ResultMax = IntFormEidt(&ResultMax);	//结果范围
	Arguement.ResultMin = IntFormEidt(&ResultMin);	//结果范围
	Arguement.KuoHao = IsOp(&IsKuoHao);	//有无括号
	//------确定操作符种类------
	int OpCheck[4];		//操作符种类校验
	OpCheck[0]= IsOp(&IsJia);	//有无加号
	OpCheck[1] = IsOp(&IsJian);	//有无减号
	OpCheck[2] = IsOp(&IsCheng);//有无乘号
	OpCheck[3] = IsOp(&IsChu);	//有无除号
	//------确定操作符代码------
	for (int i = 0; i < 4; i++) {
		if (OpCheck[i]) {	//正序找最小
			Arguement.OpTypeMin = i;
			break;
		}
	}
	for (int i = 3; i >= 0; i--) {
		if (OpCheck[i])	{	//反序找最大
			Arguement.OpTypeMax = i;
			break;
		}
	}
	if (OpCheck[0] + OpCheck[1] + OpCheck[2] + OpCheck[3] == 0) {	//这代表一个都没选
		MessageBoxA("请选择操作符。");
		return;
	}
	//------检验操作和理性------
	if (ArgumentCheck(&Arguement) == ERROR)
		return;	//错误提示出现在函数中
	//------开始生成------
	if (TestGen(EXP, &Arguement) == OK) {
		UpLoadQuestion(this->CourseID, EXP);	//上传
		MessageBoxA("表达式生成成功。");
	}
	else {
		UpLoadQuestion(this->CourseID, EXP);	//上传
		MessageBoxA("表达式生成超时。\r\n只生成了数量较少的表达式。");
	}
	//------关闭窗口------
	CDialogEx::OnCancel();	
}


void TestGenor::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

int IsOp(CButton * Edit){	//是否存在此类操作符
	if (Edit->GetCheck() == true)
		return TRUE;
	else return FLASE;
}

int UpLoadQuestion(int CourseID, ExpressionNode * ExpAddr) {	//上传题目
	MySQLHostVariable host;
	FILE *fp = NULL;
	char cmd[256];
	char buf[EXPLEN];
	//------连接MySQL数据库------
	int sta = InitMySQL(&host);		
	if (sta != TRUE)
		return ERROR;
	//------逐条上传------
	for (int i = 0; ExpAddr[i].Expression[0]; i++) {	//表达式不为空字符串即继续
		sprintf_s(buf, sizeof(buf), "%d", ExpAddr[i].Value);	//答案以字符串形式保存
		sprintf_s(cmd, sizeof(cmd), "Insert Into `Questions` (`Course`,`Text`,`Answer`) "
			" Values('%d','%s','%s');", CourseID, ExpAddr[i].Expression, buf);
		mysql_query(&host.mysql, cmd);
	}
	//------关闭MySQL连接------
	CloseMySQL(&host);		
	return OK;
}

int TestGenor::ArgumentCheck(ExpressionList * Argument) {	//检验操作合理性
	//------各参数均需填写（即均不为负）------	
	if (Argument->EqualsNum < 0) {
		MessageBoxA("请填写需要生成的表达式数目。");
		return ERROR;
	}
	if (Argument->NumMin < 0) {
		MessageBoxA("请填写操作数最小值。");
		return ERROR;
	}
	if (Argument->NumMax < 0) {
		MessageBoxA("请填写操作数最大值。");
		return ERROR;
	}	
	if (Argument->OpCount < 0) {
		MessageBoxA("请填写需要生成的表达式中运算符数目。");
		return ERROR;
	}
	if (Argument->ResultMin < 0) {
		MessageBoxA("请填写运算结果最小值。");
		return ERROR;
	}
	if (Argument->ResultMax < 0) {
		MessageBoxA("请填写运算结果最大值。");
		return ERROR;
	}
	//------一次性不能生成太多算式------
	if (Argument->EqualsNum >= EXPNUM) {
		MessageBoxA("生成的表达式数目过多。");
		return ERROR;
	}
	//------最小值不能超过最大值------
	if (Argument->NumMax < Argument->NumMin) {
		MessageBoxA("操作数最大值不能小于最小值。");
		return ERROR;
	}
	if (Argument->ResultMax < Argument->ResultMin) {
		MessageBoxA("结果最大值不能小于最小值。");
		return ERROR;
	}
	//------小学1~3年级处理不超过4位数------
	if (Argument->NumMax >= 10000 || Argument->ResultMax >= 10000) {
		MessageBoxA("小学1~3年级处理不超过4位数。");
		return ERROR;
	}
	//------运算符不超过4个------
	if (Argument->OpCount > 4) {
		MessageBoxA("小学1~3年级处理不超过4个运算符。");
		return ERROR;
	}
	//------运算符至少有一个------
	if (Argument->OpCount == 0) {
		MessageBoxA("运算符至少有一个。");
		return ERROR;
	}
	//------运算符只有一个时不能含有括号------
	if (Argument->OpCount == 1 && Argument->KuoHao == TRUE) {
		MessageBoxA("运算符只有一个时表达式中不能含有括号。");
		return ERROR;
	}
	return OK;
}