#include "GPUBuffer.h"
#include <open/interface/RenderSystem/RenderEnumeration.h>

#include "../GlState.h"
using namespace open::gl;

namespace open
{
	GPUBuffer::GPUBuffer(BufferUseType type)
		:_capacity(0), _size(0), _bufferID(0), _data(nullptr), _type(type)
	{
	}

	GPUBuffer::~GPUBuffer()
	{
		releaseGPUResource();
	}

	void GPUBuffer::releaseGPUResource()
	{
		if (!_bufferID)
			gl_DeleteBuffers(1, &_bufferID);
	}

	void GPUBuffer::genBuffer()
	{
		if (!_bufferID)
		{
			gl_GenBuffers(1, &_bufferID);
		}
	}

	uint32 GPUBuffer::convertType(BufferUseType type)
	{
		uint32 t = 0;
		switch (type)
		{
		case open::VERT_USE:
			t = OPEN_GL_ARRAY_BUFFER;
			break;
		case open::INDEX_USE:
			t = OPEN_GL_ELEMENT_ARRAY_BUFFER;
			break;
		case open::INSTANCED_USE:
			t = OPEN_GL_ARRAY_BUFFER;
			break;
		case open::UNIFORM_USE:
			break;
		case open::TEXTURE_USE:
			break;
		case open::PIXEL_PACK_USE:
			t = OPEN_GL_PIXEL_PACK_BUFFER;
			break;
		case open::PIXEL_UNPACK_USE:
			t = OPEN_GL_PIXEL_UNPACK_BUFFER;
			break;
		case open::OTHER_USE:
			break;
		default:
			break;
		}
		ASSERT_(t != 0);
		return t;
	}

	uint32 GPUBuffer::applyBuffer()
	{
		uint32 t = convertType(_type);
		gl_BindBuffer(t, _bufferID);

		return t;
	}

	void GPUBuffer::setBuffer(uint32 type, void* data, uint32 size)
	{
		if (!_dirty)
			return;
		_dirty = false;

		if (size > _capacity)
		{
			gl_BindBuffer(type, size, data, OPEN_GL_DYNAMIC_DRAW);
			_capacity = size;
		}
		else
		{
			void* gpuBuffer = gl_MapBufferRange(type, 0, size, OPEN_GL_MAP_WRITE_BIT);
			::memcpy(gpuBuffer, data, size);
			gl_UnmapBuffer(type);
		}
	}

	void GPUBuffer::setData(void* data, uint32 size)
	{
		if (!data || !size)
			return;

		_size = size;
		_data = data;
		_dirty = true;
	}

	void GPUBuffer::apply()
	{
		genBuffer();
		if (!checkBuffer())
			return;

		uint32 type = applyBuffer();
		setBuffer(type, _data, _size);
	}

#define CHECK_VAO_BUFFER_SIZE(index) ASSERT_(index >= 0 && index < 4);

	VertexArrayObject::VertexArrayObject()
		:_vao(0)
	{
		memset(_gpuBuffer, 0, sizeof(_gpuBuffer));
	}

	IGPUBuffer* VertexArrayObject::getGPUBuffer(uint32 index)
	{
		CHECK_VAO_BUFFER_SIZE(index);

		if (!_gpuBuffer[index])
		{
			if (index < 3)
				_gpuBuffer[index] = new GPUBuffer(VERT_USE);
			else
				_gpuBuffer[index] = new GPUBuffer(INSTANCED_USE);
		}
		return _gpuBuffer[index];
	}

	void VertexArrayObject::genVAO()
	{
		if (!_vao)
			gl_GenVertexArrays(1, &_vao);
	}

	void VertexArrayObject::releaseGPUResource()
	{
		if (!_vao)
			gl_DeleteVertexArrays(1, &_vao);
	}

	void VertexArrayObject::applyVAO()
	{
		gl_BindVertexArray(_vao);

		IGPUBuffer* gpuBuffer = nullptr;

		for (uint8 i = 0; i < 4; i++)
		{
			GPUBuffer* gpuBuffer = _gpuBuffer[i];
			if (!gpuBuffer)
				continue;

			gpuBuffer->apply();

			if (!_layerout.update(i))
				continue;

			bool instance = gpuBuffer->getUseType() == INSTANCED_USE ? true : false;
			const std::vector<uint8>& usages = _layerout.getUsages(i);

			for (uint32 j = 0; j < usages.size(); j++)
			{
				uint8 usage = usages[j];
				const VertexLayout::ElementContent& ele = _layerout.getVertexLayout(usage);
				if (!ele.vaild())
					continue;

				gl_EnableVertexAttribArray(usage);
				gl_VertexAttribPointer(usage, ele.elemetDateCount, ele.dataType, OPEN_GL_FALSE, ele.stride, (const void*)ele.offset);

				if (instance)
					gl_VertexAttribDivisor(usage, 1);
				else
					gl_VertexAttribDivisor(usage, 0);
			}
		}
	}

	void VertexArrayObject::getVertexLayout(uint32 index, uint32 vertexUsage, uint32& size, DataType& dataType, uint32& offset, uint32& stride)
	{
		CHECK_VAO_BUFFER_SIZE(index);

		const VertexLayout::ElementContent& e = _layerout.getVertexLayout(vertexUsage);
		size = e.elemetDateCount;
		dataType = (DataType)e.dataType;
		offset = e.offset;
		stride = e.stride;
	}

	void VertexArrayObject::setVertexLayout(uint32 index, uint32 vertexUsage, uint32 size, DataType dataTypes, uint32 offset, uint32 stride)
	{
		CHECK_VAO_BUFFER_SIZE(index);
		_layerout.set(index, vertexUsage, dataTypes, size, offset, stride);
	}

	void VertexArrayObject::apply()
	{
		genVAO();

		if (!checkVAO())
			return;

		applyVAO();
	}

	VertexArrayObject::~VertexArrayObject()
	{
		releaseGPUResource();

		delete _gpuBuffer[0];
		delete _gpuBuffer[1];
		delete _gpuBuffer[2];
		delete _gpuBuffer[3];
	}

	VertexIndexObject::VertexIndexObject()
		:_type(DATATYPE_UNSIGNED_SHORT)
	{
		_gpuBuffer = new GPUBuffer(INDEX_USE);
	}

	VertexIndexObject::VertexIndexObject(uint16* data, int32 count)
		: _type(DATATYPE_UNSIGNED_SHORT)
	{
		_gpuBuffer = new GPUBuffer(INDEX_USE);
		_gpuBuffer->setData(data, sizeof(uint16)* count);
	}

	VertexIndexObject::VertexIndexObject(uint32* data, int32 count)
		:_type(DATATYPE_UNSIGNED_INT)
	{
		_gpuBuffer = new GPUBuffer(INDEX_USE);
		_gpuBuffer->setData(data, sizeof(uint32)* count);
	}

	VertexIndexObject::VertexIndexObject(uint8* data, int32 count)
		:_type(DATATYPE_INT)
	{
		_gpuBuffer = new GPUBuffer(INDEX_USE);
		_gpuBuffer->setData(data, sizeof(uint8)* count);
	}

	VertexIndexObject::~VertexIndexObject()
	{
		delete _gpuBuffer;
	}

	void VertexIndexObject::releaseGPUResource()
	{
	}

	void VertexIndexObject::apply()
	{
		_gpuBuffer->apply();
	}
}