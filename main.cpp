#include "FileCompress.h"
#include "HuffmanTree.h"
#include <windows.h>

//��λ��Ľ�ѹ���ļ�������ֹ�������
//���ļ���һ���ֽ�ѹ
int main()
{
	int begin = GetTickCount();
	TestCompress();
	int end = GetTickCount();
	cout << "ѹ�����õ�ʱ��" << (end - begin)/1000<<"��" << endl;


	TestUnCompress();
	return 0;
}