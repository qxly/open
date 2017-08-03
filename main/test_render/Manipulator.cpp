#include "Manipulator.h"
#include <Render/RenderEngine.h>
#include <Render/RenderFactoryInterface.h>
#include <Render/DrawableInterface.h>
#include <RenderSystem/RenderInterface.h>
#include <RenderSystem/RenderEnumeration.h>

namespace open
{
	Manipulator::Manipulator()
		:_init(false), _camera(nullptr), _terrain(nullptr), _lastX(-1), _lastY(-1), _step(3), _nearTerrain(false), _lock(false)
	{
		this->guiListener = [=](const Input* e) {
			if (!_init)
			{
				_init = true;
				init();
			}
			const Input& event = *e;

			InputType et = event.eventType;
			switch (et)
			{
			case PUSH:
			case RELEASE:
			case DOUBLECLICK:
			case DRAG:
				mouseEvent((MouseButtonMask)event.buttonMask, et, event.mx, event.my, false);
				break;

			case KEYDOWN:
			case KEYUP:
			{
				bool down = (et == KEYDOWN) ? true : false;
				keyEvent(event.key, down);
			}
			break;
			case RESIZE:
				_camera->setViewport(0, 0, event.windowWidth, event.windowHeight);
				_camera->setPerspective(DEG2RAD(45), event.windowWidth / (Real)event.windowHeight, 0.1f, _far);
				break;
			}
		};
	}

	void Manipulator::init()
	{
		const RBoundingBox& aabb = _terrain->getBoudingBox();

		Real3 eye = aabb.center();

		Real radius = aabb.radius();
		_far = radius * 2;

		_camera->setLookAt(eye, eye + Real3(0, 1, 0), Real3(0, 0, 1));
		_camera->setPerspective(DEG2RAD(45), 800 / (Real)600, 0.1f, _far);
	}

	Real3 Manipulator::getElevationPoint(const Real3& point)
	{
		Real ele = getElevation(point);
		if (isNaN(ele))
			return Real3(FLOAT_NAN, FLOAT_NAN, FLOAT_NAN);

		return Real3(point.x(), point.y(), ele);
	}

	void Manipulator::forward(Real step)
	{
		Real3 eye, center, up;

		_camera->getLookAt(eye, center, up);
		Real3 dir = center - eye;
		dir.normalize();

		move(eye, center, up, dir * step);
	}

	void Manipulator::nearTerrainForward(Real step)
	{
		Real3 eye, center, up;
		_camera->getLookAt(eye, center, up);

		Real eyeEle = getElevation(eye);
		if (isNaN(eyeEle))
			return;

		eyeEle += _forceEle;
		eye.z() = eyeEle;
		center.z() = eyeEle;

		Real3 dir = Real3(center.x() - eye.x(), center.y() - eye.y(), 0);
		dir.normalize();

		move(eye, center, up, dir * step);
	}

	void Manipulator::left(Real step)
	{
		Real3 eye, center, up;
		_camera->getLookAt(eye, center, up);

		Real3 dir = center - eye;
		dir.normalize();
		Real3 right = dir ^ up;
		right.normalize();

		move(eye, center, up, right * step);
	}

	void Manipulator::nearTerrainLeft(Real step)
	{
		Real3 eye, center, up;
		_camera->getLookAt(eye, center, up);

		Real eyeEle = getElevation(eye);
		if (isNaN(eyeEle))
			return;

		eyeEle += _forceEle;
		eye.z() = eyeEle;
		center.z() = eyeEle;

		Real3 dir = Real3(center.x() - eye.x(), center.y() - eye.y(), 0);
		dir.normalize();

		Real3 right = dir ^ up;

		move(eye, center, up, right * step);
	}

	void Manipulator::move(const Real3& eye, const Real3& center,
		const Real3& up, const Real3& step)
	{
		Real3 nextEeye = eye + step;
		Real nextEyeEle = getElevation(nextEeye);
		if (isNaN(nextEyeEle))
			return;

		if (nextEeye.z() < nextEyeEle)
		{
			nextEeye.z() = nextEyeEle + 1;
		}
		_camera->setLookAt(nextEeye, center + step, up);
	}

	void Manipulator::head(Real angle)
	{
		Real3 eye, center, up;
		_camera->getLookAt(eye, center, up);
		Real3 dir = center - eye;
		dir.normalize();

		dir = RQuat(angle, up) * dir;
		Real lg = (center - eye).length();
		center = eye + dir * lg;

		_camera->setLookAt(eye, center, up);
	}

	void Manipulator::pitch(Real angle)
	{
		Real3 eye, center, up;
		_camera->getLookAt(eye, center, up);
		Real3 dir = center - eye;
		dir.normalize();

		Real3 right = dir ^ up;
		dir = RQuat(angle, right) * dir;

		Real lg = (eye - center).length();
		center = eye + dir * lg;

		_camera->setLookAt(eye, center, up);
	}


	void Manipulator::keyEvent(unsigned int key, bool down)
	{
		if (down)
		{
			if (key == 'W')
			{
				if (!_nearTerrain)
					forward(_step);
				else
					nearTerrainForward(_step);
			}
			else if (key == 'S')
			{
				if (!_nearTerrain)
					forward(-_step);
				else
					nearTerrainForward(-_step);
			}
			else if (key == 'A')
			{
				if (!_nearTerrain)
					left(-_step);
				else
					nearTerrainLeft(-_step);
			}
			else if (key == 'D')
			{
				if (!_nearTerrain)
					left(_step);
				else
					nearTerrainLeft(_step);
			}
			else if (key == 'F')
			{
				_nearTerrain = !_nearTerrain ? true : false;
				if (_nearTerrain)
				{
					Real3 eye, center, up;
					_camera->getLookAt(eye, center, up);

					Real e = getElevation(eye);
					if (isNaN(e))
						_forceEle = 10;
					else
						_forceEle = eye.z() - e;
				}
			}
			else if (key == 'G')
			{
				_nearTerrain = !_nearTerrain ? true : false;
				if (_nearTerrain)
				{
					_forceEle = 1;
				}
			}
			else if (key == 'O')
			{
				_step *= 2;
			}
			else if (key == 'P')
			{
				_step /= 2.0f;
			}
			else if (key == 'L')
			{
				_lock = !_lock;
			}
			else if (key == 'N') {
				Real3 eye(500, 461, 100);
				Real3 center(500, 462, 100);

				_camera->setLookAt(eye, center, Real3(0, 0, 1));
			}
			else if (key == 'M')
			{
				Real near_, far_;

				ILight* light = RenderFactory::s_factory->getRenderEngine()->getRenderPool()->getLightManager()->getMainLight();
				light->getLightRange(near_, far_);

				const Real3& light_p = light->getLightPosition();
				const Real3& light_d = light->getLightDirection();
				LightType lt = light->getLightType();

				if (lt == LIGHT_DIRECT)//·½Ïò¹â
				{
					Real3 center = light_p + light_d * far_;
					_camera->setLookAt(light_p, center, computeOrthogonalVector(-light_d));
					//_camera->setFrustum(-far_, far_, -far_, far_, near_, far_);
				}
			}
		}
	}

	Real3 Manipulator::computeOrthogonalVector(const Real3& direction)
	{
		Real length = direction.length();
		Real3 orthogonalVector = direction ^ Real3(0.0f, 1.0f, 0.0f);
		if (orthogonalVector.normalize() < length * 0.5f)
		{
			orthogonalVector = direction ^ Real3(0.0f, 0.0f, 1.0f);
			orthogonalVector.normalize();
		}
		return orthogonalVector;
	}

	void Manipulator::mouseEvent(MouseButtonMask button, InputType state, int x, int y, int scroll)
	{
		if (state == DRAG)
		{
			if (this->_lock)
				return;

			if (button == LEFT_MOUSE_BUTTON)
			{
				int subX = 0;
				if (_lastX == -1)
					subX = 1;
				else
				{
					subX = x - _lastX;
				}
				head(DEG2RAD(subX*0.2));
				_lastX = x;
			}
			else if (button == RIGHT_MOUSE_BUTTON)
			{
				int subY = 0;
				if (_lastY == -1)
					subY = 1;
				else
				{
					subY = y - _lastY;
				}
				pitch(DEG2RAD(subY*0.2));
				_lastY = y;
			}
		}
		else if (state == RELEASE)
		{
			_lastX = -1;
			_lastY = -1;
		}
	}

}