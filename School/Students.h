#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// Students �Ի���

class Students : public CDialogEx
{
	DECLARE_DYNAMIC(Students)

public:
	Students(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Students();

// �Ի�������
	enum { IDD = IDD_Students_DIALOG };
	HICON m_hIcon;
private:
	int InitCmbCourse(void);	//��ʼ���γ�������
	int	ShowMode(void);		//�л���ʾģʽ

	CStatic Question[30],Flag[30];
	CEdit	Answer[30];
	CEdit	Answer2[30];
	CButton Prompt[30];
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);	//���ܰ�����Ϣ
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void CreateCtrl(void);	////�趨�ؼ�λ��
	BOOL Init();	//��ʼ��
	int  WriteExam();	//���Խ��дMySQL���ݿ�
	int  StatsStu(int CmdNum, int ClickTime);	//ͳ�Ʋ���ɼ�
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


#define  ModNone  0	 //ģʽ-��
#define  ModStats 1	 //ģʽ-ͳ��
#define  ModExamA 2	 //ģʽ-����ǰ
#define  ModExamB 3	 //ģʽ-������
#define  ModExamC 4	 //ģʽ-�����
#define  ModReview 5 //ģʽ-�ع�

#define  ExamTest   0	 //���ⷽʽ-����
#define  ExamTry    1	 //���ⷽʽ-ˢ��
#define  ExamRetry  2	 //���ⷽʽ-���Ȿ

#define  RankNum	0	 //����ʽ-ʱ��
#define  RankTime	1	 //����ʽ-��ʱ
#define  RankCourse	2	 //����ʽ-�γ�
#define  RankOk		3	 //����ʽ-��ȷ��
#define  RankError	4	 //����ʽ-������
#define  RankScore	5	 //����ʽ-��ȷ��


typedef struct studentslocal {	//student���干�ñ���
	int   WorkMode;	//ModNone-��,ModStats-ͳ��,ModExamA-����ǰ,ModExamB-������,ModExamC-�����,ModReview-�ع�
	DWORD ExamStart;//���鿪ʼʱ��(ms)
	int   RemainingSec;//����ʣ��ʱ��(S)
	long  ExamID;//�����¼ID
	int   ExamMs;//������ʱ
	int   ExamMode;//���ⷽʽ
	int   CourseID;	//�γ�ID
	int   OpertorID;//ѧԱID
	int   Correct;//������Ŀ��
	int   Error;  //������
	double	Score;//�÷�
	question ques[30];	//��ǰ������
}studentslocal;

char * RecCourse(char * resource);			//�Ƽ���ѧϰ�γ�
static int RndExchQues(int size, int tim);	//��������������
static int ReadOrSkip(int now, int max, int need, int needmax, int mode);//�����Ƿ��ȡ��ǰ��¼