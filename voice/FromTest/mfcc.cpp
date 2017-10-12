/** @file
********************************************************************************
<PRE>
ģ����: mfcc��������ȡ
�ļ���: mfcc.c
����ļ�: mfcc.h
�ļ�ʵ�ֹ���: mfcc��ȡ��غ�����ʵ��*/

#include "stdafx.h"
#include "mfcc.h"
//#define mBYTE unsigned char
//
//static void PreEmphasis(mBYTE * pRawVoice, int iSampleNum, double * pDiffVoice); // Ԥ��ǿ
//static double * JudgeTrainFrame2(double * pDiffVoice, int iSampleNum, int train_frame_num); // �ж��Ƿ�Ϊ��������
//static int FrameMultiHamming(double * pDiffVoice, VOICEFRAME * pVoiceFrame, int train_frame_num);
//static void FrameToMFCC(VOICEFRAME * pVoiceFrame, double  (*MFCC)[D], int train_frame_num);
//static double HalfFrameEnergy(double * pDiffVoice); // �����֡����
//int voiceToMFCC(mBYTE * pRawVoice, int iSampleNum, double (*MFCC)[D], int train_frame_num);
static int count=0;
/**
* @brief ���Ǵ�ͨ�˲�������
*/
static double melf16[239] =
{
	//0     [3]
	1.4254, 1.2616, 0.0445,
	//1     [3]
	0.7384, 1.9555, 0.9103,
	//2     [3]
	1.0897, 1.8484, 0.8501,
	//3     [5]
	0.1516, 1.1499, 1.9082, 1.0167, 0.1705,
	//4     [5]
	0.0918, 0.9833, 1.8295, 1.3652, 0.5970,
	//5     [5]
	0.6348, 1.4030, 1.8627, 1.1593, 0.4845,
	//6     [7]
	0.1373, 0.8407, 1.5155, 1.8359, 1.2116, 0.6099, 0.0291,
	//7     [7]
	0.1641, 0.7884, 1.3901, 1.9709, 1.4679, 0.9249, 0.3991,
	//8     [7]
	0.5321, 1.0751, 1.6009, 1.8894, 1.3948, 0.9144, 0.4476,
	//9     [9]
	0.1106, 0.6052, 1.0856, 1.5524, 1.9934, 1.5513, 1.1206, 0.7008, 0.2912,
	//10     [11]
	0.0066, 0.4487, 0.8794, 1.2992, 1.7088, 1.8915, 1.5012, 1.1198, 0.7469,
	0.3822, 0.0253,
	//11     [12]
	0.1085, 0.4988, 0.8802, 1.2531, 1.6178, 1.9747, 1.6759, 1.3336, 0.9983,
	0.6696, 0.3472, 0.0309,
	//12     [12]
	0.3241, 0.6664, 1.0017, 1.3304, 1.6528, 1.9691, 1.7205, 1.4158, 1.1166,
	0.8226, 0.5338, 0.2498,
	//13     [14]
	0.2795, 0.5842, 0.8834, 1.1774, 1.4662, 1.7502, 1.9707, 1.6961, 1.4260,
	1.1601, 0.8985, 0.6409, 0.3872, 0.1373,
	//14     [17]
	0.0293, 0.3039, 0.5740, 0.8399, 1.1015, 1.3591, 1.6128, 1.8627, 1.8911,
	1.6485, 1.4094, 1.1737, 0.9412, 0.7119, 0.4858, 0.2627, 0.0425,
	//15     [18]
	0.1089, 0.3515, 0.5906, 0.8263, 1.0588, 1.2881, 1.5142, 1.7373, 1.9575,
	1.8252, 1.6107, 1.3989, 1.1898, 0.9833, 0.7793, 0.5777, 0.3786, 0.1818,
	//16     [20]
	0.1748, 0.3893, 0.6011, 0.8102, 1.0167, 1.2207, 1.4223, 1.6214, 1.8182,
	1.9874, 1.7951, 1.6051, 1.4172, 1.2314, 1.0476, 0.8659, 0.6861, 0.5082,
	0.3322, 0.1580,
	//17     [24]
	0.0126, 0.2049, 0.3949, 0.5828, 0.7686, 0.9524, 1.1341, 1.3139, 1.4918,
	1.6678, 1.8420, 1.9857, 1.8151, 1.6462, 1.4791, 1.3136, 1.1497, 0.9874,
	0.8267, 0.6675, 0.5098, 0.3536, 0.1988, 0.0455,
	//18     [27]
	0.0143, 0.1849, 0.3538, 0.5209, 0.6864, 0.8503, 1.0126, 1.1733, 1.3325,
	1.4902, 1.6464, 1.8012, 1.9545, 1.8936, 1.7430, 1.5938, 1.4459, 1.2993,
	1.1540, 1.0100, 0.8672, 0.7256, 0.5852, 0.4459, 0.3078, 0.1709, 0.0350,
	//19     [29]
	0.1064, 0.2570, 0.4062, 0.5541, 0.7007, 0.8460, 0.9900, 1.1328, 1.2744,
	1.4148, 1.5541, 1.6922, 1.8291, 1.9650, 1.9003, 1.7667, 1.6341, 1.5025,
	1.3720, 1.2425, 1.1140, 0.9865, 0.8599, 0.7343, 0.6097, 0.4859, 0.3631,
	0.2412, 0.1202
};


/**
* @brief ���Ǵ�ͨ�˲��������±���������ʾ���˲��������е���ʼλ�ã���С�кţ�����кţ�
*/
static int indexM16[D][3] =
{
	{0, 1, 3}, {3, 2, 4}, {6, 4, 6}, {9, 5, 9},
	{14, 7, 11}, {19, 10, 14}, {24, 12, 18}, {31, 15, 21},
	{38, 19, 25}, {45, 22, 30}, {54, 26, 36}, {65, 31, 42},
	{77, 37, 48}, {89, 43, 56}, {103, 49, 65}, {120, 57, 74},
	{138, 66, 85}, {158, 75, 98}, {182, 86, 112}, {209, 99, 127},
};


/** @function
********************************************************************************
<PRE>
������: PreEmphasis()
����: Ԥ��ǿ����ԭʼ�������������
�÷�:
����:
[IN] pRawVoice: ԭʼ��������
[IN] iSampleNum: ����������
[OUT] pDiffVoice: ��ֺ������
����:
����:
��������: voiceToMFCC()
</PRE>
*******************************************************************************/
static void PreEmphasis(mBYTE * pRawVoice, int iSampleNum, double * pDiffVoice)
{
	int i;

	for (i = 84; i < iSampleNum; ++i)
	{
		pDiffVoice[i-84] = (pRawVoice[i] - 128) * 0.0078125;
	}
	/*for(i=iSampleNum-1;i>0;i--)
	pDiffVoice[i]=pRawVoice[i]-0.9375*pRawVoice[i-1];*/
}


/** @function
********************************************************************************
<PRE>
������: JudgeTrainFrame2()
����: �ж�Ԥ��ǿ��������Ƿ������Ϊѵ��֡��ȥ�����������Ķ������������
�÷�:
����:
[IN] pDiffVoice: ��ǿ����֣������������
[IN] iSampleNum: ����������
[IN] train_frame_num: ѵ��֡֡��
����:
�ǿ�: ������Ϊѵ��֡���ź��׵�ַ
NULL: ������Ϊѵ��֡
����: HalfFrameEnergy()
��������: voiceToMFCC()
��ע: ���ص��ڴ���Ҫ�û���voiceToMFCC()�������ͷ�
</PRE>
*******************************************************************************/
static double * JudgeTrainFrame2(double * pDiffVoice, int iSampleNum, int train_frame_num)
{
	int len = 0;
	int iNum;   // �ϸ������֡��
	int iFrame; // ֡���
	int iFrameNum = (iSampleNum>>7) - 1;   // ֡����=2*(������/256)-1
	double PriorHalfFrame = 0;             // ǰ��֡����(ǰ128B)
	double NextHalfFrame = 0;              // ���֡����(��128B)
	double FrameEnergy = 0;                // һ֡���ۻ�����
	double * pTranDiff = NULL;             // �ϸ�����
	double * pSave;                        // �ϸ������洢��ַ
	double * pDiffHead = pDiffVoice;
	double * pDiffNext  = pDiffVoice;


	if (iFrameNum < train_frame_num)
	{
		return NULL;
	}
	pTranDiff = (double *)calloc((train_frame_num/2 + 1)*FRAME_LEN, sizeof(double));
	if (!pTranDiff)
	{
		return NULL;
	}
	pSave = pTranDiff;
	PriorHalfFrame = HalfFrameEnergy(pDiffHead);
	for (iFrame=1, iNum=0; iFrame<iFrameNum && iNum<train_frame_num; ++iFrame)
	{
		count++;
		//if(count==6820)
		//	printf("break");
		pDiffNext += (FRAME_LEN/2);        // ָ��iFrame��֡�ĺ��֡
		NextHalfFrame = HalfFrameEnergy(pDiffNext);
		FrameEnergy = PriorHalfFrame + NextHalfFrame;
		if (FrameEnergy < SILENCE_VALUE)   // �Ǿ�������
		{
			if (iFrameNum-iFrame < train_frame_num-iNum)   // ʣ�µ�֡��������Ϊѵ��֡
			{
				;//(pTranDiff);
				return NULL;
			}
			len = pDiffNext - pDiffHead - FRAME_LEN/2;
			if (len > 0)
			{
				memcpy(pSave, pDiffHead, len * sizeof(double));
				pSave += len;
			}
			pDiffNext +=  FRAME_LEN/2;  // ���¶�λ������ѵ����֡
			pDiffHead= pDiffNext;
			NextHalfFrame = HalfFrameEnergy(pDiffHead);
		}
		else
		{
			++iNum;
		}

		PriorHalfFrame = NextHalfFrame;
	} // end: for(;;)

	len = pDiffNext - pDiffHead + FRAME_LEN/2;
	if (len > 0)
	{
		memcpy(pSave, pDiffHead, len * sizeof(double));
	}

	return pTranDiff;
}


/** @function
********************************************************************************
<PRE>
������: FrameMultiHamming()
����: ���򻯲��˺�������Ȼ����ٸ���Ҷ�任���Բ�ֺ���������ݷ�֡,���ų���Ĭ������256��
������Ϊһ������λ��֡������128Ϊ����֮����ص���λȻ����Ժ�����,���ÿ֡��FFT2��
�÷�:
����:
[IN] pDiffVoice: ��ǿ����֣������������
[OUT] pVoiceFrame: ָ����õ�ѵ��֡
[IN] train_frame_num: ѵ��֡֡��
����:
1: ��֡�ɹ�
0: ��֡ʧ��
����:
��������: voiceToMFCC()
��ע: pVoiceFrameָ���ѵ��֡�洢�ռ��ڴ˺���������
</PRE>
*******************************************************************************/
static int FrameMultiHamming(double * pDiffVoice, VOICEFRAME * pVoiceFrame, int train_frame_num)
{
	int i;            // ��֡�м����
	int iFrame = 0;   // ֡���
	int iPointNum =  FRAME_LEN -1;


	//---------------------------------------------------------------------------
	// �߷�֡�߳˺�������FFT2�任
	//
	for (iFrame = 0 ; iFrame < train_frame_num; ++iFrame)
	{
		pVoiceFrame[iFrame].pCmpx = (COMPX *)malloc(sizeof(COMPX)*FRAME_LEN);
		if (!pVoiceFrame[iFrame].pCmpx)
		{
			printf("malloc Frame error");
			while (iFrame > 0)
			{
				;//(pVoiceFrame[--iFrame].pCmpx);
			}
			return 0;
		}

		for (i = 0; i < FRAME_LEN; ++i)
		{
			// �˺�����
			pVoiceFrame[iFrame].pCmpx[i].real = pDiffVoice[128*iFrame+i] * (0.54-0.46*cos(2*PI*i/iPointNum));
			pVoiceFrame[iFrame].pCmpx[i].imag = 0;
		}

		FFT2(pVoiceFrame[iFrame].pCmpx, FRAME_LEN); // �������ٸ���Ҷ�任��ʱ��
	}
	return 1;
}


/** @function
********************************************************************************
<PRE>
������: FrameToMFCC()
����: ��ѵ��֡�õ�MFCC������
�÷�:
����:
[IN] pVoiceFrame: FFT2���ѵ��֡
[OUT] MFCC: ָ����MFCC�����������ָ�룬��СΪMFCC[֡��][20]
[IN] train_frame_num: ѵ��֡֡��
����:
����:
��������: voiceToMFCC()
��ע: �˺������ͷ�pVoiceFrameָ���ѵ��֡�ռ�
</PRE>
*******************************************************************************/
static void FrameToMFCC(VOICEFRAME * pVoiceFrame, double (*MFCC)[D], int train_frame_num)
{
	int i;                // �����к�
	int j;                // �����к�
	int iFrame  = 0;      // ֡���
	double Z[D] = {0};    // �˲�������Ķ�������
	double a[2] = {0};

	//---------------------------------------------------------------------------
	// ��ÿһ֡�������Ǵ�ͨ�˲�����DCT�����ͷ�֡�ռ�
	//
	for (iFrame = 0; iFrame < train_frame_num; ++iFrame)
	{
		for (i = 0; i < 129; ++i)
		{
			for (j = 0; j < D; ++j)
			{
				if (i < indexM16[j][1] || i > indexM16[j][2])  // �˲���Ԫ��λ�ò���������
				{
					// �˲�����Ӧλ�þ���Ԫ��Ϊ0
					continue;
				}
				else
				{
					// �˲�������ĳ˷�[20][129]*[129][1]=[20]
					Z[j] += melf16[indexM16[j][0] + i - indexM16[j][1]] *
						(pow(pVoiceFrame[iFrame].pCmpx[i].real, 2)+pow(pVoiceFrame[iFrame].pCmpx[i].imag, 2));
				}
			}    // for (j = 0; j < D; ++j)
		}    // for (i = 0; i < 129; ++i)

		for (j = 0; j < D; j++) // �õ��˲�������Ķ�������
		{
			Z[j] = log(Z[j]);
		}
		a[0] = pow(0.05, 0.5);
		a[1] = pow(0.1, 0.5);
		for (i = 0; i < D; i++) // DCT����
		{
			MFCC[iFrame][i] = 0;
			for (j = 0; j < D; j++)
			{
				MFCC[iFrame][i] += Z[j] * cos(((2 * j + 1) * i * PI) / 40);
			}
			MFCC[iFrame][i] *= a[i != 0 ? 1 : 0];
		}
		for (j = 0; j < D; j++)
		{
			Z[j] = 0; // ������Ϊ0���Ա���һ֡ʹ��
		}

		;//(pVoiceFrame[iFrame].pCmpx);
	}
}


/** @function
********************************************************************************
<PRE>
������: HalfFrameEnergy()
����: �����֡��128BYTE�����ۻ�����
�÷�:
����:
[IN] pDiffVoice: ÿ��֡����ʼָ��
����: ��֡�ۻ�����ֵ
����:
��������: JudgeTrainFrame2()
��ע: Ч����Ҫ�Ľ�
</PRE>
*******************************************************************************/
static double HalfFrameEnergy(double * pDiffVoice)
{
	int i;
	double iHalfEnergy = 0;


	for (i = 0; i < FRAME_LEN/2;  ++i)
	{
		iHalfEnergy += pow(pDiffVoice[i], 2);
	}
	return iHalfEnergy;
}


/** @function
********************************************************************************
<PRE>
������: voiceToMFCC()
����: �õ�������MFCC������
�÷�:
����:
[IN] pRawVoice: ԭʼ��������
[IN] iSampleNum: ����������
[OUT] MFCC: ָ����MFCC�������ָ�룬��СΪMFCC[֡��][20]
[IN] train_frame_num: ѵ��֡֡��
����:
1: �ɹ���ȡ
0: ��ȡʧ��
����: PreEmphasis(), JudgeTrainFrame(), FrameMultiHamming(), FrameToMFCC()
��������:
��ע: �ṩ�����Ľӿ�
</PRE>
*******************************************************************************/
int voiceToMFCC(mBYTE * pRawVoice, int iSampleNum, double (*MFCC)[D], int train_frame_num)
{
	double * pDiffVoice = NULL;
	double * pDiff = NULL;
	VOICEFRAME * pVoiceFrame = NULL; // ָ����õ�ѵ��֡
	iSampleNum=iSampleNum-84;

	//pDiffVoice = (double *)malloc(iSampleNum * sizeof(double));
	pDiffVoice = (double *)calloc(iSampleNum, sizeof(double));
	if (!pDiffVoice)
	{
		printf("pDiffVoice");
		return 0;
	}
	PreEmphasis(pRawVoice, iSampleNum, pDiffVoice);    // Ԥ��ǿ
	pDiff = JudgeTrainFrame2(pDiffVoice, iSampleNum, train_frame_num);
	if (!pDiff)    // �жϾ�������
	{
		printf("Error: It`s silent voice!");
		;//(pDiffVoice);
		return 0;
	}

	pVoiceFrame = (VOICEFRAME *)malloc(train_frame_num * sizeof(VOICEFRAME));
	FrameMultiHamming(pDiff, pVoiceFrame, train_frame_num);  // �õ�Ƶ��֡
	;//(pDiff);
	FrameToMFCC(pVoiceFrame, MFCC, train_frame_num); // ��֡�õ�MFCC������

	;//(pVoiceFrame);
	;//(pDiffVoice);
	return 1;
}
