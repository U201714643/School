#pragma once
#include "afxwin.h"


// Combo �Ի���

class Combo : public CDialogEx
{
	DECLARE_DYNAMIC(Combo)

public:
	Combo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Combo();

// �Ի�������
	enum { IDD = IDD_Combo_DIALOG };
	HICON m_hIcon;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox ComboBox;
	int SetRet(ComobRet * lpret);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCmdok2();
	afx_msg void OnBnClickedCmdexit();
	afx_msg void OnBnClickedCmdok();
	afx_msg void OnCbnSelchangeCmb();
	CButton CmdOk;
};
