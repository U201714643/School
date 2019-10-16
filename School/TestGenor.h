#pragma once
#include "afxwin.h"

// TestGenor 对话框


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


class TestGenor : public CDialogEx
{
	DECLARE_DYNAMIC(TestGenor)

public:
	TestGenor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TestGenor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NewTest };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int ArgumentCheck(ExpressionList * Argument);	//检验参数合理性
	CEdit Count;
	CEdit NumMin;
	CEdit NumMax;
	CEdit Opcount;
	CEdit ResultMin;
	CEdit ResultMax;
	CButton IsKuoHao;
	CButton IsJia;
	CButton IsJian;
	CButton IsCheng;
	CButton IsChu;
};
