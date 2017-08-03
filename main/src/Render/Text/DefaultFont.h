#ifndef TEXT_DEFAULTFONT_H
#define TEXT_DEFAULTFONT_H 1

#include "Font.h"

namespace open
{
	class Glyph;
	class DefaultFont : public Font
	{
	public:

		DefaultFont();

		virtual std::string getFileName() const { return ""; }

		virtual bool supportsMultipleFontResolutions() const { return false; }

		virtual Glyph* getGlyph(const FontResolution& fontRes, unsigned int charcode);

		virtual bool hasVertical() const;

	protected:

		virtual ~DefaultFont();

		void constructGlyphs();
	};
}

#endif