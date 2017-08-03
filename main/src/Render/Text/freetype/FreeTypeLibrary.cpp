#include <ft2build.h>
#include FT_FREETYPE_H

#include "FreeTypeLibrary.h"
#include "FreeTypeFont.h"


#ifdef PRint32_OUT_FONT_DETAILS
#include <freetype/ftsnames.h>
#endif

#include FT_TRUETYPE_IDS_H

namespace open {

	FreeTypeLibrary::FreeTypeLibrary()
	{
		FT_Error error = FT_Init_FreeType(&_ftlibrary);
		if (error)
		{
			ASSERT_(false);
		}
	}

	FreeTypeLibrary::~FreeTypeLibrary()
	{
		FT_Done_FreeType(_ftlibrary);
	}

	FreeTypeLibrary* FreeTypeLibrary::instance()
	{
		static FreeTypeLibrary s_library;
		return &s_library;
	}

	bool FreeTypeLibrary::getFace(const char* fontfile, uint32 index, FT_Face & face)
	{
		FT_Error error = FT_New_Face(_ftlibrary, fontfile, index, &face);
		if (error == FT_Err_Unknown_File_Format)
		{
			return false;
		}
		else if (error)
		{
			return false;
		}

		verifyCharacterMap(face);

		return true;
	}

	bool FreeTypeLibrary::getFace(uint8* buffer, uint32 length, uint32 index, FT_Face & face)
	{
		FT_Open_Args args;
		args.flags = FT_OPEN_MEMORY;
		args.memory_base = buffer;
		args.memory_size = length;

		FT_Error error = FT_Open_Face(_ftlibrary, &args, index, &face);

		if (error == FT_Err_Unknown_File_Format)
		{
			return false;
		}
		else if (error)
		{
			return false;
		}
		verifyCharacterMap(face);

		return true;
	}


	Font* FreeTypeLibrary::getFont(const char* fontfile, uint32 index, uint32 flags)
	{
		FT_Face face = nullptr;
		if (getFace(fontfile, index, face) == false)
			return nullptr;

		FreeTypeFont* fontImp = new FreeTypeFont(fontfile, face, flags);
		Font* font = new Font();
		font->setFontImp(fontImp);
		fontImp->setFont(font);

		return font;
	}

	Font* FreeTypeLibrary::getFont(uint8* bytes, uint32 size, uint32 index, uint32 flags)
	{
		FT_Face face = nullptr;
		if (getFace(bytes, size, index, face) == false)
			return nullptr;

		FreeTypeFont* fontImp = new FreeTypeFont(face, flags);
		Font* font = new Font();
		font->setFontImp(fontImp);
		fontImp->setFont(font);

		return font;
	}

	void FreeTypeLibrary::verifyCharacterMap(FT_Face face)
	{
		FT_CharMap charmap;
		if (face->charmap == nullptr)
		{
			for (int32 n = 0; n < face->num_charmaps; n++)
			{
				charmap = face->charmaps[n];
				if (charmap->platform_id == TT_PLATFORM_MICROSOFT)
				{
					FT_Set_Charmap(face, charmap);
					break;
				}
			}
		}
	}

}