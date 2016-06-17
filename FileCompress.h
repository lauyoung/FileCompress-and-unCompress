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
	unsigned char _ch;			//字符
	LongType _count;				//字符出现次数
	string _code;				//字符对应的哈夫曼编码

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

		//1,统计文件中字符出现的次数；
		long long chSize = 0;
		char ch = fgetc(fout);
		while (ch!=EOF)
		{
			chSize++;
			_info[(unsigned char)ch]._count++;
			ch = fgetc(fout);
		}

		//2,构建哈夫曼树
		CharInfo invalid;		//过滤没出现的字符，没出现的字符不需要构建哈弗曼树
		HuffManTree<CharInfo> Tree(_info,256,invalid);

		//3,生成哈夫曼编码
		string code;
		GenerateTree(Tree.GetRoot(), code);

		//4,写入压缩文件
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
					value |= 1;  //最低位制1

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
		//补位操作
		if (size > 0)
		{
			value <<= (7 - size);
			fputc(value, FileIn);

		}
		//写出配置文件供解压缩，内容是；字符。次数
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
		//读取配置文件，得到字符出现的次数
		//根据根节点的权值得到字符出现的总次数，已达到与解压文件，字符数相同。
		//重建哈弗曼树
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
