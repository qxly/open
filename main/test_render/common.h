#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include "terrain.h"

#include <open/Maths.h>
#include <open/Memory.h>
#include <open/Ptr.h>
#include <open/Array.h>
#include <open/InputInterface.h>

#include <open/interface/RenderSystem/RenderEnumeration.h>
#include <open/interface/RenderSystem/RenderStatesInterface.h>
#include <open/interface/RenderSystem/RenderInterface.h>
#include <open/interface/RenderSystem/RenderSystemFactoryInterface.h>

#include <open/interface/Render/DrawableInterface.h>
#include <open/interface/Render/MaterialInterface.h>
#include <open/interface/Render/RenderEngine.h>
#include <open/interface/Render/RenderFactoryInterface.h>

#include <open/interface/Engine/EngineInterface.h>

namespace open
{
	class Demo
	{
	public:
		virtual void createNode(ICamera* camera) = 0;
	};
	typedef DynamicArray<Demo*> DemoArray;

	class DemoManager
	{
	public:

		static DemoManager* getSingletonPtr()
		{
			static DemoManager dm;
			return &dm;
		}

		void addDemo(Demo* demo)
		{
			_demos.push_back(demo);
		}

		void build(ICamera* camera)
		{
			for (int32 i = 0; i < _demos.size(); i++)
			{
				_demos[i]->createNode(camera);
			}
		}

		~DemoManager()
		{
			for (int32 i = 0; i < _demos.size(); i++)
			{
				delete _demos[i];
			}
		}

	private:
		DemoArray _demos;
	};

	class RegistDemo
	{
	public:
		RegistDemo(Demo* demo)
		{
			DemoManager::getSingletonPtr()->addDemo(demo);
		}
	};

#define DECLARE_DEMO(className) static RegistDemo RegistDemo##className = RegistDemo(new className);


	inline IMesh* createPlane()
	{
		IMesh* geometry = RenderFactory::s_factory->createMesh();
		Vec3f* vertexs = new Vec3f[4];
		Vec3f* normals = new Vec3f[4];
		Vec2f* coords = new Vec2f[4];

		Vec3f center(0, 0, 0);
		vertexs[0] = (Vec3f(0, 0, 0) - center);
		vertexs[1] = (Vec3f(30, 0, 0) - center);
		vertexs[2] = (Vec3f(30, 0, 30) - center);
		vertexs[3] = (Vec3f(0,0 , 30) - center);

		normals[0] = Vec3f(0, 1, 0);
		normals[1] = Vec3f(0, 1, 0);
		normals[2] = Vec3f(0, 1, 0);
		normals[3] = Vec3f(0, 1, 0);

		coords[0] = Vec2f(0, 0);
		coords[1] = Vec2f(1, 0);
		coords[2] = Vec2f(1, 1);
		coords[3] = Vec2f(0, 1);

		uint16* indexs = new uint16[4];
		indexs[0] = 3;
		indexs[1] = 0;
		indexs[2] = 2;
		indexs[3] = 1;

		geometry->begin(4);

		geometry->setVertexArray(vertexs);
		geometry->setTextureCoordArray(0, coords);
		geometry->setNormalArray(normals);

		geometry->end();

		geometry->addIndexArray(TRIANGLE_STRIP, indexs, 4);

		return geometry;
	}

	inline IDrawable* createCoords()
	{
		IDrawableBase* drawable = RenderFactory::s_factory->createDrawable();

		IMesh* geometry = RenderFactory::s_factory->createMesh();
		drawable->addGeometry(geometry, RenderFactory::s_factory->getOrCreateMaterial("greenMaterial"));

		Vec3f* vertexs = new Vec3f[4];

		vertexs[0] = Vec3f(0, 0, 0);
		vertexs[1] = Vec3f(2000, 0, 0);
		vertexs[2] = Vec3f(0, 2000, 0);
		vertexs[3] = Vec3f(0, 0, 2000);

		geometry->begin(4);
		geometry->setVertexArray(vertexs);
		geometry->end();

		uint16* indexs = new uint16[6];
		indexs[0] = 0;
		indexs[1] = 1;
		indexs[2] = 0;
		indexs[3] = 2;
		indexs[4] = 0;
		indexs[5] = 3;
		geometry->addIndexArray(LINE_LIST, indexs, 6);

		return drawable;
	}

	inline IDrawable* createTerrain(const Real3& origin, const Real3& size)
	{
		uint32 numColumns = 38;
		uint32 numRows = 39;
		uint32 r;
		uint32 c;

		Real min_z = FLT_MAX;
		Real max_z = -FLT_MAX;
		for (r = 0; r < numRows; ++r)
		{
			for (c = 0; c < numColumns; ++c)
			{
				min_z = min(min_z, terrain_vertex[r + c*numRows][2]);
				max_z = max(max_z, terrain_vertex[r + c*numRows][2]);
			}
		}

		Real scale_z = size.z() / (max_z - min_z);

		SolidShape solidShape;
		solidShape.type = SolidShape::TYPE_HEIGHTMAP;

		solidShape.shape.heightMap.origin[0] = origin.x();
		solidShape.shape.heightMap.origin[1] = origin.y();
		solidShape.shape.heightMap.origin[2] = origin.z();

		solidShape.shape.heightMap.dx = size.x() / (Real)(numColumns - 1);
		solidShape.shape.heightMap.dy = size.y() / (Real)(numRows - 1);

		Real* heights = new Real[numRows * numColumns];
		solidShape.shape.heightMap.heights = heights;

		solidShape.shape.heightMap.columns = numColumns;
		solidShape.shape.heightMap.rows = numRows;

		for (r = 0; r < numRows; ++r)
		{
			for (c = 0; c<numColumns; ++c)
			{
				heights[c + r * numColumns] = (terrain_vertex[r + c*numRows][2] - min_z)*scale_z;
			}
		}

		ISolidGeometry* sd = RenderFactory::s_factory->createSolidGeometry();
		sd->setSoildShape(solidShape);

		RBoundingBox bbo = sd->getBoudingBox();

		IDrawableBase* drawable = RenderFactory::s_factory->createDrawable();

		IMaterial* material = RenderFactory::s_factory->createMaterial();
		{
			ITexture* texture = RenderSystemFactory::s_factory->createTexture("e:\\data\\world.png");
			ITextureUnitSet* tus = material->getTextureUnitSet();
			ITextureUnit* tu = tus->createTextureUnit(0);
			tu->setTexture(texture);
		}
		drawable->addGeometry(sd, material);

		return drawable;
	}
}

#endif