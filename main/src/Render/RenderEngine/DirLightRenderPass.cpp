#include "DeferredUtil.h"
#include "DefalutSchema.h"

#include <open/Ptr.h>

namespace open
{
	using namespace DeferredUtil;
	const RenderScript& genDirLightPass()
	{
		static Shader shader[] = {
			Shader("res/shaders/LightPass.vert", "res/shaders/LightPass.frag", "#define DIRECT_LIGHT\n")
		};

		static UniformDetial uds[] = {
			UniformDetial("_lightDir_eye",4 ,[](void* data ,IMaterial* material , IRenderPool* renderPool,Value& value) {

			ILight* light = (ILight*)data;
			const RMatrix& viewMatrix = renderPool->getViewMatrix();
			Real3 lp_3 = light->getLightDirection();
			lp_3 *= -1;

			RQuat nn = viewMatrix.getRotate();
			lp_3 = nn * lp_3;
			lp_3.normalize();

			value.set(lp_3);
		}),
			UniformDetial("_light_ambientColor",4,[](void* data,IMaterial* material , IRenderPool* renderPool, Value& value) {
			ILight* light = (ILight*)data;
			value.set(light->getAmbientColor());
		}),
			UniformDetial("_light_diffuseColor", 4,[](void* data,IMaterial* material , IRenderPool* renderPool, Value& value) {
			ILight* light = (ILight*)data;
			value.set(light->getDiffuseColor());
		}),
			UniformDetial("_screenSize",3,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(Real2((Real)renderPool->getRenderTarget()->getWidth(), (Real)renderPool->getRenderTarget()->getHeight()));
		}),
			UniformDetial("_depthTexture",1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(0);
		}),
			UniformDetial("_normal_shininessTexture",1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(1);
		}),
			UniformDetial("_diffuse_specularTexture",1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(2);
		}),
			UniformDetial(0)
		};

		static RenderScript rps;
		rps.shader = shader;
		rps.uniformDetials = uds;

		return rps;
	}

	IMaterial* genDirLightMaterial() {

		IMaterial* material = RenderFactory::s_factory->createMaterial();

		material->getRenderStaeSet().setMode(BLEND, true);
		material->getRenderStaeSet().setBlendEquationSeparate(BLENDEQUATION_ADD, BLENDEQUATION_ADD);
		material->getRenderStaeSet().setBlendFunSeparate(BLENDFUN_ONE, BLENDFUN_ONE, BLENDFUN_ONE, BLENDFUN_ONE);

		bindGBufferTexture(&DeferredUtil::g_buffer, material->getTextureUnitSet());

		return material;
	}
}