#include "TextureUnit.h"
#include <glew/glew.h>

namespace open
{
	TexureUnitParame TextureUnit::s_templateTexureUnitParame[16] = { TexureUnitParame() };

	TextureUnit::TextureUnit()
		:_texture(NULL), _name(NULL)
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

	bool TextureUnit::apply(int32 unit)
	{
		if (!_texture)
			return false;

		glActiveTexture(GL_TEXTURE0 + unit);

		_texture->bind();

		//if (s_templateTexureUnitParame[unit] != (*this))
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap_s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap_t);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, _wrap_r);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _min_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _mag_filter);

			s_templateTexureUnitParame[unit] = *this;
		}

#if DOUBLE_PRECISION

		float vv[4];
		vv[0] = static_cast<float>(_borderColor.x());
		vv[1] = static_cast<float>(_borderColor.y());
		vv[2] = static_cast<float>(_borderColor.z());
		vv[3] = static_cast<float>(_borderColor.w());

		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, vv);
#else
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _borderColor.ptr());
#endif
		return true;
	}

	TextureUnit::~TextureUnit()
	{
		delete[] _name;
	}

	void TextureUnit::initTextureUnitState()
	{
		/*for (int i = 0; i < 16; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, CLAMP);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, LINEAR);
		}*/
	}
}