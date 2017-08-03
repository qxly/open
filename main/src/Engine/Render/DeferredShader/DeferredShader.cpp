#include "DeferredShader.h"
#include "GBufferRender.h"
#include "LightPassRender.h"

namespace open
{
	DeferredShader::DeferredShader()
	{
		_gbufferRender = new GBufferRender;
		_lightPassRender = new LightPassRender;
	}

	DeferredShader::~DeferredShader()
	{
		delete _gbufferRender;
		delete _lightPassRender;
	}

	void DeferredShader::genGBufferPass(IRenderPool* renderPool, int32 width, int32 height)
	{
		_gbufferRender->resizeGBuffer(width, height);
		_gbufferRender->begin(renderPool);
	}

	IApplyMaterialFun& DeferredShader::getGBufferApplyMaterialFun()
	{
		return _gbufferRender->getApplyMaterialFun();
	}

	GBuffer* DeferredShader::getGBuffer()
	{
		return _gbufferRender->getGBuffer();
	}

	void DeferredShader::renderGBuffer()
	{
		_gbufferRender->renderGBuffer();
	}

	void DeferredShader::processLightPass(ICamera* camear,IRenderPool* renderPool)
	{
		_lightPassRender->render(_gbufferRender->getGBuffer(), camear,renderPool);
	}
}