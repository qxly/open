#ifndef WYIN_MEMORY_H
#define WYIN_MEMORY_H 1

#include <open/openDef.h>

namespace open
{
	template<typename T>
	class TMeomryQueue
	{
	public:

		TMeomryQueue()
			:_front(nullptr), _back(nullptr), _size(0)
		{
		}

		inline void push_back(T* t)
		{
			if (empty())
			{
				_front = t;
				_back = t;
				_size++;
				return;
			}
			t->_perv = _back;
			_back->_next = t;
			_back = t;
			_size++;
		}

		inline void push_front(T* t)
		{
			if (empty())
			{
				_front = t;
				_back = t;
				_size++;
				return;
			}

			t->_next = _front;
			_front->_perv = t;

			_front = t;
			_size++;
		}

		inline int32 size() const { return _size; }

		inline bool empty() const
		{
			return (_front == nullptr);
		}

		inline T* front()
		{
			return _front;
		}

		inline T* back()
		{
			return _back;
		}

		inline void erase(T* t)
		{
			if (t == _front)
				return pop_front();
			else if (t == _back)
			{
				return pop_back();
			}
			else
			{
				t->_perv->_next = t->_next;
				t->_next->_perv = t->_perv;
				_size--;
			}
		}

		inline void pop_front()
		{
			if (!_front)
				return;

			if (!_front->_next)
			{
				_front = nullptr;
				_back = nullptr;
				_size--;
				return;
			}

			_front->_next->_perv = nullptr;
			T* temp = _front;
			_front = _front->_next;
			_size--;
		}

		inline void pop_back()
		{
			if (!_back)
				return;

			if (!_back->_perv)
			{
				_back = nullptr;
				_front = nullptr;
				_size--;
				return;
			}

			_back->_perv->_next = nullptr;
			T* temp = _back;
			_back = _back->_perv;
			_size--;
		}

	private:
		T* _front;
		T* _back;
		int32 _size;
	};

	class COMMON_API MemoryAlloc
	{
	public:
		static void* malloc(size_t size);
		static void free(void* p);

		static void* align_malloc(size_t size, size_t alignment);
		static void align_free(void* p);
	};

	template<typename T, typename Alloc = MemoryAlloc>
	class TMemoryManager
	{
	public:
		TMemoryManager()
			:_memorySize(0) {}

		inline T* alloc(size_t size)
		{
			T* t = _unvaildQueue.back();
			if (t)
				_unvaildQueue.pop_back();
			else
			{
				t = (T*)Alloc::malloc(size);
				t->_next = nullptr;
				t->_perv = nullptr;
				_memorySize += size;

				_vaildQueue.push_back(t);
			}
			return t;
		}

		inline void dealloc(T* t)
		{
			_vaildQueue.erase(t);
			_unvaildQueue.push_front(t);
		}

	private:
		TMeomryQueue<T> _vaildQueue;
		TMeomryQueue<T> _unvaildQueue;

		int32 _memorySize;
	};

#ifdef _DEBUG
	class COMMON_API Object_Debug
	{
	public:

		Object_Debug()
		{
			++s_objectCount;
		}

		virtual ~Object_Debug()
		{
			--s_objectCount;
		}

		static void* operator new (size_t size)
		{
			return MemoryAlloc::malloc(size);
		}

			static void operator delete (void* p)
		{
			MemoryAlloc::free(p);
		}

		inline static int64 printErrorMemoryObjectCount()
		{
			printf("ErrorMemoryObjectCount is: %lld\n", s_objectCount);
			return s_objectCount;
		}

	private:
		static int64 s_objectCount;
	};

#define PUBLIC_OBJECT_DEBUG ,public Object_Debug
#define PRINT_ERROR_MEMORY_OBJECT_COUNT() Object_Debug::printErrorMemoryObjectCount();\


#else

#define PUBLIC_OBJECT_DEBUG
#define PRINT_ERROR_MEMORY_OBJECT_COUNT()

#endif

	template<typename T>
	class TPtr : public T
	{
	public:
		TPtr() :_refCount(0) {}

		virtual int32 addRef() { ++_refCount; return _refCount; }

		virtual int32 release() { ASSERT_(_refCount > 0); --_refCount; int32 r = _refCount; if (_refCount == 0) dispose(); return r; }

		virtual void releaseRefCountOnly() { ASSERT_(_refCount > 0); --_refCount; }

		virtual void dispose() { delete this; }
	private:
		int32 _refCount;
	};
}

#endif