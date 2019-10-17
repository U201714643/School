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
	CStatic InstructorTxt;
	CEdit InsTxt;
};


#define  ModNone  0	 //模式-无
#define  ModStats 1	 //模式-统计
#define  ModExamA 2	 //模式-测验前
#define  ModExamB 3	 //模式-测验中
#define  ModExamC 4	 //模式-测验后
#define  ModReview 5 //模式-回顾

#define  ExamTest   0	 //答题方式-考试
#define  ExamTry    1	 //答题方式-刷题
#define  ExamRetry  2	 //答题方式-错题本

#define  RankNum	0	 //排序方式-时间
#define  RankTime	1	 //排序方式-用时
#define  RankCourse	2	 //排序方式-课程
#define  RankOk		3	 //排序方式-正确数
#define  RankError	4	 //排序方式-错误数
#define  RankScore	5	 //排序方式-正确率


typedef struct studentslocal {	//student窗体共用变量
	int   WorkMode;	//ModNone-无,ModStats-统计,ModExamA-测验前,ModExamB-测验中,ModExamC-测验后,ModReview-回顾
	DWORD ExamStart;//测验开始时间(ms)
	int   RemainingSec;//测验剩余时间(S)
	long  ExamID;//测验记录ID
	int   ExamMs;//测验用时
	int   ExamMode;//答题方式
	int   CourseID;	//课程ID
	int   OpertorID;//学员ID
	int   Correct;//正解题目数
	int   Error;  //错题数
	double	Score;//得分
	question ques[30];	//当前测试题
}studentslocal;

char * RecCourse(char * resource);			//推荐的学习课程
static int RndExchQues(int size, int tim);	//随机交换试题次序
static int ReadOrSkip(int now, int max, int need, int needmax, int mode);//决定是否读取当前记录