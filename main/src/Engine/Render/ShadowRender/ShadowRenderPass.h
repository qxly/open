#ifndef RENDERSYSTEM_SHADOWRENDERPASS_H
#define RENDERSYSTEM_SHADOWRENDERPASS_H

#include <common/Maths.h>
#include <common/Ptr.h>

namespace open
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

	/*class ShadowRenderPass_Deferred
	{
	struct ShadowRenderPassRenderScheme;

	public:

	ShadowRenderPass_Deferred();

	~ShadowRenderPass_Deferred();

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
	};*/

	class INode;
	struct GeneralInstance;
	class ShadowRenderPass_Forward
	{
		struct ShadowRenderPassRenderScheme;

	public:

		ShadowRenderPass_Forward();

		~ShadowRenderPass_Forward();

		void renderShaodw(IRenderPool* renderPool, ILight* light, INode** nodes, int32 size);

	private:

		ShadowRenderPassRenderScheme* _renderScheme;
		GeneralInstance* _currentDrawInstance;
	};
}

#endif