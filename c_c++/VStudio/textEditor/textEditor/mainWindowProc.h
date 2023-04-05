#pragma once
#define WIN32_LEAN_AND_MEAN
#include<windows.h>

RECT translateRect(HWND hwnd, RECT offset);

void paintLines(HDC dc, HWND textHWnd, int* currentMargin, BOOL defaultColor, COLORREF textColor, COLORREF bkgrnd);

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);