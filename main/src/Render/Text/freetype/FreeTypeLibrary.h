#ifndef FREETYPE_LIBRARY
#define FREETYPE_LIBRARY

#include <open/Array.h>

typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

namespace open {

	class Font;
	class FreeTypeLibrary
	{
	public:

		virtual ~FreeTypeLibrary();

		static FreeTypeLibrary* instance();

		Font* getFont(const char* fontfile, uint32 index = 0, uint32 flags = 0);

		Font* getFont(uint8* bytes, uint32 size, uint32 index = 0, uint32 flags = 0);

	protected:

		FreeTypeLibrary();

		bool getFace(const char* fontfile, uint32 index, FT_Face & face);

		bool getFace(uint8* buffer, uint32 length, uint32 index, FT_Face & face);

		void  verifyCharacterMap(FT_Face face);

	private:
		FT_Library _ftlibrary;
	};
}

#endif