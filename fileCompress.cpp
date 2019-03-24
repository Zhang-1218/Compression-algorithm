#define _CRT_SECURE_NO_WARNINGS

#include"huffmantree.h"
#include"fileCompress.h"

FileCompression::FileCompression()
{
	//将0-256的所有字符都包含进去以进行比对
	for (size_t i = 0; i < 256; ++i)
	{
		_infos[i]._ch = i;
		_infos[i]._count = 0;
		_infos[i]._code = " ";
	}
}

// Compress压缩；input.txt -> input.huffman
void FileCompression::Compress(const char* filename)
{
	assert(filename);
	//fopen改为二进制方式打开文件
	//如果是文本格式，遇到字符'\0'会当作结束符号，解析失败
	//FILE* fout = fopen(filename, "r");
	FILE* fout = fopen(filename, "rb");
	if (fout == NULL)
	{
		cout << "fail to open the file！" << endl;
		return;
	}

	// 1.统计各字符出现的次数
	// int fgetc ( FILE * stream );
	int ch = fgetc(fout);
	while (ch != EOF)
	{
		//强转：遇到中文字符的时候，ch是负数
		//但是_infos的范围是0~256，不强转会发生越界
		(_infos[(unsigned char)ch]._count)++;
		ch = fgetc(fout);
	}

	// 2.构建HuffmantTree
	/*
	不应该只有权值，应该是CharInfor
	256个字符中不是每个字符都有用到，只有有效字符才用来构建HuffmanTree,所以设定非法值
	*/
	CharInfo invalid;
	invalid._count = 0;
	HuffmanTree<CharInfo> tree(_infos, 256, invalid);

	// 3.生成哈夫曼编码
	// 注意，code不能初始化，否则会编码错误
	string code;
	GenerateHuffmanCode(tree.GetRoot(), code);

	// 4.压缩文件
	string compressFile = filename;
	//更改文件名称
	//rfind():Find last occurrence of content in string
	//反向查找：查找成功，返回位置；查找失败，返回string::npos（static const size_t npos = -1）
	size_t spot = compressFile.rfind('.');
	assert(spot != string::npos);
	compressFile.erase(spot);
	compressFile += ".huffman";

	//const char* c_str() const;
	//Get C string equivalent(得到C字符串等效)
	FILE* fin = fopen(compressFile.c_str(), "wb");
	assert(fin);

	//写入信息
	for (int i = 0; i < 256; ++i)
	{
		if (_infos[i]._count != 0)
		{
			TempCharInfo info;
			info._ch = _infos[i]._ch;
			info._count = _infos[i]._count;
			//二进制写入
			fwrite(&info, sizeof(info), 1, fin);
		}
	}

	//标记源文件字符和个数结束，方便读取停止
	TempCharInfo info;
	info._count = -1;
	//size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
	fwrite(&info, sizeof(info), 1, fin);

	//原文件指针已经读到尾了，要重新指向文件起始位置
	fseek(fout, 0, SEEK_SET);
	ch = fgetc(fout);

	//value为要写入的数据
	//当对应位的code为1时，将该位修改为1
	char value = 0;
	//pos用来修改对应位，同时作为标志：当pos为8的时候,表示已经完成一个字节的修改，此时将value写入压缩文件
	size_t pos = 0;

	while (ch != EOF)
	{
		string& code = _infos[(unsigned char)ch]._code;
		for (size_t i = 0; i < code.size(); ++i)
		{
			if (code[i] == '0')
			{
				value &= ~(1 << pos);
			}
			else if (code[i] == '1')
			{
				value |= (1 << pos);
			}
			else
			{
				assert(false);
			}
			++pos;

			//只能单个字节写入
			if (pos == 8)
			{
				fputc(value, fin);
				value = 0;
				pos = 0;
			}
		}
		ch = fgetc(fout);
	}
	// 当文件读写完成，但是最后的value不满8位，此时需要补齐
	if (pos > 0)
	{
		fputc(value, fin);
	}
	fclose(fout);
	fclose(fin);
}

// unCompress 解压缩：input.huffman -> input.unhuffman
void FileCompression::unCompress(const char* filename)
{
	assert(filename);
	//修改名称
	string uncompressFile = filename;
	size_t pos = uncompressFile.rfind('.');
	assert(pos != string::npos);
	uncompressFile.erase(pos);
	uncompressFile += ".unhuffman";

	// 1.从原文件获取字符信息（字符与出现次数）
	FILE* fin = fopen(uncompressFile.c_str(), "wb");
	assert(fin);
	FILE* fout = fopen(filename, "rb");
	assert(fout);
	TempCharInfo info;
	//size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
	fread(&info, sizeof(info), 1, fout);
	while (info._count != -1)
	{
		_infos[(unsigned char)info._ch]._ch = info._ch;
		_infos[(unsigned char)info._ch]._count = info._count;
		fread(&info, sizeof(info), 1, fout);
	}

	// 2.重建huffmantree：解压时可以参考哈夫曼树，从而获取字符详细信息
	CharInfo invalid;
	invalid._count = 0;
	HuffmanTree<CharInfo> tree(_infos, 256, invalid);

	Node* root = tree.GetRoot();
	Node* cur = root;
	//利用huaffmantree特点，根节点权值(此处为filesize)即是所有字符出现的次数之和，以此来作为解压的界限
	longtype filesize = root->_weight._count;

	// 3.解压缩
	int ch = fgetc(fout);
	//特殊情况：文件之中只有一个字符
	if (root->_left == NULL)
	{
		while (filesize--)
		{
			fputc(root->_weight._ch, fin);
		}
	}
	//文件包含多个字符
	else
	{
		while (ch != EOF)
		{
			for (size_t i = 0; i < 8; ++i)
			{
				//右边为1
				if ((unsigned char)ch & (1 << i))
				{
					cur = cur->_right;
				}
				//左边为0
				else
				{
					cur = cur->_left;
				}
				if (cur->_left == NULL && cur->_right == NULL)
				{
					//当解压字符个数达到压缩字符个数filesize时，证明解压完毕，跳出循环
					if (filesize-- == 0)
					{
						break;
					}
					fputc(cur->_weight._ch, fin);
					cur = root;
				}
			}
			ch = fgetc(fout);
		}
	}
	fclose(fin);
	fclose(fout);
}

void FileCompression::GenerateHuffmanCode(const Node* root, string& code)
{
	if (root == NULL)
		return;

	if (root->_left == NULL && root->_right == NULL)
	{
		_infos[(unsigned char)(root->_weight._ch)]._code = code;
		return;
	}
	GenerateHuffmanCode(root->_left, code + '0');
	GenerateHuffmanCode(root->_right, code + '1');
}

void FileCompression::GenerateHuffmanCode(const Node* root)
{
	if (root == NULL)
		return;

	if (root->_left == NULL && root->_right == NULL)
	{
		string& code = _infos[(unsigned char)root->_weight._ch]._code;

		Node* cur = (Node*)root;
		Node* parent = cur->_parent;
		while (parent)
		{
			// 左子树为0
			if (cur == parent->_left)
				code.push_back('0');
			// 右子树为1
			else
				code.push_back('1');
			cur = parent;
			parent = cur->_parent;
		}

		reverse(code.begin(), code.end());
		return;
	}
	GenerateHuffmanCode(root->_left);
	GenerateHuffmanCode(root->_right);
}

void FileCompression::PrintCode()const
{
	for (int i = 0; i < 256; ++i)
	{
		if (_infos[i]._count != 0)
		{
			cout << _infos[i]._ch << ":>" << _infos[i]._code << endl;
		}
	}
}