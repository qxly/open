#ifndef RENDERSYSTEM_TEXTUREUNIT_H
#define RENDERSYSTEM_TEXTUREUNIT_H

#include <common/Ptr.h>
#include <common/Memory.h>
#include <RenderSystem/RenderStatesInterface.h>

namespace open
{
	class TexureUnitParame
	{
	public:

		TextureWrapMode _wrap_s;
		TextureWrapMode _wrap_t;
		TextureWrapMode _wrap_r;

		TextureFilterMode _min_filter;
		TextureFilterMode _mag_filter;

		Real4 _borderColor;

		TexureUnitParame()
			:_wrap_s(CLAMP), _wrap_t(CLAMP), _wrap_r(CLAMP), _min_filter(LINEAR), _mag_filter(LINEAR){}

		inline bool operator == (const TexureUnitParame& other)
		{
			return (_wrap_s == other._wrap_s && _wrap_t == other._wrap_t && _wrap_r == other._wrap_r
				&& _min_filter == other._min_filter && _mag_filter == other._mag_filter);
		}

		inline bool operator != (const TexureUnitParame& other)
		{
			return (_wrap_s != other._wrap_s || _wrap_t != other._wrap_t || _wrap_r != other._wrap_r
				|| _min_filter != other._min_filter || _mag_filter != other._mag_filter);
		}
	};

	class TextureUnit : public ITextureUnit, public TexureUnitParame PUBLIC_OBJECT_DEBUG
	{
	public:
		TextureUnit();

		~TextureUnit();

		void setName(const char* name)
		{
			delete[] _name;
			int32 size = strlen(name);
			_name = new char[size + 1];
			_name[size] = '\0';
			memcpy(_name, name, size);
		}

		virtual const char* getName() { return _name; }

		virtual void setTextureLayer(TextureLayer layer) { _textureLayer = layer; }

		virtual void setTexture(ITexture* texture) { _texture = texture; }

		virtual void setWrap(TextureWrapParameter which, TextureWrapMode wrap);

		virtual void setBorderColor(const Real4& color) { _borderColor = color; }

		virtual void setMatrix(const RMatrix& matrix) { _textureMatrix = matrix; }

		virtual ITexture* getTexture() { return _texture; }

		virtual TextureLayer getTextureLayer() { return _textureLayer; }

		virtual TextureWrapMode getWrap(TextureWrapParameter which);

		virtual const Real4& getBorderColor() { return _borderColor; }

		virtual const RMatrix& getMatrix() { return _textureMatrix; }

		virtual bool apply(int32 unit);

		static void initTextureUnitState();
	private:
		Ptr<ITexture> _texture;
		RMatrix _textureMatrix;

		TextureLayer _textureLayer;

		char* _name;

		static TexureUnitParame s_templateTexureUnitParame[16];
	};
}

#endif