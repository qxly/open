#include "ShadowRender.h"
#include "ShadowPass.h"
#include "ShadowRenderPass.h"
#include "Scene/NodeInterface.h"

namespace open
{
	ShadowRender::ShadowRender()
	{
		_shadowPass = new ShadowPass;
		_shadowRender_Forward = new ShadowRenderPass_Forward;
	}

	ShadowRender::~ShadowRender()
	{
		delete _shadowPass;
		delete _shadowRender_Forward;
	}

	void ShadowRender::renderShadow(IRenderPool* renderPool, INode** nodes, int32 size)
	{
		////³öÒõÓ°Í¼
		ILightManager* lightManager = renderPool->getLightManager();
		lightManager->begin();
		ILight* light = lightManager->getMainLight();
		if (!light)
			return;

		_shadowPass->applyLight(renderPool, light);
		const DrawableRenderTrace* drt = NULL;
		for (int32 i = 0; i < size; i++)
		{
			_shadowPass->applyNode(nodes[i]);
		}
		_shadowPass->genShadowMap();
	}
}