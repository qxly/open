#include <Render/RenderFactoryInterface.h>

#include "Mesh/Drawable.h"
#include "Mesh/Mesh.h"
#include "Text/Text.h"
#include "Mesh/SolidDrawable.h"

#include "Material/Material.h"

#include "RenderEngine/RenderEngine.h"
#include "RenderEngine/Camera.h"
#include "RenderEngine/DefalutSchema.h"

#include <map>
#include <open/Ptr.h>


namespace open
{
	class RenderFactory_Internal : public RenderFactory
	{
	public:

		RenderFactory_Internal()
		{
			s_factory = this;
			_renderEngine = new RenderEngine;
		}

		~RenderFactory_Internal()
		{
			delete _renderEngine;
		}

		virtual IMesh* createMesh() { return new Mesh; }

		virtual ISolidGeometry* createSolidGeometry() { return new SolidGeometry; }

		virtual IDrawableBase* createDrawable() { return new Drawable; }

		virtual IDrawable* createDrawable(const char* file, const Config& config) {
			return DrawableReadWriteHelper::readDrawable(file, config);
		}

		virtual IText* createText() { return new Text; }

		virtual IMaterial* createMaterial(const char* renderSchemaName)
		{
			Material* material = new Material;
			return material;
		}

		virtual IMaterial* getMaterial(const char* name)
		{
			std::map<std::string, Ptr<IMaterial>>::iterator it = _materials.find(name);
			if (it != _materials.end())
				return it->second;

			return nullptr;
		}

		virtual IMaterial* getOrCreateMaterial(const char* name, const char* renderSchema)
		{
			std::map<std::string, Ptr<IMaterial>>::iterator it = _materials.find(name);
			if (it != _materials.end())
				return it->second;

			IMaterial* material = createMaterial(renderSchema);
			if (!material)
				return nullptr;

			_materials[name] = material;
			return material;
		}

		virtual IRenderTechnique* createRenderTechnique() { return new RenderTechnique; }

		virtual IRenderEngine* getRenderEngine() { return _renderEngine; }

		virtual ICamera* createCamera() { return new Camera; }

	private:

		std::map<std::string, Ptr<IMaterial>> _materials;

		RenderEngine* _renderEngine;
	};

	struct Init_RenderFactory_Internal
	{
		Init_RenderFactory_Internal()
		{
			RenderFactory_Internal::s_factory = new RenderFactory_Internal();
		}

		~Init_RenderFactory_Internal()
		{
			delete RenderFactory_Internal::s_factory;
		}
	};
	static Init_RenderFactory_Internal g_Init_RenderFactory_Internal;
}