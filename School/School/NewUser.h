#pragma once
#include "afxwin.h"


// NewUser �Ի���

class NewUser : public CDialogEx
{
	DECLARE_DYNAMIC(NewUser)

public:
	NewUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NewUser();

// �Ի�������
	enum { IDD = IDD_NewUser_DIALOG };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCmdquit();
	CComboBox cCmbGrade;
	afx_msg void OnBnClickedCmdok();
	CEdit cTxtNo;
	CEdit cTxtName;
	CEdit cTxtPwd1;
	CEdit cTxtPwd2;
	CComboBox cCmbType;
};
