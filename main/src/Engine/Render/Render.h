#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H 1

#include <open/Array.h>

namespace open
{
	class IRenderSorter;
	class DeferredShader;
	class ForwardShader;
	class ShadowRender;
	class INode;
	class IRenderPool;
	class IRenderTarget;
	class ICamera;
	struct RenderTrace;

	class Render
	{
	public:

		Render();

		~Render();

		void setRenderPool(IRenderPool* renderPool) { _renderPool = renderPool; }

		void setRenderTarget(IRenderTarget* renderTarget) { _renderTarget = renderTarget; }

		void render(ICamera* camera, const DynamicArray<INode*>& drts);

	private:
		void renderObjects(ICamera* camera, RenderTrace* rts, int32 size, RenderTrace* transparents, int32 transparentsSize);

		void renderShadow(const DynamicArray<INode*>& drts);

	private:
		IRenderSorter* _renderSort;
		DeferredShader* _deferredShader;
		ForwardShader* _forwardShader;
		ShadowRender* _shadowRender;

		IRenderPool* _renderPool;
		IRenderTarget* _renderTarget;
	};
}

#endif