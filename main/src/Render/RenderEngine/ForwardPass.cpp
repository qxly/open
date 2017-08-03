#include "DeferredUtil.h"
#include "DefalutSchema.h"

#include <open/Ptr.h>

namespace open
{
	Real3 computeLightDir(ILight* light, const RMatrix& viewMatrix) {
		Real f, n;
		light->getLightRange(n, f);

		if (light->getLightType() == LIGHT_DIRECT)
		{
			Real3 lp_3 = light->getLightDirection();
			lp_3 *= -1;

			RQuat nn = viewMatrix.getRotate();
			lp_3 = nn * lp_3;
			lp_3.normalize();
			return lp_3;
		}
		else if (light->getLightType() == LIGHT_POINT)
		{
			Real3 lp_3 = light->getLightPosition();
			lp_3 = viewMatrix * lp_3;

			return lp_3;
		}
	}

	using namespace DeferredUtil;
	const RenderScript& genForwardPass()
	{
		static Shader shader[] = {
			Shader("res/shaders/Basic.vert", "res/shaders/Basic.frag")
		};

		static UniformDetial uds[] = {
			UniformDetial("_lightPosition",4,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(computeLightDir(renderPool->getLightManager()->getMainLight(),renderPool->getViewMatrix()));
		}),
			UniformDetial("_projectInverse",6,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			RMatrix pro = renderPool->getProjectMatrix();
			pro.inverse();
			value.set(pro);
		}),
			UniformDetial("_lightRange",2,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			ILight* light = renderPool->getLightManager()->getMainLight();
			if (light->getLightType() == LIGHT_POINT) {
				Real n, f;
				light->getLightRange(n, f);
				value.set(f);
			}
			else {
				value.set(-1.0f);
			}
		}),
			UniformDetial("_light_diffuseColor",4,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(renderPool->getLightManager()->getMainLight()->getDiffuseColor());
		}),
			UniformDetial("_light_ambientColor",4,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(renderPool->getLightManager()->getMainLight()->getAmbientColor());
		}),
			UniformDetial("_shininess",2 ,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(material->getRenderStaeSet().getShininess() / 128.0f);
		}),
			UniformDetial("_diffuseColor",4,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(Real3::fromRGB(material->getRenderStaeSet().getDiffuseColor()));
		}),
			UniformDetial("_emissiveColor",4,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(Real3::fromRGB(material->getRenderStaeSet().getEmissiveColor()));
		}),
			UniformDetial("_diffuseTexture",1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(0);
		}),
			UniformDetial("_emissiveTexture", 1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(1);
		}),
			UniformDetial("_opacityTexture", 1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(2);
		}),
			UniformDetial("_hasOpacityTexture", 1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(material->getTextureUnitSet()->hasTextureUnit(2));
		}),
			UniformDetial(0)
		};

		static RenderScript rps;
		rps.shader = shader;
		rps.uniformDetials = uds;
		auto verifyMaterialHandle = [](IMaterial* material) {
			return material->getRenderStaeSet().getMode(BLEND);
		};

		return rps;
	}
}