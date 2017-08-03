#include <ft2build.h>
#include FT_FREETYPE_H

#include "FreeTypeFont.h"
#include "FreeTypeLibrary.h"

#include <freetype/ftoutln.h>
#include <freetype/ftbbox.h>

#include "../Glyph.h"

namespace open
{
	FreeTypeFont::FreeTypeFont(const char* filename, FT_Face face, uint32 flags) :
		_currentRes(FontResolution(0, 0)),
		_filename(filename),
		_face(face),
		_flags(flags)
	{
		init();
	}

	FreeTypeFont::FreeTypeFont(FT_Face face, uint32 flags) :
		_currentRes(FontResolution(0, 0)),
		_filename(""),
		_face(face),
		_flags(flags)
	{
		init();
	}

	FreeTypeFont::~FreeTypeFont()
	{
		if (_face)
		{
			FreeTypeLibrary* freeTypeLibrary = FreeTypeLibrary::instance();
			if (freeTypeLibrary)
			{
				FT_Done_Face(_face);
				_face = 0;
			}
		}
	}

	void FreeTypeFont::init()
	{
		FT_Error _error;
		_error = FT_Set_Pixel_Sizes(_face, 32, 32);
		if (_error)
		{
			return;
		}
		_currentRes.first = 32;
		_currentRes.second = 32;
	}

	void FreeTypeFont::setFontResolution(const FontResolution& fontSize)
	{
		if (fontSize == _currentRes)
			return;

		int32 width = fontSize.first;
		int32 height = fontSize.second;
		int32 maxAxis = max(width, height);
		int32 margin = _facade->getGlyphImageMargin() + (int32)((float)maxAxis * _facade->getGlyphImageMarginRatio());

		if ((uint32)(width + 2 * margin) > _facade->getTextureWidthHint() ||
			(uint32)(width + 2 * margin) > _facade->getTextureHeightHint())
		{
			width = _facade->getTextureWidthHint() - 2 * margin;
			height = _facade->getTextureHeightHint() - 2 * margin;
		}

		FT_Error error = FT_Set_Pixel_Sizes(_face,
			width,
			height);

		if (error)
		{
		}
		else
		{
			_currentRes = fontSize;
		}
	}

	GlyphImage* FreeTypeFont::getGlyph(const FontResolution& fontRes, uint32 charcode)
	{
		setFontResolution(fontRes);

		uint32 charindex = charcode;
		if (_face->charmap != nullptr)
		{
			if (_face->charmap->encoding == FT_ENCODING_MS_SYMBOL)
			{
				charindex |= 0xF000;
			}
		}

		FT_Error error = FT_Load_Char(_face, charindex, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | _flags);
		if (error)
		{
			return 0;
		}

		FT_GlyphSlot glyphslot = _face->glyph;

		int32 pitch = glyphslot->bitmap.pitch;
		unsigned char* buffer = glyphslot->bitmap.buffer;

		uint32 sourceWidth = glyphslot->bitmap.width;;
		uint32 sourceHeight = glyphslot->bitmap.rows;

		uint32 width = sourceWidth;
		uint32 height = sourceHeight;

		GlyphImage* img = new GlyphImage(charcode);
		img->alloc(width, height);
		uint32 dataSize = width*height;
		unsigned char* data = img->data();

		switch (glyphslot->bitmap.pixel_mode)
		{
		case FT_PIXEL_MODE_MONO:
			for (int32 r = sourceHeight - 1;r >= 0;--r)
			{
				unsigned char* ptr = buffer + r*pitch;
				for (uint32 c = 0;c < sourceWidth;++c)
				{
					(*data++) = (ptr[c >> 3] & (1 << (~c & 7))) ? 255 : 0;
				}
			}
			break;


		case FT_PIXEL_MODE_GRAY:
			for (int32 r = sourceHeight - 1;r >= 0;--r)
			{
				unsigned char* ptr = buffer + r*pitch;
				for (uint32 c = 0;c < sourceWidth;++c, ++ptr)
				{
					(*data++) = *ptr;
				}
			}
			break;

		default:
			return img;
		}
		return img;
	}

	bool FreeTypeFont::hasVertical()
	{
		return FT_HAS_VERTICAL(_face) != 0;
	}
}