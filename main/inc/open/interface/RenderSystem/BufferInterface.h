#ifndef RENDERSYSTEM_BUFFERINTERFACE_H
#define RENDERSYSTEM_BUFFERINTERFACE_H

#include <open/openDef.h>

namespace open
{
	enum BufferUseType : uint8;
	enum DataType : uint16;

	class IGPUBuffer
	{
	public:
		virtual ~IGPUBuffer() {}

		virtual void setData(void* data, uint32 size) = 0;

		virtual uint32 size() = 0;
		virtual void* getData() = 0;

		virtual BufferUseType getUseType() = 0;
	};

	class IVertexArrayObject
	{
	public:
		virtual ~IVertexArrayObject() {}

		virtual IGPUBuffer* getGPUBuffer(uint32 index = 0) = 0;

		virtual void getVertexLayout(uint32 index, uint32 vertexUsage, uint32& size, DataType& dataTypes, uint32& offset, uint32& strides) = 0;

		virtual void setVertexLayout(uint32 index, uint32 vertexUsage, uint32 size, DataType dataTypes, uint32 offset, uint32 stride) = 0;
	};

	class IVertexIndexObject
	{
	public:
		virtual ~IVertexIndexObject() {}

		virtual IGPUBuffer* getGPUBuffer() = 0;
		virtual void setDataType(DataType dataType) = 0;
		virtual DataType getDataType() = 0;
	};

	class IVertexStream
	{
	public:
		enum VerxtexDataType
		{
			VERXTEXDATATYPE_UNSIGNED_BYTE = 1 << 1,
			VERXTEXDATATYPE_UNSIGNED_SHORT = 1 << 2,
			VERXTEXDATATYPE_UNSIGNED_INT = 1 << 3,
			VERXTEXDATATYPE_INT = 1 << 4,
			VERXTEXDATATYPE_FLOAT = 1 << 5,

			VERXTEXDATATYPE_UNSIGNED_INT2 = 1 << 6,
			VERXTEXDATATYPE_UNSIGNED_INT3 = 1 << 7,
			VERXTEXDATATYPE_UNSIGNED_INT4 = 1 << 8,

			VERXTEXDATATYPE_INT2 = 1 << 9,
			VERXTEXDATATYPE_INT3 = 1 << 10,
			VERXTEXDATATYPE_INT4 = 1 << 11,

			VERXTEXDATATYPE_FLOAT2 = 1 << 12,
			VERXTEXDATATYPE_FLOAT3 = 1 << 13,
			VERXTEXDATATYPE_FLOAT4 = 1 << 14,
		};
		enum VertexUsage
		{
			VERXT_POSITOIN = 0,
			VERXT_NORMAL = 1 << 1,
			VERXT_COOR = 1 << 2,

			VERXT_TEXTURECOORD0 = 1 << 3,
			VERXT_TEXTURECOORD1 = 1 << 4,
			VERXT_TEXTURECOORD2 = 1 << 5,
			VERXT_TEXTURECOORD3 = 1 << 6,

			VERXT_INSTANCE0 = 1 << 7,
			VERXT_INSTANCE1 = 1 << 8,
			VERXT_INSTANCE2 = 1 << 9,
			VERXT_INSTANCE3 = 1 << 10,
			VERXT_INSTANCE4 = 1 << 11,
			VERXT_INSTANCE5 = 1 << 12,
			VERXT_INSTANCE6 = 1 << 13,
			VERXT_INSTANCE7 = 1 << 14,
		};

		static uint64 genVertexType(VerxtexDataType type, VertexUsage usage) { return (uint64)type << 32 | usage; }

		virtual ~IVertexStream() {}

		virtual void reset() = 0;

		virtual void add(const uint64* vertexType, uint32 size) = 0;

		virtual void addVerxtxData(const void* data, uint32 size) = 0;

		virtual void setSize(uint32 size) = 0;

		virtual uint32 getSize() = 0;

		virtual void accept(IVertexArrayObject* vao, uint32 index) const = 0;
	};

	class IVertextArray
	{
	public:

		virtual ~IVertextArray() {}

		virtual void reset() = 0;

		virtual void resetVertexNum(uint32 num) = 0;

		virtual IVertexStream* getVertexStream(uint32 index) = 0;

		virtual IVertexStream* getInstanceStream() = 0;

		virtual void accept(IVertexArrayObject* vao) const = 0;
	};
}

#endif