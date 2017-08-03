#ifndef SCENE_NODEINTERFACE_H
#define SCENE_NODEINTERFACE_H 1

#include <open/Array.h>
#include <open/interface/Scene/MeshInterface.h>

namespace open
{
	class IDrawable;
	class INode;
	class IMaterial;
	class IGeometry;
	class IRenderPool;
	class ICamera;

	class INode
	{
	public:

		virtual void setName(const char* name) = 0;

		virtual const char* getName() = 0;

		virtual void setVisible(bool value) = 0;

		virtual bool getVisible() = 0;

		virtual void applyRenderMask(uint32 renderMask) = 0;

		virtual void removeRenderMask(uint32 renderMask) = 0;

		virtual uint32 getRenderMask() = 0;

		virtual void setParentMatrix(const RMatrix& parentMatrix) = 0;

		virtual void setTranse(const Real3& transe) = 0;

		virtual void setScale(const Real3& scale) = 0;

		virtual void setRotate(const RQuat& quat) = 0;

		virtual const RMatrix& getMatrix() = 0;

		virtual void setDrawable(IDrawable* drawable) = 0;

		virtual IDrawable* getDrawable() = 0;

		virtual const RBoundingBox& getBoudingBox() = 0;
	};

	
}

#endif