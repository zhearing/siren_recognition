/** 
********************************************************************************
<PRE>
ģ����: ��˹���ģ�͹���
�ļ���: gmm.h
����ļ�: gmm.c
�ļ�ʵ�ֹ���: ��˹���ģ�͹�����غ���������������
*******************************************************************************/
#pragma once
#include "mfcc.h"
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

//#define DEBUG_GMM

// #ifndef DEBUG_GMM
// #define M 6  //!< GMM����(�����)8  3
// #else
// #define M 3
// #endif // DEBUG_GMM

/**
* ��˹���ģ�ͽṹ
*/
struct GMM
{//�ṹ���ڸĳɶ�̬�ڴ�
	void (* f_Init)(struct GMM *, int);
	int m;             //!< ��˹�����
	double * p;        //!< M���Ȩϵ��
	double ** u;    	  //!< M��Dά��ֵ
	double ** cMatrix; //!< M��Э�������(Dά�Խ���)
	void * (* f_CallocGMM)(struct GMM *);
	void (* f_FreeGMM)(struct GMM *);
};


int GMMs(double (*X)[D], GMM * pGMM, int train_frame_num, int m); // GMM��ģ����
int GMM_identify(double (*X)[D], double * value, GMM * pGMM, int frame_num, int m); // ʶ�����
void InitGMMClass(GMM * pGMM, int m);
void * CallocGMM(GMM * pGMM); // ����GMM�ڴ�
void FreeGMM(GMM * pGMM); // �ͷ�GMM�ڴ�
double* str2doubles(char* p);
double** ToMatrix(double* p,int m,int d);
void GMM2Txt(ofstream& f,GMM* p,int d);
void GMMCalloc(GMM* pGmm,int m);
void load(ifstream& f,GMM* p);
extern string names[10];

/* _GMM_H_ */
