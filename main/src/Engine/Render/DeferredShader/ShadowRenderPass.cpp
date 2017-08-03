#include <common/interface/Scene/MeshInterface.h>
#include <RenderSystem/MaterialInterface.h>
#include <RenderSystem/RenderFactoryInterface.h>
#include <RenderSystem/RenderInterface.h>
#include <Scene/SceneFactoryInterface.h>

#include "ShadowRenderPass.h"
#include "GBufferRender.h"
#include "ShadowPass.h"

namespace wyin
{
	struct ShadowRenderPass::ShadowRenderPassRenderScheme : public IRenderScheme
	{
		virtual void begin(IRenderPool* renderPool)
		{
			int32 width = renderPool->getRenderTarget()->getWidth();
			int32 height = renderPool->getRenderTarget()->getHeight();
			renderPool->applyViewport(0, 0, width, height);

			_program->apply();

			RMatrix matrix = renderPool->getProjectMatrix() * renderPool->getViewMatrix();
			matrix.inverse();

			_program->setUniform16f("_mvpInverse", matrix);
		}

		virtual void applyMaterial(IMaterial* materail)
		{
			ITextureUnitSet* ts = materail->getTextureUnitSet();
			if (!ts)
				return;

			ts->apply(_program);
		}

		virtual void apply(const RMatrix& modelMatrix)
		{
		}
		IProgram* _program;
	};

	ShadowRenderPass::ShadowRenderPass()
		:_shadowRenderMaterial(NULL), _program(NULL), _screenRect(NULL)
	{
		_renderScheme = new ShadowRenderPassRenderScheme();
	}

	ShadowRenderPass::~ShadowRenderPass()
	{
		delete _renderScheme;
		delete _program;

		_screenRect = NULL;
		_shadowRenderMaterial = NULL;
	}

	void ShadowRenderPass::initProgram()
	{
		_program = RenderFactory::s_factory->createProgram();
		_program->loadFromFile("Shaders/ShadowRenderPass.vert", "Shaders/ShadowRenderPass.frag");

		_renderScheme->_program = _program;
	}

	void ShadowRenderPass::initScreenRect()
	{
		_screenRect = SceneFactory::s_factory->createMesh();

		Vec3f* vertex = new Vec3f[4];
		uint16* index = new uint16[6];

		vertex[0] = Vec3f(-1, -1, 0);
		vertex[1] = Vec3f(1, -1, 0);
		vertex[2] = Vec3f(1, 1, 0);
		vertex[3] = Vec3f(-1, 1, 0);

		index[0] = 0; index[1] = 1; index[2] = 2;
		index[3] = 0; index[4] = 2; index[5] = 3;

		_screenRect->begin(4);

		_screenRect->setVertexArray(vertex);
		_screenRect->addIndexArray(TRIANGLE_LIST, index, 6);

		_screenRect->end();
	}

	void ShadowRenderPass::initMaterial(GBuffer* gbuffer)
	{
		_shadowRenderMaterial = RenderFactory::s_factory->createMaterial();
		ITextureUnitSet* lightPassTextures = _shadowRenderMaterial->getTextureUnitSet();

		ITextureUnit* depth = RenderFactory::s_factory->createTextureUnit();
		depth->setTexture(gbuffer->depthTexture);
		lightPassTextures->addTextureUnit(depth, "_depthTexture");

		ITextureUnit* shadow_textureUnit = RenderFactory::s_factory->createTextureUnit();
		lightPassTextures->addTextureUnit(shadow_textureUnit, "_shadowTexture");

		_shadowRenderMaterial->setMode(BLEND, true);
	}

	void ShadowRenderPass::renderShadow(ILight* light, IShadowData* shadowData,GBuffer* gbuffer, IRenderPool* renderPool)
	{
		if (!_program)
		{
			initScreenRect();
			initProgram();
			initMaterial(gbuffer);
		}

		renderPool->applyRenderScheme(_renderScheme);
		RMatrix matrix;

		ShadowData* sd = (ShadowData*)shadowData;
		if (!sd)
			return;

		_program->setUniform16f("_lightShdowMapMVP", sd->lightVP);
		_program->setUniform3f("_shadowColor", Real3(0, 0, 0));

		_shadowRenderMaterial->getTextureUnitSet()->getTextureUnit(1)->setTexture(sd->shadowTexture);
		renderPool->applyMaterial(_shadowRenderMaterial);

		DrawInstance di;
		_screenRect->draw(renderPool,&di,1);
	}

	//////////////////////////////////////////
	struct ShadowRenderPass_Forward::ShadowRenderPassRenderScheme : public IRenderScheme
	{
		ShadowRenderPassRenderScheme()
		:_program(NULL), _renderPool(NULL)
		{
		}

		~ShadowRenderPassRenderScheme()
		{
			delete _program;
			_shadowRenderMaterial = NULL;
		}

		IProgram* _program;
		IRenderPool* _renderPool;
		Ptr<IMaterial> _shadowRenderMaterial;

		void init()
		{
			_program = RenderFactory::s_factory->createProgram();
			_program->loadFromFile("Shaders/ShadowRenderPass_forward.vert", "Shaders/ShadowRenderPass_forward.frag");

			_shadowRenderMaterial = RenderFactory::s_factory->createMaterial();
			ITextureUnitSet* us = _shadowRenderMaterial->getTextureUnitSet();
			ITextureUnit* unit = RenderFactory::s_factory->createTextureUnit();
			us->addTextureUnit(unit, "_shadowTexture");
		}

		virtual void begin(IRenderPool* renderPool)
		{
			if (!_program)
			{
				init();
			}
			_program->apply();
			_renderPool = renderPool;
		}

		virtual void applyMaterial(IMaterial* materail)
		{
		}

		virtual void apply(const RMatrix& modelMatrix)
		{
			RMatrix matrix = _renderPool->getProjectMatrix() * _renderPool->getViewMatrix() * modelMatrix;
			_program->setUniform16f("_mvp", matrix);
		}
	};

	ShadowRenderPass_Forward::ShadowRenderPass_Forward()
	{
		_renderScheme = new ShadowRenderPassRenderScheme();
	}

	ShadowRenderPass_Forward::~ShadowRenderPass_Forward()
	{
		delete _renderScheme;
	}

	void ShadowRenderPass_Forward::renderShaodw(IRenderPool* renderPool, ILight* light)
	{
		ShadowData* sd = (ShadowData*)light->getShadowData();
		if (!sd)
			return;

		renderPool->applyRenderScheme(_renderScheme);

		_renderScheme->_program->setUniform16f("_lightShdowMapMVP", sd->lightVP);
		_renderScheme->_program->setUniform3f("_shadowColor", Real3(0, 0, 0));

		_renderScheme->_shadowRenderMaterial->getTextureUnitSet()->getTextureUnit(0)->setTexture(sd->shadowTexture);
		renderPool->applyMaterial(_renderScheme->_shadowRenderMaterial);

		_renderScheme->_shadowRenderMaterial->getTextureUnitSet()->apply(_renderScheme->_program);
	}
}