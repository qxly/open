#include "RenderSorter.h"
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderInterface.h>
#include <Scene/NodeInterface.h>
#include <Scene/MeshInterface.h>

#include <common/Algorithm.h>

namespace open
{
	void SimpleRenderSorter::begin()
	{
		destoryRenderTraces(_transparentList);
		destoryRenderTraces(_opacityList);
		_transparentList.clear();
		_opacityList.clear();
	}

	void SimpleRenderSorter::sort(ICamera* camera, INode** nodes, int32 count, bool ignorMaterial)
	{
		_ignorMaterial = ignorMaterial;
		for (int32 i = 0; i < count; i++)
		{
			INode* node = nodes[i];
			IDrawable* drawable = node->getDrawable();

			////
			_currentDrawInstance.matrix = camera->getProject() * camera->getViewMatrix() * node->getMatrix();
			_currentDrawInstance.matrix = _currentDrawInstance.matrix.getTranspose();

			////·¨Ïß
			_currentDrawInstance.normalMatrix = camera->getViewMatrix() * node->getMatrix();
			RQuat nn = _currentDrawInstance.normalMatrix.getRotate();
			_currentDrawInstance.normalMatrix.makeRotate(nn);

			_currentDrawInstance.normalMatrix = _currentDrawInstance.normalMatrix.getTranspose();

			drawable->accept(functor(*this, &SimpleRenderSorter::sortGeometry));
		}
	}

	struct Find_Material
	{
		inline static bool compare(const  RenderTrace& l, const RenderTrace& r)
		{
			return l.material < r.material;
		}
	};

	RenderTrace& SimpleRenderSorter::getOrCreateRenderTrace(RenderTraceArray& rts, IMaterial* material)
	{
		RenderTrace rt(material);
		if (rts.empty())
		{
			rt.geometries = new GeometryInstanceArray;
			rts.push_back(rt);
			return rts[0];
		}

		int32 find = lower_bound<RenderTrace, Find_Material>(rts.getDataPointer(), rts.size(), rt);
		if (find == -1 || find >= rts.size() || Find_Material::compare(rt, rts[find]))
		{
			rt.geometries = new GeometryInstanceArray;
			rts.insert(find, rt);
		}
		return rts[find];
	}

	GeometryInstance& SimpleRenderSorter::getOrCreateGeometryInstance(RenderTrace& rt, IGeometry* geometry)
	{
		for (int32 i = 0; i < rt.geometries->size(); i++)
		{
			GeometryInstance& gi = rt.geometries->at(i);
			if (gi.geometry == geometry)
			{
				return gi;
			}
		}
		GeometryInstance gi;
		gi.geometry = geometry;
		gi.instanceInfos = new GeneralInstanceArray;
		rt.geometries->push_back(gi);

		return rt.geometries->back();
	}

	void SimpleRenderSorter::setInstanceInfos(GeometryInstance& gi)
	{
		gi.instanceInfos->push_back(_currentDrawInstance);
	}

	void SimpleRenderSorter::sortGeometry(IGeometry* geometry, IMaterial* material)
	{
		if (!_ignorMaterial)
		{
			if (material->isTransparentMaterial())
			{
				Real3 ps = _currentDrawInstance.matrix.getTrans();
				Real lg = (ps - _eyePoint).length2();
				Real d = _dirction * ps;
				lg = d > 0 ? lg : -lg;

				RenderTrace& rt = getOrCreateRenderTrace(_transparentList, material);
				GeometryInstance& gi = getOrCreateGeometryInstance(rt, geometry);
				setInstanceInfos(gi);
			}
			else
			{
				RenderTrace& rt = getOrCreateRenderTrace(_opacityList, material);
				GeometryInstance& gi = getOrCreateGeometryInstance(rt, geometry);
				setInstanceInfos(gi);
			}
		}
		else
		{
			RenderTrace& rt = getOrCreateRenderTrace(_opacityList, NULL);
			GeometryInstance& gi = getOrCreateGeometryInstance(rt, geometry);
			setInstanceInfos(gi);
		}
	}

	void SimpleRenderSorter::insert(Real lg, IGeometry* geometry, IMaterial* material)
	{
	}

	RenderTraceArray& SimpleRenderSorter::end()
	{
		return _opacityList;
	}

	RenderTraceArray& SimpleRenderSorter::endTransparents()
	{
		return _transparentList;
	}
}