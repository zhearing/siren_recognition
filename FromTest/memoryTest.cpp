#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

int main()
{
	int** data=(int**)calloc(100,sizeof(int*));
	for(int i=0;i<100;i++)
		data[i]=(int*)calloc(1000,sizeof(int));
	
	ofstream fout("text.txt");
	for(int i=0;i<1000000;i++)
		fout<<i<<endl;
	fout.close();
	ifstream fin("text.txt");
	while(fin.peek()!=EOF)
	{
		string str;
		//循环读取，循环使用缓冲区
		for(int i=0;i<100;i++)
		{
			for(int j=0;j<1000;j++)
			{
				getline(fin,str);
				data[i][j]=atoi(str.c_str());
			}
		}
		for(int i=0;i<10;i++)
			cout<<data[0][i]<<" ";
		cout<<endl;

	}
	fin.close();
	system("pause");
	return 0;
}