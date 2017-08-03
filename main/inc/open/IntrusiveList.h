#ifndef WYIN_INTRUSIVELIST_H
#define WYIN_INTRUSIVELIST_H

#include <open/openDef.h>

namespace open
{
	template<class T>
	class TLink 
	{
	public:
		TLink()
		{
			_nextNode = (T*) ((size_t) this + 1 - 0);
			_prevLink = this;
		}

		~TLink()
		{
			removeFromList();
		}

		inline bool linked() const
		{
			return _prevLink != this;
		}

		inline void unlink()
		{
			removeFromList();
			_nextNode = (T *) ((size_t) this + 1);
			_prevLink = this;
		}

		inline T* prev()
		{
			T * prevNode = _prevLink->_prevLink->_nextNode;
			if ((size_t) prevNode & 1)
				return nullptr;
			return prevNode;
		}

		inline T* next()
		{
			if ((size_t) _nextNode & 1)
				return nullptr;
			return _nextNode;
		}

		inline const T* prev() const
		{
			const T * prevNode = _prevLink->_prevLink->_nextNode;
			if ((size_t) prevNode & 1)
				return nullptr;
			return prevNode;
		}

		inline const T* next() const
		{
			if ((size_t) _nextNode & 1)
				return nullptr;
			return _nextNode;
		}

		inline TLink(size_t offset)
		{
			_nextNode = (T *) ((size_t) this + 1 - offset);
			_prevLink = this;
		}

		inline void setOffset(size_t offset)
		{
			_nextNode = (T *) ((size_t) this + 1 - offset);
			_prevLink = this;
		}

		inline TLink<T>* nextLink()
		{
			size_t offset = (size_t) this - ((size_t) _prevLink->_nextNode & ~1);
			return (TLink<T> *) (((size_t) _nextNode & ~1) + offset);
		}

		inline TLink<T>* prevLink(){ return _prevLink; }

		inline void insertBefore(T* node,TLink<T>* nextLink)
		{
			removeFromList();

			_prevLink = nextLink->_prevLink;
			_nextNode = _prevLink->_nextNode;

			nextLink->_prevLink->_nextNode = node;
			nextLink->_prevLink = this;
		}

		inline void insertAfter(T* node,TLink<T>* prevLink)
		{
			removeFromList();

			_prevLink = prevLink;
			_nextNode = prevLink->_nextNode;

			prevLink->nextLink()->_prevLink = this;
			prevLink->_nextNode = node;
		}

	private:
		inline void removeFromList()
		{
			nextLink()->_prevLink = _prevLink;
			_prevLink->_nextNode = _nextNode;
		}

		TLink(const TLink&);
		TLink & operator = (const TLink &);

		T* _nextNode;
		TLink<T>* _prevLink;
	};

	template<class T>
	class IntrusiveList 
	{
	public:
		IntrusiveList()
			:_link(),_offset((size_t) -1)
		{
		}

		~IntrusiveList()
		{
			clear();
		}

		inline bool empty() const
		{
			return _link.next() == nullptr;
		}

		inline void clear()
		{
			for (;;) 
			{
				TLink<T> * link = _link.prevLink();
				if (link == &_link)
					break;
				link->unlink();
			}
		}

		inline void clear_destory()
		{
			while (T* node = _link.next())
				delete node;
		}

		inline T* front()
		{
			return _link.next();
		}

		inline const T* front() const
		{
			return _link.next();
		}

		inline T* back()
		{
			return _link.prev();
		}

		inline const T* back() const
		{
			return _link.prev();
		}

		inline T* prev(T * node)
		{
			return getLinkFromNode(node)->prev();
		}

		const T* prev (const T * node) const
		{
			return getLinkFromNode(node)->prev();
		}

		inline T* next(T * node)
		{
			return getLinkFromNode(node)->next();
		}

		inline const T* next(const T* node) const
		{
			return getLinkFromNode(node)->next();
		}

		inline void push_front(T* node)
		{
			insertAfter(node, nullptr);
		}

		inline void push_back(T* node)
		{
			insertBefore(node, nullptr);
		}

		inline void insertBefore(T* node,T* before)
		{
			ASSERT_(!((size_t)node & 1));
			getLinkFromNode(node)->insertBefore(
				node,
				before ? getLinkFromNode(before) : &_link
				);
		}

		inline void insertAfter(T* node,T* after)
		{
			ASSERT(!((size_t) node & 1));
			getLinkFromNode(node)->insertAfter(
				node,
				after ? getLinkFromNode(after) : &_link);
		}

		inline void erase(T* node)
		{
			getLinkFromNode(node)->unlink();
		}

		inline void erase_destroy(T* node)
		{
			delete node;
		}

	private:

		IntrusiveList( size_t offset)
			:_link(offset),_offset(offset)
		{
		}

		inline TLink<T>* getLinkFromNode(const T * node) const
		{
			ASSERT_(_offset != (size_t)-1);
			return (TLink<T>*) ((size_t) node + _offset);
		}

		template<class T, size_t offset> friend class IntrusiveListDeclare;

		IntrusiveList(const IntrusiveList &);
		IntrusiveList & operator= (const IntrusiveList &);

		TLink<T> _link;
		size_t _offset;
	};

	template<class T, size_t offset>
	class IntrusiveListDeclare : public IntrusiveList<T> 
	{
	public:
		IntrusiveListDeclare()
			:IntrusiveList<T>(offset){}
	};

#define INTRUSIVELIST_DECLARE(T,link) IntrusiveListDeclare<T, offsetof(T, link)>

#define INTRUSIVELIST_LINK(T) TLink<T>

#define INTRUSIVELIST_PTR(T) IntrusiveList<T>*
}

#endif