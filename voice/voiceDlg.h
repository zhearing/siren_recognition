
// voiceDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "MyDlg1.h"
#include "MyDlg2.h"


// CvoiceDlg �Ի���
class CvoiceDlg : public CDialogEx
{
	// ����
public:
	CvoiceDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_VOICE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	MyDlg1 mdg1;
	MyDlg2 mdg2;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
