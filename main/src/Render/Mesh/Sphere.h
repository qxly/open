#ifndef MESH_SPHERE_H
#define MESH_SPHERE_H 1

#include <open/interface/Render/DrawableInterface.h>

namespace open
{
	class Mesh;
	class SphereDrawable
	{
	public:

		SphereDrawable(Mesh* geometry, const SolidShape::Shape::Sphere& sphere);

	private:
		void computeBound(Mesh* geometry, const SolidShape::Shape::Sphere& sphere);

		void resetGeometry(Mesh* geometry, const SolidShape::Shape::Sphere& sphere);
	};
}

#endif