/*#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>
#include<crtdbg.h>
#endif*/

#define WIN32_LEAN_AND_MEAN

#include<windows.h>
#include<d3d9.h>
#include<string>
#include<sstream>

#include<exception>

#include"mainWindow.h"
#include"settings.h"
#include"game.h"

class DirectTest : public usGame::Game
{
private:
	//usGraphics::FrameRate FPS(true, 30.f);

	bool render() { return false; }

	bool loopStart()
	{
		return !(calcFrameRate());
	}
	void loopEnd() { return; }
public:
	using usGame::Game::Game;
};

class Monitor : public usGraphics::Window
{
public:
	using usGraphics::Window::Window;

	bool loop()
	{
		PeekMessage(&msg, windowHwnd, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		return true;
	}
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow)
{
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	try {
		usGraphics::Program Program;

		DirectTest mainWindow(hInstance, nCmdShow, false, mainWindowProc, windowWidth, windowHeight, windowPosX, windowPosY);
		mainWindow.setBkColor(D3DCOLOR_XRGB(0xFF, 0, 0));
		mainWindow.limitFrameRate(true, 200.f, 0.f);
		mainWindow.linkInput();

		monitorData FPS;
		Monitor monitor(hInstance, nCmdShow, monitorProc, TEXT("monitor"), CW_USEDEFAULT, CW_DEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, WS_OVERLAPPEDWINDOW, reinterpret_cast<LPVOID>(&FPS));

		int intances = mainWindow.getObjCount();
		while (intances > 0)
		{
			try {
				FPS.FPS = mainWindow.getFPS();
				FPS.effectiveFPS = mainWindow.getEffectiveFPS();
				if (mainWindow.loop() == false)
					--intances;
			}
			catch (std::exception e)
			{
				mainWindow.handleLostGraphicsDevice();
			}
		}
	}
	catch (std::exception e)
	{
		MessageBoxA(nullptr, e.what(), "Error!", MB_ICONERROR | MB_OK);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "Unknown error!", "Error!", MB_ICONERROR | MB_OK);
	}
	return 0;
}