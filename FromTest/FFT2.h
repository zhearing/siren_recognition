/**
********************************************************************************
<PRE>
模块名: 基二时域快速傅立叶变换
文件名: FFT2.h
相关文件: FFT2.c
文件实现功能: 声明基二时域快速傅立叶变换相关函数
*******************************************************************************/
#pragma once
#include <stdio.h>
#include <malloc.h>
#include <math.h>
//#include "global.h"

#define PI 3.1415926
/**
* 复数结构
*/
struct COMPX
{
	double real; //!< 实部
	double imag; //!< 虚部
};


void FFT2(COMPX * xin, int N);

  //_FFT2_
