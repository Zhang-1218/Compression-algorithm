#pragma once

#include<cstdio>
#include<cstring>
#include<string>
#include"huffmantree.h"

//为防止数据个数超过整形的最大值，采用long long类型
typedef long long longtype;

struct CharInfo
{
	//防止_ch超过125(-1),采用无符号
	unsigned char _ch;//字符信息
	longtype _count;  //出现次数
	string _code;	  //huffman编码

	CharInfo(const longtype count = 0)
		:_count(count)
	{}

	bool operator!=(const CharInfo& info)
	{
		return _count != info._count;
	}

	bool operator < (const CharInfo& info)
	{
		return _count < info._count;
	}

	CharInfo operator +(const CharInfo& info)
	{
		return _count + info._count;
	}
};

class FileCompression
{
	typedef HuffmanTreeNode<CharInfo> Node;
	
	/*
		TempCharInfo:用来将源文件的_ch(字符)和_count(次数)传入压缩文件
		此处不传入_code(哈夫曼编码)：
		1) string类型的_count太大，传入也用不到，还会造成空间的浪费，也会降低效率
		2) 通过二进制写入压缩文件中，这样是值传递，再次读出来使用的时候，程序会崩溃，因为访问了非法空间。
	*/
	struct TempCharInfo{
		unsigned char _ch;
		longtype _count;
	};
public:
	// structure
	FileCompression();

	// Compress:test.txt -> test.huffman
	void Compress(const char* filename);

	// unCompress:test.hunffman -> test.unhuffman
	void unCompress(const char* filename);	

	void PrintCode()const;

protected:
	// generate huffmantreecode
	void GenerateHuffmanCode(const Node* root, string& code);
	void GenerateHuffmanCode(const Node* root);
private:
	CharInfo _infos[256];
};

