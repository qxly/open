#include "RenderEngine.h"
#include <Render/MaterialInterface.h>
#include <Render/DrawableInterface.h>
#include <Render/RenderFactoryInterface.h>

#include <RenderSystem/RenderSystemFactoryInterface.h>
#include <RenderSystem/RenderInterface.h>
#include <RenderSystem/RenderEnumeration.h>
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/BufferInterface.h>

#include <vector>
#include <map>
#include <algorithm>

#include "RenderQueue.h"
#include "DefalutSchema.h"
#include "DeferredUtil.h"

namespace open
{
	struct InstanceLeaf
	{
		inline const PreInstancData* ptr() const
		{
			return &(matrixs.front());
		}

		inline void add(const RMatrix& matrix, void* userData)
		{
			matrixs.push_back({ matrix,userData });
		}

		inline size_t size() const
		{
			return matrixs.size();
		}

		inline void setGeometry(IGeometry* geo) { geometry = geo; }

		IGeometry* getGeometry() const { return geometry; }

		inline void reset() {
			matrixs.clear();
			geometry = nullptr;
		}

	private:
		IGeometry* geometry = nullptr;
		std::vector<PreInstancData> matrixs;
	};

	struct RenderEngine::TempRenderValue {
		RMatrix vp;
		uint32 viewport[4];

		ICamera* tempCamera = nullptr;
		RMatrix currentDrawableMatrix;
		void* _userData = nullptr;

		IMaterial* lastMaterial = nullptr;
	};

	inline IProgram* genProgram(const RenderScript& script) {

		IProgram* program = RenderSystemFactory::s_factory->createProgram();
		program->loadFromFile(script.shader->vert, script.shader->frag, script.shader->define);

		return program;
	}

	inline RMatrix computeMVP(const RMatrix& vp, const RMatrix& model)
	{
		return (vp * model).getTranspose();
	}

	inline RMatrix computeModelNormal(const RMatrix& viewMatrix, const RMatrix& model)
	{
		RMatrix modelNormal;
		modelNormal = viewMatrix * model;
		RQuat nn = modelNormal.getRotate();

		modelNormal.makeIdentity();
		modelNormal.makeRotate(nn);

		return modelNormal.getTranspose();
	}

	RenderEngine::RenderEngine()
	{
		_renderPool = RenderSystemFactory::s_factory->createRenderPool();
		_camera = RenderFactory::s_factory->createCamera();
		_renderQueue = new RenderQueue;
		_instanceStream = RenderSystemFactory::s_factory->createVertexStream();
		_temp = new TempRenderValue;
		_pointLightInfoTemp = new PointLightInfo;
	}

	RenderEngine::~RenderEngine()
	{
	}

	void RenderEngine::init() {
		_gpass.renderScript = genGBufferPass();
		_gpass.renderTarget = genGBufferTarget();
		_gpass.program = genProgram(_gpass.renderScript);

		_emissivePass.renderScript = genEmissivePass();
		_emissivePass.program = genProgram(_emissivePass.renderScript);
		_emissivePass.material = genEmissiveMaterial();
		_emissivePass.geometry = DeferredUtil::genScreenRect();

		_dirLightPass.renderScript = genDirLightPass();
		_dirLightPass.program = genProgram(_dirLightPass.renderScript);
		_dirLightPass.material = genDirLightMaterial();
		_dirLightPass.geometry = _emissivePass.geometry;

		_pointLightPass.renderScript = genPointLightPass();
		_pointLightPass.program = genProgram(_pointLightPass.renderScript);
		_pointLightPass.material = genPointLightMaterial();
		_pointLightPass.geometry = DeferredUtil::genSphere();

		_fowardPass.renderScript = genForwardPass();
		_fowardPass.program = genProgram(_fowardPass.renderScript);
	}

	void RenderEngine::addRender(IDrawable * drawable, const RMatrix & matrix)
	{
		_temp->currentDrawableMatrix = matrix;
		drawable->accept([=](IGeometry* geo, IMaterial* material, void* use) {
			_renderQueue->add(geo, material, _temp->currentDrawableMatrix, use);
		});
	}

	void RenderEngine::render()
	{
		static bool g_init_ = false;

		if (!g_init_) {
			init();
			g_init_ = true;
		}

		prepear();
		preRender();
		rendering();
	}

	void RenderEngine::prepear() {
		_renderQueue->sort();
		_camera->getViewport(_temp->viewport[0], _temp->viewport[1], _temp->viewport[2], _temp->viewport[3]);
	}

	void RenderEngine::preRender() {
	}

	void RenderEngine::rendering() {
		updateGPass();

		update(nullptr, _camera->getViewMatrix(), _camera->getProject(), _temp->viewport);

		updateEmissivePass();
		updateDiffusePass();
		updateFowardPass();
	}

	void RenderEngine::updateGPass()
	{
		updateGBuffer(_gpass.renderTarget, _renderPool);
		update(_gpass.renderTarget, _camera->getViewMatrix(), _camera->getProject(), _temp->viewport);
		_renderPool->clear(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT | COLOR_BUFFER_BIT);

		render(&(_renderQueue->getOpacityRenderLeaf()), &_gpass);
	}

	void RenderEngine::updateEmissivePass()
	{
		_renderPool->clear(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT | COLOR_BUFFER_BIT);

		applyMaterial(_emissivePass.material, &_emissivePass);
		_emissivePass.geometry->draw(_renderPool);
	}

	bool RenderEngine::cullLight(ILight * light)
	{
		if (light->getLightType() == LIGHT_POINT) {
			const Real3& ps = light->getLightPosition();
			Real f, n;
			light->getLightRange(n, f);

			RBoundingBox aabb;
			aabb.expandBy(ps, f);
			if (!_camera->inFrustum(aabb))
				return false;
		}
		return true;
	}

	std::vector<ILight*> RenderEngine::cullLights(ILightManager* lm) {
		std::vector<ILight*> lights;
		ILight* light = nullptr;
		lm->begin();
		while (light = lm->nextIterator())
		{
			if (cullLight(light)) {
				lights.push_back(light);
			}
		}
		return lights;
	}

	void RenderEngine::updateDiffusePass()
	{
		update(nullptr, _camera->getViewMatrix(), _camera->getProject(), _temp->viewport);
		ILightManager* lightManager = _renderPool->getLightManager();
		lightManager->begin();

		ILight* light = nullptr;
		while (light = lightManager->nextIterator())
		{
			LightType lt = light->getLightType();
			IProgram* program = nullptr;

			if (cullLight(light)) {
				switch (lt)
				{
				case LIGHT_POINT:
					genPointLightInfo(*_pointLightInfoTemp, light, _renderPool);
					applyMaterial(_pointLightPass.material, &_pointLightPass, _pointLightInfoTemp);
					_pointLightPass.geometry->draw(_renderPool);
					break;
				case LIGHT_DIRECT:
					applyMaterial(_dirLightPass.material, &_dirLightPass, light);
					_dirLightPass.geometry->draw(_renderPool);
					break;
				}
			}
		}
	}

	void RenderEngine::updateFowardPass()
	{
		render(&(_renderQueue->getTransparencyRenderLeaf()), &_fowardPass);
	}

	void RenderEngine::update(IRenderTarget* renderTarget, const RMatrix& view, const RMatrix& pro, uint32 viewport[4]) {

		if (!renderTarget)
			renderTarget = _renderPool->getDefaultRenderTarget();

		_renderPool->applyRenderTarget(renderTarget);
		_renderPool->applyViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		_renderPool->setViewMatrix(view);
		_renderPool->setProjectMatrix(pro);

		_temp->vp = pro * view;
	}

	void RenderEngine::applyMaterial(IMaterial* material, const RenderEngine::PassResource* pass, void* data) {

		const UniformDetial* uniforms = pass->renderScript.uniformDetials;
		IProgram* program = pass->program;

		void* d = pass->renderScript.begin ? pass->renderScript.begin() : data;

		applyMaterial(material, program, uniforms, d);

		if (pass->renderScript.end && pass->renderScript.begin) {
			pass->renderScript.end(d);
		}
	}

	void RenderEngine::applyMaterial(IMaterial * material, IProgram * program, const UniformDetial * uniforms, void* data)
	{
		_renderPool->applyProgram(program);
		_renderPool->applyRenderStateSet(material->getRenderStaeSet());
		_renderPool->applyTextureUnits(material->getTextureUnitSet());

		int i = 0;
		Value value;
		while (!uniforms[i].isNull())
		{
			uniforms[i].uniformHandle(data, material, _renderPool, value);
			const char* name = uniforms[i].name;

			switch (uniforms[i].type)
			{
			case 1:
				program->setUniformi(name, value.toInt32());
				break;
			case 2:
				program->setUniformf(name, value.toReal());
				break;
			case 3:
				program->setUniform2f(name, value.toReal2());
				break;
			case 4:
				program->setUniform3f(name, value.toReal3());
				break;
			case 5:
				program->setUniform4f(name, value.toReal4());
				break;
			case 6:
				program->setUniform16f(name, value.toReal16());
				break;
			default:
				break;
			}
			i++;
		}
	}

	void RenderEngine::render(const void* gs, const RenderEngine::PassResource* pass) {

		const RenderQueue::Leafs& leafs = *(const RenderQueue::Leafs*)(gs);

		IMaterial* material = nullptr;
		IGeometry* geometry = nullptr;
		const RenderScript* rs = nullptr;
		InstanceLeaf leaf;

		for (auto t : leafs) {

			if (geometry != t->geometry) {

				if (geometry)
					applyGeometries(*rs, geometry, leaf.ptr(), leaf.size());

				geometry = t->geometry;
				leaf.reset();
				leaf.setGeometry(geometry);
			}
			leaf.add(t->matrix, t->userData);

			if (material != t->materail) {
				material = t->materail;
				if (!material)
					continue;

				if (!material->getRenderTechnique()) {
					applyMaterial(material, pass);
					rs = &(pass->renderScript);
				}
				else {
					rs = &(material->getRenderTechnique()->getRenderScript());
					applyMaterial(material, material->getRenderTechnique()->getProgram(), rs->uniformDetials);
				}
			}
		}
	}

	void RenderEngine::applyGeometries(const RenderScript& script, IGeometry* geometry, const void* data, size_t size)
	{
		const PreInstancData* matrix = (const PreInstancData*)data;
		if (!script.genInstance) {
			std::vector<Matrixf> mvp_normal;
			mvp_normal.reserve(size * 2);

			for (size_t i = 0; i < size; i++)
			{
				mvp_normal.emplace_back(computeMVP(_temp->vp, matrix[i].matrix));
				mvp_normal.emplace_back(computeModelNormal(_renderPool->getViewMatrix(), matrix[i].matrix));
			}

			uint64 vdt[] = { IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE0),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE1),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE2),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE3),

				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE4),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE5),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE6),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE7),
			};

			_instanceStream->reset();
			_instanceStream->add(vdt, 8);
			_instanceStream->addVerxtxData(&mvp_normal.front(), (uint32)mvp_normal.size() * sizeof(Matrixf));
			_instanceStream->setSize((uint32)size);

			geometry->draw(_renderPool, _instanceStream);
		}
		else {
			_instanceStream->reset();

			script.genInstance(_renderPool, _instanceStream, matrix, (uint32)size);
			geometry->draw(_renderPool, _instanceStream);
		}
	}
}