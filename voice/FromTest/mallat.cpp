#include <iostream>
using namespace std;

void Reverse(double* src,int len,double*& dst)
{
	if(src==NULL)
		return;
	for(int i=0;i<len;i++)
		dst[len-i-1]=src[i];
}
void mirror(double* src,int len,double*& dst)
{
	if(src==NULL)
		return;
	for(int i=0;i<len;i++)
	{
		if(i%2==0)
			dst[len-i-1]=-src[i];
		else
			dst[len-i-1]=src[i];
	}
}



int main()
{
	double src[5]={1.0,2.0,3.0,4.0,5.0};
	double* dst=(double*)calloc(5,sizeof(double)); 
	Reverse(src,5,dst);
	for(int i=0;i<5;i++)
		cout<<dst[i]<<" ";

	system("pause");
	return 0;
}