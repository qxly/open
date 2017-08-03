#include "RenderScheme.h"
#include <RenderSystem/MaterialInterface.h>
#include <RenderSystem/RenderFactoryInterface.h>
#include "..\Material\Program.h"

namespace wyin
{
	DefaultRenderScheme::DefaultRenderScheme()
		:_program(NULL)
	{
	}

	void DefaultRenderScheme::begin(IRenderPool* renderPool)
	{
		if (!_program)
		{
			_program = new Program();
			_program->loadFromFile("Shaders/Basic.vert", "Shaders/Basic.frag");
		}

		_renderPool = renderPool;
		_program->apply();
	}

	void DefaultRenderScheme::applyMaterial(IMaterial* materail)
	{
		_currentMaterial = materail;

		int flag = 0;

		////光照相关
		flag = materail->getMode(LIGHT);

		/////纹理相关
		bool hasTexture = false;
		if (materail->getTextureUnitSet())
			hasTexture = materail->getTextureUnitSet()->apply(_program);

		if (hasTexture)
			flag |= 1 << 2;

		_program->setUniformi("_flag", flag);
	}

	void DefaultRenderScheme::apply(const RMatrix& modelMatrix)
	{
		RMatrix matrix = _renderPool->getProjectMatrix() * _renderPool->getViewMatrix() * modelMatrix;
		_program->setUniform16f("_mvp", matrix);

		matrix = _renderPool->getViewMatrix() * modelMatrix;
		_program->setUniform16f("_mv", matrix);

		RQuat r = matrix.getRotate();
		matrix.makeRotate(r);
		_program->setUniform16f("_normalMatrix", matrix);

		////光照相关

		Real4 lightPosition_eye;
		Real3 color;

		ILightManager* lightManager = _renderPool->getLightManager();
		ILight* light = NULL;
		lightManager->begin();

		/*while(light = lightManager->nextIterator())
		{
		if(light->getRenderMask() & drawable->getRenderMask())
		{
		const RMatrix& viewMatrix = _renderPool->getViewMatrix();
		const Real4& lp_4 = light->getLightPosition();
		Real3 lp(lp_4.x(),lp_4.y(),lp_4.z());

		lp = viewMatrix * lp;
		lp.normalize();
		lightPosition_eye = Real4(lp,light->getLightPosition().w());

		_program->setUniform4f("_lightPosition",lightPosition_eye);

		color = light->getDiffuseColor() ^ _currentMaterial->getDiffuseColor();
		_program->setUniform3f("_diffuseColor",color);

		_program->setUniform3f("_ambientColor",light->getAmbientColor());

		drawable->draw();
		}
		}*/
	}
}