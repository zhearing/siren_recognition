#include "stdafx.h"
#include "..\FromTest\WaveIn.h"//"F:\VS2008\test\test\WaveIn.h"

static INT NUMBER=0;
WAVEHDR* addbase=NULL;
WAVEHDR* baseAdd=NULL;
static HWAVEIN hWaveIn;
static HWAVEOUT hWaveOut;
WAVEFORMATEX waveform;
bool IsRecordClosed=false;
bool isPlayClosed=false;
GMM gmm[10];
int dataLen;
bool isInit=true;
int MyCount=0;
mBYTE* pBuffer[10]={};
bool DataFilled=false;

//DWORD start;
UINT waveIn(LPVOID lpParam)
//DWORD WINAPI waveIn(LPVOID lpParam)	
{

	//mBYTE* pBuffer[10];//10组
	WAVEHDR wHdr[10];
	//mBYTE* pBuffer[10];
	for(int i=0;i<10;i++)
		pBuffer[i]=new mBYTE[BUFFER_SIZE];
	for(int i=0;i<10;++i)
	{
		for(int j=0;j<BUFFER_SIZE;++j)
			pBuffer[i][j]=128;//初始化对应的数值为0
	}
	
	waveform.wFormatTag=WAVE_FORMAT_PCM;
	waveform.nSamplesPerSec=11025;//44100;//8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz
	waveform.wBitsPerSample=8;//8 or 16
	waveform.nChannels=1;//1 or 2
	waveform.nAvgBytesPerSec=11025*1;//nSamplesPerSec * nBlockAlign//读取的时间应该是所设定的总的数目(256*500)除以此数
	waveform.nBlockAlign=1;//nChannels * wBitsPerSample / 8
	waveform.cbSize=0;

	MMRESULT mmr = waveInOpen(&hWaveIn,WAVE_MAPPER,&waveform,
		(DWORD)(WaveInCallback),NULL, CALLBACK_FUNCTION);
	if(mmr != MMSYSERR_NOERROR)
		return 0;// false;
	DWORD bufsize=BUFFER_SIZE;
	for(int i=0;i<10;++i)
	{
		//pBuffer[i]=new mBYTE[bufsize];
		wHdr[i].lpData=(LPSTR)pBuffer[i];
		wHdr[i].dwBufferLength=BUFFER_SIZE;//bufsize;
		wHdr[i].dwBytesRecorded=0;
		wHdr[i].dwUser=i;
		wHdr[i].dwFlags=0;
		wHdr[i].dwLoops=1;
		wHdr[i].lpNext=NULL;
		wHdr[i].reserved=0;
		waveInPrepareHeader(hWaveIn,&wHdr[i],sizeof(WAVEHDR));
		waveInAddBuffer(hWaveIn,&wHdr[i],sizeof(WAVEHDR));
	}
	addbase=&wHdr[0];
	//if(!IsRecordClosed)
		waveInStart(hWaveIn);
	//start=GetTickCount();
	while(!IsRecordClosed)//是否已经有关闭的消息
	{
		;//NUMBER++;//记录变化次数 (调试用)
		//Sleep(1);
	}
	//while(!IsRecordClosed)//不满足关闭条件
	//{
	//	for(int i=0;i<9;++i)//更新后，可继续进行
	//	{
	//		//pBuffer[i]=new mBYTE[bufsize];
	//		wHdr[i].lpData=(LPSTR)pBuffer[i];
	//		wHdr[i].dwBufferLength=BUFFER_SIZE;//bufsize;
	//		wHdr[i].dwBytesRecorded=0;
	//		wHdr[i].dwUser=0;
	//		wHdr[i].dwFlags=0;
	//		wHdr[i].dwLoops=1;
	//		wHdr[i].lpNext=NULL;
	//		wHdr[i].reserved=0;
	//		waveInPrepareHeader(hWaveIn,&wHdr[i],sizeof(WAVEHDR));
	//		waveInAddBuffer(hWaveIn,&wHdr[i],sizeof(WAVEHDR));
	//	}
	//	Sleep(10000);
	//}
	//WaveInStop();
	//cout<<"---------------------"<<endl;
	return 0;
	//system("pause");
}

void WaveInStop()
{
	//IsRecordClosed=true;//如果在此处进行赋值，函数在执行完waveInStop后直接跳回到主函数，原因待解
	waveInStop(hWaveIn);
	waveInReset(hWaveIn);
	for(int i=9;i>=0;i--)
	{
		waveInUnprepareHeader(hWaveIn,addbase+i,sizeof(WAVEHDR));
		delete ((WAVEHDR*)(addbase+i))->lpData;
	}
	waveInClose(hWaveIn);
	IsRecordClosed=false;//不设置的话 不能再次收音
}

DWORD CALLBACK WaveInCallback(
						  HWAVEIN hWaveIn,
						  UINT uMsg,
						  DWORD dwInstance,
						  DWORD dwParam1,
						  DWORD dwParam2)/*Applications should not call any system-defined functions
										 from inside a callback function, except for EnterCriticalSection,
										 LeaveCriticalSection, midiOutLongMsg, midiOutShortMsg, 
										 OutputDebugString, PostMessage, PostThreadMessage, SetEvent,
										 timeGetSystemTime, timeGetTime, timeKillEvent, and timeSetEvent.
										 Calling other wave functions will cause deadlock.*/
{
	switch(uMsg)
	{
	case WIM_OPEN:
		AfxMessageBox(_T("Device opened"));
		break;
	case WIM_CLOSE:
		AfxMessageBox(_T("Device closed"));
		break;
	case WIM_DATA://The WIM_DATA message is sent to the given
		//waveform-audio input callback function when 
		//waveform-audio data is present in the input buffer
		//and the buffer is being returned to the application. 
		//The message can be sent when the buffer is full or 
		//after the waveInReset function is called.
		if(!IsRecordClosed)//判断下。不然调用waveInReset时也会进入
		{
			//DWORD stop=GetTickCount();
			//cout<<stop-start<<"ms"<<endl;
			//AfxMessageBox(_T("A data block finished"));
			//MyCount+=((LPWAVEHDR)dwParam1)->dwBytesRecorded;//用来统计缓冲区是否使用完毕(后期更新缓冲区内容)
			if(isIden)
			{
				if(!voiceToMFCC(pBuffer[MyCount],GOOD_FRAME_NUM*FRAME_LEN,MFCC,GOOD_FRAME_NUM))
				{
					;//(pRawVoice);
					return 0;
				}
				
				double maxI=0.0;
				int j=0;
				double value=0.0;
				for(int i=0;i<10;i++)
				{
					int res=GMM_identify(MFCC,&value,&gmm[i],GOOD_FRAME_NUM,16);
					if(value>maxI)
					{
						maxI=value;
						j=i;
					}
				}
				//CString id;
				//id.Format(_T("%d"),j);
				//int temp=AfxMessageBox(_T("GMM ")+id+_T(" identified!"));
				AfxMessageBox((CString)(names[j]).c_str()+_T(" identified!"));
			}
			DataFilled=true;
			waveInAddBuffer(hWaveIn,/*((LPWAVEHDR)dwParam1)*/addbase+MyCount,sizeof(WAVEHDR));
			MyTemp=MyCount;
			MyCount=(MyCount+1)%10;//0-9循环,重新设立结束标志位
			if(isInit)
				isInit=false;
			//NUMBER++;
			//cout<<NUMBER<<endl;
			//++num;
			//if(num==20)
			//{
			//	//WaveInStop();
			//	//内存处理
			//	//num=0;
			//	IsRecordClosed=true;
			//	//waveInMessage(hWaveIn,MM_WIM_CLOSE,dwParam1,dwParam2);
			//	//cout<<"########################"<<endl;
			//}
			//start=GetTickCount();
		}
		//processing
		break;
	default:
		break;
	}
	return 0;
}

UINT WaveOutOnline(LPVOID lpParam)
{
	waveform.wFormatTag=WAVE_FORMAT_PCM;
	waveform.nSamplesPerSec=11025;//44100;//8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz
	waveform.wBitsPerSample=8;//8 or 16
	waveform.nChannels=1;//1 or 2
	waveform.nAvgBytesPerSec=11025*1;//nSamplesPerSec * nBlockAlign//读取的时间应该是所设定的总的数目(256*500)除以此数
	waveform.nBlockAlign=1;//nChannels * wBitsPerSample / 8
	waveform.cbSize=0;  
	waveOutOpen(&hWaveOut,WAVE_MAPPER,&waveform,(DWORD_PTR)WaveOutOnlineCallback,NULL,CALLBACK_FUNCTION);
	//WAVEHDR PlayHdr[10]={};
	baseAdd=&PlayHdr[0];//保存基地址
	for(int i=0;i<10;i++)
	{
		PlayHdr->lpData=(LPSTR)pBuffer[i];//(MyCount+i)%10?
		PlayHdr->dwBufferLength=BUFFER_SIZE;
		PlayHdr->dwFlags=0;
		PlayHdr->dwLoops=0;
		waveOutPrepareHeader(hWaveOut,&PlayHdr[i],sizeof(WAVEHDR));
		waveOutWrite(hWaveOut,&PlayHdr[i],sizeof(WAVEHDR));
	}
	while(!isPlayClosed);
	return 0;
}

UINT WaveOutOffline(LPVOID lpParam)
{
	waveform.wFormatTag=WAVE_FORMAT_PCM;
	waveform.nSamplesPerSec=11025;
	waveform.wBitsPerSample=8;
	waveform.nChannels=1;
	waveform.nAvgBytesPerSec=11025*1;
	waveform.nBlockAlign=1;
	waveform.cbSize=0;//先把格式设置好，不然无法打开
	MMRESULT res=waveOutOpen(&hWaveOut,WAVE_MAPPER,&waveform,(DWORD_PTR)WaveOutOfflineCallback,NULL,CALLBACK_FUNCTION);
	/*switch (res)
	{
	case MMSYSERR_NOERROR:AfxMessageBox(_T("NOERROR"));
	break;
	case MMSYSERR_BADDEVICEID:AfxMessageBox(_T("BADDEVICEID"));
	break;
	case MMSYSERR_NODRIVER:AfxMessageBox(_T("NODRIVE"));
	break;
	case MMSYSERR_NOMEM:AfxMessageBox(_T("NOMEM"));
	break;
	case WAVERR_BADFORMAT:AfxMessageBox(_T("BADFORMAT"));
	break;
	case WAVERR_SYNC:AfxMessageBox(_T("SYNC"));
	break;
	default:AfxMessageBox(_T("OTHERS"));
	}*/
	while(res!=MMSYSERR_NOERROR)
	{
		res=waveOutOpen(&hWaveOut,WAVE_MAPPER,&waveform,(DWORD_PTR)WaveOutOfflineCallback,NULL,CALLBACK_FUNCTION);
	}
	//WAVEHDR PlayOffHdr;
	if(pRawVoice==NULL)
	{
		AfxMessageBox(_T("请输入声音文件！"));
		return -1;
	}
	PlayOffHdr.lpData=(LPSTR)pRawVoice;
	PlayOffHdr.dwBufferLength=dataLen;
	PlayOffHdr.dwFlags=0;
	PlayOffHdr.dwLoops=0;
	MMRESULT mmr=waveOutPrepareHeader(hWaveOut,&PlayOffHdr,sizeof(WAVEHDR));
	while (mmr!=MMSYSERR_NOERROR)
	{
		mmr=waveOutPrepareHeader(hWaveOut,&PlayOffHdr,sizeof(WAVEHDR));
	}
	/*switch (mmr)
	{
	case MMSYSERR_NOERROR:AfxMessageBox(_T("NOERROR"));
	break;
	case MMSYSERR_INVALHANDLE:AfxMessageBox(_T("INVALHANDLE"));
	break;
	case MMSYSERR_NODRIVER:AfxMessageBox(_T("NODRIVE"));
	break;
	case MMSYSERR_NOMEM:AfxMessageBox(_T("NOMEM"));
	break;
	default:AfxMessageBox(_T("OTHERS"));
	}*/
	waveOutWrite(hWaveOut,&PlayOffHdr,sizeof(WAVEHDR));
	while(!isPlayClosed);
	return 0;
}

void WaveOutOnlineStop()
{
	waveOutReset(hWaveOut);
	for(int i=9;i>=0;i--)
	{
		waveOutUnprepareHeader(hWaveOut,baseAdd+i,sizeof(WAVEHDR));
		delete ((WAVEHDR*)(baseAdd+i))->lpData;
	}
	waveOutClose(hWaveOut);
	isPlayClosed=true;
}
void WaveOutOfflineStop()
{
	isPlayClosed=true;//写在前面  首先改变标志位
	waveOutReset(hWaveOut);//会把系统内所有未播放的buffer全部被标记为WOM_DONE返回给WaveCallbackFunc
	waveOutUnprepareHeader(hWaveOut,&PlayOffHdr,sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	
}
DWORD CALLBACK WaveOutOnlineCallback(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	if(IsRecordClosed)
		isPlayClosed=true;
	return 0;
}
DWORD CALLBACK WaveOutOfflineCallback(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	if(uMsg==WOM_DONE)//触发WOM_DONE的时间与设置的缓冲区的大小有关
	{
		if(!isPlayClosed)
		{
			waveOutPrepareHeader(hWaveOut,&PlayOffHdr,sizeof(WAVEHDR));
			waveOutWrite(hWaveOut,&PlayOffHdr,sizeof(WAVEHDR));//循环播放
		}
		
		//isPlayClosed=true;
	}
	return 0;
}

