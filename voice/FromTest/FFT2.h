/**
********************************************************************************
<PRE>
ģ����: ����ʱ����ٸ���Ҷ�任
�ļ���: FFT2.h
����ļ�: FFT2.c
�ļ�ʵ�ֹ���: ��������ʱ����ٸ���Ҷ�任��غ���
*******************************************************************************/
#pragma once
#include <stdio.h>
#include <malloc.h>
#include <math.h>
//#include "global.h"

#define PI 3.1415926
/**
* �����ṹ
*/
struct COMPX
{
	double real; //!< ʵ��
	double imag; //!< �鲿
};


void FFT2(COMPX * xin, int N);

  //_FFT2_
