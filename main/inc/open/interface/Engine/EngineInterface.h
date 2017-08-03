#ifndef ENGINE_ENGINEINTERFACE_H
#define ENGINE_ENGINEINTERFACE_H

#include <functional>
#include <open/intrusivelist.h>

namespace open
{
	class IRenderWindow;
	class IInputQueue;
	struct Input;
	struct InputListener
	{
		typedef std::function<void(const Input*)> InputFunc;

		InputFunc guiListener;
		TLink<InputListener> link;
	};
	struct FrameListener
	{
		typedef std::function<void(void)> FrameListenerFunc;

		FrameListenerFunc preRender;
		FrameListenerFunc postRender;

		TLink<FrameListener> link;
	};

	class IRenderEngine;
	class COMMON_API IEngine
	{
	public:

		virtual ~IEngine() {}

		virtual void setRenderWindow(IRenderWindow* renderWindow) = 0;

		virtual IRenderWindow* getRenderWindow() = 0;

		virtual IRenderEngine* getRenderEngine() = 0;

		virtual void addFrameListener(FrameListener* listener) = 0;

		virtual void removeFrameListener(FrameListener* listener) = 0;

		virtual void addInputListener(InputListener* guiListener) = 0;

		virtual void removeInputListener(InputListener* guiListener) = 0;

		virtual void frame() = 0;

		virtual void run() = 0;
	};
}

#endif