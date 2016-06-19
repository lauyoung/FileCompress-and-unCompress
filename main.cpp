#include "FileCompress.h"
#include "HuffmanTree.h"
#include <windows.h>

//补位完的解压缩文件处理，防止多余解析
//大文件的一部分解压
int main()
{
	int begin = GetTickCount();
	TestCompress();
	int end = GetTickCount();
	cout << "压缩所用的时间" << (end - begin)/1000<<"秒" << endl;


	TestUnCompress();
	return 0;
}