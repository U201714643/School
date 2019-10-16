#pragma once
#include "afxwin.h"
#include "mylib.h"

// TestGenor 对话框

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
	int CourseID;	//当前课程ID
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
