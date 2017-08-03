#include "DefaultFont.h"
#include "Glyph.h"

namespace open
{
	DefaultFont::DefaultFont()
	{
		constructGlyphs();
	}

	DefaultFont::~DefaultFont()
	{
	}

	Glyph* DefaultFont::getGlyph(const FontResolution& fontRes, unsigned int charcode)
	{
		if (_sizeGlyphMap.empty())
			return 0;

		FontSizeGlyphMap::iterator itr = _sizeGlyphMap.find(fontRes);
		if (itr == _sizeGlyphMap.end())
		{
			// no font found of correct size, will need to find the nearest.
			itr = _sizeGlyphMap.begin();
			int mindeviation = abs((int)fontRes.first - (int)itr->first.first) +
				abs((int)fontRes.second - (int)itr->first.second);
			FontSizeGlyphMap::iterator sitr = itr;
			++sitr;
			for (; sitr != _sizeGlyphMap.end(); ++sitr)
			{
				int deviation = abs((int)fontRes.first - (int)sitr->first.first) +
					abs((int)fontRes.second - (int)sitr->first.second);
				if (deviation < mindeviation)
				{
					mindeviation = deviation;
					itr = sitr;
				}
			}
		}
		// new find the glyph for the required charcode.
		GlyphMap& glyphmap = itr->second;
		GlyphMap::iterator gitr = glyphmap.find(charcode);

		if (gitr != glyphmap.end())
			return gitr->second;
		else return 0;
	}

	bool DefaultFont::hasVertical() const
	{
		return true;
	}

	void DefaultFont::constructGlyphs()
	{
		static uint8 rasters[][12] = { // ascii symbols 32-127, small font
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x00 },
			{ 0x00, 0x00, 0x28, 0x28, 0x7e, 0x14, 0x14, 0x14, 0x3f, 0x0a, 0x0a, 0x00 },
			{ 0x00, 0x00, 0x08, 0x1c, 0x22, 0x02, 0x1c, 0x20, 0x22, 0x1c, 0x08, 0x00 },
			{ 0x00, 0x00, 0x02, 0x45, 0x22, 0x10, 0x08, 0x04, 0x22, 0x51, 0x20, 0x00 },
			{ 0x00, 0x00, 0x3b, 0x44, 0x4a, 0x49, 0x30, 0x10, 0x20, 0x20, 0x18, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x00 },
			{ 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x00 },
			{ 0x10, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x08, 0x10, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x36, 0x1c, 0x7f, 0x1c, 0x36, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x08, 0x08, 0x08, 0x7f, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00 },
			{ 0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, 0x08, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x20, 0x10, 0x08, 0x04, 0x02, 0x02, 0x22, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x22, 0x02, 0x02, 0x0c, 0x02, 0x02, 0x22, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x0e, 0x04, 0x3e, 0x24, 0x14, 0x14, 0x0c, 0x0c, 0x04, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x22, 0x02, 0x02, 0x3c, 0x20, 0x20, 0x20, 0x3e, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x22, 0x22, 0x22, 0x3c, 0x20, 0x20, 0x10, 0x0c, 0x00 },
			{ 0x00, 0x00, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x22, 0x3e, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x22, 0x22, 0x22, 0x1c, 0x22, 0x22, 0x22, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x18, 0x04, 0x02, 0x02, 0x1e, 0x22, 0x22, 0x22, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x04, 0x02, 0x02, 0x22, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x20, 0x4e, 0x55, 0x55, 0x55, 0x4d, 0x21, 0x1e, 0x00 },
			{ 0x00, 0x00, 0x77, 0x22, 0x3e, 0x22, 0x14, 0x14, 0x08, 0x08, 0x18, 0x00 },
			{ 0x00, 0x00, 0x7e, 0x21, 0x21, 0x21, 0x3e, 0x21, 0x21, 0x21, 0x7e, 0x00 },
			{ 0x00, 0x00, 0x1e, 0x21, 0x40, 0x40, 0x40, 0x40, 0x40, 0x21, 0x1e, 0x00 },
			{ 0x00, 0x00, 0x7c, 0x22, 0x21, 0x21, 0x21, 0x21, 0x21, 0x22, 0x7c, 0x00 },
			{ 0x00, 0x00, 0x7f, 0x21, 0x20, 0x24, 0x3c, 0x24, 0x20, 0x21, 0x7f, 0x00 },
			{ 0x00, 0x00, 0x78, 0x20, 0x20, 0x24, 0x3c, 0x24, 0x20, 0x21, 0x7f, 0x00 },
			{ 0x00, 0x00, 0x1e, 0x21, 0x41, 0x47, 0x40, 0x40, 0x40, 0x21, 0x1e, 0x00 },
			{ 0x00, 0x00, 0x77, 0x22, 0x22, 0x22, 0x3e, 0x22, 0x22, 0x22, 0x77, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00 },
			{ 0x00, 0x00, 0x38, 0x44, 0x44, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1e, 0x00 },
			{ 0x00, 0x00, 0x73, 0x22, 0x24, 0x38, 0x28, 0x24, 0x24, 0x22, 0x73, 0x00 },
			{ 0x00, 0x00, 0x7f, 0x11, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00 },
			{ 0x00, 0x00, 0x77, 0x22, 0x22, 0x2a, 0x2a, 0x36, 0x36, 0x22, 0x63, 0x00 },
			{ 0x00, 0x00, 0x72, 0x22, 0x26, 0x26, 0x2a, 0x32, 0x32, 0x22, 0x67, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x78, 0x20, 0x20, 0x20, 0x3e, 0x21, 0x21, 0x21, 0x7e, 0x00 },
			{ 0x00, 0x1b, 0x1c, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x73, 0x22, 0x24, 0x24, 0x3e, 0x21, 0x21, 0x21, 0x7e, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x41, 0x01, 0x01, 0x3e, 0x40, 0x40, 0x41, 0x3e, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x49, 0x7f, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x77, 0x00 },
			{ 0x00, 0x00, 0x08, 0x08, 0x14, 0x14, 0x14, 0x22, 0x22, 0x22, 0x77, 0x00 },
			{ 0x00, 0x00, 0x14, 0x14, 0x2a, 0x2a, 0x2a, 0x22, 0x22, 0x22, 0x77, 0x00 },
			{ 0x00, 0x00, 0x77, 0x22, 0x14, 0x14, 0x08, 0x14, 0x14, 0x22, 0x77, 0x00 },
			{ 0x00, 0x00, 0x1c, 0x08, 0x08, 0x08, 0x14, 0x14, 0x22, 0x22, 0x77, 0x00 },
			{ 0x00, 0x00, 0x7f, 0x21, 0x10, 0x10, 0x08, 0x04, 0x04, 0x42, 0x7f, 0x00 },
			{ 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00 },
			{ 0x1c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1c, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x14, 0x08 },
			{ 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00 },
			{ 0x00, 0x00, 0x3d, 0x42, 0x42, 0x3e, 0x02, 0x3c, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x7e, 0x21, 0x21, 0x21, 0x21, 0x3e, 0x20, 0x20, 0x60, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x41, 0x40, 0x40, 0x41, 0x3e, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x3f, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x06, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x41, 0x40, 0x7f, 0x41, 0x3e, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x3c, 0x10, 0x10, 0x10, 0x10, 0x3c, 0x10, 0x10, 0x0c, 0x00 },
			{ 0x3c, 0x02, 0x02, 0x3e, 0x42, 0x42, 0x42, 0x3f, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x77, 0x22, 0x22, 0x22, 0x32, 0x2c, 0x20, 0x20, 0x60, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x08, 0x00 },
			{ 0x38, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x3c, 0x00, 0x00, 0x04, 0x00 },
			{ 0x00, 0x00, 0x63, 0x24, 0x38, 0x28, 0x24, 0x26, 0x20, 0x20, 0x60, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x18, 0x00 },
			{ 0x00, 0x00, 0x6b, 0x2a, 0x2a, 0x2a, 0x2a, 0x74, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x77, 0x22, 0x22, 0x22, 0x32, 0x6c, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x41, 0x41, 0x41, 0x41, 0x3e, 0x00, 0x00, 0x00, 0x00 },
			{ 0x70, 0x20, 0x3e, 0x21, 0x21, 0x21, 0x21, 0x7e, 0x00, 0x00, 0x00, 0x00 },
			{ 0x07, 0x02, 0x3e, 0x42, 0x42, 0x42, 0x42, 0x3f, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x19, 0x76, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x3e, 0x41, 0x06, 0x38, 0x41, 0x3e, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x0c, 0x12, 0x10, 0x10, 0x10, 0x3c, 0x10, 0x10, 0x00, 0x00 },
			{ 0x00, 0x00, 0x1b, 0x26, 0x22, 0x22, 0x22, 0x66, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x08, 0x14, 0x14, 0x22, 0x22, 0x77, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x14, 0x14, 0x2a, 0x2a, 0x22, 0x77, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x77, 0x22, 0x1c, 0x1c, 0x22, 0x77, 0x00, 0x00, 0x00, 0x00 },
			{ 0x30, 0x08, 0x08, 0x14, 0x14, 0x22, 0x22, 0x77, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x7e, 0x22, 0x10, 0x08, 0x44, 0x7e, 0x00, 0x00, 0x00, 0x00 },
			{ 0x06, 0x08, 0x08, 0x08, 0x08, 0x30, 0x08, 0x08, 0x08, 0x08, 0x06, 0x00 },
			{ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 },
			{ 0x30, 0x08, 0x08, 0x08, 0x08, 0x06, 0x08, 0x08, 0x08, 0x08, 0x30, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x49, 0x31, 0x00, 0x00 },
			{ 0x00, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x00, 0x00 }
		};

		uint32 sourceWidth = 8;
		uint32 sourceHeight = 12;

		FontResolution fontRes(sourceWidth, sourceHeight);

		// populate the glyph mp
		for (uint32 i = 32; i < 127; i++)
		{
			GlyphImage* img = new GlyphImage(i);

			uint32 dataSize = sourceWidth*sourceHeight;

			img->alloc(sourceWidth, sourceHeight);
			unsigned char* data = img->data();

			// now populate data array by converting bitmap into a luminance_alpha map.
			unsigned char* ptr = rasters[i - 32];
			unsigned char value_on = 255;
			unsigned char value_off = 0;

			for (uint32 row = 0; row < sourceHeight; ++row, ++ptr)
			{
				(*data++) = ((*ptr) & 128) ? value_on : value_off;
				(*data++) = ((*ptr) & 64) ? value_on : value_off;
				(*data++) = ((*ptr) & 32) ? value_on : value_off;
				(*data++) = ((*ptr) & 16) ? value_on : value_off;
				(*data++) = ((*ptr) & 8) ? value_on : value_off;
				(*data++) = ((*ptr) & 4) ? value_on : value_off;
				(*data++) = ((*ptr) & 2) ? value_on : value_off;
				(*data++) = ((*ptr) & 1) ? value_on : value_off;
			}

			Glyph* glyph = new Glyph(img);
			addGlyph(fontRes, i, glyph);
		}
	}
}