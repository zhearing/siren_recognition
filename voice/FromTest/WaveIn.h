#pragma once
#include<windows.h>
#include<mmsystem.h>
#include<iostream>
#include "FromTest\mfcc.h"
#include "FromTest\gmm.h"
#include "FromTest\cluster.h"
#include "FromTest\FFT2.h"
using namespace std;

#pragma comment(lib, "winmm.lib")   

#define BUFFER_SIZE GOOD_FRAME_NUM * FRAME_LEN//缓存区  // 录制声音长度
static DWORD CALLBACK WaveInCallback(
								 HWAVEIN hWaveIn,
								 UINT uMsg,
								 DWORD dwInstance,
								 DWORD dwParam1,
								 DWORD dwParam2);
static DWORD CALLBACK WaveOutOnlineCallback(
								 HWAVEOUT hWaveout,
								 UINT uMsg,
								 DWORD dwInstance,
								 DWORD dwParam1,
								 DWORD dwParam2);
static DWORD CALLBACK WaveOutOfflineCallback(
								 HWAVEOUT hWaveout,
								 UINT uMsg,
								 DWORD dwInstance,
								 DWORD dwParam1,
								 DWORD dwParam2);
extern mBYTE* pBuffer[10];
UINT waveIn(LPVOID lpParam);
//DWORD WINAPI waveIn(LPVOID lpParam);
void WaveInStop();
UINT WaveOutOnline(LPVOID lpParam);
UINT WaveOutOffline(LPVOID lpParam);
void WaveOutOnlineStop();
void WaveOutOfflineStop();
void Byte2Double(mBYTE* data,double* DouData);
void LeftMoveArray(double* ptr,size_t length,double data);
extern int MyCount;//如果用static声明的话，mydlg1中改变了MyCount,在MyDlg2中却不会改变 还是初始的0
static int num=0;
extern bool IsRecordClosed;
extern bool isPlayClosed;
extern bool isIden;
extern GMM gmm[10]; 
extern bool isInit;//用来判断是不是刚开始进行收音
extern int dataLen;//如果定义放到MyDlg1.cpp中则会出现error lnk1120
extern double MFCC[GOOD_FRAME_NUM][D];
extern mBYTE* pRawVoice;
static WAVEHDR PlayHdr[10];
static WAVEHDR PlayOffHdr;
extern int MyTemp;
extern bool DataFilled;

