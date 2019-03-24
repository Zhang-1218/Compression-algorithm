#pragma once

#include"heap.h"

template<class W>
struct HuffmanTreeNode
{
	W _weight;  //权值
	HuffmanTreeNode<W>* _left;
	HuffmanTreeNode<W>* _right;
	HuffmanTreeNode<W>* _parent;

	HuffmanTreeNode(const W& w)
		:_weight(w)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{}
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	HuffmanTree(W* a, size_t n, const W& invalid)
	{
		// 建立内部类，利用仿函数以达到权值比较的效果
		struct NodeCompare
		{
			bool operator()(Node* left, Node* right)
			{
				return left->_weight < right->_weight;
			}
		};

		// 建立一个小堆
		Heap<Node*, NodeCompare> minheap;
		
		// 将数据插入到小堆之中
		for (size_t i = 0; i < n; i++)
		{
			// 检验数据是否非法
			if (a[i] != invalid)
			{
				minheap.Push(new Node(a[i]));
			}
		}
		
		// 建立Huffman树，当最终小堆里只有一个数据时证明建树完成
		while (minheap.Size() > 1)
		{
			// 取堆顶数据为左孩子
			Node* left = minheap.Top();
			minheap.Pop();

			// 取堆顶孩子为右孩子
			Node* right = minheap.Top();
			minheap.Pop();

			// 建立父节点，并将其与左右孩子连接起来
			Node* parent = new Node(left->_weight + right->_weight);
			parent->_right = right;
			right->_parent = parent;	
			parent->_left = left;
			left->_parent = parent;

			// 将父节点插入小堆中
			minheap.Push(parent);
		}

		// 堆顶数据即为根节点
		_root = minheap.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}

	void PostOrder()
	{
		_PostOrder(_root);
	}

	void _PostOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}

		_PostOrder(root->_left);
		_PostOrder(root->_right);
		cout << root->_weight << " ";
	}

	void _Clear(Node* root)
	{
		if (root)
		{
			_Clear(root->_left);
			_Clear(root->_right);
			delete root;
		}
	}

	~HuffmanTree()
	{
		_Clear(_root);
	}
protected:
	// 封装
	HuffmanTree(const HuffmanTree<W>& huf);
	Node*& operator=(const HuffmanTree<W>& huf);
	Node* _root;
};


