#pragma once
#include <iostream>
#include "heap.h"
using namespace std;


template<class T>
struct HuffManTreeNode
{
	HuffManTreeNode(const T& weight)
		:_left(NULL),
		_right(NULL),
		_weight(weight)
	{}

	HuffManTreeNode<T>* _left;
	HuffManTreeNode<T>* _right;
	T _weight;					//加权值char_info
};

template<class T>
struct NodeCompare
{
	bool operator()(HuffManTreeNode<T>* l, HuffManTreeNode<T>* r)
	{
		return l->_weight < r->_weight;
	}
};

template<class T>
class HuffManTree
{
	typedef HuffManTreeNode<T> Node;
public:
	HuffManTree(const T* arr, size_t len, const T& invalid)
	{
		
		Heap<Node*, NodeCompare<T>> minHeap;     //构建小堆
		for (size_t i = 0;i <len;++i)
		{
			if (arr[i]!=invalid)
			{
				minHeap.Push(new Node(arr[i]));
			}
		}
		while (minHeap.size()>1)
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			Node* parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			minHeap.Push(parent);
		}
		_root = minHeap.Top();
		minHeap.Pop();
	}
	Node* GetRoot()
	{
		return _root;
	}
protected:
	Node* _root;
	//operator=
	//拷贝构造
};