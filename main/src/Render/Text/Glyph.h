#ifndef TEXT_GLYPH_H
#define TEXT_GLYPH_H

#include <string.h>
#include <map>
#include <vector>

#include <open/Ptr.h>
#include <open/Memory.h>
#include <open/Maths.h>

namespace open
{
	struct PtrObject {};

	class ITexture;
	class ITextureUnit;
	class IMaterial;
	class GlyphTexture;

	class GlyphImage : public TPtr<PtrObject>
	{
	public:

		GlyphImage(uint32 glyphCode);

		~GlyphImage();

		inline uint32 s() { return _s; }

		inline uint32 t() { return _t; }

		inline unsigned char* data() { return _data; }

		inline void alloc(uint32 w, uint32 h)
		{
			_data = new unsigned char[w * h];
			_s = w;
			_t = h;
			memset(_data, 0, w * h);
		}

		inline unsigned char& data(uint32 col, uint32 row) { return _data[row * _s + col]; }

		inline uint32 getGlyphCode() { return _glyphCode; }

	private:
		uint32 _glyphCode = 0;

		unsigned char* _data = nullptr;
		uint32 _s = 0;
		uint32 _t = 0;
	};

	class Glyph
	{
	public:
		Glyph(GlyphImage* glyphImage)
			:_glyphImage(glyphImage) {}

		inline GlyphImage* getGlyphImage() { return _glyphImage; }

		inline void setTexture(GlyphTexture* texture) { _texture = texture; }

		inline GlyphTexture* getTexture() { return _texture; }

		inline void setTexturePosition(int32 posX, int32 posY) { _texturePosX = posX; _texturePosY = posY; }

		inline int32 getTexturePositionX() { return _texturePosX; }

		inline int32 getTexturePositionY() { return _texturePosY; }

		inline void setMinTexCoord(const Vec2f& coord) { _minTexCoord = coord; }
		inline const Vec2f& getMinTexCoord() { return _minTexCoord; }

		inline void setMaxTexCoord(const Vec2f& coord) { _maxTexCoord = coord; }
		inline const Vec2f& getMaxTexCoord() { return _maxTexCoord; }

		inline Vec2f* getTexCoords()
		{
			Vec2f* coords = new Vec2f[4];
			coords[0].x() = _minTexCoord.x();
			coords[0].y() = _maxTexCoord.y();
			coords[1] = _minTexCoord;
			coords[2] = _maxTexCoord;
			coords[3].x() = _maxTexCoord.x();
			coords[3].y() = _minTexCoord.y();

			return coords;
		}
		void subload();

	private:

		Ptr<GlyphImage> _glyphImage;

		GlyphTexture* _texture;
		int32 _texturePosX;
		int32 _texturePosY;
		Vec2f _minTexCoord;
		Vec2f _maxTexCoord;
	};

	class GlyphTexture
	{
	public:

		static int32 getMaxTextureWidth();

		static int32 getMaxTextureHeight();

		GlyphTexture();

		~GlyphTexture();

		void setGlyphImageMargin(uint32 margin) { _margin = margin; }

		uint32 getGlyphImageMargin() const { return _margin; }

		void setGlyphImageMarginRatio(float margin) { _marginRatio = margin; }

		float getGlyphImageMarginRatio() const { return _marginRatio; }

		bool getSpaceForGlyph(Glyph* glyph, int32& posX, int32& posY);

		void addGlyph(Glyph* glyph, int32 posX, int32 posY);

		inline ITexture* getTexture() { return _texture; }

		inline IMaterial* getMaterial() { return _material; }

		void applySubGlyph();

	protected:

		int32 _margin;
		float _marginRatio;
		int32 _usedY;
		int32 _partUsedX;
		int32 _partUsedY;

		typedef std::vector<Glyph*> GlyphPtrList;
		GlyphPtrList _glyphs;
		GlyphPtrList _subToBindGlyphs;

		ITexture* _texture;
		ITextureUnit* _textureUnit;
		IMaterial* _material;
	};
}
#endif