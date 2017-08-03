#include "Mesh.h"
#include <open/interface/RenderSystem/RenderEnumeration.h>
#include <open/interface/RenderSystem/BufferInterface.h>
#include <open/interface/RenderSystem/RenderSystemFactoryInterface.h>
#include <open/interface/RenderSystem/RenderInterface.h>

namespace open
{
	Mesh::IndexObject::~IndexObject()
	{
		if (dt == START_COUNT_IDT)
			return;

		delete index.ptr_index.p;
	}

	Mesh::Mesh()
		:_indexsNum(0),_autoBoundingBox(true), _firstDraw(true)
	{
		_vao = RenderSystemFactory::s_factory->createVertexArrayObject();
		_vertexStream = RenderSystemFactory::s_factory->createVertexStream();

		memset(&_swapBuffer, 0, sizeof(_swapBuffer));
	}

	Mesh::~Mesh()
	{
		///É¾³ývao
		delete[]((uint8*)_vao->getGPUBuffer()->getData());
		delete _vao;

		///É¾³ýË÷Òý
		IndexObject* index = _indexs.front();
		while (index)
		{
			freeIndexObject(index);
			index = _indexs.next(index);
		}
		_indexs.clear_destory();

		//É¾³ý»º´æbuffer
		freeVertexArray();
	}

	void Mesh::begin(uint32 vertexNum)
	{
		_swapBuffer.vertexSize = vertexNum;
	}

	void Mesh::setVertexArray(Vec3f* vertexs)
	{
		if (_swapBuffer.vertexs && vertexs != _swapBuffer.vertexs)
			delete[] _swapBuffer.vertexs;

		_swapBuffer.vertexs = vertexs;

		if (this->_autoBoundingBox)
		{
			_boudingBox = RBoundingBox();
			for (int32 i = 0; i < _swapBuffer.vertexSize; i++)
			{
				_boudingBox.expandBy(vertexs[i]);
			}
		}
	}

	void Mesh::setNormalArray(Vec3f* normals)
	{
		if (_swapBuffer.normals && normals != _swapBuffer.normals)
			delete[] _swapBuffer.normals;

		_swapBuffer.normals = normals;
	}

	void Mesh::setSkinArray(Skin* skins)
	{

	}
	void Mesh::setTextureCoordArray(uint32 index, Vec2f* coords)
	{
		if (_swapBuffer.coords[index] && coords != _swapBuffer.coords[index])
			delete[] _swapBuffer.coords[index];

		_swapBuffer.coords[index] = coords;
	}

	void Mesh::end()
	{
		_vertexStream->reset();

		uint64 vdt = IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT3,IVertexStream::VERXT_POSITOIN);

		_vertexStream->add(&vdt, 1);
		_vertexStream->addVerxtxData(_swapBuffer.vertexs, sizeof(float) * 3 * _swapBuffer.vertexSize);

		if (_swapBuffer.normals)
		{
			vdt = IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT3, IVertexStream::VERXT_NORMAL);

			_vertexStream->add(&vdt, 1);
			_vertexStream->addVerxtxData(_swapBuffer.normals, sizeof(float) * 3 * _swapBuffer.vertexSize);
		}
			
		///ÎÆÀí×ø±ê
		for (uint8 i = 0; i < 8; i++)
		{
			if (!_swapBuffer.coords[i])
				continue;

			vdt = IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT2, (IVertexStream::VertexUsage)(IVertexStream::VERXT_TEXTURECOORD0 + i));

			_vertexStream->add(&vdt, 1);
			_vertexStream->addVerxtxData(_swapBuffer.coords[i], sizeof(float) * 2 * _swapBuffer.vertexSize);
		}

		_vertexStream->accept(_vao, 0);
	}

	void Mesh::addIndexArray(DrawMode mode, uint16* indexs, uint32 size)
	{
		IndexObject* index = new IndexObject();
		index->mode = mode;
		index->dt = UINT16_IDT;
		index->index.ptr_index.p = RenderSystemFactory::s_factory->createVertexIndexObject(indexs, size);

		_indexs.push_back(index);
		_indexsNum++;
	}

	void Mesh::addIndexArray(DrawMode mode, uint32* indexs, uint32 size)
	{
		IndexObject* index = new IndexObject();
		index->mode = mode;
		index->dt = UINT32_IDT;
		index->index.ptr_index.p = RenderSystemFactory::s_factory->createVertexIndexObject(indexs, size);

		_indexs.push_back(index);
		_indexsNum++;
	}

	void Mesh::addIndexArray(DrawMode mode, uint8* indexs, uint32 size)
	{
		IndexObject* index = new IndexObject();
		index->mode = mode;
		index->dt = UINT8_IDT;
		index->index.ptr_index.p = RenderSystemFactory::s_factory->createVertexIndexObject(indexs, size);

		_indexs.push_back(index);
		_indexsNum++;
	}

	void Mesh::addIndexArray(DrawMode mode, uint32 first, uint32 count)
	{
		IndexObject* index = new IndexObject();
		index->mode = mode;
		index->dt = START_COUNT_IDT;
		index->index.start_count.start = first;
		index->index.start_count.count = count;

		_indexs.push_back(index);
		_indexsNum++;
	}

	void Mesh::releaseGPUResource()
	{
	}

	void Mesh::draw(IRenderPool* renderPool)
	{
		IndexObject* index = _indexs.front();
		while (index)
		{
			if (index->dt != START_COUNT_IDT)
			{
				renderPool->apply(_vao, index->index.ptr_index.p, index->mode, 1);
			}
			else
				renderPool->apply(_vao, index->index.start_count.start, index->index.start_count.count, index->mode, 1);

			index = _indexs.next(index);
		}
	}

	void Mesh::draw(IRenderPool* renderPool, IVertexStream* instanceStream)
	{
		instanceStream->accept(_vao, 3);

		IndexObject* index = _indexs.front();
		while (index)
		{
			if (index->dt != START_COUNT_IDT)
			{
				renderPool->apply(_vao, index->index.ptr_index.p, index->mode, instanceStream->getSize());
			}
			else
				renderPool->apply(_vao, index->index.start_count.start, index->index.start_count.count, index->mode, instanceStream->getSize());

			index = _indexs.next(index);
		}
	}

	int32 Mesh::getNumIndexArray()
	{
		return _indexsNum;
	}

	void Mesh::freeIndexObject(IndexObject* object)
	{
		switch (object->dt)
		{
		case UINT16_IDT:
			delete[](uint16*)(object->index.ptr_index.p->getGPUBuffer()->getData());
			break;
		case UINT32_IDT:
			delete[](uint32*)(object->index.ptr_index.p->getGPUBuffer()->getData());
			break;
		}
	}

	void Mesh::setIndexArray(uint32 index, DrawMode mode, uint16* indexs, uint32 size)
	{
		setIndexArray<uint16, DATATYPE_UNSIGNED_SHORT, UINT16_IDT>(index, mode, indexs, size);
	}

	void Mesh::setIndexArray(uint32 index, DrawMode mode, uint32* indexs, uint32 size)
	{
		setIndexArray<uint32, DATATYPE_UNSIGNED_INT, UINT32_IDT>(index, mode, indexs, size);
	}

	void Mesh::setIndexArray(uint32 index, DrawMode mode, uint8* indexs, uint32 size)
	{
		setIndexArray<uint8, DATATYPE_INT, UINT8_IDT>(index, mode, indexs, size);
	}

	void Mesh::setIndexArray(uint32 index, DrawMode mode, uint32 first, uint32 count)
	{
		if (index >= _indexsNum)
			return;

		IndexObject* object = getIndexObject(index);
		freeIndexObject(object);

		if (object->dt != START_COUNT_IDT)
			delete object->index.ptr_index.p;

		object->mode = mode;
		object->dt = START_COUNT_IDT;
		object->index.start_count.start = first;
		object->index.start_count.count = count;
	}

	void Mesh::accept(VisitGeometryVertex& dv)
	{
		GeometryVertex gv;
		gv.vertexs = _swapBuffer.vertexs;
		gv.size = _swapBuffer.vertexSize;

		IndexObject* index = _indexs.front();
		while (index)
		{
			gv.drawMode = index->mode;
			gv.indexType = index->dt;
			switch (index->dt)
			{
			case UINT16_IDT:
				gv.indexs.drawElements.indexs = index->index.ptr_index.p->getGPUBuffer()->getData();
				gv.indexs.drawElements.size = index->index.ptr_index.p->getGPUBuffer()->size() / sizeof(uint16);
				break;
			case UINT32_IDT:
				gv.indexs.drawElements.indexs = index->index.ptr_index.p->getGPUBuffer()->getData();
				gv.indexs.drawElements.size = index->index.ptr_index.p->getGPUBuffer()->size() / sizeof(uint32);
				break;
			case START_COUNT_IDT:
				gv.indexs.drawArray.start = index->index.start_count.start;
				gv.indexs.drawArray.count = index->index.start_count.count;
				break;
			}
			dv(gv, this);
			index = _indexs.next(index);
		}
	}
}