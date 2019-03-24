#pragma once

#include<cstdio>
#include<cstring>
#include<string>
#include"huffmantree.h"

//Ϊ��ֹ���ݸ����������ε����ֵ������long long����
typedef long long longtype;

struct CharInfo
{
	//��ֹ_ch����125(-1),�����޷���
	unsigned char _ch;//�ַ���Ϣ
	longtype _count;  //���ִ���
	string _code;	  //huffman����

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
		TempCharInfo:������Դ�ļ���_ch(�ַ�)��_count(����)����ѹ���ļ�
		�˴�������_code(����������)��
		1) string���͵�_count̫�󣬴���Ҳ�ò�����������ɿռ���˷ѣ�Ҳ�ή��Ч��
		2) ͨ��������д��ѹ���ļ��У�������ֵ���ݣ��ٴζ�����ʹ�õ�ʱ�򣬳�����������Ϊ�����˷Ƿ��ռ䡣
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

