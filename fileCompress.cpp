#define _CRT_SECURE_NO_WARNINGS

#include"huffmantree.h"
#include"fileCompress.h"

FileCompression::FileCompression()
{
	//��0-256�������ַ���������ȥ�Խ��бȶ�
	for (size_t i = 0; i < 256; ++i)
	{
		_infos[i]._ch = i;
		_infos[i]._count = 0;
		_infos[i]._code = " ";
	}
}

// Compressѹ����input.txt -> input.huffman
void FileCompression::Compress(const char* filename)
{
	assert(filename);
	//fopen��Ϊ�����Ʒ�ʽ���ļ�
	//������ı���ʽ�������ַ�'\0'�ᵱ���������ţ�����ʧ��
	//FILE* fout = fopen(filename, "r");
	FILE* fout = fopen(filename, "rb");
	if (fout == NULL)
	{
		cout << "fail to open the file��" << endl;
		return;
	}

	// 1.ͳ�Ƹ��ַ����ֵĴ���
	// int fgetc ( FILE * stream );
	int ch = fgetc(fout);
	while (ch != EOF)
	{
		//ǿת�����������ַ���ʱ��ch�Ǹ���
		//����_infos�ķ�Χ��0~256����ǿת�ᷢ��Խ��
		(_infos[(unsigned char)ch]._count)++;
		ch = fgetc(fout);
	}

	// 2.����HuffmantTree
	/*
	��Ӧ��ֻ��Ȩֵ��Ӧ����CharInfor
	256���ַ��в���ÿ���ַ������õ���ֻ����Ч�ַ�����������HuffmanTree,�����趨�Ƿ�ֵ
	*/
	CharInfo invalid;
	invalid._count = 0;
	HuffmanTree<CharInfo> tree(_infos, 256, invalid);

	// 3.���ɹ���������
	// ע�⣬code���ܳ�ʼ���������������
	string code;
	GenerateHuffmanCode(tree.GetRoot(), code);

	// 4.ѹ���ļ�
	string compressFile = filename;
	//�����ļ�����
	//rfind():Find last occurrence of content in string
	//������ң����ҳɹ�������λ�ã�����ʧ�ܣ�����string::npos��static const size_t npos = -1��
	size_t spot = compressFile.rfind('.');
	assert(spot != string::npos);
	compressFile.erase(spot);
	compressFile += ".huffman";

	//const char* c_str() const;
	//Get C string equivalent(�õ�C�ַ�����Ч)
	FILE* fin = fopen(compressFile.c_str(), "wb");
	assert(fin);

	//д����Ϣ
	for (int i = 0; i < 256; ++i)
	{
		if (_infos[i]._count != 0)
		{
			TempCharInfo info;
			info._ch = _infos[i]._ch;
			info._count = _infos[i]._count;
			//������д��
			fwrite(&info, sizeof(info), 1, fin);
		}
	}

	//���Դ�ļ��ַ��͸��������������ȡֹͣ
	TempCharInfo info;
	info._count = -1;
	//size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
	fwrite(&info, sizeof(info), 1, fin);

	//ԭ�ļ�ָ���Ѿ�����β�ˣ�Ҫ����ָ���ļ���ʼλ��
	fseek(fout, 0, SEEK_SET);
	ch = fgetc(fout);

	//valueΪҪд�������
	//����Ӧλ��codeΪ1ʱ������λ�޸�Ϊ1
	char value = 0;
	//pos�����޸Ķ�Ӧλ��ͬʱ��Ϊ��־����posΪ8��ʱ��,��ʾ�Ѿ����һ���ֽڵ��޸ģ���ʱ��valueд��ѹ���ļ�
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

			//ֻ�ܵ����ֽ�д��
			if (pos == 8)
			{
				fputc(value, fin);
				value = 0;
				pos = 0;
			}
		}
		ch = fgetc(fout);
	}
	// ���ļ���д��ɣ���������value����8λ����ʱ��Ҫ����
	if (pos > 0)
	{
		fputc(value, fin);
	}
	fclose(fout);
	fclose(fin);
}

// unCompress ��ѹ����input.huffman -> input.unhuffman
void FileCompression::unCompress(const char* filename)
{
	assert(filename);
	//�޸�����
	string uncompressFile = filename;
	size_t pos = uncompressFile.rfind('.');
	assert(pos != string::npos);
	uncompressFile.erase(pos);
	uncompressFile += ".unhuffman";

	// 1.��ԭ�ļ���ȡ�ַ���Ϣ���ַ�����ִ�����
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

	// 2.�ؽ�huffmantree����ѹʱ���Բο������������Ӷ���ȡ�ַ���ϸ��Ϣ
	CharInfo invalid;
	invalid._count = 0;
	HuffmanTree<CharInfo> tree(_infos, 256, invalid);

	Node* root = tree.GetRoot();
	Node* cur = root;
	//����huaffmantree�ص㣬���ڵ�Ȩֵ(�˴�Ϊfilesize)���������ַ����ֵĴ���֮�ͣ��Դ�����Ϊ��ѹ�Ľ���
	longtype filesize = root->_weight._count;

	// 3.��ѹ��
	int ch = fgetc(fout);
	//����������ļ�֮��ֻ��һ���ַ�
	if (root->_left == NULL)
	{
		while (filesize--)
		{
			fputc(root->_weight._ch, fin);
		}
	}
	//�ļ���������ַ�
	else
	{
		while (ch != EOF)
		{
			for (size_t i = 0; i < 8; ++i)
			{
				//�ұ�Ϊ1
				if ((unsigned char)ch & (1 << i))
				{
					cur = cur->_right;
				}
				//���Ϊ0
				else
				{
					cur = cur->_left;
				}
				if (cur->_left == NULL && cur->_right == NULL)
				{
					//����ѹ�ַ������ﵽѹ���ַ�����filesizeʱ��֤����ѹ��ϣ�����ѭ��
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
			// ������Ϊ0
			if (cur == parent->_left)
				code.push_back('0');
			// ������Ϊ1
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