#include "Font.h"
#include "Glyph.h"

namespace open
{
	Font::Font()
		:_margin(1), _marginRatio(0.02f), _textureWidthHint(1024), _textureHeightHint(1024), _depth(1), _numCurveSamples(10)
	{
	}

	Font::~Font()
	{
	}

	const char* Font::getFileName()
	{
		return nullptr;
	}

	void Font::setGlyphImageMargin(uint32 margin)
	{
		_margin = margin;
	}

	uint32 Font::getGlyphImageMargin()
	{
		return _margin;
	}

	void Font::setGlyphImageMarginRatio(float ratio)
	{
		_marginRatio = ratio;
	}

	float Font::getGlyphImageMarginRatio()
	{
		return _marginRatio;
	}

	void Font::setTextureSizeHint(uint32 width, uint32 height)
	{
		_textureWidthHint = width;
		_textureHeightHint = height;
	}

	uint32 Font::getTextureWidthHint()
	{
		return _textureWidthHint;
	}

	uint32 Font::getTextureHeightHint()
	{
		return _textureHeightHint;
	}

	bool Font::getVerticalSize(float& ascender, float& descender)
	{
		return false;
	}

	Glyph* Font::getGlyph(const FontResolution& fontRes, uint32 charcode)
	{
		if (!_fontImp)
			return nullptr;

		FontResolution fontResUsed(0, 0);
		if (_fontImp->supportsMultipleFontResolutions())
			fontResUsed = fontRes;

		{
			FontSizeGlyphMap::iterator itr = _sizeGlyphMap.find(fontResUsed);
			if (itr != _sizeGlyphMap.end())
			{
				GlyphMap& glyphmap = itr->second;
				GlyphMap::iterator gitr = glyphmap.find(charcode);
				if (gitr != glyphmap.end())
					return gitr->second;
			}
		}

		GlyphImage* image = _fontImp->getGlyph(fontResUsed, charcode);
		if (image)
		{
			Glyph* glyph = new Glyph(image);
			addGlyph(fontResUsed, charcode, glyph);
			return glyph;
		}
		else
			return nullptr;
	}

	bool Font::hasVertical()
	{
		return true;
	}

	void Font::addGlyph(const FontResolution& fontRes, uint32 charcode, Glyph* glyph)
	{
		_sizeGlyphMap[fontRes][charcode] = glyph;

		int32 posX = 0, posY = 0;

		GlyphTexture* glyphTexture = 0;
		for (GlyphTextureList::iterator itr = _glyphTextureList.begin();
			itr != _glyphTextureList.end() && !glyphTexture; ++itr)
		{
			if ((*itr)->getSpaceForGlyph(glyph, posX, posY))
			{
				glyphTexture = (*itr);
				break;
			}
		}

		if (!glyphTexture)
		{
			glyphTexture = new GlyphTexture;

			static int32 numberOfTexturesAllocated = 0;
			++numberOfTexturesAllocated;

			glyphTexture->setGlyphImageMargin(_margin);
			glyphTexture->setGlyphImageMarginRatio(_marginRatio);

			_glyphTextureList.push_back(glyphTexture);

			if (!glyphTexture->getSpaceForGlyph(glyph, posX, posY))
			{
				ASSERT_(false);
			}
		}
		glyphTexture->addGlyph(glyph, posX, posY);
	}
}