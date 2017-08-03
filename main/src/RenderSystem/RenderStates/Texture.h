#ifndef RENDERSYSTEM_TEXTURE_H
#define RENDERSYSTEM_TEXTURE_H 1

#include <open/Ptr.h>
#include <string>
#include "../GPUResource.h"
#include <RenderSystem/RenderStatesInterface.h>
#include <open/Memory.h>

namespace open
{
	class Texture : public TPtr<ITexture>, public IGPUResource PUBLIC_OBJECT_DEBUG
	{
	public:
		Texture();

		~Texture();

		virtual void releaseGPUResource();

		virtual DataType getDataType() { return _dataType; }

		virtual PixelInternalFormat getPixelInternalFormat() { return _internalFormat; }

		virtual PixelFormat getPixeFormat() { return _pixeFormat; }

		virtual int32 getWidth() { return _width; }

		virtual int32 getHeight() { return _height; }

		virtual int32 getDepth() { return _depth; }

		virtual void alloc(PixelInternalFormat internalFormate, int32 width, int32 height, int32 depth,
			PixelFormat pixeformat, DataType dataType, void* data, int32 packing = 1);

		virtual void subLoad(int32 startX, int32 startY, int32 width, int32 height, void* data);

		virtual bool bind(uint32 uint);

		virtual bool apply();

		virtual Handle getHandle() { return _textureID; }

		static uint32 computePixelFormat(PixelFormat pixelFormat);

		static uint32 computeFormatDataType(PixelFormat pixelFormat);

		static uint32 computeBlockSize(PixelFormat pixelFormat, uint32 packing);

		static uint32 computeNumComponents(PixelFormat pixelFormat);

		static uint32 computePixelSizeInBits(PixelFormat pixelFormat, uint32 type);

		static uint32 computeRowWidthInBytes(int32 width, PixelFormat pixelFormat, uint32 type, int packing);

		static uint32 computeImageSizeInBytes(int32 width, int32 height, int32 depth, PixelFormat pixelFormat, uint32 type, int packing = 1, int slice_packing = 1, int image_packing = 1);

		static int32 computeNearestPowerOfTwo(int32 s, float bias = 0.5f);

		static int32 computeNumberOfMipmapLevels(int32 s, int32 t = 1, int32 r = 1);

	private:
		inline bool vaild() {
			return _textureID != 0;
		}
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

#ifdef _DEBUG
		std::string _name;
#endif

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