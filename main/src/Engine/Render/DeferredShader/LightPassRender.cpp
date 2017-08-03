#include "LightPassRender.h"
#include "GBufferRender.h"

#include <common/Array.h>
#include <common/interface/Scene/MeshInterface.h>
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>
#include <RenderSystem/RenderInterface.h>
#include <Scene/SceneFactoryInterface.h>
#include <Scene/NodeInterface.h>

namespace open
{
	LightPassRender::LightPassRender()
		:_screenRect(NULL),
		_programLightDir(NULL), _programLightPoint(NULL), _programNull(NULL), _programEmissive(NULL)
	{
	}

	LightPassRender::~LightPassRender()
	{
		delete _programNull;
		delete _programLightDir;
		delete _programLightPoint;
		delete _programEmissive;

		_screenRect = NULL;
		_dirLightPassMaterial = NULL;
	}

	void LightPassRender::initProgram()
	{
		_programLightDir = RenderSystemFactory::s_factory->createProgram();
		_programLightDir->loadFromFile("Shaders/LightPass.vert", "Shaders/LightPass.frag", "#define DIRECT_LIGHT\n");

		_programLightPoint = RenderSystemFactory::s_factory->createProgram();
		_programLightPoint->loadFromFile("Shaders/LightPass.vert", "Shaders/LightPass.frag", "#define POINT_LIGHT\n");

		_programNull = RenderSystemFactory::s_factory->createProgram();
		_programNull->loadFromFile("Shaders/Null.vert", "Shaders/Null.frag");

		_programEmissive = RenderSystemFactory::s_factory->createProgram();
		_programEmissive->loadFromFile("Shaders/LightPass.vert", "Shaders/LightPass.frag", "#define EMISSIVE_RENDER\n");
	}

	void LightPassRender::initScreenRect()
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

	void LightPassRender::initSphere()
	{
		_sphere = SceneFactory::s_factory->createMesh();

		DynamicArray<Vec3f> verts;

		int32 numSegments = 20;
		int32 numRows = 20;
		float ratio = 3;

		float lDelta = PI / (float)numRows;
		float vDelta = 1.0f / (float)numRows;

		float angleDelta = PI * 2.0f / (float)numSegments;
		float texCoordHorzDelta = 1.0f / (float)numSegments;


		float lBase = -PI * 0.5f;
		float rBase = 0.0f;
		float zBase = -1;
		float vBase = 0.0f;
		float nzBase = -1.0f;
		float nRatioBase = 0.0f;

		for (int32 rowi = 0; rowi < numRows; ++rowi)
		{
			float lTop = lBase + lDelta;
			float rTop = cos(lTop);
			float zTop = sin(lTop);
			float vTop = vBase + vDelta;
			float nzTop = sin(lTop);
			float nRatioTop = cos(lTop);

			float angle = 0.0f;

			for (int32 topi = 0; topi < numSegments; ++topi, angle += angleDelta)
			{
				float c = cos(angle);
				float s = sin(angle);

				verts.push_back(Vec3f(c * rTop, s * rTop, zTop));
				verts.push_back(Vec3f(c * rBase, s * rBase, zBase));
			}

			lBase = lTop;
			rBase = rTop;
			zBase = zTop;
			vBase = vTop;
			nzBase = nzTop;
			nRatioBase = nRatioTop;
		}

		int32 size = verts.size();

		_sphere->begin(size);
		_sphere->setVertexArray(verts.getDataPointer());
		_sphere->end();
		_sphere->addIndexArray(TRIANGLE_STRIP, 0, size);

		verts.detachPointer();
	}

	void LightPassRender::bindGBufferTexture(GBuffer* gbuffer, IMaterial* material)
	{
		ITextureUnitSet* lightPassTextures = material->getTextureUnitSet();

		ITextureUnit* depth = lightPassTextures->getTextureUnit(DIFFUSE_LAYER);
		depth->setTexture(gbuffer->depthTexture);
		lightPassTextures->setTexutreUnitName(DIFFUSE_LAYER, "_depthTexture");

		ITextureUnit* normal_shininess = lightPassTextures->getTextureUnit(EMISSIVE_LAYER);
		normal_shininess->setTexture(gbuffer->normal_shininessTexture);
		lightPassTextures->setTexutreUnitName(EMISSIVE_LAYER, "_normal_shininessTexture");

		ITextureUnit* diffuse_specular = lightPassTextures->getTextureUnit(OPACITY_LAYER);
		diffuse_specular->setTexture(gbuffer->diffuse_specularTexture);
		lightPassTextures->setTexutreUnitName(OPACITY_LAYER, "_diffuse_specularTexture");
	}

	void LightPassRender::initMaterial(GBuffer* gbuffer)
	{
		///自发光
		_emissiveRenderMaterial = RenderSystemFactory::s_factory->createMaterial();
		ITextureUnitSet* tus = _emissiveRenderMaterial->getTextureUnitSet();
		ITextureUnit* tu = tus->getTextureUnit(EMISSIVE_LAYER);
		tu->setTexture(gbuffer->emissive_materialTexture);
		tu = tus->getTextureUnit(DIFFUSE_LAYER);
		tu->setTexture(gbuffer->depthTexture);
		tus->setTexutreUnitName(DIFFUSE_LAYER, "_depthTexture");


		///方向光
		_dirLightPassMaterial = RenderSystemFactory::s_factory->createMaterial();
		_dirLightPassMaterial->setMode(BLEND, true);
		_dirLightPassMaterial->setBlendEquationSeparate(BLENDEQUATION_ADD, BLENDEQUATION_ADD);
		_dirLightPassMaterial->setBlendFunSeparate(BLENDFUN_ONE, BLENDFUN_ONE, BLENDFUN_ONE, BLENDFUN_ONE);
		bindGBufferTexture(gbuffer, _dirLightPassMaterial);

		///点光源
		_pointLightStencilPassMaterial = RenderSystemFactory::s_factory->createMaterial();
		/*_pointLightStencilPassMaterial->setMode(DEPTH_TEST, true);
		_pointLightStencilPassMaterial->setMode(CULL_FACE, false);
		_pointLightStencilPassMaterial->setMode(STENCIL_TEST, true);
		_pointLightStencilPassMaterial->setColorMask(0);
		_pointLightStencilPassMaterial->setDepthMask(false);
		_pointLightStencilPassMaterial->setStencilFunction(ALWAYS);
		_pointLightStencilPassMaterial->setStencilFunMask(0);
		_pointLightStencilPassMaterial->setStencilFunRef(0);
		_pointLightStencilPassMaterial->setStencilOperation(STENCILOPERATION_KEEP, STENCILOPERATION_INCR_WRAP, STENCILOPERATION_KEEP, FACE_BACK);
		_pointLightStencilPassMaterial->setStencilOperation(STENCILOPERATION_KEEP, STENCILOPERATION_DECR_WRAP, STENCILOPERATION_KEEP, FACE_FRONT);
		*/
		_pointLightPassMaterial = _pointLightStencilPassMaterial->clone();
		_pointLightPassMaterial->setMode(BLEND, true);
		//_pointLightPassMaterial->setMode(DEPTH_TEST, false);
		//_pointLightPassMaterial->setCullFace(FACE_FRONT);
		/*_pointLightPassMaterial->setMode(CULL_FACE, true);
		_pointLightPassMaterial->setCullFace(FACE_FRONT);
		_pointLightPassMaterial->setStencilFunction(NOTEQUAL);
		_pointLightPassMaterial->setStencilMask(0xFF);*/
		_pointLightPassMaterial->setBlendEquationSeparate(BLENDEQUATION_ADD, BLENDEQUATION_ADD);
		_pointLightPassMaterial->setBlendFunSeparate(BLENDFUN_ONE, BLENDFUN_ONE, BLENDFUN_ONE, BLENDFUN_ONE);
		bindGBufferTexture(gbuffer, _pointLightPassMaterial);
	}

	void LightPassRender::render(GBuffer* gbuffer, ICamera* camera, IRenderPool* renderPool)
	{
		if (!_programLightDir)
		{
			initScreenRect();
			initSphere();
			initProgram();
			initMaterial(gbuffer);
		}

		_renderTargetWidth = renderPool->getRenderTarget()->getWidth();
		_renderTargetHeight = renderPool->getRenderTarget()->getHeight();
		renderPool->applyViewport(0, 0, _renderTargetWidth, _renderTargetHeight);

		renderEmissive(renderPool);
		renderDiffuse(camera, renderPool);
	}

	void LightPassRender::renderDiffuse(ICamera* camera, IRenderPool* renderPool)
	{
		////渲染漫反射
		ILightManager* lightManager = renderPool->getLightManager();
		lightManager->begin();

		ILight* light = NULL;
		while (light = lightManager->nextIterator())
		{
			applyPreLight(light, camera, renderPool);
		}
	}

	void LightPassRender::renderEmissive(IRenderPool* renderPool)
	{
		renderPool->applyProgram(_programEmissive);

		_programEmissive->setUniform2f("_screenSize", Real2((Real)_renderTargetWidth, (Real)_renderTargetHeight));
		renderPool->applyMaterial(_emissiveRenderMaterial);
		_emissiveRenderMaterial->getTextureUnitSet()->apply(_programEmissive);

		_screenRect->draw(renderPool);
	}

	void LightPassRender::applyCommonLight(IProgram* program, ILight* light, IRenderPool* renderPool)
	{
		renderPool->applyProgram(program);

		program->setUniform3f("_light_ambientColor", light->getAmbientColor());
		program->setUniform3f("_light_diffuseColor", light->getDiffuseColor());
		program->setUniform2f("_screenSize", Real2((Real)_renderTargetWidth, (Real)_renderTargetHeight));
	}

	void LightPassRender::applyDirectionLight(ILight* light, IRenderPool* renderPool)
	{
		applyCommonLight(_programLightDir, light, renderPool);
		renderPool->applyMaterial(_dirLightPassMaterial);
		_dirLightPassMaterial->getTextureUnitSet()->apply(_programLightDir);

		const RMatrix& viewMatrix = renderPool->getViewMatrix();
		Real3 lp_3 = light->getLightDirection();
		lp_3 *= -1;

		RQuat nn = viewMatrix.getRotate();
		lp_3 = nn * lp_3;
		lp_3.normalize();

		_programLightDir->setUniform3f("_lightDir_eye", lp_3);

		_screenRect->draw(renderPool);
	}

	void LightPassRender::applyPointLight(ILight* light, ICamera* camera, IRenderPool* renderPool)
	{
		const Real3& ps = light->getLightPosition();
		Real f, n;
		light->getLightRange(n, f);

		RBoundingBox aabb;
		aabb.expandBy(ps, f);
		if (!camera->inFrustum(aabb))
			return;

		Matrixf t;
		Matrixf s;
		Matrixf transe;
		t.makeTranslate(ps);
		s.makeScale(f, f, f);
		transe = t * s;

		RMatrix mvp = renderPool->getProjectMatrix() * renderPool->getViewMatrix() * transe;

		////填充模板缓存区
		/*renderPool->clear(STENCIL_BUFFER_BIT);
		renderPool->applyProgram(_program_Null);
		renderPool->applyMaterial(_pointLightStencilPassMaterial);
		_program_Null->setUniform16f("_mvp", mvp);
		_sphere->draw(renderPool);*/

		///光照
		applyCommonLight(_programLightPoint, light, renderPool);
		renderPool->applyMaterial(_pointLightPassMaterial);
		_pointLightPassMaterial->getTextureUnitSet()->apply(_programLightPoint);
		RMatrix mvpInvers = mvp;
		mvpInvers.inverse();

		_programLightPoint->setUniform16f("_mvp", mvp);
		_programLightPoint->setUniform16f("_mv", renderPool->getViewMatrix() * transe);
		_programLightPoint->setUniform16f("_mvpInverse", mvpInvers);

		Real3 lightPoint_eye = renderPool->getViewMatrix() * ps;
		_programLightPoint->setUniform3f("_lightPoint_eye", lightPoint_eye);
		_programLightPoint->setUniformf("_lightRange", f * 2);

		_sphere->draw(renderPool);
	}

	void LightPassRender::applyPreLight(ILight* light, ICamera* camera, IRenderPool* renderPool)
	{
		LightType lt = light->getLightType();
		IProgram* program = NULL;

		switch (lt)
		{
		case LIGHT_POINT:
			applyPointLight(light, camera, renderPool);
			break;
		case LIGHT_DIRECT:
			applyDirectionLight(light, renderPool);
			break;
		}
	}
}