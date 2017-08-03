#include "ReaderDDS.h"
#include "Texture.h"
#include <RenderSystem/RenderEnumeration.h>
#include <RenderSystem/RenderFactoryInterface.h>

#include <glew/glew.h>

#if defined(_WIN32)
#include <windows.h>
#else  // Unix
#include <stdio.h>
#endif

#if defined(GLES1_AVAILABLE) || defined(GLES2_AVAILABLE)
#define GL_RED                  0x1903
#define GL_LUMINANCE4_ALPHA4    0x8043
#endif

#if defined(GL3_AVAILABLE)
#define GL_LUMINANCE4_ALPHA4    0x8043
#endif

//
// DDSURFACEDESC2 flags that mark the validity of the struct data
//
#define DDSD_CAPS               0x00000001l     // default
#define DDSD_HEIGHT             0x00000002l        // default
#define DDSD_WIDTH              0x00000004l        // default
#define DDSD_PIXELFORMAT        0x00001000l        // default
#define DDSD_PITCH              0x00000008l     // For uncompressed formats
#define DDSD_MIPMAPCOUNT        0x00020000l
#define DDSD_LINEARSIZE         0x00080000l     // For compressed formats
#define DDSD_DEPTH              0x00800000l        // Volume Textures

//
// DDPIXELFORMAT flags
//
#define DDPF_ALPHAPIXELS        0x00000001l
#define DDPF_FOURCC             0x00000004l        // Compressed formats
#define DDPF_RGB                0x00000040l        // Uncompressed formats
#define DDPF_ALPHA              0x00000002l
#define DDPF_COMPRESSED         0x00000080l
#define DDPF_LUMINANCE          0x00020000l
#define DDPF_BUMPLUMINANCE      0x00040000l        // L,U,V
#define DDPF_BUMPDUDV           0x00080000l        // U,V

//
// DDSCAPS flags
//
#define DDSCAPS_TEXTURE         0x00001000l     // default
#define DDSCAPS_COMPLEX         0x00000008l
#define DDSCAPS_MIPMAP          0x00400000l
#define DDSCAPS2_VOLUME         0x00200000l


#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((UI32)(char)(ch0) | ((UI32)(char)(ch1) << 8) | \
	((UI32)(char)(ch2) << 16) | ((UI32)(char)(ch3) << 24))
#endif //defined(MAKEFOURCC)

/*
* FOURCC codes for DX compressed-texture pixel formats
*/
#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT2  (MAKEFOURCC('D','X','T','2'))
#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT4  (MAKEFOURCC('D','X','T','4'))
#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

/*
* FOURCC codes for 3dc compressed-texture pixel formats
*/
#define FOURCC_ATI1  (MAKEFOURCC('A','T','I','1'))
#define FOURCC_ATI2  (MAKEFOURCC('A','T','I','2'))

/*
* FOURCC codes for DX10 files
*/
#define FOURCC_DX10  (MAKEFOURCC('D','X','1','0'))

namespace open
{
	typedef uint32 UI32;
	typedef int32 I32;

	struct  DDCOLORKEY
	{
		DDCOLORKEY() :dwColorSpaceLowValue(0), dwColorSpaceHighValue(0) {}

		UI32    dwColorSpaceLowValue;
		UI32    dwColorSpaceHighValue;
	};

	struct DDPIXELFORMAT
	{
		DDPIXELFORMAT() :dwSize(0),
		dwFlags(0),
		dwFourCC(0),
		dwRGBBitCount(0),
		dwRBitMask(0),
		dwGBitMask(0),
		dwBBitMask(0),
		dwRGBAlphaBitMask(0) {}


		UI32    dwSize;
		UI32    dwFlags;
		UI32    dwFourCC;
		union
		{
			UI32    dwRGBBitCount;
			UI32    dwYUVBitCount;
			UI32    dwZBufferBitDepth;
			UI32    dwAlphaBitDepth;
			UI32    dwLuminanceBitDepth;
		};
		union
		{
			UI32    dwRBitMask;
			UI32    dwYBitMask;
		};
		union
		{
			UI32    dwGBitMask;
			UI32    dwUBitMask;
		};
		union
		{
			UI32    dwBBitMask;
			UI32    dwVBitMask;
		};
		union
		{
			UI32    dwRGBAlphaBitMask;
			UI32    dwYUVAlphaBitMask;
			UI32    dwRGBZBitMask;
			UI32    dwYUVZBitMask;
		};
	};

	struct  DDSCAPS2
	{
		DDSCAPS2() :dwCaps(0), dwCaps2(0), dwCaps3(0), dwCaps4(0) {}

		UI32       dwCaps;
		UI32       dwCaps2;
		UI32       dwCaps3;
		union
		{
			UI32       dwCaps4;
			UI32       dwVolumeDepth;
		};
	};

	struct DDSURFACEDESC2
	{
		DDSURFACEDESC2() :dwSize(0), dwFlags(0), dwHeight(0), dwWidth(0),
		lPitch(0),
		dwBackBufferCount(0),
		dwMipMapCount(0),
		dwAlphaBitDepth(0),
		dwReserved(0),
		lpSurface(0),
		dwTextureStage(0) {}


		UI32         dwSize;
		UI32         dwFlags;
		UI32         dwHeight;
		UI32         dwWidth;
		union
		{
			I32              lPitch;
			UI32     dwLinearSize;
		};
		union
		{
			UI32      dwBackBufferCount;
			UI32      dwDepth;
		};
		union
		{
			UI32     dwMipMapCount;
			UI32     dwRefreshRate;
		};
		UI32         dwAlphaBitDepth;
		UI32         dwReserved;
		UI32        lpSurface;
		DDCOLORKEY    ddckCKDestOverlay;
		DDCOLORKEY    ddckCKDestBlt;
		DDCOLORKEY    ddckCKSrcOverlay;
		DDCOLORKEY    ddckCKSrcBlt;
		DDPIXELFORMAT ddpfPixelFormat;
		DDSCAPS2      ddsCaps;
		UI32 dwTextureStage;
	};

	//
	// Structure of a DXT-1 compressed texture block
	// see page "Opaque and 1-Bit Alpha Textures (Direct3D 9)" on http://msdn.microsoft.com
	// url at time of writing http://msdn.microsoft.com/en-us/library/bb147243(v=VS.85).aspx
	//
	struct DXT1TexelsBlock
	{
		unsigned short color_0;     // colors at their
		unsigned short color_1;     // extreme
		uint32   texels4x4;   // interpolated colors (2 bits per texel)
	};

	typedef enum DXGI_FORMAT {
		DXGI_FORMAT_UNKNOWN = 0,
		DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
		DXGI_FORMAT_R32G32B32A32_UINT = 3,
		DXGI_FORMAT_R32G32B32A32_SINT = 4,
		DXGI_FORMAT_R32G32B32_TYPELESS = 5,
		DXGI_FORMAT_R32G32B32_FLOAT = 6,
		DXGI_FORMAT_R32G32B32_UINT = 7,
		DXGI_FORMAT_R32G32B32_SINT = 8,
		DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
		DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
		DXGI_FORMAT_R16G16B16A16_UNORM = 11,
		DXGI_FORMAT_R16G16B16A16_UINT = 12,
		DXGI_FORMAT_R16G16B16A16_SNORM = 13,
		DXGI_FORMAT_R16G16B16A16_SINT = 14,
		DXGI_FORMAT_R32G32_TYPELESS = 15,
		DXGI_FORMAT_R32G32_FLOAT = 16,
		DXGI_FORMAT_R32G32_UINT = 17,
		DXGI_FORMAT_R32G32_SINT = 18,
		DXGI_FORMAT_R32G8X24_TYPELESS = 19,
		DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
		DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
		DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
		DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
		DXGI_FORMAT_R10G10B10A2_UNORM = 24,
		DXGI_FORMAT_R10G10B10A2_UINT = 25,
		DXGI_FORMAT_R11G11B10_FLOAT = 26,
		DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
		DXGI_FORMAT_R8G8B8A8_UNORM = 28,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
		DXGI_FORMAT_R8G8B8A8_UINT = 30,
		DXGI_FORMAT_R8G8B8A8_SNORM = 31,
		DXGI_FORMAT_R8G8B8A8_SINT = 32,
		DXGI_FORMAT_R16G16_TYPELESS = 33,
		DXGI_FORMAT_R16G16_FLOAT = 34,
		DXGI_FORMAT_R16G16_UNORM = 35,
		DXGI_FORMAT_R16G16_UINT = 36,
		DXGI_FORMAT_R16G16_SNORM = 37,
		DXGI_FORMAT_R16G16_SINT = 38,
		DXGI_FORMAT_R32_TYPELESS = 39,
		DXGI_FORMAT_D32_FLOAT = 40,
		DXGI_FORMAT_R32_FLOAT = 41,
		DXGI_FORMAT_R32_UINT = 42,
		DXGI_FORMAT_R32_SINT = 43,
		DXGI_FORMAT_R24G8_TYPELESS = 44,
		DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
		DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
		DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
		DXGI_FORMAT_R8G8_TYPELESS = 48,
		DXGI_FORMAT_R8G8_UNORM = 49,
		DXGI_FORMAT_R8G8_UINT = 50,
		DXGI_FORMAT_R8G8_SNORM = 51,
		DXGI_FORMAT_R8G8_SINT = 52,
		DXGI_FORMAT_R16_TYPELESS = 53,
		DXGI_FORMAT_R16_FLOAT = 54,
		DXGI_FORMAT_D16_UNORM = 55,
		DXGI_FORMAT_R16_UNORM = 56,
		DXGI_FORMAT_R16_UINT = 57,
		DXGI_FORMAT_R16_SNORM = 58,
		DXGI_FORMAT_R16_SINT = 59,
		DXGI_FORMAT_R8_TYPELESS = 60,
		DXGI_FORMAT_R8_UNORM = 61,
		DXGI_FORMAT_R8_UINT = 62,
		DXGI_FORMAT_R8_SNORM = 63,
		DXGI_FORMAT_R8_SINT = 64,
		DXGI_FORMAT_A8_UNORM = 65,
		DXGI_FORMAT_R1_UNORM = 66,
		DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
		DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
		DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
		DXGI_FORMAT_BC1_TYPELESS = 70,
		DXGI_FORMAT_BC1_UNORM = 71,
		DXGI_FORMAT_BC1_UNORM_SRGB = 72,
		DXGI_FORMAT_BC2_TYPELESS = 73,
		DXGI_FORMAT_BC2_UNORM = 74,
		DXGI_FORMAT_BC2_UNORM_SRGB = 75,
		DXGI_FORMAT_BC3_TYPELESS = 76,
		DXGI_FORMAT_BC3_UNORM = 77,
		DXGI_FORMAT_BC3_UNORM_SRGB = 78,
		DXGI_FORMAT_BC4_TYPELESS = 79,
		DXGI_FORMAT_BC4_UNORM = 80,
		DXGI_FORMAT_BC4_SNORM = 81,
		DXGI_FORMAT_BC5_TYPELESS = 82,
		DXGI_FORMAT_BC5_UNORM = 83,
		DXGI_FORMAT_BC5_SNORM = 84,
		DXGI_FORMAT_B5G6R5_UNORM = 85,
		DXGI_FORMAT_B5G5R5A1_UNORM = 86,
		DXGI_FORMAT_B8G8R8A8_UNORM = 87,
		DXGI_FORMAT_B8G8R8X8_UNORM = 88,
		DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
		DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
		DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
		DXGI_FORMAT_BC6H_TYPELESS = 94,
		DXGI_FORMAT_BC6H_UF16 = 95,
		DXGI_FORMAT_BC6H_SF16 = 96,
		DXGI_FORMAT_BC7_TYPELESS = 97,
		DXGI_FORMAT_BC7_UNORM = 98,
		DXGI_FORMAT_BC7_UNORM_SRGB = 99,
		DXGI_FORMAT_AYUV = 100,
		DXGI_FORMAT_Y410 = 101,
		DXGI_FORMAT_Y416 = 102,
		DXGI_FORMAT_NV12 = 103,
		DXGI_FORMAT_P010 = 104,
		DXGI_FORMAT_P016 = 105,
		DXGI_FORMAT_420_OPAQUE = 106,
		DXGI_FORMAT_YUY2 = 107,
		DXGI_FORMAT_Y210 = 108,
		DXGI_FORMAT_Y216 = 109,
		DXGI_FORMAT_NV11 = 110,
		DXGI_FORMAT_AI44 = 111,
		DXGI_FORMAT_IA44 = 112,
		DXGI_FORMAT_P8 = 113,
		DXGI_FORMAT_A8P8 = 114,
		DXGI_FORMAT_B4G4R4A4_UNORM = 115,
		DXGI_FORMAT_FORCE_UINT = 0xffffffffUL
	} DXGI_FORMAT;

	typedef enum D3D10_RESOURCE_DIMENSION
	{
		D3D10_RESOURCE_DIMENSION_UNKNOWN = 0,
		D3D10_RESOURCE_DIMENSION_BUFFER = 1,
		D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
		D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
		D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4
	} D3D10_RESOURCE_DIMENSION;

	typedef struct
	{
		DXGI_FORMAT              dxgiFormat;
		D3D10_RESOURCE_DIMENSION resourceDimension;
		UI32                     miscFlag;
		UI32                     arraySize;
		UI32                     reserved;
	} DDS_HEADER_DXT10;

	static uint32 computeImageSizeInBytes(int32 width, int32 height, int32 depth,
		uint32 pixelFormat, uint32 pixelType,
		int32 packing = 1, int32 slice_packing = 1, int32 image_packing = 1)
	{
		if (width < 1)
			width = 1;
		if (height < 1)
			height = 1;
		if (depth < 1)
			depth = 1;

		return ITexture::computeImageSizeInBytes(width, height, depth, (PixelFormat)pixelFormat, pixelType, packing, slice_packing, image_packing);
	}

	void readStream(char** ddsBuffer, int32 size, void* dest)
	{
		memcpy(dest, *ddsBuffer, size);
		*ddsBuffer += size;
	}

	ITexture* ReadDDSFile(char* ddsBuffer, int32 bufferSize)
	{
		DDSURFACEDESC2 ddsd;

		char filecode[4];
		readStream(&ddsBuffer, 4, filecode);

		if (strncmp(filecode, "DDS ", 4) != 0)
		{
			return NULL;
		}

		readStream(&ddsBuffer, sizeof(ddsd), (char*)(&ddsd));
		ITexture* texture = RenderFactory::s_factory->createTexture();

		//Check valid structure sizes
		if (ddsd.dwSize != 124 && ddsd.ddpfPixelFormat.dwSize != 32)
		{
			return NULL;
		}

		int32 depth = 1;

		// Check for volume image
		if (ddsd.dwDepth > 0 && (ddsd.dwFlags & DDSD_DEPTH))
		{
			depth = ddsd.dwDepth;
		}

		// Retreive image properties.
		int32 s = ddsd.dwWidth;
		int32 t = ddsd.dwHeight;
		int32 r = depth;
		uint32 dataType = DATATYPE_UNSIGNED_BYTE;
		uint32 pixelFormat = 0;
		uint32 internalFormat = 0;

		// Handle some esoteric formats
		if (ddsd.ddpfPixelFormat.dwFlags & DDPF_BUMPDUDV)
		{
			printf("ReadDDSFile warning: DDPF_BUMPDUDV format is not supported");
			return NULL;
		}
		if (ddsd.ddpfPixelFormat.dwFlags & DDPF_BUMPLUMINANCE)
		{
			printf("ReadDDSFile warning: DDPF_BUMPLUMINANCE format is not supported");
		}

		// Uncompressed formats will usually use DDPF_RGB to indicate an RGB format,
		// while compressed formats will use DDPF_FOURCC with a four-character code.
		bool usingAlpha = ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS;
		int32 packing(1);
		bool isDXTC(false);

		// Uncompressed formats.
		if (ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB)
		{
			struct RGBFormat
			{
				const char*  name;
				UI32         bitCount;
				UI32         rBitMask;
				UI32         gBitMask;
				UI32         bBitMask;
				UI32         aBitMask;
				uint32 internalFormat;
				uint32 pixelFormat;
				uint32 dataType;
			};

			const uint32 UNSUPPORTED = 0;

			static const RGBFormat rgbFormats[] =
			{
				{ "R3G3B2", 8, 0xe0, 0x1c, 0x03, 0x00,
				PIXEFORMAT_RGB, PIXEFORMAT_RGB, GL_UNSIGNED_BYTE_3_3_2 },

				{ "R5G6B5", 16, 0xf800, 0x07e0, 0x001f, 0x0000,
				PIXEFORMAT_RGB, PIXEFORMAT_RGB, GL_UNSIGNED_SHORT_5_6_5 },
				{ "A1R5G5B5", 16, 0x7c00, 0x03e0, 0x001f, 0x8000,
				PIXEFORMAT_RGBA, PIXEFORMAT_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV },
				{ "X1R5G5B5", 16, 0x7c00, 0x03e0, 0x001f, 0x0000,
				PIXEFORMAT_RGB, PIXEFORMAT_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV },
				{ "A4R4G4B4", 16, 0x0f00, 0x00f0, 0x000f, 0xf000,
				PIXEFORMAT_RGBA, PIXEFORMAT_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV },
				{ "X4R4G4B4", 16, 0x0f00, 0x00f0, 0x000f, 0x0000,
				PIXEFORMAT_RGB, PIXEFORMAT_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV },
				{ "A8R3G3B2", 16, 0x00e0, 0x001c, 0x0003, 0xff00,
				PIXEFORMAT_RGBA, PIXEFORMAT_BGRA, UNSUPPORTED },

				{ "R8G8B8", 24, 0xff0000, 0x00ff00, 0x0000ff, 0x000000,
				PIXEFORMAT_RGB, PIXEFORMAT_BGR, GL_UNSIGNED_BYTE },

				{ "B8G8R8", 24, 0x0000ff, 0x00ff00, 0xff0000, 0x000000,
				PIXEFORMAT_RGB, PIXEFORMAT_RGB, GL_UNSIGNED_BYTE },

				{ "A8R8G8B8", 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000,
				PIXEFORMAT_RGBA, PIXEFORMAT_BGRA, GL_UNSIGNED_BYTE },
				{ "X8R8G8B8", 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000,
				PIXEFORMAT_RGB, PIXEFORMAT_BGRA, GL_UNSIGNED_BYTE },
				{ "A8B8G8R8", 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000,
				PIXEFORMAT_RGBA, PIXEFORMAT_RGBA, GL_UNSIGNED_BYTE },
				{ "X8B8G8R8", 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000,
				PIXEFORMAT_RGB, PIXEFORMAT_RGBA, GL_UNSIGNED_BYTE },
				{ "A2R10G10B10", 32, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000,
				PIXEFORMAT_RGBA, PIXEFORMAT_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV },
				{ "A2B10G10R10", 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000,
				PIXEFORMAT_RGBA, PIXEFORMAT_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV },
				{ "G16R16", 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000,
				PIXEFORMAT_RGB, UNSUPPORTED, GL_UNSIGNED_SHORT },
			};

			bool found = false;

			for (uint32 i = 0; i < sizeof (rgbFormats) / sizeof (RGBFormat); i++)
			{
				const RGBFormat& f = rgbFormats[i];
				if (ddsd.ddpfPixelFormat.dwRGBBitCount == f.bitCount &&
					ddsd.ddpfPixelFormat.dwRBitMask == f.rBitMask &&
					ddsd.ddpfPixelFormat.dwGBitMask == f.gBitMask &&
					ddsd.ddpfPixelFormat.dwBBitMask == f.bBitMask &&
					ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == f.aBitMask)
				{
					if (f.internalFormat != UNSUPPORTED &&
						f.pixelFormat != UNSUPPORTED &&
						f.dataType != UNSUPPORTED)
					{
						internalFormat = f.internalFormat;
						pixelFormat = f.pixelFormat;
						dataType = f.dataType;
						found = true;
						break;
					}
					else
					{
						return NULL;
					}
				}
			}

			if (!found)
			{
				return NULL;
			}
		}
		else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_LUMINANCE)
		{
			internalFormat = usingAlpha ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			pixelFormat = usingAlpha ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			if (usingAlpha && ddsd.ddpfPixelFormat.dwLuminanceBitDepth == 8)
			{
				pixelFormat = GL_LUMINANCE4_ALPHA4;
			}
			else if (usingAlpha && ddsd.ddpfPixelFormat.dwLuminanceBitDepth == 32)
			{
				dataType = GL_UNSIGNED_SHORT;
			}
			else if (!usingAlpha && ddsd.ddpfPixelFormat.dwLuminanceBitDepth == 16)
			{
				dataType = GL_UNSIGNED_SHORT;
			}
			else if (usingAlpha)
			{
			}
			else
			{
			}
		}
		else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHA)
		{
			internalFormat = GL_ALPHA;
			pixelFormat = GL_ALPHA;
		}
		// Compressed formats
		else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC)
		{
			switch (ddsd.ddpfPixelFormat.dwFourCC)
			{
			case FOURCC_DXT1:
				if (usingAlpha)
				{
					internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
					pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				}
				else
				{
					internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
					pixelFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
				}
				packing = 2;        // 4 bits/pixel. 4 px = 2 bytes
				isDXTC = true;
				break;
			case FOURCC_DXT3:
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				packing = 4;        // 8 bits/pixel. 4 px = 4 bytes
				isDXTC = true;
				break;
			case FOURCC_DXT5:
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				packing = 4;        // 8 bits/pixel. 4 px = 4 bytes
				isDXTC = true;
				break;
			case FOURCC_ATI1:
				internalFormat = GL_COMPRESSED_RED_RGTC1_EXT;
				pixelFormat = GL_COMPRESSED_RED_RGTC1_EXT;
				break;
			case FOURCC_ATI2:
				internalFormat = GL_COMPRESSED_RED_GREEN_RGTC2_EXT;
				pixelFormat = GL_COMPRESSED_RED_GREEN_RGTC2_EXT;
				break;
			case 0x00000024: // A16B16G16R16
				internalFormat = GL_RGBA;
				pixelFormat = GL_RGBA;
				dataType = GL_UNSIGNED_SHORT;
				break;
			case 0x00000071: // A16B16G16R16F
				internalFormat = GL_RGBA; // why no transparency?
				pixelFormat = GL_RGBA;
				dataType = GL_HALF_FLOAT;
				break;
			case 0x0000006E: // Q16W16V16U16
				internalFormat = GL_RGBA;
				pixelFormat = GL_RGBA;
				dataType = GL_UNSIGNED_SHORT;
				break;
			case 0x00000070: // G16R16F
				return NULL;
				//             internalFormat = GL_RGB;
				//             pixelFormat    = must be GL_RED and GL_GREEN
				//             dataType       = GL_HALF_FLOAT;
				break;
			case 0x00000073: // G32R32F
				return NULL;
				//             internalFormat = GL_RGB;
				//             pixelFormat    = must be GL_RED and GL_GREEN
				//             dataType       = GL_FLOAT;
				break;
			case 0x00000072: // R32F
				internalFormat = GL_RGB;
				pixelFormat = GL_RED;
				dataType = GL_FLOAT;
				break;
			case 0x0000006F: // R16F
				internalFormat = GL_RGB;
				pixelFormat = GL_RED;
				dataType = GL_HALF_FLOAT;
				break;
			case 0x00000074: // A32B32G32R32F
				internalFormat = GL_RGBA;
				pixelFormat = GL_RGBA;
				dataType = GL_FLOAT;
				break;
			case 0x00000075: // CxV8U8
				return NULL;

			case FOURCC_DX10:
			{
								DDS_HEADER_DXT10 header10;
								readStream(&ddsBuffer, sizeof(header10), (char*)(&header10));
								switch (header10.dxgiFormat)
								{
								case DXGI_FORMAT_R32G32B32A32_FLOAT:
									internalFormat = GL_RGBA32F_ARB;
									pixelFormat = GL_RGBA;
									dataType = GL_FLOAT;
									break;

								case DXGI_FORMAT_R32G32B32A32_UINT:
									internalFormat = GL_RGBA32UI_EXT;
									pixelFormat = GL_RGBA;
									dataType = GL_UNSIGNED_INT;
									break;

								case DXGI_FORMAT_R32G32B32A32_SINT:
									internalFormat = GL_RGBA32I_EXT;
									pixelFormat = GL_RGBA;
									dataType = GL_INT;
									break;

								case DXGI_FORMAT_R32G32B32_FLOAT:
									internalFormat = GL_RGB32F_ARB;
									pixelFormat = GL_RGB;
									dataType = GL_FLOAT;
									break;

								case DXGI_FORMAT_R32G32B32_UINT:
									internalFormat = GL_RGB32UI_EXT;
									pixelFormat = GL_RGB;
									dataType = GL_UNSIGNED_INT;
									break;

								case DXGI_FORMAT_R32G32B32_SINT:
									internalFormat = GL_RGB32I_EXT;
									pixelFormat = GL_RGB;
									dataType = GL_INT;
									break;

								case DXGI_FORMAT_R16G16B16A16_FLOAT:
									internalFormat = GL_RGBA16F_ARB;
									pixelFormat = GL_RGBA;
									dataType = GL_HALF_FLOAT;
									break;

								case DXGI_FORMAT_R16G16B16A16_UNORM:
									internalFormat = GL_RGBA16;
									pixelFormat = GL_RGBA;
									dataType = GL_UNSIGNED_SHORT;
									break;

								case DXGI_FORMAT_R16G16B16A16_UINT:
									internalFormat = GL_RGBA16UI_EXT;
									pixelFormat = GL_RGBA;
									dataType = GL_UNSIGNED_SHORT;
									break;

								case DXGI_FORMAT_R16G16B16A16_SNORM:
									internalFormat = GL_RGBA16_SNORM;
									pixelFormat = GL_RGBA;
									dataType = GL_SHORT;
									break;

								case DXGI_FORMAT_R16G16B16A16_SINT:
									internalFormat = GL_RGBA16I_EXT;
									pixelFormat = GL_RGBA;
									dataType = GL_SHORT;
									break;

								case DXGI_FORMAT_R32G32_FLOAT:
									internalFormat = GL_RG32F;
									pixelFormat = GL_RG;
									dataType = GL_FLOAT;
									break;

								case DXGI_FORMAT_R32G32_UINT:
									internalFormat = GL_RG32UI;
									pixelFormat = GL_RG;
									dataType = GL_UNSIGNED_INT;
									break;

								case DXGI_FORMAT_R32G32_SINT:
									internalFormat = GL_RG32I;
									pixelFormat = GL_RG;
									dataType = GL_INT;
									break;

								case DXGI_FORMAT_R16G16_FLOAT:
									internalFormat = GL_RG16F;
									pixelFormat = GL_RG;
									dataType = GL_HALF_FLOAT;
									break;

								case DXGI_FORMAT_R16G16_UNORM:
									internalFormat = GL_RG16;
									pixelFormat = GL_RG;
									dataType = GL_UNSIGNED_SHORT;
									break;

								case DXGI_FORMAT_R16G16_UINT:
									internalFormat = GL_RG16UI;
									pixelFormat = GL_RG;
									dataType = GL_UNSIGNED_SHORT;
									break;

								case DXGI_FORMAT_R16G16_SNORM:
									internalFormat = GL_RG16_SNORM;
									pixelFormat = GL_RG;
									dataType = GL_UNSIGNED_SHORT;
									break;

								case DXGI_FORMAT_R16G16_SINT:
									internalFormat = GL_RG16I;
									pixelFormat = GL_RG;
									dataType = GL_SHORT;
									break;

								case DXGI_FORMAT_R32_FLOAT:
									internalFormat = GL_R32F;
									pixelFormat = GL_RED;
									dataType = GL_FLOAT;
									break;

								case DXGI_FORMAT_R32_UINT:
									internalFormat = GL_R32UI;
									pixelFormat = GL_RED;
									dataType = GL_UNSIGNED_INT;
									break;

								case DXGI_FORMAT_R32_SINT:
									internalFormat = GL_R32I;
									pixelFormat = GL_RED;
									dataType = GL_INT;
									break;

								case DXGI_FORMAT_R8G8_UNORM:
									internalFormat = GL_RG;
									pixelFormat = GL_RG;
									dataType = GL_UNSIGNED_BYTE;
									break;

								case DXGI_FORMAT_R8G8_UINT:
									internalFormat = GL_RG8UI;
									pixelFormat = GL_RG;
									dataType = GL_UNSIGNED_BYTE;
									break;

								case DXGI_FORMAT_R8G8_SNORM:
									internalFormat = GL_RG_SNORM;
									pixelFormat = GL_RG;
									dataType = GL_BYTE;
									break;

								case DXGI_FORMAT_R8G8_SINT:
									internalFormat = GL_RG8I;
									pixelFormat = GL_RG;
									dataType = GL_BYTE;
									break;

								case DXGI_FORMAT_R16_FLOAT:
									internalFormat = GL_R16F;
									pixelFormat = GL_RED;
									dataType = GL_HALF_FLOAT;
									break;

								case DXGI_FORMAT_R16_UNORM:
									internalFormat = GL_RED;
									pixelFormat = GL_RED;
									dataType = GL_HALF_FLOAT;
									break;

								case DXGI_FORMAT_R16_UINT:
									internalFormat = GL_R16UI;
									pixelFormat = GL_RED;
									dataType = GL_UNSIGNED_SHORT;
									break;

								case DXGI_FORMAT_R16_SNORM:
									internalFormat = GL_RED_SNORM;
									pixelFormat = GL_RED;
									dataType = GL_SHORT;
									break;

								case DXGI_FORMAT_R16_SINT:
									internalFormat = GL_R16I;
									pixelFormat = GL_RED;
									dataType = GL_SHORT;
									break;

								default:
									return NULL;
								}
			}
				break;

			case MAKEFOURCC('U', 'Y', 'V', 'Y'): // not supported in OSG
			case MAKEFOURCC('U', 'Y', 'V', '2'): // not supported in OSG
			case MAKEFOURCC('R', 'G', 'B', 'G'): // R8G8_B8G8 -- what is it?
			case MAKEFOURCC('G', 'R', 'G', 'B'): // G8R8_G8B8 -- what is it?
				//break;

			default:
				return NULL;
			}
		}
		else
		{
			return NULL;
		}

		unsigned int size = computeImageSizeInBytes(s, t, r, pixelFormat, dataType, packing);

		// Take care of mipmaps if any.
		unsigned int sizeWithMipmaps = size;
		uint32* mipmap_offsets = NULL;
		if (ddsd.dwMipMapCount > 1)
		{
			unsigned numMipmaps = ITexture::computeNumberOfMipmapLevels(s, t, r);
			if (numMipmaps > ddsd.dwMipMapCount)
				numMipmaps = ddsd.dwMipMapCount;

			mipmap_offsets = new uint32[numMipmaps - 1];

			int width = s;
			int height = t;
			int depth = r;

			for (unsigned int k = 0; k < numMipmaps - 1; ++k)
			{
				mipmap_offsets[k] = sizeWithMipmaps;

				width = max(width >> 1, 1);
				height = min(height >> 1, 1);
				depth = max(depth >> 1, 1);

				sizeWithMipmaps += ITexture::computeImageSizeInBytes(width, height, depth, (PixelFormat)pixelFormat, dataType, packing);
			}
		}

		unsigned char* imageData = new unsigned char[sizeWithMipmaps];

		readStream(&ddsBuffer, size, imageData);
		// If loading mipmaps in second chunk fails we may still use main image
		if (size < sizeWithMipmaps)
		{
			readStream(&ddsBuffer, sizeWithMipmaps - size, (char*)imageData + size);
		}

		texture->alloc((PixelInternalFormat)internalFormat, s, t, r, (PixelFormat)pixelFormat, (DataType)dataType, imageData, packing);

		/*if (mipmap_offsets)
			texture->setMipmapLevels(mipmap_offsets);*/

		return texture;
	}
}