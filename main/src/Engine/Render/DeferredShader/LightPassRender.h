#ifndef RENDERSYSTEM_LIGHT_PASS_H
#define RENDERSYSTEM_LIGHT_PASS_H 1

#include <common/Maths.h>
#include <common/Ptr.h>

namespace open
{
	struct GBuffer;
	class IProgram;
	class IMesh;
	class IMaterial;
	class ITextureUnitSet;
	class ILight;
	class IRenderPool;
	class ICamera;
	enum LightType;

	class LightPassRender
	{
	public:

		LightPassRender();

		~LightPassRender();

		void render(GBuffer* gbuffer, ICamera* camera, IRenderPool* renderPool);

	private:

		void initProgram();

		void initScreenRect();

		void initSphere();

		void initMaterial(GBuffer* gbuffer);

		void bindGBufferTexture(GBuffer* gbuffer, IMaterial* material);

		void applyPreLight(ILight* light, ICamera* camera, IRenderPool* renderPool);

		void applyDirectionLight(ILight* light, IRenderPool* renderPool);

		void applyPointLight(ILight* light, ICamera* camera, IRenderPool* renderPool);

		void applyCommonLight(IProgram* program, ILight* light, IRenderPool* renderPool);

		void renderDiffuse(ICamera* camera, IRenderPool* renderPool);

		void renderEmissive(IRenderPool* renderPool);

	private:

		IProgram* _programLightDir;
		IProgram* _programNull;
		IProgram* _programLightPoint;
		IProgram* _programEmissive;

		Ptr<IMesh> _screenRect;
		Ptr<IMesh> _sphere;

		Ptr<IMaterial> _dirLightPassMaterial;

		Ptr<IMaterial> _pointLightStencilPassMaterial;
		Ptr<IMaterial> _pointLightPassMaterial;

		Ptr<IMaterial> _emissiveRenderMaterial;

		int32 _renderTargetWidth;
		int32 _renderTargetHeight;
	};
}

#endif