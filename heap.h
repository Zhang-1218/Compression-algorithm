#pragma once

#include<iostream>
#include<assert.h>
#include<vector>
#include<list>
#include<stack>
#include<queue>
using namespace std;

// �º���
// ���д�С�ıȽϣ���������Ӧ�Ĵ�С��
template<class T>
struct Less
{
	bool operator()(T& left, T& right)
	{
		return left < right; 
	}
};
template<class T>
struct Greater
{
	bool operator()(T& left, T& right)
	{
		return left > right;
	}
};

template<class T, class Compare=Greater<T>>
class Heap
{
public:
	// �˿չ��캯������ɾ
	// ���ǿն�ʱ�����ڳ�ʼ���б������vector��ȱʡ���캯�����г�ʼ��
	Heap()
	{}

	Heap(T* a, size_t n)
	{
		// ��ǰ���ٿռ䣬���ⲻ�ϵ����ݣ�����Ч��
		_array.reserve(n);

		for (size_t i = 0; i < n; i++)
		{
			_array.push_back(a[i]);
		}

		// ����
		// �����һ����Ҷ�ڵ㿪ʼ���е������˴���Ϊi
		for (int i = ((int)_array.size() - 2) >> 1; i >= 0; i--)
		{
			AdjustDown(i);
		}
	}

	void Push(const T& x)
	{
		// �������ݵ�vector
		_array.push_back(x);
		// �������ϵ���
		AdjustUp(_array.size() - 1);
	}

	void Pop()
	{
		// ����β�����н���
		swap(_array[_array.size() - 1], _array[0]);
		// ɾ��β�ڵ�
		_array.pop_back();
		// ͨ�����µ����ظ�����
		AdjustDown(0);
	}

	size_t Size()
	{
		return _array.size();
	}

	T& Top()
	{
		return _array[0];
	}

	bool Empty()
	{
		return _array.empty();
	}

	void PrintfHeap()
	{
		if (!Empty())
		{
			for (int i = 0; i < _array.size(); i++)
			{
				cout << _array[i] << " ";
			}
			cout << endl;
		}
	}

protected:
	// ���ϵ�����
	void AdjustUp(size_t child)
	{
		Compare com;
		size_t parent = (child - 1) / 2;
		while (child > 0)
		{
			// �Ƚϸ����뺢�ӽ�㣬���������ϵ���
			if (com(_array[child], _array[parent]))
			{
				swap(_array[parent], _array[child]);
				// ���Ͻ��е���
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}

	// ���µ�����
	void AdjustDown(size_t root)
	{
		Compare com;
		int parent = root;
		int child = parent * 2 + 1; //Ĭ��Ϊ����
		while (child < _array.size())
		{
			// ѡ���Һ�����������
			if (child + 1 < _array.size() && com(_array[child+1],_array[child]))
			{
				child++;
			}

			// �����븸�׽��бȽϺ͵���
			if (com(_array[child], _array[parent]))
			{
				swap(_array[parent], _array[child]);//���ڵ��뺢�ӽ��н���
				
				parent = child;// �������½��е���
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}


	void AdjustDown(int* heap, int k, int root)
	{
		int parent = root;
		int child = parent * 2 + 1;
		while (child < k)
		{
			if (heap[child + 1]>heap[child])
			{
				child++;
			}

			if (heap[parent] > heap[child])
			{
				swap(heap[parent], heap[child]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}

protected:
	vector<T> _array;
};
