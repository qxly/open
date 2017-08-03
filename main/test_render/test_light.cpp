
#include "common.h"
#include <vector>
#include <Engine/EngineFactoryInterface.h>

namespace open {

	Real random(Real min, Real max)
	{
		return min + (max - min) * rand() / (Real)RAND_MAX;
	}

	struct Light_Sphere_Move
	{
		ILight* light;

		Real angleX;
		Real angleY;
		Real angleZ;

		Real3 center;
		Real radius;

		Light_Sphere_Move(IDrawable* drawable)
			:angleX(0), angleY(0), angleZ(0), radius(15)
		{
			IRenderPool* renderPool = EngineFactory::s_factory->getEngine()->getRenderEngine()->getRenderPool();

			light = renderPool->getLightManager()->createLight();
			light->setLightRange(0.1, radius);
			light->setLightType(LightType::LIGHT_POINT);

			light->setAmbientColor(Real3(random(0, 1), random(0, 1), random(0, 1)));
			light->setDiffuseColor(Real3(random(0, 1), random(0, 1), random(0, 1)));

			renderPool->getLightManager()->addLight(light);
		}

		void setCenter(Real3 c)
		{
			center = c;
		}

		void move()
		{
			Real a = 3.14159265 / 90.0;

			angleX += random(0, a);
			angleY += random(0, a);
			angleZ += random(0, a);

			RMatrix quat;
			quat.makeRotateXYZ(angleX, angleY, angleZ);

			RMatrix scale;
			scale.makeScale(radius, radius, radius);

			Real3 p(1, 0, 0);
			p = scale * quat * p + center;
			light->setLightPosition(p);
		}
	};

	struct LightUpdate : public FrameListener
	{
		std::vector<Light_Sphere_Move*> lights;
		Ptr<IDrawableBase> sphereDrawable;

		LightUpdate()
		{
			initSphere();

			for (int32 i = 0; i < 50; i++)
			{
				Light_Sphere_Move* lsm = new Light_Sphere_Move(sphereDrawable);
				Real3 lightPoint(i * 20, 500 + i * 20, 100);

				lsm->setCenter(lightPoint);

				lights.push_back(lsm);

				RMatrix transe;
				transe.makeTranslate(lightPoint);

				RenderFactory::s_factory->getRenderEngine()->addRender(sphereDrawable, transe);
			}

			preRender = [&]() {
				for (auto lsm : lights)
				{
					lsm->move();
				}
			};

			EngineFactory::s_factory->getEngine()->addFrameListener(this);
		}

		~LightUpdate()
		{
			for (auto lsm : lights)
			{
				delete lsm;
			}
		}

		void initSphere()
		{
			SolidShape shape;
			shape.type = SolidShape::TYPE_SPHERE;

			shape.shape.sphere.radius = 10;
			shape.shape.sphere.center[0] = 0;
			shape.shape.sphere.center[1] = 0;
			shape.shape.sphere.center[2] = 0;

			sphereDrawable = RenderFactory::s_factory->createDrawable();
			ISolidGeometry* sd = RenderFactory::s_factory->createSolidGeometry();
			sd->setSoildShape(shape);

			auto material = RenderFactory::s_factory->getOrCreateMaterial("Tulips_Diffuse");
			{
				ITexture* texture = RenderSystemFactory::s_factory->createTexture("./data/xiong.jpg");
				ITextureUnitSet* tus = material->getTextureUnitSet();
				ITextureUnit* tu = tus->createTextureUnit(0);
				tu->setTexture(texture);
			}
			sphereDrawable->addGeometry(sd, material);

		}
	};

	class LightDemo : public Demo
	{
	public:
		LightUpdate* _lightUpdate;
		virtual void createNode(ICamera* camera)
		{
			_lightUpdate = new LightUpdate();
		}

		~LightDemo()
		{
			delete _lightUpdate;
		}
	};
	DECLARE_DEMO(LightDemo);
}