#pragma once
#include "afxwin.h"


// Approval �Ի���

class Approval : public CDialogEx
{
	DECLARE_DYNAMIC(Approval)

public:
	Approval(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Approval();

// �Ի�������
	enum { IDD = IDD_Approval_DIALOG };

protected:
	HICON m_hIcon;
	int ID;	//�������û�ID
	int right;//�������û�Ȩ��
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
