#include <open/interface/RenderSystem/RenderSystemFactoryInterface.h>
#include <open/interface/Render/RenderFactoryInterface.h>
#include <open/interface/Engine/EngineFactoryInterface.h>
#include <open/interface/Engine/EngineInterface.h>

namespace open
{
	RenderSystemFactory* RenderSystemFactory::s_factory = nullptr;
	RenderFactory* RenderFactory::s_factory = nullptr;
	EngineFactory* EngineFactory::s_factory = nullptr;
}