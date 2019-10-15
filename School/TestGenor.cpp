// TestGenor.cpp: 实现文件
//

#include "stdafx.h"
#include "School.h"
#include "Support.h"
#include "TestGenor.h"
#include "afxdialogex.h"

typedef struct ExpressionList {	//生成表达式使用的参数表
	int EqualsNum;	//生成的表达式数目
	int OpCount;	//操作符数目
	int OpTypeMin;	//操作符种类范围
	int OpTypeMax;	//操作符种类范围
	int NumMin;		//操作数范围
	int NumMax;		//操作数范围
	int ResultMin;	//结果范围
	int ResultMax;	//结果范围
	int KuoHao;		//是否允许括号
	int Repeat;		//是否允许重复
};

int IsOp(CButton * Edit);	//是否存在此类操作符

#define FLASE 0

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
	Arguement.ResultMin++;
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