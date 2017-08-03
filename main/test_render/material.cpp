
#include "common.h"
#include <vector>
#include <Engine/EngineFactoryInterface.h>

namespace open {

	struct MaterialDemo : public Demo
	{
		enum TestMaterialType
		{
			USE_DIFFUSECOLOR = 1 << 0,
			USE_DIFFUSETEXTURE = 1 << 1,

			USE_EMISSIVECOLOR = 1 << 2,
			USE_EMISSIVETEXTURE = 1 << 3,

			USE_TRANSPARENCY = 1 << 4,
		};

		ITexture* emissiveTexture;
		ITexture* diffuseTexture;
		ITexture* emissiveTransparency;
		ITexture* diffuseTransparency;
		void genTextures() {
			emissiveTexture = RenderSystemFactory::s_factory->createTexture("e:/data/Koala.jpg");
			diffuseTexture = RenderSystemFactory::s_factory->createTexture("e:/data/0_2.jpg");
			diffuseTransparency = RenderSystemFactory::s_factory->createTexture("e:/data/0_02222.png");
			emissiveTransparency = RenderSystemFactory::s_factory->createTexture("e:/data/lobster.png");
		}

		IMaterial* genTestMaterial(int materialType) {

			IMaterial* material = RenderFactory::s_factory->createMaterial();

			if (materialType & USE_DIFFUSECOLOR) {
				if (materialType & USE_TRANSPARENCY) {
					material->getRenderStaeSet().setDiffuseColor(Real3(0, 1, 1).asRGB());
				}
				else
					material->getRenderStaeSet().setDiffuseColor(Real3(0, 0, 1).asRGB());
			}

			if (materialType & USE_DIFFUSETEXTURE) {
				if (materialType & USE_TRANSPARENCY) {
					material->getTextureUnitSet()->createTextureUnit(0)->setTexture(diffuseTransparency);
				}
				else {
					material->getTextureUnitSet()->createTextureUnit(0)->setTexture(diffuseTexture);
				}
			}

			if (materialType & USE_EMISSIVECOLOR) {
				if (materialType & USE_TRANSPARENCY) {
					material->getRenderStaeSet().setEmissiveColor(Real3(1, 1, 0).asRGB());
				}
				else {
					material->getRenderStaeSet().setEmissiveColor(Real3(0, 1, 0).asRGB());
				}
			}

			if (materialType & USE_EMISSIVETEXTURE) {
				if (materialType & USE_TRANSPARENCY) {
					material->getTextureUnitSet()->createTextureUnit(1)->setTexture(emissiveTransparency);
				}
				else {
					material->getTextureUnitSet()->createTextureUnit(1)->setTexture(emissiveTexture);
				}
			}

			if (materialType & USE_TRANSPARENCY) {
				material->getRenderStaeSet().setMode(BLEND, true);
			}

			return material;
		}

		IGeometry* createSphere(Real x, Real y, Real z, Real r) {
			SolidShape shape;
			shape.type = SolidShape::TYPE_SPHERE;

			shape.shape.sphere.radius = r;
			shape.shape.sphere.center[0] = x;
			shape.shape.sphere.center[1] = y;
			shape.shape.sphere.center[2] = z;

			ISolidGeometry* geo = RenderFactory::s_factory->createSolidGeometry();
			geo->setSoildShape(shape);

			return geo;
		};

		IDrawable* genTestDrawable(Real x, Real y, Real z, Real r, int materialType) {
			IGeometry* geo = createSphere(x, y, z, r);
			IMaterial* material = genTestMaterial(materialType);

			IDrawable* drawable = RenderFactory::s_factory->createDrawable();
			drawable->asDrawableBase()->addGeometry(geo, material);

			return drawable;
		}
		
		virtual void createNode(ICamera* camera)
		{
			genTextures();

			Real x = 500;
			Real y = 500;
			Real z = 100;
			Real r = 10;
			Real d = 2.5;

			for (int i = 0; i < 4; i++) {
				Real xf = x + r * i * d;
				int32 mask = (1 << i);
				mask |= USE_TRANSPARENCY;
				RenderFactory::s_factory->getRenderEngine()->addRender(genTestDrawable(xf, y, z, r, mask), RMatrix());
			}

			y = 600;
			for (int i = 0; i < 4; i++) {
				Real xf = x + r * i * d;
				int32 mask = (1 << i);
				RenderFactory::s_factory->getRenderEngine()->addRender(genTestDrawable(xf, y, z, r, mask), RMatrix());
			}
		}
	};

	DECLARE_DEMO(MaterialDemo);
}
