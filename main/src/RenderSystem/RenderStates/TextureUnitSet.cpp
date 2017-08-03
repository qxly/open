#include "TextureUnitSet.h"
#include "TextureUnit.h"
#include "../GlState.h"

namespace open
{
	TextureUnitSet::TextureUnitSet()
		:_vaildNum(0)
	{
		_uinitName[0] = "_diffuseTexture";
		_uinitName[1] = "_emissiveTexture";
		_uinitName[2] = "_opacityTexture";
	}

	ITextureUnit* TextureUnitSet::createTextureUnit(uint8 layer, const char* name)
	{
		if (!_unitSet[layer].get())
		{
			_vaildNum++;
			TextureUnit* unit = new TextureUnit;
			_unitSet[layer] = unit;

			if (name != nullptr)
				_uinitName[layer] = name;

			return unit;
		}
		return _unitSet[layer];
	}

	bool TextureUnitSet::hasTextureUnit(uint8 unitIndex)
	{
		return _unitSet[unitIndex] != nullptr;
	}

	ITextureUnit* TextureUnitSet::getTextureUnit(uint8 unit)
	{
		return _unitSet[unit];
	}

	void TextureUnitSet::setTextureUnitNam(uint8 unit, const char * name)
	{
		_uinitName[unit] = name;
	}

	const char * TextureUnitSet::getTextureUnitNam(uint8 unit)
	{
		return _uinitName[unit].c_str();
	}

	void TextureUnitSet::replace(uint8 unitIndex, ITextureUnit * unit)
	{
		_unitSet[unitIndex] = unit;
	}

	int32 TextureUnitSet::getVaildUnitNum()
	{
		return _vaildNum;
	}

	void TextureUnitSet::apply()
	{
		for (int32 i = 0; i < 16; i++)
		{
			TextureUnit* unit = (TextureUnit*)_unitSet[i].get();
			if (!unit || !unit->getEnable() || !unit->getTexture() || !unit->apply(i))
				gl::gl_DisableTexture(i);
		}
	}

	TextureUnitSet::~TextureUnitSet()
	{
		for (int32 i = 0; i < 16; i++)
		{
			delete _unitSet[i];
		}
	}
}