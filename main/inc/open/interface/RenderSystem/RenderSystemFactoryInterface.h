#ifndef RENDERSYSTEM_FACTORYINTERFACE_H
#define RENDERSYSTEM_FACTORYINTERFACE_H 1

#include <open/openDef.h>

namespace open
{
	class IGPUBuffer;
	class IVertexArrayObject;
	class IVertexIndexObject;
	class IVertexStream;
	class IVertextArray;

	class IInputQueue;

	class IProgram;
	class ITexture;
	class ITextureUnit;
	class ITextureUnitSet;
	class IMaterial;

	class IDrawable;
	class IGeometry;
	class ISolidGeometry;

	class ICamera;
	class ILight;
	class ILightManager;
	class IRenderTarget;
	class IRenderWindow;
	class IRenderWindowFactory;
	class IRenderBuffer;
	class IFrameBuffer;
	class IRenderPool;

	enum BufferUseType : uint8;

	class COMMON_API RenderSystemFactory
	{
	public:
		static RenderSystemFactory* s_factory;

		////buffer;
		virtual IGPUBuffer* createGPUBuffer(BufferUseType type) = 0;

		virtual IVertexStream* createVertexStream() = 0;

		virtual IVertextArray* createVertextArray() = 0;

		virtual IVertexArrayObject* createVertexArrayObject() = 0;

		virtual IVertexIndexObject* createVertexIndexObject() = 0;

		virtual IVertexIndexObject* createVertexIndexObject(uint16* data, uint32 count) = 0;

		virtual IVertexIndexObject* createVertexIndexObject(uint32* data, uint32 count) = 0;

		virtual IVertexIndexObject* createVertexIndexObject(uint8* data, uint32 count) = 0;

		///renderState
		virtual IProgram* createProgram() = 0;

		virtual ITexture* createTexture() = 0;

		virtual ITexture* createTexture(const char* filePath) = 0;

		virtual ITextureUnit* createTextureUnit() = 0;

		virtual ITextureUnitSet* createTextureUnitSet() = 0;

		///render
		virtual IRenderWindowFactory* getRenderWindowFactory() = 0;

		virtual IRenderBuffer* createRenderBuffer() = 0;
		virtual IFrameBuffer* createFrameBuffer() = 0;

		virtual IRenderPool* createRenderPool() = 0;
	};
}

#endif