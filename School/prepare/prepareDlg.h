
// prepareDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CprepareDlg �Ի���
class CprepareDlg : public CDialogEx
{
// ����
public:
	CprepareDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PREPARE_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCmdquit();
	afx_msg void OnBnClickedCmdstart();
	CButton cmdQuit;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCmdend();
	CStatic txtNote;
};
