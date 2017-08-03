#ifndef RENDERSYSTEM_RENDERWINDOW_H
#define RENDERSYSTEM_RENDERWINDOW_H

#include <string>
#include <vector>

#include <RenderSystem/RenderInterface.h>
#include <open/Memory.h>

#ifdef _WIN32
#include <windows.h>

namespace open
{
	class RenderWindow_Win32 : public IRenderWindow PUBLIC_OBJECT_DEBUG
	{
	public:

		RenderWindow_Win32();

		~RenderWindow_Win32();

		virtual bool setupWindow(const WindowTraits& traits);

		virtual void apply();

		virtual void swapBuffer();

		virtual IInputQueue* getInputQueue();

		virtual void* getWindowHandle() { return (void*)_hWnd; }

		virtual int32 getWidth() { return _windowRect.right - _windowRect.left; }

		virtual int32 getHeight() { return _windowRect.bottom - _windowRect.top; }

	public:
		RECT         _windowRect;
	private:
		bool killWindow();

	private:
		HWND         _hWnd;
		HGLRC        _hRC;
		HDC          _hDC;
		HINSTANCE    _hInstance;
		WNDCLASS     _windClass;

		IInputQueue* _input;
	};

	static IRenderWindow* win_renderWindowCreator()
	{
		return new RenderWindow_Win32();
	}

	class RenderWindowFactory : public IRenderWindowFactory
	{
		struct Name_Creator
		{
			std::string name;
			RenderWindowCreator creator;

			Name_Creator()
			{
			}

			Name_Creator(const char* n, const RenderWindowCreator& r)
				:creator(r), name(n)
			{
			}
		};

	public:

		RenderWindowFactory();

		virtual IRenderWindow* createRenderWindow(const char* windowType);

		virtual void registeRenderWindowCreator(const char* windowType, RenderWindowCreator creator);

		~RenderWindowFactory();

	private:
		std::vector<Name_Creator> _lists;
	};
}
#else

#endif

#endif