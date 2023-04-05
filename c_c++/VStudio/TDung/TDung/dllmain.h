#pragma once

extern "C" _declspec (dllexport) int TDSetCount(int newCount);
extern "C" _declspec (dllexport) int TDGetCount();

//image functions
extern "C" _declspec (dllexport) HBITMAP GetColorMask(HWND hwnd, HBITMAP image, COLORREF colorKey);

extern "C" _declspec (dllexport) HDC ApplyTransparency(HDC dst, int x, int y, int w, int h, int x2, int y2, int w2, int h2, HBITMAP image, HBITMAP mask);