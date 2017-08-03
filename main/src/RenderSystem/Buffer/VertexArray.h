#include <open/interface/RenderSystem/BufferInterface.h>
#include <functional>
#include <vector>

namespace open
{
	class VertexStream : public IVertexStream
	{
	public:

		typedef std::function<void(const uint64*, uint32,uint32,uint32)> Visitor;

		VertexStream();

		inline bool vaild() const { return _count != 0; }

		inline void* getBuffer() const { return (void*)(&(datas.front())); }

		inline uint32 getBufferSize() const { return datas.size(); }

		virtual void reset();

		virtual void add(const uint64* type, uint32 size);

		virtual void addVerxtxData(const void* data, uint32 size);

		virtual void setSize(uint32 size) { _size = size; }

		virtual uint32 getSize() { return _size; }

		virtual void accept(IVertexArrayObject* vao, uint32 index) const;

		void VertexStream::aceept(Visitor v) const;

	private:
		uint32 _count;
		uint32 _memorySize;
		uint32 _vertexElementNum;

		uint32 _size = 0;

		uint32 _vdt[64];
		uint32 _vertexNum;

		std::vector<uint8> datas;
		std::vector<uint32> _preDataMemoryOffset;
	};

	class VertextArray : public IVertextArray
	{
	public:

		virtual void reset();

		virtual void resetVertexNum(uint32 num);

		virtual IVertexStream* getVertexStream(uint32 index) { ASSERT_(index >= 0 && index < 3); return &(_layouts[index]); }

		virtual IVertexStream* getInstanceStream() { return &(_layouts[3]); }

		virtual void accept(IVertexArrayObject* vao) const;

	private:
		VertexStream _layouts[4];
	};
}