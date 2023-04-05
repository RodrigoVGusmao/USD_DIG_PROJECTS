#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#include"mainWindowProc.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdLine, int nCmd)
{

	WNDCLASS wndClass = { CS_HREDRAW | CS_VREDRAW,
		mainWindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
		NULL,
		TEXT("mainWindow")
	};
	ATOM wndAtom = RegisterClass(&wndClass);

	HWND mainWindow = CreateWindow(MAKEINTATOM(wndAtom), L"Text editor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(mainWindow, SW_MAXIMIZE);

	MSG msg;
	while (1)
	{
		int ret = GetMessage(&msg, NULL, 0, 0);
		if (ret < 0)
		{
			return 1;
		}
		if (ret == 0)
		{
			break;
		}
		if (ret > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}