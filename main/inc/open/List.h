//#ifndef WYIN_LIST_H
//#define WYIN_LIST_H
//
//#include <open/openDef.h>
//
//template<typename T>
//class List
//{
//public:
//	struct Link
//	{
//		Link* _next;
//		Link* _perv;
//		T _t;
//
//		Link()
//			:_next(nullptr),_perv(nullptr)
//		{
//		}
//
//		Link(const T& t)
//			:_t(t),_next(nullptr),_perv(nullptr)
//		{
//		}
//	};
//	typedef Link* iterator;
//	typedef const Link* const_iterator;
//
//public:
//
//	List()
//		:_front(nullptr),_back(nullptr),_size(0)
//	{
//	}
//
//	List(const List& deque)
//		:_front(nullptr),_back(nullptr),_size(0)
//	{
//		Link* temp = deque._front;
//		if(!temp)
//			return;
//
//		while(temp)
//		{
//			push_back(temp->_t);
//			temp = temp->_next;
//		}
//	}
//	
//	~List()
//	{
//		clear();
//	}
//
//	inline void push_back(const T& t)
//	{
//		Link* link = new Link(t);
//		if(empty())
//		{
//			_front = link;
//			_back = link;
//			_size++;
//			return;
//		}
//
//		link->_perv = _back;
//		_back->_next = link;
//		_back = link;
//		_size++;
//	}
//
//	inline void push_front(const T& t)
//	{
//		Link* link = new Link(t);
//		if(empty())
//		{
//			_front = link;
//			_back = link;
//			_size++;
//			return;
//		}
//
//		link->_next = _front;
//		_front->_perv = link;
//
//		_front = link;
//		_size++;
//	}
//
//	inline int32 size() const{ return _size;}
//
//	inline bool empty() const
//	{
//		return (_front == nullptr);
//	}
//
//	inline const T& front() const
//	{
//		return _front->_t;
//	}
//
//	inline T& front()
//	{
//		return _front->_t;
//	}
//
//	inline const T& back() const
//	{
//		return _back->_t;
//	}
//
//	inline T& back()
//	{
//		return _back->_t;
//	}
//
//	inline const iterator next(const_iterator it) const
//	{
//		return it->_next;
//	}
//
//	inline iterator next(const_iterator it)
//	{
//		return it->_next;
//	}
//
//	inline const iterator prev(const_iterator it) const
//	{
//		return it->_perv;
//	}
//
//	inline iterator prev(const_iterator it)
//	{
//		return it->_perv;
//	}
//
//	inline iterator begin() { return _front;}
//
//	inline const_iterator begin() const { return _front;}
//
//	inline iterator end() { if(_back) return _back->_next; return nullptr;}
//
//	inline const_iterator end() const { if(_back) return _back->_next; return nullptr;}
//
//	inline void insert(iterator it,const T& t)
//	{
//		if(empty() || it == _back || it == end())
//			push_back(t);
//		else
//		{
//			Link* link = new Link(t);
//			Line* temp = it._next;
//			it._next = link;
//			link->_perv = &it;
//			link->_next = temp;
//			temp->_perv = link;
//			_size++;
//		}
//	}
//
//	inline void erase(iterator it)
//	{
//		if(it == _front)
//			return pop_front();
//		else if(it == _back)
//		{
//			return pop_back();
//		}
//		else
//		{
//			it._perv->_next = it._next;
//			it._next->_perv = it._perv;
//
//			delete it;
//			_size--;
//		}
//	}
//
//	inline void clear()
//	{
//		Link* temp = _front;
//		if(!temp)
//			return;
//
//		while(temp)
//		{
//			Link* t = temp->_next;;
//			delete temp;
//			temp = t;
//		}
//		_front = nullptr;
//		_back = nullptr;
//		_size = 0;
//	}
//
//	inline void pop_front()
//	{
//		if(!_front)
//			return;
//
//		if(!_front->_next)
//		{
//			delete _front;
//			_front = nullptr;
//			_back = nullptr;
//			_size--;
//			return;
//		}
//
//		_front->_next->_perv = nullptr;
//		Link* temp = _front;
//		_front = _front->_next;
//
//		delete temp;
//		_size--;
//	}
//
//	inline void pop_back()
//	{
//		if(!_back)
//			return;
//
//		if(!_back->_perv)
//		{
//			delete _back;
//			_back = nullptr;
//			_front = nullptr;
//			_size--;
//			return;
//		}
//
//		_back->_perv->_next = nullptr;
//		Link* temp = _back;
//		_back = _back->_perv;
//
//		delete temp;
//		_size--;
//	}
//
//private:
//	Link* _front;
//	Link* _back;
//	int32 _size;
//};
//
//#endif