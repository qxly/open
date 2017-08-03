#include "DeferredUtil.h"
#include "DefalutSchema.h"

#include <open/Ptr.h>

namespace open
{
	using namespace DeferredUtil;
	const RenderScript& genEmissivePass()
	{
		static Shader shader[] = {
			Shader("res/shaders/LightPass.vert", "res/shaders/LightPass.frag", "#define EMISSIVE_RENDER\n")
		};

		static UniformDetial uds[] = {
			UniformDetial("_screenSize",3,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(Real2((Real)renderPool->getRenderTarget()->getWidth(), (Real)renderPool->getRenderTarget()->getHeight()));
		}),
			UniformDetial("_depthTexture",1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(0);
		}),
			UniformDetial("_emissiveTexture",1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(1);
		}),
			UniformDetial(0)
		};

		static RenderScript rps;
		rps.shader = shader;
		rps.uniformDetials = uds;


		return rps;
	}

	IMaterial* genEmissiveMaterial() {
		IMaterial* material = RenderFactory::s_factory->createMaterial();
		ITextureUnit* tu = material->getTextureUnitSet()->createTextureUnit(0, "_depthTexture");
		tu->setTexture(g_buffer.depthTexture);

		tu = material->getTextureUnitSet()->createTextureUnit(1, "_emissiveTexture");
		tu->setTexture(g_buffer.emissive_materialTexture);

		return material;
	}
}