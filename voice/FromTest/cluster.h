/** 
********************************************************************************
<PRE>
ģ����: ���������������
�ļ���: cluster.h
����ļ�: cluser.c
�ļ�ʵ�ֹ���: �����������ຯ������
*******************************************************************************/
#pragma once
#include <string.h>
#include "FromTest\gmm.h"

void k_means(double (*c_vector)[D], GMM * pGMM, int * clusterIndex,  int * cSize, int train_frame_num, int m);