#include "Render.h"

#include "RenderSorter.h"
#include "DeferredShader/DeferredShader.h"
#include "ForwardRender/ForwardShader.h"
#include "ShadowRender/ShadowRender.h"

#include <Scene/NodeInterface.h>
#include <RenderSystem/RenderStatesInterface.h>
#include "RenderSystem/RenderEnumeration.h"

namespace open
{
	Render::Render()
	{
		_renderSort = new SimpleRenderSorter;
		_deferredShader = new DeferredShader;
		_forwardShader = new ForwardShader;
		_shadowRender = new ShadowRender;
	}

	Render::~Render()
	{
		delete _renderSort;
		delete _deferredShader;
		delete _forwardShader;
		delete _shadowRender;
	}

	void Render::render(ICamera* camera, const DynamicArray<INode*>& drts)
	{
		_renderSort->begin();
		_renderSort->sort(camera, const_cast<INode**>(drts.getDataPointer()), drts.size(), false);
		RenderTraceArray& rts = _renderSort->end();
		RenderTraceArray& rts2 = _renderSort->endTransparents();

		_renderPool->beginStatisticsBatch();

		renderObjects(camera, rts.getDataPointer(), rts.size(), rts2.getDataPointer(), rts2.size());

		printf("frame batch is: %d\n", _renderPool->endStatisticsBatch());

		//renderShadow(drts);
		_deferredShader->renderGBuffer();
	}

	void Render::renderObjects(ICamera* camera, RenderTrace* rts, int32 size, RenderTrace* transparents, int32 transparentsSize)
	{
		_renderPool->setViewMatrix(camera->getViewMatrix());
		_renderPool->setProjectMatrix(camera->getProject());

		////—”≥Ÿ‰÷»æ
		if (size > 0)
		{
			_deferredShader->genGBufferPass(_renderPool, _renderTarget->getWidth(), _renderTarget->getHeight());
			IApplyMaterialFun& amf = _deferredShader->getGBufferApplyMaterialFun();

			for (int32 i = 0; i < size; i++)
			{
				const RenderTrace& rt = rts[i];
				_renderPool->applyMaterial(rt.material, amf);
				for (int32 j = 0; j < rt.geometries->size(); j++)
				{
					GeometryInstance& gi = rt.geometries->at(j);
					
					const GeneralInstance* instances = gi.instanceInfos->getDataPointer();
					int32 size = gi.instanceInfos->size();

					gi.geometry->draw(_renderPool, instances[0].matrix.ptr(), size * instances->floatSize(), instances->instancesElementSize(),
						instances->elementFloatSize(), instances->offset(), instances->stride());
				}
			}
			_renderPool->applyRenderTarget(_renderTarget);
			_renderPool->resetRenderState();
			_renderPool->clear(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT | COLOR_BUFFER_BIT);
			_deferredShader->processLightPass(camera, _renderPool);
		}

		///«∞œÚ‰÷»æ
		if (transparentsSize > 0)
		{
			_renderPool->resetRenderState();
			_forwardShader->begin(_renderPool);

			ILightManager* lightManager = _renderPool->getLightManager();
			ILight* light = lightManager->getMainLight();

			IApplyMaterialFun& amf = _forwardShader->getApplyMaterialFun();
			for (int32 i = 0; i < transparentsSize; i++)
			{
				const RenderTrace& rt = transparents[i];
				_renderPool->applyMaterial(rt.material, amf);

				if (rt.material->getMode(LIGHT))
				{
					_forwardShader->apply(light, rt.material);
					for (int32 j = 0; j < rt.geometries->size(); j++)
					{
						GeometryInstance& gi = rt.geometries->at(j);

						const GeneralInstance* instances = gi.instanceInfos->getDataPointer();
						int32 size = gi.instanceInfos->size();

						gi.geometry->draw(_renderPool, instances[0].matrix.ptr(), size * instances->floatSize(), instances->instancesElementSize(),
							instances->elementFloatSize(), instances->offset(), instances->stride());
					}
				}
				else
				{
					for (int32 j = 0; j < rt.geometries->size(); j++)
					{
						GeometryInstance& gi = rt.geometries->at(j);

						const GeneralInstance* instances = gi.instanceInfos->getDataPointer();
						int32 size = gi.instanceInfos->size();

						gi.geometry->draw(_renderPool, instances[0].matrix.ptr(), size * instances->floatSize(), instances->instancesElementSize(),
							instances->elementFloatSize(), instances->offset(), instances->stride());

					}
				}
			}
		}

		////‰÷»æŒƒ◊÷

		///‰÷»æ¡£◊”

		////Ãÿ ‚‰÷»æ
	}

	void Render::renderShadow(const DynamicArray<INode*>& drts)
	{
		//_shadowRender->renderShadow(_renderPool, const_cast<INode**>(drts.getDataPointer()), drts.size());
		////—”≥Ÿ‰÷»æ
		//_deferredShader->renderShadow(_renderPool, const_cast<INode**>(drts.getDataPointer()), drts.size());

		///«∞œÚ‰÷»æ
		/*ILightManager* lightManager = _renderPool->getLightManager();
		lightManager->begin();

		ILight* light = NULL;
		const DynamicArray<DrawableRenderTrace>& drts = _drts->drts;

		while(light = lightManager->nextIterator())
		{
		_deferredShader->applyShadowRender_ForwardRender(_renderPool,light);

		for(int32 i =0; i < drts.size(); i++)
		{
		const DrawableRenderTrace& drt = drts[i];
		if(! ( drt.node->getRenderMask() & RENDER_RECEIVES_SHADOW) != 0 )
		continue;

		_renderPool->apply(drt.drawable,drt.modelMatrix);
		}
		}*/
	}
}