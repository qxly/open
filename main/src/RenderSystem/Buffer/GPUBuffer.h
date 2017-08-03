#ifndef RENDERSYSTEM_GPUBUFFER_H
#define RENDERSYSTEM_GPUBUFFER_H

#include <vector>

#include "../GPUResource.h"
#include <open/interface/RenderSystem/BufferInterface.h>
#include <open/IntrusiveList.h>
#include <open/Memory.h>

namespace open
{
	static const uint32 MAX_ELEMENT_SIZE = 32;
	static const int MAX_STREAM_SIZE = 4;

	class GPUBuffer : public IGPUBuffer, public IGPUResource PUBLIC_OBJECT_DEBUG
	{
	public:

		GPUBuffer(BufferUseType type);

		~GPUBuffer();

		virtual void setData(void* data, uint32 size);

		virtual uint32 size() { return _size; }
		virtual void* getData() { return _data; }

		virtual BufferUseType getUseType() { return _type; }

		virtual void apply();

		void releaseGPUResource();

	private:

		void setBuffer(uint32 type, void* data, uint32 size);

		void genBuffer();

		uint32 applyBuffer();

		bool checkBuffer() { return _bufferID != 0; }

		uint32 convertType(BufferUseType type);

	private:
		uint32 _bufferID;
		uint32 _capacity;
		uint32 _size;
		void* _data;

		bool _dirty = false;

		BufferUseType _type;
	};

	class VertexArrayObject : public IVertexArrayObject PUBLIC_OBJECT_DEBUG
	{
	public:
		VertexArrayObject();
		~VertexArrayObject();

		virtual IGPUBuffer* getGPUBuffer(uint32 index = 0);

		virtual void getVertexLayout(uint32 index, uint32 vertexUsage, uint32& size, DataType& dataTypes, uint32& offset, uint32& stride);

		virtual void setVertexLayout(uint32 index, uint32 vertexUsage, uint32 size, DataType dataTypes, uint32 offset, uint32 stride);

		virtual void apply();

		void releaseGPUResource();

	private:

		void genVAO();

		void applyVAO();

		bool checkVAO() { return _vao != 0; }

	private:

		class VertexLayout
		{
		public:
			struct ElementContent
			{
				uint16 dataType;
				uint8 elemetDateCount;
				uint32 offset;
				uint32 stride;

				ElementContent()
					:elemetDateCount(0), dataType(0), offset(0), stride(0){}

				inline bool vaild() const { return elemetDateCount > 0; }
			};


		public:

			VertexLayout()
			{
				memset(_update, 0, sizeof(_update));
			}

			inline void set(uint32 streamIndex, uint32 usage, DataType dataType_, uint32 elemetDateCount_, uint32 offset_, uint32 stride_)
			{
				ASSERT_(streamIndex < MAX_STREAM_SIZE);
				ASSERT_(usage < MAX_ELEMENT_SIZE);

				ElementContent& content = _elementContents[usage];

				if(content.dataType != dataType_ || content.elemetDateCount != elemetDateCount_ ||
					content.offset != offset_ || content.stride != stride_)
				{
					content.dataType = (uint16)dataType_;
					content.elemetDateCount = (uint8)elemetDateCount_;
					content.offset = offset_;
					content.stride = stride_;
					_update[streamIndex] = true;

					for (uint32 i = 0; i < _streamFlag[streamIndex].size();i++)
					{
						if (_streamFlag[streamIndex][i] == usage)
							return;
					}
					_streamFlag[streamIndex].push_back(usage);
				}
			}

			inline uint32 getVertexElementNum(int32 streamIndex) const
			{ 
				ASSERT_(streamIndex < MAX_STREAM_SIZE);
				_streamFlag[streamIndex].size();
			}

			inline const ElementContent& getVertexLayout(uint32 usage) const
			{
				return _elementContents[usage];
			}

			const std::vector<uint8>& getUsages(uint32 streamIndex) const { return _streamFlag[streamIndex]; }

			inline bool update(uint32 index) { if (_update[index]) { _update[index] = false; return true; } return false; }

			inline void release() {}

		private:

			ElementContent _elementContents[MAX_ELEMENT_SIZE];
			std::vector<uint8> _streamFlag[MAX_STREAM_SIZE];
			bool _update[MAX_STREAM_SIZE];
		};

		GPUBuffer* _gpuBuffer[MAX_STREAM_SIZE];
		VertexLayout _layerout;

		uint32 _vao;
	};

	class VertexIndexObject : public IVertexIndexObject PUBLIC_OBJECT_DEBUG
	{
	public:

		VertexIndexObject();

		VertexIndexObject(uint16* data, int32 count);

		VertexIndexObject(uint32* data, int32 count);

		VertexIndexObject(uint8* data, int32 count);

		~VertexIndexObject();

		virtual IGPUBuffer* getGPUBuffer() { return _gpuBuffer; }

		virtual void setDataType(DataType dataType) { _type = dataType; }

		virtual DataType getDataType() { return _type; }

		void apply();

		void releaseGPUResource();
	private:
		DataType _type;
		GPUBuffer* _gpuBuffer;
	};
}
#endif