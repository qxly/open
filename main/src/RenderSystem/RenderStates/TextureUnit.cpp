#include "TextureUnit.h"
#include <open/interface/RenderSystem/RenderEnumeration.h>

#include "../GlState.h"
using namespace open::gl;

namespace open
{
	TexureUnitParame::TexureUnitParame()
		:_wrap_s(CLAMP), _wrap_t(CLAMP), _wrap_r(CLAMP), _min_filter(LINEAR), _mag_filter(LINEAR) {}

	TextureUnit::TextureUnit()
		: _texture(nullptr), _enable(true)
	{
	}

	void TextureUnit::setWrap(TextureWrapParameter which, TextureWrapMode wrap)
	{
		switch (which)
		{
		case WRAP_S:
			_wrap_s = wrap;
			break;
		case WRAP_T:
			_wrap_t = wrap;
			break;
		case WRAP_R:
			_wrap_r = wrap;
			break;
		}
	}

	void TextureUnit::setFilter(TextureFilterParameter which, TextureFilterMode mode)
	{
		switch (which)
		{
		case open::MIN_FILTER:
			_min_filter = mode;
			break;
		case open::MAG_FILTER:
			_mag_filter = mode;
			break;
		default:
			break;
		}
	}

	TextureWrapMode TextureUnit::getWrap(TextureWrapParameter which)
	{
		switch (which)
		{
		case WRAP_S:
			return _wrap_s;
		case WRAP_T:
			return _wrap_t;
		case WRAP_R:
			return _wrap_r;
		}
		return _wrap_s;
	}

	TextureFilterMode TextureUnit::getFilter(TextureFilterParameter which)
	{
		switch (which)
		{
		case open::MIN_FILTER:
			return _min_filter;
		case open::MAG_FILTER:
			return _mag_filter;
		default:
			return _min_filter;
		}
	}

	bool TextureUnit::apply(int32 unit)
	{
		if (!_texture)
			return false;

		if (_texture->bind(unit)) {

			gl_TexParameteri(OPEN_GL_TEXTURE_2D, OPEN_GL_TEXTURE_WRAP_S, _wrap_s);
			gl_TexParameteri(OPEN_GL_TEXTURE_2D, OPEN_GL_TEXTURE_WRAP_T, _wrap_t);
			gl_TexParameteri(OPEN_GL_TEXTURE_2D, OPEN_GL_TEXTURE_WRAP_R, _wrap_r);

			gl_TexParameteri(OPEN_GL_TEXTURE_2D, OPEN_GL_TEXTURE_MIN_FILTER, _min_filter);
			gl_TexParameteri(OPEN_GL_TEXTURE_2D, OPEN_GL_TEXTURE_MAG_FILTER, _mag_filter);

#if DOUBLE_PRECISION

			float vv[4];
			vv[0] = static_cast<float>(_borderColor.x());
			vv[1] = static_cast<float>(_borderColor.y());
			vv[2] = static_cast<float>(_borderColor.z());
			vv[3] = static_cast<float>(_borderColor.w());

			gl_TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, vv);
#else
			gl_TexParameterfv(OPEN_GL_TEXTURE_2D, OPEN_GL_TEXTURE_BORDER_COLOR, _borderColor.ptr());
#endif
			return true;
		}
		else {
			return false;
		}
	}

	TextureUnit::~TextureUnit()
	{
	}
}