#include "FrameBuffer.h"
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderEnumeration.h>
#include <open/Ptr.h>

#include "../GlState.h"
using namespace open::gl;

namespace open
{
	void RenderBuffer::releaseGPUContent()
	{
		if(!_renderBuffer)
			return;

		gl_DeleteRenderbuffers(1,&_renderBuffer);
		_renderBuffer = 0;
	}

	void RenderBuffer::apply()
	{
		if(_dirty)
		{
			_dirty = false;

			if(_renderBuffer)
				releaseGPUContent();

			gl_GenRenderbuffers(1, &_renderBuffer);
			gl_BindRenderbuffer(OPEN_GL_RENDERBUFFER, _renderBuffer);
			gl_RenderbufferStorage(OPEN_GL_RENDERBUFFER, _format, _width,_height);
		}
	}

	struct TextureAttachmentStorage;
	struct RenderBufferAttachmentStorage;

	struct FrameBufferAttachmentInternal::RenderAttachmentStorage
	{
		virtual ~RenderAttachmentStorage(){}

		virtual void releaseGPUContent() = 0;

		virtual void apply(FrameBufferAttachment attachment,uint32 frameBuffer) = 0;

		virtual int32 getWidth() = 0;

		virtual int32 getHeight() = 0;
	};

	struct TextureAttachmentStorage : public FrameBufferAttachmentInternal::RenderAttachmentStorage
	{
		TextureAttachmentStorage(ITexture* t,uint32 l)
			:texture(t),level(l){}

		virtual void apply(FrameBufferAttachment attachment,uint32 frameBuffer)
		{
			if(!texture)
				return;
			texture->apply();

			gl_FramebufferTexture2D(OPEN_GL_DRAW_FRAMEBUFFER,attachment, OPEN_GL_TEXTURE_2D,texture->getHandle(),level);
		}

		virtual void releaseGPUContent()
		{
		}

		virtual int32 getWidth() { 
			if (texture) 
				return texture->getWidth();
			return 0; 
		}

		virtual int32 getHeight() {
			if (texture)
				return texture->getHeight();
			return 0;
		}

		Ptr<ITexture> texture;
		uint32 level;
	};
	struct RenderBufferAttachmentStorage  : public FrameBufferAttachmentInternal::RenderAttachmentStorage
	{
		RenderBufferAttachmentStorage(RenderBuffer* rb)
			:renderBuffer(rb){}

		virtual void apply(FrameBufferAttachment attachment,uint32 frameBuffer)
		{
			if(!renderBuffer)
				return;

			renderBuffer->apply();
			gl_RenderbufferStorage(OPEN_GL_RENDERBUFFER,attachment, renderBuffer->getWidth(),renderBuffer->getHeight());
		}

		virtual void releaseGPUContent()
		{
		}

		virtual int32 getWidth() {
			if (renderBuffer)
				return renderBuffer->getWidth();
			return 0;
		}

		virtual int32 getHeight() {
			if (renderBuffer)
				return renderBuffer->getHeight();
			return 0;
		}

		RenderBuffer* renderBuffer;
	};

	FrameBufferAttachmentInternal::FrameBufferAttachmentInternal()
		:_storage(nullptr)
	{
	}

	FrameBufferAttachmentInternal::FrameBufferAttachmentInternal(IRenderBuffer* renderBuffer)
	{
		_storage = new RenderBufferAttachmentStorage((RenderBuffer*)renderBuffer);
	}

	FrameBufferAttachmentInternal::FrameBufferAttachmentInternal(ITexture* texture,uint32 mipLevel)
	{
		_storage = new TextureAttachmentStorage(texture,mipLevel);
	}

	FrameBufferAttachmentInternal::~FrameBufferAttachmentInternal()
	{
		delete _storage;
	}

	/////////////////////////////////
	FrameBuffer::FrameBuffer()
		:_frameBuffer(0),_drawBufferSize(0),_updateState(0)
	{
		memset(_drawBuffers,0,sizeof(_drawBuffers));
		memset(_attachmentMap,0,sizeof(_attachmentMap));
	}

	FrameBuffer::~FrameBuffer()
	{
		detach();
	}

	bool FrameBuffer::vaild()
	{
		return _frameBuffer != 0;
	}

	void FrameBuffer::attach(FrameBufferAttachment ap,FrameBufferAttachmentInternal* fba)
	{
		switch(ap)
		{
		case DEPTH_ATTACHMENT:
			_attachmentMap[0] = fba;
			break;
		case STENCIL_ATTACHMENT:
			_attachmentMap[1] = fba;
			break;
		case DEPTH_STENCIL_ATTACHMENT:
			_attachmentMap[2] = fba;
			break;
		default:
			_attachmentMap[ap - COLOR_ATTACHMENT0 + 3] = fba;
		}
		_updateState |= UPDATE_DRAWBUFFER;
	}

	inline	FrameBufferAttachment FrameBuffer::convertToFrameBufferAttachment(int index)
	{
		switch (index)
		{
		case 0:
			return DEPTH_ATTACHMENT;
		case 1:
			return STENCIL_ATTACHMENT;
			break;
		case 2:
			return DEPTH_STENCIL_ATTACHMENT;
		default:
			return (FrameBufferAttachment)(COLOR_ATTACHMENT0 + index - 3);
		}
	}

	void FrameBuffer::attach(FrameBufferAttachment ap,IRenderBuffer* renderable)
	{
		FrameBufferAttachmentInternal* rfa = new FrameBufferAttachmentInternal(renderable);
		attach(ap,rfa);
	}

	void FrameBuffer::attach(FrameBufferAttachment ap,ITexture* texture)
	{
		FrameBufferAttachmentInternal* rfa = new FrameBufferAttachmentInternal(texture);
		attach(ap,rfa);
	}

	void FrameBuffer::detach(FrameBufferAttachment ap)
	{
		FrameBufferAttachmentInternal* interanl = nullptr;
		switch(ap)
		{
		case DEPTH_ATTACHMENT:
			interanl = _attachmentMap[0];
			_attachmentMap[0] = nullptr;
			break;
		case STENCIL_ATTACHMENT:
			interanl = _attachmentMap[1];
			_attachmentMap[1] = nullptr;
			break;
		case DEPTH_STENCIL_ATTACHMENT:
			interanl = _attachmentMap[2];
			_attachmentMap[2] = nullptr;
			break;
		default:
			interanl = _attachmentMap[ap - COLOR_ATTACHMENT0 + 3];
			_attachmentMap[ap - COLOR_ATTACHMENT0 + 3] = nullptr;
		}

		if(interanl)
			delete interanl;
	}

	void FrameBuffer::detach()
	{
		for(int32 i = 0; i < 19; i++)
		{
			if(!_attachmentMap[i])
				continue;

			delete _attachmentMap[i];
			_attachmentMap[i] = nullptr;
		}
	}

	void FrameBuffer::releaseGPUContent()
	{
		if(!_frameBuffer)
			return;

		gl_DeleteFramebuffers(1,&_frameBuffer);
		_frameBuffer = 0;

		detach();
	}

	int32 FrameBuffer::getWidth()
	{
		int32 width = 0;
		for (int32 i = 3; i < 19; i++)
		{
			if (!_attachmentMap[i])
				continue;

			width = _attachmentMap[i]->_storage->getWidth();
			if (!width)
				return width;
		}
		return width;
	}

	int32 FrameBuffer::getHeight()
	{
		int32 height = 0;
		for (int32 i = 3; i < 19; i++)
		{
			if (!_attachmentMap[i])
				continue;

			height = _attachmentMap[i]->_storage->getHeight();
			if (!height)
				return height;
		}
		return height;
	}

	void FrameBuffer::updateDrawBuffer()
	{
		if((_updateState & UPDATE_DRAWBUFFER) != 0)
		{
			_updateState &= ~UPDATE_DRAWBUFFER;
			_drawBufferSize = 0;

			reset();

			for(int32 i = 3; i < 19; i++)
			{
				if(!_attachmentMap[i])
					continue;

				_drawBuffers[_drawBufferSize++] = convertToFrameBufferAttachment(i);
			}
		}
	}

	void FrameBuffer::reset()
	{
		releaseGPUContent();

		gl_GenFramebuffers(1,&_frameBuffer);
		gl_BindFramebuffer(OPEN_GL_DRAW_FRAMEBUFFER,_frameBuffer);

		FrameBufferAttachmentInternal* fba = nullptr;
		for(int32 i = 0; i < 19; i++)
		{
			if(!_attachmentMap[i])
				continue;

			_attachmentMap[i]->_storage->apply(convertToFrameBufferAttachment(i),_frameBuffer);
		}

		uint32 error = gl_CheckFramebufferStatus(OPEN_GL_DRAW_FRAMEBUFFER);

		if(!(OPEN_GL_FRAMEBUFFER_COMPLETE == error))
			ASSERT_(0);

		gl_BindFramebuffer(OPEN_GL_DRAW_FRAMEBUFFER,0);
	}

	void FrameBuffer::apply()
	{
		updateDrawBuffer();

		if(!_frameBuffer)
			return;

		gl_BindFramebuffer(OPEN_GL_DRAW_FRAMEBUFFER, _frameBuffer);

		if(_drawBufferSize > 0)
			gl_DrawBuffers(_drawBufferSize,_drawBuffers);
	}

	//void FrameBuffer::end()
	//{
	//	//uint32 default_wind[] = { GL_BACK_LEFT };

	//	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	//	//glDrawBuffers(1,default_wind);
	//}
} 