#include "TextureUnitSet.h"
#include "TextureUnit.h"
#
namespace open
{
	TextureUnitSet::TextureUnitSet()
	{
		memset(_unitSet, 0, sizeof(_unitSet));
	}

	ITextureUnit* TextureUnitSet::getTextureUnit(TextureLayer layer)
	{
		if (!_unitSet[(int32)layer])
		{
			TextureUnit* unit = new TextureUnit;
			unit->setTextureLayer(layer);
			_unitSet[(int32)layer] = unit;
			return unit;
		}
		return _unitSet[(int32)layer];
	}

	void TextureUnitSet::setTexutreUnitName(TextureLayer unit, const char* name)
	{
		((TextureUnit*)getTextureUnit(unit))->setName(name);
	}

	int32 TextureUnitSet::apply(IProgram* program)
	{
		int32 unitNum = 0;
		int32 flag = 0;
		for (int32 i = 0; i < 16; i++)
		{
			ITextureUnit* unit = _unitSet[i];
			if (!unit)
				continue;

			TextureLayer layer = unit->getTextureLayer();
			const char* name = unit->getName();
			int id = -1;
			switch (layer)
			{
			case DIFFUSE_LAYER:
				id = program->getUniformID(!name ? "_diffuseTexture" : name);
				break;
			case EMISSIVE_LAYER:
				id = program->getUniformID(!name ? "_emissiveTexture" : name);
				break;
			case OPACITY_LAYER:
				id = program->getUniformID(!name ? "_opacityTexture" : name);
				break;
			case BUMP_LAYER:
				id = program->getUniformID(!name ? "_bumpTexture" : name);
				break;
			case UNKOWN_LAYER1:
				id = program->getUniformID(unit->getName());
				break;
			case UNKOWN_LAYER2:
				id = program->getUniformID(unit->getName());
				break;
			default:
				break;
			}
			if (id < 0)
				continue;

			if (!unit->apply(unitNum))
				continue;
			program->setUniformi(id, unitNum);

			flag |= (1 << i);
			unitNum++;
		}
		return flag;
	}

	TextureUnitSet::~TextureUnitSet()
	{
		for (int32 i = 0; i < 16; i++)
		{
			delete _unitSet[i];
		}
	}
}