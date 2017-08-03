#ifndef MESH_BOX_H
#define MESH_BOX_H 1

#include <open/interface/Render/DrawableInterface.h>

namespace open
{
	class Mesh;
	class BoxDrawalbe
	{
	public:

		BoxDrawalbe(Mesh* geometry, const SolidShape::Shape::Box& box);

	private:
		void computeBound(Mesh* geometry, const SolidShape::Shape::Box& box);

		void reset(Mesh* geometry, const SolidShape::Shape::Box& box);

		void createNormals(Mesh* geometry);

		void createVertexs(Mesh* geometry, const SolidShape::Shape::Box& box);
	};
}

#endif