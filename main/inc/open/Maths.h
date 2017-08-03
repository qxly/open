#ifndef WYIN_MATH_H
#define WYIN_MATH_H

#include <open/openDef.h>

namespace open
{
	const float QUARTER_PI = atanf(1.0f);

	const float HALF_PI = QUARTER_PI * 2.0f;

	const float PI = HALF_PI * 2.0f;

	const float DOUBLE_PI = PI * 2.0f;

#define RAD2DEG(rad) (rad / open::PI * 180.0f)

#define DEG2RAD(deg) (deg / 180.0f * open::PI)

#ifndef max
#define max(a,b) ( ((a)>(b)) ? (a):(b) )
#endif

#ifndef min
#define min(a,b) ( ((a)>(b)) ? (b):(a) )
#endif


#define BIT_SET(a,bit) a |= (1 << bit)

#define BIT_GET(a,bit) (a & (1 << bit)) != 0

	///判断是否是2的n次方
	template<typename T>
	bool isPO2(T n);

	unsigned int log2i(uint32 x);

	/// inc dec
	int32 incm3(int32 i);

	int32 decm3(int32 i);


	static const int32 FLOAT_NAN_INT = 0xffc00000;
	static const float FLOAT_NAN = *((float*)&FLOAT_NAN_INT);

	static const int64 DOUBLE_NAN_INT = 0xfff8000000000000;
	static const double DOUBLE_NAN = *((double*)&DOUBLE_NAN_INT);

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MWERKS__)
#include <float.h>
	inline bool isNaN(float v) { return _isnan(v) != 0; }
	inline bool isNaN(double v) { return _isnan(v) != 0; }
#else
#if defined(__APPLE__)
	inline bool isNaN(float v) { return std::isnan(v); }
	inline bool isNaN(double v) { return std::isnan(v); }
#else
	inline bool isNaN(float v) { return isnan(v); }
	inline bool isNaN(double v) { return isnan(v); }
#endif
#endif

	inline bool isNaN(bool) { return false; }
	inline bool isNaN(int) { return false; }
	inline bool isNaN(unsigned int) { return false; }
	inline bool isNaN(short) { return false; }
	inline bool isNaN(unsigned short) { return false; }
	inline bool isNaN(char) { return false; }
	inline bool isNaN(unsigned char) { return false; }

	inline bool isNaN(__int64) { return false; }
	inline bool isNaN(long) { return false; }

	template<typename T>
	T clampTo(T v, T min, T max);

	template<typename T>
	class Vec2
	{
	public:
		typedef T value_type;
		enum { num_components = 2 };

		value_type _v[2];

		Vec2();
		Vec2(value_type x, value_type y);
		Vec2(const Vec2& vec);

		bool operator == (const Vec2& v) const;
		bool operator != (const Vec2& v) const;
		bool operator <  (const Vec2& v) const;

		value_type* ptr();
		const value_type* ptr() const;

		void set(value_type x, value_type y);
		value_type& operator [] (int i);
		value_type operator [] (int i) const;
		value_type& x();
		value_type& y();
		value_type x() const;
		value_type y() const;

		bool valid() const;
		bool isNaN() const;

		Vec2 operator - () const;
		Vec2 operator + (const Vec2& rhs) const;
		Vec2 operator - (const Vec2& rhs) const;
		value_type operator * (const Vec2& rhs) const;
		Vec2 operator * (value_type rhs) const;
		Vec2 operator / (value_type rhs) const;
		Vec2& operator += (const Vec2& rhs);
		Vec2& operator -= (const Vec2& rhs);
		Vec2& operator *= (value_type rhs);
		Vec2& operator /= (value_type rhs);

		value_type length() const;
		value_type length2(void) const;
		value_type normalize();
	};
	typedef Vec2<float> Vec2f;
	typedef Vec2<double> Vec2d;

	template<typename T>
	class Vec3
	{
	public:
		typedef T value_type;
		enum { num_components = 3 };

		value_type _v[3];

		Vec3();
		Vec3(const Vec3& vec);
		Vec3(value_type x, value_type y, value_type z);

		bool operator == (const Vec3& v) const;
		bool operator != (const Vec3& v) const;
		bool operator <  (const Vec3& v) const;

		value_type* ptr();
		const value_type* ptr() const;

		void set(value_type x, value_type y, value_type z);
		void set(const Vec3& rhs);

		value_type& operator [] (int i);
		value_type operator [] (int i) const;
		value_type& x();
		value_type& y();
		value_type& z();
		value_type x() const;
		value_type y() const;
		value_type z() const;

		bool valid() const;
		bool isNaN() const;

		Vec3 operator - () const;
		Vec3 operator + (const Vec3& rhs) const;
		Vec3 operator - (const Vec3& rhs) const;
		value_type operator * (const Vec3& rhs) const;
		Vec3 operator * (value_type rhs) const;
		Vec3 operator / (value_type rhs) const;
		Vec3 operator ^ (const Vec3& rhs) const;
		Vec3& operator += (const Vec3& rhs);
		Vec3& operator -= (const Vec3& rhs);
		Vec3& operator *= (value_type rhs);
		Vec3& operator /= (value_type rhs);

		value_type length() const;
		value_type length2() const;
		value_type normalize();

		uint32 asBGR() const;
		uint32 asABGR() const;
		uint32 asRGB() const;
		uint32 asRGBA() const;
		static Vec3 fromRGB(uint32 rgb);
		static void fromRGB(Vec3& vec, uint32 rgb);
	};
	typedef Vec3<float> Vec3f;
	typedef Vec3<double> Vec3d;

	template<typename T>
	class Vec4
	{
	public:
		typedef T value_type;
		enum { num_components = 4 };

		value_type _v[4];

		Vec4();
		Vec4(value_type x, value_type y, value_type z, value_type w);
		Vec4(const Vec3<T>& v3, value_type w);
		Vec4(const Vec4& vec);

		bool operator == (const Vec4& v) const;
		bool operator != (const Vec4& v) const;
		bool operator <  (const Vec4& v) const;

		value_type* ptr();
		const value_type* ptr() const;

		void set(value_type x, value_type y, value_type z, value_type w);
		value_type& operator [] (int i);
		value_type  operator [] (int i) const;
		value_type& x();
		value_type& y();
		value_type& z();
		value_type& w();
		value_type x() const;
		value_type y() const;
		value_type z() const;
		value_type w() const;

		bool valid() const;
		bool isNaN() const;

		Vec4 operator - () const;
		Vec4 operator + (const Vec4& rhs) const;
		Vec4 operator - (const Vec4& rhs) const;
		value_type operator * (const Vec4& rhs) const;
		Vec4 operator * (value_type rhs) const;
		Vec4 operator / (value_type rhs) const;
		Vec4 operator ^ (const Vec4& rhs) const;
		Vec4& operator += (const Vec4& rhs);
		Vec4& operator -= (const Vec4& rhs);
		Vec4& operator *= (value_type rhs);
		Vec4& operator /= (value_type rhs);

		value_type length() const;
		value_type length2() const;
		value_type normalize();
		uint32 asABGR() const;
		uint32 asRGBA() const;
	};
	typedef Vec4<float> Vec4f;
	typedef Vec4<double> Vec4d;


	template<typename T>
	class Quat
	{
	public:
		typedef T value_type;
		value_type  _v[4];

		Quat();
		Quat(value_type x, value_type y, value_type z, value_type w);
		Quat(const Vec4<T>& v);
		Quat(value_type angle, const Vec3<T>& axis);
		Quat(value_type angle1, const Vec3<T>& axis1,
			value_type angle2, const Vec3<T>& axis2,
			value_type angle3, const Vec3<T>& axis3);

		Quat& operator = (const Quat& v);
		bool operator == (const Quat& v) const;
		bool operator != (const Quat& v) const;
		bool operator <  (const Quat& v) const;

		void set(value_type x, value_type y, value_type z, value_type w);
		value_type & operator [] (int i);
		value_type   operator [] (int i) const;
		value_type & x();
		value_type & y();
		value_type & z();
		value_type & w();
		value_type x() const;
		value_type y() const;
		value_type z() const;
		value_type w() const;

		Quat operator - () const;
		Quat operator + (const Quat& rhs) const;
		Quat operator - (const Quat& rhs) const;
		Quat operator * (value_type rhs) const;
		Quat operator * (const Quat& rhs) const;
		Vec3<T> operator * (const Vec3<T>& v) const;
		Quat operator / (value_type rhs) const;
		Quat operator / (const Quat& denom) const;

		Quat& operator += (const Quat& rhs);
		Quat& operator -= (const Quat& rhs);
		Quat& operator *= (value_type rhs);
		Quat& operator *= (const Quat& rhs);
		Quat& operator /= (value_type rhs);
		Quat& operator /= (const Quat& denom);

		void makeRotate(value_type angle, value_type x, value_type y, value_type z);
		void makeRotate(value_type angle, const Vec3<T>& vec);
		void makeRotate(value_type angle1, const Vec3<T>& axis1, value_type angle2, const Vec3<T>& axis2, value_type angle3, const Vec3<T>& axis3);
		void makeRotate(const Vec3<T>& from, const Vec3<T>& to);
		void getRotate(value_type & angle, value_type & x, value_type & y, value_type & z) const;
		void getRotate(value_type & angle, Vec3<T>& vec) const;

		value_type length() const;
		value_type length2() const;
		Quat conj() const;
		bool zeroRotation() const;
		Quat getInverse() const;
		Quat& inverse();
		void slerp(value_type  t, const Quat& from, const Quat& to);
	};
	typedef Quat<float> Quatf;
	typedef Quat<double> Quatd;

	template<typename T>
	class Matrix
	{
	public:
		typedef T value_type;
		union
		{
			struct
			{
				T _m00, _m01, _m02, _m03;
				T _m10, _m11, _m12, _m13;
				T _m20, _m21, _m22, _m23;
				T _m30, _m31, _m32, _m33;
			};
			T _m[16];
			T _mat[4][4];
		};

		Matrix();
		Matrix(const Matrix& mat);
		Matrix(value_type a00, value_type a01, value_type a02, value_type a03,
			value_type a10, value_type a11, value_type a12, value_type a13,
			value_type a20, value_type a21, value_type a22, value_type a23,
			value_type a30, value_type a31, value_type a32, value_type a33);

		const value_type* operator[] (int32 i) const;

		value_type* operator[] (int32 i);

		value_type& operator()(int32 row, int32 col);

		value_type operator()(int32 row, int32 col) const;

		bool valid() const;

		bool isNaN() const;

		void set(T const * const ptr);

		void set(value_type a00, value_type a01, value_type a02, value_type a03,
			value_type a10, value_type a11, value_type a12, value_type a13,
			value_type a20, value_type a21, value_type a22, value_type a23,
			value_type a30, value_type a31, value_type a32, value_type a33);

		Matrix& operator = (const Matrix& rhs);
		bool operator == (const Matrix& m2) const;
		bool operator != (const Matrix& m2) const;

		value_type * ptr();
		const value_type * ptr() const;

		bool isIdentity() const;
		void makeIdentity();
		void makeScale(value_type x, value_type y, value_type z);
		void makeScale(const Vec3<T>& v);
		void makeTranslate(value_type x, value_type y, value_type z);
		void makeTranslate(const Vec3<T>& v);
		void makeRotate(value_type c, value_type s, value_type axisX, value_type axisY, value_type axisZ);
		void makeRotate(value_type angle, value_type x, value_type y, value_type z);
		void makeRotate(value_type angle, const Vec3<T>& axis);
		void makeRotateX(value_type angle);
		void makeRotateY(value_type angle);
		void makeRotateZ(value_type angle);
		void makeRotateXYZ(value_type rad_x, value_type rad_y, value_type rad_z);
		void makeRotate(const Quat<T>& quat);

		Vec3<T> getTrans() const;
		Vec3<T> getScale() const;
		Quat<T> getRotate() const;
		Matrix getTranspose() const;
		void inverse();

		Vec3<T> preMult(const Vec3<T>& v) const;
		Vec3<T> postMult(const Vec3<T>& v) const;
		Vec4<T> preMult(const Vec4<T>& v) const;
		Vec4<T> postMult(const Vec4<T>& v) const;
		Vec3<T> operator * (const Vec3<T>& v) const;
		Vec4<T> operator * (const Vec4<T>& v) const;
		Matrix operator * (const Matrix &m2) const;
		void operator *= (const Matrix& m2);

		void preMultTranslate(const Vec3<T>& v);
		void makeLookAt(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up);
		void makeFrustum(value_type left, value_type right, value_type bottom, value_type top, value_type zNear, value_type zFar);
		void makePerspective(value_type fovy, value_type aspectRatio, value_type zNear, value_type zFar);
		void makeOrtho(value_type left, value_type right, value_type bottom, value_type top, value_type zNear, value_type zFar);
		bool getFrustum(value_type& left, value_type& right, value_type& bottom, value_type& top, value_type& zNear, value_type& zFar) const;
	};
	typedef Matrix<float> Matrixf;
	typedef Matrix<double> Matrixd;

	template<typename T>
	Vec4<T> operator * (const Vec4<T>& v, const Matrix<T>& m)
	{
		return m.preMult(v);
	}

	template<typename T>
	Vec3<T> operator * (const Vec3<T>& v, const Matrix<T>& m)
	{
		return m.preMult(v);
	}

	template<typename T>
	class BoundingBox
	{
	public:
		typedef T value_type;
		typedef typename Vec3<T> vec_type;

		vec_type _min;
		vec_type _max;

		BoundingBox();
		BoundingBox(value_type xmin, value_type ymin, value_type zmin, value_type xmax, value_type ymax, value_type zmax);
		BoundingBox(const vec_type& min, const vec_type& max);

		void init();
		bool valid() const;

		void set(value_type xmin, value_type ymin, value_type zmin, value_type xmax, value_type ymax, value_type zmax);
		void set(const vec_type& min, const vec_type& max);

		value_type& xMin();
		value_type xMin() const;
		value_type& yMin();
		value_type yMin() const;
		value_type& zMin();
		value_type zMin() const;
		value_type& xMax();
		value_type xMax() const;
		value_type& yMax();
		value_type yMax() const;
		value_type& zMax();
		value_type zMax() const;

		vec_type center() const;
		value_type radius() const;
		value_type radius2() const;
		vec_type corner(unsigned int pos) const;

		template<typename F>
		void expandBy(const Vec3<F>& v);
		void expandBy(const vec_type& v);
		void expandBy(value_type x, value_type y, value_type z);
		void expandBy(const BoundingBox& bb);
		void expandBy(const vec_type& center, value_type radius);

		BoundingBox intersect(const BoundingBox& bb) const;
		bool intersects(const BoundingBox& bb) const;
		bool contains(const vec_type& v) const;

		void setTransformedAABB(const Matrix<value_type>& matrix);
	};
	typedef BoundingBox<float> BoundingBoxf;
	typedef BoundingBox<double> BoundingBoxd;

	template <typename T>
	class Ray
	{
	public:
		Vec3<T> _point;
		Vec3<T> _direction;

		Ray();
		Ray(const Vec3<T>& p, const Vec3<T> d);
		Ray<T>& operator = (const Ray<T> &r);
		Vec3<T> getPointOnRay(T t) const;
	};
	typedef Ray<float> Rayf;
	typedef Ray<double> Rayd;

	template<typename T>
	class Plane
	{
	public:
		typedef T value_type;
		typedef Vec3<T> Vec3_type;
		typedef Vec4<T> Vec4_type;

		enum { num_components = 3 };

		Plane();
		Plane(const Plane& pl);
		Plane(value_type a, value_type b, value_type c, value_type d);
		Plane(const Vec3_type& norm, value_type d);
		Plane(const Vec3_type& v1, const Vec3_type& v2, const Vec3_type& v3);
		Plane(const Vec3_type& norm, const Vec3_type& point);
		Plane& operator = (const Plane& pl);

		bool operator == (const Plane& plane) const;
		bool operator != (const Plane& plane) const;

		void set(const Plane& pl);
		void set(const Vec4_type& p);
		void set(value_type a, value_type b, value_type c, value_type d);
		void set(const Vec3_type& norm, double d);
		void set(const Vec3_type& v1, const Vec3_type& v2, const Vec3_type& v3);
		void set(const Vec3_type& norm, const Vec3_type& point);

		value_type* ptr();
		const value_type* ptr() const;

		void flip();
		void makeUnitLength();

		Vec3_type getNormal() const;
		value_type distance(const Vec3<T>& v) const;

		/** intersection test between plane and vertex list
		return 1 if the bs is completely above plane,
		return 0 if the bs intersects the plane,
		return -1 if the bs is completely below the plane.*/
		int intersect(const Vec3<T>* vertices, int32 size) const;

	protected:
		value_type _fv[4];
	};
	typedef Plane<float> Planef;
	typedef Plane<double> Planed;

	//0x00 无相交发生,0x01 相交,0x02 射线起点在包围盒内
	template <typename T>
	uint8 intersectRay_BoudingBox(const Ray<T>& ray, const BoundingBox<T>& aabb, Vec3<T>& output);

	template <typename T>
	bool intersectRay_Triangle(const Ray<T>& ray, const Vec3<T>& v0, const Vec3<T>& v1, const Vec3<T>& v2, Vec3<T>& output);

	template<typename T>
	void getRay(int32 screenX, int32 screenY, const Matrix<T>& view, const Matrix<T>& pro,int32 width,int32 height, Ray<T>& ray);

	template<typename T>
	void getWorldPoint(T screenX, T screenY, T z, const Matrix<T>& view, const Matrix<T>& pro, int32 width, int32 height, Vec3<T>& worldPoint);

#if DOUBLE_PRECISION == 1

	typedef Vec2d Real2;
	typedef Vec3d Real3;
	typedef Vec4d Real4;
	typedef Quatd RQuat;
	typedef Matrixd RMatrix;
	typedef BoundingBoxd RBoundingBox;
	typedef Rayd RRay;
	typedef Planed RPlane;

#else

	typedef Vec2f Real2;
	typedef Vec3f Real3;
	typedef Vec4f Real4;
	typedef Quatf RQuat;
	typedef Matrixf RMatrix;
	typedef BoundingBoxf RBoundingBox;
	typedef Rayf RRay;
	typedef Planef RPlane;

#endif
}

#include "Mathsimp.h"

#endif