#ifndef MESH_HEIGHTMAP_H
#define MESH_HEIGHTMAP_H 1

#include <open/interface/Render/DrawableInterface.h>

namespace open
{
	class Mesh;
	class HeightMapDrawalbe
	{
	public:

		HeightMapDrawalbe(Mesh* geometry, const SolidShape::Shape::HeightMap& hm);

	private:

		inline const Real* getHeightList() const 
		{ 
			return _hm.heights;
		}

		inline void setHeight(int32 c,int32 r,Real value)
		{
			(_hm.heights)[c + r * _hm.columns] = value;
		}

		inline Real& getHeight(int32 c,int32 r) 
		{
			return (_hm.heights)[c + r * _hm.columns];
		}

		inline float getHeight(int32 c,int32 r) const
		{
#if DOUBLE_PRECISION
			return static_cast<float>((_hm.heights)[c + r * _hm.columns]);
#else
			return (_hm.heights)[c + r * _hm.columns];
#endif
		}

		inline Vec3f getVertex(int32 c,int32 r) const
		{
#if DOUBLE_PRECISION
			return Vec3f(static_cast<float>(_hm.origin[0] + _hm.dx * c),
				static_cast<float>(_hm.origin[1] + _hm.dy * r),
				static_cast<float>(_hm.origin[2]+ (_hm.heights)[c + r * _hm.columns]));
#else
			return Vec3f(_hm.origin[0] + _hm.dx * c,
				_hm.origin[1] + _hm.dy * r,
				_hm.origin[2]+ (_hm.heights)[c + r * _hm.columns]);
#endif
		}

		void getNormal(int32 c,int32 r,Vec3f& noraml) const;

		void computeBound();

		void reset();

	private:
		SolidShape::Shape::HeightMap _hm;
		Mesh* _geometry;
	};
}

#endif