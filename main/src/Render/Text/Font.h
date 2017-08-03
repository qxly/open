#ifndef TEXT_FONT_H
#define TEXT_FONT_H

#include <open/openDef.h>
#include <open/Ptr.h>
#include <map>
#include <vector>

namespace open
{
	struct FontResolution
	{
		uint32 first;
		uint32 second;

		FontResolution()
			:first(0), second(0){}

		FontResolution(uint32 f, uint32 s)
			:first(f), second(s){}

		inline bool operator < (const FontResolution& right) const
		{
			if (first < right.first)
				return true;
			return false;
		}

		inline bool operator == (const FontResolution& right) const
		{
			return (first == right.first && second == right.second);
		}
	};

	class Glyph;
	class GlyphImage;
	class GlyphTexture;

	class FontImp
	{
	public:
		virtual bool supportsMultipleFontResolutions() = 0;

		virtual GlyphImage* getGlyph(const FontResolution& fontRes, uint32 charcode) = 0;

		virtual bool hasVertical() = 0;
	};

	class Font
	{
	protected:
		typedef std::map<uint32, Glyph*> GlyphMap;
		typedef std::map<FontResolution, GlyphMap> FontSizeGlyphMap;
		typedef std::vector<GlyphTexture*>       GlyphTextureList;

	public:
		Font();

		void setFontImp(FontImp* fontImp) { _fontImp = fontImp; }

		virtual const char* getFileName();

		virtual Glyph* getGlyph(const FontResolution& fontSize, uint32 charcode);

		virtual bool hasVertical();

		virtual bool getVerticalSize(float& ascender, float& descender);

		void setGlyphImageMargin(uint32 margin);

		uint32 getGlyphImageMargin();

		void setGlyphImageMarginRatio(float margin);

		float getGlyphImageMarginRatio();

		void setTextureSizeHint(uint32 width, uint32 height);

		uint32 getTextureWidthHint();

		uint32 getTextureHeightHint();

		uint32 getFontDepth() { return _depth; }

		void setNumberCurveSamples(uint32 numSamples) { _numCurveSamples = numSamples; }

		uint32 getNumberCurveSamples() { return _numCurveSamples; }

		inline GlyphTextureList& getGlyphTextureList() { return _glyphTextureList; }

		void addGlyph(const FontResolution& fontRes, uint32 charcode, Glyph* glyph);

	protected:

		virtual ~Font();

		FontSizeGlyphMap _sizeGlyphMap;
		GlyphTextureList _glyphTextureList;

		FontResolution _fontSize;
		uint32 _margin;
		float _marginRatio;

		uint32 _textureWidthHint;
		uint32 _textureHeightHint;

		uint32 _depth;
		uint32 _numCurveSamples;

		FontImp* _fontImp;
	};
}
#endif