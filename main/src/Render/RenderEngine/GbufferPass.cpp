#include <open/Ptr.h>

#include "DeferredUtil.h"
#include "DefalutSchema.h"

#include <Render/RenderEngine.h>

namespace open
{
	namespace DeferredUtil
	{
		GBuffer g_buffer;
	}
	using namespace DeferredUtil;
	const RenderScript& genGBufferPass()
	{
		static OutFrag frags[] = {
			OutFrag("_normal_shininess",1),
			OutFrag("_diffuse_specular",2),
			OutFrag("_emissive_materailMask",3),
			OutFrag(0)
		};

		static Shader shader[] = {
			Shader("res/shaders/GBuffer.vert" , "res/shaders/GBuffer.frag")
		};

		static UniformDetial uds[] = {
			UniformDetial("_shininess",2 ,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(material->getRenderStaeSet().getShininess() / 128.0f);
		}),
			UniformDetial("_diffuseColor",4,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(Real3::fromRGB(material->getRenderStaeSet().getDiffuseColor()));
		}),
			UniformDetial("_emissiveColor", 4,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(Real3::fromRGB(material->getRenderStaeSet().getEmissiveColor()));
		}),
			UniformDetial("_diffuseTexture",1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(0);
		}),
			UniformDetial("_emissiveTexture", 1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(1);
		}),
			UniformDetial(0)
		};

		static RenderScript rps;
		rps.shader = shader;
		rps.outFrags = frags;
		rps.uniformDetials = uds;
		rps.name = "GenGBuff";

		return rps;
	}

	IRenderTarget* genGBufferTarget() {

		IFrameBuffer* frameBuffer = RenderSystemFactory::s_factory->createFrameBuffer();

		g_buffer.depthTexture = RenderSystemFactory::s_factory->createTexture();
		g_buffer.normal_shininessTexture = RenderSystemFactory::s_factory->createTexture();
		g_buffer.diffuse_specularTexture = RenderSystemFactory::s_factory->createTexture();
		g_buffer.emissive_materialTexture = RenderSystemFactory::s_factory->createTexture();

		frameBuffer->attach(DEPTH_ATTACHMENT, g_buffer.depthTexture);
		frameBuffer->attach(COLOR_ATTACHMENT0, g_buffer.normal_shininessTexture);
		frameBuffer->attach(COLOR_ATTACHMENT1, g_buffer.diffuse_specularTexture);
		frameBuffer->attach(COLOR_ATTACHMENT2, g_buffer.emissive_materialTexture);

		return frameBuffer;
	}

	void updateGBuffer(IRenderTarget* renderTarget, IRenderPool* renderPool) {

		static int width = 0;
		static int height = 0;

		int32 w = renderPool->getDefaultRenderTarget()->getWidth();
		int32 h = renderPool->getDefaultRenderTarget()->getHeight();

		if (width == w && height == h)
			return;

		width = w;
		height = h;
		g_buffer.depthTexture->alloc(PIXELINTERNALFORMAT_DEPTH_COMPONENT32, width, height, 0, PIXEFORMAT_DEPTH_COMPONENT, DATATYPE_FLOAT, nullptr);
		g_buffer.normal_shininessTexture->alloc(PIXELINTERNALFORMAT_RGB, width, height, 0, PIXEFORMAT_RGB, DATATYPE_UNSIGNED_BYTE, nullptr);
		g_buffer.diffuse_specularTexture->alloc(PIXELINTERNALFORMAT_RGB, width, height, 0, PIXEFORMAT_RGB, DATATYPE_UNSIGNED_BYTE, nullptr);
		g_buffer.emissive_materialTexture->alloc(PIXELINTERNALFORMAT_RGB, width, height, 0, PIXEFORMAT_RGB, DATATYPE_UNSIGNED_BYTE, nullptr);
	}
}