#include "common.h"
#include <vector>
#include <Engine/EngineFactoryInterface.h>

namespace open {
	class GeometryDemo : public Demo
	{
	public:
		Real3 _center = { 200, 700, 70 };

		IGeometry* createPlane()
		{
			IMesh* geometry = RenderFactory::s_factory->createMesh();
			DynamicArray<Vec3f> vertexs;
			DynamicArray<Vec3f> normals;
			DynamicArray<uint16> indexs;
			DynamicArray<Vec2f> coords;

			vertexs.push_back(Vec3f(0, 0, 0));
			vertexs.push_back(Vec3f(50, 0, 0));
			vertexs.push_back(Vec3f(50, 0, 50));
			vertexs.push_back(Vec3f(0, 0, 50));

			coords.push_back(Vec2f(0, 0));
			coords.push_back(Vec2f(1, 0));
			coords.push_back(Vec2f(1, 1));
			coords.push_back(Vec2f(0, 1));

			indexs.push_back(3);
			indexs.push_back(0);
			indexs.push_back(2);
			indexs.push_back(1);

			geometry->begin(4);

			geometry->setVertexArray(vertexs.getDataPointer());
			geometry->setTextureCoordArray(0, coords.getDataPointer());
			geometry->addIndexArray(TRIANGLE_STRIP, indexs.getDataPointer(), indexs.size());

			geometry->end();

			vertexs.detachPointer();
			indexs.detachPointer();
			coords.detachPointer();

			return geometry;
		}

		void createPlaneNode()
		{
			IGeometry* geometry = createPlane();

			{
				IDrawableBase* drwable = RenderFactory::s_factory->createDrawable();
				IMaterial* material = RenderFactory::s_factory->getOrCreateMaterial("bd_logo1");
				ITexture* texture1 = RenderSystemFactory::s_factory->createTexture("./data/t1.png");
				ITexture* texture2 = RenderSystemFactory::s_factory->createTexture("./data/t1.jpg");
				material->getTextureUnitSet()->createTextureUnit(1)->setTexture(texture1);
				material->getTextureUnitSet()->createTextureUnit(2)->setTexture(texture2);
				material->getRenderStaeSet().setBlendFunSeparate(BLENDFUN_SRC_COLOR, BLENDFUN_ONE_MINUS_DST_COLOR, BLENDFUN_SRC_ALPHA, BLENDFUN_ONE_MINUS_DST_ALPHA);
				material->getRenderStaeSet().setMode(BLEND, true);
				material->getRenderStaeSet().setMode(CULL_FACE, false);
				material->getRenderStaeSet().setDepthMask(false);

				RMatrix transe;
				transe.makeTranslate(_center);

				drwable->addGeometry(geometry, material);
				RenderFactory::s_factory->getRenderEngine()->addRender(drwable, transe);
			}
		}

		virtual void createNode(ICamera* camera)
		{
			createPlaneNode();
		}
	};
	DECLARE_DEMO(GeometryDemo);
}
