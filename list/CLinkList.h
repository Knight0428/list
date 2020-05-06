#pragma once
#include <iostream>


template<typename TYPE>
class CLinkList
{
private:
	//链表节点
	struct CLinkNode
	{
		CLinkNode()
			:prev_(NULL), next_(NULL)
		{
		}
		//拷贝构造
		CLinkNode(const TYPE& data,
			CLinkNode* prev = NULL,
			CLinkNode* next = NULL)
			:data_(data), prev_(prev), next_(next)
		{
		}
		//移动构造
		CLinkNode(TYPE&& data,
			CLinkNode* prev = NULL,
			CLinkNode* next = NULL)
			:data_(std::move(data)), prev_(prev), next_(next)
		{
		}

		//节点数据
		TYPE data_;
		//前驱
		CLinkNode *prev_;
		//后继
		CLinkNode *next_;
	};

public:
	//迭代器
	class const_Iterator
	{
	public:
		const_Iterator(CLinkNode* node = nullptr) :node_(node) { }
		//获取节点数据
		const TYPE& operator* ()
		{
			return node_->data_;
		}
		//指向下一个节点
		const_Iterator operator++ ()
		{
			node_ = node_->next_;
			return *this;
		}
		//取出节点
		const_Iterator operator++ (int)
		{
			const_Iterator old(node_);
			++(*this);
			return old;
		}
		//重载==
		bool operator== (const const_Iterator& it)
		{
			return node_ == it.node_;
		}
		//重载！=
		bool operator!= (const const_Iterator& it)
		{
			return node_ != it.node_;
		}
	protected:
		CLinkNode* node_;
	};

	//链表迭代器
	class Iterator :public const_Iterator
	{
	public:
		Iterator(CLinkNode* node = nullptr) :const_Iterator(node) { }


		TYPE& operator* ()
		{
			return const_Iterator::node_->data_;
		}


		Iterator operator++ ()
		{
			const_Iterator::node_ = const_Iterator::node_->next_;
			return *this;
		}
		Iterator operator++ (int)
		{
			Iterator old(const_Iterator::node_);
			const_Iterator::node_ = const_Iterator::node_->next_;
			return old;
		}
		bool operator== (const Iterator& it)
		{
			return const_Iterator::node_ == it.const_Iterator::node_;
		}
		bool operator!= (const Iterator& it)
		{
			return const_Iterator::node_ != it.const_Iterator::node_;
		}

		//友元链表
		friend class CLinkList;
	};

	//链表的构造与析构，拷贝构造与移动构造
public:
	CLinkList();
	~CLinkList();
	CLinkList(const CLinkList& list);
	CLinkList(CLinkList&& list);
	CLinkList& operator= (const CLinkList& list);
	CLinkList& operator= (CLinkList&& list);

public:
	const_Iterator begin() const
	{
		return front();
	}
	const_Iterator end() const
	{
		return back().node_->next_;
	}
	//迭代器头节点
	Iterator begin()
	{
		return front();
	}
	//迭代器尾节点
	Iterator end()
	{
		return back().node_->next_;
	}

public:
	//插入元素到尾部
	void push_back(const TYPE& elem);
	void push_back(TYPE&& elem);
	//插入元素到头部
	void push_front(const TYPE& elem);
	void push_front(TYPE&& elem);
	//链表合并
	void append(const CLinkList& list);
	//获取链表大小
	size_t size() const;
	//迭代器头元素
	Iterator back()  const
	{
		//链表头节点的下一个
		return tail_->prev_;
	}
	//迭代器尾元素
	Iterator front() const
	{
		//链表尾节点的前一个
		return head_->next_;
	}
	//插入元素到指定位置
	Iterator insert(Iterator pos, const TYPE& elem)
	{
		pos.node_->prev_ = pos.node_->prev_->next_ = new CLinkNode(elem, pos.node_->prev_, pos.node_);
		size_++;
		return pos.node_->prev_;
	}
	Iterator insert(Iterator pos, TYPE&& elem)
	{
		pos.node_->prev_ = pos.node_->prev_->next_ = new CLinkNode(std::move(elem), pos.node_->prev_, pos.node_);
		size_++;
		return pos.node_->prev_;
	}
	//删除尾元素
	void pop_back();
	//删除头元素
	void pop_front();
	//是否空表
	bool empty() const;
	//清空表
	void clear();
	//删除元素
	void erese(Iterator pos);
	//按范围删除元素
	void erese(Iterator beg, Iterator end);


private:
	//初始化
	void init();


private:
	//链表大小
	size_t  size_;
	//链表头节点
	CLinkNode *head_;
	//链表尾节点
	CLinkNode *tail_;
};


///////////////////
//实现
///////////////////
template<typename  TYPE>
CLinkList<TYPE>::CLinkList()
{
	init();
}

template<typename  TYPE>
CLinkList<TYPE>::~CLinkList()
{
	clear();
	delete head_;
	delete tail_;
}

template<typename  TYPE>
CLinkList<TYPE>::CLinkList(const CLinkList<TYPE>& list)
{
	init();

	if (list.empty())
		return;

	//遍历
	for (CLinkList<TYPE>::const_Iterator it = list.begin();
		it != list.end();
		it++)
	{
		push_back(*it);
	}
}

//移动构造
template<typename  TYPE>
CLinkList<TYPE>::CLinkList(CLinkList<TYPE>&& list)
{
	init();
	//两个链表交换
	std::swap(size_, list.size_);
	std::swap(head_, list.head_);
	std::swap(tail_, list.tail_);
}

template<typename  TYPE>
CLinkList<TYPE>& CLinkList<TYPE>::operator= (const CLinkList<TYPE>& list)
{
	clear();


	if (list.empty())
		return *this;


	//遍历
	for (CLinkList<TYPE>::const_Iterator it = list.begin();
		it != list.end();
		it++)
	{
		push_back(*it);
	}



	return *this;
}

template<typename  TYPE>
CLinkList<TYPE>& CLinkList<TYPE>::operator=(CLinkList<TYPE>&& list)
{
	std::swap(size_, list.size_);
	std::swap(head_, list.head_);
	std::swap(tail_, list.tail_);

	return *this;
}

//尾部插入
template<typename  TYPE>
void CLinkList<TYPE>::push_back(const TYPE& elem)
{
	tail_->prev_ = tail_->prev_->next_ = new CLinkNode(elem, tail_->prev_, tail_);


	size_++;
}

template<typename  TYPE>
void CLinkList<TYPE>::push_back(TYPE&& elem)
{
	tail_->prev_ = tail_->prev_->next_ = new CLinkNode(std::move(elem), tail_->prev_, tail_);


	size_++;
}

//头部插入
template<typename  TYPE>
void CLinkList<TYPE>::push_front(const TYPE& elem)
{
	head_->next_ = head_->next_->prev_ = new CLinkNode(elem, head_, head_->next_);
	size_++;
}

template<typename  TYPE>
void CLinkList<TYPE>::push_front(TYPE&& elem)
{
	head_->next_ = head_->next_->prev_ = new CLinkNode(std::move(elem), head_, head_->next_);
	size_++;
}

//合并链表
template<typename  TYPE>
void CLinkList<TYPE>::append(const CLinkList<TYPE>& list)
{
	if (list.empty())
		return;

	//遍历
	for (CLinkList<TYPE>::const_Iterator it = list.begin();
		it != list.end();
		it++)
	{
		push_back(*it);
	}
}

//链表大小
template<typename  TYPE>
size_t CLinkList<TYPE>::size() const
{
	return size_;
}

//删除尾元素
template<typename  TYPE>
void CLinkList<TYPE>::pop_back()
{
	erese(back());
}


//删除头元素
template<typename  TYPE>
void CLinkList<TYPE>::pop_front()
{
	erese(front());
}


//是否空表
template<typename  TYPE>
bool CLinkList<TYPE>::empty() const
{
	return size_ == 0;
}


//清空表
template<typename  TYPE>
void CLinkList<TYPE>::clear()
{
	while (!empty())
	{
		pop_back();
	}
}


//删除元素
template<typename  TYPE>
void CLinkList<TYPE>::erese(Iterator pos)
{
	//1 [2] 3
	pos.node_->prev_->next_ = pos.node_->next_;
	pos.node_->next_->prev_ = pos.node_->prev_;

	size_--;
	delete pos.node_;
}


//按范围删除元素
template<typename  TYPE>
void CLinkList<TYPE>::erese(Iterator beg, Iterator end)
{
	//1 [2 ....5]  6
	beg.node_->prev_->next_ = end.node_->next_;
	end.node_->next_->prev_ = beg.node_->prev_;

	CLinkNode *next;
	//循环删除区间内元素
	while (true)
	{
		next = beg.node_->next_;
		delete beg.node_;
		beg = next;
		size_--;
		if (beg == end)
		{
			delete beg.node_;
			size_--;
			break;
		}
	}
}

template<typename  TYPE>
void CLinkList<TYPE>::init()
{
	size_ = 0;
	head_ = new CLinkNode();
	tail_ = new CLinkNode();
	head_->next_ = tail_;
	tail_->prev_ = head_;
}