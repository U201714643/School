
// SchoolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CSchoolDlg �Ի���
class CSchoolDlg : public CDialogEx
{
// ����
public:
	CSchoolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SCHOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
