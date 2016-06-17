#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
#include "HuffmanTree.h"
using namespace std;


typedef unsigned long LongType;
struct CharInfo
{
	unsigned char _ch;			//�ַ�
	LongType _count;				//�ַ����ִ���
	string _code;				//�ַ���Ӧ�Ĺ���������

	CharInfo(LongType count=0)
		:_count(count)
	{}
	bool operator<(const CharInfo& info)const
	{
		return _count < info._count;
	}
	bool operator!=(const CharInfo& info)const 
	{
		return this->_count != info._count;
	}
	CharInfo operator+(const CharInfo& info)const 
	{
		return CharInfo(_count + info._count);
	}
};
class FileCompress
{
public:
	FileCompress()
	{
		for (size_t i = 0;i < 256;++i)
		{
			_info[i]._ch = i;
			_info[i]._count = 0;
		}
	}
	bool ComPress(const char* filename)
	{
		FILE* fout = fopen(filename, "r");
		assert(fout);

		//1,ͳ���ļ����ַ����ֵĴ�����
		long long chSize = 0;
		char ch = fgetc(fout);
		while (ch!=EOF)
		{
			chSize++;
			_info[(unsigned char)ch]._count++;
			ch = fgetc(fout);
		}

		//2,������������
		CharInfo invalid;		//����û���ֵ��ַ���û���ֵ��ַ�����Ҫ������������
		HuffManTree<CharInfo> Tree(_info,256,invalid);

		//3,���ɹ���������
		string code;
		GenerateTree(Tree.GetRoot(), code);

		//4,д��ѹ���ļ�
		string compressfilename = filename;
		compressfilename += ".HuffMan";
		FILE* FileIn = fopen(compressfilename.c_str(), "wb");
		assert(FileIn);


		fseek(fout, 0, SEEK_SET);
		ch = fgetc(fout);
		unsigned char value = 0;
		int size = 0;
		while (ch !=EOF)
		{
			string& code = _info[(unsigned char)ch]._code;
			for (size_t i = 0;i < code.size();++i)
			{
				if (code[i] == '1')
					value |= 1;  //���λ��1

				++size;
				if (size==8)
				{
					fputc(value, FileIn);
					value = 0;
					size = 0;
				}
				value <<= 1;
			}
			ch = fgetc(fout);
		}
		//��λ����
		if (size > 0)
		{
			value <<= (7 - size);
			fputc(value, FileIn);

		}
		//д�������ļ�����ѹ���������ǣ��ַ�������
		string configCompress = filename;
		configCompress += ".con";
		FILE* FileCon = fopen(configCompress.c_str(), "wb");
		assert(FileCon);

		char str[128];

		itoa(chSize >> 32, str, 10);
		fputs(str, FileCon);
		fputc('\n', FileCon);

		itoa(chSize, str, 10);
		fputs(str, FileCon);
		fputc('\n', FileCon);

		for (size_t i = 0;i < 256;++i)
		{
			string Inconfig;
			if (_info[i]._count > 0)
			{
				Inconfig += _info[i]._ch;
				Inconfig += ',';
				Inconfig += itoa(_info[i]._count, str, 10);
				Inconfig += '\n';
			}
			//Inconfig.clear();
			fputs(Inconfig.c_str(), FileCon);
		}

		fclose(fout);
		fclose(FileIn);
		fclose(FileCon);
		return true;
	}
	void UnComPress(const char* filename)
	{
		//��ȡ�����ļ����õ��ַ����ֵĴ���
		//���ݸ��ڵ��Ȩֵ�õ��ַ����ֵ��ܴ������Ѵﵽ���ѹ�ļ����ַ�����ͬ��
		//�ؽ���������
	}
protected:
	void GenerateTree(HuffManTreeNode<CharInfo>* root,string& code)
	{
		if (root == NULL)
			return;

		if (root->_left==NULL &&root->_right==NULL)
		{
			_info[root->_weight._ch]._code = code;
			return;
		}
		GenerateTree(root->_left, code+'0');
		GenerateTree(root->_right, code+'1');
	}
protected:
	CharInfo _info[256];
};
