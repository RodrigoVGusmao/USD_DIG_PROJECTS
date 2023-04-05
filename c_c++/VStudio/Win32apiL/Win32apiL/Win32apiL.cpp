#ifndef _UNICODE
#define _UNICODE
#endif

#include<windows.h>

#include<cinttypes>
#include<vector>

#include"DrawArea.h"
#include"ToolArea.h"
#include"MainWindow.h"
#include"debug.h"
#include"resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR argv, int cmdShow)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = wndProc;
	wndClass.lpszClassName = L"Window";
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wndClass);

	wndClass.lpfnWndProc = drawAreaProc;
	wndClass.lpszClassName = L"drawArea";
	wndClass.lpszMenuName = nullptr;
	wndClass.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
	wndClass.hIcon = nullptr;
	wndClass.hIconSm = nullptr;
	RegisterClassEx(&wndClass);

	wndClass.lpfnWndProc = toolWndProc;
	wndClass.lpszClassName = L"toolArea";
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	RegisterClassEx(&wndClass);

	wndClass.lpfnWndProc = colorWndProc;
	wndClass.lpszClassName = L"colorArea";
	RegisterClassEx(&wndClass);

	wndClass.lpfnWndProc = toolPropertiesWndProc;
	wndClass.lpszClassName = L"propertiesArea";
	RegisterClassEx(&wndClass);

	HWND mainWnd = CreateWindowEx(0, L"Window", L"winDraw", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(mainWnd, cmdShow);
	UpdateWindow(mainWnd);

	HACCEL accelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(mainWnd, accelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
