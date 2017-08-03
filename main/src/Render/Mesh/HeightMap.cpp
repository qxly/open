#include "HeightMap.h"
#include "Mesh.h"
#include <open/Array.h>
#include <open/interface/RenderSystem/RenderEnumeration.h>

namespace open
{
	HeightMapDrawalbe::HeightMapDrawalbe(Mesh* geometry, const SolidShape::Shape::HeightMap& hm)
		:_geometry(geometry),_hm(hm)
	{
		reset();
	}

	void HeightMapDrawalbe::getNormal(int32 c,int32 r,Vec3f& normal) const
	{
		float dz_dx;

#if DOUBLE_PRECISION
		float dx = static_cast<float>(_hm.dx);
#else
		float dx = _hm.dx;
#endif
		if (c == 0)
		{
			dz_dx = (getHeight(c + 1,r) - getHeight(c,r)) / dx;
		}
		else if (c == _hm.columns - 1)
		{
			dz_dx = (getHeight(c,r) - getHeight(c - 1,r)) / dx;
		}
		else
		{
			dz_dx = 0.5f*(getHeight(c + 1,r) - getHeight(c - 1,r)) / dx;
		}

		float dz_dy;

#if DOUBLE_PRECISION
		float dy = static_cast<float>(_hm.dy);
#else
		float dy = _hm.dx;
#endif

		if (r == 0)
		{
			dz_dy = (getHeight(c,r + 1) - getHeight(c,r)) / dy;
		}
		else if (r == _hm.rows - 1)
		{
			dz_dy = (getHeight(c,r) - getHeight(c,r-1)) / dy;
		}
		else
		{
			dz_dy = 0.5f * (getHeight(c,r + 1 ) - getHeight(c,r - 1)) / dy;
		}

		normal.set(-dz_dx, -dz_dy,1.0f);
		normal.normalize();
	}

	void HeightMapDrawalbe::computeBound()
	{
		Real zMin = FLT_MAX;
		Real zMax = -FLT_MAX;

		for(uint32 row = 0; row < _hm.rows; ++row)
		{
			for(uint32 col = 0; col < _hm.columns; ++col)
			{
				Real z = getHeight(col,row);
				if (z < zMin) 
					zMin = z;
				if (z > zMax) 
					zMax = z;
			}
		}
		RBoundingBox boudingBox;
		boudingBox._min.set(0.0f + _hm.origin[0],0.0f + _hm.origin[1],zMin + _hm.origin[2]);
		boudingBox._max = boudingBox._min;

		boudingBox.expandBy(Real3(_hm.dx * (_hm.columns - 1) + _hm.origin[0] , _hm.dy * (_hm.rows - 1) + _hm.origin[1],zMax + _hm.origin[2] ));
		_geometry->autoBoundingBox(false);
		_geometry->setBoudingBox(boudingBox);
	}

	void HeightMapDrawalbe::reset()
	{
		if (_hm.columns==0 || _hm.rows==0) 
			return;

		Vec3f vertex;
		Vec3f normal;
		Vec2f texCoord;
		float u,v;

#if DOUBLE_PRECISION
		float dx = static_cast<float>(_hm.dx);
		float dy = static_cast<float>(_hm.dy);
		float originx = static_cast<float>(_hm.origin[0]);
		float originy = static_cast<float>(_hm.origin[1]);
#else
		float dx = _hm.dx;
		float dy = _hm.dx;
		float originx = _hm.origin[0];
		float originy = _hm.origin[1];
#endif

		DynamicArray<Vec3f> verts;
		DynamicArray<Vec2f> texCoords;
		DynamicArray<Vec3f> normals;
		DynamicArray<uint32> indexs;

		for(uint32 r = 0; r < _hm.rows;++r)
		{
			for(uint32 c = 0; c < _hm.columns;++c)
			{
				vertex = getVertex(c,r);

				u = (vertex.x() - originx) / (dx * _hm.rows-1);
				v = (vertex.y() - originy) / (dy * _hm.columns-1);
				texCoord.set(u,v);

				getNormal(c,r,normal);

				verts.push_back(vertex);
				normals.push_back(normal);
				texCoords.push_back(texCoord);

				int32 num = r * _hm.columns + c;

				if(c == _hm.columns - 1)
					continue;

				if(r == _hm.rows - 1)
					continue;

				indexs.push_back(num);
				indexs.push_back(num + 1);
				indexs.push_back(num + 1 + _hm.columns);

				indexs.push_back(num);
				indexs.push_back(num + 1 + _hm.columns);
				indexs.push_back(num + _hm.columns);
			}
		}
		_geometry->begin(verts.size());
		_geometry->setVertexArray(verts.getDataPointer());
		_geometry->setNormalArray(normals.getDataPointer());
		_geometry->setTextureCoordArray(0,texCoords.getDataPointer());
		_geometry->end();

		if(_geometry->getNumIndexArray())
			_geometry->setIndexArray(0,TRIANGLE_LIST,indexs.getDataPointer(),indexs.size());
		else
			_geometry->addIndexArray(TRIANGLE_LIST,indexs.getDataPointer(),indexs.size());

		verts.detachPointer();
		normals.detachPointer();
		texCoords.detachPointer();
		indexs.detachPointer();

		computeBound();
	}
}