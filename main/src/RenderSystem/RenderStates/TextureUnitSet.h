#ifndef RENDERSYSTEM_TEXTUREUNITSET_H
#define RENDERSYSTEM_TEXTUREUNITSET_H

#include <RenderSystem/RenderStatesInterface.h>
#include <open/Memory.h>
#include <open/Ptr.h>
#include <string>

namespace open
{
	class TextureUnitSet : public ITextureUnitSet PUBLIC_OBJECT_DEBUG
	{
	public:

		TextureUnitSet();

		~TextureUnitSet();

		virtual ITextureUnit* createTextureUnit(uint8 unit,const char* name = nullptr);

		virtual bool hasTextureUnit(uint8 unitIndex);

		virtual ITextureUnit* getTextureUnit(uint8 unit);

		virtual void setTextureUnitNam(uint8 unit, const char* name);

		virtual const char* getTextureUnitNam(uint8 unit);

		virtual void replace(uint8 unitIndex, ITextureUnit* unit);

		virtual int32 getVaildUnitNum();

		virtual void apply();

	private:
		Ptr<ITextureUnit> _unitSet[16];

		std::string _uinitName[16];

		int32 _vaildNum;
	};

}
#endif