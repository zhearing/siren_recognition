/** 
********************************************************************************
<PRE>
模块名: 高斯混合模型过程
文件名: gmm.h
相关文件: gmm.c
文件实现功能: 高斯混合模型过程相关函数、变量的声明
*******************************************************************************/
#pragma once
#include "mfcc.h"
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

//#define DEBUG_GMM

// #ifndef DEBUG_GMM
// #define M 6  //!< GMM阶数(混合数)8  3
// #else
// #define M 3
// #endif // DEBUG_GMM

/**
* 高斯混合模型结构
*/
struct GMM
{//结构体内改成动态内存
	void (* f_Init)(struct GMM *, int);
	int m;             //!< 高斯混合数
	double * p;        //!< M组加权系数
	double ** u;    	  //!< M组D维均值
	double ** cMatrix; //!< M个协方差矩阵(D维对角阵)
	void * (* f_CallocGMM)(struct GMM *);
	void (* f_FreeGMM)(struct GMM *);
};


int GMMs(double (*X)[D], GMM * pGMM, int train_frame_num, int m); // GMM建模过程
int GMM_identify(double (*X)[D], double * value, GMM * pGMM, int frame_num, int m); // 识别过程
void InitGMMClass(GMM * pGMM, int m);
void * CallocGMM(GMM * pGMM); // 分配GMM内存
void FreeGMM(GMM * pGMM); // 释放GMM内存
double* str2doubles(char* p);
double** ToMatrix(double* p,int m,int d);
void GMM2Txt(ofstream& f,GMM* p,int d);
void GMMCalloc(GMM* pGmm,int m);
void load(ifstream& f,GMM* p);
extern string names[10];

/* _GMM_H_ */
