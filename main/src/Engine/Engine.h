#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <open/Memory.h>
#include <open/intrusivelist.h>
#include <Engine/EngineInterface.h>

namespace open
{
	class Engine : public IEngine PUBLIC_OBJECT_DEBUG
	{
	public:

		Engine();

		~Engine();

		virtual void setRenderWindow(IRenderWindow* renderWindow);

		virtual IRenderWindow* getRenderWindow() { return _renderWindow; }

		virtual IRenderEngine* getRenderEngine() { return _render; }

		virtual void addFrameListener(FrameListener* listener) { _frameListeners.push_back(listener); }

		virtual void removeFrameListener(FrameListener* listener) { _frameListeners.erase(listener); }

		virtual void addInputListener(InputListener* guiListener) { _guiEventListeners.push_back(guiListener); }

		virtual void removeInputListener(InputListener* guiListener) { _guiEventListeners.erase(guiListener); }

		virtual void frame();

		virtual void run();

	private:

		void update();

		void event();

		void render();

		void cull();

		void init();

	private:
		IRenderWindow* _renderWindow;
		IRenderEngine* _render;

		INTRUSIVELIST_DECLARE(FrameListener, link) _frameListeners;

		INTRUSIVELIST_DECLARE(InputListener, link) _guiEventListeners;

		bool _stop;
		bool _init;
	};
}

#endif