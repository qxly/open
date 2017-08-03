#ifndef OPEN_LUA_FACTORY_H
#define OPEN_LUA_FACTORY_H 1

#include <open/openDef.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>
#include <Render/RenderFactoryInterface.h>

namespace open
{
	class Config;
	class IMesh;
	class ISolidGeometry;
	class IDrawable;
	class IDrawableBase;

	class IMaterial;
	class ICamera;
	class IRenderSchema;
	class IRenderEngine;

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

	inline IGPUBuffer* createGPUBuffer(BufferUseType type) { return RenderSystemFactory::s_factory->createGPUBuffer(type); }

	inline IVertexStream* createVertexStream() { return RenderSystemFactory::s_factory->createVertexStream(); }

	inline IVertextArray* createVertextArray() { return RenderSystemFactory::s_factory->createVertextArray(); }

	inline IVertexArrayObject* createVertexArrayObject() { return RenderSystemFactory::s_factory->createVertexArrayObject(); }

	inline IVertexIndexObject* createVertexIndexObject() { return RenderSystemFactory::s_factory->createVertexIndexObject(); }

	inline IVertexIndexObject* createVertexIndexObject(uint16* data, uint32 count) { return RenderSystemFactory::s_factory->createVertexIndexObject(data, count); }

	inline IVertexIndexObject* createVertexIndexObject(uint32* data, uint32 count) { return RenderSystemFactory::s_factory->createVertexIndexObject(data, count); }

	inline IProgram* createProgram() { return RenderSystemFactory::s_factory->createProgram(); }

	inline ITexture* createTexture() { return RenderSystemFactory::s_factory->createTexture(); }

	inline ITexture* createTexture(const char* filePath) { return RenderSystemFactory::s_factory->createTexture(filePath); }

	inline ITextureUnit* createTextureUnit() { return RenderSystemFactory::s_factory->createTextureUnit(); }

	inline ITextureUnitSet* createTextureUnitSet() { return RenderSystemFactory::s_factory->createTextureUnitSet(); }

	///render
	inline IRenderWindowFactory* getRenderWindowFactory() { return RenderSystemFactory::s_factory->getRenderWindowFactory(); }

	inline IRenderBuffer* createRenderBuffer() { return RenderSystemFactory::s_factory->createRenderBuffer(); }
	inline IFrameBuffer* createFrameBuffer() { return RenderSystemFactory::s_factory->createFrameBuffer(); }

	inline IRenderPool* createRenderPool() { return RenderSystemFactory::s_factory->createRenderPool(); }



	////////////////////


	inline IMesh* createMesh() { return RenderFactory::s_factory->createMesh(); }

	inline ISolidGeometry* createSolidGeometry() { return RenderFactory::s_factory->createSolidGeometry(); }

	inline IDrawableBase* createDrawable() { return RenderFactory::s_factory->createDrawable(); }

	inline IDrawable* createDrawable(const char* file, const Config& config) { return RenderFactory::s_factory->createDrawable(file, config); }

	inline IMaterial* createMaterial(const char* renderSchema = nullptr) { return RenderFactory::s_factory->createMaterial(renderSchema); }

	inline IMaterial* getMaterial(const char* name) { return RenderFactory::s_factory->getMaterial(name); }

	inline IMaterial* getOrCreateMaterial(const char* name, const char* renderSchema = nullptr) { return RenderFactory::s_factory->getOrCreateMaterial(name, renderSchema); }

	inline IRenderEngine* getRenderEngine() { return RenderFactory::s_factory->getRenderEngine(); }

	inline ICamera* createCamera() { return RenderFactory::s_factory->createCamera(); }
}

#endif