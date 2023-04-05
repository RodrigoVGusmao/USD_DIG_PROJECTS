#include "graphics.h"

#include<exception>
#include<string>

#include "error.h"
#include "settings.h"

#ifndef GET_WINDOW_SIZE
#define GET_WINDOW_SIZE(lpRect, dwStyle, hasMenu) AdjustWindowRect((lpRect), (dwStyle), (hasMenu))
#endif

//Program
usGraphics::Program::Program()
{
	std::exception except(nullptr);

	mutex = CreateMutex(nullptr, TRUE, TEXT("direct2dL-IK"));
	if ((mutex == nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
		goto _GOTO_US_ERR_GENERIC_ALReADY_RUNNING;

	return;

_GOTO_US_ERR_GENERIC_ALReADY_RUNNING:
	if (except.what() == nullptr)
		except = std::exception(US_ERR_GENERIC_ALREADY_RUNNING_MSG);
	throw except;
}

usGraphics::Program::~Program()
{
	if (mutex != NULL)
		ReleaseMutex(mutex);
}

//Window
int usGraphics::Window::objectCount = 0;

usGraphics::Window::Window(HINSTANCE hInstance, int nCmdShow, WNDPROC wndProc, const TCHAR* windowTitle, int x, int y, int w, int h, int style, LPVOID extraParam)
{
	std::basic_string<TCHAR> className = TEXT("US_WC_") + std::to_tstring(objectCount);
	this->hInstance = hInstance;

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = reinterpret_cast<TCHAR*>(&className);

	std::exception except(nullptr);
	wndClassAtom = RegisterClass(&wndClass);
	if (wndClassAtom == 0)
		goto _GOTO_US_ERR_WINDOW_CLASS;

	windowSize = { 0, 0, w, h };
	if (!GET_WINDOW_SIZE(&windowSize, style, FALSE))
		goto _GOTO_US_ERR_WINDOW_ADJUST;

	windowHwnd = CreateWindow(MAKEINTRESOURCE(wndClassAtom), windowTitle, style,
		x, y, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, NULL, NULL, hInstance, extraParam);
	if (windowHwnd == NULL)
		goto _GOTO_US_ERR_WINDOW_CREATE;

	ShowWindow(windowHwnd, nCmdShow);

	++objectCount;
	return;

_GOTO_US_ERR_WINDOW_CREATE:
	if (except.what() == nullptr)
		except = std::exception(US_ERR_WINDOW_CREATE_MSG);

_GOTO_US_ERR_WINDOW_ADJUST:
	UnregisterClass(MAKEINTRESOURCE(wndClassAtom), hInstance);
	if (except.what() == nullptr)
		except = std::exception(US_ERR_WINDOW_ADJUST_MSG);

_GOTO_US_ERR_WINDOW_CLASS:
	if (except.what() == nullptr)
		except = std::exception(US_ERR_WINDOW_CLASS_MSG);

	throw except;
}

usGraphics::Window::~Window()
{
	if (IsWindow(windowHwnd)) {
		DestroyWindow(windowHwnd);
		MSG msg;
		while (GetMessage(&msg, windowHwnd, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(MAKEINTRESOURCE(wndClassAtom), hInstance);
}

bool usGraphics::Window::loop()
{
	if (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		return true;
	}

	return false;
}


//WindowD9
void initD3Dpp(D3DPRESENT_PARAMETERS* d3dpp, HWND wndHwnd, RECT windowSize, bool fullscreen);

usGraphics::WindowD9::WindowD9(HINSTANCE hInstance, int nCmdShow, bool fullscreen, WNDPROC wndProc, int w, int h, int x, int y, const TCHAR* windowTitle, int style, LPVOID extraParam) : 
	Window(hInstance, nCmdShow, wndProc, windowTitle, x, y, w, h, style, extraParam)
{
	std::exception except(nullptr);

	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == nullptr)
		goto _GOTO_US_ERR_DIRECTX9_CREATE_INTERFACE;

	initD3Dpp(&d3dpp, windowHwnd, windowSize, fullscreen);

	if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, windowHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device3d)))
		goto _GOTO_US_ERR_DIRECTX9_CREATE_DEVICE;

	return;

_GOTO_US_ERR_DIRECTX9_CREATE_DEVICE:
	direct3d->Release();
	if(except.what() == nullptr)
		except = std::exception(US_ERR_DIRECTX9_CREATE_DEVICE_MSG);

_GOTO_US_ERR_DIRECTX9_CREATE_INTERFACE:
	if (except.what() == nullptr)
		except = std::exception(US_ERR_DIRECTX9_CREATE_INTERFACE_MSG);

	throw except;
}

usGraphics::WindowD9::~WindowD9()
{
	releaseAll();
}

void usGraphics::WindowD9::resetDevice()
{
	initD3Dpp(&d3dpp, windowHwnd, windowSize, fullscreen);

	if (FAILED(device3d->Reset(&d3dpp)))
		throw std::exception(US_ERR_DIRECTX9_RESET_DEVICE_MSG);
}

void usGraphics::WindowD9::releaseAll()
{
	if (direct3d != nullptr)
		direct3d->Release();
	if (device3d != nullptr)
		device3d->Release();
}

bool usGraphics::WindowD9::loop()
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else
	{
		if (loopStart())
			return true;

		if (device3d == nullptr)
			throw std::exception(US_ERR_DIRECTX9_INVALID_DEVICE_MSG);
		if (FAILED(device3d->Clear(0, nullptr, D3DCLEAR_TARGET, backgroundColor, 0.f, 0))) 
			throw std::exception(US_ERR_DIRECTX9_CLEAR_BACKGROUND_MSG);
		if (FAILED(device3d->BeginScene()))
			throw std::exception(US_ERR_DIRECTX9_BEGIN_SCENE_MSG);

		if (render())
			return true;

		if (FAILED(device3d->EndScene()))
			throw std::exception(US_ERR_DIRECTX9_BEGIN_SCENE_MSG);
		if (FAILED(device3d->Present(nullptr, nullptr, nullptr, nullptr))) 
			throw std::exception(US_ERR_DIRECTX9_PAINT_MSG);

		loopEnd();
	}

	return true;
}

void usGraphics::WindowD9::handleLostGraphicsDevice()
{
	HRESULT result;
	if(device3d == nullptr)
		throw std::exception(US_ERR_DIRECTX9_INVALID_DEVICE_MSG);

	result = device3d->TestCooperativeLevel();
	if (FAILED(result))
	{
		if (result == D3DERR_DEVICELOST)
		{
			Sleep(100);
			return;
		}
		else if (result == D3DERR_DEVICENOTRESET)
		{
			//releaseDevice();
			resetDevice();
		}
		else
		{
			throw std::exception(US_ERR_DIRECTX9_UNKNOW_MSG);
		}
	}
}

void initD3Dpp(D3DPRESENT_PARAMETERS* d3dpp, HWND wndHwnd, RECT windowSize, bool fullscreen)
{
	d3dpp->BackBufferWidth = windowSize.right - windowSize.left;
	d3dpp->BackBufferHeight = windowSize.bottom - windowSize.top;
	if (fullscreen)
		d3dpp->BackBufferFormat = D3DFMT_X8R8G8B8;
	else
		d3dpp->BackBufferFormat = D3DFMT_UNKNOWN;

	d3dpp->BackBufferCount = 1;

	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp->hDeviceWindow = wndHwnd;
	d3dpp->Windowed = (!fullscreen);

	d3dpp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}