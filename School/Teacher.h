#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// Teacher �Ի���

class Teacher : public CDialogEx
{
	DECLARE_DYNAMIC(Teacher)

public:
	Teacher(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Teacher();

// �Ի�������
	enum { IDD = IDD_Teacher_DIALOG };
	HICON m_hIcon;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int ChMode(int mode);	//��ͳ�Ʊ�
	int ChManageMode(void);	//�ֹܰ༶
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCmdok();
	CTreeCtrl cTree;
	CTabCtrl cTabMod;
	CListCtrl ListCtrl;
	CButton cCmdManage;
	afx_msg void OnTcnSelchangeTabmod(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCmdsave();
	afx_msg void OnClose();
	afx_msg void OnmnuExit();
	afx_msg void OnmnuNewPwd();
	afx_msg void OnmnuExport();
	afx_msg void OnBnClickedCmdmanage();
};
