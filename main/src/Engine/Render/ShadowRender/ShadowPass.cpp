#include "ShadowPass.h"
#include "../DeferredShader/GBufferRender.h"
#include "ShadowRenderPass.h"

#include <common/interface/Scene/MeshInterface.h>
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>
#include <RenderSystem/RenderInterface.h>

#include <Scene/NodeInterface.h>
#include <Scene/SceneFactoryInterface.h>

#include <common/Algorithm.h>

namespace open
{
	struct ShadowPass::ShadowPassRenderScheme
	{
		virtual void begin(IRenderPool* renderPool)
		{
			renderPool->applyProgram(_program);
		}

		virtual void applyMaterial(IMaterial* materail)
		{
		}

		virtual void apply(const RMatrix& modelMatrix)
		{
			RMatrix matrix = _sd->lightProjectMatrix * _sd->lightViewMatrix * modelMatrix;
			_program->setUniform16f("_mvp", matrix);
		}
		IProgram* _program;
		ShadowData* _sd;
	};

	ShadowPass::ShadowPass()
		:_shadowPassMaterial(NULL), _program(NULL), _currentLight(NULL), _shadowData(NULL)
	{
		_renderScheme = new ShadowPassRenderScheme();
	}

	ShadowPass::~ShadowPass()
	{
		delete _program;
		delete _renderScheme;

		_shadowPassMaterial = NULL;
	}

	void ShadowPass::initProgram()
	{
		_program = RenderSystemFactory::s_factory->createProgram();
		_program->loadFromFile("Shaders/Depth.vert", "Shaders/Depth.frag");

		_renderScheme->_program = _program;
	}

	void ShadowPass::initShadowFrameBuffer()
	{

	}

	void ShadowPass::initShadowPassMaterial()
	{
		_shadowPassMaterial = RenderSystemFactory::s_factory->createMaterial();
		_shadowPassMaterial->setColorMask(0);
	}

	void ShadowPass::updateShadowData(ILight* light)
	{
		ShadowData* shadowData = _shadowData;

		RMatrix v;
		RMatrix p;
		computeLightCamera(light, v, p);

		shadowData->lightVP = p * v;
		shadowData->lightViewMatrix = v;
		shadowData->lightProjectMatrix = p;
	}

	void ShadowPass::applyLight(IRenderPool* renderPool,ILight* light)
	{
		_currentLight = light;
		_renderPool = renderPool;
		if (!_program)
		{
			initProgram();
			initShadowPassMaterial();
			initShadowData();
		}
	}

	void ShadowPass::genShadowMap()
	{
		updateShadowData(_currentLight);

		ShadowData* shadowData = _shadowData;

		_renderPool->applyRenderTarget(shadowData->frameBuffer);
		_renderPool->resetRenderState();
		_renderPool->clear(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT | COLOR_BUFFER_BIT);
		_renderPool->applyViewport(0, 0, 1024, 1024);

		_renderScheme->begin(_renderPool);
		_renderPool->applyMaterial(_shadowPassMaterial);
		_renderScheme->applyMaterial(_shadowPassMaterial);
		_renderScheme->_sd = shadowData;

		for (int32 i = 0; i < _shadowGeometries.size(); i++)
		{
			ShadowGeometry& sg = _shadowGeometries[i];

			const GeneralInstance* instances = sg.instanceInfos->getDataPointer();
			int32 size = sg.instanceInfos->size();

			sg.geometry->draw(_renderPool, instances[0].matrix.ptr(), size * instances->floatSize(), instances->instancesElementSize(),
				instances->elementFloatSize(), instances->offset(), instances->stride());

			delete sg.instanceInfos;
		}
	}

	void ShadowPass::applyNode(INode* node)
	{
		if (!(node->getRenderMask() & RENDER_CAST_SHADOW) != 0)
			return;

		_currentMatrix = node->getMatrix();

		_tempBox = node->getBoudingBox();
		_boundingBox.expandBy(_tempBox);

		node->getDrawable()->accept(functor(*this, &ShadowPass::insertGeometry));
	}

	void ShadowPass::insertGeometry(IGeometry* geometry, IMaterial*)
	{
		ShadowGeometry sg(geometry);
		GeneralInstance di;
		di.matrix = _currentMatrix;


		int32 find = lower_bound(_shadowGeometries.getDataPointer(), _shadowGeometries.size(), sg);
		if (find == -1 || find >= _shadowGeometries.size() || sg < _shadowGeometries[find])
		{
			sg.instanceInfos = new DynamicArray<GeneralInstance>;
			sg.instanceInfos->push_back(di);

			_shadowGeometries.push_back(sg);
		}
		else
		{
			_shadowGeometries[find].instanceInfos->push_back(di);
		}
	}

	void ShadowPass::computeLightCamera(ILight* light, RMatrix& viewMatrix, RMatrix& projectMatrix)
	{
		Real near_, far_;
		light->getLightRange(near_, far_);

		const Real3& light_p = light->getLightPosition();
		const Real3& light_d = light->getLightDirection();
		LightType lt = light->getLightType();

		if (lt == LIGHT_DIRECT)//方向光
		{
			Real3 center = light_p + light_d * far_;
			viewMatrix.makeLookAt(light_p, center, computeOrthogonalVector(-light_d));
			projectMatrix.makeOrtho(-far_, far_, -far_, far_, near_, far_);
		}
		else if (lt == LIGHT_POINT)//点光源
		{
			///构建6锥
		}
		else///锥光源
		{
			Real3 center = light_p + light_d * far_;
			viewMatrix.makeLookAt(light_p, center, computeOrthogonalVector(-light_d));
			projectMatrix.makeFrustum(-far_, far_, -far_, far_, near_, far_);
		}
	}

	Real3 ShadowPass::computeOrthogonalVector(const Real3& direction)
	{
		Real length = direction.length();
		Real3 orthogonalVector = direction ^ Real3(0.0f, 1.0f, 0.0f);
		if (orthogonalVector.normalize() < length * 0.5f)
		{
			orthogonalVector = direction ^ Real3(0.0f, 0.0f, 1.0f);
			orthogonalVector.normalize();
		}
		return orthogonalVector;
	}

	void ShadowPass::initShadowData()
	{
		_shadowData = new ShadowData();

		_shadowData->shadowTexture = RenderSystemFactory::s_factory->createTexture();
		_shadowData->shadowTexture->alloc(PIXELINTERNALFORMAT_DEPTH_COMPONENT32, 1024, 1024, 0, PIXEFORMAT_DEPTH_COMPONENT, DATATYPE_FLOAT, NULL);

		_shadowData->frameBuffer = RenderSystemFactory::s_factory->createFrameBuffer();
		_shadowData->frameBuffer->attach(DEPTH_ATTACHMENT, _shadowData->shadowTexture);
	}
}