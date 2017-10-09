/** @file
********************************************************************************
<PRE>
ģ����: ���������������
�ļ���: cluster.c
����ļ�: cluser.h
�ļ�ʵ�ֹ���: ���������������ʵ��
</PRE>
*******************************************************************************/
#include "stdafx.h"
#include "cluster.h"

#define DIFF_CENTER_VALUE  1   //!< k��ֵ��һ��ֵ

static double getDistance(double * p1, double * p2);

/** @function
********************************************************************************
<PRE>
������: getDistance()
����: ���������ľ���
�÷�:
����:
[IN] p1: Dά������
[IN] p2: Dά������
����: ��������ƽ��
����:
��������: k_means()
</PRE>
*******************************************************************************/
static double getDistance(double * p1, double * p2)
{
	int i = 0;
	double distance = 0;

	while (i < D)
	{
		distance += pow(p1[i]-p2[i], 2);
		++i;
	}
	return distance;
}


/** @function
********************************************************************************
<PRE>
������: k_means()
����: ͨ��k��ֵ�㷨�õ�M���ֵ
�÷�:
����:
[IN] c_vector: ����������
[OUT] clusterIndex: ÿ������������������
[OUT] cSize: ÿ����������������
[IN] train_frame_num: ѵ��֡֡��
[IN] m: ��˹�����
����:
����: getDistance()
��������: InitGMM()
</PRE>
*******************************************************************************/
void k_means(double (*c_vector)[D], GMM * pGMM, int * clusterIndex,  int * cSize, int train_frame_num, int m)
{
	int i;
	int j;
	int k; // ��������������״̬��Ӧ��ΪM
	int farIndex = 0;
	int * clusterSize = NULL;        // ��������������
	double maxDistance = 0;          // ��¼���ĵ���Զ�����ľ���
	double minDistance = 0;          // ��¼���ĵ���������ľ���
	double distance = 0;             // ��¼���ĵ������ľ������ʱ����
	double ** mCenter       = NULL;  // ����
	double diff_Center       = 0;
	GMM gmm = *pGMM;


	clusterSize = (int *)calloc(m, sizeof(int));
	mCenter = (double **)malloc(m * sizeof(double *));
	for (i = 0; i < m; ++i)
	{
		mCenter[i] = (double *)calloc(D, sizeof(double));
	}


	// ���ȫ������
	for (i = 0; i < D; ++i)
	{
		for(j = 0; j < train_frame_num; ++j)
		{
			gmm.u[0][i] += c_vector[j][i];
		}
		gmm.u[0][i] /= train_frame_num;
	}

	//---------------------------------------------------------------------------------------------------
	// M�θ������ĵ�������
	//
	for (k = 1; k < m; ++k)
	{
		farIndex = 0; // ��¼��Զ��������
		maxDistance = 0;
		//--------------------------------------------------------------------------------
		// �������ѵ㣺����ÿ��������ÿ�����ĵľ���,Ѱ��������������Զ��������Ϊ������
		//
		for (j = 0; j < train_frame_num; ++j)
		{
			distance  = 0;
			// ������ÿ�����ĵľ����
			for (i = 0; i < k; ++i)    // i��ʾ������������
			{
				distance += sqrt(getDistance(gmm.u[i], c_vector[j]));
			}
			if (distance > maxDistance)
			{
				maxDistance = distance;
				farIndex = j;
			}
		}
		memcpy(gmm.u[k], c_vector[farIndex], D*sizeof(double)); // ��������


		//--------------------------------------------------------------------------------
		// ���·��༰��������
		//
NEXTLOOP: // |D1-D2| < DIFF_CENTER_VALUE ����
		for (j = 0; j < k+1; ++j)
		{
			memcpy(mCenter[j], gmm.u[j], D*sizeof(double));
		}

		for (j = 0; j < train_frame_num; ++j)
		{
			farIndex = 0;
			minDistance = getDistance(mCenter[0], c_vector[j]);
			// ������̣�farIndexΪ����������������
			for (i = 1; i <= k; ++i)    // i��ʾ������������
			{
				distance = getDistance(mCenter[i], c_vector[j]);
				if (distance < minDistance)
				{
					minDistance = distance;
					farIndex = i;
				}
			}

			// ��ȡ����������������������
			if (k == m - 1)
			{
				clusterIndex[j] = farIndex;
			}
			clusterSize[farIndex] += 1;
			// �������Ĺ���
			for (i = 0; i < D; ++i)
			{
				if (clusterSize[farIndex] == 1)
				{
					// ������
					gmm.u[farIndex][i] = 0;
				}
				gmm.u[farIndex][i] += c_vector[j][i];
			}

		} // end for (j = 0; j < GOOD_FRAME_NUM; ++j)

		// TODO   clusterSize��0ֵ�����ֶѲ���


		// ��ȡ������ĸ�����������������
		if (k == m - 1)
		{
			memcpy(cSize, clusterSize, m * sizeof(int));
		}

		// TODO �õ��µ�����(�������������ɼ������¹���)
		diff_Center = 0;
		for (i = 0; i <= k; ++i)
		{
			double temp = 0;

			if (clusterSize[i] > 0)
			{
				for (j = 0; j < D; ++j)
				{
					gmm.u[i][j] /= clusterSize[i];
					temp += pow(gmm.u[i][j] - mCenter[i][j], 2);
				}
				diff_Center += sqrt(temp);
			}
			clusterSize[i] = 0; // ������0
		}
		if (diff_Center >= DIFF_CENTER_VALUE)
		{
			goto NEXTLOOP;
		}

	}  // end for (k = 1; k < M; ++k)
	;//(clusterSize);
	for (i = 0; i < m; ++i)
	{
		;//(mCenter[i]);
	}
	;//(mCenter);
}