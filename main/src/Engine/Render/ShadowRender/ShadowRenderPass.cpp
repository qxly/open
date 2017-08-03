#include <common/interface/Scene/MeshInterface.h>
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>
#include <RenderSystem/RenderInterface.h>


#include "ShadowRenderPass.h"
#include "../DeferredShader/GBufferRender.h"
#include "ShadowPass.h"
#include <Scene/SceneFactoryInterface.h>
#include <Scene/NodeInterface.h>

#include "../../InstanceInfo.h"

namespace open
{
	/*struct ShadowRenderPass_Deferred::ShadowRenderPassRenderScheme : public IRenderScheme
	{x
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

	ShadowRenderPass_Deferred::ShadowRenderPass_Deferred()
	:_shadowRenderMaterial(NULL), _program(NULL), _screenRect(NULL)
	{
	_renderScheme = new ShadowRenderPassRenderScheme();
	}

	ShadowRenderPass_Deferred::~ShadowRenderPass_Deferred()
	{
	delete _renderScheme;
	delete _program;

	_screenRect = NULL;
	_shadowRenderMaterial = NULL;
	}

	void ShadowRenderPass_Deferred::initProgram()
	{
	_program = RenderSystemFactory::s_factory->createProgram();
	_program->loadFromFile("Shaders/ShadowRenderPass.vert", "Shaders/ShadowRenderPass.frag");

	_renderScheme->_program = _program;
	}

	void ShadowRenderPass_Deferred::initScreenRect()
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

	void ShadowRenderPass_Deferred::initMaterial(GBuffer* gbuffer)
	{
	_shadowRenderMaterial = RenderSystemFactory::s_factory->createMaterial();
	ITextureUnitSet* lightPassTextures = _shadowRenderMaterial->getTextureUnitSet();

	ITextureUnit* depth = lightPassTextures->getTextureUnit(UNKOWN_LAYER1);
	lightPassTextures->setTexutreUnitName(UNKOWN_LAYER1, "_depthTexture");

	depth->setTexture(gbuffer->depthTexture);

	ITextureUnit* shadow_textureUnit = lightPassTextures->getTextureUnit(UNKOWN_LAYER2);
	lightPassTextures->setTexutreUnitName(UNKOWN_LAYER2, "_shadowTexture");

	_shadowRenderMaterial->setMode(BLEND, true);
	}

	void ShadowRenderPass_Deferred::renderShadow(ILight* light, IShadowData* shadowData, GBuffer* gbuffer, IRenderPool* renderPool)
	{
	if (!_program)
	{
	initScreenRect();
	initProgram();
	initMaterial(gbuffer);
	}

	renderPool->applyProgram(_renderScheme);
	RMatrix matrix;

	ShadowData* sd = (ShadowData*)shadowData;
	if (!sd)
	return;

	_program->setUniform16f("_lightShdowMapMVP", sd->lightVP);
	_program->setUniform3f("_shadowColor", Real3(0, 0, 0));

	_shadowRenderMaterial->getTextureUnitSet()->getTextureUnit(UNKOWN_LAYER2)->setTexture(sd->shadowTexture);
	renderPool->applyMaterial(_shadowRenderMaterial);

	GeneralInstance di;
	_screenRect->draw(renderPool,&di,1);
	}*/

	//////////////////////////////////////////
	struct ShadowRenderPass_Forward::ShadowRenderPassRenderScheme
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
			_program = RenderSystemFactory::s_factory->createProgram();
			_program->loadFromFile("Shaders/ShadowRenderPass_forward.vert", "Shaders/ShadowRenderPass_forward.frag");

			_shadowRenderMaterial = RenderSystemFactory::s_factory->createMaterial();
			ITextureUnitSet* us = _shadowRenderMaterial->getTextureUnitSet();
			ITextureUnit* unit = us->getTextureUnit(UNKOWN_LAYER1);
			us->setTexutreUnitName(UNKOWN_LAYER1, "_shadowTexture");
		}

		virtual void begin(IRenderPool* renderPool)
		{
			if (!_program)
			{
				init();
			}
			renderPool->applyProgram(_program);
			_renderPool = renderPool;
		}

		virtual void applyMaterial(IMaterial* materail)
		{
			materail->getTextureUnitSet()->apply(_program);
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

	void ShadowRenderPass_Forward::renderShaodw(IRenderPool* renderPool, ILight* light, INode** nodes, int32 size)
	{
		ShadowData* sd = (ShadowData*)light->getShadowData();
		if (!sd)
			return;

		_renderScheme->begin(renderPool);

		_renderScheme->_program->setUniform16f("_lightShdowMapMVP", sd->lightVP);
		_renderScheme->_program->setUniform3f("_shadowColor", Real3(0, 0, 0));

		_renderScheme->_shadowRenderMaterial->getTextureUnitSet()->getTextureUnit(UNKOWN_LAYER1)->setTexture(sd->shadowTexture);
		renderPool->applyMaterial(_renderScheme->_shadowRenderMaterial);
		_renderScheme->applyMaterial(_renderScheme->_shadowRenderMaterial);
		/////
		for (int32 i = 0; i < size; i++)
		{
			INode* node = nodes[i];
			if ((node->getRenderMask() & RENDER_RECEIVES_SHADOW) != 0)
			{
				IDrawable* drawable = node->getDrawable();

				////
				_currentDrawInstance->matrix = renderPool->getProjectMatrix() * renderPool->getViewMatrix() * node->getMatrix();
				_currentDrawInstance->matrix = _currentDrawInstance->matrix.getTranspose();
			}
		}
	}
}