#pragma once
#include <iostream>


template<typename TYPE>
class CLinkList
{
private:
	//����ڵ�
	struct CLinkNode
	{
		CLinkNode()
			:prev_(NULL), next_(NULL)
		{
		}
		//��������
		CLinkNode(const TYPE& data,
			CLinkNode* prev = NULL,
			CLinkNode* next = NULL)
			:data_(data), prev_(prev), next_(next)
		{
		}
		//�ƶ�����
		CLinkNode(TYPE&& data,
			CLinkNode* prev = NULL,
			CLinkNode* next = NULL)
			:data_(std::move(data)), prev_(prev), next_(next)
		{
		}

		//�ڵ�����
		TYPE data_;
		//ǰ��
		CLinkNode *prev_;
		//���
		CLinkNode *next_;
	};

public:
	//������
	class const_Iterator
	{
	public:
		const_Iterator(CLinkNode* node = nullptr) :node_(node) { }
		//��ȡ�ڵ�����
		const TYPE& operator* ()
		{
			return node_->data_;
		}
		//ָ����һ���ڵ�
		const_Iterator operator++ ()
		{
			node_ = node_->next_;
			return *this;
		}
		//ȡ���ڵ�
		const_Iterator operator++ (int)
		{
			const_Iterator old(node_);
			++(*this);
			return old;
		}
		//����==
		bool operator== (const const_Iterator& it)
		{
			return node_ == it.node_;
		}
		//���أ�=
		bool operator!= (const const_Iterator& it)
		{
			return node_ != it.node_;
		}
	protected:
		CLinkNode* node_;
	};

	//���������
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

		//��Ԫ����
		friend class CLinkList;
	};

	//����Ĺ����������������������ƶ�����
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
	//������ͷ�ڵ�
	Iterator begin()
	{
		return front();
	}
	//������β�ڵ�
	Iterator end()
	{
		return back().node_->next_;
	}

public:
	//����Ԫ�ص�β��
	void push_back(const TYPE& elem);
	void push_back(TYPE&& elem);
	//����Ԫ�ص�ͷ��
	void push_front(const TYPE& elem);
	void push_front(TYPE&& elem);
	//����ϲ�
	void append(const CLinkList& list);
	//��ȡ�����С
	size_t size() const;
	//������ͷԪ��
	Iterator back()  const
	{
		//����ͷ�ڵ����һ��
		return tail_->prev_;
	}
	//������βԪ��
	Iterator front() const
	{
		//����β�ڵ��ǰһ��
		return head_->next_;
	}
	//����Ԫ�ص�ָ��λ��
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
	//ɾ��βԪ��
	void pop_back();
	//ɾ��ͷԪ��
	void pop_front();
	//�Ƿ�ձ�
	bool empty() const;
	//��ձ�
	void clear();
	//ɾ��Ԫ��
	void erese(Iterator pos);
	//����Χɾ��Ԫ��
	void erese(Iterator beg, Iterator end);


private:
	//��ʼ��
	void init();


private:
	//�����С
	size_t  size_;
	//����ͷ�ڵ�
	CLinkNode *head_;
	//����β�ڵ�
	CLinkNode *tail_;
};


///////////////////
//ʵ��
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

	//����
	for (CLinkList<TYPE>::const_Iterator it = list.begin();
		it != list.end();
		it++)
	{
		push_back(*it);
	}
}

//�ƶ�����
template<typename  TYPE>
CLinkList<TYPE>::CLinkList(CLinkList<TYPE>&& list)
{
	init();
	//����������
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


	//����
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

//β������
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

//ͷ������
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

//�ϲ�����
template<typename  TYPE>
void CLinkList<TYPE>::append(const CLinkList<TYPE>& list)
{
	if (list.empty())
		return;

	//����
	for (CLinkList<TYPE>::const_Iterator it = list.begin();
		it != list.end();
		it++)
	{
		push_back(*it);
	}
}

//�����С
template<typename  TYPE>
size_t CLinkList<TYPE>::size() const
{
	return size_;
}

//ɾ��βԪ��
template<typename  TYPE>
void CLinkList<TYPE>::pop_back()
{
	erese(back());
}


//ɾ��ͷԪ��
template<typename  TYPE>
void CLinkList<TYPE>::pop_front()
{
	erese(front());
}


//�Ƿ�ձ�
template<typename  TYPE>
bool CLinkList<TYPE>::empty() const
{
	return size_ == 0;
}


//��ձ�
template<typename  TYPE>
void CLinkList<TYPE>::clear()
{
	while (!empty())
	{
		pop_back();
	}
}


//ɾ��Ԫ��
template<typename  TYPE>
void CLinkList<TYPE>::erese(Iterator pos)
{
	//1 [2] 3
	pos.node_->prev_->next_ = pos.node_->next_;
	pos.node_->next_->prev_ = pos.node_->prev_;

	size_--;
	delete pos.node_;
}


//����Χɾ��Ԫ��
template<typename  TYPE>
void CLinkList<TYPE>::erese(Iterator beg, Iterator end)
{
	//1 [2 ....5]  6
	beg.node_->prev_->next_ = end.node_->next_;
	end.node_->next_->prev_ = beg.node_->prev_;

	CLinkNode *next;
	//ѭ��ɾ��������Ԫ��
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