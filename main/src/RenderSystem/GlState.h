#ifndef RENDERSYSTEM_GL_STATE_H
#define RENDERSYSTEM_GL_STATE_H 1

#include <RenderSystem/RenderStatesInterface.h>
#include <open/interface/RenderSystem/RenderEnumeration.h>

#define OPEN_GL_ARRAY_BUFFER 0x8892
#define OPEN_GL_ELEMENT_ARRAY_BUFFER 0x8893
#define OPEN_GL_ARRAY_BUFFER_BINDING 0x8894
#define OPEN_GL_ARRAY_BUFFER_BINDING 0x8894

#define OPEN_GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define OPEN_GL_PIXEL_PACK_BUFFER 0x88EB
#define OPEN_GL_PIXEL_UNPACK_BUFFER 0x88EC
#define OPEN_GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define OPEN_GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF

#define OPEN_GL_READ_ONLY 0x88B8
#define OPEN_GL_WRITE_ONLY 0x88B9
#define OPEN_GL_READ_WRITE 0x88BA
#define OPEN_GL_BUFFER_ACCESS 0x88BB
#define OPEN_GL_BUFFER_MAPPED 0x88BC
#define OPEN_GL_BUFFER_MAP_POINTER 0x88BD
#define OPEN_GL_STREAM_DRAW 0x88E0
#define OPEN_GL_STREAM_READ 0x88E1
#define OPEN_GL_STREAM_COPY 0x88E2
#define OPEN_GL_STATIC_DRAW 0x88E4
#define OPEN_GL_STATIC_READ 0x88E5
#define OPEN_GL_STATIC_COPY 0x88E6
#define OPEN_GL_DYNAMIC_DRAW 0x88E8
#define OPEN_GL_DYNAMIC_READ 0x88E9
#define OPEN_GL_DYNAMIC_COPY 0x88EA
#define OPEN_GL_SAMPLES_PASSED 0x8914

#define OPEN_GL_MAP_READ_BIT 0x0001
#define OPEN_GL_MAP_WRITE_BIT 0x0002
#define OPEN_GL_MAP_PERSISTENT_BIT 0x00000040
#define OPEN_GL_MAP_COHERENT_BIT 0x00000080

#define OPEN_GL_TRUE 1
#define OPEN_GL_ZERO 0
#define OPEN_GL_FALSE 0
#define OPEN_GL_LOGIC_OP 0x0BF1
#define OPEN_GL_NONE 0

#define OPEN_GL_FRAMEBUFFER 0x8D40
#define OPEN_GL_RENDERBUFFER 0x8D41

#define OPEN_GL_READ_FRAMEBUFFER 0x8CA8
#define OPEN_GL_DRAW_FRAMEBUFFER 0x8CA9
#define OPEN_GL_FRAMEBUFFER_COMPLETE 0x8CD5

#define OPEN_GL_TEXTURE_1D 0x0DE0
#define OPEN_GL_TEXTURE_2D 0x0DE1


#define OPEN_GL_BYTE 0x1400
#define OPEN_GL_UNSIGNED_BYTE 0x1401
#define OPEN_GL_SHORT 0x1402
#define OPEN_GL_UNSIGNED_SHORT 0x1403
#define OPEN_GL_INT 0x1404
#define OPEN_GL_UNSIGNED_INT 0x1405
#define OPEN_GL_FLOAT 0x1406

#define OPEN_GL_FRAGMENT_SHADER 0x8B30
#define OPEN_GL_VERTEX_SHADER 0x8B31
#define OPEN_GL_COMPILE_STATUS 0x8B81

#define OPEN_GL_COMPILE_STATUS 0x8B81
#define OPEN_GL_LINK_STATUS 0x8B82
#define OPEN_GL_VALIDATE_STATUS 0x8B83
#define OPEN_GL_INFO_LOG_LENGTH 0x8B84


#define OPEN_GL_RGBA32F_ARB 0x8814
#define OPEN_GL_RGB32F_ARB 0x8815
#define OPEN_GL_ALPHA32F_ARB 0x8816
#define OPEN_GL_INTENSITY32F_ARB 0x8817
#define OPEN_GL_LUMINANCE32F_ARB 0x8818
#define OPEN_GL_LUMINANCE_ALPHA32F_ARB 0x8819
#define OPEN_GL_RGBA16F_ARB 0x881A
#define OPEN_GL_RGB16F_ARB 0x881B
#define OPEN_GL_ALPHA16F_ARB 0x881C
#define OPEN_GL_INTENSITY16F_ARB 0x881D
#define OPEN_GL_LUMINANCE16F_ARB 0x881E
#define OPEN_GL_LUMINANCE_ALPHA16F_ARB 0x881F
#define OPEN_GL_TEXTURE_RED_TYPE_ARB 0x8C10
#define OPEN_GL_TEXTURE_GREEN_TYPE_ARB 0x8C11
#define OPEN_GL_TEXTURE_BLUE_TYPE_ARB 0x8C12
#define OPEN_GL_TEXTURE_ALPHA_TYPE_ARB 0x8C13
#define OPEN_GL_TEXTURE_LUMINANCE_TYPE_ARB 0x8C14
#define OPEN_GL_TEXTURE_INTENSITY_TYPE_ARB 0x8C15
#define OPEN_GL_TEXTURE_DEPTH_TYPE_ARB 0x8C16
#define OPEN_GL_UNSIGNED_NORMALIZED_ARB 0x8C17


#define OPEN_GL_RED 0x1903
#define OPEN_GL_GREEN 0x1904
#define OPEN_GL_BLUE 0x1905
#define OPEN_GL_ALPHA 0x1906
#define OPEN_GL_RG8UI 0x8238
#define OPEN_GL_RG16UI 0x823A
#define OPEN_GL_RGBA8UI 0x8D7C

#define OPEN_GL_ALPHA4 0x803B
#define OPEN_GL_ALPHA8 0x803C
#define OPEN_GL_ALPHA12 0x803D
#define OPEN_GL_ALPHA16 0x803E
#define OPEN_GL_LUMINANCE4 0x803F
#define OPEN_GL_LUMINANCE8 0x8040
#define OPEN_GL_LUMINANCE12 0x8041
#define OPEN_GL_LUMINANCE16 0x8042
#define OPEN_GL_LUMINANCE4_ALPHA4 0x8043
#define OPEN_GL_LUMINANCE6_ALPHA2 0x8044
#define OPEN_GL_LUMINANCE8_ALPHA8 0x8045
#define OPEN_GL_LUMINANCE12_ALPHA4 0x8046
#define OPEN_GL_LUMINANCE12_ALPHA12 0x8047
#define OPEN_GL_LUMINANCE16_ALPHA16 0x8048
#define OPEN_GL_INTENSITY 0x8049
#define OPEN_GL_INTENSITY4 0x804A
#define OPEN_GL_INTENSITY8 0x804B
#define OPEN_GL_INTENSITY12 0x804C
#define OPEN_GL_INTENSITY16 0x804D
#define OPEN_GL_RGB4 0x804F
#define OPEN_GL_RGB5 0x8050
#define OPEN_GL_RGB8 0x8051
#define OPEN_GL_RGB10 0x8052
#define OPEN_GL_RGB12 0x8053
#define OPEN_GL_RGB16 0x8054
#define OPEN_GL_RGBA2 0x8055
#define OPEN_GL_RGBA4 0x8056
#define OPEN_GL_RGB5_A1 0x8057
#define OPEN_GL_RGBA8 0x8058
#define OPEN_GL_RGB10_A2 0x8059
#define OPEN_GL_RGBA12 0x805A
#define OPEN_GL_RGBA16 0x805B

#define OPEN_GL_RED 0x1903
#define OPEN_GL_GREEN 0x1904
#define OPEN_GL_BLUE 0x1905
#define OPEN_GL_ALPHA 0x1906
#define OPEN_GL_RGB 0x1907
#define OPEN_GL_RGBA 0x1908
#define OPEN_GL_LUMINANCE 0x1909
#define OPEN_GL_LUMINANCE_ALPHA 0x190A
#define OPEN_GL_BGR 0x80E0
#define OPEN_GL_BGRA 0x80E1

#define OPEN_GL_RGBA32UI_EXT 0x8D70
#define OPEN_GL_RGB32UI_EXT 0x8D71
#define OPEN_GL_ALPHA32UI_EXT 0x8D72
#define OPEN_GL_INTENSITY32UI_EXT 0x8D73
#define OPEN_GL_LUMINANCE32UI_EXT 0x8D74
#define OPEN_GL_LUMINANCE_ALPHA32UI_EXT 0x8D75
#define OPEN_GL_RGBA16UI_EXT 0x8D76
#define OPEN_GL_RGB16UI_EXT 0x8D77
#define OPEN_GL_ALPHA16UI_EXT 0x8D78
#define OPEN_GL_INTENSITY16UI_EXT 0x8D79
#define OPEN_GL_LUMINANCE16UI_EXT 0x8D7A
#define OPEN_GL_LUMINANCE_ALPHA16UI_EXT 0x8D7B
#define OPEN_GL_RGBA8UI_EXT 0x8D7C
#define OPEN_GL_RGB8UI_EXT 0x8D7D
#define OPEN_GL_ALPHA8UI_EXT 0x8D7E
#define OPEN_GL_INTENSITY8UI_EXT 0x8D7F
#define OPEN_GL_LUMINANCE8UI_EXT 0x8D80
#define OPEN_GL_LUMINANCE_ALPHA8UI_EXT 0x8D81
#define OPEN_GL_RGBA32I_EXT 0x8D82
#define OPEN_GL_RGB32I_EXT 0x8D83
#define OPEN_GL_ALPHA32I_EXT 0x8D84
#define OPEN_GL_INTENSITY32I_EXT 0x8D85
#define OPEN_GL_LUMINANCE32I_EXT 0x8D86
#define OPEN_GL_LUMINANCE_ALPHA32I_EXT 0x8D87
#define OPEN_GL_RGBA16I_EXT 0x8D88
#define OPEN_GL_RGB16I_EXT 0x8D89
#define OPEN_GL_ALPHA16I_EXT 0x8D8A
#define OPEN_GL_INTENSITY16I_EXT 0x8D8B
#define OPEN_GL_LUMINANCE16I_EXT 0x8D8C
#define OPEN_GL_LUMINANCE_ALPHA16I_EXT 0x8D8D
#define OPEN_GL_RGBA8I_EXT 0x8D8E
#define OPEN_GL_RGB8I_EXT 0x8D8F
#define OPEN_GL_ALPHA8I_EXT 0x8D90
#define OPEN_GL_INTENSITY8I_EXT 0x8D91
#define OPEN_GL_LUMINANCE8I_EXT 0x8D92
#define OPEN_GL_LUMINANCE_ALPHA8I_EXT 0x8D93
#define OPEN_GL_RED_INTEGER_EXT 0x8D94
#define OPEN_GL_GREEN_INTEGER_EXT 0x8D95
#define OPEN_GL_BLUE_INTEGER_EXT 0x8D96
#define OPEN_GL_ALPHA_INTEGER_EXT 0x8D97
#define OPEN_GL_RGB_INTEGER_EXT 0x8D98
#define OPEN_GL_RGBA_INTEGER_EXT 0x8D99
#define OPEN_GL_BGR_INTEGER_EXT 0x8D9A
#define OPEN_GL_BGRA_INTEGER_EXT 0x8D9B
#define OPEN_GL_LUMINANCE_INTEGER_EXT 0x8D9C
#define OPEN_GL_LUMINANCE_ALPHA_INTEGER_EXT 0x8D9D
#define OPEN_GL_RGBA_INTEGER_MODE_EXT 0x8D9E

#define OPEN_GL_DEPTH_COMPONENT 0x1902

#define OPEN_GL_DEPTH_COMPONENT16_SGIX 0x81A5
#define OPEN_GL_DEPTH_COMPONENT24_SGIX 0x81A6
#define OPEN_GL_DEPTH_COMPONENT32_SGIX 0x81A7

#define OPEN_GL_DEPTH_COMPONENT16 0x81A5
#define OPEN_GL_DEPTH_COMPONENT24 0x81A6
#define OPEN_GL_DEPTH_COMPONENT32 0x81A7

#define OPEN_GL_DEPTH_COMPONENT32F 0x8CAC
#define OPEN_GL_DEPTH32F_STENCIL8 0x8CAD
#define OPEN_GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD

#define OPEN_GL_DEPTH_COMPONENT32F_NV 0x8DAB
#define OPEN_GL_DEPTH32F_STENCIL8_NV 0x8DAC
#define OPEN_GL_FLOAT_32_UNSIGNED_INT_24_8_REV_NV 0x8DAD
#define OPEN_GL_DEPTH_BUFFER_FLOAT_MODE_NV 0x8DAF

#define OPEN_GL_UNPACK_SWAP_BYTES 0x0CF0
#define OPEN_GL_UNPACK_LSB_FIRST 0x0CF1
#define OPEN_GL_UNPACK_ROW_LENGTH 0x0CF2
#define OPEN_GL_UNPACK_SKIP_ROWS 0x0CF3
#define OPEN_GL_UNPACK_SKIP_PIXELS 0x0CF4
#define OPEN_GL_UNPACK_ALIGNMENT 0x0CF5
#define OPEN_GL_PACK_SWAP_BYTES 0x0D00
#define OPEN_GL_PACK_LSB_FIRST 0x0D01
#define OPEN_GL_PACK_ROW_LENGTH 0x0D02
#define OPEN_GL_PACK_SKIP_ROWS 0x0D03
#define OPEN_GL_PACK_SKIP_PIXELS 0x0D04
#define OPEN_GL_PACK_ALIGNMENT 0x0D05

#define OPEN_GL_NEAREST 0x2600
#define OPEN_GL_LINEAR 0x2601
#define OPEN_GL_NEAREST_MIPMAP_NEAREST 0x2700
#define OPEN_GL_LINEAR_MIPMAP_NEAREST 0x2701
#define OPEN_GL_NEAREST_MIPMAP_LINEAR 0x2702
#define OPEN_GL_LINEAR_MIPMAP_LINEAR 0x2703
#define OPEN_GL_TEXTURE_MAG_FILTER 0x2800
#define OPEN_GL_TEXTURE_MIN_FILTER 0x2801
#define OPEN_GL_TEXTURE_WRAP_S 0x2802
#define OPEN_GL_TEXTURE_WRAP_T 0x2803
#define OPEN_GL_TEXTURE_WRAP_R 0x8072
#define OPEN_GL_TEXTURE_BORDER_COLOR 0x1004
#define OPEN_GL_TEXTURE_BORDER 0x1005

namespace open
{
	namespace gl
	{
		struct GLState {
			RenderStateSet g_gl_renderStateSet;
			bool g_textureActive[16];

			struct Viewport
			{
				int32 x = 0;
				int32 y = 0;
				int32 width = 1;
				int32 height = 1;
			};
			Viewport g_gl_viewport;

			int32 g_gl_program = 0;
			int32 g_gl_vao = 0;
			int32 g_gl_vbo = 0;
		};
		extern GLState g_glState;

		void resetGL();

		inline void gl_init() {
			resetGL();
		}

		void gl_Viewport(int32 x, int32 y, int32 width, int32 height);


		void gl_Enable(StateMode mode, bool enable);

		void gl_ColorMask(uint32 colorMask);

		void gl_CullFace(Face face);

		void gl_FrontFace(FrontFace front);

		void gl_PolygonMode(Face face, PolygonMode mode);

		void gl_DepthMask(bool depthMask);

		void gl_DepthFunc(CompareFunction depthFunction);

		void gl_StencilFunc(CompareFunction stencilFunction, uint32 stencilRef, uint32 stencilFunMask);

		void gl_StencilMask(uint32 stencilMask);

		void gl_StencilOpSeparate(Face face, StencilOperation sfai, StencilOperation zfail, StencilOperation zpass);

		void gl_BlendFuncSeparate(BlendFun srcRGB, BlendFun destRGB, BlendFun srcAlpha, BlendFun destAlpha);

		void gl_BlendEquationSeparate(BlendEquation rgbBlendEquation, BlendEquation alphaBlendEquation);

		void gl_ActiveTexture(int unit);

		void gl_DisableTexture(int unit);

		void gl_DeleteBuffers(int size, uint32* bufferID);

		void gl_GenBuffers(int size, uint32* bufferID);

		void gl_BindBuffer(uint32 type, uint32 bufferID);

		void gl_BindBuffer(uint32 target, uint32 size, const void* data, uint32 usage);

		void* gl_MapBufferRange(uint32 target, uint32 offset, uint32 length, uint32 access);

		void gl_UnmapBuffer(uint32 type);

		void gl_GenVertexArrays(uint32 size, uint32* ids);

		void gl_DeleteVertexArrays(uint32 size, uint32* ids);

		void gl_BindVertexArray(uint32 id);

		void gl_EnableVertexAttribArray(uint32 usage);

		void gl_VertexAttribPointer(uint32 index, uint32 size, uint32 type, bool normalized, uint32 stride, const void* pointer);

		void gl_VertexAttribDivisor(uint32 index, uint32 divisor);

		void gl_GenRenderbuffers(uint32 size, uint32* ids);

		void gl_DeleteRenderbuffers(uint32 size, uint32* ids);

		void gl_BindRenderbuffer(uint32 type, uint32 id);

		void gl_RenderbufferStorage(uint32 type, uint32 format, uint32 width, uint32 height);

		void gl_GenFramebuffers(uint32 size, uint32* ids);

		void gl_BindFramebuffer(uint32 useage, uint32 id);

		void gl_FramebufferTexture2D(uint32 useage, uint32 attachment, uint32 textarget, uint32 texture_id, uint32 level);

		uint32 gl_CheckFramebufferStatus(uint32 usage);

		void gl_DeleteFramebuffers(uint32 size, uint32* ids);

		void gl_DrawBuffers(uint32 n, const uint32* bufs);

		void gl_ClearColor(float x, float y, float z, float w);

		void gl_ClearDepth(float clearDepth);

		void gl_ClearStencil(uint32 clearStencil);

		void gl_Clear(uint32 bufferBit);

		void gl_DrawElementsInstanced(uint32 mode, uint32 count, uint32 type, const void* indices, uint32 primcount);

		void gl_DrawArraysInstanced(uint32 mode, uint32 first, uint32 count, uint32 primcount);

		uint32 gl_CreateProgram();

		void gl_DeleteProgram(uint32 id);

		void gl_UseProgram(uint32 id);

		uint32 gl_CreateShader(uint32 useage);

		void gl_DeleteShader(uint32 shader);

		void gl_ShaderSource(uint32 shader, uint32 count, const char *const* string, const int32* length);

		void gl_CompileShader(uint32 id);

		void gl_GetShaderiv(uint32 shader, uint32 pname, int32* param);

		void gl_GetShaderInfoLog(uint32 shader, int32 bufSize, int32* length, char* infoLog);

		void gl_AttachShader(uint32 program, uint32 shader);

		void gl_LinkProgram(uint32 program);

		void gl_GetProgramiv(uint32 program, uint32 usage, int32* info);

		void gl_GetProgramInfoLog(uint32 program, int32 bufSize, int32* length, char* infoLog);

		uint32 gl_GetUniformLocation(uint32 program, const char* name);

		void gl_Uniform1i(uint32 l, uint32 v);

		void gl_Uniform1f(uint32 l, float v);

		void gl_Uniform2fv(uint32 l, int32 count, const float* v);

		void gl_Uniform3fv(uint32 l, int32 count, const float* v);

		void gl_Uniform4fv(uint32 l, int32 count, const float* v);

		void gl_UniformMatrix4fv(uint32 location, uint32 count, bool transpose, const float* value);

		void gl_GenTextures(uint32 count, uint32* textures);

		void gl_BindTexture(uint32 usage, uint32 texture);

		void gl_PixelStorei(uint32 usage, uint32 pack);

		void gl_TexImage2D(uint32 target, int32 level, int32 internalformat, int32 width, int32 height, int32 border, uint32 format, uint32 type, const void *pixels);

		void gl_TexSubImage2D(uint32 target, int32 level, int32 xoffset, int32 yoffset, int32 width, int32 height, uint32 format, uint32 type, const void *pixels);

		void gl_TexParameterfv(uint32 target, uint32 pname, const float *params);

		void gl_TexParameteri(uint32 target, uint32 pname, int32 param);
	}
}

#endif