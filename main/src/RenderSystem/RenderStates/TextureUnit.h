#ifndef RENDERSYSTEM_TEXTUREUNIT_H
#define RENDERSYSTEM_TEXTUREUNIT_H

#include <open/Ptr.h>
#include <open/Memory.h>
#include <string>
#include <RenderSystem/RenderStatesInterface.h>

namespace open
{
	enum TextureFilterMode : uint16;
	class TexureUnitParame
	{
	public:

		TextureWrapMode _wrap_s;
		TextureWrapMode _wrap_t;
		TextureWrapMode _wrap_r;

		TextureFilterMode _min_filter;
		TextureFilterMode _mag_filter;

		Real4 _borderColor;

		TexureUnitParame();

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

	class TextureUnit : public TPtr<ITextureUnit>, public TexureUnitParame  PUBLIC_OBJECT_DEBUG
	{
	public:
		TextureUnit();

		~TextureUnit();

		virtual void setTexture(ITexture* texture) { _texture = texture; }

		virtual void setWrap(TextureWrapParameter which, TextureWrapMode wrap);

		virtual void setFilter(TextureFilterParameter which, TextureFilterMode mode);

		virtual void setBorderColor(const Real4& color) { _borderColor = color; }

		virtual void setMatrix(const RMatrix& matrix) { _textureMatrix = matrix; }

		virtual void enable(bool value) { _enable = value; }

		virtual bool getEnable() { return _enable; }

		virtual ITexture* getTexture() { return _texture; }

		virtual TextureWrapMode getWrap(TextureWrapParameter which);

		virtual TextureFilterMode getFilter(TextureFilterParameter which);

		virtual const Real4& getBorderColor() { return _borderColor; }

		virtual const RMatrix& getMatrix() { return _textureMatrix; }

		virtual bool apply(int32 unit);

	private:

		Ptr<ITexture> _texture;
		RMatrix _textureMatrix;
		bool _enable;
	};
}

#endif