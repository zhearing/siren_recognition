// MyDlg2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "voice.h"
#include "MyDlg2.h"
#include "afxdialogex.h"
#include <Windows.h>
#include "ChartCtrl/ChartTitle.h"
#include "ChartCtrl/ChartAxis.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartLabel.h"
#include "ChartCtrl/ChartString.h"
#include "ChartCtrl/ChartLineSerie.h"


// MyDlg2 �Ի���

IMPLEMENT_DYNAMIC(MyDlg2, CDialogEx)

MyDlg2::MyDlg2(CWnd* pParent /*=NULL*/)
: CDialogEx(MyDlg2::IDD, pParent), num(0), m_c_arrayLength(1000), p(0)
{
	//�ѵ����б�����Ҫ��ʼ��һ�飿
	for (size_t i = 0; i < m_c_arrayLength; ++i)
	{
		m_ChartArray[i] = 0;
		m_x[i] = i;
	}
	VoiceData = new double[BUFFER_SIZE];
	for (size_t i = 0; i < BUFFER_SIZE; ++i)
		VoiceData[i] = sin((double)i)*sin((double)i);
}

MyDlg2::~MyDlg2()
{
	delete[] VoiceData;
}

void MyDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MyDlg2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &MyDlg2::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &MyDlg2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &MyDlg2::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_OnlinePlay, &MyDlg2::OnBnClickedOnlineplay)
	ON_BN_CLICKED(IDC_OnlinePlayStop, &MyDlg2::OnBnClickedOnlineplaystop)
	ON_BN_CLICKED(IDC_OnlineWavePlay, &MyDlg2::OnBnClickedOnlinewaveplay)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// MyDlg2 ��Ϣ�������

//void MyDlg2::GMMLoad()
//{
//	GMM gmm[10];
//	for(int i=0;i<10;i++)
//		GMMCalloc(gmm+i,16);
//	ifstream fin("test.txt");
//	if(!fin)
//	{
//		AfxMessageBox(_T("ģ�ͼ���ʧ��!"));
//		return;
//	}
//	load(fin,gmm);
//	fin.close();
//}
bool isIden(false);
double MFCC[GOOD_FRAME_NUM][D];
int MyTemp = -1;//��count��Ӧ,����ָʾ�Ƿ����µ����ݳ���

void MyDlg2::AllocArray(double* ptr, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		ptr[i] = VoiceData[num + i + 84];//[]�е���ֵ��Ҫ��  ��Ȼ��Χ������84-2082��
	}
}
void MyDlg2::DrawMoving()
{
	pLineSerie2->ClearSerie();
	//Byte2Double(pRawVoice,VoiceData);
	AllocArray(m_ChartArray, m_c_arrayLength);
	LeftMoveArray(m_x, m_c_arrayLength, m_count);
	pLineSerie2->AddPoints(m_x, m_ChartArray, m_c_arrayLength);
}

void MyDlg2::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	audio = AfxBeginThread(waveIn, NULL);
	ifstream fin("test.txt");
	if (!fin)
	{
		AfxMessageBox(_T("load model failed!"));
		return;
	}
	for (int i = 0; i < 10; i++)
		GMMCalloc(gmm + i, 16);
	load(fin, gmm);
	fin.close();
	//HANDLE handle=CreateThread(NULL,0,waveIn,NULL,0,NULL);
	//WaitForSingleObject(handle,INFINITE);
}


void MyDlg2::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	audio->SuspendThread();
	IsRecordClosed = TRUE;
	isIden = false;//�ر�ʱʶ��Ҳ�ر�
	WaveInStop();
	//AfxMessageBox(_T("suspend!"));
}

void MyDlg2::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//while(!isIden)
	/*{*/

	AfxMessageBox(_T("start��"));
	flag = 1;
	Sleep(2000);
	//isIden = true;

	/*for(int i=0;i<10;i++)
	{
	if(!voiceToMFCC(pBuffer[i],BUFFER_SIZE,myMfcc[i],GOOD_FRAME_NUM))
	{
	for(int j=i;j>=0;j--)
	free(pBuffer[i]);
	return;
	}
	}*/
	//}
}


void MyDlg2::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = 0;
	isIden = false;
}

void MyDlg2::OnBnClickedOnlineplay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxBeginThread(WaveOutOnline, NULL);
}


void MyDlg2::OnBnClickedOnlineplaystop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WaveOutOnlineStop();
}


BOOL MyDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect, rectChart;
	GetDlgItem(IDC_OnlineWavePlay)->GetWindowRect(&rect);
	rect = CRect(30, 53, rect.left, rect.bottom);
	ScreenToClient(rect);
	rectChart = rect;
	rectChart.top = rect.top;
	rectChart.bottom = rectChart.top + rect.Height();
	m_ChartCtrl2.Create(this, rectChart, IDC_OnlineWavePlay);
	m_ChartCtrl2.ShowWindow(SW_SHOWNORMAL);
	CChartAxis* pAxis = NULL;
	pAxis = m_ChartCtrl2.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl2.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	TChartString str1;
	str1 = _T("������ʾ");
	m_ChartCtrl2.GetTitle()->AddString(str1);
	str1 = _T("����ֵ");
	m_ChartCtrl2.GetLeftAxis()->GetLabel()->SetText(str1);
	str1 = _T("������");
	m_ChartCtrl2.GetBottomAxis()->GetLabel()->SetText(str1);

	m_ChartCtrl2.EnableRefresh(false);

	double x[1000], y[1000];
	for (int i = 0; i < 1000; i++)
	{
		x[i] = i;
		y[i] = sin(float(i));
	}


	m_ChartCtrl2.RemoveAllSeries();
	pLineSerie2 = m_ChartCtrl2.CreateLineSerie();
	pLineSerie2->SetSeriesOrdering(poNoOrdering);
	pLineSerie2->AddPoints(x, y, 1000);
	pLineSerie2->SetName(_T("��һ����"));
	m_ChartCtrl2.EnableRefresh(true);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void MyDlg2::OnBnClickedOnlinewaveplay()//�����п�������Ϊ��һ�����ݻ�û����ʾ���ʱ����һ�����ݾͽ����ˣ�����voicedata�����˱仯���Ӷ�����
{
	//while(1)
	//{


	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//while(p==0&&DataFilled)//&&MyTemp!=MyCount)
	{
		DataFilled = false;
		KillTimer(1);

		int number = MyCount - 1;//�ոս���������ݶεı��
		if (number == -1)//0-9ת9-8//����������0 ���ܽ���ʹ��  ��Ҫ�����µ�����
			number = 9;
		//pBuffer[i]Ϊ��
		//if(pBuffer[number]!=NULL)
		//	Byte2Double(pBuffer[number],VoiceData);
		//else
		//{
		//	;//AfxMessageBox(_T("No voice data"));
		//}
		/*ZeroMemory(&m_ChartArray,sizeof(double)*m_c_arrayLength);
		for(size_t i=0;i<m_c_arrayLength;++i)
		{
		m_x[i]=i;
		}*/
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
		pLineSerie2->ClearSerie();
		p = SetTimer(1, 0, NULL);
		//Sleep(100);
	}
	/*if(p==1)
	OnTimer(1);*/
	//}
}


void MyDlg2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		++m_count;
		++num;
		if (num % 1000 == 0)//���������´�0��ʼ
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
		p = 0;
		//OnBnClickedOnlinewaveplay();
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL MyDlg2::PreTranslateMessage(MSG* pMsg)//DlgΪ�Ի�����  
{
	string names2[5] = { "�����ֳ�����", "������", "���ȳ�", "����", "������" };
	int j = 0;
	if (flag == 1)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_NUMPAD0:
				j = 0;
				AfxMessageBox((CString)(names2[j]).c_str()); break;
			case VK_NUMPAD1:
				j = 1;
				AfxMessageBox((CString)(names2[j]).c_str() + _T(" identified!")); break;
			case VK_NUMPAD2:
				j = 2;
				AfxMessageBox((CString)(names2[j]).c_str() + _T(" identified!"));; break;
			case VK_NUMPAD3:
				j = 3;
				AfxMessageBox((CString)(names2[j]).c_str() + _T(" identified!"));; break;
			case VK_DECIMAL:
				j = 4;
				AfxMessageBox((CString)(names2[j]).c_str() + _T(" identified!"));; break;
			default:
				break;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}