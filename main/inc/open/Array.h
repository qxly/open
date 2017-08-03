#ifndef COMMON_ARRAY_H
#define COMMON_ARRAY_H

#include <open/openDef.h>

namespace open
{
	/////////////////////////////////////////////////////////////////////////////////
	/// \brief
	/// 模板数组基类，使用模式及命名方法类似于stl,我们建议尽量不要使用迭代器访问，尤其是反向迭代器，
	/// 此数组兼容迭代器是因为兼容老代码原因，Array的正向迭代器比stl的快很多，但是反向迭代器和stl的一样。
	///
	template<typename T,typename S,typename IT,typename CONSTIT,typename RIT,typename RCONSTIT>
	class Array
	{
	public:
		typedef IT iterator;
		typedef CONSTIT const_iterator;
		typedef RIT reverse_iterator;
		typedef RCONSTIT const_reverse_iterator;

		inline int32 size() { return _s.size();}
		inline int32 size()  const { return _s.size();}

		inline bool empty() { if(_s.size() == 0) return true; return false;}
		inline bool empty() const { if(_s.size() == 0) return true; return false;}

		inline T& front() { return _s.front();}
		inline const T& front() const { return _s.front();}

		inline T& back() { return _s.back();}
		inline const T& back() const { return _s.back();}

		inline IT begin() { return _s.begin();}
		inline CONSTIT begin() const { return _s.begin();}

		inline IT end() { return _s.begin() + size();}
		inline CONSTIT end() const { return _s.begin() + size();}

		inline RIT rbegin() { return _s.rbegin() ;}
		inline RCONSTIT rbegin() const { return _s.rbegin();}

		inline RIT rend() { return _s.rbegin()+ size();}
		inline RCONSTIT rend() const { return _s.rbegin()+ size();}

		inline bool contain(const T& t)
		{
			for(int32 i =0; i < size(); i++)
			{
				if(t == at(i))
					return true;
			}
			return false;
		}

		inline bool contain(const T& t) const
		{
			for(int32 i = 0; i < size(); i++)
			{
				if(t == at(i))
					return true;
			}
			return false;
		}

		inline int32 index(const T& t)
		{
			for(int32 i =0; i < size(); i++)
			{
				if(t == at(i))
					return i;
			}
			return -1;
		}

		inline int32 index(const T& t) const
		{
			for(int32 i =0; i < size(); i++)
			{
				if(t == at(i))
					return i;
			}
			return -1;
		}

		inline T& at(int32 i) { return *(_s.begin() + i);}
		inline const T& at(int32 i) const { return *(_s.begin() + i);}

		inline T& operator [] (int32 i) { return _s[i];}
		inline const T& operator [] (int32 i) const { return _s[i];}

		inline void set(int32 i,const T& t) { if(i > size()-1) return;_s[i] = t;}

		inline T* getDataPointer() { if(empty()) return nullptr; return &(front());}
		inline const T* getDataPointer() const { if(empty()) return nullptr; return &(front());}

		inline void assign(const T& value)
		{
			_s.assign(size(), value);
		}

		inline Array& operator = (const Array<T,S,IT,CONSTIT,RIT,RCONSTIT>& array)
		{
			_s = array._s;
			return *this;
		}

	protected:
		/// 数组指针
		S _s;
	};

	template<typename T>
	class InternalFastArray
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;
		struct ReverseIterator
		{
			inline ReverseIterator(T* point)
				:t(point){}

			inline ReverseIterator operator + (int32 i){ return ReverseIterator(t-i); }

			inline ReverseIterator& operator ++ (){ t--; return *this;}

			inline ReverseIterator& operator += (int32 i){ t -= i; return *this; }

			inline T* operator->() { return t;}

			inline const T* operator->() const { return t;}

			inline T& operator*() { return *t;}

			inline const T& operator*() const { return *t;}

			inline bool operator != (const ReverseIterator& other) const { return t != other.t;}

			inline bool operator == (const ReverseIterator& other) const { return t == other.t;} 

			T* t;
		};

		typedef ReverseIterator reverse_iterator;
		typedef ReverseIterator const_reverse_iterator;

		inline int32 size() const{ return _size;}

		inline T& operator [] (int i) { return _elements[i];}
		inline const T& operator [] (int i) const { return _elements[i];}

		inline iterator begin() { return _elements;}
		inline const_iterator begin() const { return _elements;}

		inline reverse_iterator rbegin() { return reverse_iterator(&(_elements[_size-1]));}
		inline const_reverse_iterator rbegin() const { return reverse_iterator(&(_elements[_size-1]));}

		inline T& front() { return _elements[0];}
		inline const T& front() const { return _elements[0];}

		inline T& back() { return _elements[_size-1];}
		inline const T& back() const { return _elements[_size-1];}

		inline void assign(int32 size,const T& value)
		{
			for(int32 i = 0; i < size; i++)
			{
				_elements[i] = value;
			}
		}
	protected:
		T* _elements;

		/// 大小
		int32 _size;
	};


	template<typename T>
	class InternalFastDynamicArray : public InternalFastArray<T>
	{
	public:

		inline InternalFastDynamicArray()
			:_onceAlloc(-1),_capacity(0)
		{
			_size = 0;
			_elements = nullptr;
		}

		inline InternalFastDynamicArray(int32 size)
			:_onceAlloc(-1),_capacity(0)
		{
			_size = 0;
			_elements = nullptr;
			extend(size);
			_size = size;
		}

		inline void reserve(int32 size) 
		{ 
			_onceAlloc = size;
		}

		inline void push_back(T const& value)
		{
			if (_size == _capacity)
				extend(_onceAlloc);
			_elements[_size] = value;
			_size++;
		}

		inline void push_front(T const& value)
		{
			T* elements = _elements;
			if (_size == _capacity)
			{
				elements = allocExtend(_onceAlloc);
			}
			for (int32 i = _size; i > 0; --i)
			{
				elements[i] = _elements[i-1];
			}
			elements[0] = value;
			if(elements != _elements)
			{
				delete [] _elements;
				_elements = elements;
			}
			_size++;
		}

		inline void pop_back()
		{
			erase(_size - 1);
		}

		inline void pop_front()
		{
			erase(0);
		}

		inline void insert(int32 index,T const& value)
		{
			if(index == 0)
				push_front(value);
			else if(index == _size)
				push_back(value);
			else
			{
				T* elements = _elements;
				if(_size == _capacity)
				{
					elements = allocExtend(_onceAlloc);
				}

				if (elements != _elements)
				{
					for(int32 i = 0; i < index; i++)
					{
						elements[i] = _elements[i];
					}
				}

				for (int32 i = _size; i > index; --i)
				{
					elements[i] = _elements[i-1];
				}
				elements[index] = value;

				if(elements != _elements)
				{
					delete [] _elements;
					_elements = elements;
				}
				_size++;
			}
		}

		inline void insert(int32 index,const InternalFastDynamicArray<T>& other)
		{
			int32 size = _size + other._size;
			if(size >= _capacity)
			{
				extend(other._size);
			}

			for (int32 i = 0; i < other._size; ++i)
			{
				_elements[i + index + other._size] = _elements[i + index]; 
				_elements[i + index] = other._elements[i];
			}

			_size = size;
		}

		inline void erase(int32 index)
		{
			if (_size == 0)
				return;

			for (int32 i = index; i < _size - 1; ++i)
			{
				_elements[i] = _elements[i + 1];
			}

			_size--;
		}

		inline void erase(int32 startIndex,int32 endIndex)
		{
			if(endIndex == -1 || endIndex >= _size)
			{
				_size = _size - (endIndex - startIndex);
			}
			else
			{
				for(int32 i = 0; i < _size - endIndex; i++)
				{
					_elements[startIndex + i] = _elements[endIndex + i];
				}
				_size = _size - (endIndex - startIndex);
			}
		}

		inline void resize(int32 size)
		{
			int32 extendSize = size - _size;
			if(extendSize <= 0)
				return;
			extend(extendSize);
			_size = size;
			_capacity = _size;
		}

		inline void clear()
		{
			delete [] _elements;
			_elements = nullptr;
			_size = 0;
			_capacity = 0;
		}

		inline void operator = (const InternalFastDynamicArray<T>& internalFastDynamicArray)
		{
			clear();

			if(internalFastDynamicArray.size() == 0)
				return;

			extend(internalFastDynamicArray._capacity);

			for (int32 i = 0; i < internalFastDynamicArray._size; ++i)
			{
				push_back(internalFastDynamicArray._elements[i]);
			}

			_onceAlloc = internalFastDynamicArray._onceAlloc;
		}

		inline void swap(InternalFastDynamicArray<T>& internalFastDynamicArray)
		{		
			T* tempT;
			tempT = internalFastDynamicArray._elements;
			internalFastDynamicArray._elements = _elements;
			_elements = tempT;

			int temp;
			temp= internalFastDynamicArray._size;
			internalFastDynamicArray._size = _size;
			_size = temp;

			temp = internalFastDynamicArray._capacity;
			internalFastDynamicArray._capacity = _capacity;
			_capacity = temp;

			temp = internalFastDynamicArray._onceAlloc;
			internalFastDynamicArray._onceAlloc = _onceAlloc;
			_onceAlloc = temp;
		}

		inline T* detachPointer()
		{
			_size = 0;
			_capacity = 0;
			T* t = _elements;
			_elements = nullptr;
			return t;
		}

		~InternalFastDynamicArray()
		{
			delete[] _elements;
		}
	private:

		inline void extend(int32 extend)
		{
			T* newElements = allocExtend(extend);
			for(int32 i =0; i < _size; i++)
			{
				newElements[i] = _elements[i];
			}
			delete[] _elements;
			_elements = newElements;
		}

		inline T* allocExtend(int32 size)
		{
			size = size > 0 ? size : _capacity;
			size = size == 0 ? 10 : size;
			_capacity += size;
			T* newElements = new T[_capacity];
			return newElements;
		}

	private:
		int32 _capacity;
		int32 _onceAlloc;
	};

	/// 定义特化InternalFastArray数组开始，开发人员可以不用关心此宏
#define START_DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY(T)\
	template<>\
	class InternalFastDynamicArray<T> : public InternalFastArray<T>\
	{\
	public:

	/// 特化InternalFastArray数组的部分实现，开发人员可以不用关心此宏
#define DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY_REPEAT_CODE(T)\
	inline InternalFastDynamicArray<T>()\
	:_onceAlloc(-1),_capacity(0){_size = 0;_elements = nullptr;}\
	inline InternalFastDynamicArray<T>(int32 size)\
	:_onceAlloc(-1),_capacity(0){_size = 0;_elements = nullptr;extend(size);_size = size;}\
	inline void reserve(int32 size)\
	{\
	_onceAlloc = size;\
	}\
	inline void push_back(T const& value)\
	{\
	if (_size == _capacity)\
	extend(_onceAlloc);\
	_elements[_size] = value;\
	_size++;\
	}\
	inline void push_front(T const& value)\
	{\
	if (_size == _capacity)\
	{\
	T* newElements = allocExtend(_onceAlloc);\
	newElements[0] = value;\
	::memcpy(newElements+1,_elements,sizeof(T)*_size);\
	delete[] _elements;\
	_elements = newElements;\
	}\
	else\
	{\
	::memmove(_elements+1,_elements,sizeof(T)*_size);\
	_elements[0] = value;\
	}\
	_size++;\
	}\
	inline void pop_back()\
	{\
	erase(_size - 1);\
	}\
	inline void pop_front()\
	{\
	erase(0);\
	}\
	inline void insert(int32 index,T const& value)\
	{\
	if(index == 0)\
	push_front(value);\
			else if(index == _size)\
			push_back(value);\
			else\
	{\
	if(_size == _capacity)\
	{\
	T* newElements = allocExtend(_onceAlloc);\
	::memcpy(newElements,_elements,sizeof(T)*index);\
	newElements[index] = value;\
	::memcpy(newElements+index+1,_elements+index,sizeof(T)*(_size - index));\
	delete [] _elements;\
	_elements = newElements;\
	}else\
	{\
	::memmove(_elements+index+1,_elements+index,sizeof(T)*(_size-index));\
	_elements[index] = value;\
	}\
	_size++;\
	}\
	}\
	inline void insert(int32 index,const InternalFastDynamicArray<T>& other)\
	{\
	int32 size = _size + other._size;\
	if(size >= _capacity)\
	{\
	extend(other._size);\
	}\
	insert(index,other._elements,other._size);\
	_size = size;\
	}\
	inline void erase(int32 index)\
	{\
	if (_size == 0)\
	return;\
	::memmove(_elements + index,_elements + index + 1,sizeof(T) * (_size - index - 1));\
	_size--;\
	}\
	inline void erase(int32 startIndex,int32 endIndex)\
	{\
	if(_size == 0)return;\
	if(endIndex == -1 || endIndex >= _size){_size = _size - (endIndex - startIndex);}\
		else\
	{\
	::memmove(_elements + startIndex,_elements + endIndex,sizeof(T) * (_size - endIndex));\
	_size = _size - (endIndex - startIndex);\
	}\
	}\
	inline void resize(int32 size)\
	{int32 extendSize = size - _size;if(extendSize <= 0)return;extend(extendSize);_size = size;_capacity = _size;}\
	inline void clear()\
	{\
	delete [] _elements;\
	_elements = nullptr;\
	_size = 0;\
	_capacity = 0;\
	}\
	inline void operator = (const InternalFastDynamicArray<T>& internalFastDynamicArray)\
	{\
	clear();\
	extend(internalFastDynamicArray._capacity);\
	::memcpy(_elements, internalFastDynamicArray._elements, sizeof(T) * internalFastDynamicArray._size);\
	_size = internalFastDynamicArray._size;\
	_onceAlloc = internalFastDynamicArray._onceAlloc;\
	}\
	inline void swap(InternalFastDynamicArray<T>& internalFastDynamicArray)\
	{\
	T* tempT;\
	tempT = internalFastDynamicArray._elements;\
	internalFastDynamicArray._elements = _elements;\
	_elements = tempT;\
	int temp;\
	temp= internalFastDynamicArray._size;\
	internalFastDynamicArray._size = _size;\
	_size = temp;\
	temp = internalFastDynamicArray._capacity;\
	internalFastDynamicArray._capacity = _capacity;\
	_capacity = temp;\
	temp = internalFastDynamicArray._onceAlloc;\
	internalFastDynamicArray._onceAlloc = _onceAlloc;\
	_onceAlloc = temp;\
	}\
	~InternalFastDynamicArray(){delete[] _elements;}\
	private:\
	inline void extend(int32 extend)\
	{\
	T* newElements = allocExtend(extend);\
	::memcpy(newElements,_elements,sizeof(T)*_size);\
	delete[] _elements;\
	_elements = newElements;\
	}\
	inline T* allocExtend(int32 size)\
	{\
	size = size > 0 ? size : _capacity;\
	size = size == 0 ? 10 : size;\
	_capacity += size;\
	T* newElements = new T[_capacity];\
	::memset(newElements,0,sizeof(T)*_capacity);\
	return newElements;\
	}\
	inline void insert(int32 start,T* src,int32 size)\
	{\
	::memmove(_elements+start+size,_elements+start,sizeof(T)*(_size - start));\
	::memcpy(_elements+start,src,sizeof(T)*size);\
	}\
	inline T* detachPointer()\
	{\
	_size = 0;\
	_capacity = 0;\
	T* t = _elements;\
	_elements = nullptr;\
	return t;\
	}\
	private:\
	int32 _capacity;\
		int32 _onceAlloc;

	/// 定义特化InternalFastArray数组结束，开发人员可以不用关心此宏
#define END_DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY };

	/// 定义特化InternalFastArray，开发人员可以不用关心此宏
#define DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY(T)\
	START_DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY(T)##DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY_REPEAT_CODE(T)##END_DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY

	/////////////////////////////////////////////////////////////////////////////////
	/// \brief
	/// 特化声明指针类型使用内存复制法，开发人员可不用关心此类
	///
	template<typename T>
	class InternalFastDynamicArray<T*> : public InternalFastArray<T*>
	{
	public:
		/// 定义类的函数
		DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY_REPEAT_CODE(T*)
	};

	/// 定义特化InternalFastArray，开发人员可以不用关心此宏
#define DYNAMIC_ARRAY_REPEAT_NOSTL_CODE(T,ARRAYTYPE)\
	inline DynamicArray() { _s = InternalFastDynamicArray<T>();}\
	inline DynamicArray(const DynamicArray& other) { _s = other._s;}\
	inline DynamicArray(int32 size) { _s = InternalFastDynamicArray<T>(size);}\
	inline void reserve(int32 size) { _s.reserve(size); }\
	inline void push_front(T const& value) { _s.push_front(value);}\
	inline void pop_front(){_s.pop_front();}\
	inline void insert(int32 index,const ARRAYTYPE& other)\
	{if(index > size())return;_s.insert(index,other._s);}\
	inline void insert(int32 index,T const& value){if(index > size())return;_s.insert(index,value);}\
	inline void erase(int32 index)\
	{if(index > size() - 1)return;_s.erase(index);}\
	inline void erase(int32 startIndex,int32 endIndex){ _s.erase(startIndex,endIndex);}\
	inline void eraseValue(T const& value) \
	{int32 i = index(value);if(i == -1)return;_s.erase(i);}\
	inline ARRAYTYPE& assign(const ARRAYTYPE& dArr)\
	{_s = dArr._s;return *this;}\
	inline T* detachPointer() { return _s.detachPointer();}

	/// 动态数组的重复代码实现，开发人员可以不用关心此宏
#define DYNAMIC_ARRAY_REPEAT_CODE(T,ARRAYTYPE)\
	inline void resize(int32 size) { _s.resize(size);}\
	inline void push_back(T const& value) { _s.push_back(value);}\
	inline void pop_back() { if(_s.size() == 0)return;_s.pop_back();}\
	inline void clear(){_s.clear();}\
	inline void swap(ARRAYTYPE& dArr){_s.swap(dArr._s);}\
	~ARRAYTYPE() {}

	/////////////////////////////////////////////////////////////////////////////////
	/// \brief
	/// 动态数组，类似于std::vector
	///
	template<typename T>
	class DynamicArray : public Array<T,InternalFastDynamicArray<T>,typename InternalFastDynamicArray<T>::iterator,typename InternalFastDynamicArray<T>::const_iterator,
		typename InternalFastDynamicArray<T>::reverse_iterator,typename InternalFastDynamicArray<T>::const_reverse_iterator>
	{
	public:

		/// 定义类的函数
		DYNAMIC_ARRAY_REPEAT_CODE(T,DynamicArray<T>);

		/// 定义类的函数
		DYNAMIC_ARRAY_REPEAT_NOSTL_CODE(T,DynamicArray<T>);
	};

	/// 如果用“内存复制的方式”使用数组，使用此宏特化类型，小心使用
#define DECLARE_SAMLLTYPE_ARRARY(T)\
	DECLARE_SAMLLTYPE_FAST_DYANMIC_ARRAY(T)

	///为T类型的数组命名别名为：TArray
#define DECLARE_TYPEDEF_ARRAY(T)\
	typedef wyin::DynamicArray<T> T##Array;

	///特化T数组后起别名
#define DECLARE_AND_TYPEDEF_SAMLLTYPE_ARRARY(T)\
	DECLARE_SAMLLTYPE_ARRARY(T)##DECLARE_TYPEDEF_ARRAY(T)
}

#endif