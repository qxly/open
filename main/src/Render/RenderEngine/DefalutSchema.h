#include <Render/RenderEngine.h>

namespace open
{
	struct PointLightInfo
	{
		ILight* light = nullptr;
		Real f;
		Real3 lightPosition;
		RBoundingBox lightAABB;
		Matrixf lightTranse;
		RMatrix lightMVP;
	};

	class IRenderTarget;

	const RenderScript& genGBufferPass();

	IRenderTarget* genGBufferTarget();

	void updateGBuffer(IRenderTarget* renderTarget, IRenderPool* renderPool);

	const RenderScript& genEmissivePass();

	IMaterial* genEmissiveMaterial();

	const RenderScript& genDirLightPass();

	IMaterial* genDirLightMaterial();

	const RenderScript& genPointLightPass();

	void genPointLightInfo(PointLightInfo& info, ILight* light, IRenderPool* renderPool);

	IMaterial* genPointLightMaterial();

	const RenderScript& genForwardPass();
}