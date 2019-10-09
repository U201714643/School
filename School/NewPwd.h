#pragma once
#include "afxwin.h"


// NewPwd 对话框

class NewPwd : public CDialogEx
{
	DECLARE_DYNAMIC(NewPwd)

public:
	NewPwd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NewPwd();

// 对话框数据
	enum { IDD = IDD_Password_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCmdexit();
	afx_msg void OnBnClickedCmdok();
	CEdit TxtOldPwd;
	CEdit TxtNewPwd1;
	CEdit TxtNewPwd2;
};
