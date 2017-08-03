#ifndef SCENEMANAGER_RENDERSORTER_H
#define SCENEMANAGER_RENDERSORTER_H 1

#include <common/Array.h>
#include <common/Maths.h>
#include <common/interface/Scene/MeshInterface.h>

#include "../InstanceInfo.h"

namespace open
{
	class INode;
	class IGeometry;
	class IMaterial;
	class ICamera;
	class IVertexArrayObject;
	class IVertexIndexObject;

	DECLARE_AND_TYPEDEF_SAMLLTYPE_ARRARY(Real);
	

	struct GeometryInstance
	{
		IGeometry* geometry;
		GeneralInstanceArray* instanceInfos;
		floatArray* distanceToEyes;
		GeometryInstance()
			:geometry(NULL), instanceInfos(NULL), distanceToEyes(NULL){}

		inline void destory()
		{
			delete instanceInfos;
			delete distanceToEyes;
		}
	};
	DECLARE_AND_TYPEDEF_SAMLLTYPE_ARRARY(GeometryInstance);

	struct RenderTrace
	{
		RenderTrace()
		:material(NULL), geometries(NULL){}

		RenderTrace(IMaterial* material_)
			:material(material_), geometries(NULL){}

		void destory()
		{
			if (!geometries)
				return;
			for (int32 i = 0; i < geometries->size(); i++)
			{
				geometries->at(i).destory();
			}
			delete geometries;
		}

		IMaterial* material;
		GeometryInstanceArray* geometries;
	};
	DECLARE_AND_TYPEDEF_SAMLLTYPE_ARRARY(RenderTrace);

	class IRenderSorter
	{
	public:
		virtual void begin() = 0;

		virtual void sort(ICamera* camera,INode** nodes, int32 count,bool ignorMaterial) = 0;

		virtual RenderTraceArray& end() = 0;

		virtual RenderTraceArray& endTransparents() = 0;
	};

	////渲染排序的原则是：先渲染不透明物体，在渲染透明物体，不透明减少材质切换(相同材质、纹理、状态),透明物体按照位置
	class SimpleRenderSorter : public IRenderSorter
	{
	public:
		virtual void begin();

		virtual void sort(ICamera* camera, INode** nodes, int32 count, bool ignorMaterial);

		virtual RenderTraceArray& end();

		virtual RenderTraceArray& endTransparents();

	private:

		void sortGeometry(IGeometry* geometry, IMaterial* material);

		void insert(Real lg, IGeometry* geometry, IMaterial* material);

		RenderTrace& getOrCreateRenderTrace(RenderTraceArray& rts, IMaterial* material);

		GeometryInstance& getOrCreateGeometryInstance(RenderTrace& rt, IGeometry* geometry);

		void setInstanceInfos(GeometryInstance& gi);

		inline void destoryRenderTraces(RenderTraceArray& drs)
		{
			for (int32 i = 0; i < drs.size(); i++)
			{
				drs[i].destory();
			}
		}
	private:

		RenderTraceArray _transparentList;
		RenderTraceArray _opacityList;

		GeneralInstance _currentDrawInstance;

		Real3 _eyePoint;
		Real3 _dirction;

		int32 _count;
		int32 _current;

		bool _ignorMaterial;
	};
}

#endif