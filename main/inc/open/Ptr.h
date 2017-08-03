#ifndef WYIN_PTR_H
#define WYIN_PTR_H

namespace open
{
	template<typename T>
	class Ptr
	{
	public:

		inline Ptr()
			: _ptr(0)
		{
		}

		inline Ptr(T* ptr)
			: _ptr(ptr)
		{
			if (_ptr)
				_ptr->addRef();
		}

		inline Ptr(const Ptr& rp)
			: _ptr(rp._ptr)
		{
			if (_ptr)
				_ptr->addRef();
		}

		template<typename Other>
		inline Ptr(const Ptr<Other>& rp)
			: _ptr(rp._ptr)
		{
			if (_ptr)
				_ptr->addRef();
		}

		inline ~Ptr()
		{
			if (_ptr)
				_ptr->release();
			_ptr = 0;
		}

		inline Ptr& operator = (const Ptr& rp)
		{
			assign(rp);
			return *this;
		}

		template<typename Other>
		inline Ptr& operator = (const Ptr<Other>& rp)
		{
			assign(rp);
			return *this;
		}

		inline Ptr& operator = (T* ptr)
		{
			set(ptr);
			return *this;
		}

		inline void set(T* ptr)
		{
			if (_ptr == ptr)
				return;

			T* tmp_ptr = _ptr;
			_ptr = ptr;
			if (_ptr)
				_ptr->addRef();

			if (tmp_ptr)
				tmp_ptr->release();
		}

		inline operator T*() const { return _ptr; }

		inline T& operator*() const { return *_ptr; }

		inline T* operator->() const { return _ptr; }

		inline bool operator!() const { return _ptr == 0; }

		inline T* get() const { return _ptr; }

		inline bool valid() const { return _ptr != 0; }

		inline T* detach()
		{
			T* tmp = _ptr;
			if (_ptr)
				_ptr->releaseRefCountOnly();
			_ptr = 0;
			return tmp;
		}

		inline void swap(Ptr& rp)
		{
			T* tmp = _ptr;
			_ptr = rp._ptr;
			rp._ptr = tmp;
		}

	protected:

		template<typename Other>
		inline void assign(const Ptr<Other>& rp)
		{
			if (_ptr == rp._ptr)
				return;
			T* tmp_ptr = _ptr;
			_ptr = rp._ptr;
			if (_ptr)
				_ptr->addRef();

			if (tmp_ptr)
				tmp_ptr->release();
		}

		template<typename Other> friend class Ptr;

		T* _ptr;
	};

	template<typename T>
	inline void swap(Ptr<T>& rp1, Ptr<T>& rp2) { rp1.swap(rp2); }

	template<class T>
	inline T* get_pointer(const Ptr<T>& rp) { return rp.get(); }

	template<class T, class Y>
	inline Ptr<T> static_pointer_cast(const Ptr<Y>& rp) { return static_cast<T*>(rp.get()); }

	template<class T, class Y>
	inline Ptr<T> dynamic_pointer_cast(const Ptr<Y>& rp) { return dynamic_cast<T*>(rp.get()); }

	template<class T, class Y>
	inline Ptr<T> const_pointer_cast(const Ptr<Y>& rp) { return const_cast<T*>(rp.get()); }

}

#endif