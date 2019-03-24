#pragma once

#include<iostream>
#include<assert.h>
#include<vector>
#include<list>
#include<stack>
#include<queue>
using namespace std;

// 仿函数
// 进行大小的比较，来构建对应的大小堆
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
	// 此空构造函数不可删
	// 当是空堆时，会在初始化列表里调用vector的缺省构造函数进行初始化
	Heap()
	{}

	Heap(T* a, size_t n)
	{
		// 提前开辟空间，避免不断地增容，降低效率
		_array.reserve(n);

		for (size_t i = 0; i < n; i++)
		{
			_array.push_back(a[i]);
		}

		// 建堆
		// 从最后一个非叶节点开始进行调整，此处即为i
		for (int i = ((int)_array.size() - 2) >> 1; i >= 0; i--)
		{
			AdjustDown(i);
		}
	}

	void Push(const T& x)
	{
		// 插入数据到vector
		_array.push_back(x);
		// 进行向上调整
		AdjustUp(_array.size() - 1);
	}

	void Pop()
	{
		// 将首尾结点进行交换
		swap(_array[_array.size() - 1], _array[0]);
		// 删掉尾节点
		_array.pop_back();
		// 通过向下调整重复建堆
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
	// 向上调整法
	void AdjustUp(size_t child)
	{
		Compare com;
		size_t parent = (child - 1) / 2;
		while (child > 0)
		{
			// 比较父亲与孩子结点，并进行向上调整
			if (com(_array[child], _array[parent]))
			{
				swap(_array[parent], _array[child]);
				// 向上进行调整
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}

	// 向下调整法
	void AdjustDown(size_t root)
	{
		Compare com;
		int parent = root;
		int child = parent * 2 + 1; //默认为左孩子
		while (child < _array.size())
		{
			// 选左右孩子里面最大的
			if (child + 1 < _array.size() && com(_array[child+1],_array[child]))
			{
				child++;
			}

			// 孩子与父亲进行比较和调整
			if (com(_array[child], _array[parent]))
			{
				swap(_array[parent], _array[child]);//父节点与孩子进行交换
				
				parent = child;// 继续向下进行调整
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
