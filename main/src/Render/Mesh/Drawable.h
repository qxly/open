#ifndef RENDER_DRAWABLE_H
#define RENDER_DRAWABLE_H 1

#include <vector>
#include <open/Ptr.h>
#include <open/Memory.h>
#include <open/interface/Render/DrawableInterface.h>

namespace open
{
	class Config;
	class Drawable : public TPtr<IDrawableBase> PUBLIC_OBJECT_DEBUG
	{
		struct GeometryMaterialArray;

	public:

		Drawable();

		virtual ~Drawable();

		virtual void setBone(IBone* bone);
		virtual IBone* getBone();

		virtual uint32 getNumGeometry();
		virtual IGeometry* getGeometry(uint32 index);
		virtual IMaterial* getMaterial(uint32 index);

		virtual void addGeometry(IGeometry* geometry, IMaterial* material);
		virtual void setMaterial(uint32 index, IMaterial* material);

		virtual void removeGeometry(uint32 index);

		virtual const RBoundingBox& getBoudingBox();

		virtual void accept(VisitGeometryVertex dv);
		virtual void accept(VisitGeometry vg);

	private:
		void computeBounding();
	private:
		IBone* _bone;
		GeometryMaterialArray* _geometryMaterials;

		bool _resetBoudingBox;
		RBoundingBox _boudingBox;
	};

	class DrawableReadWriteHelper
	{
	public:
		static IDrawable* readDrawable(const char* file, const Config& config);

		static IDrawable* readDrawable(const char* type, const char* buffer, uint32 size, const Config& config);

		static bool registe(DrawableReadWriteHelper* helper);

		static void unstall();

		virtual ~DrawableReadWriteHelper() {}

	protected:

		virtual const char* support() const = 0;

		virtual IDrawable* readDrawable(const char* buffer, uint32 size, const Config& config) = 0;

		static std::vector<DrawableReadWriteHelper*> s_helpers;
	};
#define REGISTE_DRAWABLEREADWRITEHELPER(helper)\
	int helper##DrawableReadWriteHelper = DrawableReadWriteHelper::registe(new helper);

}

#endif