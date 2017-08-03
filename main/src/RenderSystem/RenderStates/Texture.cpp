#include "..\..\..\inc\open\interface\RenderSystem\RenderStatesInterface.h"
#include "..\..\..\inc\open\interface\RenderSystem\RenderStatesInterface.h"
#include "Texture.h"
#include <open/interface/RenderSystem/RenderEnumeration.h>

#pragma warning(disable: 4819)
#include <freeimage/freeimage.h>
#include "../GlState.h"
#include "ReaderDDS.h"
using namespace open::gl;

namespace open
{
	uint32 Texture::computePixelFormat(PixelFormat format)
	{
		switch (format)
		{
		case(OPEN_GL_ALPHA16F_ARB) :
		case(OPEN_GL_ALPHA32F_ARB) :
			return OPEN_GL_ALPHA;

		case(OPEN_GL_LUMINANCE16F_ARB) :
		case(OPEN_GL_LUMINANCE32F_ARB) :
			return OPEN_GL_LUMINANCE;

		case(OPEN_GL_INTENSITY16F_ARB) :
		case(OPEN_GL_INTENSITY32F_ARB) :
			return OPEN_GL_INTENSITY;

		case(OPEN_GL_LUMINANCE_ALPHA16F_ARB) :
		case(OPEN_GL_LUMINANCE_ALPHA32F_ARB) :
			return OPEN_GL_LUMINANCE_ALPHA;

		case(OPEN_GL_RGB32F_ARB) :
		case(OPEN_GL_RGB16F_ARB) :
			return OPEN_GL_RGB;

		case(OPEN_GL_RGBA8) :
		case(OPEN_GL_RGBA16) :
		case(OPEN_GL_RGBA32F_ARB) :
		case(OPEN_GL_RGBA16F_ARB) :
			return OPEN_GL_RGBA;

		case(OPEN_GL_ALPHA8I_EXT) :
		case(OPEN_GL_ALPHA16I_EXT) :
		case(OPEN_GL_ALPHA32I_EXT) :
		case(OPEN_GL_ALPHA8UI_EXT) :
		case(OPEN_GL_ALPHA16UI_EXT) :
		case(OPEN_GL_ALPHA32UI_EXT) :
			return OPEN_GL_ALPHA_INTEGER_EXT;

		case(OPEN_GL_LUMINANCE8I_EXT) :
		case(OPEN_GL_LUMINANCE16I_EXT) :
		case(OPEN_GL_LUMINANCE32I_EXT) :
		case(OPEN_GL_LUMINANCE8UI_EXT) :
		case(OPEN_GL_LUMINANCE16UI_EXT) :
		case(OPEN_GL_LUMINANCE32UI_EXT) :
			return OPEN_GL_LUMINANCE_INTEGER_EXT;

		case(OPEN_GL_INTENSITY8I_EXT) :
		case(OPEN_GL_INTENSITY16I_EXT) :
		case(OPEN_GL_INTENSITY32I_EXT) :
		case(OPEN_GL_INTENSITY8UI_EXT) :
		case(OPEN_GL_INTENSITY16UI_EXT) :
		case(OPEN_GL_INTENSITY32UI_EXT) :
			return OPEN_GL_LUMINANCE_INTEGER_EXT;

		case(OPEN_GL_LUMINANCE_ALPHA8I_EXT) :
		case(OPEN_GL_LUMINANCE_ALPHA16I_EXT) :
		case(OPEN_GL_LUMINANCE_ALPHA32I_EXT) :
		case(OPEN_GL_LUMINANCE_ALPHA8UI_EXT) :
		case(OPEN_GL_LUMINANCE_ALPHA16UI_EXT) :
		case(OPEN_GL_LUMINANCE_ALPHA32UI_EXT) :
			return OPEN_GL_LUMINANCE_ALPHA_INTEGER_EXT;

		case(OPEN_GL_RGB32I_EXT) :
		case(OPEN_GL_RGB16I_EXT) :
		case(OPEN_GL_RGB8I_EXT) :
		case(OPEN_GL_RGB32UI_EXT) :
		case(OPEN_GL_RGB16UI_EXT) :
		case(OPEN_GL_RGB8UI_EXT) :
			return OPEN_GL_RGB_INTEGER_EXT;

		case(OPEN_GL_RGBA32I_EXT) :
		case(OPEN_GL_RGBA16I_EXT) :
		case(OPEN_GL_RGBA8I_EXT) :
		case(OPEN_GL_RGBA32UI_EXT) :
		case(OPEN_GL_RGBA16UI_EXT) :
		case(OPEN_GL_RGBA8UI_EXT) :
			return OPEN_GL_RGBA_INTEGER_EXT;

		case(OPEN_GL_DEPTH_COMPONENT16) :
		case(OPEN_GL_DEPTH_COMPONENT24) :
		case(OPEN_GL_DEPTH_COMPONENT32) :
		case(OPEN_GL_DEPTH_COMPONENT32F) :
		case(OPEN_GL_DEPTH_COMPONENT32F_NV) :
			return OPEN_GL_DEPTH_COMPONENT;

		default:
			return format;
		}
	}

	uint32 Texture::computeFormatDataType(PixelFormat pixelFormat)
	{
		switch (pixelFormat)
		{
		case OPEN_GL_LUMINANCE32F_ARB:
		case OPEN_GL_LUMINANCE16F_ARB:
		case OPEN_GL_LUMINANCE_ALPHA32F_ARB:
		case OPEN_GL_LUMINANCE_ALPHA16F_ARB:
		case OPEN_GL_RGB32F_ARB:
		case OPEN_GL_RGB16F_ARB:
		case OPEN_GL_RGBA32F_ARB:
		case OPEN_GL_RGBA16F_ARB:
			return OPEN_GL_FLOAT;

		case OPEN_GL_RGBA32UI_EXT:
		case OPEN_GL_RGB32UI_EXT:
		case OPEN_GL_LUMINANCE32UI_EXT:
		case OPEN_GL_LUMINANCE_ALPHA32UI_EXT:
			return OPEN_GL_UNSIGNED_INT;

		case OPEN_GL_RGB16UI_EXT:
		case OPEN_GL_RGBA16UI_EXT:
		case OPEN_GL_LUMINANCE16UI_EXT:
		case OPEN_GL_LUMINANCE_ALPHA16UI_EXT:
			return OPEN_GL_UNSIGNED_SHORT;

		case OPEN_GL_RGBA8UI_EXT:
		case OPEN_GL_RGB8UI_EXT:
		case OPEN_GL_LUMINANCE8UI_EXT:
		case OPEN_GL_LUMINANCE_ALPHA8UI_EXT:
			return OPEN_GL_UNSIGNED_BYTE;

		case OPEN_GL_RGBA32I_EXT:
		case OPEN_GL_RGB32I_EXT:
		case OPEN_GL_LUMINANCE32I_EXT:
		case OPEN_GL_LUMINANCE_ALPHA32I_EXT:
			return OPEN_GL_INT;

		case OPEN_GL_RGBA16I_EXT:
		case OPEN_GL_RGB16I_EXT:
		case OPEN_GL_LUMINANCE16I_EXT:
		case OPEN_GL_LUMINANCE_ALPHA16I_EXT:
			return OPEN_GL_SHORT;

		case OPEN_GL_RGB8I_EXT:
		case OPEN_GL_RGBA8I_EXT:
		case OPEN_GL_LUMINANCE8I_EXT:
		case OPEN_GL_LUMINANCE_ALPHA8I_EXT:
			return OPEN_GL_BYTE;

		case OPEN_GL_RGBA:
		case OPEN_GL_RGB:
		case OPEN_GL_LUMINANCE:
		case OPEN_GL_LUMINANCE_ALPHA:
		case OPEN_GL_ALPHA:
			return OPEN_GL_UNSIGNED_BYTE;

		default:
		{
			return 0;
		}
		}
	}

	uint32 Texture::computeNumComponents(PixelFormat pixelFormat)
	{
		switch (pixelFormat)
		{
		//case(OPEN_GL_COMPRESSED_RGB_S3TC_DXT1_EXT) : return 3;
		//case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) : return 4;
		//case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) : return 4;
		//case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) : return 4;
		//case(OPEN_GL_COMPRESSED_SIGNED_RED_RGTC1_EXT) : return 1;
		//case(OPEN_GL_COMPRESSED_RED_RGTC1_EXT) : return 1;
		//case(OPEN_GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT) : return 2;
		//	/*case(OPEN_GL_COMPRESSED_RED_GREEN_RGTC2_EXT): return 2;
		//	case(OPEN_GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG): return 3;
		//	case(OPEN_GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG): return 3;
		//	case(OPEN_GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG): return 4;
		//	case(OPEN_GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG): return 4;
		//	case(OPEN_GL_ETC1_RGB8_OES): return 3;*/
		//case(OPEN_GL_COLOR_INDEX) : return 1;
		//case(OPEN_GL_STENCIL_INDEX) : return 1;
		case(OPEN_GL_DEPTH_COMPONENT) : return 1;
		case(OPEN_GL_DEPTH_COMPONENT16) : return 1;
		case(OPEN_GL_DEPTH_COMPONENT24) : return 1;
		case(OPEN_GL_DEPTH_COMPONENT32) : return 1;
		case(OPEN_GL_DEPTH_COMPONENT32F) : return 1;
		case(OPEN_GL_DEPTH_COMPONENT32F_NV) : return 1;
		case(OPEN_GL_RED) : return 1;
		case(OPEN_GL_GREEN) : return 1;
		case(OPEN_GL_BLUE) : return 1;
		case(OPEN_GL_ALPHA) : return 1;
		case(OPEN_GL_ALPHA8I_EXT) : return 1;
		case(OPEN_GL_ALPHA8UI_EXT) : return 1;
		case(OPEN_GL_ALPHA16I_EXT) : return 1;
		case(OPEN_GL_ALPHA16UI_EXT) : return 1;
		case(OPEN_GL_ALPHA32I_EXT) : return 1;
		case(OPEN_GL_ALPHA32UI_EXT) : return 1;
		case(OPEN_GL_ALPHA16F_ARB) : return 1;
		case(OPEN_GL_ALPHA32F_ARB) : return 1;
		case(OPEN_GL_RGB) : return 3;
		case(OPEN_GL_BGR) : return 3;
		case(OPEN_GL_RGB8I_EXT) : return 3;
		case(OPEN_GL_RGB8UI_EXT) : return 3;
		case(OPEN_GL_RGB16I_EXT) : return 3;
		case(OPEN_GL_RGB16UI_EXT) : return 3;
		case(OPEN_GL_RGB32I_EXT) : return 3;
		case(OPEN_GL_RGB32UI_EXT) : return 3;
		case(OPEN_GL_RGB16F_ARB) : return 3;
		case(OPEN_GL_RGB32F_ARB) : return 3;
		case(OPEN_GL_RGBA16F_ARB) : return 4;
		case(OPEN_GL_RGBA32F_ARB) : return 4;
		case(OPEN_GL_RGBA) : return 4;
		case(OPEN_GL_BGRA) : return 4;
		case(OPEN_GL_RGBA8) : return 4;
		case(OPEN_GL_LUMINANCE) : return 1;
		case(OPEN_GL_LUMINANCE4) : return 1;
		case(OPEN_GL_LUMINANCE8) : return 1;
		case(OPEN_GL_LUMINANCE12) : return 1;
		case(OPEN_GL_LUMINANCE16) : return 1;
		case(OPEN_GL_LUMINANCE8I_EXT) : return 1;
		case(OPEN_GL_LUMINANCE8UI_EXT) : return 1;
		case(OPEN_GL_LUMINANCE16I_EXT) : return 1;
		case(OPEN_GL_LUMINANCE16UI_EXT) : return 1;
		case(OPEN_GL_LUMINANCE32I_EXT) : return 1;
		case(OPEN_GL_LUMINANCE32UI_EXT) : return 1;
		case(OPEN_GL_LUMINANCE16F_ARB) : return 1;
		case(OPEN_GL_LUMINANCE32F_ARB) : return 1;
		case(OPEN_GL_LUMINANCE4_ALPHA4) : return 2;
		case(OPEN_GL_LUMINANCE6_ALPHA2) : return 2;
		case(OPEN_GL_LUMINANCE8_ALPHA8) : return 2;
		case(OPEN_GL_LUMINANCE12_ALPHA4) : return 2;
		case(OPEN_GL_LUMINANCE12_ALPHA12) : return 2;
		case(OPEN_GL_LUMINANCE16_ALPHA16) : return 2;
		case(OPEN_GL_INTENSITY) : return 1;
		case(OPEN_GL_INTENSITY4) : return 1;
		case(OPEN_GL_INTENSITY8) : return 1;
		case(OPEN_GL_INTENSITY12) : return 1;
		case(OPEN_GL_INTENSITY16) : return 1;
		case(OPEN_GL_INTENSITY8UI_EXT) : return 1;
		case(OPEN_GL_INTENSITY8I_EXT) : return 1;
		case(OPEN_GL_INTENSITY16I_EXT) : return 1;
		case(OPEN_GL_INTENSITY16UI_EXT) : return 1;
		case(OPEN_GL_INTENSITY32I_EXT) : return 1;
		case(OPEN_GL_INTENSITY32UI_EXT) : return 1;
		case(OPEN_GL_INTENSITY16F_ARB) : return 1;
		case(OPEN_GL_INTENSITY32F_ARB) : return 1;
		case(OPEN_GL_LUMINANCE_ALPHA) : return 2;
		case(OPEN_GL_LUMINANCE_ALPHA8I_EXT) : return 2;
		case(OPEN_GL_LUMINANCE_ALPHA8UI_EXT) : return 2;
		case(OPEN_GL_LUMINANCE_ALPHA16I_EXT) : return 2;
		case(OPEN_GL_LUMINANCE_ALPHA16UI_EXT) : return 2;
		case(OPEN_GL_LUMINANCE_ALPHA32I_EXT) : return 2;
		case(OPEN_GL_LUMINANCE_ALPHA32UI_EXT) : return 2;
		case(OPEN_GL_LUMINANCE_ALPHA16F_ARB) : return 2;
		case(OPEN_GL_LUMINANCE_ALPHA32F_ARB) : return 2;
		//case(OPEN_GL_HILO_NV) : return 2;
		//case(OPEN_GL_DSDT_NV) : return 2;
		//case(OPEN_GL_DSDT_MAG_NV) : return 3;
		//case(OPEN_GL_DSDT_MAG_VIB_NV) : return 4;
		case(OPEN_GL_RED_INTEGER_EXT) : return 1;
		case(OPEN_GL_GREEN_INTEGER_EXT) : return 1;
		case(OPEN_GL_BLUE_INTEGER_EXT) : return 1;
		case(OPEN_GL_ALPHA_INTEGER_EXT) : return 1;
		case(OPEN_GL_RGB_INTEGER_EXT) : return 3;
		case(OPEN_GL_RGBA_INTEGER_EXT) : return 4;
		case(OPEN_GL_BGR_INTEGER_EXT) : return 3;
		case(OPEN_GL_BGRA_INTEGER_EXT) : return 4;
		case(OPEN_GL_LUMINANCE_INTEGER_EXT) : return 1;
		case(OPEN_GL_LUMINANCE_ALPHA_INTEGER_EXT) : return 2;

		default:
		{
			return 0;
		}
		}
	}

	uint32 Texture::computePixelSizeInBits(PixelFormat format, uint32 type)
	{
		switch (format)
		{
		/*case(OPEN_GL_COMPRESSED_RGB_S3TC_DXT1_EXT) : return 4;
		case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) : return 4;
		case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) : return 8;
		case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) : return 8;
		case(OPEN_GL_COMPRESSED_SIGNED_RED_RGTC1_EXT) : return 4;
		case(OPEN_GL_COMPRESSED_RED_RGTC1_EXT) : return 4;
		case(OPEN_GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT) : return 8;
		case(OPEN_GL_COMPRESSED_RED_GREEN_RGTC2_EXT) : return 8;*/
			/*case(OPEN_GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG): return 4;
			case(OPEN_GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG): return 2;
			case(OPEN_GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG): return 4;
			case(OPEN_GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG): return 2;
			case(OPEN_GL_ETC1_RGB8_OES): return 4;*/
		default: break;
		}

		switch (format)
		{
		/*case(OPEN_GL_COMPRESSED_ALPHA) :
		case(OPEN_GL_COMPRESSED_LUMINANCE) :
		case(OPEN_GL_COMPRESSED_LUMINANCE_ALPHA) :
		case(OPEN_GL_COMPRESSED_INTENSITY) :
		case(OPEN_GL_COMPRESSED_RGB) :
		case(OPEN_GL_COMPRESSED_RGBA) :*/
			return 0;
		default: break;
		}

		switch (format)
		{
		case(OPEN_GL_LUMINANCE4) : return 4;
		case(OPEN_GL_LUMINANCE8) : return 8;
		case(OPEN_GL_LUMINANCE12) : return 12;
		case(OPEN_GL_LUMINANCE16) : return 16;
		case(OPEN_GL_LUMINANCE4_ALPHA4) : return 8;
		case(OPEN_GL_LUMINANCE6_ALPHA2) : return 8;
		case(OPEN_GL_LUMINANCE8_ALPHA8) : return 16;
		case(OPEN_GL_LUMINANCE12_ALPHA4) : return 16;
		case(OPEN_GL_LUMINANCE12_ALPHA12) : return 24;
		case(OPEN_GL_LUMINANCE16_ALPHA16) : return 32;
		case(OPEN_GL_INTENSITY4) : return 4;
		case(OPEN_GL_INTENSITY8) : return 8;
		case(OPEN_GL_INTENSITY12) : return 12;
		case(OPEN_GL_INTENSITY16) : return 16;
		default: break;
		}

		switch (type)
		{

		/*case(OPEN_GL_BITMAP) : return computeNumComponents(format);

		case(OPEN_GL_BYTE) :
		case(OPEN_GL_UNSIGNED_BYTE) : return 8 * computeNumComponents(format);

		case(OPEN_GL_HALF_FLOAT) :
		case(OPEN_GL_SHORT) :
		case(OPEN_GL_UNSIGNED_SHORT) : return 16 * computeNumComponents(format);

		case(OPEN_GL_INT) :
		case(OPEN_GL_UNSIGNED_INT) :
		case(OPEN_GL_FLOAT) : return 32 * computeNumComponents(format);


		case(OPEN_GL_UNSIGNED_BYTE_3_3_2) :
		case(OPEN_GL_UNSIGNED_BYTE_2_3_3_REV) : return 8;

		case(OPEN_GL_UNSIGNED_SHORT_5_6_5) :
		case(OPEN_GL_UNSIGNED_SHORT_5_6_5_REV) :
		case(OPEN_GL_UNSIGNED_SHORT_4_4_4_4) :
		case(OPEN_GL_UNSIGNED_SHORT_4_4_4_4_REV) :
		case(OPEN_GL_UNSIGNED_SHORT_5_5_5_1) :
		case(OPEN_GL_UNSIGNED_SHORT_1_5_5_5_REV) : return 16;

		case(OPEN_GL_UNSIGNED_INT_8_8_8_8) :
		case(OPEN_GL_UNSIGNED_INT_8_8_8_8_REV) :
		case(OPEN_GL_UNSIGNED_INT_10_10_10_2) :
		case(OPEN_GL_UNSIGNED_INT_2_10_10_10_REV) : return 32;*/
		default:
		{
			return 0;
		}
		}

	}

	uint32 Texture::computeBlockSize(PixelFormat pixelFormat, uint32 packing)
	{
		switch (pixelFormat)
		{
		//case(OPEN_GL_COMPRESSED_RGB_S3TC_DXT1_EXT) :
		//case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) :
		//	return max(8u, packing); // block size of 8
		//case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) :
		//case(OPEN_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) :
		//	//case(OPEN_GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG):
		//	//case(OPEN_GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG):
		//	//case(OPEN_GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG):
		//	//case(OPEN_GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG):
		//	//case(OPEN_GL_ETC1_RGB8_OES):
		//	return max(16u, packing); // block size of 16
		//case(OPEN_GL_COMPRESSED_SIGNED_RED_RGTC1_EXT) :
		//case(OPEN_GL_COMPRESSED_RED_RGTC1_EXT) :
		//	return max(8u, packing); // block size of 8
		//	break;
		//case(OPEN_GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT) :
		//case(OPEN_GL_COMPRESSED_RED_GREEN_RGTC2_EXT) :
		//	return max(16u, packing); // block size of 16
		default:
			break;
		}
		return packing;
	}

	uint32 Texture::computeRowWidthInBytes(int32 width, PixelFormat pixelFormat, uint32 type, int32 packing)
	{
		uint32 pixelSize = computePixelSizeInBits(pixelFormat, type);
		int32 widthInBits = width*pixelSize;
		int32 packingInBits = packing != 0 ? packing * 8 : 8;
		return (widthInBits / packingInBits + ((widthInBits%packingInBits) ? 1 : 0))*packing;
	}

	uint32 Texture::computeImageSizeInBytes(int32 width, int32 height, int32 depth, PixelFormat pixelFormat, uint32 type, int32 packing, int32 slice_packing, int32 image_packing)
	{
		if (width <= 0 || height <= 0 || depth <= 0) return 0;

		/*if (pixelFormat >= OPEN_GL_COMPRESSED_RGB_S3TC_DXT1_EXT &&
			pixelFormat <= OPEN_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
		{
			width = (width + 3) & ~3;
			height = (height + 3) & ~3;
		}

		if (pixelFormat >= OPEN_GL_COMPRESSED_RED_RGTC1_EXT &&
			pixelFormat <= OPEN_GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT)
		{
			width = (width + 3) & ~3;
			height = (height + 3) & ~3;
		}*/

		uint32 size = computeRowWidthInBytes(width, pixelFormat, type, packing);

		// now compute size of slice
		size *= height;
		size += slice_packing - 1;
		size -= size % slice_packing;

		// compute size of whole image
		size *= depth;
		size += image_packing - 1;
		size -= size % image_packing;

		return max(size, computeBlockSize(pixelFormat, packing));
	}

	int32 Texture::computeNearestPowerOfTwo(int32 s, float bias)
	{
		if ((s & (s - 1)) != 0)
		{
			float p2 = logf((float)s) / logf(2.0f);
			float rounded_p2 = floorf(p2 + bias);
			s = (int32)(powf(2.0f, rounded_p2));
		}
		return s;
	}

	int32 Texture::computeNumberOfMipmapLevels(int32 s, int32 t, int32 r)
	{
		int32 w = max(s, t);
		w = max(w, r);
		return 1 + static_cast<int32>(floor(logf(static_cast<float>(w)) / logf(2.0f)));
	}

}

namespace open
{
	TMemoryManager<Texture> g_textureMemoryManager;

	void* Texture::operator new (size_t size)
	{
		return g_textureMemoryManager.alloc(size);
	}

		void Texture::operator delete (void* p)
	{
		if (!p)
			return;

		g_textureMemoryManager.dealloc((Texture*)p);
	}

	Texture::Texture()
		:_width(0), _height(0), _depth(0), _textureID(0), _dataType(DATATYPE_UNSIGNED_BYTE),
		_pixeFormat(PIXEFORMAT_RGB), _internalFormat(PIXELINTERNALFORMAT_RGB)
	{
	}

	Texture::~Texture()
	{
	}

	void Texture::releaseGPUResource()
	{
	}

	void Texture::alloc(PixelInternalFormat internalFormat, int32 width, int32 height, int32 depth,
		PixelFormat pixeformat, DataType dataType, void* data, int32 packing)
	{
		_width = width;
		_height = height;
		_depth = depth;
		_dataType = dataType;
		_pixeFormat = pixeformat;
		_internalFormat = internalFormat;
		_packing = packing;

		if (_textureID == 0)
			gl_GenTextures(1, &_textureID);

		if (!_textureID)
			return;

		gl_BindTexture(OPEN_GL_TEXTURE_2D, _textureID);
		gl_PixelStorei(OPEN_GL_UNPACK_ALIGNMENT, packing);

		gl_TexImage2D(OPEN_GL_TEXTURE_2D, 0, _internalFormat, _width, _height, _depth, _pixeFormat, _dataType, data);
	}

	void Texture::subLoad(int32 startX, int32 startY, int32 width, int32 height, void* data)
	{
		if (!vaild())
			return;

		gl_BindTexture(OPEN_GL_TEXTURE_2D, _textureID);
		gl_PixelStorei(OPEN_GL_UNPACK_ALIGNMENT, _packing);
		gl_TexSubImage2D(OPEN_GL_TEXTURE_2D, 0, startX, startY, width, height, _pixeFormat, _dataType, data);
	}

	bool Texture::bind(uint32 unit)
	{
		if (!vaild())
			return false;

		gl_ActiveTexture(unit);
		gl_BindTexture(OPEN_GL_TEXTURE_2D, _textureID);
		return true;
	}

	bool Texture::apply() {

		if (!vaild())
			return false;

		gl_BindTexture(OPEN_GL_TEXTURE_2D, _textureID);
		return true;
	}

	Texture* Texture::createImage(const char* path)
	{
		const char* find = strstr(path, ".dds");
		if (find)
		{
			FILE* f;
			char* content = nullptr;
			int count = 0;

			f = fopen(path, "rt");

			if (f != nullptr)
			{
				fseek(f, 0, SEEK_END);
				count = ftell(f);
				rewind(f);

				if (count > 0)
				{
					content = new char[sizeof(char)* (count)];
					count = fread(content, sizeof(char), count, f);
				}
				fclose(f);
				//return (Texture*)ReadDDSFile(content, count);
			}
			else
				return nullptr;
		}

		static bool init = false;
		if (!init)
		{
			FreeImage_Initialise();
			init = true;
		}

		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(path);

		if ((fif == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(fif))
			return nullptr;

		FIBITMAP* bit = FreeImage_Load(fif, path);
		if (!bit)
			return nullptr;

		BITMAPINFO* info = FreeImage_GetInfo(bit);
		Texture* image = nullptr;

		if (info->bmiHeader.biBitCount == 8)///
		{
			bit = FreeImage_ConvertTo8Bits(bit);
			if (!bit)
				return image;

			int32 width = FreeImage_GetWidth(bit);
			int32 height = FreeImage_GetHeight(bit);

			BYTE *pixels = (BYTE*)FreeImage_GetBits(bit);
			unsigned char* buffer = new unsigned char[width * height];
			memcpy(buffer, pixels, width * height);

			FreeImage_Unload(bit);

			image = new Texture();
			image->alloc(PIXELINTERNALFORMAT_R8, width, height, 0, PIXEFORMAT_R, DATATYPE_UNSIGNED_BYTE, buffer);
			delete[] buffer;
		}
		else if (info->bmiHeader.biBitCount == 24)///
		{
			bit = FreeImage_ConvertTo24Bits(bit);
			if (!bit)
				return nullptr;

			int32 width = FreeImage_GetWidth(bit);
			int32 height = FreeImage_GetHeight(bit);

			BYTE *pixels = (BYTE*)FreeImage_GetBits(bit);
			unsigned char* buffer = new unsigned char[width * height * 3];
			memcpy(buffer, pixels, width * height * 3);

			FreeImage_Unload(bit);

			image = new Texture();
			image->alloc(PIXELINTERNALFORMAT_RGB, width, height, 0, PIXEFORMAT_BGR, DATATYPE_UNSIGNED_BYTE, buffer);
		}
		else if (info->bmiHeader.biBitCount == 32)///
		{
			bit = FreeImage_ConvertTo32Bits(bit);
			if (!bit)
				return nullptr;

			int32 width = FreeImage_GetWidth(bit);
			int32 height = FreeImage_GetHeight(bit);

			BYTE *pixels = (BYTE*)FreeImage_GetBits(bit);
			unsigned char* buffer = new unsigned char[width * height * 4];
			memcpy(buffer, pixels, width * height * 4);

			FreeImage_Unload(bit);

			image = new Texture();
			image->alloc(PIXELINTERNALFORMAT_RGBA, width, height, 0, PIXEFORMAT_BGRA, DATATYPE_UNSIGNED_BYTE, buffer);
		}
#ifdef _DEBUG
		image->_name = path;
#endif
		return image;
	}

	bool Texture::saveImage(Texture* image, const char* filePath)
	{
		/*unsigned char* mpixels = (unsigned char*)image->_d;
		FIBITMAP* bitmap = FreeImage_Allocate(image->_width,image->_height,32,8,8,8);
		for(uint32 y =0 ;y < FreeImage_GetHeight(bitmap);y++)
		{
		BYTE *bits = FreeImage_GetScanLine(bitmap,y);
		for(uint32 x =0 ;x<FreeImage_GetWidth(bitmap);x++)
		{
		bits[0] = mpixels[(y * image->_width + x) * 4 + 2];
		bits[1] = mpixels[(y * image->_width + x) * 4 + 1];
		bits[2] = mpixels[(y * image->_width + x) * 4 + 0];
		bits[3] = 255;
		bits += 4;
		}
		}
		BOOL success = FreeImage_Save(FIF_PNG, bitmap,filePath);
		FreeImage_Unload(bitmap);

		return (success != 0);*/
		return false;
	}
}