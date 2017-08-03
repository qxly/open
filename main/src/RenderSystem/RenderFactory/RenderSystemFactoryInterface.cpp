#include <RenderSystem/RenderSystemFactoryInterface.h>

#include "../Buffer/GPUBuffer.h"
#include "../Buffer/VertexArray.h"

#include "../RenderStates/Program.h"
#include "../RenderStates/Texture.h"
#include "../RenderStates/TextureUnit.h"
#include "../RenderStates/TextureUnitSet.h"

#include "../RenderPool/Light.h"
#include "../RenderPool/FrameBuffer.h"
#include "../RenderPool/RenderPool.h"
#include "../RenderPool/RenderWindow.h"

namespace open
{
	class RenderSystemFactory_Internal : public RenderSystemFactory
	{
	public:
		////buffer;
		virtual IGPUBuffer* createGPUBuffer(BufferUseType type);

		virtual IVertexStream* createVertexStream() { return new VertexStream; }

		virtual IVertextArray* createVertextArray() { return new VertextArray; }

		virtual IVertexArrayObject* createVertexArrayObject();

		virtual IVertexIndexObject* createVertexIndexObject();

		virtual IVertexIndexObject* createVertexIndexObject(uint16* data, uint32 count);

		virtual IVertexIndexObject* createVertexIndexObject(uint32* data, uint32 count);

		virtual IVertexIndexObject* createVertexIndexObject(uint8* data, uint32 count);

		///material
		virtual IProgram* createProgram();

		virtual ITexture* createTexture();

		virtual ITexture* createTexture(const char* filePath);

		virtual ITextureUnit* createTextureUnit();

		virtual ITextureUnitSet* createTextureUnitSet();

		///render
		virtual IRenderWindowFactory* getRenderWindowFactory();

		virtual IRenderBuffer* createRenderBuffer();
		virtual IFrameBuffer* createFrameBuffer();

		virtual IRenderPool* createRenderPool();
	};
	struct Init_RenderSystemFactory_Internal
	{
		Init_RenderSystemFactory_Internal()
		{
			RenderSystemFactory::s_factory = new RenderSystemFactory_Internal();
		}

		~Init_RenderSystemFactory_Internal()
		{
			delete RenderSystemFactory::s_factory;
		}
	};
	static Init_RenderSystemFactory_Internal g_Init_RenderSystemFactory_Internal;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IGPUBuffer* RenderSystemFactory_Internal::createGPUBuffer(BufferUseType type) { return new GPUBuffer(type); }

	IVertexArrayObject* RenderSystemFactory_Internal::createVertexArrayObject() { return new VertexArrayObject; }

	IVertexIndexObject* RenderSystemFactory_Internal::createVertexIndexObject() { return new VertexIndexObject; }

	IVertexIndexObject* RenderSystemFactory_Internal::createVertexIndexObject(uint16* data, uint32 count) { return new VertexIndexObject(data, count); }

	IVertexIndexObject* RenderSystemFactory_Internal::createVertexIndexObject(uint32* data, uint32 count) { return new VertexIndexObject(data, count); }

	IVertexIndexObject* RenderSystemFactory_Internal::createVertexIndexObject(uint8* data, uint32 count) { return new VertexIndexObject(data, count); }

	///material
	IProgram* RenderSystemFactory_Internal::createProgram() { return new Program; }

	ITexture* RenderSystemFactory_Internal::createTexture() { return new Texture; }

	ITexture* RenderSystemFactory_Internal::createTexture(const char* filePath){ return Texture::createImage(filePath); }

	ITextureUnit* RenderSystemFactory_Internal::createTextureUnit() { return new TextureUnit; }

	ITextureUnitSet* RenderSystemFactory_Internal::createTextureUnitSet() { return new TextureUnitSet; }

	///render
	IRenderWindowFactory* RenderSystemFactory_Internal::getRenderWindowFactory(){ return new RenderWindowFactory; }

	IRenderBuffer* RenderSystemFactory_Internal::createRenderBuffer() { return new RenderBuffer; }
	IFrameBuffer* RenderSystemFactory_Internal::createFrameBuffer() { return new FrameBuffer; }

	IRenderPool* RenderSystemFactory_Internal::createRenderPool() { return new RenderPool; }
}