#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <open/Memory.h>
#include <Render/RenderEngine.h>

#include <vector>

namespace open
{
	class IRenderTarget;
	class Camera : public ICamera PUBLIC_OBJECT_DEBUG
	{
	public:

		Camera();

		virtual void setLookAt(const Real3& eye, const Real3& center, const Real3& up);

		virtual void setFrustum(Real left, Real right, Real bottom, Real top, Real zn, Real zf);

		virtual void setPerspective(Real fovy, Real aspectRatio, Real zNear, Real zFar);

		virtual void setOrtho2D(Real w, Real h, Real zn, Real zf);

		virtual const RMatrix& getViewMatrix();

		virtual const RMatrix& getProject();

		virtual void getLookAt(Real3& eye, Real3& center, Real3& up);

		virtual void getNearFar(Real& zn, Real& zf);

		virtual const RPlane* getPlanes();

		virtual bool inFrustum(const RBoundingBox& bound);

		virtual void setRenderMask(uint32 mask) { _mask = mask; }

		virtual uint32 getRenderMask() { return _mask; }

		virtual void getViewport(uint32& x, uint32& y, uint32& width, uint32& height);

		virtual void setViewport(uint32 x, uint32 y, uint32 width, uint32 height);

		virtual void setRenderTarget(IRenderTarget* renderTarget) { _renderTarget = renderTarget; }

		virtual IRenderTarget* getRenderTarget() { return _renderTarget; }

	private:
		enum FrustumPlane
		{
			PLANE_NEAR = 0,
			PLANE_FAR = 1,
			PLANE_LEFT = 2,
			PLANE_RIGHT = 3,
			PLANE_TOP = 4,
			PLANE_BOTTOM = 5
		};
	private:

		void computeDir();

		void computeViewMatrix();

		void computePorjectMatrix();

		void updateFrustumPlanes();

		int32 getSide(const RPlane& plane, const Real3& center, const Real3& halfSize);

	private:

		uint32 _mask = 0xffffffff;

		Real3 _eye;
		Real3 _center;
		Real3 _up;
		Real3 _dirNormal;
		RMatrix _viewMatrix;

		RMatrix _pro;
		Real _left;
		Real _right;
		Real _top;
		Real _bottom;
		Real _zn;
		Real _zf;

		uint32 _viewport[4];
		IRenderTarget* _renderTarget;

		bool _viewChange;
		bool _projectChange;
		bool _updateFrustumPlanes;
		RPlane _frustumPlanes[6];

		bool _frustumOrtho;
	};
}


#endif