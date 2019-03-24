#pragma once

#include"heap.h"

template<class W>
struct HuffmanTreeNode
{
	W _weight;  //Ȩֵ
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
		// �����ڲ��࣬���÷º����ԴﵽȨֵ�Ƚϵ�Ч��
		struct NodeCompare
		{
			bool operator()(Node* left, Node* right)
			{
				return left->_weight < right->_weight;
			}
		};

		// ����һ��С��
		Heap<Node*, NodeCompare> minheap;
		
		// �����ݲ��뵽С��֮��
		for (size_t i = 0; i < n; i++)
		{
			// ���������Ƿ�Ƿ�
			if (a[i] != invalid)
			{
				minheap.Push(new Node(a[i]));
			}
		}
		
		// ����Huffman����������С����ֻ��һ������ʱ֤���������
		while (minheap.Size() > 1)
		{
			// ȡ�Ѷ�����Ϊ����
			Node* left = minheap.Top();
			minheap.Pop();

			// ȡ�Ѷ�����Ϊ�Һ���
			Node* right = minheap.Top();
			minheap.Pop();

			// �������ڵ㣬�����������Һ�����������
			Node* parent = new Node(left->_weight + right->_weight);
			parent->_right = right;
			right->_parent = parent;	
			parent->_left = left;
			left->_parent = parent;

			// �����ڵ����С����
			minheap.Push(parent);
		}

		// �Ѷ����ݼ�Ϊ���ڵ�
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
	// ��װ
	HuffmanTree(const HuffmanTree<W>& huf);
	Node*& operator=(const HuffmanTree<W>& huf);
	Node* _root;
};


