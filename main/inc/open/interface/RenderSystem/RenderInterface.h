#ifndef RENDERSYSTEM_RENDERINTERFACE_H
#define RENDERSYSTEM_RENDERINTERFACE_H

#include <functional>
#include <open/Maths.h>

namespace open
{
	struct RenderStateSet;
	class IProgram;
	class IRenderPool;
	class ITexture;
	class ITextureUnitSet;
	class IVertexArrayObject;
	class IVertexIndexObject;
	enum LightType : uint8;
	enum PixelInternalFormat : uint16;
	enum FrameBufferAttachment : uint16;
	enum DrawMode : uint8;

	class IShadowData
	{
	public:
		virtual ~IShadowData() {}
	};

	class ILight
	{
	public:
		virtual ~ILight() {}

		virtual void setLightType(LightType light) = 0;
		virtual LightType getLightType() = 0;

		virtual void setLightRange(Real near, Real far) = 0;
		virtual void getLightRange(Real& near, Real& far) = 0;

		virtual void setLightPosition(const Real3& position) = 0;
		virtual const Real3& getLightPosition() = 0;

		virtual void setLightDirection(const Real3& direction) = 0;
		virtual const Real3& getLightDirection() = 0;

		virtual void setAmbientColor(const Real3& color) = 0;
		virtual const Real3& getAmbientColor() = 0;

		virtual void setDiffuseColor(const Real3& color) = 0;
		virtual const Real3& getDiffuseColor() = 0;

		virtual void setRenderMask(uint32 renderMask) = 0;
		virtual uint32 getRenderMask() = 0;

		virtual IShadowData* getShadowData() = 0;
		virtual void setShadowData(IShadowData* shadowData) = 0;
	};

	class ILightManager
	{
	public:

		virtual ~ILightManager() {}

		virtual ILight* createLight() = 0;

		virtual void addLight(ILight* light) = 0;

		virtual void removeLight(ILight* light) = 0;

		virtual void setMainLight(ILight* light) = 0;

		virtual ILight* getMainLight() = 0;

		virtual void begin() = 0;

		virtual ILight* nextIterator() = 0;
	};

	class IRenderWindow;
	class IFrameBuffer;

	class IRenderTarget
	{
	public:
		virtual ~IRenderTarget() {}

		virtual void apply() = 0;

		virtual int32 getWidth() = 0;

		virtual int32 getHeight() = 0;

		virtual IRenderWindow* asRenderWindow() { return nullptr; }

		virtual IFrameBuffer* asFrameBuffer() { return nullptr; }
	};

	struct WindowTraits
	{
		int32 x;
		int32 y;
		int32 width;
		int32 height;

		Handle parentHandle;

		WindowTraits()
			:x(0), y(0), width(1024), height(768), parentHandle(0) {}
	};

	class IInputQueue;
	class IRenderWindow : public IRenderTarget
	{
	public:
		virtual bool setupWindow(const WindowTraits& traits) = 0;

		virtual IInputQueue* getInputQueue() = 0;

		virtual void* getWindowHandle() = 0;

		virtual void swapBuffer() = 0;

		virtual IRenderWindow* asRenderWindow() { return this; }
	};

	class IRenderWindowFactory
	{
	public:

		typedef std::function<IRenderWindow*(void)> RenderWindowCreator;

	public:

		virtual ~IRenderWindowFactory() {}

		virtual IRenderWindow* createRenderWindow(const char* windowType = "win32") = 0;

		virtual void registeRenderWindowCreator(const char* windowType, RenderWindowCreator creator) = 0;
	};

	class IRenderBuffer
	{
	public:

		virtual ~IRenderBuffer() {}

		virtual void alloc(int32 width, int32 height, PixelInternalFormat format) = 0;

		virtual int32 getWidth() = 0;

		virtual int32 getHeight() = 0;
	};

	class IFrameBuffer : public IRenderTarget
	{
	public:

		virtual void attach(FrameBufferAttachment ap, IRenderBuffer* renderable) = 0;

		virtual void attach(FrameBufferAttachment ap, ITexture* texture) = 0;

		virtual void detach(FrameBufferAttachment ap) = 0;

		virtual void detach() = 0;

		virtual IFrameBuffer* asFrameBuffer() { return this; }
	};

	class IRenderPool
	{
	public:

		virtual ~IRenderPool() {}

		virtual void setViewMatrix(const RMatrix& viewMatrix) = 0;

		virtual void setProjectMatrix(const RMatrix& projectMatrix) = 0;

		virtual const RMatrix& getViewMatrix() = 0;

		virtual const RMatrix& getProjectMatrix() = 0;

		virtual IRenderTarget* getRenderTarget() = 0;

		virtual void applyRenderTarget(IRenderTarget* renderTarget) = 0;

		virtual IRenderTarget* getDefaultRenderTarget() = 0;

		virtual void setDefaultRenderTarget(IRenderTarget* renderTarget) = 0;

		virtual ILightManager* getLightManager() = 0;

		virtual void resetRenderState() = 0;

		virtual void setClearColor(const Vec4f& color) = 0;

		virtual void setClearDepth(Real depth) = 0;

		virtual void setClearStencil(uint32 value) = 0;

		virtual void clear(uint32 bufferBit) = 0;

		virtual void applyViewport(int32 x, int32 y, int32 width, int32 height) = 0;

		virtual void applyProgram(IProgram* program) = 0;

		virtual void applyRenderStateSet(const RenderStateSet& rss) = 0;

		virtual void applyTextureUnits(ITextureUnitSet * unitSet) = 0;

		virtual void beginStatisticsBatch() = 0;

		virtual int32 endStatisticsBatch() = 0;

		virtual void apply(IVertexArrayObject* vao, IVertexIndexObject* indexs, DrawMode drawMode, int32 primcount = 1) = 0;

		virtual void apply(IVertexArrayObject* vao, int32 start, int32 count, DrawMode drawMode, int32 primcount = 1) = 0;

		virtual void garbageResource() = 0;
	};

	struct GBuffer
	{
		ITexture* depthTexture;
		ITexture* normal_shininessTexture;
		ITexture* diffuse_specularTexture;
		ITexture* emissive_materialTexture;
	};
}

#endif