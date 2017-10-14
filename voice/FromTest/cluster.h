/** 
********************************************************************************
<PRE>
模块名: 特征向量聚类过程
文件名: cluster.h
相关文件: cluser.c
文件实现功能: 特征向量聚类函数声明
*******************************************************************************/
#pragma once
#include <string.h>
#include "FromTest\gmm.h"

void k_means(double (*c_vector)[D], GMM * pGMM, int * clusterIndex,  int * cSize, int train_frame_num, int m);