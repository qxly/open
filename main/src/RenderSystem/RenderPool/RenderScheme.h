#ifndef RENDERSYSTEM_RENDERSCHEME_H
#define RENDERSYSTEM_RENDERSCHEME_H 1

#include <RenderSystem/RenderInterface.h>

namespace wyin
{
	class IProgram;
	class RenderPool;

	class DefaultRenderScheme : public IRenderScheme
	{
	public:

		DefaultRenderScheme();

		virtual void begin(IRenderPool* renderPool);

		virtual void applyMaterial(IMaterial* materail);

		virtual void apply(const RMatrix& modelMatrix);

	private:
		IProgram* _program;
		IMaterial* _currentMaterial;
		IRenderPool* _renderPool;
	};
}

#endif