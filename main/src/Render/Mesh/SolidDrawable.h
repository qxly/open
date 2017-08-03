#ifndef MESH_SOLIDGEOMETRY_H
#define MESH_SOLIDGEOMETRY_H

#include <open/Ptr.h>
#include <open/Memory.h>
#include <open/interface/Render/DrawableInterface.h>

namespace open
{
	class Mesh;
	class SolidGeometry : public TPtr<ISolidGeometry> PUBLIC_OBJECT_DEBUG
	{
	public:

		SolidGeometry();

		~SolidGeometry();

		virtual void setSoildShape(const SolidShape& solid) ;

		virtual const SolidShape& getSolidShape() { return _shape;}

		virtual void draw(IRenderPool* renderPool);

		void draw(IRenderPool* renderPool, IVertexStream* instances);

		virtual const RBoundingBox& getBoudingBox();
		
		virtual void accept(VisitGeometryVertex& dv);
	private:
		void rebuild();

	private:
		Mesh* _geometry;
		SolidShape _shape;
	};
}

#endif