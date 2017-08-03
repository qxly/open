#include "Engine.h"
#include "open/InputInterface.h"

#include <Render/RenderFactoryInterface.h>
#include <Render/RenderEngine.h>

#include <RenderSystem/RenderInterface.h>
#include <RenderSystem/RenderEnumeration.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace open
{
	Engine::Engine()
		:_init(false), _stop(false)
	{
		_render = RenderFactory::s_factory->getRenderEngine();
	}

	Engine::~Engine()
	{
		delete _render;
		delete _renderWindow;

		_frameListeners.clear();
		_guiEventListeners.clear();
	}

	void Engine::setRenderWindow(IRenderWindow* renderWindow)
	{
		_renderWindow = renderWindow;
		_render->getRenderPool()->setDefaultRenderTarget(_renderWindow);
	}

	void Engine::cull()
	{
	}

	void Engine::frame()
	{
		event();
		update();

		render();
	}

	void Engine::run()
	{
		while (!_stop)
		{
			frame();
		}
	}

	void Engine::update()
	{
		FrameListener* frameListener = _frameListeners.front();
		while (frameListener)
		{
			if(frameListener->preRender)
				frameListener->preRender();
			frameListener = _frameListeners.next(frameListener);
		}
	}

	void Engine::event()
	{
#ifdef _WIN32

		HWND handle = (HWND)_renderWindow->getWindowHandle();
		MSG msg;
		while (::PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		const Input* input = nullptr;
		InputListener* listener = nullptr;

		while (input = _renderWindow->getInputQueue()->nextEvent())
		{
			listener = _guiEventListeners.front();
			while (listener)
			{
				listener->guiListener(input);
				listener = _guiEventListeners.next(listener);
			}
			if (input->eventType == CLOSE)
			{
				_stop = true;
				break;
			}
		}
		_renderWindow->getInputQueue()->endEvent();
#else
#endif
	}

	void Engine::render()
	{
		cull();
		_render->getRenderPool()->applyRenderTarget(_renderWindow);

		_render->getRenderPool()->resetRenderState();
		_render->getRenderPool()->clear(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT | COLOR_BUFFER_BIT);

		_render->render();

		_renderWindow->swapBuffer();

		_render->getRenderPool()->garbageResource();
	}

	IEngine* createEngine() { return new Engine; }
}