#pragma once


// ApprovalManage �Ի���

class ApprovalManage : public CDialogEx
{
	DECLARE_DYNAMIC(ApprovalManage)

public:
	ApprovalManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ApprovalManage();

// �Ի�������
	enum { IDD = IDD_IDD_ApprovalManage_DIALOG_DIALOG2 };

protected:
	HICON m_hIcon;
	int	Init(void);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCmdok();
};
