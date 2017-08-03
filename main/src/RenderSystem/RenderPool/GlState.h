#ifndef RENDERSYSTEM_GL_STATE_H
#define RENDERSYSTEM_GL_STATE_H 1

#include <glew/glew.h>
#include <RenderSystem/RenderStatesInterface.h>

namespace open
{
	namespace gl
	{
		RenderStateSet g_gl_renderStateSet;

		struct Viewport
		{
			int32 x = 0;
			int32 y = 0;
			int32 width = 1;
			int32 height = 1;
		};
		Viewport g_gl_viewport;

		inline void gl_Viewport(int32 x,int32 y,int32 width,int32 height)
		{
			if (g_gl_viewport.x == x && g_gl_viewport.y == y && g_gl_viewport.width == width && g_gl_viewport.height == height)
				return;
			g_gl_viewport.x = x; 
			g_gl_viewport.y = y;
			g_gl_viewport.width = width; 
			g_gl_viewport.height = height;

			glViewport(x, y, width, height);
		}

		int32 g_gl_program = 0;
		int32 g_gl_vao = 0;
		int32 g_gl_vbo = 0;


		inline void gl_Enable(StateMode mode, bool enable)
		{
			bool last_enable = g_gl_renderStateSet.getMode(mode);

			if (enable == last_enable)
				return;

			if (enable)
				glEnable(mode);
			else
				glDisable(mode);

			g_gl_renderStateSet.setMode(mode, enable);
		}

		inline void gl_ColorMask(uint32 colorMask)
		{
			if (g_gl_renderStateSet.colorMask == colorMask)
				return;

			g_gl_renderStateSet.colorMask = colorMask;

			glColorMask((colorMask & COLORMASK_R) != 0,
				(colorMask & COLORMASK_G) != 0,
				(colorMask & COLORMASK_B) != 0,
				(colorMask & COLORMASK_A) != 0);
		}

		inline void gl_CullFace(Face face)
		{
			if (g_gl_renderStateSet.cullFace == face)
				return;

			g_gl_renderStateSet.cullFace = face;
			glCullFace(face);
		}

		inline void gl_FrontFace(FrontFace front)
		{
			if (g_gl_renderStateSet.cullFrontFace == front)
				return;

			g_gl_renderStateSet.cullFrontFace = front;
			glFrontFace(front);
		}

		inline void gl_PolygonMode(Face face, PolygonMode mode)
		{
			if (g_gl_renderStateSet.polygonMode == mode && g_gl_renderStateSet.polygonModeFace == face)
				return;

			g_gl_renderStateSet.polygonMode = mode; g_gl_renderStateSet.polygonModeFace = face;
			glPolygonMode(face, mode);
		}

		inline void gl_DepthMask(bool depthMask)
		{
			if (g_gl_renderStateSet.depthMask == depthMask)
				return;

			g_gl_renderStateSet.depthMask = depthMask;
			glDepthMask(depthMask);
		}

		inline void gl_DepthFunc(CompareFunction depthFunction)
		{
			if (g_gl_renderStateSet.depthFunction == depthFunction)
				return;

			g_gl_renderStateSet.depthFunction = depthFunction;
			glDepthFunc(depthFunction);
		}

		inline void gl_StencilFunc(CompareFunction stencilFunction, uint32 stencilRef, uint32 stencilFunMask)
		{
			if (g_gl_renderStateSet.stencilFunction == stencilFunction &&
				g_gl_renderStateSet.stencilRef == stencilRef &&
				g_gl_renderStateSet.stencilFunMask == stencilFunMask)
				return;

			g_gl_renderStateSet.stencilFunction = stencilFunction;
			g_gl_renderStateSet.stencilRef = stencilRef;
			g_gl_renderStateSet.stencilFunMask = stencilFunMask;

			glStencilFunc(stencilFunction, stencilRef, stencilFunMask);
		}

		inline void gl_StencilMask(uint32 stencilMask)
		{
			if (g_gl_renderStateSet.stencilMask == stencilMask)
				return;

			g_gl_renderStateSet.stencilMask = stencilMask;
			glStencilMask(stencilMask);
		}

		inline void gl_StencilOpSeparate(Face face, StencilOperation sfai, StencilOperation zfail, StencilOperation zpass)
		{
			switch (face)
			{
			case open::FACE_FRONT:
				if (g_gl_renderStateSet.sfail_front == sfai &&
					g_gl_renderStateSet.zfail_front == zfail &&
					g_gl_renderStateSet.zpass_front == zpass)
					return;
				g_gl_renderStateSet.sfail_front = sfai;
				g_gl_renderStateSet.zfail_front = zfail;
				g_gl_renderStateSet.zpass_front = zpass;
				glStencilOpSeparate(GL_FRONT, sfai, zfail, zpass);
				return;
			case open::FACE_BACK:
				if (g_gl_renderStateSet.sfail_back == sfai &&
					g_gl_renderStateSet.zfail_back == zfail &&
					g_gl_renderStateSet.zpass_back == zpass)
					return;
				g_gl_renderStateSet.sfail_back = sfai;
				g_gl_renderStateSet.zfail_back = zfail;
				g_gl_renderStateSet.zpass_back = zpass;
				glStencilOpSeparate(GL_BACK, sfai, zfail, zpass);
				return;
			}
		}

		inline void gl_BlendFuncSeparate(BlendFun srcRGB, BlendFun destRGB, BlendFun srcAlpha, BlendFun destAlpha)
		{
			if (g_gl_renderStateSet.srcRGB == srcRGB &&
				g_gl_renderStateSet.destRGB == destRGB &&
				g_gl_renderStateSet.srcAlpha == srcAlpha &&
				g_gl_renderStateSet.destAlpha == destAlpha)
				return;

			g_gl_renderStateSet.srcRGB = srcRGB;
			g_gl_renderStateSet.destRGB = destRGB;
			g_gl_renderStateSet.srcAlpha = srcAlpha;
			g_gl_renderStateSet.destAlpha = destAlpha;

			glBlendFuncSeparate(srcRGB, destRGB, srcAlpha, destAlpha);
		}

		inline void gl_BlendEquationSeparate(BlendEquation rgbBlendEquation, BlendEquation alphaBlendEquation)
		{
			if (g_gl_renderStateSet.rgbBlendEquation == rgbBlendEquation &&
				g_gl_renderStateSet.alphaBlendEquation == alphaBlendEquation)
				return;

			g_gl_renderStateSet.rgbBlendEquation = rgbBlendEquation;
			g_gl_renderStateSet.alphaBlendEquation = alphaBlendEquation;

			glBlendEquationSeparate(rgbBlendEquation, alphaBlendEquation);
		}
	}
}

#endif