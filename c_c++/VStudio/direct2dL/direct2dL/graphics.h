#pragma once
#define WIN32_LEAN_AND_MEAN

#include<windows.h>
#include<d3d9.h>
//#include<d3dx9.h>

#define US_WM_INPUT 0x7FFF

#ifdef UNICODE
#define to_tstring(a) to_wstring(a)
#else
#define to_tstring(a) to_string(a)
#endif // UNICODE

namespace usGraphics
{
	//classes
	class Program
	{
	private:
		HANDLE mutex;

	public:
		Program();
		~Program();
	};

	class Window
	{
	private:
		static int objectCount;

	protected:
		HINSTANCE hInstance;
		ATOM wndClassAtom = 0;
		HWND windowHwnd = nullptr;
		RECT windowSize;
		MSG msg = { 0 };

	public:
		Window(HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(0x400000), int nCmdShow = SW_SHOWDEFAULT, WNDPROC wndProc = DefWindowProc, const TCHAR* windowTitle = TEXT("windows application"),
			int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int w = CW_USEDEFAULT, int h = CW_USEDEFAULT, int style = WS_OVERLAPPEDWINDOW, LPVOID extraParam = NULL);
		virtual ~Window();

		virtual bool loop();

		HWND getHWND()
		{
			return windowHwnd;
		}

		ATOM getATOM()
		{
			return wndClassAtom;
		}

		RECT getWindowSize()
		{
			return windowSize;
		}

		int getObjCount() { return objectCount; };
	};

	class WindowD9 : public Window
	{
	protected:
		IDirect3D9* direct3d = nullptr;
		IDirect3DDevice9* device3d = nullptr;
		//D3DXSPRITE* sprite = nullptr;
		D3DPRESENT_PARAMETERS d3dpp = {};
		D3DCOLOR backgroundColor = 0;
		bool fullscreen;

		void resetDevice();
		void releaseAll();

		//virtual functions
		virtual bool render() = 0;
		virtual bool loopStart() = 0;
		virtual void loopEnd() = 0;

	public:
		WindowD9(HINSTANCE hInstance, int nCmdShow, bool fullscreen, WNDPROC wndProc, int w, int h, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
			const TCHAR* windowTitle = TEXT("windows application"), int style = WS_OVERLAPPEDWINDOW, LPVOID extraParam = NULL);
		virtual ~WindowD9();

		bool loop();

		void handleLostGraphicsDevice();

		D3DCOLOR getBkColor()
		{
			return backgroundColor;
		}

		void setBkColor(D3DCOLOR color)
		{
			backgroundColor = color;
		}
	};
}