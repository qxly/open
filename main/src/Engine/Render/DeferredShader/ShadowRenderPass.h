#ifndef RENDERSYSTEM_SHADOWRENDERPASS_H
#define RENDERSYSTEM_SHADOWRENDERPASS_H

#include <common/Maths.h>
#include <common/Ptr.h>

namespace wyin
{
	struct GBuffer;
	class IProgram;
	class IMesh;
	class IMaterial;
	class ILight;
	class IRenderPool;
	class IFrameBuffer;
	class ITexture;
	class IShadowData;

	class ShadowRenderPass
	{
		struct ShadowRenderPassRenderScheme;

	public:

		ShadowRenderPass();

		~ShadowRenderPass();

		void renderShadow(ILight* light, IShadowData* shadowData, GBuffer* gbuffer, IRenderPool* renderPool);

	private:

		void initProgram();

		void initScreenRect();

		void initMaterial(GBuffer* gbuffer);

		void initFrameBuffer();

	private:

		IProgram* _program;
		Ptr<IMesh> _screenRect;
		Ptr<IMaterial> _shadowRenderMaterial;

		ShadowRenderPassRenderScheme* _renderScheme;
	};

	class ShadowRenderPass_Forward
	{
		struct ShadowRenderPassRenderScheme;

	public:

		ShadowRenderPass_Forward();

		~ShadowRenderPass_Forward();

		void renderShaodw(IRenderPool* renderPool, ILight* light);

	private:

		ShadowRenderPassRenderScheme* _renderScheme;
	};
}

#endif