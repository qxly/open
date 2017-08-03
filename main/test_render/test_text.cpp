
#include "common.h"
#include <vector>
#include <Engine/EngineFactoryInterface.h>

namespace open {

	class TextDemo : public Demo
	{
	public:
		virtual void createNode(ICamera* camera)
		{
			IText* text = RenderFactory::s_factory->createText();
			//text->setFont("./data/STHUPO.TTF");
			text->setText("open engine is testing text,it need new line!\n now,it's two lines ");
			text->setCharacterSize(4);
			text->setFontSize(128);

			RMatrix matrix;
			matrix.makeTranslate(Real3(500, 500, 90));

			RenderFactory::s_factory->getRenderEngine()->addRender(text, matrix);
		}
	private:
		IDrawable* createCoords()
		{
			IDrawableBase* drawable = RenderFactory::s_factory->createDrawable();

			IMesh* geometry = RenderFactory::s_factory->createMesh();
			Vec3f* vertexs = new Vec3f[4];

			uint16 i = 0;
			vertexs[i++] = Vec3f(0, 0, 0);
			vertexs[i++] = Vec3f(1, 0, 0);

			vertexs[i++] = Vec3f(0, 0, 0);
			vertexs[i++] = Vec3f(-1, 0, 0);

			vertexs[i++] = Vec3f(0, 0, 0);
			vertexs[i++] = Vec3f(0, 1, 0);

			vertexs[i++] = Vec3f(0, 0, 0);
			vertexs[i++] = Vec3f(0, -1, 0);
			
			vertexs[i++] = Vec3f(0, 0, 0);
			vertexs[i++] = Vec3f(0, 0, 1);

			vertexs[i++] = Vec3f(0, 0, 0);
			vertexs[i++] = Vec3f(0, 0, -1);

			geometry->begin(i);
			geometry->setVertexArray(vertexs);
			geometry->end();

			geometry->addIndexArray(LINE_LIST, 0U,i);

			return drawable;
		}

		~TextDemo()
		{
		}
	};
	//DECLARE_DEMO(TextDemo);
}