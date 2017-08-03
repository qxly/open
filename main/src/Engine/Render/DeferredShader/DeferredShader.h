#ifndef RENDERSYSTEM_DEFERREDSHADER_H
#define RENDERSYSTEM_DEFERREDSHADER_H 1

#include "RenderSystem/RenderInterface.h"

namespace open
{
	class INode;
	class GBufferRender;
	class LightPassRender;
	class ICamera;
	struct DrawableRenderTrace;

	class DeferredShader
	{
	public:
		DeferredShader();

		~DeferredShader();

		virtual void genGBufferPass(IRenderPool* renderPool, int32 width, int32 height);

		virtual IApplyMaterialFun& getGBufferApplyMaterialFun();

		virtual GBuffer* getGBuffer();

		virtual void processLightPass(ICamera* camera, IRenderPool* renderPool);

		virtual void renderGBuffer();

	private:
		GBufferRender* _gbufferRender;
		LightPassRender* _lightPassRender;
	};
}

#endif