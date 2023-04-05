#ifndef _UNICODE
#define _UNICODE
#endif

#include<windows.h>
#include<dllmain.h>

#include"framework.h"
#include"TheDungeon.h"
#include"mainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR cmdLine, int cmdShow)
{
	WNDCLASS wndClass;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	wndClass.lpfnWndProc = mainWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = L"main";
	RegisterClassW(&wndClass);

	HWND mainWindow = CreateWindowExW(0, L"main", L"The Dungeon", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(mainWindow, cmdShow);
	UpdateWindow(mainWindow);

	//HACCEL accelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_THEDUNGEON));

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		//if (!TranslateAcceleratorW(mainWindow, accelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return msg.wParam;
}