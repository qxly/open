#include "ClusteredUtil.h"
#include <open/interface/Render/RenderEngine.h>
#include <open/interface/RenderSystem/RenderInterface.h>
#include <open/interface/RenderSystem/RenderEnumeration.h>

#define TILE_SIZE 32
#define NUM_DEPTH_SLICES 8
#define PRE_TILE_MAX_LIGHT 64

namespace open {

	Real2 calcTileScale(uint32 targetWidth, uint32 targetHeight) {
		uint32 w = (targetWidth + TILE_SIZE - 1) & ~(TILE_SIZE - 1);
		uint32 h = (targetHeight + TILE_SIZE - 1) & ~(TILE_SIZE - 1);
		return Real2(w / (2.0f * TILE_SIZE), h / (2.0f * TILE_SIZE));
	}

	void calcNearFars(Real nearPlane, Real farPlane, Real depthSlices[NUM_DEPTH_SLICES + 1]) {
		Real const base = farPlane / nearPlane;
		depthSlices[0] = nearPlane;
		for (uint32 i = 1; i < NUM_DEPTH_SLICES; ++i)
		{
			depthSlices[i] = nearPlane * pow(base, static_cast<float>(i) / NUM_DEPTH_SLICES);
		}
		depthSlices[NUM_DEPTH_SLICES] = farPlane;
	}

	void calcTileViewFrustum(uint32 x, uint32 y, float nearPlane, float farPlane,
		const Real2& tileScale, const RMatrix& pvp, RPlane planes[6])
	{
		if (farPlane - nearPlane < 1e-3f)
		{
			farPlane += 1e-3f;
		}

		Real2 coord = Real2(static_cast<float>(x), static_cast<float>(y));
		Real2 tile_bias = tileScale - coord;
		float q = farPlane / (farPlane - nearPlane);

		Real4 column1 = Real4(pvp(0, 0) * tileScale.x(), 0, tile_bias.x(), 0);
		Real4 column2 = Real4(0, -pvp(1, 1) * tileScale.y(), tile_bias.y(), 0);
		Real4 column3 = Real4(0, 0, q, -nearPlane * q);
		Real4 column4 = Real4(0, 0, 1, 0);
		planes[0].set(column4 - column1);
		planes[1].set(column4 + column1);
		planes[2].set(column4 - column2);
		planes[3].set(column4 + column2);
		planes[4].set(column4 - column3);
		planes[5].set(column4 + column3);
		for (int i = 0; i < 6; ++i)
		{
			planes[i].makeUnitLength();
		}
	}

	bool overlapTestSphere(RPlane planes[6], const Real3& center, Real range)
	{
		int overlap = 1;
		for (int i = 0; i < 6; ++i)
		{
			Real d = planes[i].distance(center);
			overlap *= (d > -range);
		}
		return overlap;
	}

	bool overlapTestBox(RPlane planes[6], const Real3& aabbMin, const Real3& aabbMax)
	{
		Real3 min = aabbMin;
		Real3 max = aabbMax;

		int overlap = 1;
		for (int i = 0; i < 6; ++i)
		{
			const Real* ptr = planes[i].ptr();
			Real3 v0(ptr[0] < 0 ? min.x() : max.x(), ptr[1] < 0 ? min.y() : max.y(), ptr[2] < 0 ? min.z() : max.z());
			float d = planes[i].distance(v0);
			overlap *= (d >= 0);
		}
		return overlap;
	}

	bool isLightInTile(ILight* light, const RMatrix& view, RPlane planes[6]) {
		auto type = light->getLightType();
		Real zn, zf;
		light->getLightRange(zn, zf);

		if (type == LIGHT_POINT) {
			Real3 loc = view * light->getLightPosition();
			return overlapTestSphere(planes, loc, zf);
		}
		return false;
	}

	ClusteredCPU::ClusteredCPU()
		:_tileIndexsSize(0), _tileLightIndexsSize(0) {
		_tileCount = TILE_SIZE * TILE_SIZE * (NUM_DEPTH_SLICES - 1);
		_tileIndexs = new uint32[_tileCount];
		_tileLightIndexs = new uint16[_tileCount * PRE_TILE_MAX_LIGHT];
	}

	void ClusteredCPU::calcLightList(ICamera* camera, ILight** lights, uint16 size) {
		ASSERT_(lights && size > 0);
		_tileIndexsSize = 0;
		_tileLightIndexsSize = 0;

		uint32 vp[4];
		Real2 nearFar;
		const RMatrix& proj = camera->getProject();
		const RMatrix& view = camera->getViewMatrix();
		camera->getViewport(vp[0], vp[1], vp[2], vp[3]);
		camera->getNearFar(nearFar.x(), nearFar.y());

		Real2 tileScale = calcTileScale(vp[2], vp[3]);
		Real depthSlices[NUM_DEPTH_SLICES];
		calcNearFars(nearFar.x(), nearFar.y(), depthSlices);

		uint32 x = 0;
		uint32 y = 0;
		uint32 z = 0;
		uint32 xy = TILE_SIZE * TILE_SIZE;
		RPlane planes[6];
		for (size_t i = 0; i < _tileCount; i++) {
			_tileLightIndexs[_tileLightIndexsSize++] = 0;
			_tileIndexs[i] = _tileLightIndexsSize - 1;

			z = i % xy;
			y = (i - z * xy) % TILE_SIZE;
			x = i - z * xy - y * TILE_SIZE;
			calcTileViewFrustum(x, y, depthSlices[z], depthSlices[z + 1], tileScale, proj, planes);

			uint8 inTileLightNum = 0;
			for (uint16 l = 0; l < size; ++l) {
				auto light = lights[l];
				if (!isLightInTile(light, view, planes)) {
					continue;
				}
				if (inTileLightNum > PRE_TILE_MAX_LIGHT) {
					break;
				}
				inTileLightNum++;
				_tileLightIndexs[_tileLightIndexsSize++] = l;
			}
			_tileLightIndexs[_tileLightIndexsSize - inTileLightNum] = inTileLightNum;
		}
	}
}