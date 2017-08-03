#include "DeferredUtil.h"
#include "DefalutSchema.h"

#include <open/Ptr.h>

namespace open
{
	using namespace DeferredUtil;

	void genPointLightInfo(PointLightInfo& info, ILight* light, IRenderPool* renderPool) {
		info.light = light;
		info.lightPosition = light->getLightPosition();
		Real f, n;
		light->getLightRange(n, f);
		info.f = f;

		RBoundingBox aabb;
		info.lightAABB.expandBy(info.lightPosition, f);

		Matrixf t;
		Matrixf s;
		t.makeTranslate(info.lightPosition);
		s.makeScale(f, f, f);
		info.lightTranse = t * s;

		info.lightMVP = renderPool->getProjectMatrix() * renderPool->getViewMatrix() * info.lightTranse;
	}

	const RenderScript& genPointLightPass()
	{
		static Shader shader[] = {
			Shader("res/shaders/LightPass.vert", "res/shaders/LightPass.frag", "#define POINT_LIGHT\n")
		};

		static UniformDetial uds[] = {
			UniformDetial("_mvp",6,[](void* data,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			PointLightInfo* info = (PointLightInfo*)data;

			value.set(info->lightMVP);
		}),
			UniformDetial("_mv",6,[](void* data,IMaterial* material ,IRenderPool* renderPool, Value& value) {

			PointLightInfo* info = (PointLightInfo*)data;

			value.set(renderPool->getViewMatrix() * info->lightTranse);
		}),
			UniformDetial("_mvpInverse",6,[](void* data,IMaterial* material ,IRenderPool* renderPool, Value& value) {

			PointLightInfo* info = (PointLightInfo*)data;

			RMatrix mm(info->lightMVP);
			mm.inverse();

			value.set(mm);
		}),
			UniformDetial("_lightPoint_eye",4,[](void* data,IMaterial* material ,IRenderPool* renderPool, Value& value) {

			PointLightInfo* info = (PointLightInfo*)data;

			value.set(renderPool->getViewMatrix() * info->lightPosition);
		}),
			UniformDetial("_lightRange",2,[](void* data,IMaterial* material ,IRenderPool* renderPool, Value& value) {

			PointLightInfo* info = (PointLightInfo*)data;

			value.set(info->f * 2);
		}),
			UniformDetial("_light_ambientColor",4,[](void* data,IMaterial* material , IRenderPool* renderPool, Value& value) {

			PointLightInfo* info = (PointLightInfo*)data;

			value.set(info->light->getAmbientColor());
		}),
			UniformDetial("_light_diffuseColor", 4,[](void* data,IMaterial* material , IRenderPool* renderPool, Value& value) {

			PointLightInfo* info = (PointLightInfo*)data;

			value.set(info->light->getDiffuseColor());
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

	IMaterial* genPointLightMaterial() {

		IMaterial* material = RenderFactory::s_factory->createMaterial();

		material->getRenderStaeSet().setMode(BLEND, true);
		material->getRenderStaeSet().setBlendEquationSeparate(BLENDEQUATION_ADD, BLENDEQUATION_ADD);
		material->getRenderStaeSet().setBlendFunSeparate(BLENDFUN_ONE, BLENDFUN_ONE, BLENDFUN_ONE, BLENDFUN_ONE);

		bindGBufferTexture(&DeferredUtil::g_buffer, material->getTextureUnitSet());

		return material;
	}
}