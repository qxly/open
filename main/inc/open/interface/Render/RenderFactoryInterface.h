#ifndef RENDER_RENDERFACTORY_INTERFACE_H
#define RENDER_RENDERFACTORY_INTERFACE_H

#include <open/openDef.h>

namespace open
{
	class Config;
	class IMesh;
	class IText;
	class ISolidGeometry;
	class IDrawable;
	class IDrawableBase;

	class IRenderTechnique;
	class IMaterial;
	class ICamera;
	class IRenderEngine;

	class COMMON_API RenderFactory
	{
	public:
		static RenderFactory* s_factory;

		virtual IMesh* createMesh() = 0;

		virtual ISolidGeometry* createSolidGeometry() = 0;

		virtual IDrawableBase* createDrawable() = 0;

		virtual IText* createText() = 0;

		virtual IDrawable* createDrawable(const char* file, const Config& config) = 0;

		virtual IRenderTechnique* createRenderTechnique() = 0;

		virtual IMaterial* createMaterial(const char* renderSchema = nullptr) = 0;

		virtual IMaterial* getMaterial(const char* name) = 0;

		virtual IMaterial* getOrCreateMaterial(const char* name, const char* renderSchema = nullptr) = 0;

		virtual IRenderEngine* getRenderEngine() = 0;

		virtual ICamera* createCamera() = 0;
	};
}

#endif