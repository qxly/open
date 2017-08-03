#include "GlState.h"
#include <glew/glew.h>

#ifdef _DEBUG
#define	ASSERT_GL_ERROR() { int error = glGetError(); ASSERT_ARGS(!error,"gl error code is :%d",error);};
#else
#define ASSERT_GL_ERROR()
#endif

namespace open
{
	namespace gl
	{
		GLState g_glState = GLState();

		void resetGL()
		{
			ASSERT_GL_ERROR();

			memset(&g_glState.g_gl_renderStateSet, 123456789, sizeof(RenderStateSet));
			memset(g_glState.g_textureActive, 0, sizeof(g_glState.g_textureActive));

			for (int i = 0; i < 16; i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
			}


			glDepthMask(true);
			glColorMask(true, true, true, true);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glDisable(GL_STENCIL_TEST);

			glCullFace(FACE_BACK);
		}

		void gl_Viewport(int32 x, int32 y, int32 width, int32 height)
		{
			if (g_glState.g_gl_viewport.x == x && g_glState.g_gl_viewport.y == y && g_glState.g_gl_viewport.width == width && g_glState.g_gl_viewport.height == height)
				return;
			g_glState.g_gl_viewport.x = x;
			g_glState.g_gl_viewport.y = y;
			g_glState.g_gl_viewport.width = width;
			g_glState.g_gl_viewport.height = height;

			glViewport(x, y, width, height);
			ASSERT_GL_ERROR();
		}


		void gl_Enable(StateMode mode, bool enable)
		{
			bool last_enable = g_glState.g_gl_renderStateSet.getMode(mode);

			if (enable == last_enable)
				return;

			if (enable)
				glEnable(mode);
			else
				glDisable(mode);

			ASSERT_GL_ERROR();

			g_glState.g_gl_renderStateSet.setMode(mode, enable);
		}

		void gl_ColorMask(uint32 colorMask)
		{
			if (g_glState.g_gl_renderStateSet.colorMask == colorMask)
				return;

			g_glState.g_gl_renderStateSet.colorMask = colorMask;

			glColorMask((colorMask & COLORMASK_R) != 0,
				(colorMask & COLORMASK_G) != 0,
				(colorMask & COLORMASK_B) != 0,
				(colorMask & COLORMASK_A) != 0);

			ASSERT_GL_ERROR();
		}

		void gl_CullFace(Face face)
		{
			if (g_glState.g_gl_renderStateSet.cullFace == face)
				return;

			g_glState.g_gl_renderStateSet.cullFace = face;
			glCullFace(face);

			ASSERT_GL_ERROR();
		}

		void gl_FrontFace(FrontFace front)
		{
			if (g_glState.g_gl_renderStateSet.cullFrontFace == front)
				return;

			g_glState.g_gl_renderStateSet.cullFrontFace = front;
			glFrontFace(front);

			ASSERT_GL_ERROR();
		}

		void gl_PolygonMode(Face face, PolygonMode mode)
		{
			if (g_glState.g_gl_renderStateSet.polygonMode == mode && g_glState.g_gl_renderStateSet.polygonModeFace == face)
				return;

			g_glState.g_gl_renderStateSet.polygonMode = mode; g_glState.g_gl_renderStateSet.polygonModeFace = face;
			glPolygonMode(face, mode);

			ASSERT_GL_ERROR();
		}

		void gl_DepthMask(bool depthMask)
		{
			if (g_glState.g_gl_renderStateSet.depthMask == depthMask)
				return;

			g_glState.g_gl_renderStateSet.depthMask = depthMask;
			glDepthMask(depthMask);

			ASSERT_GL_ERROR();
		}

		void gl_DepthFunc(CompareFunction depthFunction)
		{
			if (g_glState.g_gl_renderStateSet.depthFunction == depthFunction)
				return;

			g_glState.g_gl_renderStateSet.depthFunction = depthFunction;
			glDepthFunc(depthFunction);

			ASSERT_GL_ERROR();
		}

		void gl_StencilFunc(CompareFunction stencilFunction, uint32 stencilRef, uint32 stencilFunMask)
		{
			if (g_glState.g_gl_renderStateSet.stencilFunction == stencilFunction &&
				g_glState.g_gl_renderStateSet.stencilRef == stencilRef &&
				g_glState.g_gl_renderStateSet.stencilFunMask == stencilFunMask)
				return;

			g_glState.g_gl_renderStateSet.stencilFunction = stencilFunction;
			g_glState.g_gl_renderStateSet.stencilRef = stencilRef;
			g_glState.g_gl_renderStateSet.stencilFunMask = stencilFunMask;

			glStencilFunc(stencilFunction, stencilRef, stencilFunMask);

			ASSERT_GL_ERROR();
		}

		void gl_StencilMask(uint32 stencilMask)
		{
			if (g_glState.g_gl_renderStateSet.stencilMask == stencilMask)
				return;

			g_glState.g_gl_renderStateSet.stencilMask = stencilMask;
			glStencilMask(stencilMask);

			ASSERT_GL_ERROR();
		}

		void gl_StencilOpSeparate(Face face, StencilOperation sfai, StencilOperation zfail, StencilOperation zpass)
		{
			switch (face)
			{
			case open::FACE_FRONT:
				if (g_glState.g_gl_renderStateSet.sfail_front == sfai &&
					g_glState.g_gl_renderStateSet.zfail_front == zfail &&
					g_glState.g_gl_renderStateSet.zpass_front == zpass)
					return;
				g_glState.g_gl_renderStateSet.sfail_front = sfai;
				g_glState.g_gl_renderStateSet.zfail_front = zfail;
				g_glState.g_gl_renderStateSet.zpass_front = zpass;
				glStencilOpSeparate(GL_FRONT, sfai, zfail, zpass);
				ASSERT_GL_ERROR();
				return;
			case open::FACE_BACK:
				if (g_glState.g_gl_renderStateSet.sfail_back == sfai &&
					g_glState.g_gl_renderStateSet.zfail_back == zfail &&
					g_glState.g_gl_renderStateSet.zpass_back == zpass)
					return;
				g_glState.g_gl_renderStateSet.sfail_back = sfai;
				g_glState.g_gl_renderStateSet.zfail_back = zfail;
				g_glState.g_gl_renderStateSet.zpass_back = zpass;
				glStencilOpSeparate(GL_BACK, sfai, zfail, zpass);
				ASSERT_GL_ERROR();
				return;
			}
		}

		void gl_BlendFuncSeparate(BlendFun srcRGB, BlendFun destRGB, BlendFun srcAlpha, BlendFun destAlpha)
		{
			if (g_glState.g_gl_renderStateSet.srcRGB == srcRGB &&
				g_glState.g_gl_renderStateSet.destRGB == destRGB &&
				g_glState.g_gl_renderStateSet.srcAlpha == srcAlpha &&
				g_glState.g_gl_renderStateSet.destAlpha == destAlpha)
				return;

			g_glState.g_gl_renderStateSet.srcRGB = srcRGB;
			g_glState.g_gl_renderStateSet.destRGB = destRGB;
			g_glState.g_gl_renderStateSet.srcAlpha = srcAlpha;
			g_glState.g_gl_renderStateSet.destAlpha = destAlpha;

			glBlendFuncSeparate(srcRGB, destRGB, srcAlpha, destAlpha);
			ASSERT_GL_ERROR();
		}

		void gl_BlendEquationSeparate(BlendEquation rgbBlendEquation, BlendEquation alphaBlendEquation)
		{
			if (g_glState.g_gl_renderStateSet.rgbBlendEquation == rgbBlendEquation &&
				g_glState.g_gl_renderStateSet.alphaBlendEquation == alphaBlendEquation)
				return;

			g_glState.g_gl_renderStateSet.rgbBlendEquation = rgbBlendEquation;
			g_glState.g_gl_renderStateSet.alphaBlendEquation = alphaBlendEquation;

			glBlendEquationSeparate(rgbBlendEquation, alphaBlendEquation);
			ASSERT_GL_ERROR();
		}

		void gl_ActiveTexture(int unit) {
			glActiveTexture(GL_TEXTURE0 + unit);
			g_glState.g_textureActive[unit] = true;
			ASSERT_GL_ERROR();
		}

		void gl_DisableTexture(int unit) {

			if (!g_glState.g_textureActive[unit])
				return;

			g_glState.g_textureActive[unit] = false;
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, 0);
			ASSERT_GL_ERROR();
		}

		void gl_DeleteBuffers(int size, uint32* bufferID) {
			glDeleteBuffers(size, bufferID);
			ASSERT_GL_ERROR();
		}

		void gl_GenBuffers(int size, uint32* bufferID) {
			glGenBuffers(size, bufferID);
			ASSERT_GL_ERROR();
		}

		void gl_BindBuffer(uint32 type, uint32 bufferID) {
			glBindBuffer(type, bufferID);
			ASSERT_GL_ERROR();
		}

		void gl_BindBuffer(uint32 target, uint32 size, const void* data, uint32 usage) {
			glBufferData(target, size, data, usage);
			ASSERT_GL_ERROR();
		}

		void* gl_MapBufferRange(uint32 target, uint32 offset, uint32 length, uint32 access) {
			void* buffer = glMapBufferRange(target, offset, length, access);
			ASSERT_GL_ERROR();
			return buffer;
		}

		void gl_UnmapBuffer(uint32 type) {
			glUnmapBuffer(type);
			ASSERT_GL_ERROR();
		}

		void gl_GenVertexArrays(uint32 size, uint32* ids) {
			glGenVertexArrays(size, ids);
			ASSERT_GL_ERROR();
		}

		void gl_DeleteVertexArrays(uint32 size, uint32* ids) {
			glDeleteVertexArrays(size, ids);
			ASSERT_GL_ERROR();
		}

		void gl_BindVertexArray(uint32 id) {
			glBindVertexArray(id);
			ASSERT_GL_ERROR();
		}

		void gl_EnableVertexAttribArray(uint32 usage) {
			glEnableVertexAttribArray(usage);
			ASSERT_GL_ERROR();
		}

		void gl_VertexAttribPointer(uint32 index, uint32 size, uint32 type, bool normalized, uint32 stride, const void* pointer) {
			glVertexAttribPointer(index, size, type, normalized, stride, pointer);
			ASSERT_GL_ERROR();
		}

		void gl_VertexAttribDivisor(uint32 index, uint32 divisor) {
			glVertexAttribDivisor(index, divisor);
			ASSERT_GL_ERROR();
		}

		void gl_GenRenderbuffers(uint32 size, uint32* ids) {
			glGenRenderbuffers(size, ids);
			ASSERT_GL_ERROR();
		}

		void gl_DeleteRenderbuffers(uint32 size, uint32* ids) {
			glDeleteRenderbuffers(size, ids);
			ASSERT_GL_ERROR();
		}

		void gl_BindRenderbuffer(uint32 type, uint32 id) {
			glBindRenderbuffer(type, id);
			ASSERT_GL_ERROR();
		}

		void gl_RenderbufferStorage(uint32 type, uint32 format, uint32 width, uint32 height) {
			glRenderbufferStorage(type, format, width, height);
			ASSERT_GL_ERROR();
		}

		uint32 gl_CheckFramebufferStatus(uint32 usage) {
			uint32 error = glCheckFramebufferStatus(usage);
			ASSERT_GL_ERROR();
			return error;
		}

		void gl_GenFramebuffers(uint32 size, uint32* ids) {
			glGenFramebuffers(size, ids);
			ASSERT_GL_ERROR();
		}

		void gl_BindFramebuffer(uint32 useage, uint32 id) {
			glBindFramebuffer(useage, id);
			ASSERT_GL_ERROR();
		}

		void gl_FramebufferTexture2D(uint32 useage, uint32 attachment, uint32 textarget, uint32 texture_id, uint32 level) {
			glFramebufferTexture2D(useage, attachment, textarget, texture_id, level);
			ASSERT_GL_ERROR();
		}

		void gl_DeleteFramebuffers(uint32 size, uint32* ids) {
			glDeleteFramebuffers(size, ids);
			ASSERT_GL_ERROR();
		}

		void gl_DrawBuffers(uint32 n, const uint32* bufs) {
			glDrawBuffers(n, bufs);
			ASSERT_GL_ERROR();
		}

		void gl_ClearColor(float x, float y, float z, float w) {
			glClearColor(x, y, z, w);
			ASSERT_GL_ERROR();
		}

		void gl_ClearDepth(float clearDepth) {
			glClearDepth(clearDepth);
			ASSERT_GL_ERROR();
		}

		void gl_ClearStencil(uint32 clearStencil) {
			glClearStencil(clearStencil);
			ASSERT_GL_ERROR();
		}

		void gl_Clear(uint32 bufferBit) {
			glClear(bufferBit);
			ASSERT_GL_ERROR();
		}

		void gl_DrawElementsInstanced(uint32 mode, uint32 count, uint32 type, const void* indices, uint32 primcount) {
			glDrawElementsInstanced(mode, count, type, indices, primcount);
			ASSERT_GL_ERROR();
		}

		void gl_DrawArraysInstanced(uint32 mode, uint32 first, uint32 count, uint32 primcount) {
			glDrawArraysInstanced(mode, first, count, primcount);
			ASSERT_GL_ERROR();
		}

		uint32 gl_CreateProgram() {
			uint32 id = glCreateProgram();
			ASSERT_GL_ERROR();
			return id;
		}

		void gl_DeleteProgram(uint32 id) {
			glDeleteProgram(id);
			ASSERT_GL_ERROR();
		}

		void gl_UseProgram(uint32 id) {
			glUseProgram(id);
			ASSERT_GL_ERROR();
		}

		uint32 gl_CreateShader(uint32 useage) {
			uint32 id = glCreateShader(useage);
			ASSERT_GL_ERROR();
			return id;
		}

		void gl_DeleteShader(uint32 shader) {
			glDeleteShader(shader);
			ASSERT_GL_ERROR();
		}

		void gl_ShaderSource(uint32 shader, uint32 count, const char *const* string, const int32* length) {
			glShaderSource(shader, count, string, length);
			ASSERT_GL_ERROR();
		}

		void gl_CompileShader(uint32 id) {
			glCompileShader(id);
			ASSERT_GL_ERROR();
		}

		void gl_GetShaderiv(uint32 shader, uint32 pname, int32* param) {
			glGetShaderiv(shader, pname, param);
			ASSERT_GL_ERROR();
		}

		void gl_GetShaderInfoLog(uint32 shader, int32 bufSize, int32* length, char* infoLog) {
			glGetShaderInfoLog(shader, bufSize, length, infoLog);
			ASSERT_GL_ERROR();
		}

		void gl_AttachShader(uint32 program, uint32 shader) {
			glAttachShader(program, shader);
			ASSERT_GL_ERROR();
		}

		void gl_LinkProgram(uint32 program) {
			glLinkProgram(program);
			ASSERT_GL_ERROR();
		}

		void gl_GetProgramiv(uint32 program, uint32 usage, int32* info) {
			glGetProgramiv(program, usage, info);
			ASSERT_GL_ERROR();
		}

		void gl_GetProgramInfoLog(uint32 program, int32 bufSize, int32* length, char* infoLog) {
			glGetProgramInfoLog(program, bufSize, length, infoLog);
			ASSERT_GL_ERROR();
		}

		uint32 gl_GetUniformLocation(uint32 program, const char* name) {
			uint32 l = glGetUniformLocation(program, name);
			ASSERT_GL_ERROR();
			return l;
		}

		void gl_Uniform1i(uint32 l, uint32 v) {
			glUniform1i(l, v);
			ASSERT_GL_ERROR();
		}

		void gl_Uniform1f(uint32 l, float v) {
			glUniform1f(l, v);
			ASSERT_GL_ERROR();
		}

		void gl_Uniform2fv(uint32 l, int32 count, const float* v) {
			glUniform2fv(l, count, v);
			ASSERT_GL_ERROR();
		}

		void gl_Uniform3fv(uint32 l, int32 count, const float* v) {
			glUniform3fv(l, count, v);
			ASSERT_GL_ERROR();
		}

		void gl_Uniform4fv(uint32 l, int32 count, const float* v) {
			glUniform4fv(l, count, v);
			ASSERT_GL_ERROR();
		}

		void gl_UniformMatrix4fv(uint32 location, uint32 count, bool transpose, const float* value) {
			glUniformMatrix4fv(location, count, transpose, value);
			ASSERT_GL_ERROR();
		}

		void gl_GenTextures(uint32 count, uint32* textures) {
			glGenTextures(count, textures);
			ASSERT_GL_ERROR();
		}

		void gl_BindTexture(uint32 usage, uint32 texture) {
			glBindTexture(usage, texture);
			ASSERT_GL_ERROR();
		}

		void gl_PixelStorei(uint32 usage, uint32 pack) {
			glPixelStorei(usage, pack);
			ASSERT_GL_ERROR();
		}

		void gl_TexImage2D(uint32 target, int32 level, int32 internalformat, int32 width, int32 height, int32 border, uint32 format, uint32 type, const void *pixels) {
			glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
			ASSERT_GL_ERROR();
		}

		void gl_TexSubImage2D(uint32 target, int32 level, int32 xoffset, int32 yoffset, int32 width, int32 height, uint32 format, uint32 type, const void *pixels) {
			glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
			ASSERT_GL_ERROR();
		}

		void gl_TexParameterfv(uint32 target, uint32 pname, const float *params) {
			glTexParameterfv(target, pname, params);
			ASSERT_GL_ERROR();
		}

		void gl_TexParameteri(uint32 target, uint32 pname, int32 param) {
			glTexParameteri(target, pname, param);
			ASSERT_GL_ERROR();
		}
	}
}