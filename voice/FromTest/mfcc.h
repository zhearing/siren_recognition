/** 
********************************************************************************
<PRE>
模块名: mfcc参数的提取
文件名: mfcc.h
相关文件: mfcc.c
文件实现功能: 声明mfcc提取相关函数,定义训练帧长度及特征微量维度
*******************************************************************************/
#pragma once
#include <string.h>
#include <float.h>
#include "FFT2.h"


#define SIGNAL_TIME 5       //!< 训练信号持续时间(s)
#define FRAME_LEN 256       //!< 帧长度

////帧数=音频采样频率*训练信号持续时间/帧长度
//#define FRAME_NUM (SAMPLE_FREQUENCY * SIGNAL_TIME / FRAME_LEN)
#define SILENCE_VALUE   0.0016  //!< 静寂状态阈值/帧，取0.016

#define GOOD_FRAME_NUM  200   //!< 连续大于静寂状态阈值的帧数（可用于训练的最小帧数） 
#define D 20                   //!< MFCC参数向量维度20


/**
* 语音帧结构
*/
struct VOICEFRAME
{
	COMPX  * pCmpx;
};

#define mBYTE unsigned char

static void PreEmphasis(mBYTE * pRawVoice, int iSampleNum, double * pDiffVoice); // 预增强
static double * JudgeTrainFrame2(double * pDiffVoice, int iSampleNum, int train_frame_num); // 判断是否为静寂语音
static int FrameMultiHamming(double * pDiffVoice, VOICEFRAME * pVoiceFrame, int train_frame_num);
static void FrameToMFCC(VOICEFRAME * pVoiceFrame, double  (*MFCC)[D], int train_frame_num);
static double HalfFrameEnergy(double * pDiffVoice); // 计算半帧能量
int voiceToMFCC(mBYTE * pRawVoice, int iSampleNum, double (*MFCC)[D], int train_frame_num);

//_MFCC_
