#include "Glyph.h"

#include <RenderSystem/RenderEnumeration.h>
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>

#include <Render/Materialinterface.h>
#include <Render/RenderFactoryInterface.h>

namespace open
{
	GlyphImage::GlyphImage(uint32 glyphCode)
		:_glyphCode(glyphCode), _data(nullptr)
	{
	}

	GlyphImage::~GlyphImage()
	{
		delete[] _data;
	}

	void Glyph::subload()
	{
		_texture->getTexture()->subLoad(_texturePosX, _texturePosY, _glyphImage->s(), _glyphImage->t(), _glyphImage->data());
	}

	static const int32 GLYPH_TEXTURE_SIZE = 1024;

	int32 GlyphTexture::getMaxTextureWidth() {

		return GLYPH_TEXTURE_SIZE;
	}

	int32 GlyphTexture::getMaxTextureHeight() {

		return GLYPH_TEXTURE_SIZE;
	}

	GlyphTexture::GlyphTexture() :_margin(1), _marginRatio(0.02f), _usedY(0), _partUsedX(0), _partUsedY(0)
	{
		_material = RenderFactory::s_factory->createMaterial("TextSchema");
		_material->getRenderStaeSet().setBlendFunSeparate(BLENDFUN_SRC_ALPHA, BLENDFUN_ONE_MINUS_SRC_ALPHA, BLENDFUN_SRC_ALPHA, BLENDFUN_ONE_MINUS_SRC_ALPHA);
		_material->getRenderStaeSet().setMode(BLEND, true);
		_material->getRenderStaeSet().setDepthMask(false);

		_textureUnit = _material->getTextureUnitSet()->createTextureUnit(0);
		_texture = RenderSystemFactory::s_factory->createTexture();
		_texture->alloc(PIXELINTERNALFORMAT_R8, GLYPH_TEXTURE_SIZE, GLYPH_TEXTURE_SIZE, 0, PIXEFORMAT_R, DATATYPE_UNSIGNED_BYTE, nullptr);

		_textureUnit->setTexture(_texture);
	}

	GlyphTexture::~GlyphTexture()
	{
	}

	bool GlyphTexture::getSpaceForGlyph(Glyph* glyph, int32& posX, int32& posY)
	{
		int32 maxAxis = max(glyph->getGlyphImage()->s(), glyph->getGlyphImage()->t());
		int32 margin = _margin + (int32)((float)maxAxis * _marginRatio);

		int32 width = glyph->getGlyphImage()->s() + 2 * margin;
		int32 height = glyph->getGlyphImage()->t() + 2 * margin;

		if (width <= (getTexture()->getWidth() - _partUsedX) && height <= (getTexture()->getHeight() - _usedY))
		{
			posX = _partUsedX + margin;
			posY = _usedY + margin;

			_partUsedX += width;
			if (_usedY + height > _partUsedY)
				_partUsedY = _usedY + height;

			return true;
		}

		if (width <= getTexture()->getWidth() && height <= (getTexture()->getHeight() - _partUsedY))
		{
			_partUsedX = 0;
			_usedY = _partUsedY;

			posX = _partUsedX + margin;
			posY = _usedY + margin;
			_partUsedX += width;

			if (_usedY + height > _partUsedY)
				_partUsedY = _usedY + height;

			return true;
		}
		return false;
	}

	void GlyphTexture::addGlyph(Glyph* glyph, int32 posX, int32 posY)
	{
		_glyphs.push_back(glyph);
		_subToBindGlyphs.push_back(glyph);

		glyph->setTexture(this);
		glyph->setTexturePosition(posX, posY);

		glyph->setMinTexCoord(Vec2f(static_cast<float>(posX) / static_cast<float>(getTexture()->getWidth()),
			static_cast<float>(posY) / static_cast<float>(getTexture()->getHeight())));
		glyph->setMaxTexCoord(Vec2f(static_cast<float>(posX + glyph->getGlyphImage()->s()) / static_cast<float>(getTexture()->getWidth()),
			static_cast<float>(posY + glyph->getGlyphImage()->t()) / static_cast<float>(getTexture()->getHeight())));
	}

	void GlyphTexture::applySubGlyph()
	{
		if (_subToBindGlyphs.empty())
			return;

		for (uint32 i = 0; i < _subToBindGlyphs.size(); i++)
		{
			_subToBindGlyphs[i]->subload();
		}
		_subToBindGlyphs.clear();
	}
}