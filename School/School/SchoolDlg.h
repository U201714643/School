
// SchoolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CSchoolDlg 对话框
class CSchoolDlg : public CDialogEx
{
// 构造
public:
	CSchoolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCHOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton ChkNewPwd;
	CEdit TxtPwd2;
	CEdit TxtPwd;
	CEdit TxtPwd1;
	CEdit TxtName;
	CButton CmdLogin;
	CButton CmdModify;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCmdquit();
	afx_msg void OnBnClickedCmdlogin();
	afx_msg void OnBnClickedCmdnewuser();
	CStatic Check4Wrod;
	afx_msg void OnClose();
};
