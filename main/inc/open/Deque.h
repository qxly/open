#ifndef WYIN_QUEUE_H
#define WYIN_QUEUE_H

#include <open/openDef.h>

template<typename T>
class Deque
{
private:
	struct Link
	{
		Link* _next;
		Link* _perv;
		T _t;

		Link()
			:_next(nullptr),_perv(nullptr)
		{
		}

		Link(const T& t)
			:_t(t),_next(nullptr),_perv(nullptr)
		{
		}
	};

public:
	Deque()
		:_front(nullptr),_back(nullptr)
	{
	}

	Deque(const Deque& deque)
		:_front(nullptr),_back(nullptr)
	{
		Link* temp = deque._front;
		if(!temp)
			return;

		while(temp)
		{
			push_back(temp->_t);
			temp = temp->_next;
		}
	}

	void push_back(const T& t)
	{
		Link* link = new Link(t);
		if(empty())
		{
			_front = link;
			_back = link;
			return;
		}

		link->_perv = _back;
		_back->_next = link;
		_back = link;
	}

	void push_front(const T& t)
	{
		Link* link = new Link(t);
		if(empty())
		{
			_front = link;
			_back = link;
			return;
		}

		link->_next = _front;
		_front->_perv = link;

		_front = link;
	}

	bool empty() const
	{
		return (_front == nullptr);
	}

	const T& front() const
	{
		return _front->_t;
	}

	T& front()
	{
		return _front->_t;
	}

	const T& back() const
	{
		return _back->_t;
	}

	T& back()
	{
		return _back->_t;
	}

	void pop_front()
	{
		if(!_front)
			return;

		if(!_front->_next)
		{
			delete _front;
			_front = nullptr;
			_back = nullptr;
			return;
		}

		_front->_next->_perv = nullptr;
		Link* temp = _front;
		_front = _front->_next;

		delete temp;
	}

	void pop_back()
	{
		if(!_back)
			return;

		if(!_back->_perv)
		{
			delete _back;
			_back = nullptr;
			_front = nullptr;
			return;
		}

		_back->_perv->_next = nullptr;
		Link* temp = _back;
		_back = _back->_perv;

		delete temp;
	}

private:
	Link* _front;
	Link* _back;
};

#endif