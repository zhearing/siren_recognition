/** @file
********************************************************************************
<PRE>
模块名: 高斯混合模型过程
文件名: gmm.c
相关文件: gmm.h
文件实现功能: 高斯混合模型过程相关函数的实现
--------------------------------------------------------------------------------
备注: 高斯混合模型：GMM＝｛p[i], u[i][D], cMatrix[i][D]｝，i=0,...,M-1
--------------------------------------------------------------------------------

</PRE>
*******************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "gmm.h"
#include "cluster.h"


#define DIFF_GMM_VALUE     1             //!< GMM归一阈值
#define IDENTIFY_VALUE    (-30000)        //!< 识别阈值
//#define IDENTITY_FRAME_NUM 100           //!< 识别帧数
#define PI 3.1415926


static double getDistance(double * p1, double * p2);
static int InitGMM(double (*X)[D], GMM * pGMM, int train_frame_num, int m); // 初始化GMM参数
static int BuildGMM(GMM * pGMM, GMM * Out, double (*X)[D], int train_frame_num, int m);
static double GMM_diff(GMM * gmm1, GMM * gmm2);
static double GMM_density(GMM * pGMM, double * X, int index); // 高斯密度函数


/** @function
********************************************************************************
<PRE>
函数名: InitGMMClass()
功能: 初始化GMM结构
用法: 由GMM结构的f_init调用
参数:
[OUT] pGMM: GMM结构指针
[IN] m: GMM的高期混合数
返回: 
调用:
</PRE>
*******************************************************************************/
void InitGMMClass(GMM * pGMM, int m)
{
	pGMM->m = m;
	pGMM->p = NULL;
	pGMM->u = NULL;
	/*double** mat=(double**)calloc(16,sizeof(double*));
	for(int j=0;j<16;j++)
	mat[j]=(double*)calloc(20,sizeof(double));
	for(int j=0;j<16;j++)
	{
	for(int k=0;k<20;k++)
	mat[j][k]=1.0;
	}*/
	pGMM->cMatrix = NULL;//mat;
	pGMM->f_CallocGMM = CallocGMM;
	pGMM->f_FreeGMM = FreeGMM;
}


/** @function
********************************************************************************
<PRE>
函数名: CallocGMM()
功能: 为GMM结构内的指针分配内存
用法: 由GMM结构的f_CallocGMM调用
参数:
[OUT] pGMM: GMM结构指针
返回: 
调用:
</PRE>
*******************************************************************************/
void * CallocGMM(GMM * pGMM)
{
	int i;
	int m = pGMM->m;

	pGMM->p = (double *)calloc(m, sizeof(double));
	if (!pGMM->p)
	{
		return NULL;
	}
	pGMM->u = (double **)malloc(m * sizeof(double *));
	if (!pGMM->u)
	{
		free(pGMM->p);
		return NULL;
	}
	pGMM->cMatrix = (double **)malloc(m * sizeof(double *));
	if (!pGMM->cMatrix)
	{
		free(pGMM->p);
		free(pGMM->u);
		return NULL;
	}
	for (i = 0; i < m; ++i)
	{
		pGMM->u[i] = (double *)calloc(D, sizeof(double)); 
		if (!pGMM->u[i])
		{
			for(--i; i >=0 ; --i)
			{
				free(pGMM->u[i]);
				free(pGMM->cMatrix[i]);
			}
			free(pGMM->p);
			free(pGMM->u);
			free(pGMM->cMatrix);
			return NULL;
		}
		pGMM->cMatrix[i] = (double *)calloc(D, sizeof(double));
		if (!pGMM->cMatrix[i])
		{
			//free(pGMM->u[i]);
			for(--i; i >=0 ; --i)
			{
				free(pGMM->u[i]);
				free(pGMM->cMatrix[i]);
			}
			free(pGMM->p);
			free(pGMM->u);
			free(pGMM->cMatrix);
			return NULL;
		}
	} // for (i = 0; i < m; ++i)
	return pGMM;
}


/** @function
********************************************************************************
<PRE>
函数名: FreeGMM()
功能: 释放为GMM结构内的指针分配内存
用法: 由GMM结构的f_FreeGMM调用
参数:
[IN] pGMM: GMM结构指针
返回: 
调用:
</PRE>
*******************************************************************************/
void FreeGMM(GMM * pGMM)
{
	int i;
	int m = pGMM->m;

	if (!pGMM)
	{
		return;
	}
	if (pGMM->p)
	{
		free(pGMM->p);
	}
	if (!pGMM->u && !pGMM->cMatrix)
	{
		return;
	}
	for (i = 0; i < m; ++i)
	{
		if (pGMM->u && pGMM->u[i])
		{
			free(pGMM->u[i]);
		}
		if (pGMM->cMatrix && pGMM->cMatrix[i])
		{
			free(pGMM->cMatrix[i]);
		}
	}
	free(pGMM->u);
	free(pGMM->cMatrix);
}





/** @function
********************************************************************************
<PRE>
函数名: InitGMM()
功能: 获得GMM初值
用法:
参数:
[IN] X: 特征向量组
[OUT] pGMM: GMM初值
[IN] train_frame_num: 训练帧帧数
[IN] m: 高斯混合数
返回:
1: 成功
0：失败
调用: k_means()
主调函数: GMMs()
</PRE>
*******************************************************************************/
static int InitGMM(double (*X)[D], GMM * pGMM, int train_frame_num, int m)
{
	int i;
	int j;
	int * clusterIndex = NULL;  // 向量所属质心索引
	int * clusterSize = NULL;   // 聚类所含向量数
	double ** sum = NULL;
	GMM gmm = *pGMM;


	clusterIndex = (int *)calloc(train_frame_num, sizeof(int));
	clusterSize = (int *)calloc(m, sizeof(int));
	sum = (double **)malloc(m * sizeof(double *));
	for (i = 0; i < m; ++i)
	{
		sum[i] = (double *)calloc(D, sizeof(double));
	}
	// 初始化均值u
	k_means(X, pGMM, clusterIndex, clusterSize, train_frame_num, m);

	// 初始化加权系数p
	for (i = 0; i < m; ++i)
	{
		gmm.p[i] = 1.0 / m;
	}

	 	for(i=0;i<m;i++)
	 	{
	 		for(j=0;j<D;j++)
	 			gmm.cMatrix[i][j]=1.0;
	 	}//自己加的协方差矩阵初始化为多个单位对角阵
	// 初始化协方差矩阵cMatrix
	//for (i = 0; i < train_frame_num; ++i)
	//{
	//	for (j = 0; j < D; ++j)
	//	{
	//		sum[clusterIndex[i]][j] += pow(X[i][j], 2);
	//	}
	//}
	//for (i = 0; i < m; i++)
	//{
	//	/*		printf("第%d个clusterSize为%d\n",i,clusterSize[i]);*/
	//	if (clusterSize[i] > 0) // 防止分母为0
	//	{
	//		for (j = 0; j < D; j++)
	//		{
	//			// TODO: 此处有负值和0值
	//			gmm.cMatrix[i][j] = sum[i][j] / clusterSize[i] - pow(gmm.u[i][j], 2);
	//			// 				if (gmm.cMatrix[i][j] < 0)
	//			// 				{
	//			// 					printf("Initial value of GMM`cMatrix < 0");
	//			// 					gmm.f_FreeGMM(&gmm);
	//			// 					free(clusterIndex);
	//			// 					free(clusterSize);
	//			// 					for (i = 0; i < m; ++i)
	//			// 					{
	//			// 						free(sum[i]);
	//			// 					}
	//			// 					free(sum);
	//			// 					return 0;
	//			// 				}
	//			// 				else if(gmm.cMatrix[i][j] < 1e-10) // 消除相同double值相减的误差
	//			// 				{
	//			// 					gmm.cMatrix[i][j] = 0;
	//			// 				}
	//			if(gmm.cMatrix[i][j]<1e-6)
	//				gmm.cMatrix[i][j]=1e-4;
	//		}
	//	} // end if (clusterSize[i] > 0)
	//	else
	//	{
	//		for(j=0;j<D;j++)
	//			gmm.cMatrix[i][j]=1.0;
	//		// 			printf("InitGMM: Decrease M and increase frame number");
	//		// 			printf("clusterSize[%d]: %d\n", i, clusterSize[i]);
	//		// 			gmm.f_FreeGMM(&gmm);
	//		// 			free(clusterIndex);
	//		// 			free(clusterSize);
	//		// 			for (i = 0; i < m; ++i)
	//		// 			{
	//		// 				free(sum[i]);
	//		// 			}
	//		// 			free(sum);
	//		// 			return 0;
	//	}
	//} // end for (i = 0; i < m; ++i)


	free(clusterIndex);
	free(clusterSize);
	for (i = 0; i < m; ++i)
	{
		free(sum[i]);
	}
	free(sum);
	return 1;
}


/** @function
********************************************************************************
<PRE>
函数名: GMM_diff()
功能: 计算迭代GMM的差值
用法:
参数:
[IN] gmm1: 这一轮迭代的结果
[IN] gmm2: 上一轮迭代的结果
返回: 两次迭代GMM的差值（各对应点差的平方和）
调用:
主调函数: GMMs()
</PRE>
*******************************************************************************/
static double GMM_diff(GMM * gmm1, GMM * gmm2)
{
	int i;
	int j;
	double diff = 0;

	int m=gmm1->m;
	for (i = 0; i < m; ++i)
	{
		diff += pow(gmm1->p[i] - gmm2->p[i], 2);
		for (j = 0; j < D; ++j)
		{
			diff += pow(gmm1->u[i][j] - gmm2->u[i][j], 2);
			diff += pow(gmm1->cMatrix[i][j] - gmm2->cMatrix[i][j], 2);
		}
	}

	return diff;
}


/** @function
********************************************************************************
<PRE>
函数名: GMM_density()
功能: 计算高斯概率密度
用法:
参数:
[IN] pGMM: 上一轮迭代的结果
[IN] X: 特征向量组
[IN] index: 高斯混合模型阶数索引
返回: 高斯概率密度值
调用:
主调函数: BuildGMM()
</PRE>
*******************************************************************************/
static double GMM_density(GMM * pGMM, double * X, int index) //高斯密度函数
{
	int i;
	double temp = 0.0;
	double sqrt_Matrix_value = 1.0;
	double res = 0.0;


	for (i = 0; i < D; ++i)
	{
		//TODO 对角矩阵的对角线中出现0，导致无穷大1#INF
		temp += pow(X[i] - pGMM->u[index][i], 2) / pGMM->cMatrix[index][i];
		//TODO sqrt出现-nan,因为开方值为负
		sqrt_Matrix_value *= sqrt(pGMM->cMatrix[index][i]);
	}
	//TODO temp值太大，导致exp(temp / -2)为0
	res = pow(2*PI, D/-2) / sqrt_Matrix_value * exp(temp / -2);
	return res;
}


/** @function
********************************************************************************
<PRE>
函数名: BuildGMM()
功能: 迭代下一轮GMM
用法:
参数:
[IN] pGMM: 上一次迭代的结果
[OUT] Out: 迭代得到的GMM
[IN] X: 特征向量组
[IN] train_frame_num: 训练帧帧数
[IN] m: 高斯混合数
返回: 
1: 成功
0: 失败
调用: GMM_density()
主调函数: GMMs()
</PRE>
*******************************************************************************/
static int BuildGMM(GMM * pGMM, GMM * Out, double (*X)[D], int train_frame_num, int m)
{
	int i;
	int j;
	int k;
	GMM temp_gmm = *Out;
	double ** p = NULL;      // train_frame_num个m组的事后概率
	double * sum_p = NULL;
	double * pb = NULL;      // 计算每个向量事后概率的中间量
	double sum_pb = 0;


	pb = (double *)calloc(m, sizeof(double));
	sum_p = (double *)calloc(m, sizeof(double));
	p = (double **)malloc(train_frame_num * sizeof(double *));
	for (i = 0; i < train_frame_num; ++i)
	{
		p[i] = (double *)calloc(m, sizeof(double));
	}
	// 计算事后概率
	for (i = 0; i < train_frame_num; ++i)
	{
		sum_pb = 0;
		for (j = 0; j < m; ++j)
		{
			pb[j] = pGMM->p[j] * GMM_density(pGMM, X[i], j);
			sum_pb += pb[j];
		}
		if (sum_pb == 0 || _isnan(sum_pb))
		{
			printf("sum_ph = %lf", sum_pb);
			//goto ERROR_RETURN;
			return 0;
		}
		for (j = 0; j < m; ++j)
		{
			p[i][j] = pb[j] / sum_pb;
			sum_p[j] += p[i][j];
		}
	}

	// 计算下一次迭代的GMM
	for (j = 0; j < m; ++j)
	{
		// 计算加权系数P
		temp_gmm.p[j] = sum_p[j] / train_frame_num;

		// 计算均值和协方差矩阵
		for (i = 0; i < train_frame_num; ++i)
		{
			for (k = 0; k < D; ++k)
			{
				if (i == 0)
				{
					temp_gmm.u[j][k] = 0;
					temp_gmm.cMatrix[j][k] = 0;
				}
				temp_gmm.u[j][k] += p[i][j] * X[i][k];
				temp_gmm.cMatrix[j][k] += p[i][j] * pow(X[i][k], 2);
			}
		}
		for (k = 0; k < D; ++k)//使用不使用单位矩阵初始化
		{
			// 均值u
			temp_gmm.u[j][k] /= sum_p[j];
			// 协方差矩阵cMatrix
			temp_gmm.cMatrix[j][k] /= sum_p[j];
			temp_gmm.cMatrix[j][k] -= pow(temp_gmm.u[j][k], 2);
			if (temp_gmm.cMatrix[j][k]<1e-6)//(temp_gmm.cMatrix[j][k] <= 0)
			{
				// 				printf("BuildGMM: cMatrix <= 0");
				// ERROR_RETURN:				
				// 				for (i = 0; i < train_frame_num; ++i)
				// 				{
				// 					free(p[i]);
				// 				}
				// 				free(p);
				// 				free(pb);
				// 				free(sum_p);
				// 				return 0;
				temp_gmm.cMatrix[j][k]=1e-4;
			}
		} // for (k = 0; k < D; ++k)
	}

	for (i = 0; i < train_frame_num; ++i)
	{
		free(p[i]);
	}
	free(p);
	free(pb);
	free(sum_p);
	return 1;
}


/** @function
********************************************************************************
<PRE>
函数名: GMMs()
功能: GMM建模过程
用法:
参数:
[IN] X: 特征向量组
[OUT] pGMM: GMM模型
[IN] train_frame_num: 训练帧帧数
[IN] m: 高斯混合数
返回:
1: 成功
0: 失败
调用: BuildGMM(), GMM_density()
主调函数:
</PRE>
*******************************************************************************/
int GMMs(double (*X)[D], GMM * pGMM, int train_frame_num, int m)
{
	GMM first_gmm = {InitGMMClass};
	GMM second_gmm = {InitGMMClass};
	GMM * first = &first_gmm;
	GMM * second = &second_gmm;
	GMM * temp = NULL;
	int bRes;
	double diff = 0;


	first_gmm.f_Init(&first_gmm, m);
	first_gmm.f_CallocGMM(&first_gmm);
	second_gmm.f_Init(&second_gmm, m);
	second_gmm.f_CallocGMM(&second_gmm);
	// TODO 初值的对角阵有0值和负值
	if (!InitGMM(X, first, train_frame_num, m)) // 获得GMM初值失败
	{
		return 0;
	}

	do // GMM迭代过程
	{
		bRes = BuildGMM(first, second, X, train_frame_num, m);
		if (!bRes)
		{
			first->f_FreeGMM(first);
			second->f_FreeGMM(second);
			return 0;
		}
		temp = first;
		first = second;
		second = temp;
		diff = GMM_diff(first, second);

		printf("diff = %lf\n", diff);
	}
	while (diff >= DIFF_GMM_VALUE);

	first->f_FreeGMM(first);
	*pGMM = *second;
	return 1;
}


/** @function
********************************************************************************
<PRE>
函数名: GMM_identify()
功能: 计算后验概率，声音识别过程
用法:
参数:
[IN] X: 特征向量组
[OUT] value: 识别值
[IN] pGMM: GMM模型
[IN] frame_num: 识别帧数
[IN] m: GMM模型的高斯混合数
返回:
成功: 1
失败: 0
调用: GMM_density()
主调函数:
</PRE>
*******************************************************************************/
int GMM_identify(double (*X)[D], double * value, GMM * pGMM, int frame_num, int m)
{
	int j;
	int k;
	double temp = 0;
	double * pb = NULL;
	double sum_pb = 0.0;


	pb = (double *)calloc(m, sizeof(double));
	for (j = 0; j < frame_num; ++j)
	{

		for (k = 0; k < m; k++)
		{
			pb[k] = pGMM->p[k] * GMM_density(pGMM, X[j], k);
			sum_pb += pb[k];
		}
		// TODO sum_pb==0导致负无穷-nan的temp_max
		if (sum_pb <= 0)
		{
			printf("后验概率sum_pb <= 0");
			return 0;
		}
		else
		{
			temp += log(sum_pb);
		}
	}
	//printf("max = %lf  ", temp);

	*value = temp;
	free(pb);
	return 1;
}

void GMMCalloc(GMM* pGmm,int m)
{
	GMM first_gmm={InitGMMClass};
	GMM* first=&first_gmm;
	GMM* temp=NULL;
	double diff=0.0;

	first_gmm.f_Init(&first_gmm,m);
	first_gmm.f_CallocGMM(&first_gmm);
	*pGmm=*first;
}

double* str2doubles(char* p)//获取的(之前double型数组写入的（空格隔开的）)字符串组转化为double型数组
{
	char* temp=(char*)calloc(15,sizeof(char));
	vector<double> d;
	d.clear();
	int i=0;
	int j=0;
	while(p[i]!='\0')//读到最后
	{
		if(p[i]!=' ')
			temp[j++]=p[i++];
		else
		{
			temp[j]='\0';
			double t=atof(temp);
			d.push_back(t);
			j=0;
			i++;
		}
	}
	free(temp);
	int s=d.size();
	double* res=(double*)calloc(s*15,sizeof(double));
	for(i=0;i<s;i++)
		res[i]=d[i];
	return res;
}

double** ToMatrix(double* p,int m,int d)//获取的(之前二维数组写入的（空格隔开的）)字符串组转化为二维数组
{
	double** res=(double**)calloc(m,sizeof(double*));
	for(int i=0;i<d;i++)
	{
		res[i]=(double*)calloc(d,sizeof(double));
		if(!res[i])//分配内存失败，把已分配的释放
		{
			for(--i;i>=0;--i)
				free(res[i]);
			return NULL;
		}
	}
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<d;j++)
			res[i][j]=p[d*i+j];
	}
	return res;
}

void GMM2Txt(ofstream& f,GMM* p,int d)//写入文件，d为MFCC维数
{
	int m=p->m;
	f<<m<<'\n';
	int i=0;
	int j=0;
	for(i=0;i<m;i++)
		f<<p->p[i]<<' ';
	f<<'\n';
	for(i=0;i<m;i++)
	{
		for(j=0;j<d;j++)
			f<<p->u[i][j]<<' ';
	}
	f<<'\n';
	for(i=0;i<m;i++)
	{
		for(j=0;j<d;j++)
			f<<p->cMatrix[i][j]<<' ';
	}
	f<<'\n';

}

void load(ifstream& f,GMM* p)
{
	int i=0;
	while(f.peek()!=EOF&&i<10)
	{
		char c[3];
		f.getline(c,3);
		int n=atoi(c);
		p[i].m=n;

		char* str=(char*)malloc(n*20*sizeof(char));
		if(str==NULL)
		{
			AfxMessageBox(_T("malloc failed！"));
			return;
		}
		f.getline(str,n*20);
		//str=(char*)realloc(str,f.gcount()*sizeof(char));
		p[i].p=str2doubles(str);
		free(str);
		str=NULL;

		char* u=(char*)calloc(n*20*D,sizeof(char));//二维数组的总长度，n*20 * D维
		if(u==NULL)
		{
			AfxMessageBox(_T("calloc failed！"));
			return;
		}
		f.getline(u,n*20*D+2);
		if(u==NULL)
		{
			AfxMessageBox(_T("getline failed！"));
			return;
		}
		//u=(char*)realloc(u,f.gcount()*sizeof(char));
		double* t=str2doubles(u);
		p[i].u=ToMatrix(t,n,D);
		free(u);
		u=NULL;
		
		//mark  error  cMatrix赋值有问题（初始化时维数弄错了）
		char* Matrix=(char*)calloc(n*20*D,sizeof(char));//二维数组的总长度，n*20 * D维
		f.getline(Matrix,n*20*D+2);
		if(Matrix==NULL)
		{
			AfxMessageBox(_T("Matrix failed！"));
			return;
		}
		//Matrix=(char*)realloc(Matrix,f.gcount()*sizeof(char));
		double* q=str2doubles(Matrix);
		//p[i].cMatrix=mat;
		p[i].cMatrix=ToMatrix(q,n,D);
		free(Matrix);
		Matrix=NULL;
		++i;
	}
}

//#ifdef DEBUG_GMM
//int main(void)
//{
//
//	GMM gmm;
//	double MFCC[9][2] = {{1, 1}, {1, 1.5}, {1.5, 1},
//	{3, 3}, {3.5, 3}, {3, 3.5},
//	{5, 0}, {5, 0.5}, {5.5, 0}
//	};
//	gmm = GMMs(MFCC);
//
//	return 0;
//}
//#endif //DEBUG_GMM