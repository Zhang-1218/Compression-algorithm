#pragma once

#include<iostream>
#include<Windows.h>

using namespace std;

class TimeStatistics
{
public:
	TimeStatistics()
		:start_time(0.0)
	{}

	void StartTime()
	{
		start_time = GetTickCount();
	}
	
	void ShowCompressTime()
	{
		cout << "ѹ��ʱ��: " << GetTickCount() - start_time << "ms" << endl;
	}

	void ShowunCompressTime()
	{
		cout << "��ѹ��ʱ��: " << GetTickCount() - start_time << "ms" << endl;
	}

private:
	DWORD start_time;
};
