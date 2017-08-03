#ifndef RENDER_DEFERREDUTIL_H
#define RENDER_DEFERREDUTIL_H 1

#include <open/Array.h>

#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>
#include <RenderSystem/RenderEnumeration.h>

#include <Render/DrawableInterface.h>
#include <Render/MaterialInterface.h>
#include <Render/RenderFactoryInterface.h>

namespace open
{
	namespace DeferredUtil
	{
		extern GBuffer g_buffer;

		struct Material
		{
			RenderStateSet stateSet;
			ITextureUnitSet* textureUnitSet;

			Material()
			{
				textureUnitSet = RenderSystemFactory::s_factory->createTextureUnitSet();
			}

			Material(const Material&) = delete;

			void apply(IRenderPool* renderPool)
			{
				renderPool->applyRenderStateSet(stateSet);
				renderPool->applyTextureUnits(textureUnitSet);
			}
		};

		inline IGeometry* genScreenRect()
		{
			IMesh* screenRect = RenderFactory::s_factory->createMesh();

			Vec3f* vertex = new Vec3f[4];
			uint16* index = new uint16[6];

			vertex[0] = Vec3f(-1, -1, 0);
			vertex[1] = Vec3f(1, -1, 0);
			vertex[2] = Vec3f(1, 1, 0);
			vertex[3] = Vec3f(-1, 1, 0);

			index[0] = 0; index[1] = 1; index[2] = 2;
			index[3] = 0; index[4] = 2; index[5] = 3;

			screenRect->begin(4);

			screenRect->setVertexArray(vertex);
			screenRect->addIndexArray(TRIANGLE_LIST, index, 6);

			screenRect->end();

			return screenRect;
		}

		inline IMesh* genSphere()
		{
			IMesh* sphere = RenderFactory::s_factory->createMesh();

			DynamicArray<Vec3f> verts;

			int32 numSegments = 20;
			int32 numRows = 20;
			float ratio = 3;

			float lDelta = PI / (float)numRows;
			float vDelta = 1.0f / (float)numRows;

			float angleDelta = PI * 2.0f / (float)numSegments;
			float texCoordHorzDelta = 1.0f / (float)numSegments;


			float lBase = -PI * 0.5f;
			float rBase = 0.0f;
			float zBase = -1;
			float vBase = 0.0f;
			float nzBase = -1.0f;
			float nRatioBase = 0.0f;

			for (int32 rowi = 0; rowi < numRows; ++rowi)
			{
				float lTop = lBase + lDelta;
				float rTop = cosf(lTop);
				float zTop = sinf(lTop);
				float vTop = vBase + vDelta;
				float nzTop = sinf(lTop);
				float nRatioTop = cosf(lTop);

				float angle = 0.0f;

				for (int32 topi = 0; topi < numSegments; ++topi, angle += angleDelta)
				{
					float c = cosf(angle);
					float s = sinf(angle);

					verts.push_back(Vec3f(c * rTop, s * rTop, zTop));
					verts.push_back(Vec3f(c * rBase, s * rBase, zBase));
				}

				lBase = lTop;
				rBase = rTop;
				zBase = zTop;
				vBase = vTop;
				nzBase = nzTop;
				nRatioBase = nRatioTop;
			}

			uint32 size = verts.size();

			sphere->begin(size);
			sphere->setVertexArray(verts.getDataPointer());
			sphere->end();
			sphere->addIndexArray(TRIANGLE_STRIP, (uint32)0, size);

			verts.detachPointer();

			return sphere;
		}

		inline void bindGBufferTexture(GBuffer* gbuffer, ITextureUnitSet* lightPassTextures)
		{
			ITextureUnit* depth = lightPassTextures->createTextureUnit(0, "_depthTexture");
			depth->setTexture(gbuffer->depthTexture);

			ITextureUnit* normal_shininess = lightPassTextures->createTextureUnit(1, "_normal_shininessTexture");
			normal_shininess->setTexture(gbuffer->normal_shininessTexture);

			ITextureUnit* diffuse_specular = lightPassTextures->createTextureUnit(2, "_diffuse_specularTexture");
			diffuse_specular->setTexture(gbuffer->diffuse_specularTexture);
		}
	}
}

#endif