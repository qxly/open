#include "Camera.h"

namespace open
{
	Camera::Camera()
		:_eye(2.0731573f, -23.696234f, 4.4531121f), _center(0, 0, 0), _up(0, 0, 1),
		_viewChange(true), _projectChange(true), _updateFrustumPlanes(true),
		_frustumOrtho(true)
	{
		computeDir();
	}

	void Camera::computeDir()
	{
		_dirNormal = _center - _eye;
		_dirNormal.normalize();
	}

	void Camera::setLookAt(const Real3& eye, const Real3& center, const Real3& up)
	{
		_eye = eye;
		_center = center;
		_up = up;
		computeDir();
		_viewChange = true;
		_updateFrustumPlanes = true;
	}

	void Camera::setFrustum(Real left, Real right, Real bottom, Real top, Real zn, Real zf)
	{
		_left = left;
		_right = right;
		_top = top;
		_bottom = bottom;
		_zn = zn;
		_zf = zf;

		_projectChange = true;
		_frustumOrtho = true;
		_updateFrustumPlanes = true;
	}

	void Camera::setPerspective(Real fovy, Real aspectRatio, Real zNear, Real zFar)
	{
		RMatrix project;
		project.makePerspective(fovy, aspectRatio, zNear, zFar);

		Real left;
		Real right;
		Real bottom;
		Real top;
		project.getFrustum(left, right, bottom, top, zNear, zFar);

		setFrustum(left, right, bottom, top, zNear, zFar);
	}

	void Camera::setOrtho2D(Real w, Real h, Real zn, Real zf)
	{
		_right = w * 0.5f;
		_left = -_right;

		_top = h * 0.5f;;
		_bottom = -_top;

		_zn = zn;
		_zf = zf;

		_projectChange = true;
		_frustumOrtho = false;
		_updateFrustumPlanes = true;
	}

	void Camera::getLookAt(Real3& eye, Real3& center, Real3& up)
	{
		eye = _eye;
		center = _center;
		up = _up;
	}

	void Camera::getNearFar(Real& zn, Real& zf)
	{
		zn = _zn;
		zf = _zf;
	}

	const RMatrix& Camera::getViewMatrix()
	{
		if (_viewChange)
		{
			_viewChange = false;
			computeViewMatrix();
		}
		return _viewMatrix;
	}

	const RMatrix& Camera::getProject()
	{
		if (_projectChange)
		{
			_projectChange = false;
			computePorjectMatrix();
		}
		return _pro;
	}

	void Camera::computeViewMatrix()
	{
		_viewMatrix.makeLookAt(_eye, _center, _up);
	}

	void Camera::computePorjectMatrix()
	{
		if (_frustumOrtho)
			_pro.makeFrustum(_left, _right, _bottom, _top, _zn, _zf);
		else
			_pro.makeOrtho(_left, _right, _bottom, _top, _zn, _zf);
	}

	void Camera::updateFrustumPlanes()
	{
		if (!_updateFrustumPlanes)
			return;

		_updateFrustumPlanes = false;
		RMatrix combo = _pro * _viewMatrix;

		_frustumPlanes[PLANE_LEFT].set(combo[3][0] + combo[0][0], combo[3][1] + combo[0][1], combo[3][2] + combo[0][2], combo[3][3] + combo[0][3]);
		_frustumPlanes[PLANE_RIGHT].set(combo[3][0] - combo[0][0], combo[3][1] - combo[0][1], combo[3][2] - combo[0][2], combo[3][3] - combo[0][3]);

		_frustumPlanes[PLANE_TOP].set(combo[3][0] - combo[1][0], combo[3][1] - combo[1][1], combo[3][2] - combo[1][2], combo[3][3] - combo[1][3]);
		_frustumPlanes[PLANE_BOTTOM].set(combo[3][0] + combo[1][0], combo[3][1] + combo[1][1], combo[3][2] + combo[1][2], combo[3][3] + combo[1][3]);

		_frustumPlanes[PLANE_NEAR].set(combo[3][0] + combo[2][0], combo[3][1] + combo[2][1], combo[3][2] + combo[2][2], combo[3][3] + combo[2][3]);
		_frustumPlanes[PLANE_FAR].set(combo[3][0] - combo[2][0], combo[3][1] - combo[2][1], combo[3][2] - combo[2][2], combo[3][3] - combo[2][3]);

		for (int32 i = 0; i < 6; i++)
		{
			_frustumPlanes[i].makeUnitLength();
		}
	}

	int32 Camera::getSide(const RPlane& plane, const Real3& center, const Real3& halfSize)
	{
		Real dist = plane.distance(center);
		const Real3& normal = plane.getNormal();

		Real maxAbsDist = static_cast<Real>(fabs(normal.x() * halfSize.x()) + fabs(normal.y() * halfSize.y()) + fabs(normal.z() * halfSize.z()));

		if (dist < -maxAbsDist)
			return 2;

		if (dist > maxAbsDist)
			return 1;

		return 3;
	}

	const RPlane* Camera::getPlanes() {
		return _frustumPlanes;
	}

	bool Camera::inFrustum(const RBoundingBox& bound)
	{
		if (!bound.valid())
			return false;

		updateFrustumPlanes();

		Real3 centre = bound.center();
		Real3 halfSize = (bound._max - bound._min) * 0.5f;

		for (int plane = 0; plane < 6; ++plane)
		{
			int side = getSide(_frustumPlanes[plane], centre, halfSize);
			if (side == 2)
			{
				return false;
			}
		}
		return true;
	}

	void Camera::getViewport(uint32& x, uint32& y, uint32& width, uint32& height) {
		x = _viewport[0];
		y = _viewport[1];
		width = _viewport[2];
		height = _viewport[3];
	}

	void Camera::setViewport(uint32 x, uint32 y, uint32 width, uint32 height) {
		_viewport[0] = x;
		_viewport[1] = y;
		_viewport[2] = width <= 0 ? 1 : width;
		_viewport[3] = height <=0 ? 1 : height;
	}
}