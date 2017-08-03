#ifndef MESH_MESH_H
#define MESH_MESH_H

#include <open/Ptr.h>
#include <open/Memory.h>
#include <open/IntrusiveList.h>
#include <open/interface/Render/DrawableInterface.h>
#include <string.h>

namespace open
{
	class IVertexArrayObject;
	class IVertexIndexObject;
	enum DrawMode : uint8;
	enum DataType : uint16;

	class Mesh : public TPtr<IMesh> PUBLIC_OBJECT_DEBUG
	{
	private:
		struct VertexBufferArray
		{
			uint32 vertexSize;

			Vec3f* vertexs;
			Vec3f* normals;
			Vec2f* coords[8];
		};
		enum IndexDateType
		{
			UINT16_IDT,
			UINT32_IDT,
			UINT8_IDT,
			START_COUNT_IDT
		};
		struct IndexObject
		{
			union Index
			{
				struct ptr
				{
					IVertexIndexObject* p;
				}ptr_index;
				struct start_size
				{
					uint32 start;
					uint32 count;
				}start_count;
			};

			DrawMode mode;
			IndexDateType dt;
			Index index;

			~IndexObject();

			TLink<IndexObject> link;
		};

	public:

		Mesh();

		~Mesh();

		virtual void autoBoundingBox(bool value) { _autoBoundingBox = value; }

		virtual bool isAutoBoundingBox() { return _autoBoundingBox; }

		virtual void setBoudingBox(const RBoundingBox& box) { _boudingBox = box; }

		virtual const RBoundingBox& getBoudingBox() { return _boudingBox; }

		virtual void begin(uint32 vertexNum);

		virtual void setVertexArray(Vec3f* vertexs);

		virtual void setNormalArray(Vec3f* normals);

		virtual void setSkinArray(Skin* skins);

		virtual void setTextureCoordArray(uint32 index, Vec2f* coords);

		virtual void end();

		virtual int32 getNumIndexArray();

		virtual void addIndexArray(DrawMode mode, uint16* indexs, uint32 size);

		virtual void addIndexArray(DrawMode mode, uint32* indexs, uint32 size);

		virtual void addIndexArray(DrawMode mode, uint8* indexs, uint32 size);

		virtual void addIndexArray(DrawMode mode, uint32 first, uint32 count);

		virtual void setIndexArray(uint32 index, DrawMode mode, uint16* indexs, uint32 size);

		virtual void setIndexArray(uint32 index, DrawMode mode, uint32* indexs, uint32 size);

		virtual void setIndexArray(uint32 index, DrawMode mode, uint8* indexs, uint32 size);

		virtual void setIndexArray(uint32 index, DrawMode mode, uint32 first, uint32 count);

		void releaseGPUResource();

		virtual void draw(IRenderPool* renderPool);

		virtual void draw(IRenderPool* renderPool,IVertexStream* instanceStream);

		virtual void accept(VisitGeometryVertex& dv);

	private:
		void freeVertexArray()
		{
			if (_swapBuffer.vertexs)
				delete[] _swapBuffer.vertexs;
			if (_swapBuffer.normals)
				delete[] _swapBuffer.normals;

			for (uint8 i = 0; i < 8; i++)
			{
				if (_swapBuffer.coords[i])
					delete[] _swapBuffer.coords[i];
			}
		}

		inline IndexObject* getIndexObject(uint32 index)
		{
			IndexObject* object = _indexs.front();
			while (index > 0)
			{
				object = _indexs.next(object);
				index--;
			}
			return object;
		}

		void freeIndexObject(IndexObject* object);

		template<typename T, DataType dt, IndexDateType IDT>
		void setIndexArray(uint32 index, DrawMode mode, T* indexs, uint32 size)
		{
			if (index >= _indexsNum)
				return;

			IndexObject* object = getIndexObject(index);

			freeIndexObject(object);
			if (object->dt == START_COUNT_IDT)
				object->index.ptr_index.p = RenderSystemFactory::s_factory->createVertexIndexObject();

			object->mode = mode;
			object->dt = IDT;
			object->index.ptr_index.p->setDataType(dt);
			object->index.ptr_index.p->getGPUBuffer()->setData(indexs, sizeof(T) * size);
		}

	private:
		IVertexArrayObject* _vao;
		RBoundingBox _boudingBox;

		INTRUSIVELIST_DECLARE(IndexObject, link) _indexs;
		uint32 _indexsNum;

		VertexBufferArray _swapBuffer;
		IVertexStream* _vertexStream;

		bool _autoBoundingBox;
		bool _firstDraw;
	};
}
#endif