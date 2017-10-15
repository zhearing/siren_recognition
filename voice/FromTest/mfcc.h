/** 
********************************************************************************
<PRE>
ģ����: mfcc��������ȡ
�ļ���: mfcc.h
����ļ�: mfcc.c
�ļ�ʵ�ֹ���: ����mfcc��ȡ��غ���,����ѵ��֡���ȼ�����΢��ά��
*******************************************************************************/
#pragma once
#include <string.h>
#include <float.h>
#include "FFT2.h"


#define SIGNAL_TIME 5       //!< ѵ���źų���ʱ��(s)
#define FRAME_LEN 256       //!< ֡����

////֡��=��Ƶ����Ƶ��*ѵ���źų���ʱ��/֡����
//#define FRAME_NUM (SAMPLE_FREQUENCY * SIGNAL_TIME / FRAME_LEN)
#define SILENCE_VALUE   0.0016  //!< ����״̬��ֵ/֡��ȡ0.016

#define GOOD_FRAME_NUM  200   //!< �������ھ���״̬��ֵ��֡����������ѵ������С֡���� 
#define D 20                   //!< MFCC��������ά��20


/**
* ����֡�ṹ
*/
struct VOICEFRAME
{
	COMPX  * pCmpx;
};

#define mBYTE unsigned char

static void PreEmphasis(mBYTE * pRawVoice, int iSampleNum, double * pDiffVoice); // Ԥ��ǿ
static double * JudgeTrainFrame2(double * pDiffVoice, int iSampleNum, int train_frame_num); // �ж��Ƿ�Ϊ��������
static int FrameMultiHamming(double * pDiffVoice, VOICEFRAME * pVoiceFrame, int train_frame_num);
static void FrameToMFCC(VOICEFRAME * pVoiceFrame, double  (*MFCC)[D], int train_frame_num);
static double HalfFrameEnergy(double * pDiffVoice); // �����֡����
int voiceToMFCC(mBYTE * pRawVoice, int iSampleNum, double (*MFCC)[D], int train_frame_num);

//_MFCC_
