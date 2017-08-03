#include "RenderQueue.h"

#include <Render/RenderEngine.h>
#include <Render/MaterialInterface.h>

#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderEnumeration.h>

#include <vector>

namespace open {

	inline int RenderQueue::lessRenderLeaf(const void* left, const void* right)
	{
		RenderQueue::Leaf* l = *((RenderQueue::Leaf**)left);
		RenderQueue::Leaf* r = *((RenderQueue::Leaf**)right);

		if (l->materail < r->materail)
			return 1;
		else if (l->materail > r->materail)
			return -1;
		else if (l->geometry < r->geometry)
			return true;
		else if (l->geometry > r->geometry)
			return -1;
		else if (l->userData < r->userData)
			return 1;
		else
			return -1;
	}

	struct RenderQueue::RenderLeafs
	{
		inline void add(IGeometry* geometry, IMaterial* material, const RMatrix& matrix, void* userData)
		{
			if (material->getRenderStaeSet().getMode(BLEND)) {
				_transparencyRenderLeaf.push_back(new RenderQueue::Leaf(material, geometry, matrix, userData));
			}
			else {
				_opacityRenderLeaf.push_back(new RenderQueue::Leaf(material, geometry, matrix, userData));
			}
		}

		inline void sort() {
			sortOpacity();
			sortTransparency();
		}

		inline void clear() {

		}

		inline const RenderQueue::Leafs& getOpacityRenderLeaf() { return _opacityRenderLeaf; }

		inline const RenderQueue::Leafs& getTransparencyRenderLeaf() { return _transparencyRenderLeaf; }

	private:
		inline void sortOpacity() {
			if (_opacityRenderLeaf.empty())
				return;

			qsort(&_opacityRenderLeaf.front(), _opacityRenderLeaf.size(), sizeof(RenderQueue::Leaf*), &RenderQueue::lessRenderLeaf);
			_opacityRenderLeaf.push_back(new RenderQueue::Leaf());

		}

		void sortTransparency() {
			if (_transparencyRenderLeaf.empty())
				return;

			qsort(&_transparencyRenderLeaf.front(), _transparencyRenderLeaf.size(), sizeof(RenderQueue::Leaf*), &RenderQueue::lessRenderLeaf);
			_transparencyRenderLeaf.push_back(new RenderQueue::Leaf());
		}

	private:
		RenderQueue::Leafs _opacityRenderLeaf;
		RenderQueue::Leafs _transparencyRenderLeaf;
	};


	RenderQueue::RenderQueue()
	{
		_renderLeafs = new RenderLeafs;
	}

	RenderQueue::~RenderQueue()
	{
		delete _renderLeafs;
	}

	void RenderQueue::add(IGeometry* geometry, IMaterial* material, const RMatrix& matrix, void* userData) {

		if (!geometry)
			return;

		_renderLeafs->add(geometry, material, matrix, userData);
		_reSort = true;
	}

	const  RenderQueue::Leafs& RenderQueue::getOpacityRenderLeaf() {
		return _renderLeafs->getOpacityRenderLeaf();
	}

	const  RenderQueue::Leafs& RenderQueue::getTransparencyRenderLeaf() {
		return _renderLeafs->getTransparencyRenderLeaf();
	}

	void RenderQueue::clear() {
		_reSort = true;
	}

	void RenderQueue::sort() {
		if (_reSort) {
			_reSort = false;
			_renderLeafs->sort();
		}
	}
}