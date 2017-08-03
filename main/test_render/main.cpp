#include "common.h"
#include "Manipulator.h"
#include <open/Module.h>
#include <open/Config.h>

#include <Engine/EngineFactoryInterface.h>
#include <RenderSystem/BufferInterface.h>

using namespace open;

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
	emissiveTexture = RenderSystemFactory::s_factory->createTexture("./data/Koala.jpg");
	diffuseTexture = RenderSystemFactory::s_factory->createTexture("./data/0_2.jpg");
	diffuseTransparency = RenderSystemFactory::s_factory->createTexture("./data/0_02222.png");
	emissiveTransparency = RenderSystemFactory::s_factory->createTexture("./data/lobster.png");
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


void genObj()
{
	Config config;
	config.setProperty("Path", "DataPath", ".data/mop/");
	IDrawable* drawable = RenderFactory::s_factory->createDrawable("./data/mop/mop.obj", config);

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			RMatrix matrix;
			matrix.makeTranslate(Real3(500, 500, 100) + Real3(i * 3, j * 3, 15));

			RenderFactory::s_factory->getRenderEngine()->addRender(drawable, matrix);
		}
	}
}

IDrawable* g_terrain = nullptr;
void createObj()
{
	/*Real x = 500;
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
	}*/

	genObj();
}

void init(IRenderPool* renderPool)
{
	///light_dir
	Real3 lightDir(-1, -2, -3);
	lightDir.normalize();
	Real3 lightPoint(600, 600, 200);

	ILight* light = renderPool->getLightManager()->createLight();
	renderPool->getLightManager()->addLight(light);
	renderPool->getLightManager()->setMainLight(light);

	light->setAmbientColor(Real3(0.08, 0.08, 0.08));
	light->setDiffuseColor(Real3(0.8, 0.8, 0.8));
	light->setLightPosition(lightPoint);
	light->setLightDirection(lightDir);
	light->setLightRange(1, 500);

	genTextures();

	g_terrain = createTerrain(Real3(), Real3(1000, 1000, 200));
	RenderFactory::s_factory->getRenderEngine()->addRender(g_terrain, RMatrix());

	IDrawable* coordAxia = createCoords();
	RenderFactory::s_factory->getRenderEngine()->addRender(coordAxia, RMatrix());

	Manipulator* manipulator = new Manipulator;
	manipulator->setCamera(RenderFactory::s_factory->getRenderEngine()->getCamera());
	manipulator->setTerrain(g_terrain);
	EngineFactory::s_factory->getEngine()->addInputListener(manipulator);

	DemoManager::getSingletonPtr()->build(RenderFactory::s_factory->getRenderEngine()->getCamera());

	createObj();
}

void main(int argc, char *argv[])
{
	ModuleManager::s_manager->loadModule("RenderSystem.dll");
	ModuleManager::s_manager->loadModule("Render.dll");
	ModuleManager::s_manager->loadModule("Engine.dll");

	WindowTraits wt;
	wt.x = 0;
	wt.y = 0;
	wt.width = 800;
	wt.height = 600;

	IRenderWindow* renderWindow = RenderSystemFactory::s_factory->getRenderWindowFactory()->createRenderWindow();
	bool success = renderWindow->setupWindow(wt);
	if (!success)
	{
		getchar();
		return;
	}

	RenderFactory::s_factory->getRenderEngine()->getRenderPool()->setClearColor(Vec4f(0, 0, 0, 0));

	IEngine* engine = EngineFactory::s_factory->getEngine();
	engine->setRenderWindow(renderWindow);

	init(engine->getRenderEngine()->getRenderPool());

	engine->run();

	int64 objCount = PRINT_ERROR_MEMORY_OBJECT_COUNT();
	if (objCount != 0)
		getchar();
}