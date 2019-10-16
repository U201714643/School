#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// admin 对话框

class Admin : public CDialogEx
{
	DECLARE_DYNAMIC(Admin)

public:
	Admin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Admin();

// 对话框数据
	enum { IDD = IDD_admin_DIALOG };
	HICON m_hIcon;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnExit();
	afx_msg void OnBatchQuestions();
	CTreeCtrl cTree;
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	int ChMode(int Mode, int CmdNum, int CilckTime);	//改变工作模式
	int TeacherManage();	//教师分工管理模式
	CStatic TxtNote;
	CButton CmdClrPwd;
	afx_msg void OnBnClickedCmdok();
	CListCtrl ListCtrl;
	afx_msg void OnBnClickedCmdbatch();
	afx_msg void OnNMKillfocusTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCmdclrpwd();
	afx_msg void OnBnClickedCmddel();
	afx_msg void OnmnuNewPwd();
	afx_msg void OnApprovalUser();
	afx_msg void OnmnuAutoApproval();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnmnuManage();
	afx_msg void Onmnuteacher();
	CButton CmdApprovalTask;
	CButton CmdDel;
	afx_msg void OnBnClickedCmdapprovaltask();
	CButton CmdBatch;
	afx_msg void OnBnClickedCmdsave();
	CButton CmdSave;
	afx_msg void OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult);
	CButton TestGenorArg;
	afx_msg void OnBnClickedTestgen();
};
