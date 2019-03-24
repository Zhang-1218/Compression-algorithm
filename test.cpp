#define _CRT_SECURE_NO_WARNINGS 

#include"fileCompress.h"
#include"TimeStatistics.h"

void FileCompressTest()
{
	TimeStatistics time;
	FileCompression f1;
	FileCompression f2;

	time.StartTime();
	f1.Compress("C:\\code\\Code\\Data Structure\\File_Compression\\test\\zhang.txt");
	time.ShowCompressTime();

	time.StartTime();
	f2.unCompress("C:\\code\\Code\\Data Structure\\File_Compression\\test\\zhang.huffman");
	time.ShowunCompressTime();

	//time.StartTime();
	//f1.Compress("C:\\code\\Code\\Data Structure\\File_Compression\\test\\code.cpp");
	//time.ShowCompressTime();

	//time.StartTime();
	//f1.unCompress("C:\\code\\Code\\Data Structure\\File_Compression\\test\\code.huffman");
	//time.ShowunCompressTime();

	//time.StartTime();
	//f1.Compress("C:\\code\\Code\\Data Structure\\File_Compression\\test\\“¡‹ΩºŒµ«.jpg");
	//time.ShowCompressTime();

	//time.StartTime();
	//f1.unCompress("C:\\code\\Code\\Data Structure\\File_Compression\\test\\“¡‹ΩºŒµ«.huffman");
	//time.ShowunCompressTime();


}

int main()
{
	FileCompressTest();
	system("pause");
	return 0;
}