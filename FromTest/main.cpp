#include "gmm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "F:\VS2008\test\test\WaveIn.h"
#include <Windows.h>


using namespace std;
#define mPBYTE unsigned char*
mBYTE* pBuffer[10];
static int s=0;
static int m=0;
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
	s=d.size();
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

void GMMCalloc(GMM * pGMM,int m)//结构体内存分配，否则加载时会出错，没有空间
{
	GMM first_gmm = {InitGMMClass};
	GMM * first = &first_gmm;
	GMM * temp = NULL;
	double diff = 0;


	first_gmm.f_Init(&first_gmm, m);
	first_gmm.f_CallocGMM(&first_gmm);
	*pGMM = *first;

}


void load(ifstream& f,GMM* p)//加载结构体组
{
	int i=0;
	while(f.peek()!=EOF)//要读取的字符为结束符是跳出循环
	{
		char c[3];
		f.getline(c,3);//GMM混合数
		int n=atoi(c);
		p[i].m=n;
				
		char* str=(char*)calloc(n*20,sizeof(char));//double精度为15-16，要留有空余
		f.getline(str,n*20);
		str=(char*)realloc(str,f.gcount()*sizeof(char));
		p[i].p=str2doubles(str);

		char* u=(char*)calloc(n*20*D,sizeof(char));//二维数组的总长度，n*20 * D维
		f.getline(u,n*20*D);
		u=(char*)realloc(u,f.gcount()*sizeof(char));
		double* t=str2doubles(u);
		p[i].u=ToMatrix(t,n,D);

		char* cMatrix=(char*)calloc(n*20*D,sizeof(char));//二维数组的总长度，n*20 * D维
		f.getline(cMatrix,n*20*D);
		cMatrix=(char*)realloc(cMatrix,f.gcount()*sizeof(char));
		t=str2doubles(cMatrix);
		p[i].cMatrix=ToMatrix(t,n,D);

		++i;
	}
}



#ifndef DEBUG_GMM
int main(int argc,char* argv[])
{
	enum {num = 10};
	int i;
	int j;
	int k;
	int res = 0;
	FILE * fi =NULL;
	//char path[40];
	static double MFCC[num][GOOD_FRAME_NUM][D];
	//double ** MFCC[num] = {NULL};   //用来存储MFCC值
	mPBYTE pRawVoice = NULL;   //指向原始语音数据
	double value=0.0;
	double max=0.0;
	size_t len=0;


	////动态分配内存
	//int kinds,total;
	//cin>>total>>kinds;
	//bool mark(true);
	//while(mark)//内存分配，失败则重新分配
	//{
	//	double*** mymfcc=(double***)malloc(total*sizeof(double**));//total*GOOD*D
	//	if(mymfcc==NULL)
	//		continue;
	//	for(int i=0;i<total;i++)
	//	{
	//		mymfcc[i]=(double**)malloc(GOOD_FRAME_NUM*sizeof(double*));
	//		if(mymfcc[i]==NULL)
	//		{
	//			for(int j=0;i<i;j++)
	//				free(mymfcc[j]);
	//			mark=false;

	//		}
	//		else
	//		{
	//			for(int j=0;j<GOOD_FRAME_NUM;j++)
	//			{
	//				mymfcc[i][j]=(double*)malloc(D*sizeof(double));
	//				if(mymfcc[i][j]==NULL)
	//				{
	//					for(int k=i;k>=0;k--)
	//					{
	//						for(int l=j-1;l>=0;l--)
	//							free(mymfcc[k][l]);
	//					}
	//				}
	//			}
	//		}

	//	}
	//}






	//int NumOfGuss=atoi(argv[1]);
	//char* fileName=argv[2];//文件路径不包含汉字，否则打开不了。用txt方式存放文件路径
	///*char* ModuleDir=argv[3];
	//ofstream fout(ModuleDir);
	//if(!fout)
	//{
	//	cout<<"写入文件打开失败！"<<endl;
	//	return -1;
	//}*/

	//cout<<"声音数据读取..."<<endl;
	//string str;
	//ifstream fDir(fileName);
	//int count=0;
	//getline(fDir,str);
	//FILE* f=NULL;
	//while(fDir&&str!="")//加上str!="",是为了防止多打换行符在文件结尾
	//{
	//	f = fopen(str.c_str(), "rb");
	//	if(!f)
	//	{
	//		cout<<"读取文件打开失败！"<<endl;
	//		return -1;
	//	}
	//	mPBYTE pRawVoice = (mPBYTE)calloc(GOOD_FRAME_NUM, FRAME_LEN);
	//	int len=fread(pRawVoice, 1, GOOD_FRAME_NUM * FRAME_LEN, f);
	//	fclose(f);

	//	/*ofstream fdata("datatest.txt");//声音数据写入文件，进行显示测试的
	//	for(int i=84;i<len;i++)
	//	{
	//		double t=(pRawVoice[i]-128)*0.0078125;
	//		fdata<<t<<" ";
	//	}*/
	//	if (!voiceToMFCC(pRawVoice, GOOD_FRAME_NUM * FRAME_LEN, MFCC[count], GOOD_FRAME_NUM))
	//	{
	//		// free(pRawVoice);
	//		//             #ifdef _DEBUG
	//		free(pRawVoice);
	//		return 0;
	//		/*             printf("\nMFCC[%d]:\n", i);*/
	//		//             for (j = 0; j < 10; ++j)
	//		//             {
	//		//                 for (k = 0; k < 1; ++k)
	//		//                 {
	//		//                     printf("%lf  ",MFCC[i][j][k]);
	//		//                 }
	//		//                 printf("\n");
	//		//             }
	//		//             #endif // _DEBUG

	//		/*GMMs(MFCC[i], gmm+i,GOOD_FRAME_NUM,16);
	//		GMM2Txt(fout,gmm+i,D);*/
	//	}
	//	count++;
	//	getline(fDir,str);
	//}

	//mBYTE* pBuffer[10];
	for(int i=0;i<10;i++)
		pBuffer[i]=new mBYTE[BUFFER_SIZE];
	HANDLE handle=CreateThread(NULL,0,waveIn,NULL,0,NULL);
	WaitForSingleObject(handle, INFINITE);
	//waveIn();
	for(int i=0;i<num;i++)
	{
		if(!voiceToMFCC(pBuffer[i],BUFFER_SIZE,MFCC[i],GOOD_FRAME_NUM))
		{
			for(int j=i;j>=0;j--)
				free(pBuffer[j]);
			return -1;
		}
	}

	cout<<"读取成功！"<<endl;
	int frames=atoi(argv[3]);


	int n;//高斯混合数
	cin>>n;
	m=n;
	GMM gmm[num];
	for(i=0;i<num;++i)
		GMMCalloc(gmm+i,m);
	/*ofstream fout("test.txt");
	if(!fout)
	{
		cout<<"文件打开失败！"<<endl;
		return -1;
	}*/
	//for (i = 0; i < num; ++i)
	//{
	//	sprintf(path, "E://声音//CAR wav//HORN%d.WAV", i);
	//	fi = fopen(path, "rb");
	//	pRawVoice = (mPBYTE)calloc(GOOD_FRAME_NUM, FRAME_LEN);
	//	len=fread(pRawVoice, 1, GOOD_FRAME_NUM * FRAME_LEN, fi);
	//	// 		if(len!=GOOD_FRAME_NUM*FRAME_LEN)
	//	// 			printf("break\n"); 
	//	fclose(fi);

	//	if (!voiceToMFCC(pRawVoice, GOOD_FRAME_NUM * FRAME_LEN, MFCC[i], GOOD_FRAME_NUM))
	//	{
	//		// free(pRawVoice);
	//		//             #ifdef _DEBUG
	//		free(pRawVoice);
	//		return 0;
	//		/*             printf("\nMFCC[%d]:\n", i);*/
	//		//             for (j = 0; j < 10; ++j)
	//		//             {
	//		//                 for (k = 0; k < 1; ++k)
	//		//                 {
	//		//                     printf("%lf  ",MFCC[i][j][k]);
	//		//                 }
	//		//                 printf("\n");
	//		//             }
	//		//             #endif // _DEBUG

	//		/*GMMs(MFCC[i], gmm+i,GOOD_FRAME_NUM,16);
	//		GMM2Txt(fout,gmm+i,D);*/
	//	}
	//} //end  for (i = 0; i < num; ++i)


	cout<<"模型载入，识别中..."<<endl;
	ifstream fin("test.txt");
	if(!fin)
	{
		cout<<"文件打开失败！";
		return -1;
	}
	load(fin,gmm);
	fin.close();

	for(k=0;k<num;k++)
	{
		max=0.0;
		j=0;
		for(i=0;i<num;i++)
		{
			res = GMM_identify(MFCC[k],&value,&gmm[i], GOOD_FRAME_NUM,16);
			if(value>max)
			{
				max=value;
				j=i;
			}
		}
		printf("\nvoice[%d] with GMM[%d] identified!\n",k, j);
	}
	//sprintf(path, "E://声音//CAR wav//HORN%d.WAV", 10);
	//fi=fopen(path,"rb");
	//pRawVoice=(mPBYTE)calloc(GOOD_FRAME_NUM,FRAME_LEN);
	//fread(pRawVoice,1,GOOD_FRAME_NUM*FRAME_LEN,fi);
	//fclose(fi);
	//voiceToMFCC(pRawVoice, GOOD_FRAME_NUM * FRAME_LEN, MFCC[10], GOOD_FRAME_NUM);
	//j=-1;
	//max=0.0;
	//for(k=0;k<num-1;k++)
	//{
	//	res=GMM_identify(MFCC[10],&value,&gmm[k],GOOD_FRAME_NUM,16);
	//	if(value>max)
	//	{
	//		max=value;
	//		j=k;
	//	}
	//}
	//printf("\n%d with GMM[%d] identified !\n",10,j);

	////	sprintf(path, "F:\\wav\\cupwav0004.wav");
	////	fi = fopen(path, "rb");
	////	pRawVoice = (mPBYTE)calloc(320, FRAME_LEN);
	////	fread(pRawVoice, 1, 320 * FRAME_LEN, fi);
	////	fclose(fi);
	////
	////	if (voiceToMFCC(pRawVoice, 320 * FRAME_LEN, MFCC[1], 300))
	////	{
	////		free(pRawVoice);
	////#ifdef _DEBUG
	////		printf("\nGMM[%d]:\n", i);
	////		printf("\nMFCC[%d]:\n", i);
	////		for (j = 0; j < 10; ++j)
	////		{
	////			for (k = 0; k < 1; ++k)
	////			{
	////				printf("%lf  ",MFCC[i][j][k]);
	////			}
	////			printf("\n");
	////		}
	////#endif // _DEBUG
	////	}
	////	j=-1.0;
	////	for(i=0;i<num;i++)
	////	{
	////		res = GMM_identify(MFCC[1],&value,&gmm[i], 300,3);
	////		if(value>max)
	////		{
	////			max=value;
	////			j=i;
	////		}
	////	}
	////	if(j+1<1e-6)
	////		printf("no one is matched!\n");
	////	else
	////		printf("\nGMM[%d] identified!\n",j);
	////
	////    return 0;
}
#endif //DEBUG_GMM
