#ifndef RENDER_MATERAIL_H
#define RENDER_MATERAIL_H 1

#include <open/Ptr.h>
#include <open/Memory.h>
#include <open/interface/Render/MaterialInterface.h>
#include <open/interface/RenderSystem/RenderStatesInterface.h>

#include <map>
#include <vector>

namespace open
{
	class Material : public TPtr<IMaterial> PUBLIC_OBJECT_DEBUG
	{
	public:

		Material();

		~Material();

		virtual void setRenderTechnique(IRenderTechnique* technique) { _technique = technique; }

		virtual IRenderTechnique* getRenderTechnique() { return _technique; }

		virtual RenderStateSet& getRenderStaeSet() { return _rss; }

		virtual ITextureUnitSet* getTextureUnitSet() { return _textureUnits; }

		virtual Value& getRenderParameter(const char* name);

		virtual IMaterial* clone();

	private:

		RenderStateSet _rss;
		ITextureUnitSet* _textureUnits;
		IRenderTechnique* _technique = nullptr;
		RenderScript* _renderScript = nullptr;

		std::map<std::string, Value> _parameters;
	};

	class ICamera;
	class IRenderTarget;

	class RenderTechnique : public IRenderTechnique PUBLIC_OBJECT_DEBUG {

	public:
		virtual void setCamera(ICamera* camera) { _camera = camera; }

		virtual ICamera* getCamera() { return _camera; }

		virtual void setRenderTarget(IRenderTarget* renderTarget) { _renderTarget = renderTarget; }

		virtual IRenderTarget* getRenderTarget() { return _renderTarget; }

		virtual void setRenderScript(const RenderScript& renderScript);

		virtual const RenderScript& getRenderScript() { return _renderScript; }

		virtual IProgram * getProgram();
	private:
		ICamera* _camera = nullptr;
		IRenderTarget* _renderTarget = nullptr;
		RenderScript _renderScript;

		IProgram* _program = nullptr;
	};
}

#endif