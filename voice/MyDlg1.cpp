// MyDlg1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "voice.h"
#include "MyDlg1.h"
#include "afxdialogex.h"
#include "ChartCtrl/ChartTitle.h"
#include "ChartCtrl/ChartAxis.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartLabel.h"
#include "ChartCtrl/ChartString.h"
#include "ChartCtrl/ChartLineSerie.h"


// MyDlg1 �Ի���

IMPLEMENT_DYNAMIC(MyDlg1, CDialogEx)

MyDlg1::MyDlg1(CWnd* pParent /*=NULL*/)
: CDialogEx(MyDlg1::IDD, pParent), m_c_arrayLength(2000), num(0)
{
	VoiceData = new double[BUFFER_SIZE];
	for (int i = 0; i < BUFFER_SIZE; ++i)
		VoiceData[i] = (double)i / 1000000;
}

MyDlg1::~MyDlg1()
{
	delete[] VoiceData;
}

void MyDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WavePlay, m_ChartCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
}


BEGIN_MESSAGE_MAP(MyDlg1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MyDlg1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_PLAY, &MyDlg1::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_STOPPLAY, &MyDlg1::OnBnClickedStopplay)
	ON_BN_CLICKED(IDC_ClickedButtonRunning, &MyDlg1::OnBnClickedClickedbuttonrunning)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &MyDlg1::OnBnClickedButton3)
END_MESSAGE_MAP()


// MyDlg1 ��Ϣ�������

mBYTE* pRawVoice = NULL;
void MyDlg1::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("WavFiles(*.wav)|*.wav|AllFiles(*.*)|*.*||"));
	opened = false;
	if (fileDlg.DoModal() == IDOK)
	{
		CString pathName = fileDlg.GetPathName();
		//SetDlgItemText(IDC_EDIT1,pathName);

		//CString to char*(Unicode)
		int l = WideCharToMultiByte(CP_ACP, 0, pathName, -1, NULL, 0, NULL, NULL);
		char* dir = new char[l + 1];
		WideCharToMultiByte(CP_ACP, 0, pathName, -1, dir, l, NULL, NULL);

		FILE* fi = NULL;
		fopen_s(&fi, dir, "rb");
		pRawVoice = (mPBYTE)calloc(GOOD_FRAME_NUM, FRAME_LEN);
		dataLen = fread(pRawVoice, 1, GOOD_FRAME_NUM*FRAME_LEN, fi);
		delete dir;
		CString str;
		str.Format(_T("%d"), dataLen);
		//AfxMessageBox(str);
		if (dataLen == 0)
		{
			AfxMessageBox(pathName + _T("��ʧ��"));
			return;
		}
		opened = true;
		AfxMessageBox(pathName + _T("�򿪳ɹ�"));
		fclose(fi);
	}
}
string names[10] = { "������", "������", "�Ȼ���", "�Ȼ���", "����", "����", "����", "����", "�Ȼ���", "������" };

void MyDlg1::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (opened)
	{
		if (!voiceToMFCC(pRawVoice, GOOD_FRAME_NUM*FRAME_LEN, MFCC, GOOD_FRAME_NUM))
		{
			//(pRawVoice);
			return;
		}
		GMM gmm[10] = {};//����ʼ���Ļ�  �ᵼ��ctrl+F5����ʱ����
		//if(!mark)
		for (int i = 0; i < 10; i++)
			GMMCalloc(gmm + i, 16);
		ifstream fin("test.txt");
		if (!fin)
		{
			AfxMessageBox(_T("ģ�ͼ���ʧ��!"));
			return;
		}
		load(fin, gmm);
		fin.close();
		double maxI = 0.0;
		int j = 0;
		double value = 0.0;
		for (int i = 0; i < 10; i++)
		{
			int res = GMM_identify(MFCC, &value, &gmm[i], GOOD_FRAME_NUM, 16);
			if (value > maxI)
			{
				maxI = value;
				j = i;
			}
		}
		//CString id;
		//id.Format(_T("%d"),j);
		int temp = AfxMessageBox((CString)(names[j]).c_str() + _T(" identified!"));
		//int temp=AfxMessageBox(_T("GMM")+id+_T(" identified"));
		if (temp == IDOK)
			return;
		opened = false;
		//if(markmark) 
		for (int i = 0; i < 10; i++)
			FreeGMM(&gmm[i]);
	}
	return;
}


void MyDlg1::OnBnClickedPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxBeginThread(WaveOutOffline, NULL);
}


void MyDlg1::OnBnClickedStopplay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WaveOutOfflineStop();
}

void Byte2Double(mBYTE* data, double* DouData)
{
	for (int i = 0; i < BUFFER_SIZE; ++i)
	{
		DouData[i] = (double)(data[i] - 128)*0.0078125;
	}
}

BOOL MyDlg1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����������
	CChartAxis* pAxis = NULL;
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	TChartString str1;
	str1 = _T("������ʾ");
	m_ChartCtrl.GetTitle()->AddString(str1);
	str1 = _T("����ֵ");
	m_ChartCtrl.GetLeftAxis()->GetLabel()->SetText(str1);
	str1 = _T("������");
	m_ChartCtrl.GetBottomAxis()->GetLabel()->SetText(str1);

	m_ChartCtrl.EnableRefresh(false);

	double x[2000], y[2000];
	for (int i = 0; i < 2000; i++)
	{
		x[i] = i;
		y[i] = sin(float(i));
	}


	m_ChartCtrl.RemoveAllSeries();
	pLineSerie = m_ChartCtrl.CreateLineSerie();
	pLineSerie->SetSeriesOrdering(poNoOrdering);
	pLineSerie->AddPoints(x, y, 2000);
	pLineSerie->SetName(_T("��һ����"));
	m_ChartCtrl.EnableRefresh(true);
	//

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void MyDlg1::OnBnClickedClickedbuttonrunning()//ÿ��һ�Σ�������ͻص������״̬
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!isPlayClosed)//�򿪲���ʱĬ�ϲ�������
		AfxBeginThread(WaveOutOffline, NULL);
	KillTimer(0);

	if (pRawVoice != NULL)
		Byte2Double(pRawVoice, VoiceData);
	else
	{
		AfxMessageBox(_T("No voice data"));
		return;
	}
	ZeroMemory(&m_ChartArray, sizeof(double)*m_c_arrayLength);
	for (size_t i = 0; i < m_c_arrayLength; ++i)
	{
		m_x[i] = i;
	}
	m_count = m_c_arrayLength;
	pLineSerie->ClearSerie();
	SetTimer(0, 0, NULL);

}
void LeftMoveArray(double* ptr, size_t length, double data)
{
	for (size_t i = 1; i < length; ++i)
	{
		ptr[i - 1] = ptr[i];
	}
	ptr[length - 1] = data;
}
void MyDlg1::AllocArray(double* ptr, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		ptr[i] = VoiceData[num + i + 84];
	}
}
void MyDlg1::DrawMoving()
{
	pLineSerie->ClearSerie();
	//Byte2Double(pRawVoice,VoiceData);
	AllocArray(m_ChartArray, m_c_arrayLength);
	LeftMoveArray(m_x, m_c_arrayLength, m_count);
	pLineSerie->AddPoints(m_x, m_ChartArray, m_c_arrayLength);
}


void MyDlg1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (0 == nIDEvent)
	{
		++m_count;
		++num;
		if (num % 2000 == 0)//���������´�0��ʼ
		{
			for (size_t i = 0; i < m_c_arrayLength; ++i)
			{
				m_x[i] = i;
			}
			m_count = m_c_arrayLength;
		}
		if (num == BUFFER_SIZE)
			num = 0;
		DrawMoving();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void MyDlg1::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
