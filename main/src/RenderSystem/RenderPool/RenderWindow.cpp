#include "RenderWindow.h"
#include <windowsx.h>
#include "../GlState.h"
#include <open/InputInterface.h>

using namespace open::gl;

#ifdef _WIN32
#include <glew/glew.h>
#include <glew/wglew.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>

namespace open
{
	static RenderWindow_Win32* g_renderWindow;
	LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RenderWindow_Win32* renderWindow = g_renderWindow;
		if (!renderWindow)
			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		{
			int32 mx = GET_X_LPARAM(lParam);
			int32 my = GET_Y_LPARAM(lParam);
			renderWindow->getInputQueue()->mouseMotion(mx, my);
		}
		break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			::SetCapture(hwnd);

			int button;

			if (uMsg == WM_LBUTTONDOWN)
				button = 1;
			else if (uMsg == WM_MBUTTONDOWN)
				button = 2;
			else
				button = 3;

			int32 mx = GET_X_LPARAM(lParam);
			int32 my = GET_Y_LPARAM(lParam);
			renderWindow->getInputQueue()->mouseButtonPress(mx, my, button);
		}
		break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			::ReleaseCapture();

			int button;

			if (uMsg == WM_LBUTTONUP)
				button = 1;
			else if (uMsg == WM_MBUTTONUP)
				button = 2;
			else
				button = 3;

			int32 mx = GET_X_LPARAM(lParam);
			int32 my = GET_Y_LPARAM(lParam);
			renderWindow->getInputQueue()->mouseButtonRelease(mx, my, button);
		}
		break;
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		{
			::SetCapture(hwnd);

			int button;

			if (uMsg == WM_LBUTTONDBLCLK)
				button = 1;
			else if (uMsg == WM_MBUTTONDBLCLK)
				button = 2;
			else
				button = 3;

			int32 mx = GET_X_LPARAM(lParam);
			int32 my = GET_Y_LPARAM(lParam);
			renderWindow->getInputQueue()->mouseDoubleButtonPress(mx, my, button);
		}
		break;
		case WM_MOUSEWHEEL:
			break;

		case WM_MOVE:
		case WM_SIZE:
		{
			::GetClientRect(hwnd, &(renderWindow->_windowRect));
			renderWindow->getInputQueue()->changeWindow(0, 0, renderWindow->_windowRect.right - renderWindow->_windowRect.left, renderWindow->_windowRect.bottom - renderWindow->_windowRect.top);
		}
		break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			unsigned int key = (unsigned int)wParam;
			renderWindow->getInputQueue()->keyPress(key);
		}
		break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			unsigned int key = (unsigned int)wParam;
			renderWindow->getInputQueue()->keyRelease(key);
		}
		break;

		case WM_SETFOCUS:
			for (unsigned int i = 0x08; i < 0xFF; i++)
			{
				if ((::GetAsyncKeyState(i) & 0x8000) != 0)
					::SendMessage(hwnd, WM_KEYDOWN, i, 0);
			}
			break;

		case WM_CLOSE:
			renderWindow->getInputQueue()->closeWindow();
			break;
		case WM_DESTROY:
			break;

		case WM_QUIT:
			break;
		}
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	RenderWindow_Win32::RenderWindow_Win32()
	{
		memset(&_windClass, 0, sizeof(WNDCLASS));
		_hWnd = 0;
		_hRC = 0;
		_hDC = 0;
		_hInstance = 0;
		g_renderWindow = this;
		_input = createInputQueue();
	}

	IInputQueue* RenderWindow_Win32::getInputQueue()
	{
		return _input;
	}

	void RenderWindow_Win32::apply()
	{
		uint32 default_wind[] = { GL_BACK_LEFT };

		gl_BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		gl_DrawBuffers(1, default_wind);
	}

	void RenderWindow_Win32::swapBuffer()
	{
		SwapBuffers(_hDC);
	}

	bool RenderWindow_Win32::setupWindow(const WindowTraits& traits)
	{
		int32 x = traits.x;
		int32 y = traits.y;
		int32 width = traits.width;
		int32 height = traits.height;

		bool bRetVal = true;

		int32 nWindowX = x;
		int32 nWindowY = y;
		int32 nPixelFormat = -1;
		PIXELFORMATDESCRIPTOR pfd;

		DWORD dwExtStyle;
		DWORD dwWindStyle;

		HINSTANCE _hInstance = GetModuleHandle(nullptr);

		static int32 randName = 1;

		char szWindowName[64] = { 0 };
		char szClassName[64] = { 0 };
		sprintf(szWindowName, "%d_windowName", randName);
		sprintf(szClassName, "%d_className", randName);

		randName++;

		// setup window class
		_windClass.lpszClassName = szClassName;
		_windClass.lpfnWndProc = (WNDPROC)wndProc;
		_windClass.hInstance = _hInstance;
		_windClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		_windClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		_windClass.hbrBackground = nullptr;
		_windClass.lpszMenuName = nullptr;
		_windClass.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW | CS_DBLCLKS;
		_windClass.cbClsExtra = 0;
		_windClass.cbWndExtra = 0;

		if (!RegisterClass(&_windClass))
			bRetVal = false;

		dwExtStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwWindStyle = WS_OVERLAPPEDWINDOW;
		ShowCursor(TRUE);

		_windowRect.left = nWindowX;
		_windowRect.right = nWindowX + width;
		_windowRect.top = nWindowY;
		_windowRect.bottom = nWindowY + height;

		AdjustWindowRectEx(&_windowRect, dwWindStyle, FALSE, dwExtStyle);

		int32 nWindowWidth = _windowRect.right - _windowRect.left;
		int32 nWindowHeight = _windowRect.bottom - _windowRect.top;

		_hWnd = CreateWindowEx(dwExtStyle,     // Extended style
			szClassName,    // class name
			szWindowName,   // window name
			dwWindStyle |
			WS_CLIPSIBLINGS |
			WS_CLIPCHILDREN,// window stlye
			nWindowX,       // window position, x
			nWindowY,       // window position, y
			nWindowWidth,   // height
			nWindowHeight,  // width
			nullptr,           // Parent window
			nullptr,           // menu
			_hInstance,    // getSingletonPtr
			nullptr);          // material this to WM_CREATE


		_hDC = GetDC(_hWnd);
		SetPixelFormat(_hDC, 1, &pfd);
		_hRC = wglCreateContext(_hDC);
		wglMakeCurrent(_hDC, _hRC);

		if (_hDC == 0 ||
			_hDC == 0)
		{
			bRetVal = false;
			printf("!!! An error occured creating an OpenGL window.\n");
		}
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			bRetVal = false;
			printf("Error: %s\n", glewGetErrorString(err));
		}
		const GLubyte *oglVersion = glGetString(GL_VERSION);
		printf("This system supports OpenGL Version %s.\n", oglVersion);

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(_hRC);
		ReleaseDC(_hWnd, _hDC);
		DestroyWindow(_hWnd);

		_hWnd = CreateWindowEx(dwExtStyle, szClassName, szWindowName, dwWindStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, nWindowX, nWindowY, nWindowWidth, nWindowHeight,
			nullptr, nullptr, _hInstance, nullptr);

		_hDC = GetDC(_hWnd);

		int32 nPixCount = 0;
		int32 pixAttribs[] =
		{
			WGL_SUPPORT_OPENGL_ARB, 1, // Must support OGL rendering  
			WGL_DRAW_TO_WINDOW_ARB, 1, // pf that can run a window  
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, // must be HW accelerated  
			WGL_COLOR_BITS_ARB, 32, // 8 bits of each R, G and B  
			WGL_DEPTH_BITS_ARB, 24, // 24 bits of depth precision for window, 典型情况下深度缓冲区都是24位的,试了几台机器都不支持32位深度缓冲区  
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE, // Double buffered context  
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // pf should be RGBA type  
			WGL_STENCIL_BITS_ARB, 8,//开启模板缓冲区,模板缓冲区位数=8  
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // MSAA on,开启多重采样  
			WGL_SAMPLES_ARB, 4, // 4x MSAA ,多重采样样本数量为4  
			0
		};
		wglChoosePixelFormatARB(_hDC, &pixAttribs[0], nullptr, 1, &nPixelFormat, (UINT*)&nPixCount);

		if (nPixelFormat == -1)
		{
			_hDC = 0;
			_hDC = 0;
			bRetVal = false;
			printf("!!! An error occurred trying to find a pixel format with the requested attribs.\n");
		}
		else
		{
			SetPixelFormat(_hDC, nPixelFormat, &pfd);

			GLint attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				0 };

			_hRC = wglCreateContextAttribsARB(_hDC, 0, attribs);
			ASSERT_(_hRC);
			//if (_hRC == nullptr)
			//{
			//	printf("!!! Could not create an OpenGL 3.3 context.\n");
			//	attribs[3] = 2;
			//	_hRC = wglCreateContextAttribsARB(_hDC, 0, attribs);
			//	if (_hRC == nullptr)
			//	{
			//		printf("!!! Could not create an OpenGL 3.2 context.\n");
			//		attribs[3] = 0;
			//		_hRC = wglCreateContextAttribsARB(_hDC, 0, attribs);
			//		if (_hRC == nullptr)
			//		{
			//			printf("!!! Could not create an OpenGL 3.0 context.\n");
			//			/*attribs[3] = 3;
			//			attribs[1] = 3;
			//			_hRC = wglCreateContextAttribsARB(_hDC, 0, attribs);
			//			if (_hRC == nullptr)
			//			{
			//			printf("!!! Could not create an OpenGL 3.3 context.\n");
			//			printf("!!! OpenGL 3.3 and higher are not supported on this system.\n");
			//			}*/
			//		}
			//	}
			//}
			wglMakeCurrent(_hDC, _hRC);
		}

		if (_hDC == 0 ||
			_hRC == 0)
		{
			bRetVal = false;
			printf("!!! An error occured creating an OpenGL window.\n");
		}
		if (bRetVal)
		{
			ShowWindow(_hWnd, SW_SHOW);
			SetForegroundWindow(_hWnd);
			SetFocus(_hWnd);
		}
		else
			killWindow();
		return bRetVal;
	}

	bool RenderWindow_Win32::killWindow()
	{
		if (_input)
		{
			delete _input;
			_input = nullptr;
		}

		bool bRetVal = true;
		if (_hRC)
		{
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(_hRC);
			_hRC = nullptr;
		}
		if (_hDC)
		{
			ReleaseDC(_hWnd, _hDC);
			_hDC = nullptr;
		}
		if (_hWnd)
		{
			DestroyWindow(_hWnd);
			_hWnd = nullptr;;
		}
		UnregisterClass(_windClass.lpszClassName, _hInstance);

		_hInstance = nullptr;
		ShowCursor(TRUE);
		return bRetVal;
	}

	RenderWindow_Win32::~RenderWindow_Win32()
	{
		if (!_hInstance)
			return;

		UnregisterClass(_windClass.lpszClassName, _hInstance);
		killWindow();

		delete _input;
	}

	RenderWindowFactory::RenderWindowFactory()
	{
		registeRenderWindowCreator("win32", RenderWindowCreator(&win_renderWindowCreator));
	}

	IRenderWindow* RenderWindowFactory::createRenderWindow(const char* windowType)
	{
		std::vector<Name_Creator>::iterator it = _lists.begin();
		while (it != _lists.end())
		{
			if (it->name == windowType)
				return it->creator();
			++it;
		}
		return nullptr;
	}

	void RenderWindowFactory::registeRenderWindowCreator(const char* windowType, RenderWindowCreator creator)
	{
		_lists.push_back(Name_Creator(windowType, creator));
	}

	RenderWindowFactory::~RenderWindowFactory()
	{
		_lists.clear();
	}
}

#else
#endif