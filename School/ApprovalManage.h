#pragma once


// ApprovalManage 对话框

class ApprovalManage : public CDialogEx
{
	DECLARE_DYNAMIC(ApprovalManage)

public:
	ApprovalManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ApprovalManage();

// 对话框数据
	enum { IDD = IDD_IDD_ApprovalManage_DIALOG_DIALOG2 };

protected:
	HICON m_hIcon;
	int	Init(void);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCmdok();
};
