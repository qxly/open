#ifndef TEST_MANIPULATOR_H
#define TEST_MANIPULATOR_H 1

#include <open/Maths.h>
#include <open/InputInterface.h>
#include <Engine/EngineInterface.h>

namespace open
{
	class IDrawable;
	class ICamera;

	class Manipulator : public InputListener
	{
	public:
		Manipulator();

		Manipulator(const Manipulator&) = delete;

		void setCamera(ICamera* camera) { _camera = camera; }

		void setTerrain(IDrawable* terrain)
		{
			_terrain = terrain;
		}

	private:

		Real _far;
		void init();

		Real getElevation(const Real3& point)
		{
			return 0;
		}

		Real3 getElevationPoint(const Real3& point);

		void forward(Real step);

		void nearTerrainForward(Real step);

		void left(Real step);

		void nearTerrainLeft(Real step);

		void move(const Real3& eye, const Real3& center,
			const Real3& up, const Real3& step);

		void head(Real angle);

		void pitch(Real angle);

		void keyEvent(unsigned int key, bool down);

		Real3 computeOrthogonalVector(const Real3& direction);

		void mouseEvent(MouseButtonMask button, InputType state, int x, int y, int scroll);

	private:

		bool _init;
		bool _nearTerrain;
		Real _forceEle;

		IEngine* _engine;
		ICamera* _camera;
		IDrawable* _terrain;

		int _lastX;
		int _lastY;

		Real _step;

		bool _lock;
	};
}

#endif