
#include "common.h"
#include <vector>
#include <Engine/EngineFactoryInterface.h>

namespace open {

	struct TextDemo : public Demo
	{
		IText* text = nullptr;

		virtual void createNode(ICamera* camera)
		{
			text = RenderFactory::s_factory->createText();
			text->setFont("e:/data/STHUPO.TTF");
			text->setText("asdf");
			text->setCharacterSize(4);
			text->setFontSize(128);

			RMatrix matrix;
			matrix.makeTranslate(Real3(500, 500, 90));

			RenderFactory::s_factory->getRenderEngine()->addRender(text, matrix);
		}

		~TextDemo()
		{
		}
	};
	DECLARE_DEMO(TextDemo);
}