#ifndef RENDERSYSTEM_TEXTURE_H
#define RENDERSYSTEM_TEXTURE_H 1

#include <common/Ptr.h>
#include "../GPUResource.h"
#include <RenderSystem/RenderStatesInterface.h>
#include <common/Memory.h>

namespace open
{
	class Texture : public ITexture, public IGPUResource PUBLIC_OBJECT_DEBUG
	{
		DEFINE_REFCOUNT;

	public:
		Texture();

		~Texture();

		virtual void releaseGPUResource();

		virtual DataType getDataType(){ return _dataType; }

		virtual PixelInternalFormat getPixelInternalFormat() { return _internalFormat; }

		virtual PixelFormat getPixeFormat() { return _pixeFormat; }

		virtual int32 getWidth() { return _width; }

		virtual int32 getHeight() { return _height; }

		virtual int32 getDepth() { return _depth; }

		virtual void alloc(PixelInternalFormat internalFormate, int32 width, int32 height, int32 depth,
			PixelFormat pixeformat, DataType dataType, void* data, int32 packing = 1);

		virtual void subLoad(int32 startX, int32 startY, int32 width, int32 height, void* data);

		virtual bool bind();

		virtual Handle getHandle() { return _textureID; }
	public:

		static Texture* createImage(const char* path);

		static bool saveImage(Texture* image, const char* path);

	private:
		int32 _width;
		int32 _height;
		int32 _depth;

		DataType _dataType;

		PixelInternalFormat _internalFormat;
		PixelFormat _pixeFormat;

		uint32 _textureID;
		int32 _packing;

	public:

		static void* operator new (size_t size);

		static void* operator new (size_t size, Texture* texture);

		static void operator delete (void* p);

		static void operator delete (void* p, Texture* texture);

		Texture* _next;
		Texture* _perv;
	};
}
#endif