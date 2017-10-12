#pragma once

#include "..\FromTest\WaveIn.h"
#include "..\FromTest\mfcc.h"
#include "..\FromTest\gmm.h"
#include "..\FromTest\cluster.h"
#include "..\FromTest\FFT2.h"
#include "ChartCtrl\ChartCtrl.h"


// MyDlg2 �Ի���
class MyDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(MyDlg2)

public:
	MyDlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyDlg2();

	// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	CWinThread* audio;
	afx_msg void OnBnClickedButton2();
	//double myMfcc[10][GOOD_FRAME_NUM][D];
	afx_msg void OnBnClickedButton3();
	void GMMLoad();
	CChartCtrl m_ChartCtrl2;
	CChartLineSerie* pLineSerie2;
	double m_ChartArray[1000];
	double m_x[1000];
	unsigned int m_count;
	const size_t m_c_arrayLength;
	unsigned int num;
	double* VoiceData;
	UINT_PTR p;//������ʾsettimer����ֵ


	afx_msg void OnBnClickedOnlineplay();
	afx_msg void OnBnClickedOnlineplaystop();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOnlinewaveplay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void DrawMoving();
	afx_msg void AllocArray(double* ptr, size_t length);
};