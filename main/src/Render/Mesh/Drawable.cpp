#include "Drawable.h"
#include <open/interface/Render/MaterialInterface.h>

namespace open
{
	struct Geometry_Material
	{
		IGeometry* geometry;
		IMaterial* material;

		inline void init(IGeometry* geometry_, IMaterial* material_) {
			geometry = geometry_;
			material = material_;
			addRef();
		}

		inline void addRef()
		{
			ASSERT_(geometry);
			ASSERT_(material);

			geometry->addRef();
			material->addRef();
		}

		inline void release()
		{
			ASSERT_(geometry);
			ASSERT_(material);

			geometry->release();
			material->release();
		}

		inline void replaceMaterial(IMaterial* material_)
		{
			ASSERT_(material);
			material_->addRef();
			material->release();
			material = material_;
		}
	};

	struct Drawable::GeometryMaterialArray
	{
		typedef std::vector<Geometry_Material> Geometry_Materials;
		Geometry_Materials gms;

		inline size_t size() const { return gms.size(); }

		inline Geometry_Material& operator [] (uint32 index) { return gms[index]; }

		inline const Geometry_Material& operator [] (uint32 index) const { return gms[index]; }

		inline void push_back(const Geometry_Material& gm) { gms.push_back(gm); }

		inline void erase(uint32 index) { gms.erase(gms.begin() + index); }
	};

	Drawable::Drawable()
		:_resetBoudingBox(true), _bone(nullptr)
	{
		_geometryMaterials = new GeometryMaterialArray;
	}

	Drawable::~Drawable()
	{
		for (size_t i = 0; i < _geometryMaterials->size(); i++)
		{
			(*_geometryMaterials)[i].release();
		}
	}

	void Drawable::setBone(IBone* bone)
	{
	}

	IBone* Drawable::getBone()
	{
		return _bone;
	}

	uint32 Drawable::getNumGeometry()
	{
		return _geometryMaterials->size();
	}

	IGeometry* Drawable::getGeometry(uint32 index)
	{
		if (index >= _geometryMaterials->size())
			return nullptr;

		return (*_geometryMaterials)[index].geometry;
	}

	IMaterial* Drawable::getMaterial(uint32 index)
	{
		if (index >= _geometryMaterials->size())
			return nullptr;

		return (*_geometryMaterials)[index].material;
	}

	void Drawable::addGeometry(IGeometry* geometry, IMaterial* material)
	{
		Geometry_Material gm;
		gm.init(geometry, material);
		_geometryMaterials->push_back(gm);
	}

	void Drawable::setMaterial(uint32 index, IMaterial* material)
	{
		if (index >= _geometryMaterials->size())
			return;

		(*_geometryMaterials)[index].replaceMaterial(material);
	}

	void Drawable::removeGeometry(uint32 index)
	{
		if (index >= _geometryMaterials->size())
			return;

		(*_geometryMaterials)[index].release();
		_geometryMaterials->erase(index);
	}

	void Drawable::computeBounding()
	{
		_boudingBox.init();
		int32 size = _geometryMaterials->size();
		for (int32 i = 0; i < size; i++)
		{
			_boudingBox.expandBy((*_geometryMaterials)[i].geometry->getBoudingBox());
		}
	}

	const RBoundingBox& Drawable::getBoudingBox()
	{
		if (_resetBoudingBox)
		{
			_resetBoudingBox = false;
			computeBounding();
		}
		return _boudingBox;
	}

	void Drawable::accept(VisitGeometryVertex dv)
	{
		int32 size = _geometryMaterials->size();
		for (int32 i = 0; i < size; i++)
		{
			IGeometry* geometry = (*_geometryMaterials)[i].geometry;
			geometry->accept(dv);
		}
	}

	void Drawable::accept(VisitGeometry vg)
	{
		int32 size = _geometryMaterials->size();
		for (int32 i = 0; i < size; i++)
		{
			const Geometry_Material& gm = (*_geometryMaterials)[i];
			vg(gm.geometry, gm.material,nullptr);
		}
	}

	std::vector<DrawableReadWriteHelper*> DrawableReadWriteHelper::s_helpers = std::vector<DrawableReadWriteHelper*>();
	IDrawable* DrawableReadWriteHelper::readDrawable(const char* type, const char* buffer, uint32 size, const Config& config)
	{
		for (size_t i = 0; i < s_helpers.size(); i++)
		{
			if (strcmp(type, s_helpers[i]->support()) == 0)
			{
				return s_helpers[i]->readDrawable(buffer, size, config);
			}
		}
		return nullptr;
	}

	IDrawable* DrawableReadWriteHelper::readDrawable(const char* file, const Config& config)
	{
		FILE* f;
		char* content = nullptr;
		size_t count = 0;

		f = fopen(file, "rt");

		if (f != nullptr)
		{
			fseek(f, 0, SEEK_END);
			count = ftell(f);
			rewind(f);

			if (count > 0)
			{
				content = new char[sizeof(char)* (count)];
				count = fread(content, sizeof(char), count, f);
			}
			fclose(f);

			return readDrawable("obj", content, count, config);
		}
		return nullptr;
	}

	void DrawableReadWriteHelper::unstall()
	{
		for (uint32 i = 0; i < s_helpers.size(); i++)
		{
			delete s_helpers[i];
		}
	}

	bool DrawableReadWriteHelper::registe(DrawableReadWriteHelper* helper)
	{
		s_helpers.push_back(helper);
		return true;
	}
}