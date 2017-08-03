#include <open/interface/Render/DrawableInterface.h>
#include <open/interface/RenderSystem/BufferInterface.h>
#include <open/interface/RenderSystem/RenderEnumeration.h>
#include <vector>
#include "VertexArray.h"

namespace open
{
	inline uint8 getMemorySize(uint64 type)
	{
		type >>= 32;
		switch (type)
		{
		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_BYTE:
			return 1;
		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_SHORT:
			return 2;
		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_INT:
		case IVertexStream::VERXTEXDATATYPE_INT:
		case IVertexStream::VERXTEXDATATYPE_FLOAT:
			return 4;

		case IVertexStream::VERXTEXDATATYPE_INT2:
		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_INT2:
			return 8;
		case IVertexStream::VERXTEXDATATYPE_INT3:
		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_INT3:
			return 12;
		case IVertexStream::VERXTEXDATATYPE_INT4:
		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_INT4:
			return 16;
		case IVertexStream::VERXTEXDATATYPE_FLOAT2:
			return 8;
		case IVertexStream::VERXTEXDATATYPE_FLOAT3:
			return 12;
		case IVertexStream::VERXTEXDATATYPE_FLOAT4:
			return 16;
		default:
			return 0;
		}
	}

	inline uint8 getDataTypeMemorySize(DataType type)
	{
		switch (type)
		{
		case open::DATATYPE_UNSIGNED_BYTE:
			return 1;
			break;
		case open::DATATYPE_UNSIGNED_SHORT:
			return 2;
			break;
		case open::DATATYPE_UNSIGNED_INT:
			return 4;
			break;
		case open::DATATYPE_INT:
			return 4;
			break;
		case open::DATATYPE_FLOAT:
			return 4;
			break;
		default:
			return 0;
			break;
		}
	}

	inline void getDataSize(uint64 type, uint32& dataSize, uint32& num, DataType& dt)
	{
		dataSize = 0;
		num = 0;

		type >>= 32;
		switch (type)
		{
		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_BYTE:
			dataSize = 1;
			num = 1;
			dt = DataType::DATATYPE_UNSIGNED_BYTE;
			return;

		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_SHORT:
			dataSize = 1;
			num = 1;
			dt = DataType::DATATYPE_UNSIGNED_SHORT;
			return;

		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_INT:
			dataSize = 1;
			num = 1;
			dt = DataType::DATATYPE_UNSIGNED_INT;
			return;

		case IVertexStream::VERXTEXDATATYPE_INT:
			dataSize = 1;
			num = 1;
			dt = DataType::DATATYPE_INT;
			return;

		case IVertexStream::VERXTEXDATATYPE_FLOAT:
			dataSize = 1;
			num = 1;
			dt = DataType::DATATYPE_FLOAT;
			return;

		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_INT2:
			dt = DATATYPE_UNSIGNED_INT;
			dataSize = 2;
			num = 1;
			return;
		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_INT3:
			dt = DATATYPE_UNSIGNED_INT;
			dataSize = 3;
			num = 1;
			return;

		case IVertexStream::VERXTEXDATATYPE_UNSIGNED_INT4:
			dt = DATATYPE_UNSIGNED_INT;
			dataSize = 4;
			num = 1;
			return;

		case IVertexStream::VERXTEXDATATYPE_INT2:
			dt = DATATYPE_INT;
			dataSize = 2;
			num = 1;
			return;
		case IVertexStream::VERXTEXDATATYPE_INT3:
			dt = DATATYPE_INT;
			dataSize = 3;
			num = 1;
			return;

		case IVertexStream::VERXTEXDATATYPE_INT4:
			dt = DATATYPE_INT;
			dataSize = 4;
			num = 1;
			return;

		case IVertexStream::VERXTEXDATATYPE_FLOAT2:
			dt = DATATYPE_FLOAT;
			dataSize = 2;
			num = 1;
			return;

		case IVertexStream::VERXTEXDATATYPE_FLOAT3:
			dt = DATATYPE_FLOAT;
			dataSize = 3;
			num = 1;
			return;

		case IVertexStream::VERXTEXDATATYPE_FLOAT4:
			dt = DATATYPE_FLOAT;
			dataSize = 4;
			num = 1;
			return;

		default:
			return;
		}
	}

	int32 getVertexElementNum(const uint64* types, uint32 size)
	{
		uint32 dataSize;
		uint32 num;
		DataType dt;
		uint32 count = 0;
		for (size_t i = 0; i < size; i++)
		{
			getDataSize(types[i], dataSize, num, dt);
			count += num;
		}
		return count;
	}

	uint32 getMemorySize(const uint64* type, uint32 size)
	{
		int32 m_s = 0;
		for (size_t i = 0; i < size; i++)
		{
			m_s += getMemorySize(type[i]);
		}
		return m_s;
	}

	struct BuildVertexArrayObject
	{
		IVertexArrayObject* obj;
		uint32 gpuBuffer;

		BuildVertexArrayObject(IVertexArrayObject* o, uint32 gb)
			:obj(o), gpuBuffer(gb) {}

		void operator()(const uint64* types, uint32 size, uint32 startOffset, uint32 stride)
		{
			uint32 dataSize;
			uint32 num;
			DataType dt;
			uint32 offset = 0;

			if (size == 1)
			{
				getDataSize(types[0], dataSize, num, dt);
				if (num == 1)
				{
					obj->setVertexLayout(gpuBuffer, log2i(types[0] & 0x00000000ffffffff), dataSize, dt, startOffset, 0);
					return;
				}
			}

			for (size_t i = 0; i < size; i++)
			{
				uint64 type = types[i];
				getDataSize(type, dataSize, num, dt);
				int mem = getDataTypeMemorySize(dt);

				for (size_t j = 0; j < num; j++)
				{
					obj->setVertexLayout(gpuBuffer, log2i(type & 0x00000000ffffffff), dataSize, dt, startOffset + offset, stride);
					offset += dataSize * mem;
				}
			}
		}
	};


	VertexStream::VertexStream()
		:_count(0), _vertexElementNum(0), _memorySize(0), _vertexNum(0)
	{
		memset(_vdt, 0, sizeof(_vdt));
	}

	void VertexStream::reset()
	{
		_count = 0;
		_memorySize = 0;
		_vertexElementNum = 0;
		_vertexNum = 0;
		_size = 0;

		_preDataMemoryOffset.clear();
		datas.clear();
		memset(_vdt, 0, sizeof(_vdt));
	}

	void VertexStream::add(const uint64* type, uint32 size)
	{
		int32 ms = getMemorySize(type, size);

		_memorySize += ms;
		_vertexElementNum += getVertexElementNum(type, size);

		_vdt[_count++] = size;
		_vdt[_count++] = ms;

		memcpy(&(_vdt[_count]), type, sizeof(uint64) * size);
		_count += 2 * size;
	}

	void VertexStream::addVerxtxData(const void* data, uint32 size)
	{
		_preDataMemoryOffset.push_back(datas.size());

		datas.insert(datas.end(), (uint8*)data, (uint8*)data + size);
	}

	void VertexStream::aceept(Visitor v) const
	{
		uint32 position = 0;
		uint32 size = 0;
		uint32 mem = 0;
		int r = 0;
		while (true)
		{
			size = _vdt[position++];
			mem = _vdt[position++];

			if (size == 0)
				return;

			v((uint64*)(&(_vdt[position])), size, _preDataMemoryOffset[r], mem);
			position += 2 * size;

			r++;
		}
	}

	void VertexStream::accept(IVertexArrayObject* vao, uint32 index) const
	{
		if (!vaild())
			return;

		BuildVertexArrayObject build(vao, index);
		aceept(build);

		vao->getGPUBuffer(index)->setData(getBuffer(), getBufferSize());
	}

	void VertextArray::reset()
	{
		for (int32 i = 0; i < 4;i++)
		{
			_layouts[i].reset();
		}
	}

	void VertextArray::resetVertexNum(uint32 size)
	{
	}

	void VertextArray::accept(IVertexArrayObject * vao) const
	{
		uint32 gpuBuf = 0;
		for (int i = 0;i < 4; i++)
		{
			const VertexStream& ta = _layouts[i];
			if (!ta.vaild())
				continue;

			ta.accept(vao, gpuBuf++);
		}
	}
}