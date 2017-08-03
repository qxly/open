#ifndef SCENE_SCENEMANAGERINTERFACE_H
#define SCENE_SCENEMANAGERINTERFACE_H

#include <open/Array.h>
#include <open/Maths.h>

namespace open
{
	class INode;
	class IDrawable;
	class IGeometry;
	class IMaterial;
	class ICamera;
	struct DrawableRenderTrace;

	struct IntersectResultIterator
	{
		INode* node;
		IDrawable* drawable;
		IGeometry* geometry;

		Real3 modelVertex;
		Real3 worldVertex;

		Real intersectRatio;

		bool operator < (const IntersectResultIterator& other) const
		{
			return intersectRatio < other.intersectRatio;
		}
		IntersectResultIterator()
			:node(nullptr), drawable(nullptr), geometry(nullptr), intersectRatio(FLT_MAX) {}
	};
	class IScene
	{
	public:

		virtual ~IScene() {}

		virtual void addNode(INode* node) = 0;

		virtual void removeNode(INode* node) = 0;

		virtual void intersect(const RRay& ray) = 0;

		virtual void intersect(const RRay& ray, INode* node) = 0;

		virtual void intersect(const RRay& ray, INode** nodes, int32 size) = 0;

		virtual bool nextResulte(IntersectResultIterator& iternator) = 0;

		virtual void culled(ICamera* camera, uint32 renderMask, DynamicArray<INode*>& renderNodes) = 0;
	};
}

#endif