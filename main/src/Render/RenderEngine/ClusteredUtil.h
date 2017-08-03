#ifndef RENDER_RENDERQUEUE_H
#define RENDER_RENDERQUEUE_H

#include <vector>
#include <open/Maths.h>

namespace open {
	class ILight;
	class ICamera;

	class ClusteredCPU {
	public:
		ClusteredCPU();

		void calcLightList(ICamera* camera, ILight** lights, uint16 size);

		const uint32* getTileIndexs(uint32& size) const { size = _tileIndexsSize;  return _tileIndexs; }

		const uint16* getTileLightIndexs(uint32& size) const { size = _tileLightIndexsSize;  return _tileLightIndexs; }

	private:
		uint32 _tileIndexsSize;
		uint32* _tileIndexs;

		uint32 _tileLightIndexsSize;
		uint16* _tileLightIndexs;

		uint32 _tileCount;
	};

}

#endif