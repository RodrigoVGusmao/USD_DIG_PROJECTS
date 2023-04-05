// engine.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "engine.h"
#include "mainWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSW wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = mainWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
	wndClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = L"mainWnd";

	HWND mainWnd = CreateWindowW(MAKEINTATOM(RegisterClassW(&wndClass)), L"Engine Test", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, 0x340, 0x340,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(mainWnd, nCmdShow);
	UpdateWindow(mainWnd);

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	msg.wParam;
}
