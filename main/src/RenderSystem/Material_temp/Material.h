#ifndef RENDERSYSTEM_MATERAI_H
#define RENDERSYSTEM_MATERAI_H 1

#include <RenderSystem/RenderStatesInterface.h>
#include <vector>

#include "TextureUnitSet.h"
#include <common/Ptr.h>
#include <common/Memory.h>

#include <map>
#include <string>

struct CmpChar
{
	bool operator ()(const char* l, const char* r) const
	{
		return std::less<std::string>()(std::string(l), std::string(r));
	}
};

namespace open
{
	class RenderPool;

	class Pass : public IPass  PUBLIC_OBJECT_DEBUG
	{
	public:
		Pass(){}

		virtual void useDefaultRender() { _default = true; }

		virtual bool isUseDefaultRender() { return _default; }

		virtual void setRenderSchema(const char* renderSchema)
		{
			_renderSchema = renderSchema;
		}

		virtual const char* getRenderStep()
		{
			return _renderSchema.c_str();
		}

		virtual void destoryStateSet()
		{
			delete _renderStateSet;
			_renderStateSet = nullptr;
		}

		virtual RenderStateSet* getRenderStateSet() { return _renderStateSet; }

		virtual RenderStateSet* getOrCreateRenderStateSet()
		{
			if (!_renderStateSet)
				_renderStateSet = new RenderStateSet;
			return  _renderStateSet;
		}

		virtual ITextureUnitSet* getTextureUnitSet() { return _textures; }

		virtual ITextureUnitSet* getOrCreateTextureUnitSet()
		{
			if (!_textures)
				_textures = new TextureUnitSet;
			return  _textures;
		}

		~Pass()
		{
			if (_renderStateSet)
				delete _renderStateSet;
		}
	private:
		std::string _renderSchema;
		bool _default = false;

		RenderStateSet* _renderStateSet = nullptr;
		ITextureUnitSet* _textures = nullptr;
	};

	class Material : public IMaterial PUBLIC_OBJECT_DEBUG
	{
	public:

		DEFINE_REFCOUNT;

	public:
		Material()
			:_refCount(0)
		{
		}

		~Material()
		{
			if (_textures)
				delete _textures;
		}

		virtual RenderStateSet& getRenderStateSet() { return _rss; }

		virtual ITextureUnitSet* getTextureUnitSet() { return _textures; }

		inline bool isTransparent() { return _rss.getMode(BLEND); }


		virtual void setName(const char* name)
		{
			_name = name;
		}

		virtual IPass* createPass()
		{
			_passSet.push_back(new Pass);
			return _passSet.back();
		}

		virtual bool removePass(IPass* pass)
		{
			for (std::vector<IPass*>::iterator it = _passSet.begin(); it != _passSet.end(); ++it)
			{
				if (*it != pass)
					continue;

				_passSet.erase(it);
				return true;
			}
			return false;
		}

		virtual int32 getNumPass()
		{
			return _passSet.size();
		}

		virtual IPass* getPass(int32 index)
		{
			ASSERT_(index >= 0 && index < (int32)_passSet.size());
			return _passSet[index];
		}

		virtual IMaterial* clone()
		{
			Material* material = new Material;
			material->_rss = _rss;

			return material;
		}
	public:
		static Material* getOrCreateMaterial(const char* materialName);

	private:
		typedef std::map<const char*, Material*, CmpChar> MaterailMap;
		static MaterailMap s_materailMap;

	private:

		RenderStateSet _rss;

		ITextureUnitSet* _textures = new TextureUnitSet;

		std::string _name;

		std::vector<IPass*> _passSet;
	};
}

#endif