#include "Sphere.h"
#include "Mesh.h"
#include <open/Array.h>
#include <open/interface/RenderSystem/RenderEnumeration.h>

namespace open
{
	SphereDrawable::SphereDrawable(Mesh* geometry, const SolidShape::Shape::Sphere& sphere)
	{
		computeBound(geometry,sphere);
		resetGeometry(geometry,sphere);
	}

	void SphereDrawable::computeBound(Mesh* geometry, const SolidShape::Shape::Sphere& sphere)
	{
		RBoundingBox boudingBox;
		boudingBox.expandBy(Real3(sphere.center[0],sphere.center[1],sphere.center[2]),sphere.radius);

		geometry->autoBoundingBox(false);
		geometry->setBoudingBox(boudingBox);
	}

	void SphereDrawable::resetGeometry(Mesh* geometry, const SolidShape::Shape::Sphere& sphere)
	{
		DynamicArray<Vec3f> verts;
		DynamicArray<Vec3f> normals;
		DynamicArray<Vec2f> texCoords;

		Matrixf t;
		Matrixf s;
		Matrixf transe;

		t.makeTranslate(sphere.center[0],sphere.center[1],sphere.center[2]);
		s.makeScale(sphere.radius,sphere.radius,sphere.radius);
		transe = t * s;

		int32 numSegments = 40;
		int32 numRows = 20;
		float ratio = 1.0f;

		float lDelta = PI / (float)numRows;
		float vDelta = 1.0f / (float)numRows;

		float angleDelta = PI * 2.0f / (float)numSegments;
		float texCoordHorzDelta = 1.0f / (float)numSegments;


		float lBase = -PI * 0.5f;
		float rBase = 0.0f;
		float zBase = -1;
		float vBase = 0.0f;
		float nzBase = -1.0f;
		float nRatioBase = 0.0f;

		for(int32 rowi = 0; rowi < numRows; ++rowi)
		{
			float lTop = lBase + lDelta;
			float rTop = cosf(lTop);
			float zTop = sinf(lTop);
			float vTop = vBase + vDelta;
			float nzTop = sinf(lTop);
			float nRatioTop = cosf(lTop);

			float angle = 0.0f;
			float texCoord = 0.0f;

			for(int32 topi = 0; topi < numSegments;
				++topi,angle += angleDelta,texCoord += texCoordHorzDelta)
			{
				float c = cosf(angle);
				float s = sinf(angle);

				normals.push_back(Vec3f(c * nRatioTop,s * nRatioTop,nzTop));
				texCoords.push_back(Vec2f(texCoord,vTop));
				verts.push_back(transe * Vec3f(c * rTop,s * rTop,zTop));

				normals.push_back(Vec3f(c * nRatioBase,s * nRatioBase,nzBase));
				texCoords.push_back(Vec2f(texCoord,vBase));
				verts.push_back( transe * Vec3f(c * rBase,s * rBase,zBase));
			}

			lBase=lTop;
			rBase=rTop;
			zBase=zTop;
			vBase=vTop;
			nzBase=nzTop;
			nRatioBase=nRatioTop;
		}

		int32 size = verts.size();
		geometry->begin(size);

		geometry->setVertexArray(verts.getDataPointer());
		geometry->setNormalArray(normals.getDataPointer());
		geometry->setTextureCoordArray(0,texCoords.getDataPointer());

		geometry->end();

		verts.detachPointer();
		normals.detachPointer();
		texCoords.detachPointer();

		if(geometry->getNumIndexArray())
			geometry->setIndexArray(0,TRIANGLE_STRIP,0U,size);
		else
			geometry->addIndexArray(TRIANGLE_STRIP, 0U,size);
	}
}