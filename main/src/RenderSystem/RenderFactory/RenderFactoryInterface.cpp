#include <common/interface/RenderSystem/RenderSystemFactoryInterface.h>

#include "../Buffer/GPUBuffer.h"

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
	class RenderFactory_Internal : public RenderSystemFactory
	{
	public:
		////buffer;
		virtual IGPUBuffer* createGPUBuffer(BufferUseType type);

		virtual IVertexArrayObject* createVertexArrayObject();

		virtual IVertexIndexObject* createVertexIndexObject();

		virtual IVertexIndexObject* createVertexIndexObject(uint16* data, int32 count);

		virtual IVertexIndexObject* createVertexIndexObject(uint32* data, int32 count);

		virtual IVertexIndexObject* createVertexIndexObject(int32* data, int32 count);

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
	struct Init_RenderFactory_Internal
	{
		Init_RenderFactory_Internal()
		{
			RenderSystemFactory::s_factory = new RenderFactory_Internal();
		}

		~Init_RenderFactory_Internal()
		{
			delete RenderSystemFactory::s_factory;
		}
	};
	static Init_RenderFactory_Internal g_Init_RenderFactory_Internal;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IGPUBuffer* RenderFactory_Internal::createGPUBuffer(BufferUseType type) { return new GPUBuffer(type); }

	IVertexArrayObject* RenderFactory_Internal::createVertexArrayObject() { return new VertexArrayObject; }

	IVertexIndexObject* RenderFactory_Internal::createVertexIndexObject() { return new VertexIndexObject; }

	IVertexIndexObject* RenderFactory_Internal::createVertexIndexObject(uint16* data, int32 count) { return new VertexIndexObject(data, count); }

	IVertexIndexObject* RenderFactory_Internal::createVertexIndexObject(uint32* data, int32 count) { return new VertexIndexObject(data, count); }

	IVertexIndexObject* RenderFactory_Internal::createVertexIndexObject(int32* data, int32 count) { return new VertexIndexObject(data, count); }

	///material
	IProgram* RenderFactory_Internal::createProgram() { return new Program; }

	ITexture* RenderFactory_Internal::createTexture() { return new Texture; }

	ITexture* RenderFactory_Internal::createTexture(const char* filePath){ return Texture::createImage(filePath); }

	ITextureUnit* RenderFactory_Internal::createTextureUnit() { return new TextureUnit; }

	ITextureUnitSet* RenderFactory_Internal::createTextureUnitSet() { return new TextureUnitSet; }

	///render
	IRenderWindowFactory* RenderFactory_Internal::getRenderWindowFactory(){ return new RenderWindowFactory; }

	IRenderBuffer* RenderFactory_Internal::createRenderBuffer() { return new RenderBuffer; }
	IFrameBuffer* RenderFactory_Internal::createFrameBuffer() { return new FrameBuffer; }

	IRenderPool* RenderFactory_Internal::createRenderPool() { return new RenderPool; }
}