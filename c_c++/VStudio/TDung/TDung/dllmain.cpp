// dllmain.cpp : Defines the entry point for the DLL application.
#include"pch.h"
#include"dllmain.h"

#pragma data_seg("shared")
int count = 0;
#pragma data_seg()
#pragma comment(linker,"/SECTION:shared,RWS")

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD msg, PVOID reserved)
{
	return TRUE;
}

extern "C" _declspec (dllexport) int TDSetCount(int newCount)
{
	count = newCount;
	return count;
}

extern "C" _declspec (dllexport) int TDGetCount()
{
	return count;
}

//image functions
extern "C" _declspec (dllexport) HBITMAP GetColorMask(HWND hwnd, HBITMAP image, COLORREF colorKey)
{
	if (image == nullptr)
		return nullptr;

	BITMAP bmpSpec;
	GetObject(image, sizeof(BITMAP), &bmpSpec);

	HDC screenDC = GetDC(hwnd);
	HDC srcDC = CreateCompatibleDC(screenDC);
	HDC maskDC = CreateCompatibleDC(screenDC);
	HDC	conversionDC = CreateCompatibleDC(screenDC);
	ReleaseDC(hwnd, screenDC);

	if (srcDC == nullptr || maskDC == nullptr)
		return nullptr;

	HBITMAP conversionBmp, oldBmp;
	if (conversionDC == nullptr)
	{
		return nullptr;
	}
	BITMAPINFOHEADER bmpInfo = {sizeof(BITMAPINFOHEADER), bmpSpec.bmWidth, bmpSpec.bmHeight, 1, min(24, bmpSpec.bmBitsPixel), BI_RGB, 0, 0, 0, 0, 0};
	void* bits;

	conversionBmp = CreateDIBSection(nullptr, reinterpret_cast<BITMAPINFO*>(&bmpInfo), DIB_RGB_COLORS, &bits, nullptr, 0);

	if (conversionBmp == nullptr)
		return nullptr;

	HBITMAP oldBmp2 = reinterpret_cast<HBITMAP>(SelectObject(conversionDC, conversionBmp));
	oldBmp = reinterpret_cast<HBITMAP>(SelectObject(srcDC, image));

	BitBlt(conversionDC, 0, 0, bmpSpec.bmWidth, bmpSpec.bmHeight, srcDC, 0, 0, SRCCOPY);

	HBITMAP maskBmp = CreateBitmap(bmpSpec.bmWidth, bmpSpec.bmHeight, 1, 1, nullptr);
	HBITMAP oldBmp3 = reinterpret_cast<HBITMAP>(SelectObject(maskDC, maskBmp));

	if (maskBmp == nullptr)
		return nullptr;

	SelectObject(conversionDC, CreateSolidBrush(colorKey));
	PatBlt(conversionDC, 0, 0, bmpSpec.bmWidth, bmpSpec.bmHeight, 0xA50065);			//PDxn
	BitBlt(maskDC, 0, 0, bmpSpec.bmWidth, bmpSpec.bmHeight, conversionDC, 0, 0, SRCCOPY);

	SelectObject(srcDC, oldBmp);
	DeleteDC(srcDC);

	DeleteObject(SelectObject(conversionDC, oldBmp2));
	DeleteDC(conversionDC);

	SelectObject(maskDC, oldBmp3);
	DeleteObject(SelectObject(maskDC, GetStockObject(WHITE_BRUSH)));
	DeleteDC(maskDC);

	return maskBmp;
}

extern "C" _declspec (dllexport) HDC ApplyTransparency(HDC dst, int x, int y, int w, int h, int x2, int y2, int w2, int h2, HBITMAP image, HBITMAP mask)
{
	if (dst == nullptr || image == nullptr)
		return dst;

	BITMAP bmpSpec;
	GetObject(image, sizeof(BITMAP), &bmpSpec);

	HDC imageDC = CreateCompatibleDC(dst);
	HBITMAP oldBmp = reinterpret_cast<HBITMAP>(SelectObject(imageDC, image));

	if (mask == nullptr)
	{
		BitBlt(dst, x, y, bmpSpec.bmWidth, bmpSpec.bmHeight, imageDC, 0, 0, SRCCOPY);

		SelectObject(imageDC, oldBmp);
		DeleteDC(imageDC);
		return dst;
	}

	HDC imageCopyDC = CreateCompatibleDC(dst);
	HBITMAP oldBmp3 = reinterpret_cast<HBITMAP>(SelectObject(imageCopyDC, CreateCompatibleBitmap(dst, bmpSpec.bmWidth, bmpSpec.bmHeight)));

	BitBlt(imageCopyDC, 0, 0, bmpSpec.bmWidth, bmpSpec.bmHeight, imageDC, 0, 0, SRCCOPY);

	HDC maskDC = CreateCompatibleDC(dst);
	HBITMAP oldBmp2 = reinterpret_cast<HBITMAP>(SelectObject(maskDC, mask));

	BitBlt(imageCopyDC, 0, 0, bmpSpec.bmWidth, bmpSpec.bmHeight, maskDC, 0, 0, SRCPAINT);
	StretchBlt(dst, x, y, w == 0 ? bmpSpec.bmWidth : w, h == 0 ? bmpSpec.bmHeight : h, maskDC, x2, y2, w2 == 0 ? bmpSpec.bmWidth : w2, h2 == 0 ? bmpSpec.bmHeight : h2, MERGEPAINT);
	StretchBlt(dst, x, y, w == 0 ? bmpSpec.bmWidth : w, h == 0 ? bmpSpec.bmHeight : h, imageCopyDC, x2, y2, w2 == 0 ? bmpSpec.bmWidth : w2, h2 == 0 ? bmpSpec.bmHeight : h2, SRCAND);

	SelectObject(imageDC, oldBmp);
	DeleteDC(imageDC);

	SelectObject(maskDC, oldBmp2);
	DeleteDC(maskDC);

	return dst;
}