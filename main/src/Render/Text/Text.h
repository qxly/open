#ifndef TEXT_TEXT_H
#define TEXT_TEXT_H

#include <string>
#include <open/Ptr.h>
#include <open/Memory.h>
#include <vector>
#include <Render/DrawableInterface.h>

#include "Font.h"

namespace open
{
	class IGeometry;
	class IMaterial;
	class Font;
	class IRenderTechnique;

	class Text : public TPtr<IText> PUBLIC_OBJECT_DEBUG
	{
	public:
		struct TextInstance {
			Vec3f position;
			Vec4f textureCoord;
			Text* text;
		};
	public:
		Text();

		virtual uint32 getNumGeometry();
		virtual IGeometry* getGeometry(uint32 index);
		virtual IMaterial* getMaterial(uint32 index);

		virtual const RBoundingBox& getBoudingBox();

		virtual void accept(VisitGeometryVertex dv);

		virtual void accept(VisitGeometry vg);

		virtual void setFont(const char* path);
		virtual void setText(const char* text);

		virtual void setFontSize(uint32 fontSize) { _fontHeight = fontSize; }
		virtual uint32 getFontSize() { return _fontHeight; }

		virtual void setColor(const Real4& color);
		virtual const Real4& getColor();

		virtual void setCharacterSizeMode(CharacterSizeMode mode);
		virtual CharacterSizeMode getCharacterSizeMode();

		virtual void setCharacterSize(Real height);
		virtual void setCharacterSize(Real height, Real aspectRatio);
		virtual Real getCharacterSize() { return _characterSize; }

		static IRenderTechnique* renderTextSchema();

	private:

		RBoundingBox computeBound();

		void resetGeometries();

	private:
		Font* _font = nullptr;

		bool _reset = false;
		std::string _text;
		uint32 _fontHeight = 32;
		Real4 _color = Real4(1, 0, 0, 1);
		CharacterSizeMode _characterSizeMode = OBJECT_COORDS;
		AlignmentType _alignment = CENTER_CENTER;
		Layout _layout;

		Real _characterSize = 32;
		Real _aspectRatio = 1;

		RMatrix _rotateMatrix;
		RBoundingBox _aabb;
		std::vector<TextInstance> _char_instances;

		static Ptr<IGeometry> s_text_geometry;
	};
}
#endif