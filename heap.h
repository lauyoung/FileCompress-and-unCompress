#pragma once
#include<vector>
#include<assert.h>
using namespace std;


//最大堆：每个父节点的值都大于孩子节点
//最大堆：每个父节点的值都小于孩子节点

//仿函数来确定创建大堆还是小堆 

template<class T>
struct Less
{
public:
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
public:
	bool operator()(const T& l, const T& r)
	{
		return l > r;
	}
};

template<class T, class Compare = Less<T>>
class Heap
{
public:
	Heap()
	{}
	Heap(const T* arr, size_t size)
	{
		assert(arr);
		//插入节点
		for (size_t i = 0;i < size;++i)
		{
			_arr.push_back(arr[i]);
		}

		//向下调整n*logn
		for (int i = (_arr.size() - 2) / 2;i >= 0;--i)
		{
			_AdjustDown(i);
		}
	}

protected:
	void _AdjustDown(size_t parent)
	{
		size_t child = parent * 2 + 1;
		while (child < _arr.size())
		{
			if (child + 1 < _arr.size() && Compare()(_arr[child + 1], _arr[child]))
				++child;

			//if (_arr[child]<_arr[parent])
			if (Compare()(_arr[child], _arr[parent]))
			{
				swap(_arr[child], _arr[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	void _AdjustUp(size_t child)
	{
		int parent = (child - 1) / 2;
		while (child > 0)
		{
			//if (_arr[child] > _arr[parent])
			if (Compare()(_arr[child], _arr[parent]))
			{
				swap(_arr[child], _arr[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}
public:
	void Push(const T& x)
	{
		_arr.push_back(x);
		_AdjustUp(_arr.size() - 1);
	}
	void Pop()
	{
		swap(_arr[0],_arr[_arr.size() - 1]);
		_arr.pop_back();
		_AdjustDown(0);
	}
	T& Top()
	{
		assert(_arr.size() > 0);
		return (*_arr.begin());
	}
	size_t size()
	{
		return _arr.size();
	}
private:
	vector<T> _arr;
};