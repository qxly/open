#ifndef RENDERSYSTEM_RENDERPOOL_H
#define RENDERSYSTEM_RENDERPOOL_H 1

#include <RenderSystem/RenderInterface.h>
#include <open/Memory.h>
#include <open/IntrusiveList.h>
#include <string>

#include <vector>

namespace open
{
	class IVertexArrayObject;
	class IVertexIndexObject;
	class IMaterial;
	struct RenderStateSet;

	class RenderPool : public IRenderPool PUBLIC_OBJECT_DEBUG
	{
	public:

		RenderPool();

		~RenderPool();

		virtual void setViewMatrix(const RMatrix& viewMatrix)
		{
			_viewMatrix = viewMatrix;
		}

		virtual void setProjectMatrix(const RMatrix& projectMatrix)
		{
			_projectMatrix = projectMatrix;
		}

		virtual const RMatrix& getViewMatrix() { return _viewMatrix; }

		virtual const RMatrix& getProjectMatrix() { return _projectMatrix; }

		virtual void clear(uint32 bufferBit);

		virtual void setClearColor(const Vec4f& color) { _clearColor = color; }

		virtual void setClearDepth(Real depth) { _clearDepth = depth; }

		virtual void setClearStencil(uint32 value) { _clearStencil = value; }

		virtual void resetRenderState();

		virtual IRenderTarget* getRenderTarget() { return _currentRenderTarget; }

		virtual ILightManager* getLightManager() { return _lightManager; }

		virtual void applyViewport(int32 x, int32 y, int32 width, int32 height);

		virtual void applyRenderTarget(IRenderTarget* renderTarget);

		virtual IRenderTarget* getDefaultRenderTarget() { return _mainRenderTarget; }

		virtual void setDefaultRenderTarget(IRenderTarget* renderTarget) { _mainRenderTarget = renderTarget; }

		virtual void applyProgram(IProgram* program);

		virtual void applyRenderStateSet(const RenderStateSet& rss);

		virtual void applyTextureUnits(ITextureUnitSet* unitSet);

		virtual void beginStatisticsBatch() { _batch = 0; }

		virtual int32 endStatisticsBatch() { return _batch; }

		virtual void apply(IVertexArrayObject* vao, IVertexIndexObject* indexs, DrawMode drawMode, int32 primcount = 1);

		virtual void apply(IVertexArrayObject* vao, int32 start, int32 count, DrawMode drawMode, int32 primcount = 1);

		virtual void garbageResource();

	private:

		RMatrix _viewMatrix;
		RMatrix _projectMatrix;

		Vec4f _clearColor;
		Real _clearDepth = 1;
		uint32 _clearStencil = 1;

		bool _forceAppMaterial = false;

		IVertexArrayObject* _currentVAO = nullptr;
		IVertexIndexObject* _currentIndexs = nullptr;

		IRenderTarget* _currentRenderTarget = nullptr;
		IProgram* _currentProgram = nullptr;

		ILightManager* _lightManager = nullptr;

		IRenderTarget* _mainRenderTarget = nullptr;

		int32 _batch;
	};
}

#endif