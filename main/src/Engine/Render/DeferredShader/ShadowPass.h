#ifndef RENDERSYSTEM_SHADOWMAPPING_H
#define RENDERSYSTEM_SHADOWMAPPING_H

#include <common/Maths.h>
#include <common/Ptr.h>
#include <common/Array.h>
#include <RenderSystem/RenderInterface.h>
#include <common/interface/Scene/MeshInterface.h>

namespace wyin
{
	class IProgram;
	class IGeometry;
	class IMaterial;
	class ILight;
	class INode;
	class IRenderPool;
	class ITexture;
	class IFrameBuffer;
	struct DrawableRenderTrace;
	class ShadowRenderPass;

	struct ShadowData : public IShadowData
	{
		RMatrix lightVP;
		RMatrix lightViewMatrix;
		RMatrix lightProjectMatrix;

		IFrameBuffer* frameBuffer;
		ITexture* shadowTexture;

		~ShadowData()
		{
			delete frameBuffer;
		}
	};

	struct ShadowGeometry
	{
		ShadowGeometry()
		:geometry(NULL), instanceInfos(NULL){}

		ShadowGeometry(IGeometry* geometry_)
			:geometry(geometry_){}

		inline void destory()
		{
			delete instanceInfos;
		}

		inline bool operator < (const ShadowGeometry& other) const
		{
			return geometry < other.geometry;
		}

		IGeometry* geometry;
		DynamicArray<DrawInstance>* instanceInfos;
	};
	DECLARE_AND_TYPEDEF_SAMLLTYPE_ARRARY(ShadowGeometry);

	class ShadowPass
	{
		struct ShadowPassRenderScheme;
	public:

		ShadowPass();

		~ShadowPass();

		void beginShadowPass(IRenderPool* renderPool,GBuffer* gbuffer);

		void applyLight(ILight* light);

		void endShadowPass(IRenderPool* renderPool);

		void applyNode(INode* drt);
	private:

		void initProgram();

		void initShadowPassMaterial();

		void initShadowFrameBuffer();

		void initShadowData();

		void updateShadowData(ILight* light);

		void computeLightCamera(ILight* light, RMatrix& viewMatrix, RMatrix& projectMatrix);

		Real3 computeOrthogonalVector(const Real3& direction);

		void genShadowMap();

		void renderShadow();

		void insertGeometry(IGeometry* geometry, IMaterial*);

	private:

		IProgram* _program;
		Ptr<IMaterial> _shadowPassMaterial;

		ShadowPassRenderScheme* _renderScheme;
		IRenderPool* _renderPool;

		ShadowData* _shadowData;
		GBuffer* _gbuffer;

		ILight* _currentLight;
		RBoundingBox _boundingBox;
		RBoundingBox _tempBox;

		RMatrix _currentMatrix;
		ShadowGeometryArray _shadowGeometries;

		ShadowRenderPass* _shadowRenderPass;
	};
}

#endif