#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// Students 对话框

class Students : public CDialogEx
{
	DECLARE_DYNAMIC(Students)

public:
	Students(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Students();

// 对话框数据
	enum { IDD = IDD_Students_DIALOG };
	HICON m_hIcon;
private:
	int InitCmbCourse(void);	//初始化课程下拉框
	int	ShowMode(void);		//切换显示模式

	CStatic Question[30],Flag[30];
	CEdit	Answer[30];
	CEdit	Answer2[30];
	CButton Prompt[30];
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);	//接受按键信息
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void CreateCtrl(void);	////设定控件位置
	BOOL Init();	//初始化
	int  WriteExam();	//测试结果写MySQL数据库
	int  StatsStu(int CmdNum, int ClickTime);	//统计测验成绩
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox CmbCourse;
	afx_msg void OnCbnSelchangeCmbcourse();
	CStatic TxtNote;
	CButton CmdTry;
	CButton CmdSubmit;
	CButton CmdQuit;
	afx_msg void OnBnClickedCmdsubmit();
	afx_msg void OnBnClickedCmdquit();
	afx_msg void OnBnClickedCmdtry();
	afx_msg void OnMnuExitStu();
	afx_msg void OnMnuExamTestStu();
	afx_msg void OnmnuExamTryStu();
	afx_msg void OnMnuStatsStu();
	CListCtrl cList;
	CEdit cTxtStats;
	CStatic cTxtCourse;
	afx_msg void OnUpdateMnuStatsStu(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMnuExamStu(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnmnuStatsTryStu();
	afx_msg void OnmnuStatsTestStu();
	afx_msg void OnBnClickedCmdreview();
	afx_msg void OnmnuNewPwd();
	CButton CmdReview;
	afx_msg void OnClose();
	afx_msg void OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCmdstarttest();
	CButton CmdStartTest;
	afx_msg void OnmnuExamRetry();
	CStatic TxtNote1;
	CStatic TxtNote2;
	afx_msg void OnmnuStatsRetryStu();
};
