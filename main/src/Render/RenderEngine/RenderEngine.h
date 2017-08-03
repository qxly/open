#ifndef RENDER_RENDERENGINE_H
#define RENDER_RENDERENGINE_H 1

#include <Render/RenderEngine.h>
#include <Render/MaterialInterface.h>

#include <vector>
#include <map>

namespace open
{
	class ICamera;
	class IRenderPool;
	class IUniformParameter;
	class IVertexStream;
	struct InstanceLeaf;
	class RenderQueue;
	class IProgram;
	class IGeometry;
	class ILight;
	class ILightManager;
	struct PointLightInfo;

	class RenderEngine : public IRenderEngine
	{
		struct PassResource {
			IProgram* program = nullptr;
			IRenderTarget* renderTarget = nullptr;
			IMaterial* material = nullptr;
			IGeometry* geometry = nullptr;

			RenderScript renderScript;
		};
		struct SlaveCamera
		{
			ICamera* camera;
			RMatrix viewOffset;
			RMatrix projOffset;
		};

		struct TempRenderValue;
	public:

		RenderEngine();

		~RenderEngine();

		virtual ICamera* getCamera() { return _camera; }

		virtual IRenderPool* getRenderPool() { return _renderPool; }

		virtual void addRender(IDrawable* drawable, const RMatrix& matrix);

		virtual void render();

	private:

		void init();

		void prepear();

		void preRender();

		void rendering();

		void updateGPass();

		void updateEmissivePass();

		bool cullLight(ILight* light);

		std::vector<ILight*> cullLights(ILightManager* lm);

		void updateDiffusePass();

		void updateFowardPass();

		void update(IRenderTarget* renderTarget, const RMatrix& view, const RMatrix& pro, uint32 viewport[4]);

		void render(const void* leafs, const RenderEngine::PassResource* pass);

		void applyMaterial(IMaterial* material, const PassResource* pass = nullptr,void* data = nullptr);

		void applyMaterial(IMaterial* material, IProgram* program , const UniformDetial* uniforms,void* data = nullptr);

		void applyGeometries(const RenderScript& script, IGeometry* geometry, const void* matrix, size_t size);

	private:

		IRenderPool* _renderPool;
		ICamera* _camera;
		RenderQueue* _renderQueue;

		IVertexStream* _instanceStream;
		PointLightInfo* _pointLightInfoTemp;

		std::vector<PassResource> _renderScripts;

		PassResource _gpass;
		PassResource _emissivePass;
		PassResource _dirLightPass;
		PassResource _pointLightPass;
		PassResource _fowardPass;

		TempRenderValue* _temp;
	};
}

#endif