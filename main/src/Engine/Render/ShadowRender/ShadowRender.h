#ifndef RENDERSYSTEM_SHADOWRENDER_H
#define RENDERSYSTEM_SHADOWRENDER_H 1

#include <common/wyinDef.h>

namespace open
{
	class INode;
	class IRenderPool;
	class ShadowPass;
	class ShadowRenderPass_Forward;

	class ShadowRender
	{
	public:
		ShadowRender();

		~ShadowRender();

		virtual void renderShadow(IRenderPool* renderPool, INode** nodes, int32 size);

	private:

		ShadowPass* _shadowPass;
		ShadowRenderPass_Forward* _shadowRender_Forward;
	};
}

#endif