#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
using namespace std;
ifstream fin("text.txt");
//static bool mark[10];//自动初始化全零
CRITICAL_SECTION cs[10];
static bool closed(false);
static int** data;

void mem()
{
	data=(int**)calloc(100,sizeof(int*));
	for(int i=0;i<10;i++)
		data[i]=(int*)calloc(1000,sizeof(int));
}

void ReadIn()
{
	//InitializeCriticalSection(&cs);
	string str;
	while(fin.peek()!=EOF)
	{
		for(int i=0;i<10;++i)
		{
			EnterCriticalSection(&cs[i]);
			for(int j=0;j<1000;j++)
			{
				getline(fin,str);
				data[i][j]=atoi(str.c_str());
			}
			LeaveCriticalSection(&cs[i]);
		}
	}
	closed=true;
}

void process()
{
	ofstream fout("mytest.txt");
	while(!closed)
	{
		for(int i=0;i<10;++i)
		{
			EnterCriticalSection(&cs[i]);
			for(int j=0;j<1000;j++)
			{
				//if(j!=0&&j%50==0)
				//	cout<<endl;
				//cout<<data[i][j]<<" ";
				fout<<data[i][j]<<endl;
			}
			LeaveCriticalSection(&cs[i]);
		}
	}
}


int main()
{
	mem();
	for(int i=0;i<10;i++)
		InitializeCriticalSection(&cs[i]);
	CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)ReadIn, 0, 0, 0);
	CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)process, 0, 0, 0);
	Sleep(1000);
	//system("pause");
	return 0;
}