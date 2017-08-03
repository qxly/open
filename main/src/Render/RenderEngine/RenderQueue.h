#ifndef RENDER_RENDERQUEUE_H
#define RENDER_RENDERQUEUE_H

#include <vector>
#include <open/Maths.h>

namespace open {

	class IGeometry;
	class IMaterial;
	class RenderQueue {
	private:
		struct RenderLeafs;
		static int lessRenderLeaf(const void* left, const void* right);

	public:

		struct Leaf
		{
			IMaterial* materail = nullptr;
			IGeometry* geometry = nullptr;
			RMatrix matrix;
			Real lg = 0;

			void* userData = nullptr;

			Leaf() = default;

			Leaf(IMaterial*m, IGeometry* g, const RMatrix& rm, void* u)
				:materail(m), geometry(g), matrix(rm), userData(u) {}
		};
		typedef std::vector<Leaf*> Leafs;

	public:

		RenderQueue();
		~RenderQueue();

		void add(IGeometry* geometry, IMaterial* material, const RMatrix& matrix, void* userData = nullptr);

		const RenderQueue::Leafs& getOpacityRenderLeaf();

		const RenderQueue::Leafs& getTransparencyRenderLeaf();

		void clear();

		void sort();

	private:
		RenderLeafs* _renderLeafs;
		bool _reSort = false;
	};
}

#endif