#pragma once
#include "afxwin.h"


// Approval 对话框

class Approval : public CDialogEx
{
	DECLARE_DYNAMIC(Approval)

public:
	Approval(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Approval();

// 对话框数据
	enum { IDD = IDD_Approval_DIALOG };

protected:
	HICON m_hIcon;
	int ID;	//待审批用户ID
	int right;//待审批用户权限
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	int Init(void);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit cEditNote;
	afx_msg void OnBnClickedCmdok();
	afx_msg void OnBnClickedCmdenable();
	afx_msg void OnBnClickedCmddisable();
};
