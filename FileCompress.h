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

		/*itoa(chSize >> 32, str, 10);
		fputs(str, FileCon);
		fputc('\n', FileCon);*/

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
			fputs(Inconfig.c_str(), FileCon);
			Inconfig.clear();
		}

		fclose(fout);
		fclose(FileIn);
		fclose(FileCon);
		return true;
	}
	void UnComPress(const char* filename)
	{
		//��һ�ֶ�ȡ----��ȡ�����ļ����õ��ַ����ֵĴ���
		//�ڶ��ж�ȡ----���ݸ��ڵ��Ȩֵ�õ��ַ����ֵ��ܴ������Ѵﵽ���ѹ�ļ����ַ�����ͬ��
		
		string configFile = filename;
		configFile += ".con";
		FILE* foutconfig = fopen(configFile.c_str(), "r");
		assert(foutconfig);

		string line;
		long long chSize = 0;
		/*ReadLine(foutconfig,line);
		chSize = atoi(line.c_str());
		chSize << 32;
		line.clear();*/

		ReadLine(foutconfig, line);
		chSize += atoi(line.c_str());
		line.clear();

		while (ReadLine(foutconfig, line))
		{
			if (!line.empty())
			{
				unsigned char ch = line[0];
				_info[ch]._count=atoi(line.substr(2).c_str());
				line.clear();
			}
			else
			{
				line += '\n';
			}
		}
		//�ؽ���������
		CharInfo invalid;
		HuffManTree<CharInfo> Tree(_info, 256, invalid);

		//��ȡѹ���ļ�
		string compressfilename = filename;
		compressfilename += ".HuffMan";
		FILE* FileCom = fopen(compressfilename.c_str(), "rb");
		assert(FileCom);

		string uncompressfilename = filename;
		uncompressfilename += ".uncomp";
		FILE* FileUnCom = fopen(uncompressfilename.c_str(), "wb");
		assert(FileUnCom);

		char ch = fgetc(FileCom);
		HuffManTreeNode<CharInfo>* root = Tree.GetRoot();
		HuffManTreeNode<CharInfo>* cur = root;

		int pos = 8;
		while (1)
		{
			if (cur->_left==NULL && cur->_right==NULL)
			{
				fputc(cur->_weight._ch, FileUnCom);
				cur = root;
				if (--chSize==0)
				{
					break;
				}
			}
			--pos;
			if (ch & (1<<pos))
				cur = cur->_right;
			else
				cur = cur->_left;
			if (pos==0)
			{
				ch = fgetc(FileCom);
				pos = 8;
			}
		}

		fclose(FileCom);
		fclose(FileUnCom);
		fclose(foutconfig);
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
	bool ReadLine(FILE *fOut, string &line)
	{
		char ch = fgetc(fOut);
		if (ch == EOF)
			return false;
		while (ch!=EOF && ch !='\n')
		{
			line += ch;
			ch = fgetc(fOut);
		}
		return true;
	}
protected:
	CharInfo _info[256];
};

void TestCompress()
{
	FileCompress Fc;
	Fc.ComPress("InPut");
}
void TestUnCompress()
{
	FileCompress Fc;
	Fc.UnComPress("InPut");
}