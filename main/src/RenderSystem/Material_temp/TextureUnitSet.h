#ifndef RENDERSYSTEM_TEXTUREUNITSET_H
#define RENDERSYSTEM_TEXTUREUNITSET_H

#include <RenderSystem/RenderStatesInterface.h>
#include <common/Memory.h>

namespace open
{
	class TextureUnitSet : public ITextureUnitSet PUBLIC_OBJECT_DEBUG
	{
	public:

		TextureUnitSet();

		~TextureUnitSet();

		virtual ITextureUnit* getTextureUnit(TextureLayer unit);

		virtual void setTexutreUnitName(TextureLayer unit, const char* name);

		virtual int32 apply(IProgram* program);

	private:
		ITextureUnit* _unitSet[16];
	};

}
#endif