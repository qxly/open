#ifndef RENDERSYSTEM_SHADOWMAPPING_H
#define RENDERSYSTEM_SHADOWMAPPING_H

#include <common/Maths.h>
#include <common/Ptr.h>
#include <common/Array.h>
#include <RenderSystem/RenderInterface.h>
#include <common/interface/Scene/MeshInterface.h>

#include "../../InstanceInfo.h"

namespace open
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
		DynamicArray<GeneralInstance>* instanceInfos;
	};
	DECLARE_AND_TYPEDEF_SAMLLTYPE_ARRARY(ShadowGeometry);

	class ShadowPass
	{
		struct ShadowPassRenderScheme;
	public:

		ShadowPass();

		~ShadowPass();

		void applyLight(IRenderPool* renderPool, ILight* light);

		void applyNode(INode* drt);

		void genShadowMap();
	private:

		void initProgram();

		void initShadowPassMaterial();

		void initShadowFrameBuffer();

		void initShadowData();

		void updateShadowData(ILight* light);

		void computeLightCamera(ILight* light, RMatrix& viewMatrix, RMatrix& projectMatrix);

		Real3 computeOrthogonalVector(const Real3& direction);

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
	};
}

#endif