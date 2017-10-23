// MyDlg2.cpp : 实现文件
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


// MyDlg2 对话框

IMPLEMENT_DYNAMIC(MyDlg2, CDialogEx)

MyDlg2::MyDlg2(CWnd* pParent /*=NULL*/)
: CDialogEx(MyDlg2::IDD, pParent), num(0), m_c_arrayLength(1000), p(0)
{
	//难道所有变量都要初始化一遍？
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


// MyDlg2 消息处理程序

//void MyDlg2::GMMLoad()
//{
//	GMM gmm[10];
//	for(int i=0;i<10;i++)
//		GMMCalloc(gmm+i,16);
//	ifstream fin("test.txt");
//	if(!fin)
//	{
//		AfxMessageBox(_T("模型加载失败!"));
//		return;
//	}
//	load(fin,gmm);
//	fin.close();
//}
bool isIden(false);
double MFCC[GOOD_FRAME_NUM][D];
int MyTemp = -1;//与count对应,用来指示是否有新的数据出现

void MyDlg2::AllocArray(double* ptr, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		ptr[i] = VoiceData[num + i + 84];//[]中的数值需要改  不然范围限制在84-2082了
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	audio->SuspendThread();
	IsRecordClosed = TRUE;
	isIden = false;//关闭时识别也关闭
	WaveInStop();
	//AfxMessageBox(_T("suspend!"));
}

void MyDlg2::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//while(!isIden)
	/*{*/

	AfxMessageBox(_T("start！"));
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
	// TODO: 在此添加控件通知处理程序代码
	flag = 0;
	isIden = false;
}

void MyDlg2::OnBnClickedOnlineplay()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(WaveOutOnline, NULL);
}


void MyDlg2::OnBnClickedOnlineplaystop()
{
	// TODO: 在此添加控件通知处理程序代码
	WaveOutOnlineStop();
}


BOOL MyDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	str1 = _T("波形显示");
	m_ChartCtrl2.GetTitle()->AddString(str1);
	str1 = _T("采样值");
	m_ChartCtrl2.GetLeftAxis()->GetLabel()->SetText(str1);
	str1 = _T("采样点");
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
	pLineSerie2->SetName(_T("第一条线"));
	m_ChartCtrl2.EnableRefresh(true);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void MyDlg2::OnBnClickedOnlinewaveplay()//出错有可能是因为在一段数据还没有显示完的时候，下一段数据就进来了，导致voicedata发生了变化，从而崩溃
{
	//while(1)
	//{


	// TODO: 在此添加控件通知处理程序代码
	//while(p==0&&DataFilled)//&&MyTemp!=MyCount)
	{
		DataFilled = false;
		KillTimer(1);

		int number = MyCount - 1;//刚刚接收完的数据段的标号
		if (number == -1)//0-9转9-8//如果是最初的0 则不能进行使用  需要设置新的条件
			number = 9;
		//pBuffer[i]为空
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		++m_count;
		++num;
		if (num % 1000 == 0)//横坐标重新从0开始
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

BOOL MyDlg2::PreTranslateMessage(MSG* pMsg)//Dlg为对话框类  
{
	string names2[5] = { "非特种车辆！", "公交车", "急救车", "警车", "消防车" };
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