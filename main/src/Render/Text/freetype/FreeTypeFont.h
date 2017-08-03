#ifndef FREETYPE_FONT
#define FREETYPE_FONT 1

#include <string>
#include "../Font.h"
#include <open/Maths.h>

typedef struct FT_FaceRec_*  FT_Face;

namespace open {

	class FreeTypeFont : public FontImp
	{
	public:

		FreeTypeFont(const char* filename, FT_Face face, uint32 flags);

		FreeTypeFont(FT_Face face, uint32 flags);

		virtual ~FreeTypeFont();

		virtual const char* getFileName() const { return _filename.c_str(); }

		virtual bool supportsMultipleFontResolutions() { return true; }

		GlyphImage* FreeTypeFont::getGlyph(const FontResolution& fontRes, uint32 charcode);

		virtual bool hasVertical();

		void setFont(Font* font) { _facade = font; }
	protected:

		void init();

		void setFontResolution(const FontResolution& fontSize);

		long ft_round(long x) { return ((x + 32) & -64); }
		long ft_floor(long x) { return (x & -64); }
		long ft_ceiling(long x) { return ((x + 63) & -64); }

		FontResolution _currentRes;
		Font* _facade;
		std::string  _filename;
		FT_Face _face;
		uint32 _flags;
	};
}

#endif