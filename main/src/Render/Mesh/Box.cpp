#include "Box.h"
#include "Mesh.h"
#include <open/interface/RenderSystem/RenderEnumeration.h>

namespace open
{
	BoxDrawalbe::BoxDrawalbe(Mesh* geometry, const SolidShape::Shape::Box& box)
	{
		reset(geometry, box);
	}

	void BoxDrawalbe::computeBound(Mesh* geometry, const SolidShape::Shape::Box& box)
	{
		Real3 t(box.width / 2.0f, box.length / 2.0f, box.height / 2.0f);

		RBoundingBox boudingBox(Real3(box.center[0] - t.x(), box.center[1] - t.y(), box.center[2] - t.z()),
			Real3(box.center[0] + t.x(), box.center[1] + t.y(), box.center[2] + t.z()));

		geometry->autoBoundingBox(false);
		geometry->setBoudingBox(boudingBox);
	}

	void BoxDrawalbe::createNormals(Mesh* geometry)
	{
		Vec3f* normals = new Vec3f[36];
		int index = 0;
		///前面
		normals[index++].set(1, 0, 0);
		normals[index++].set(1, 0, 0);
		normals[index++].set(1, 0, 0);
		normals[index++].set(1, 0, 0);
		normals[index++].set(1, 0, 0);
		normals[index++].set(1, 0, 0);

		///后面
		normals[index++].set(-1, 0, 0);
		normals[index++].set(-1, 0, 0);
		normals[index++].set(-1, 0, 0);
		normals[index++].set(-1, 0, 0);
		normals[index++].set(-1, 0, 0);
		normals[index++].set(-1, 0, 0);

		///右面
		normals[index++].set(0, 1, 0);
		normals[index++].set(0, 1, 0);
		normals[index++].set(0, 1, 0);
		normals[index++].set(0, 1, 0);
		normals[index++].set(0, 1, 0);
		normals[index++].set(0, 1, 0);

		////左面
		normals[index++].set(0, -1, 0);
		normals[index++].set(0, -1, 0);
		normals[index++].set(0, -1, 0);
		normals[index++].set(0, -1, 0);
		normals[index++].set(0, -1, 0);
		normals[index++].set(0, -1, 0);

		///顶面
		normals[index++].set(0, 0, 1);
		normals[index++].set(0, 0, 1);
		normals[index++].set(0, 0, 1);
		normals[index++].set(0, 0, 1);
		normals[index++].set(0, 0, 1);
		normals[index++].set(0, 0, 1);

		///底面
		normals[index++].set(0, 0, -1);
		normals[index++].set(0, 0, -1);
		normals[index++].set(0, 0, -1);
		normals[index++].set(0, 0, -1);
		normals[index++].set(0, 0, -1);
		normals[index++].set(0, 0, -1);

		geometry->setNormalArray(normals);
	}

	void BoxDrawalbe::createVertexs(Mesh* geometry, const SolidShape::Shape::Box& box)
	{
		Vec3f* verts = new Vec3f[36];
		int index = 0;

		Matrixf t;
		Matrixf s;
		Matrixf transe;

		t.makeTranslate(box.center[0], box.center[1], box.center[2]);
		s.makeScale(box.width, box.length, box.height);
		transe = t * s;

		Vec3f or (0.5, 0.5, 0.5);

		/////以中心点为原点
		///前面
		verts[index++] = transe * (Vec3f(1, 0, 1) - or );
		verts[index++] = transe * (Vec3f(1, 0, 0) - or );
		verts[index++] = transe * (Vec3f(1, 1, 0) - or );

		verts[index++] = transe * (Vec3f(1, 0, 1) - or );
		verts[index++] = transe * (Vec3f(1, 1, 0) - or );
		verts[index++] = transe * (Vec3f(1, 1, 1) - or );

		///后面
		verts[index++] = transe * (Vec3f(0, 0, 1) - or );
		verts[index++] = transe * (Vec3f(0, 1, 0) - or );
		verts[index++] = transe * (Vec3f(0, 0, 0) - or );

		verts[index++] = transe * (Vec3f(0, 0, 1) - or );
		verts[index++] = transe * (Vec3f(0, 1, 1) - or );
		verts[index++] = transe * (Vec3f(0, 1, 0) - or );

		////右面
		verts[index++] = transe * (Vec3f(1, 1, 1) - or );
		verts[index++] = transe * (Vec3f(1, 1, 0) - or );
		verts[index++] = transe * (Vec3f(0, 1, 0) - or );

		verts[index++] = transe * (Vec3f(1, 1, 1) - or );
		verts[index++] = transe * (Vec3f(0, 1, 0) - or );
		verts[index++] = transe * (Vec3f(0, 1, 1) - or );

		////左面
		verts[index++] = transe * (Vec3f(1, 0, 1) - or );
		verts[index++] = transe * (Vec3f(0, 0, 0) - or );
		verts[index++] = transe * (Vec3f(1, 0, 0) - or );

		verts[index++] = transe * (Vec3f(1, 0, 1) - or );
		verts[index++] = transe * (Vec3f(0, 0, 1) - or );
		verts[index++] = transe * (Vec3f(0, 0, 0) - or );

		////顶面
		verts[index++] = transe * (Vec3f(0, 0, 1) - or );
		verts[index++] = transe * (Vec3f(1, 0, 1) - or );
		verts[index++] = transe * (Vec3f(1, 1, 1) - or );

		verts[index++] = transe * (Vec3f(0, 0, 1) - or );
		verts[index++] = transe * (Vec3f(1, 1, 1) - or );
		verts[index++] = transe * (Vec3f(0, 1, 1) - or );

		////底面
		verts[index++] = transe * (Vec3f(0, 0, 0) - or );
		verts[index++] = transe * (Vec3f(1, 1, 0) - or );
		verts[index++] = transe * (Vec3f(1, 0, 0) - or );

		verts[index++] = transe * (Vec3f(0, 0, 0) - or );
		verts[index++] = transe * (Vec3f(0, 1, 0) - or );
		verts[index++] = transe * (Vec3f(1, 1, 0) - or );

		geometry->setVertexArray(verts);
	}

	void BoxDrawalbe::reset(Mesh* geometry, const SolidShape::Shape::Box& box)
	{
		computeBound(geometry, box);

		geometry->begin(36);

		createNormals(geometry);
		createVertexs(geometry, box);

		geometry->end();

		if (geometry->getNumIndexArray())
			geometry->setIndexArray(0, TRIANGLE_LIST, 0U, 36U);
		else
			geometry->addIndexArray(TRIANGLE_LIST, 0U, 36U);
	}
}