#include "RenderPool.h"
#include <open/Log.h>

#include "../GlState.h"
#include "../RenderStates/Program.h"
#include "../RenderStates/TextureUnitSet.h"

#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>
#include "../Buffer/GPUBuffer.h"
#include "Light.h"


using namespace open::gl;

namespace open
{
	RenderPool::RenderPool()
	{
		_lightManager = new LightManager;
	}

	RenderPool::~RenderPool()
	{
		delete _lightManager;
	}


	void RenderPool::clear(uint32 bufferBit)
	{
		gl_ClearColor(_clearColor.x(), _clearColor.y(), _clearColor.z(), _clearColor.w());
		gl_ClearDepth(static_cast<float>(_clearDepth));
		gl_ClearStencil(_clearStencil);

		gl_Clear(bufferBit);
	}

	void RenderPool::resetRenderState()
	{
		resetGL();

		_currentVAO = nullptr;
		_currentIndexs = nullptr;

	}

	void RenderPool::applyViewport(int32 x, int32 y, int32 width, int32 height)
	{
		gl_Viewport(x, y, width, height);
	}

	void RenderPool::applyRenderTarget(IRenderTarget* renderTarget)
	{
		_currentRenderTarget = renderTarget;
		_currentRenderTarget->apply();
	}

	void RenderPool::applyProgram(IProgram* program)
	{
		((Program*)program)->apply();
		_currentProgram = program;

		_forceAppMaterial = true;
	}


	void RenderPool::applyRenderStateSet(const RenderStateSet& nm)
	{
		gl_Enable(DEPTH_TEST, nm.getMode(DEPTH_TEST));
		gl_Enable(STENCIL_TEST, nm.getMode(STENCIL_TEST));
		gl_Enable(BLEND, nm.getMode(BLEND));
		gl_Enable(CULL_FACE, nm.getMode(CULL_FACE));

		gl_ColorMask(nm.colorMask);
		gl_CullFace(nm.cullFace);
		gl_FrontFace(nm.cullFrontFace);

		gl_PolygonMode(nm.polygonModeFace, nm.polygonMode);

		gl_DepthMask(nm.depthMask);
		gl_DepthFunc(nm.depthFunction);

		gl_StencilFunc(nm.stencilFunction, nm.stencilRef, nm.stencilFunMask);
		gl_StencilMask(nm.stencilMask);
		gl_StencilOpSeparate(FACE_FRONT, nm.sfail_front, nm.zfail_front, nm.zpass_front);
		gl_StencilOpSeparate(FACE_BACK, nm.sfail_back, nm.zfail_back, nm.zpass_back);

		gl_BlendFuncSeparate(nm.srcRGB, nm.destRGB, nm.srcAlpha, nm.destAlpha);

		gl_BlendEquationSeparate(nm.rgbBlendEquation, nm.alphaBlendEquation);
	}

	void RenderPool::applyTextureUnits(ITextureUnitSet * unitSet)
	{
		((TextureUnitSet*)unitSet)->apply();
	}

	void RenderPool::apply(IVertexArrayObject* vao, IVertexIndexObject* indexs, DrawMode drawMode, int32 primcount)
	{
		_batch++;

		if (_currentVAO != vao)
		{
			((VertexArrayObject*)vao)->apply();
			_currentVAO = vao;
		}
		VertexIndexObject* index_internal = (VertexIndexObject*)indexs;
		if (_currentIndexs != indexs)
		{
			_currentIndexs = indexs;
			index_internal->apply();
		}

		DataType dt = index_internal->getDataType();
		int32 size = 0;
		uint32 type = 0;

		switch (dt)
		{
		case DATATYPE_UNSIGNED_BYTE:
			size = indexs->getGPUBuffer()->size() / sizeof(uint8);
			type = OPEN_GL_UNSIGNED_BYTE;
			break;
		case DATATYPE_UNSIGNED_SHORT:
			size = indexs->getGPUBuffer()->size() / sizeof(uint16);
			type = OPEN_GL_UNSIGNED_SHORT;
			break;
		case DATATYPE_UNSIGNED_INT:
			size = indexs->getGPUBuffer()->size() / sizeof(uint32);
			type = OPEN_GL_UNSIGNED_INT;
			break;
		case DATATYPE_INT:
			size = indexs->getGPUBuffer()->size() / sizeof(int32);
			type = OPEN_GL_INT;
			break;
		}
#if _DEBUG
		ASSERT_(size && type);
#endif
		if (size && type)
			gl_DrawElementsInstanced(drawMode, size, type, nullptr, primcount);
	}

	void RenderPool::apply(IVertexArrayObject* vao, int32 start, int32 count, DrawMode drawMode, int32 primcount)
	{
		_batch++;
		if (_currentVAO != vao)
		{
			((VertexArrayObject*)vao)->apply();
			_currentVAO = vao;
		}
		gl_DrawArraysInstanced(drawMode, start, count, primcount);
	}

	void RenderPool::garbageResource()
	{
	}
}