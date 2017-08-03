namespace open
{
	///判断是否是2的n次方
	template<typename T>
	inline bool isPO2(T n)
	{
		return (n & (n - 1)) == 0;
	}

	inline unsigned int log2i(uint32 x)
	{
		x |= 1;
		uint32  n = 31;
		if (x & 0xffff0000) { n -= 16;  x >>= 16; }
		if (x & 0xff00) { n -= 8;   x >>= 8; }
		if (x & 0xf0) { n -= 4;   x >>= 4; }
		if (x & 0xc) { n -= 2;   x >>= 2; }
		if (x & 0x2) { n--; }

		return 31 - n;
	}

	/// inc dec
	inline int32 incm3(int32 i) { return i + 1 & (i - 2) >> 31; }

	inline int32 decm3(int32 i) { return i - 1 + ((i - 1) >> 31 & 3); }

	template<typename T>
	inline T clampTo(T v, T min, T max)
	{
		return v < min ? min : v > max ? max : v;
	}

	template <typename T>
	inline Vec2<T>::Vec2() { _v[0] = 0.0; _v[1] = 0.0; }

	template <typename T>
	inline Vec2<T>::Vec2(value_type x, value_type y) { _v[0] = x; _v[1] = y; }

	template <typename T>
	inline Vec2<T>::Vec2(const Vec2<T>& vec) { _v[0] = vec._v[0]; _v[1] = vec._v[1]; }

	template <typename T>
	inline bool Vec2<T>::operator == (const Vec2<T>& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1]; }

	template <typename T>
	inline bool Vec2<T>::operator != (const Vec2<T>& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1]; }

	template <typename T>
	inline bool Vec2<T>::operator <  (const Vec2<T>& v) const
	{
		if (_v[0] < v._v[0])
			return true;
		else if (_v[0] > v._v[0])
			return false;
		else
			return (_v[1] < v._v[1]);
	}

	template <typename T>
	inline typename Vec2<T>::value_type* Vec2<T>::ptr() { return _v; }

	template <typename T>
	inline const typename Vec2<T>::value_type* Vec2<T>::ptr() const { return _v; }

	template <typename T>
	inline void Vec2<T>::set(typename Vec2<T>::value_type x, typename Vec2<T>::value_type y) { _v[0] = x; _v[1] = y; }

	template <typename T>
	inline typename Vec2<T>::value_type& Vec2<T>::operator [] (int i) { return _v[i]; }

	template <typename T>
	inline typename Vec2<T>::value_type Vec2<T>::operator [] (int i) const { return _v[i]; }

	template <typename T>
	inline typename Vec2<T>::value_type& Vec2<T>::x() { return _v[0]; }

	template <typename T>
	inline typename Vec2<T>::value_type& Vec2<T>::y() { return _v[1]; }

	template <typename T>
	inline typename Vec2<T>::value_type Vec2<T>::x() const { return _v[0]; }

	template <typename T>
	inline typename Vec2<T>::value_type Vec2<T>::y() const { return _v[1]; }

	template <typename T>
	inline bool Vec2<T>::valid() const { return !isNaN(); }

	template <typename T>
	inline bool Vec2<T>::isNaN() const { return open::isNaN(_v[0]) || open::isNaN(_v[1]); }

	template <typename T>
	inline typename Vec2<T>::value_type Vec2<T>::operator * (const Vec2<T>& rhs) const
	{
		return _v[0] * rhs._v[0] + _v[1] * rhs._v[1];
	}

	template <typename T>
	inline Vec2<T> Vec2<T>::operator * (typename Vec2<T>::value_type rhs) const
	{
		return Vec2<T>(_v[0] * rhs, _v[1] * rhs);
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator *= (typename Vec2<T>::value_type rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		return *this;
	}

	template <typename T>
	inline Vec2<T> Vec2<T>::operator / (typename Vec2<T>::value_type rhs) const
	{
		return Vec2<T>(_v[0] / rhs, _v[1] / rhs);
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator /= (typename Vec2<T>::value_type rhs)
	{
		_v[0] /= rhs;
		_v[1] /= rhs;
		return *this;
	}

	template <typename T>
	inline Vec2<T> Vec2<T>::operator + (const Vec2<T>& rhs) const
	{
		return Vec2<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1]);
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator += (const Vec2<T>& rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		return *this;
	}

	template <typename T>
	inline Vec2<T> Vec2<T>::operator - (const Vec2<T>& rhs) const
	{
		return Vec2<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1]);
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator -= (const Vec2<T>& rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		return *this;
	}

	template <typename T>
	inline Vec2<T> Vec2<T>::operator - () const
	{
		return Vec2<T>(-_v[0], -_v[1]);
	}

	template <typename T>
	inline typename Vec2<T>::value_type Vec2<T>::length() const
	{
		return sqrt(_v[0] * _v[0] + _v[1] * _v[1]);
	}

	template <typename T>
	inline typename Vec2<T>::value_type Vec2<T>::length2(void) const
	{
		return _v[0] * _v[0] + _v[1] * _v[1];
	}

	template <typename T>
	inline typename Vec2<T>::value_type Vec2<T>::normalize()
	{
		value_type norm = Vec2<T>::length();
		if (norm > 0.0)
		{
			value_type inv = 1.0f / norm;
			_v[0] *= inv;
			_v[1] *= inv;
		}
		return(norm);
	}

	template<typename T>
	inline Vec3<T>::Vec3() { _v[0] = 0.0; _v[1] = 0.0; _v[2] = 0.0; }

	template<typename T>
	inline Vec3<T>::Vec3(const Vec3<T>& vec) { _v[0] = vec._v[0]; _v[1] = vec._v[1]; _v[2] = vec._v[2]; }

	template<typename T>
	inline Vec3<T>::Vec3(typename Vec3<T>::value_type x, typename Vec3<T>::value_type y, typename Vec3<T>::value_type z) { _v[0] = x; _v[1] = y; _v[2] = z; }

	template<typename T>
	inline bool Vec3<T>::operator == (const Vec3<T>& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2]; }

	template<typename T>
	inline bool Vec3<T>::operator != (const Vec3<T>& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2]; }

	template<typename T>
	inline bool Vec3<T>::operator <  (const Vec3<T>& v) const
	{
		if (_v[0] < v._v[0])
			return true;
		else if (_v[0] > v._v[0])
			return false;
		else if (_v[1] < v._v[1])
			return true;
		else if (_v[1] > v._v[1])
			return false;
		else
			return (_v[2] < v._v[2]);
	}

	template<typename T>
	inline typename Vec3<T>::value_type* Vec3<T>::ptr() { return _v; }

	template<typename T>
	inline const typename Vec3<T>::value_type* Vec3<T>::ptr() const { return _v; }

	template<typename T>
	inline void Vec3<T>::set(typename Vec3<T>::value_type x, typename Vec3<T>::value_type y, typename Vec3<T>::value_type z)
	{
		_v[0] = x; _v[1] = y; _v[2] = z;
	}

	template<typename T>
	inline void Vec3<T>::set(const Vec3<T>& rhs)
	{
		_v[0] = rhs._v[0]; _v[1] = rhs._v[1]; _v[2] = rhs._v[2];
	}

	template<typename T>
	inline typename Vec3<T>::value_type& Vec3<T>::operator [] (int i) { return _v[i]; }

	template<typename T>
	inline typename Vec3<T>::value_type Vec3<T>::operator [] (int i) const { return _v[i]; }

	template<typename T>
	inline typename Vec3<T>::value_type& Vec3<T>::x() { return _v[0]; }

	template<typename T>
	inline typename Vec3<T>::value_type& Vec3<T>::y() { return _v[1]; }

	template<typename T>
	inline typename Vec3<T>::value_type& Vec3<T>::z() { return _v[2]; }

	template<typename T>
	inline typename Vec3<T>::value_type Vec3<T>::x() const { return _v[0]; }

	template<typename T>
	inline typename Vec3<T>::value_type Vec3<T>::y() const { return _v[1]; }

	template<typename T>
	inline typename Vec3<T>::value_type Vec3<T>::z() const { return _v[2]; }

	template<typename T>
	inline bool Vec3<T>::valid() const { return !isNaN(); }

	template<typename T>
	inline bool Vec3<T>::isNaN() const { return open::isNaN(_v[0]) || open::isNaN(_v[1]) || open::isNaN(_v[2]); }

	template<typename T>
	inline typename Vec3<T>::value_type Vec3<T>::operator * (const Vec3<T>& rhs) const
	{
		return _v[0] * rhs._v[0] + _v[1] * rhs._v[1] + _v[2] * rhs._v[2];
	}

	template<typename T>
	inline Vec3<T> Vec3<T>::operator ^ (const Vec3<T>& rhs) const
	{
		return Vec3<T>(_v[1] * rhs._v[2] - _v[2] * rhs._v[1],
			_v[2] * rhs._v[0] - _v[0] * rhs._v[2],
			_v[0] * rhs._v[1] - _v[1] * rhs._v[0]);
	}

	template<typename T>
	inline Vec3<T> Vec3<T>::operator * (typename Vec3<T>::value_type rhs) const
	{
		return Vec3<T>(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs);
	}

	template<typename T>
	inline Vec3<T>& Vec3<T>::operator *= (typename Vec3<T>::value_type rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		return *this;
	}

	template<typename T>
	inline Vec3<T> Vec3<T>::operator / (typename Vec3<T>::value_type rhs) const
	{
		return Vec3<T>(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs);
	}

	template<typename T>
	inline Vec3<T>& Vec3<T>::operator /= (typename Vec3<T>::value_type rhs)
	{
		_v[0] /= rhs;
		_v[1] /= rhs;
		_v[2] /= rhs;
		return *this;
	}

	template<typename T>
	inline Vec3<T> Vec3<T>::operator + (const Vec3<T>& rhs) const
	{
		return Vec3<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2]);
	}

	template<typename T>
	inline Vec3<T>& Vec3<T>::operator += (const Vec3<T>& rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		return *this;
	}

	template<typename T>
	inline Vec3<T> Vec3<T>::operator - (const Vec3<T>& rhs) const
	{
		return Vec3<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2]);
	}

	template<typename T>
	inline Vec3<T>& Vec3<T>::operator -= (const Vec3<T>& rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		return *this;
	}

	template<typename T>
	inline Vec3<T> Vec3<T>::operator - () const
	{
		return Vec3<T>(-_v[0], -_v[1], -_v[2]);
	}

	template<typename T>
	inline typename Vec3<T>::value_type Vec3<T>::length() const
	{
		return sqrtf(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
	}

	template<typename T>
	inline typename Vec3<T>::value_type Vec3<T>::length2() const
	{
		return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2];
	}

	template<typename T>
	inline typename Vec3<T>::value_type Vec3<T>::normalize()
	{
		value_type norm = Vec3<T>::length();
		if (norm > 0.0)
		{
			value_type inv = 1.0f / norm;
			_v[0] *= inv;
			_v[1] *= inv;
			_v[2] *= inv;
		}
		return(norm);
	}

	template<typename T>
	inline uint32 Vec3<T>::asBGR() const
	{
		return (uint32)clampTo((_v[0] * 255.0f), 0.0f, 255.0f) << 24 |
			(uint32)clampTo((_v[1] * 255.0f), 0.0f, 255.0f) << 16 |
			(uint32)clampTo((_v[2] * 255.0f), 0.0f, 255.0f) << 8;
	}

	template<typename T>
	inline uint32 Vec3<T>::asABGR() const
	{
		return asBGR();
	}

	template<typename T>
	inline uint32 Vec3<T>::asRGB() const
	{
		return (uint32)clampTo((_v[2] * 255.0f), 0.0f, 255.0f) << 16 |
			(uint32)clampTo((_v[1] * 255.0f), 0.0f, 255.0f) << 8 |
			(uint32)clampTo((_v[0] * 255.0f), 0.0f, 255.0f);
	}

	template<typename T>
	inline uint32 Vec3<T>::asRGBA() const
	{
		return (uint32)1 << 24 |
			(uint32)clampTo((_v[2] * 255.0f), 0.0f, 255.0f) << 16 |
			(uint32)clampTo((_v[1] * 255.0f), 0.0f, 255.0f) << 8 |
			(uint32)clampTo((_v[0] * 255.0f), 0.0f, 255.0f);
	}

	template<typename T>
	Vec3<T> Vec3<T>::fromRGB(uint32 rgb)
	{
		return Vec3<T>((rgb & 0xff) / 255.0f, ((rgb >> 8) & 0xff) / 255.0f, ((rgb >> 16) & 0xff) / 255.0f);
	}

	template<typename T>
	void Vec3<T>::fromRGB(Vec3<T>& vec, uint32 rgb)
	{
		vec.x() = (rgb & 0xff) / 255.0f;
		vec.y() = ((rgb >> 8) & 0xff) / 255.0f;
		vec.z() = ((rgb >> 16) & 0xff) / 255.0f;
	}

	template<typename T>
	Vec4<T>::Vec4() { _v[0] = 0.0; _v[1] = 0.0; _v[2] = 0.0; _v[3] = 0.0; }

	template<typename T>
	Vec4<T>::Vec4(typename Vec4<T>::value_type x, typename Vec4<T>::value_type y, typename Vec4<T>::value_type z, typename Vec4<T>::value_type w)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	template<typename T>
	Vec4<T>::Vec4(const Vec3<T>& v3, typename Vec4<T>::value_type w)
	{
		_v[0] = v3[0];
		_v[1] = v3[1];
		_v[2] = v3[2];
		_v[3] = w;
	}

	template<typename T>
	inline Vec4<T>::Vec4(const Vec4<T>& vec) { _v[0] = vec._v[0]; _v[1] = vec._v[1]; _v[2] = vec._v[2]; _v[3] = vec._v[3]; }

	template<typename T>
	inline bool Vec4<T>::operator == (const Vec4<T>& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3]; }

	template<typename T>
	inline bool Vec4<T>::operator != (const Vec4<T>& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2] || _v[3] != v._v[3]; }

	template<typename T>
	inline bool Vec4<T>::operator <  (const Vec4<T>& v) const
	{
		if (_v[0] < v._v[0])
			return true;
		else if (_v[0] > v._v[0])
			return false;
		else if (_v[1] < v._v[1])
			return true;
		else if (_v[1] > v._v[1])
			return false;
		else if (_v[2] < v._v[2])
			return true;
		else if (_v[2] > v._v[2])
			return false;
		else
			return (_v[3] < v._v[3]);
	}

	template<typename T>
	inline typename Vec4<T>::value_type* Vec4<T>::ptr() { return _v; }

	template<typename T>
	inline const typename Vec4<T>::value_type* Vec4<T>::ptr() const { return _v; }

	template<typename T>
	inline void Vec4<T>::set(typename Vec4<T>::value_type x, typename Vec4<T>::value_type y, typename Vec4<T>::value_type z, typename Vec4<T>::value_type w)
	{
		_v[0] = x; _v[1] = y; _v[2] = z; _v[3] = w;
	}

	template<typename T>
	inline typename Vec4<T>::value_type& Vec4<T>::operator [] (int i) { return _v[i]; }

	template<typename T>
	inline typename Vec4<T>::value_type  Vec4<T>::operator [] (int i) const { return _v[i]; }

	template<typename T>
	inline typename Vec4<T>::value_type& Vec4<T>::x() { return _v[0]; }

	template<typename T>
	inline typename Vec4<T>::value_type& Vec4<T>::y() { return _v[1]; }

	template<typename T>
	inline typename Vec4<T>::value_type& Vec4<T>::z() { return _v[2]; }

	template<typename T>
	inline typename Vec4<T>::value_type& Vec4<T>::w() { return _v[3]; }

	template<typename T>
	inline typename Vec4<T>::value_type Vec4<T>::x() const { return _v[0]; }

	template<typename T>
	inline typename Vec4<T>::value_type Vec4<T>::y() const { return _v[1]; }

	template<typename T>
	inline typename Vec4<T>::value_type Vec4<T>::z() const { return _v[2]; }

	template<typename T>
	inline typename Vec4<T>::value_type Vec4<T>::w() const { return _v[3]; }

	template<typename T>
	inline uint32 Vec4<T>::asABGR() const
	{
		return (uint32)clampTo((_v[0] * 255.0), 0.0, 255.0) << 24 |
			(uint32)clampTo((_v[1] * 255.0), 0.0, 255.0) << 16 |
			(uint32)clampTo((_v[2] * 255.0), 0.0, 255.0) << 8 |
			(uint32)clampTo((_v[3] * 255.0), 0.0, 255.0);
	}

	template<typename T>
	inline uint32 Vec4<T>::asRGBA() const
	{
		return (uint32)clampTo((_v[3] * 255.0), 0.0, 255.0) << 24 |
			(uint32)clampTo((_v[2] * 255.0), 0.0, 255.0) << 16 |
			(uint32)clampTo((_v[1] * 255.0), 0.0, 255.0) << 8 |
			(uint32)clampTo((_v[0] * 255.0), 0.0, 255.0);
	}

	template<typename T>
	inline bool Vec4<T>::valid() const { return !isNaN(); }

	template<typename T>
	inline bool Vec4<T>::isNaN() const { return open::isNaN(_v[0]) || open::isNaN(_v[1]) || open::isNaN(_v[2]) || open::isNaN(_v[3]); }

	template<typename T>
	inline typename Vec4<T>::value_type Vec4<T>::operator * (const Vec4<T>& rhs) const
	{
		return _v[0] * rhs._v[0] +
			_v[1] * rhs._v[1] +
			_v[2] * rhs._v[2] +
			_v[3] * rhs._v[3];
	}

	template<typename T>
	inline Vec4<T> Vec4<T>::operator * (typename Vec4<T>::value_type rhs) const
	{
		return Vec4<T>(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs, _v[3] * rhs);
	}

	template<typename T>
	inline Vec4<T> Vec4<T>::operator ^ (const Vec4<T>& rhs) const
	{
		return Vec4<T>(_v[0] * rhs._v[0], _v[1] * rhs._v[1], _v[2] * rhs._v[2], _v[3] * rhs._v[3]);
	}

	template<typename T>
	inline Vec4<T>& Vec4<T>::operator *= (value_type rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		_v[3] *= rhs;
		return *this;
	}


	template<typename T>
	inline Vec4<T> Vec4<T>::operator / (typename Vec4<T>::value_type rhs) const
	{
		return Vec4<T>(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs, _v[3] / rhs);
	}

	template<typename T>
	inline Vec4<T>& Vec4<T>::operator /= (typename Vec4<T>::value_type rhs)
	{
		_v[0] /= rhs;
		_v[1] /= rhs;
		_v[2] /= rhs;
		_v[3] /= rhs;
		return *this;
	}

	template<typename T>
	inline Vec4<T> Vec4<T>::operator + (const Vec4<T>& rhs) const
	{
		return Vec4<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2], _v[3] + rhs._v[3]);
	}

	template<typename T>
	inline Vec4<T>& Vec4<T>::operator += (const Vec4<T>& rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		_v[3] += rhs._v[3];
		return *this;
	}

	template<typename T>
	inline Vec4<T> Vec4<T>::operator - (const Vec4<T>& rhs) const
	{
		return Vec4<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2], _v[3] - rhs._v[3]);
	}

	template<typename T>
	inline Vec4<T>& Vec4<T>::operator -= (const Vec4<T>& rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		_v[3] -= rhs._v[3];
		return *this;
	}

	template<typename T>
	inline Vec4<T> Vec4<T>::operator - () const
	{
		return Vec4<T>(-_v[0], -_v[1], -_v[2], -_v[3]);
	}

	template<typename T>
	inline typename Vec4<T>::value_type Vec4<T>::length() const
	{
#if DOUBLE_PRECISION
		return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3]);

#else
		return sqrtf(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3]);
#endif
	}

	template<typename T>
	inline typename Vec4<T>::value_type Vec4<T>::length2() const
	{
		return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3];
	}

	template<typename T>
	inline typename Vec4<T>::value_type Vec4<T>::normalize()
	{
		value_type norm = Vec4<T>::length();
		if (norm > 0.0f)
		{
			value_type inv = 1.0f / norm;
			_v[0] *= inv;
			_v[1] *= inv;
			_v[2] *= inv;
			_v[3] *= inv;
		}
		return(norm);
	}

	template<typename T>
	inline Quat<T>::Quat() { _v[0] = 0.0f; _v[1] = 0.0f; _v[2] = 0.0f; _v[3] = 1.0f; }

	template<typename T>
	inline Quat<T>::Quat(typename Quat<T>::value_type x, typename Quat<T>::value_type y, typename Quat<T>::value_type z, typename Quat<T>::value_type w)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	template<typename T>
	inline Quat<T>::Quat(const Vec4<T>& v)
	{
		_v[0] = v.x();
		_v[1] = v.y();
		_v[2] = v.z();
		_v[3] = v.w();
	}

	template<typename T>
	inline Quat<T>::Quat(typename Quat<T>::value_type angle, const Vec3<T>& axis)
	{
		makeRotate(angle, axis);
	}

	template<typename T>
	inline Quat<T>::Quat(typename Quat<T>::value_type angle1, const Vec3<T>& axis1,
		typename Quat<T>::value_type angle2, const Vec3<T>& axis2,
		typename Quat<T>::value_type angle3, const Vec3<T>& axis3)
	{
		makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
	}

	template<typename T>
	inline Quat<T>& Quat<T>::operator = (const Quat<T>& v) { _v[0] = v._v[0];  _v[1] = v._v[1]; _v[2] = v._v[2]; _v[3] = v._v[3]; return *this; }

	template<typename T>
	inline bool Quat<T>::operator == (const Quat<T>& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3]; }

	template<typename T>
	inline bool Quat<T>::operator != (const Quat<T>& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2] || _v[3] != v._v[3]; }

	template<typename T>
	inline bool Quat<T>::operator <  (const Quat<T>& v) const
	{
		if (_v[0] < v._v[0])
			return true;
		else if (_v[0] > v._v[0])
			return false;
		else if (_v[1] < v._v[1])
			return true;
		else if (_v[1] > v._v[1])
			return false;
		else if (_v[2] < v._v[2])
			return true;
		else if (_v[2] > v._v[2])
			return false;
		else
			return (_v[3] < v._v[3]);
	}

	template<typename T>
	inline void Quat<T>::set(typename Quat<T>::value_type x, typename Quat<T>::value_type y, typename Quat<T>::value_type z, typename Quat<T>::value_type w)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	template<typename T>
	inline typename Quat<T>::value_type & Quat<T>::operator [] (int i) { return _v[i]; }

	template<typename T>
	inline typename Quat<T>::value_type   Quat<T>::operator [] (int i) const { return _v[i]; }

	template<typename T>
	inline typename Quat<T>::value_type & Quat<T>::x() { return _v[0]; }

	template<typename T>
	inline typename Quat<T>::value_type & Quat<T>::y() { return _v[1]; }

	template<typename T>
	inline typename Quat<T>::value_type & Quat<T>::z() { return _v[2]; }

	template<typename T>
	inline typename Quat<T>::value_type & Quat<T>::w() { return _v[3]; }

	template<typename T>
	inline typename Quat<T>::value_type Quat<T>::x() const { return _v[0]; }

	template<typename T>
	inline typename Quat<T>::value_type Quat<T>::y() const { return _v[1]; }

	template<typename T>
	inline typename Quat<T>::value_type Quat<T>::z() const { return _v[2]; }

	template<typename T>
	inline typename Quat<T>::value_type Quat<T>::w() const { return _v[3]; }

	template<typename T>
	bool Quat<T>::zeroRotation() const { return _v[0] == 0.0 && _v[1] == 0.0 && _v[2] == 0.0 && _v[3] == 1.0; }

	template<typename T>
	inline Quat<T> Quat<T>::operator * (typename Quat<T>::value_type rhs) const
	{
		return Quat<T>(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs, _v[3] * rhs);
	}

	template<typename T>
	inline Quat<T>& Quat<T>::operator *= (typename Quat<T>::value_type rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		_v[3] *= rhs;
		return *this;
	}

	template<typename T>
	inline Quat<T> Quat<T>::operator*(const Quat<T>& rhs) const
	{
		return Quat<T>(rhs._v[3] * _v[0] + rhs._v[0] * _v[3] + rhs._v[1] * _v[2] - rhs._v[2] * _v[1],
			rhs._v[3] * _v[1] - rhs._v[0] * _v[2] + rhs._v[1] * _v[3] + rhs._v[2] * _v[0],
			rhs._v[3] * _v[2] + rhs._v[0] * _v[1] - rhs._v[1] * _v[0] + rhs._v[2] * _v[3],
			rhs._v[3] * _v[3] - rhs._v[0] * _v[0] - rhs._v[1] * _v[1] - rhs._v[2] * _v[2]);
	}

	template<typename T>
	inline Quat<T>& Quat<T>::operator*=(const Quat<T>& rhs)
	{
		value_type x = rhs._v[3] * _v[0] + rhs._v[0] * _v[3] + rhs._v[1] * _v[2] - rhs._v[2] * _v[1];
		value_type y = rhs._v[3] * _v[1] - rhs._v[0] * _v[2] + rhs._v[1] * _v[3] + rhs._v[2] * _v[0];
		value_type z = rhs._v[3] * _v[2] + rhs._v[0] * _v[1] - rhs._v[1] * _v[0] + rhs._v[2] * _v[3];
		_v[3] = rhs._v[3] * _v[3] - rhs._v[0] * _v[0] - rhs._v[1] * _v[1] - rhs._v[2] * _v[2];

		_v[2] = z;
		_v[1] = y;
		_v[0] = x;

		return (*this);
	}

	template<typename T>
	inline Quat<T> Quat<T>::operator / (typename Quat<T>::value_type rhs) const
	{
		value_type div = 1.0 / rhs;
		return Quat<T>(_v[0] * div, _v[1] * div, _v[2] * div, _v[3] * div);
	}

	template<typename T>
	inline Quat<T>& Quat<T>::operator /= (typename Quat<T>::value_type rhs)
	{
		value_type div = 1.0 / rhs;
		_v[0] *= div;
		_v[1] *= div;
		_v[2] *= div;
		_v[3] *= div;
		return *this;
	}

	template<typename T>
	inline Quat<T> Quat<T>::operator / (const Quat<T>& denom) const
	{
		return ((*this) * denom.inverse());
	}

	template<typename T>
	inline Quat<T>& Quat<T>::operator /= (const Quat<T>& denom)
	{
		(*this) = (*this) * denom.inverse();
		return (*this);
	}

	template<typename T>
	inline Quat<T> Quat<T>::operator + (const Quat<T>& rhs) const
	{
		return Quat<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1],
			_v[2] + rhs._v[2], _v[3] + rhs._v[3]);
	}

	template<typename T>
	inline Quat<T>& Quat<T>::operator += (const Quat<T>& rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		_v[3] += rhs._v[3];
		return *this;
	}

	template<typename T>
	inline Quat<T> Quat<T>::operator - (const Quat<T>& rhs) const
	{
		return Quat<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1],
			_v[2] - rhs._v[2], _v[3] - rhs._v[3]);
	}

	template<typename T>
	inline Quat<T>& Quat<T>::operator -= (const Quat<T>& rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		_v[3] -= rhs._v[3];
		return *this;
	}

	template<typename T>
	inline Quat<T> Quat<T>::operator - () const
	{
		return Quat<T>(-_v[0], -_v[1], -_v[2], -_v[3]);
	}

	template<typename T>
	typename Quat<T>::value_type Quat<T>::length() const
	{
		return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3]);
	}

	template<typename T>
	typename Quat<T>::value_type Quat<T>::length2() const
	{
		return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3];
	}

	template<typename T>
	inline Quat<T> Quat<T>::conj() const
	{
		return Quat<T>(-_v[0], -_v[1], -_v[2], _v[3]);
	}

	template<typename T>
	inline Quat<T> Quat<T>::getInverse() const
	{
		return conj() / length2();
	}

	template<typename T>
	inline Quat<T>& Quat<T>::inverse()
	{
		value_type l_2 = length2();
		_v[0] = -_v[0] / l_2;
		_v[1] = -_v[1] / l_2;
		_v[2] = -_v[2] / l_2;
		_v[3] /= l_2;

		return *this;
	}

	template<typename T>
	void Quat<T>::makeRotate(typename Quat<T>::value_type angle, typename Quat<T>::value_type x, typename Quat<T>::value_type y, typename Quat<T>::value_type z)
	{
		const value_type epsilon = 0.0000001f;

		value_type length = sqrt(x * x + y * y + z * z);
		if (length < epsilon)
		{
			// ~zero length axis, so reset rotation to zero.
			*this = Quat<T>();
			return;
		}

		value_type inversenorm = 1.0f / length;
		value_type coshalfangle = cos(0.5f * angle);
		value_type sinhalfangle = sin(0.5f * angle);

		_v[0] = x * sinhalfangle * inversenorm;
		_v[1] = y * sinhalfangle * inversenorm;
		_v[2] = z * sinhalfangle * inversenorm;
		_v[3] = coshalfangle;
	}

	template<typename T>
	void Quat<T>::makeRotate(typename Quat<T>::value_type angle, const Vec3<T>& vec)
	{
		makeRotate(angle, vec[0], vec[1], vec[2]);
	}

	template<typename T>
	void Quat<T>::makeRotate(typename Quat<T>::value_type angle1, const Vec3<T>& axis1,
		typename Quat<T>::value_type angle2, const Vec3<T>& axis2,
		typename Quat<T>::value_type angle3, const Vec3<T>& axis3)
	{
		Quat<T> q1; q1.makeRotate(angle1, axis1);
		Quat<T> q2; q2.makeRotate(angle2, axis2);
		Quat<T> q3; q3.makeRotate(angle3, axis3);

		*this = q1*q2*q3;
	}

	template<typename T>
	void Quat<T>::makeRotate(const Vec3<T>& from, const Vec3<T>& to)
	{
		Vec3d sourceVector = from;
		Vec3d targetVector = to;

		value_type fromLen2 = from.length2();
		value_type fromLen;

		if ((fromLen2 < 1.0 - 1e-7) || (fromLen2 > 1.0 + 1e-7))
		{
			fromLen = sqrt(fromLen2);
			sourceVector /= fromLen;
		}
		else
			fromLen = 1.0;

		value_type toLen2 = to.length2();
		if ((toLen2 < 1.0 - 1e-7) || (toLen2 > 1.0 + 1e-7))
		{
			value_type toLen;
			if ((toLen2 > fromLen2 - 1e-7) && (toLen2 < fromLen2 + 1e-7))
			{
				toLen = fromLen;
			}
			else
				toLen = sqrt(toLen2);
			targetVector /= toLen;
		}

		T dotProdPlus1 = 1.0 + sourceVector * targetVector;
		if (dotProdPlus1 < 1e-7)
		{
			if (fabs((float)sourceVector.x()) < 0.6)
			{
				T norm = sqrt(1.0 - sourceVector.x() * sourceVector.x());
				_v[0] = 0.0;
				_v[1] = sourceVector.z() / norm;
				_v[2] = -sourceVector.y() / norm;
				_v[3] = 0.0;
			}
			else if (fabs((float)sourceVector.y()) < 0.6)
			{
				T norm = sqrt(1.0 - sourceVector.y() * sourceVector.y());
				_v[0] = -sourceVector.z() / norm;
				_v[1] = 0.0;
				_v[2] = sourceVector.x() / norm;
				_v[3] = 0.0;
			}
			else
			{
				T norm = sqrt(1.0 - sourceVector.z() * sourceVector.z());
				_v[0] = sourceVector.y() / norm;
				_v[1] = -sourceVector.x() / norm;
				_v[2] = 0.0;
				_v[3] = 0.0;
			}
		}
		else
		{
			T s = sqrt(0.5 * dotProdPlus1);
			Vec3<T> tmp = sourceVector ^ targetVector / (2.0 * s);
			_v[0] = tmp.x();
			_v[1] = tmp.y();
			_v[2] = tmp.z();
			_v[3] = s;
		}
	}

	template<typename T>
	void Quat<T>::getRotate(typename Quat<T>::value_type & angle, typename Quat<T>::value_type & x,
		typename Quat<T>::value_type & y, typename Quat<T>::value_type & z) const
	{
		value_type sinhalfangle = sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);

		angle = 2.0 * atan2(sinhalfangle, _v[3]);
		if (sinhalfangle)
		{
			x = _v[0] / sinhalfangle;
			y = _v[1] / sinhalfangle;
			z = _v[2] / sinhalfangle;
		}
		else
		{
			x = 0.0;
			y = 0.0;
			z = 1.0;
		}
	}

	template<typename T>
	void Quat<T>::getRotate(typename Quat<T>::value_type & angle, Vec3<T>& vec) const
	{
		getRotate(angle, vec.x(), vec.y(), vec.z());
	}

	template<typename T>
	void Quat<T>::slerp(typename Quat<T>::value_type  t, const Quat& from, const Quat& to)
	{
		const T epsilon = 0.00001;
		T omega, cosomega, sinomega, scale_from, scale_to;

		Quat quatTo(to);
		cosomega = from._v[0] * to._v[0] + from._v[1] * to._v[1] + from._v[2] * to._v[2] + from._v[3] * to._v[3];

		if (cosomega < 0.0)
		{
			cosomega = -cosomega;
			quatTo = -to;
		}
		if ((1.0 - cosomega) > epsilon)
		{
			omega = acos(cosomega);
			sinomega = sin(omega);
			scale_from = sin((1.0 - t)*omega) / sinomega;
			scale_to = sin(t*omega) / sinomega;
		}
		else
		{
			scale_from = 1.0 - t;
			scale_to = t;
		}
		*this = (from * scale_from) + (quatTo * scale_to);
	}

	template<typename T>
	Vec3<T> Quat<T>::operator * (const Vec3<T>& v) const
	{
		Vec3<T> uv, uuv;
		Vec3<T> qvec(_v[0], _v[1], _v[2]);
		uv = qvec ^ v;
		uuv = qvec ^ uv;
		uv *= (2.0f * _v[3]);
		uuv *= 2.0f;
		return v + uv + uuv;
	}

	template<typename T>
	inline Matrix<T>::Matrix()
	{
		makeIdentity();
	}

	template<typename T>
	inline Matrix<T>::Matrix(const Matrix<T>& mat)
	{
		set(mat.ptr());
	}

	template<typename T>
	Matrix<T>::Matrix(typename Matrix<T>::value_type a00, typename Matrix<T>::value_type a01, typename Matrix<T>::value_type a02, typename Matrix<T>::value_type a03,
		typename Matrix<T>::value_type a10, typename Matrix<T>::value_type a11, typename Matrix<T>::value_type a12, typename Matrix<T>::value_type a13,
		typename Matrix<T>::value_type a20, typename Matrix<T>::value_type a21, typename Matrix<T>::value_type a22, typename Matrix<T>::value_type a23,
		typename Matrix<T>::value_type a30, typename Matrix<T>::value_type a31, typename Matrix<T>::value_type a32, typename Matrix<T>::value_type a33)
	{
		_m00 = a00;_m01 = a01;_m02 = a02;_m03 = a03;
		_m10 = a10;_m11 = a11;_m12 = a12;_m13 = a13;
		_m20 = a20;_m21 = a21;_m22 = a22;_m23 = a23;
		_m30 = a30;_m31 = a31;_m32 = a32;_m33 = a33;
	}

	template<typename T>
	inline const typename Matrix<T>::value_type* Matrix<T>::operator[] (int32 i) const { return _mat[i]; }

	template<typename T>
	inline typename Matrix<T>::value_type* Matrix<T>::operator[] (int32 i) { return _mat[i]; }

	template<typename T>
	inline typename Matrix<T>::value_type& Matrix<T>::operator()(int32 row, int32 col) { return _mat[row][col]; }

	template<typename T>
	inline typename Matrix<T>::value_type Matrix<T>::operator()(int32 row, int32 col) const { return _mat[row][col]; }

	template<typename T>
	inline bool Matrix<T>::valid() const { return !isNaN(); }

	template<typename T>
	inline bool Matrix<T>::isNaN() const
	{
		return open::isNaN(_mat[0][0]) || open::isNaN(_mat[0][1]) || open::isNaN(_mat[0][2]) || open::isNaN(_mat[0][3]) ||
			open::isNaN(_mat[1][0]) || open::isNaN(_mat[1][1]) || open::isNaN(_mat[1][2]) || open::isNaN(_mat[1][3]) ||
			open::isNaN(_mat[2][0]) || open::isNaN(_mat[2][1]) || open::isNaN(_mat[2][2]) || open::isNaN(_mat[2][3]) ||
			open::isNaN(_mat[3][0]) || open::isNaN(_mat[3][1]) || open::isNaN(_mat[3][2]) || open::isNaN(_mat[3][3]);
	}

	template<typename T>
	inline void Matrix<T>::set(T const * const ptr)
	{
		memcpy(_m, ptr, sizeof(T) * 16);
	}

	template<typename T>
	void Matrix<T>::set(typename Matrix<T>::value_type a00, typename Matrix<T>::value_type a01, typename Matrix<T>::value_type a02, typename Matrix<T>::value_type a03,
		typename Matrix<T>::value_type a10, typename Matrix<T>::value_type a11, typename Matrix<T>::value_type a12, typename Matrix<T>::value_type a13,
		typename Matrix<T>::value_type a20, typename Matrix<T>::value_type a21, typename Matrix<T>::value_type a22, typename Matrix<T>::value_type a23,
		typename Matrix<T>::value_type a30, typename Matrix<T>::value_type a31, typename Matrix<T>::value_type a32, typename Matrix<T>::value_type a33)
	{
		_m00 = a00;_m01 = a01;_m02 = a02;_m03 = a03;
		_m10 = a10;_m11 = a11;_m12 = a12;_m13 = a13;
		_m20 = a20;_m21 = a21;_m22 = a22;_m23 = a23;
		_m30 = a30;_m31 = a31;_m32 = a32;_m33 = a33;
	}

	template<typename T>
	inline Matrix<T>& Matrix<T>::operator = (const Matrix<T>& rhs)
	{
		set(rhs.ptr());
		return *this;
	}

	template<typename T>
	bool Matrix<T>::operator == (const Matrix<T>& m2) const
	{
		if (_mat[0][0] != m2._mat[0][0] || _mat[0][1] != m2._mat[0][1] || _mat[0][2] != m2._mat[0][2] || _mat[0][3] != m2._mat[0][3] ||
			_mat[1][0] != m2._mat[1][0] || _mat[1][1] != m2._mat[1][1] || _mat[1][2] != m2._mat[1][2] || _mat[1][3] != m2._mat[1][3] ||
			_mat[2][0] != m2._mat[2][0] || _mat[2][1] != m2._mat[2][1] || _mat[2][2] != m2._mat[2][2] || _mat[2][3] != m2._mat[2][3] ||
			_mat[3][0] != m2._mat[3][0] || _mat[3][1] != m2._mat[3][1] || _mat[3][2] != m2._mat[3][2] || _mat[3][3] != m2._mat[3][3])
			return false;
		return true;
	}

	template<typename T>
	bool Matrix<T>::operator != (const Matrix<T>& m2) const
	{
		if (_mat[0][0] != m2._mat[0][0] || _mat[0][1] != m2._mat[0][1] || _mat[0][2] != m2._mat[0][2] || _mat[0][3] != m2._mat[0][3] ||
			_mat[1][0] != m2._mat[1][0] || _mat[1][1] != m2._mat[1][1] || _mat[1][2] != m2._mat[1][2] || _mat[1][3] != m2._mat[1][3] ||
			_mat[2][0] != m2._mat[2][0] || _mat[2][1] != m2._mat[2][1] || _mat[2][2] != m2._mat[2][2] || _mat[2][3] != m2._mat[2][3] ||
			_mat[3][0] != m2._mat[3][0] || _mat[3][1] != m2._mat[3][1] || _mat[3][2] != m2._mat[3][2] || _mat[3][3] != m2._mat[3][3])
			return true;
		return false;
	}

	template<typename T>
	typename Matrix<T>::value_type * Matrix<T>::ptr() { return (value_type*)_mat; }

	template<typename T>
	const typename Matrix<T>::value_type * Matrix<T>::ptr() const { return (const value_type *)_mat; }

	template<typename T>
	bool Matrix<T>::isIdentity() const
	{
		return _mat[0][0] == 1.0 && _mat[0][1] == 0.0 && _mat[0][2] == 0.0 &&  _mat[0][3] == 0.0 &&
			_mat[1][0] == 0.0 && _mat[1][1] == 1.0 && _mat[1][2] == 0.0 &&  _mat[1][3] == 0.0 &&
			_mat[2][0] == 0.0 && _mat[2][1] == 0.0 && _mat[2][2] == 1.0 &&  _mat[2][3] == 0.0 &&
			_mat[3][0] == 0.0 && _mat[3][1] == 0.0 && _mat[3][2] == 0.0 &&  _mat[3][3] == 1.0;
	}

	template<typename T>
	void Matrix<T>::makeIdentity()
	{
		memset(_m, 0, sizeof(T) * 16);
		_mat[0][0] = 1.0;
		_mat[1][1] = 1.0;
		_mat[2][2] = 1.0;
		_mat[3][3] = 1.0;
	}

	template<typename T>
	void Matrix<T>::makeScale(typename Matrix<T>::value_type x, typename Matrix<T>::value_type y, typename Matrix<T>::value_type z)
	{
		memset(_m, 0, sizeof(T) * 16);
		_mat[0][0] = x;
		_mat[1][1] = y;
		_mat[2][2] = z;
		_mat[3][3] = 1.0;
	}

	template<typename T>
	void Matrix<T>::makeScale(const Vec3<T>& v)
	{
		makeScale(v.x(), v.y(), v.z());
	}

	template<typename T>
	void Matrix<T>::makeTranslate(typename Matrix<T>::value_type x, typename Matrix<T>::value_type y, typename Matrix<T>::value_type z)
	{
		makeIdentity();
		_mat[0][3] = x;
		_mat[1][3] = y;
		_mat[2][3] = z;
	}

	template<typename T>
	void Matrix<T>::makeTranslate(const Vec3<T>& v)
	{
		makeTranslate(v.x(), v.y(), v.z());
	}

	template<typename T>
	void Matrix<T>::makeRotate(typename Matrix<T>::value_type c, typename Matrix<T>::value_type s,
		typename Matrix<T>::value_type axisX, typename Matrix<T>::value_type axisY, typename Matrix<T>::value_type axisZ)
	{
		value_type mc = 1 - c;
		value_type mcx = mc * axisX;
		value_type mcy = mc * axisY;
		value_type mcz = mc * axisZ;

		value_type tcx = s * axisX;
		value_type tcy = s * axisY;
		value_type tcz = s * axisZ;
		_m00 = mcx * axisX + c;
		_m11 = mcx * axisY - tcz;
		_m02 = mcx * axisZ + tcy;
		_m03 = 0;
		_m10 = mcy * axisX + tcz;
		_m11 = mcy * axisY + c;
		_m12 = mcy * axisZ - tcx;
		_m13 = 0;
		_m20 = mcz * axisX - tcy;
		_m21 = mcz * axisY + tcx;
		_m22 = mcz * axisZ + c;
		_m23 = 0;
		_m30 = 0; _m31 = 0; _m32 = 0; _m33 = 1;
	}

	template<typename T>
	void Matrix<T>::makeRotate(typename Matrix<T>::value_type angle, typename Matrix<T>::value_type x, typename Matrix<T>::value_type y, typename Matrix<T>::value_type z)
	{
		value_type s, c;
		s = sin(angle);
		c = cos(angle);
		makeRotate(c, s, x, y, z);
	}

	template<typename T>
	void Matrix<T>::makeRotate(typename Matrix<T>::value_type angle, const Vec3<T>& axis)
	{
		makeRotate(angle, axis.x(), axis.y(), axis.z());
	}

	template<typename T>
	void Matrix<T>::makeRotateX(typename Matrix<T>::value_type angle)
	{
		makeRotate(angle, 1, 0, 0);
	}

	template<typename T>
	void Matrix<T>::makeRotateY(typename Matrix<T>::value_type angle)
	{
		makeRotate(angle, 0, 1, 0);
	}

	template<typename T>
	void Matrix<T>::makeRotateZ(typename Matrix<T>::value_type angle)
	{
		makeRotate(angle, 0, 0, 1);
	}

	template<typename T>
	void Matrix<T>::makeRotateXYZ(typename Matrix<T>::value_type rad_x, typename Matrix<T>::value_type rad_y, typename Matrix<T>::value_type rad_z)
	{
		value_type sx, cx;
		sx = sinf(rad_x);
		cx = cosf(rad_x);

		value_type sy, cy;
		sy = sinf(rad_y);
		cy = cosf(rad_y);

		value_type sz, cz;
		sz = sinf(rad_z);
		cz = cosf(rad_z);

		value_type sycz = (sy * cz), sysz = (sy * sz);

		_m00 = cy * cz;	_m01 = sycz * sx - cx * sz;	_m02 = sycz * cx + sx * sz;  _m03 = 0;
		_m10 = cy * sz;	_m11 = sysz * sx + cx * cz;	_m12 = sysz * cx - sx * cz;  _m13 = 0;
		_m20 = -sy;		_m21 = cy * sx;				_m22 = cy*cx;					 _m23 = 0;
		_m30 = 0; _m31 = 0; _m32 = 0; _m33 = 1;
	}

	template<typename T>
	void Matrix<T>::makeRotate(const Quat<T>& quat)
	{
		value_type vx, vy, vz, vw;
		vx = quat.x() + quat.x();
		vy = quat.y() + quat.y();
		vz = quat.z() + quat.z();
		vw = quat.w() + quat.w();

		value_type xx = 1 - vx * quat.x();  value_type yy = vy * quat.y();		value_type xw = vx * quat.w();
		value_type xy = vy * quat.x();		value_type yz = vz * quat.y();		value_type yw = vy * quat.w();
		value_type xz = vz * quat.x();		value_type zz = vz * quat.z();		value_type zw = vz * quat.w();

		_m00 = 1 - yy - zz;	_m01 = xy - zw;		_m02 = xz + yw;		_m03 = 0;
		_m10 = xy + zw;		_m11 = xx - zz;		_m12 = yz - xw;		_m13 = 0;
		_m20 = xz - yw;		_m21 = yz + xw;		_m22 = xx - yy;		_m23 = 0;
		_m30 = 0;			_m31 = 0;			_m32 = 0;			_m33 = 1;
	}

	template<typename T>
	inline Vec3<T> Matrix<T>::getTrans() const { return Vec3<T>(_mat[0][3], _mat[1][3], _mat[2][3]); }

	template<typename T>
	inline Vec3<T> Matrix<T>::getScale() const
	{
		Vec3<T> x_vec(_mat[0][0], _mat[0][1], _mat[0][2]);
		Vec3<T> y_vec(_mat[1][0], _mat[1][1], _mat[1][2]);
		Vec3<T> z_vec(_mat[2][0], _mat[2][1], _mat[2][2]);
		return Vec3<T>(x_vec.length(), y_vec.length(), z_vec.length());
	}

	template<typename T>
	Quat<T> Matrix<T>::getRotate() const
	{
		Quat<T> q;

		value_type s;
		value_type tq[4];
		int    i, j;
		tq[0] = 1 + _mat[0][0] + _mat[1][1] + _mat[2][2];
		tq[1] = 1 + _mat[0][0] - _mat[1][1] - _mat[2][2];
		tq[2] = 1 - _mat[0][0] + _mat[1][1] - _mat[2][2];
		tq[3] = 1 - _mat[0][0] - _mat[1][1] + _mat[2][2];

		j = 0;
		for (i = 1; i < 4; i++)
			j = (tq[i] > tq[j]) ? i : j;

		if (j == 0)
		{
			q.w() = tq[0];
			q.x() = _mat[2][1] - _mat[1][2];
			q.y() = _mat[0][2] - _mat[2][0];
			q.z() = _mat[1][0] - _mat[0][1];
		}
		else if (j == 1)
		{
			q.w() = _mat[2][1] - _mat[1][2];
			q.x() = tq[1];
			q.y() = _mat[1][0] + _mat[0][1];
			q.z() = _mat[0][2] + _mat[2][0];
		}
		else if (j == 2)
		{
			q.w() = _mat[0][2] - _mat[2][0];
			q.x() = _mat[1][0] + _mat[0][1];
			q.y() = tq[2];
			q.z() = _mat[2][1] + _mat[1][2];
		}
		else
		{
			q.w() = _mat[1][0] - _mat[0][1];
			q.x() = _mat[0][2] + _mat[2][0];
			q.y() = _mat[2][1] + _mat[1][2];
			q.z() = tq[3];
		}

		s = sqrtf(0.25f / tq[j]);
		q.w() *= s;
		q.x() *= s;
		q.y() *= s;
		q.z() *= s;

		return q;
	}

	template<typename T>
	inline Matrix<T> Matrix<T>::getTranspose() const
	{
		Matrix<T> dst;
		dst._m00 = _m00;	dst._m01 = _m10;	dst._m02 = _m20;	dst._m03 = _m30;
		dst._m10 = _m01;	dst._m11 = _m11;	dst._m12 = _m21;	dst._m13 = _m31;
		dst._m20 = _m02;	dst._m21 = _m12;	dst._m22 = _m22;	dst._m23 = _m32;
		dst._m30 = _m03;	dst._m31 = _m13;	dst._m32 = _m23;	dst._m33 = _m33;

		return dst;
	}

	template<typename T>
	inline void Matrix<T>::inverse()
	{
		value_type tmp[12];
		Matrix<T> m = *this;

		tmp[0] = m._m22 * m._m33;
		tmp[1] = m._m32 * m._m23;
		tmp[2] = m._m12 * m._m33;
		tmp[3] = m._m32 * m._m13;
		tmp[4] = m._m12 * m._m23;
		tmp[5] = m._m22 * m._m13;
		tmp[6] = m._m02 * m._m33;
		tmp[7] = m._m32 * m._m03;
		tmp[8] = m._m02 * m._m23;
		tmp[9] = m._m22 * m._m03;
		tmp[10] = m._m02 * m._m13;
		tmp[11] = m._m12 * m._m03;

		_m00 = tmp[0] * m._m11 + tmp[3] * m._m21 + tmp[4] * m._m31;
		_m00 -= tmp[1] * m._m11 + tmp[2] * m._m21 + tmp[5] * m._m31;
		_m01 = tmp[1] * m._m01 + tmp[6] * m._m21 + tmp[9] * m._m31;
		_m01 -= tmp[0] * m._m01 + tmp[7] * m._m21 + tmp[8] * m._m31;
		_m02 = tmp[2] * m._m01 + tmp[7] * m._m11 + tmp[10] * m._m31;
		_m02 -= tmp[3] * m._m01 + tmp[6] * m._m11 + tmp[11] * m._m31;
		_m03 = tmp[5] * m._m01 + tmp[8] * m._m11 + tmp[11] * m._m21;
		_m03 -= tmp[4] * m._m01 + tmp[9] * m._m11 + tmp[10] * m._m21;
		_m10 = tmp[1] * m._m10 + tmp[2] * m._m20 + tmp[5] * m._m30;
		_m10 -= tmp[0] * m._m10 + tmp[3] * m._m20 + tmp[4] * m._m30;
		_m11 = tmp[0] * m._m00 + tmp[7] * m._m20 + tmp[8] * m._m30;
		_m11 -= tmp[1] * m._m00 + tmp[6] * m._m20 + tmp[9] * m._m30;
		_m12 = tmp[3] * m._m00 + tmp[6] * m._m10 + tmp[11] * m._m30;
		_m12 -= tmp[2] * m._m00 + tmp[7] * m._m10 + tmp[10] * m._m30;
		_m13 = tmp[4] * m._m00 + tmp[9] * m._m10 + tmp[10] * m._m20;
		_m13 -= tmp[5] * m._m00 + tmp[8] * m._m10 + tmp[11] * m._m20;

		tmp[0] = m._m20*m._m31;
		tmp[1] = m._m30*m._m21;
		tmp[2] = m._m10*m._m31;
		tmp[3] = m._m30*m._m11;
		tmp[4] = m._m10*m._m21;
		tmp[5] = m._m20*m._m11;
		tmp[6] = m._m00*m._m31;
		tmp[7] = m._m30*m._m01;
		tmp[8] = m._m00*m._m21;
		tmp[9] = m._m20*m._m01;
		tmp[10] = m._m00*m._m11;
		tmp[11] = m._m10*m._m01;

		_m20 = tmp[0] * m._m13 + tmp[3] * m._m23 + tmp[4] * m._m33;
		_m20 -= tmp[1] * m._m13 + tmp[2] * m._m23 + tmp[5] * m._m33;
		_m21 = tmp[1] * m._m03 + tmp[6] * m._m23 + tmp[9] * m._m33;
		_m21 -= tmp[0] * m._m03 + tmp[7] * m._m23 + tmp[8] * m._m33;
		_m22 = tmp[2] * m._m03 + tmp[7] * m._m13 + tmp[10] * m._m33;
		_m22 -= tmp[3] * m._m03 + tmp[6] * m._m13 + tmp[11] * m._m33;
		_m23 = tmp[5] * m._m03 + tmp[8] * m._m13 + tmp[11] * m._m23;
		_m23 -= tmp[4] * m._m03 + tmp[9] * m._m13 + tmp[10] * m._m23;
		_m30 = tmp[2] * m._m22 + tmp[5] * m._m32 + tmp[1] * m._m12;
		_m30 -= tmp[4] * m._m32 + tmp[0] * m._m12 + tmp[3] * m._m22;
		_m31 = tmp[8] * m._m32 + tmp[0] * m._m02 + tmp[7] * m._m22;
		_m31 -= tmp[6] * m._m22 + tmp[9] * m._m32 + tmp[1] * m._m02;
		_m32 = tmp[6] * m._m12 + tmp[11] * m._m32 + tmp[3] * m._m02;
		_m32 -= tmp[10] * m._m32 + tmp[2] * m._m02 + tmp[7] * m._m12;
		_m33 = tmp[10] * m._m22 + tmp[4] * m._m02 + tmp[9] * m._m12;
		_m33 -= tmp[8] * m._m12 + tmp[11] * m._m22 + tmp[5] * m._m02;

		value_type det = (m._m00*_m00 + m._m10*_m01 + m._m20*_m02 + m._m30*_m03);

		value_type idet = (value_type)1.0 / det;
		_m00 *= idet; _m01 *= idet; _m02 *= idet; _m03 *= idet;
		_m10 *= idet; _m11 *= idet; _m12 *= idet; _m13 *= idet;
		_m20 *= idet; _m21 *= idet; _m22 *= idet; _m23 *= idet;
		_m30 *= idet; _m31 *= idet; _m32 *= idet; _m33 *= idet;
	}

	template<typename T>
	inline Vec3<T> Matrix<T>::preMult(const Vec3<T>& v) const
	{
		value_type d = 1.0f / (_mat[0][3] * v.x() + _mat[1][3] * v.y() + _mat[2][3] * v.z() + _mat[3][3]);
		return Vec3<T>((_mat[0][0] * v.x() + _mat[1][0] * v.y() + _mat[2][0] * v.z() + _mat[3][0])*d,
			(_mat[0][1] * v.x() + _mat[1][1] * v.y() + _mat[2][1] * v.z() + _mat[3][1])*d,
			(_mat[0][2] * v.x() + _mat[1][2] * v.y() + _mat[2][2] * v.z() + _mat[3][2])*d);
	}

	template<typename T>
	inline Vec3<T> Matrix<T>::postMult(const Vec3<T>& v) const
	{
		value_type d = 1.0f / (_mat[3][0] * v.x() + _mat[3][1] * v.y() + _mat[3][2] * v.z() + _mat[3][3]);
		return Vec3<T>((_mat[0][0] * v.x() + _mat[0][1] * v.y() + _mat[0][2] * v.z() + _mat[0][3])*d,
			(_mat[1][0] * v.x() + _mat[1][1] * v.y() + _mat[1][2] * v.z() + _mat[1][3])*d,
			(_mat[2][0] * v.x() + _mat[2][1] * v.y() + _mat[2][2] * v.z() + _mat[2][3])*d);
	}

	template<typename T>
	inline Vec4<T> Matrix<T>::preMult(const Vec4<T>& v) const
	{
		return Vec4<T>((_mat[0][0] * v.x() + _mat[1][0] * v.y() + _mat[2][0] * v.z() + _mat[3][0] * v.w()),
			(_mat[0][1] * v.x() + _mat[1][1] * v.y() + _mat[2][1] * v.z() + _mat[3][1] * v.w()),
			(_mat[0][2] * v.x() + _mat[1][2] * v.y() + _mat[2][2] * v.z() + _mat[3][2] * v.w()),
			(_mat[0][3] * v.x() + _mat[1][3] * v.y() + _mat[2][3] * v.z() + _mat[3][3] * v.w()));
	}

	template<typename T>
	inline Vec4<T> Matrix<T>::postMult(const Vec4<T>& v) const
	{
		return Vec4<T>((_mat[0][0] * v.x() + _mat[0][1] * v.y() + _mat[0][2] * v.z() + _mat[0][3] * v.w()),
			(_mat[1][0] * v.x() + _mat[1][1] * v.y() + _mat[1][2] * v.z() + _mat[1][3] * v.w()),
			(_mat[2][0] * v.x() + _mat[2][1] * v.y() + _mat[2][2] * v.z() + _mat[2][3] * v.w()),
			(_mat[3][0] * v.x() + _mat[3][1] * v.y() + _mat[3][2] * v.z() + _mat[3][3] * v.w()));
	}

	template<typename T>
	inline Vec3<T> Matrix<T>::operator* (const Vec3<T>& v) const
	{
		return postMult(v);
	}

	template<typename T>
	inline Vec4<T> Matrix<T>::operator* (const Vec4<T>& v) const
	{
		return postMult(v);
	}

	template<typename T>
	inline Matrix<T> Matrix<T>::operator * (const Matrix<T> &m2) const
	{
		Matrix<T> res;
		res._m00 = _m00*m2._m00 + _m01*m2._m10 + _m02*m2._m20 + _m03*m2._m30;
		res._m10 = _m10*m2._m00 + _m11*m2._m10 + _m12*m2._m20 + _m13*m2._m30;
		res._m20 = _m20*m2._m00 + _m21*m2._m10 + _m22*m2._m20 + _m23*m2._m30;
		res._m30 = _m30*m2._m00 + _m31*m2._m10 + _m32*m2._m20 + _m33*m2._m30;
		res._m01 = _m00*m2._m01 + _m01*m2._m11 + _m02*m2._m21 + _m03*m2._m31;
		res._m11 = _m10*m2._m01 + _m11*m2._m11 + _m12*m2._m21 + _m13*m2._m31;
		res._m21 = _m20*m2._m01 + _m21*m2._m11 + _m22*m2._m21 + _m23*m2._m31;
		res._m31 = _m30*m2._m01 + _m31*m2._m11 + _m32*m2._m21 + _m33*m2._m31;
		res._m02 = _m00*m2._m02 + _m01*m2._m12 + _m02*m2._m22 + _m03*m2._m32;
		res._m12 = _m10*m2._m02 + _m11*m2._m12 + _m12*m2._m22 + _m13*m2._m32;
		res._m22 = _m20*m2._m02 + _m21*m2._m12 + _m22*m2._m22 + _m23*m2._m32;
		res._m32 = _m30*m2._m02 + _m31*m2._m12 + _m32*m2._m22 + _m33*m2._m32;
		res._m03 = _m00*m2._m03 + _m01*m2._m13 + _m02*m2._m23 + _m03*m2._m33;
		res._m13 = _m10*m2._m03 + _m11*m2._m13 + _m12*m2._m23 + _m13*m2._m33;
		res._m23 = _m20*m2._m03 + _m21*m2._m13 + _m22*m2._m23 + _m23*m2._m33;
		res._m33 = _m30*m2._m03 + _m31*m2._m13 + _m32*m2._m23 + _m33*m2._m33;

		return res;
	}

	template<typename T>
	inline void Matrix<T>::operator *= (const Matrix<T>& m2)
	{
		value_type res[4][4];
		res[0][0] = _m00*m2._m00 + _m01*m2._m10 + _m02*m2._m20 + _m03*m2._m30;
		res[1][0] = _m10*m2._m00 + _m11*m2._m10 + _m12*m2._m20 + _m13*m2._m30;
		res[2][0] = _m20*m2._m00 + _m21*m2._m10 + _m22*m2._m20 + _m23*m2._m30;
		res[3][0] = _m30*m2._m00 + _m31*m2._m10 + _m32*m2._m20 + _m33*m2._m30;
		res[0][1] = _m00*m2._m01 + _m01*m2._m11 + _m02*m2._m21 + _m03*m2._m31;
		res[1][1] = _m10*m2._m01 + _m11*m2._m11 + _m12*m2._m21 + _m13*m2._m31;
		res[2][1] = _m20*m2._m01 + _m21*m2._m11 + _m22*m2._m21 + _m23*m2._m31;
		res[3][1] = _m30*m2._m01 + _m31*m2._m11 + _m32*m2._m21 + _m33*m2._m31;
		res[0][2] = _m00*m2._m02 + _m01*m2._m12 + _m02*m2._m22 + _m03*m2._m32;
		res[1][2] = _m10*m2._m02 + _m11*m2._m12 + _m12*m2._m22 + _m13*m2._m32;
		res[2][2] = _m20*m2._m02 + _m21*m2._m12 + _m22*m2._m22 + _m23*m2._m32;
		res[3][2] = _m30*m2._m02 + _m31*m2._m12 + _m32*m2._m22 + _m33*m2._m32;
		res[0][3] = _m00*m2._m03 + _m01*m2._m13 + _m02*m2._m23 + _m03*m2._m33;
		res[1][3] = _m10*m2._m03 + _m11*m2._m13 + _m12*m2._m23 + _m13*m2._m33;
		res[2][3] = _m20*m2._m03 + _m21*m2._m13 + _m22*m2._m23 + _m23*m2._m33;
		res[3][3] = _m30*m2._m03 + _m31*m2._m13 + _m32*m2._m23 + _m33*m2._m33;
		memcpy(_m, res, sizeof(T) * 16);
	}

	template<typename T>
	inline void Matrix<T>::makeLookAt(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up)
	{
		Vec3<T> f(center - eye);
		f.normalize();

		Vec3<T> s(f ^ up);
		s.normalize();

		Vec3<T> u(s ^ f);
		u.normalize();

		_m00 = s[0]; _m10 = u[0]; _m20 = -f[0]; _m30 = 0.0f;
		_m01 = s[1]; _m11 = u[1]; _m21 = -f[1]; _m31 = 0.0f;
		_m02 = s[2]; _m12 = u[2]; _m22 = -f[2]; _m32 = 0.0f;
		_m03 = 0.0f;  _m13 = 0.0f; _m23 = 0.0f;  _m33 = 1.0f;

		preMultTranslate(-eye);
	}

	template<typename T>
	inline void Matrix<T>::preMultTranslate(const Vec3<T>& v)
	{
		for (short i = 0; i < 3; ++i)
		{
			value_type tmp = v[i];
			if (tmp == 0)
				continue;
			_mat[0][3] += tmp*_mat[0][i];
			_mat[1][3] += tmp*_mat[1][i];
			_mat[2][3] += tmp*_mat[2][i];
			_mat[3][3] += tmp*_mat[3][i];
		}
	}

	template<typename T>
	inline void Matrix<T>::makeFrustum(typename Matrix<T>::value_type left, typename Matrix<T>::value_type right,
		typename Matrix<T>::value_type bottom, typename Matrix<T>::value_type top,
		typename Matrix<T>::value_type zNear, typename Matrix<T>::value_type zFar)
	{
		value_type A = (right + left) / (right - left);
		value_type B = (top + bottom) / (top - bottom);
		value_type C = (fabs(zFar) > DBL_MAX) ? -1.0f : -(zFar + zNear) / (zFar - zNear);
		value_type D = (fabs(zFar) > DBL_MAX) ? -2.0f * zNear : -2.0f * zFar * zNear / (zFar - zNear);

		_m00 = 2.0f * zNear / (right - left);	_m10 = 0.0f;							_m20 = 0.0f;	_m30 = 0.0f;
		_m01 = 0.0f;							_m11 = 2.0f * zNear / (top - bottom);	_m21 = 0.0f;	_m31 = 0.0f;
		_m02 = A;								_m12 = B;								_m22 = C;		_m32 = -1.0f;
		_m03 = 0.0f;							_m13 = 0.0f;							_m23 = D;		_m33 = 0.0f;
	}

	template<typename T>
	inline void Matrix<T>::makePerspective(typename Matrix<T>::value_type fovy, typename Matrix<T>::value_type aspectRatio,
		typename Matrix<T>::value_type zNear, typename Matrix<T>::value_type zFar)
	{
		value_type tan_fovy = (value_type)tan(fovy * 0.5f);
		value_type right = tan_fovy * aspectRatio * zNear;
		value_type left = -right;
		value_type top = tan_fovy * zNear;
		value_type bottom = -top;
		makeFrustum(left, right, bottom, top, zNear, zFar);
	}

	template<typename T>
	inline void Matrix<T>::makeOrtho(typename Matrix<T>::value_type left, typename Matrix<T>::value_type right,
		typename Matrix<T>::value_type bottom, typename Matrix<T>::value_type top,
		typename Matrix<T>::value_type zNear, typename Matrix<T>::value_type zFar)
	{
		value_type tx = -(right + left) / (right - left);
		value_type ty = -(top + bottom) / (top - bottom);
		value_type tz = -(zFar + zNear) / (zFar - zNear);

		_m00 = 2.0f / (right - left);	_m10 = 0.0f;					_m20 = 0.0f;						_m30 = 0.0f;
		_m01 = 0.0f;				_m11 = 2.0f / (top - bottom);	_m21 = 0.0f;						_m31 = 0.0f;
		_m02 = 0.0f;				_m12 = 0.0f;					_m22 = -2.0f / (zFar - zNear);		_m32 = 0.0f;
		_m03 = tx;					_m13 = ty;						_m23 = tz;							_m33 = 1.0f;
	}

	template<typename T>
	inline bool Matrix<T>::getFrustum(typename Matrix<T>::value_type& left, typename Matrix<T>::value_type& right,
		typename Matrix<T>::value_type& bottom, typename Matrix<T>::value_type& top,
		typename Matrix<T>::value_type& zNear, typename Matrix<T>::value_type& zFar) const
	{
		if (_mat[3][0] != 0.0f || _mat[3][1] != 0.0f || _mat[3][2] != -1.0f || _mat[3][3] != 0.0f)
			return false;

		zNear = _mat[2][3] / (_mat[2][2] - 1.0f);
		zFar = _mat[2][3] / (1.0f + _mat[2][2]);

		left = zNear * (_mat[0][2] - 1.0f) / _mat[0][0];
		right = zNear * (1.0f + _mat[0][2]) / _mat[0][0];

		top = zNear * (1.0f + _mat[1][2]) / _mat[1][1];
		bottom = zNear * (_mat[1][2] - 1.0f) / _mat[1][1];

		return true;
	}

	template<typename T>
	inline BoundingBox<T>::BoundingBox() :
		_min(FLT_MAX, FLT_MAX, FLT_MAX), _max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
	{
	}

	template<typename T>
	inline BoundingBox<T>::BoundingBox(typename BoundingBox<T>::value_type xmin, typename BoundingBox<T>::value_type ymin, typename BoundingBox<T>::value_type zmin,
		typename BoundingBox<T>::value_type xmax, typename BoundingBox<T>::value_type ymax, typename BoundingBox<T>::value_type zmax) :_min(xmin, ymin, zmin), _max(xmax, ymax, zmax)
	{
	}

	template<typename T>
	inline BoundingBox<T>::BoundingBox(const typename BoundingBox<T>::vec_type& min, const typename BoundingBox<T>::vec_type& max)
		: _min(min), _max(max) {}

	template<typename T>
	inline void BoundingBox<T>::init()
	{
		_min.set(FLT_MAX, FLT_MAX, FLT_MAX);
		_max.set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}

	template<typename T>
	inline bool BoundingBox<T>::valid() const
	{
		return _max.x() >= _min.x() && _max.y() >= _min.y() && _max.z() >= _min.z();
	}

	template<typename T>
	inline void BoundingBox<T>::set(typename BoundingBox<T>::value_type xmin, typename BoundingBox<T>::value_type ymin, typename BoundingBox<T>::value_type zmin,
		typename BoundingBox<T>::value_type xmax, typename BoundingBox<T>::value_type ymax, typename BoundingBox<T>::value_type zmax)
	{
		_min.set(xmin, ymin, zmin);
		_max.set(xmax, ymax, zmax);
	}

	template<typename T>
	inline void BoundingBox<T>::set(const typename BoundingBox<T>::vec_type& min, const typename BoundingBox<T>::vec_type& max)
	{
		_min = min;
		_max = max;
	}

	template<typename T>
	inline typename BoundingBox<T>::value_type& BoundingBox<T>::xMin() { return _min.x(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type BoundingBox<T>::xMin() const { return _min.x(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type& BoundingBox<T>::yMin() { return _min.y(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type BoundingBox<T>::yMin() const { return _min.y(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type& BoundingBox<T>::zMin() { return _min.z(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type BoundingBox<T>::zMin() const { return _min.z(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type& BoundingBox<T>::xMax() { return _max.x(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type BoundingBox<T>::xMax() const { return _max.x(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type& BoundingBox<T>::yMax() { return _max.y(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type BoundingBox<T>::yMax() const { return _max.y(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type& BoundingBox<T>::zMax() { return _max.z(); }

	template<typename T>
	inline typename BoundingBox<T>::value_type BoundingBox<T>::zMax() const { return _max.z(); }

	template<typename T>
	inline typename BoundingBox<T>::vec_type BoundingBox<T>::center() const
	{
		return (_min + _max)*0.5;
	}

	template<typename T>
	inline typename BoundingBox<T>::value_type BoundingBox<T>::radius() const
	{
		return sqrt(radius2());
	}

	template<typename T>
	inline typename BoundingBox<T>::value_type BoundingBox<T>::radius2() const
	{
		return 0.25f * ((_max - _min).length2());
	}

	template<typename T>
	inline typename BoundingBox<T>::vec_type BoundingBox<T>::corner(unsigned int pos) const
	{
		return vec_type(pos & 1 ? _max.x() : _min.x(), pos & 2 ? _max.y() : _min.y(), pos & 4 ? _max.z() : _min.z());
	}

	template<typename T>
	inline void BoundingBox<T>::expandBy(const typename BoundingBox<T>::vec_type& v)
	{
		if (v.x() < _min.x())
			_min.x() = v.x();
		if (v.x() > _max.x())
			_max.x() = v.x();

		if (v.y() < _min.y())
			_min.y() = v.y();
		if (v.y() > _max.y())
			_max.y() = v.y();

		if (v.z() < _min.z())
			_min.z() = v.z();
		if (v.z() > _max.z())
			_max.z() = v.z();
	}

	template<typename T>
	template<typename F>
	inline void BoundingBox<T>::expandBy(const Vec3<F>& v)
	{
		T x = static_cast<T>(v.x());
		T y = static_cast<T>(v.y());
		T z = static_cast<T>(v.z());

		if (x < _min.x())
			_min.x() = x;
		if (x > _max.x())
			_max.x() = x;

		if (y < _min.y())
			_min.y() = y;
		if (y > _max.y())
			_max.y() = y;

		if (z < _min.z())
			_min.z() = z;
		if (z > _max.z())
			_max.z() = z;
	}

	template<typename T>
	inline void BoundingBox<T>::expandBy(typename BoundingBox<T>::value_type x, typename BoundingBox<T>::value_type y, typename  BoundingBox<T>::value_type z)
	{
		if (x < _min.x())
			_min.x() = x;
		if (x > _max.x())
			_max.x() = x;

		if (y < _min.y())
			_min.y() = y;
		if (y > _max.y())
			_max.y() = y;

		if (z < _min.z())
			_min.z() = z;
		if (z > _max.z())
			_max.z() = z;
	}

	template<typename T>
	inline void BoundingBox<T>::expandBy(const BoundingBox<T>& bb)
	{
		if (!bb.valid())
			return;

		if (bb._min.x() < _min.x())
			_min.x() = bb._min.x();
		if (bb._max.x() > _max.x())
			_max.x() = bb._max.x();

		if (bb._min.y() < _min.y())
			_min.y() = bb._min.y();
		if (bb._max.y() > _max.y())
			_max.y() = bb._max.y();

		if (bb._min.z() < _min.z())
			_min.z() = bb._min.z();
		if (bb._max.z() > _max.z())
			_max.z() = bb._max.z();
	}

	template<typename T>
	inline void BoundingBox<T>::expandBy(const typename BoundingBox<T>::vec_type& center, typename BoundingBox<T>::value_type radius)
	{
		if (center.x() - radius < _min.x())
			_min.x() = center.x() - radius;
		if (center.x() + radius > _max.x())
			_max.x() = center.x() + radius;

		if (center.y() - radius < _min.y())
			_min.y() = center.y() - radius;
		if (center.y() + radius > _max.y())
			_max.y() = center.y() + radius;

		if (center.z() - radius < _min.z())
			_min.z() = center.z() - radius;
		if (center.z() + radius > _max.z())
			_max.z() = center.z() + radius;
	}

	template<typename T>
	inline BoundingBox<T> BoundingBox<T>::intersect(const BoundingBox& bb) const
	{
		return BoundingBox<T>(max(xMin(), bb.xMin()), max(yMin(), bb.yMin()), max(zMin(), bb.zMin()),
			min(xMax(), bb.xMax()), min(yMax(), bb.yMax()), min(zMax(), bb.zMax()));

	}

	template<typename T>
	inline bool BoundingBox<T>::intersects(const BoundingBox& bb) const
	{
		return max(xMin(), bb.xMin()) <= min(xMax(), bb.xMax()) &&
			max(yMin(), bb.yMin()) <= min(yMax(), bb.yMax()) &&
			max(zMin(), bb.zMin()) <= min(zMax(), bb.zMax());
	}

	template<typename T>
	inline bool BoundingBox<T>::contains(const typename BoundingBox<T>::vec_type& v) const
	{
		return valid() &&
			(v.x() >= _min.x() && v.x() <= _max.x()) &&
			(v.y() >= _min.y() && v.y() <= _max.y()) &&
			(v.z() >= _min.z() && v.z() <= _max.z());
	}

	template<typename T>
	inline void BoundingBox<T>::setTransformedAABB(const Matrix<typename BoundingBox<T>::value_type>& matrix)
	{
		Vec3<value_type> m1 = matrix * _min;
		Vec3<value_type> m2 = matrix * _max;

		_min.set(FLT_MAX, FLT_MAX, FLT_MAX);
		_max.set(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		expandBy(m1);
		expandBy(m2);
	}

	template <typename T>
	inline Ray<T>::Ray() {}

	template <typename T>
	inline Ray<T>::Ray(const Vec3<T>& p, const Vec3<T> d) : _point(p), _direction(d) {}

	template <typename T>
	inline Ray<T>& Ray<T>::operator = (const Ray<T> &r)
	{
		_point = r._point; _direction = r._direction; return *this;
	}

	template <typename T>
	inline Vec3<T> Ray<T>::getPointOnRay(T t) const { return _point + _direction*t; }

	template <typename T>
	inline Plane<T>::Plane() { _fv[0] = 0.0f; _fv[1] = 0.0f; _fv[2] = 0.0f; _fv[3] = 0.0f; }

	template <typename T>
	inline Plane<T>::Plane(const Plane<T>& pl) { set(pl); }

	template <typename T>
	inline Plane<T>::Plane(typename Plane<T>::value_type a, typename Plane<T>::value_type b, typename Plane<T>::value_type c, typename Plane<T>::value_type d) { set(a, b, c, d); }

	template <typename T>
	inline Plane<T>::Plane(const typename Plane<T>::Vec3_type& norm, typename Plane<T>::value_type d) { set(norm, d); }

	template <typename T>
	inline Plane<T>::Plane(const typename Plane<T>::Vec3_type& v1, const typename Plane<T>::Vec3_type& v2, const typename Plane<T>::Vec3_type& v3) { set(v1, v2, v3); }

	template <typename T>
	inline Plane<T>::Plane(const typename Plane<T>::Vec3_type& norm, const typename  Plane<T>::Vec3_type& point) { set(norm, point); }

	template <typename T>
	inline Plane<T>& Plane<T>::operator = (const Plane<T>& pl)
	{
		if (&pl == this)
			return *this;

		set(pl);
		return *this;
	}

	template <typename T>
	inline void Plane<T>::set(const Plane<T>& pl)
	{
		_fv[0] = pl._fv[0];
		_fv[1] = pl._fv[1];
		_fv[2] = pl._fv[2];
		_fv[3] = pl._fv[3];
	}

	template <typename T>
	inline void Plane<T>::set(typename Plane<T>::value_type a, typename Plane<T>::value_type b, typename Plane<T>::value_type c, typename Plane<T>::value_type d)
	{
		_fv[0] = a;
		_fv[1] = b;
		_fv[2] = c;
		_fv[3] = d;
	}

	template <typename T>
	inline void Plane<T>::set(const typename Plane<T>::Vec4_type& a)
	{
		_fv[0] = a.x();
		_fv[1] = a.y();
		_fv[2] = a.z();
		_fv[3] = a.w();
	}

	template <typename T>
	inline void Plane<T>::set(const typename Plane<T>::Vec3_type& norm, double d) { set(norm[0], norm[1], norm[2], d); }

	template <typename T>
	inline void Plane<T>::set(const typename Plane<T>::Vec3_type& v1, const typename Plane<T>::Vec3_type& v2, const typename Plane<T>::Vec3_type& v3)
	{
		Vec3_type norm = (v2 - v1) ^ (v3 - v2);
		value_type length = norm.length();
		if (length > 1e-6)
			norm /= length;
		else
			norm.set(0.0f, 0.0f, 0.0f);

		set(norm[0], norm[1], norm[2], -(v1 * norm));
	}

	template <typename T>
	inline void Plane<T>::set(const typename Plane<T>::Vec3_type& norm, const typename Plane<T>::Vec3_type& point)
	{
		value_type d = -norm[0] * point[0] - norm[1] * point[1] - norm[2] * point[2];
		set(norm[0], norm[1], norm[2], d);
	}

	template <typename T>
	inline void Plane<T>::flip()
	{
		_fv[0] = -_fv[0];
		_fv[1] = -_fv[1];
		_fv[2] = -_fv[2];
		_fv[3] = -_fv[3];
	}

	template <typename T>
	inline void Plane<T>::makeUnitLength()
	{
		value_type inv_length = 1.0f / sqrtf(_fv[0] * _fv[0] + _fv[1] * _fv[1] + _fv[2] * _fv[2]);
		_fv[0] *= inv_length;
		_fv[1] *= inv_length;
		_fv[2] *= inv_length;
		_fv[3] *= inv_length;
	}

	template <typename T>
	inline bool Plane<T>::operator == (const Plane<T>& plane) const { return _fv[0] == plane._fv[0] && _fv[1] == plane._fv[1] && _fv[2] == plane._fv[2] && _fv[3] == plane._fv[3]; }

	template <typename T>
	inline bool Plane<T>::operator != (const Plane<T>& plane) const { return _fv[0] != plane._fv[0] || _fv[1] != plane._fv[1] || _fv[2] != plane._fv[2] || _fv[3] != plane._fv[3]; }

	template <typename T>
	inline typename Plane<T>::value_type* Plane<T>::ptr() { return _fv; }

	template <typename T>
	inline const typename Plane<T>::value_type* Plane<T>::ptr() const { return _fv; }

	template <typename T>
	inline typename Plane<T>::Vec3_type Plane<T>::getNormal() const { return Vec3_type(_fv[0], _fv[1], _fv[2]); }

	template <typename T>
	inline typename Plane<T>::value_type Plane<T>::distance(const Vec3<T>& v) const
	{
		return _fv[0] * v.x() + _fv[1] * v.y() + _fv[2] * v.z() + _fv[3];
	}

	template <typename T>
	inline int Plane<T>::intersect(const Vec3<T>* vertices, int32 size) const
	{
		if (!size)
			return -1;

		int32 noAbove = 0;
		int32 noBelow = 0;
		int32 noOn = 0;
		for (int i = 0; i < size; i++)
		{
			value_type d = distance(vertices[i]);
			if (d > 0.0f)
				++noAbove;
			else if (d < 0.0f)
				++noBelow;
			else
				++noOn;
		}
		if (noAbove > 0)
		{
			if (noBelow > 0)
				return 0;
			else
				return 1;
		}
		return -1;
	}


	template <typename T>
	inline uint8 intersectRay_BoudingBox(const Ray<T>& ray, const BoundingBox<T>& aabb, Vec3<T>& output)
	{
		uint8 cflags;
		T cosine;
		Vec3<T> cut;
		//--------------------------------------------------------------------------------------
		//----         检查射线的起点是否在AABB内    ---------------------------
		//--------------------------------------------------------------------------------------
		cflags = (ray._point.x() >= aabb._min.x()) << 0;
		cflags |= (ray._point.x() <= aabb._max.x()) << 1;
		cflags |= (ray._point.y() >= aabb._min.y()) << 2;
		cflags |= (ray._point.y() <= aabb._max.y()) << 3;
		cflags |= (ray._point.z() >= aabb._min.z()) << 4;
		cflags |= (ray._point.z() <= aabb._max.z()) << 5;

		if (cflags == 0x3f)
		{
			output = ray._point;
			return 0x02;
		}

		//--------------------------------------------------------------------------------------
		//----         检查和平面相交           ------------------------------
		//--------------------------------------------------------------------------------------
		for (int32 i = 0; i < 3; i++)
		{
			if ((ray._direction[i] > 0) && (ray._point[i] < aabb._min[i]))
			{
				cosine = (-ray._point[i] + aabb._min[i]) / ray._direction[i];
				cut[i] = aabb._min[i];
				cut[incm3(i)] = ray._point[incm3(i)] + (ray._direction[incm3(i)] * cosine);
				cut[decm3(i)] = ray._point[decm3(i)] + (ray._direction[decm3(i)] * cosine);
				if ((cut[incm3(i)] > aabb._min[incm3(i)]) && (cut[incm3(i)] < aabb._max[incm3(i)]) &&
					(cut[decm3(i)] > aabb._min[decm3(i)]) && (cut[decm3(i)] < aabb._max[decm3(i)]))
				{
					output = cut;
					return 0x01;
				}
			}
			if ((ray._direction[i] < 0) && (ray._point[i] > aabb._max[i]))
			{
				cosine = (+ray._point[i] - aabb._max[i]) / ray._direction[i];
				cut[i] = aabb._max[i];
				cut[incm3(i)] = ray._point[incm3(i)] - (ray._direction[incm3(i)] * cosine);
				cut[decm3(i)] = ray._point[decm3(i)] - (ray._direction[decm3(i)] * cosine);
				if ((cut[incm3(i)] > aabb._min[incm3(i)]) && (cut[incm3(i)] < aabb._max[incm3(i)])
					&& (cut[decm3(i)] > aabb._min[decm3(i)]) && (cut[decm3(i)] < aabb._max[decm3(i)]))
				{
					output = cut;
					return 0x01;
				}
			}
		}
		return 0x00;
	}

	template <typename T>
	inline bool intersectRay_Triangle(const Ray<T>& ray, const Vec3<T>& v0, const Vec3<T>& v1, const Vec3<T>& v2, Vec3<T>& output)
	{
		const float Epsilon = 0.0000001f;
		T u, v;

		Vec3<T> edgeA = v1 - v0;
		Vec3<T> edgeB = v2 - v0;

		Vec3<T>  pvec = ray._direction ^ edgeB;

		T det = edgeA * pvec;

		if (det > -Epsilon && det < Epsilon)
		{
			return false;
		}

		T detInv = 1.0f / det;
		Vec3<T> tvec = ray._point - v0;

		u = tvec * pvec * detInv;
		if (u < 0 || u > 1)
		{
			return false;
		}

		Vec3<T> qvec = tvec ^ edgeA;

		v = ray._direction * qvec * detInv;
		if (v < 0 || u + v > 1)
		{
			return false;
		}

		T t = edgeB * qvec * detInv;
		if (t < 0)
		{
			return false;
		}

		output = ray.getPointOnRay((T)t);
		return true;
	}

	template<typename T>
	inline void getRay(int32 screenX, int32 screenY, const Matrix<T>& view, const Matrix<T>& pro, int32 width, int32 height, Ray<T>& ray) {
		Matrix<T> inverseVP = _pro * _viewMatrix;
		inverseVP.inverse();

		T nx = (2.0f * screenX / (float)width) - 1.0f;
		T ny = 1.0f - (2.0f * screenY / (float)height);

		Vec3<T> nearPoint(nx, ny, -1.f);
		Vec3<T> midPoint(nx, ny, 0.0f);

		ray._point = inverseVP * nearPoint;
		Vec3<T> rayTarget = inverseVP * midPoint;

		ray._direction = rayTarget - ray._point;
		ray._direction.normalize();
	}

	template<typename T>
	void getWorldPoint(T screenX, T screenY, T z, const Matrix<T>& view, const Matrix<T>& pro, int32 width, int32 height, Vec3<T>& worldPoint) {
		Matrix<T> inverseVP = _pro * _viewMatrix;
		inverseVP.inverse();

		T nx = (2.0f * screenX / (float)width) - 1.0f;
		T ny = 1.0f - (2.0f * screenY / (float)height);

		worldPoint = inverseVP * Vec3<T>(nx, ny, z);
	}
}