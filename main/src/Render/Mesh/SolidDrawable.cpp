#include "SolidDrawable.h"
#include "Mesh.h"
#include "Box.h"
#include "Sphere.h"
#include "HeightMap.h"

namespace open
{
	SolidGeometry::SolidGeometry()
	{
		_geometry = new Mesh();
		_geometry->autoBoundingBox(false);
	}

	SolidGeometry::~SolidGeometry()
	{
		delete _geometry;
	}

	void SolidGeometry::setSoildShape(const SolidShape& solid)
	{
		this->_shape = solid;
		if(solid.type == SolidShape::TYPE_BOX)
		{
			BoxDrawalbe bmd(_geometry,solid.shape.box);
		}
		else if(solid.type == SolidShape::TYPE_SPHERE)
		{
			SphereDrawable smd(_geometry,solid.shape.sphere);
		}
		else if(solid.type == SolidShape::TYPE_HEIGHTMAP)
		{
			HeightMapDrawalbe hmd(_geometry,solid.shape.heightMap);
		}
	}

	void SolidGeometry::draw(IRenderPool* renderPool) { _geometry->draw(renderPool); }

	void SolidGeometry::draw(IRenderPool* renderPool, IVertexStream* instances)
	{
		_geometry->draw(renderPool, instances);
	}

	const RBoundingBox& SolidGeometry::getBoudingBox() { return _geometry->getBoudingBox(); }

	void SolidGeometry::accept(VisitGeometryVertex& dv) { _geometry->accept(dv); }

}