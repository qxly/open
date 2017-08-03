#ifndef RENDERSYSTEM_FRAMEATTACHMENT_H
#define RENDERSYSTEM_FRAMEATTACHMENT_H 1

#include <RenderSystem/RenderInterface.h>
#include <open/Memory.h>

namespace open
{
	class RenderBuffer : public IRenderBuffer PUBLIC_OBJECT_DEBUG
	{
	public:
		RenderBuffer()
			:_renderBuffer(0),_width(0),_height(0),_dirty(false){}

		RenderBuffer(int32 width,int32 height,PixelInternalFormat format) 
			:_renderBuffer(0),_width(width),_height(height),_dirty(true),_format(format){}

		virtual void alloc(int32 width,int32 height,PixelInternalFormat format) 
		{
			_format = format;
			_width = width; 
			_height = height; 
			_dirty = true;
		}

		virtual int32 getWidth() { return _width;}

		virtual int32 getHeight() { return _height;}

		virtual uint32 getRenderBufferID() { return _renderBuffer;}

		void apply();

		virtual void releaseGPUContent();

	private:
		int32 _width;
		int32 _height;
		PixelInternalFormat _format;

		bool _dirty;

		uint32 _renderBuffer;
	};

	class FrameBufferAttachmentInternal
	{
	public:
		FrameBufferAttachmentInternal();

		FrameBufferAttachmentInternal(IRenderBuffer* renderBuffer);

		FrameBufferAttachmentInternal(ITexture* texture,uint32 mipLevel = 0);

		~FrameBufferAttachmentInternal();

		struct RenderAttachmentStorage;
		RenderAttachmentStorage* _storage;
	};

	class FrameBuffer : public IFrameBuffer PUBLIC_OBJECT_DEBUG
	{
	private:
		enum Update : uint8
		{
			UPDATE_DRAWBUFFER = 1,
			CLEAR_ATTACHMENTPOINTS = 1 << 2
		};

	public:

		FrameBuffer();

		~FrameBuffer();

		virtual void attach(FrameBufferAttachment ap,IRenderBuffer* renderable);

		virtual void attach(FrameBufferAttachment ap,ITexture* texture);

		virtual void detach(FrameBufferAttachment ap);

		virtual void detach();

		bool vaild();

		virtual void apply();

		virtual void releaseGPUContent();

		virtual int32 getWidth();

		virtual int32 getHeight();

	private:

		void reset();

		void updateDrawBuffer();

		void attach(FrameBufferAttachment ap,FrameBufferAttachmentInternal* fba);

		FrameBufferAttachment convertToFrameBufferAttachment(int index);

	private:

		FrameBufferAttachmentInternal* _attachmentMap[19] ;

		uint32 _drawBuffers[18];
		int32 _drawBufferSize;

		uint32 _frameBuffer;

		uint32 _updateState;
	};
}

#endif