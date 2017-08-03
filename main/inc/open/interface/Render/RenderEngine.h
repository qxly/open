#ifndef RENDER_RENDERSCHEMA_INTERFACE_H
#define RENDER_RENDERSCHEMA_INTERFACE_H

#include <open/Maths.h>

namespace open
{
	class IMaterial;
	class IDrawable;
	class IRenderPool;

	class ICamera
	{
	public:
		virtual ~ICamera() {}

		virtual void setLookAt(const Real3& eye, const Real3& center, const Real3& up) = 0;

		virtual void setFrustum(Real left, Real right, Real bottom, Real top, Real zn, Real zf) = 0;

		virtual void setPerspective(Real fovy, Real aspectRatio, Real zNear, Real zFar) = 0;

		virtual void setOrtho2D(Real w, Real h, Real zn, Real zf) = 0;

		virtual const RMatrix& getViewMatrix() = 0;

		virtual const RMatrix& getProject() = 0;

		virtual void getLookAt(Real3& eye, Real3& center, Real3& up) = 0;

		virtual void getNearFar(Real& zn, Real& zf) = 0;

		virtual bool inFrustum(const RBoundingBox& bound) = 0;

		virtual void setRenderMask(uint32 mask) = 0;

		virtual uint32 getRenderMask() = 0;

		virtual void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;

		virtual void getViewport(uint32& x, uint32& y, uint32& width, uint32& height) = 0;
	};

	class IRenderEngine
	{
	public:
		virtual ICamera* getCamera() = 0;

		virtual IRenderPool* getRenderPool() = 0;

		virtual void addRender(IDrawable* drawable, const RMatrix& matrix) = 0;

		virtual void render() = 0;

	};
}

#endif