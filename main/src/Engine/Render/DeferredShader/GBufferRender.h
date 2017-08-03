#ifndef RENDERSYSTEM_GBUFFERRENDER_H
#define RENDERSYSTEM_GBUFFERRENDER_H 1

#include <common/Maths.h>
#include <RenderSystem/RenderInterface.h>

namespace open
{
	class IFrameBuffer;
	class IMaterial;
	class IDrawable;
	class IProgram;
	class ITexture;
	class IRenderPool;

	class GBufferRender
	{
	private:
		struct GBufferRenderScheme;
		struct GBufferShow;
	public:

		GBufferRender();

		~GBufferRender();

		IRenderTarget* getRenderTarget();

		IApplyMaterialFun& getApplyMaterialFun();

		void resizeGBuffer(int32 width, int32 height);

		GBuffer* getGBuffer() { return &_gBuffer; }

		void begin(IRenderPool* renderPool);

		void renderGBuffer();

	private:

		void init();

		void initProgram();

		void resetFrameBuffer();

	private:

		IFrameBuffer* _frameBuffer;

		GBufferRenderScheme* _renderScheme;
		GBufferShow* _gbufferShow;

		GBuffer _gBuffer;

		int32 _width;
		int32 _height;
	};
}

#endif