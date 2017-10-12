#pragma once
#include <fstream>
#include "FromTest\mfcc.h"
#include "FromTest\gmm.h"
#include "FromTest\cluster.h"
#include "FromTest\FFT2.h"
#include "FromTest\WaveIn.h"
#include "ChartCtrl\ChartCtrl.h"
#include "afxwin.h"

// MyDlg1 对话框
#define mPBYTE unsigned	char*
class MyDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(MyDlg1)

public:
	MyDlg1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyDlg1();

	// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	bool opened;
	//bool isCalloced;

	double MFCC[GOOD_FRAME_NUM][D];
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStopplay();
	CChartCtrl m_ChartCtrl;
	CChartLineSerie* pLineSerie;
	double m_ChartArray[2000];
	double m_x[2000];
	unsigned int m_count;
	const size_t m_c_arrayLength;
	unsigned int num;
	double* VoiceData;


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedClickedbuttonrunning();
	afx_msg void DrawMoving();
	afx_msg void AllocArray(double* ptr, size_t length);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_edit1;
	afx_msg void OnBnClickedButton3();
};


