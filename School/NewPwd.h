#pragma once
#include "afxwin.h"


// NewPwd �Ի���

class NewPwd : public CDialogEx
{
	DECLARE_DYNAMIC(NewPwd)

public:
	NewPwd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NewPwd();

// �Ի�������
	enum { IDD = IDD_Password_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
