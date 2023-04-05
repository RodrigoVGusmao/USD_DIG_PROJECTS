#include<windows.h>

#include<cinttypes>
#include<vector>

#include "constants.h"
#include "debug.h"

struct data {
	HDC* memoryDC;
	std::vector<POINT>* rgnData;
};

struct package {
	HBITMAP bmpData;
	std::vector<POINT> rgnData;
};

struct extLogPen {
	uint32_t flags;
	uint16_t width;
	LOGBRUSH brushData;
	std::vector<DWORD> data;
};

EXTLOGPEN test;

//DC manipulation functions
HBITMAP createNewMemoryDC(HWND compatibleHwnd, HDC* newDC, HBITMAP* newBitmap, int bmpWidth, int bmpHeight)
{
	HDC Dc = GetDC(compatibleHwnd);
	*newBitmap = CreateCompatibleBitmap(Dc, bmpWidth, bmpHeight);
	*newDC = CreateCompatibleDC(Dc);
	ReleaseDC(compatibleHwnd, Dc);

	return reinterpret_cast<HBITMAP>(SelectObject(*newDC, *newBitmap));
}

HBITMAP changeBitmapSize(HWND compatibleHwnd, HDC memoryDC, int newWidth, int newHeight)
{
	HDC DC = GetDC(compatibleHwnd);
	HDC tmpDC = CreateCompatibleDC(DC);
	HBITMAP tmpBmp = CreateCompatibleBitmap(DC, newWidth, newHeight);
	ReleaseDC(compatibleHwnd, DC);

	HBITMAP oldBmp = reinterpret_cast<HBITMAP>(SelectObject(memoryDC, tmpBmp));

	DeleteObject(SelectObject(tmpDC, oldBmp));
	PatBlt(memoryDC, 0, 0, newWidth, newHeight, PATCOPY);
	BitBlt(memoryDC, 0, 0, newWidth, newHeight, tmpDC, 0, 0, SRCCOPY);
	DeleteDC(tmpDC);

	return tmpBmp;
}

HBITMAP copyBitmap(HWND compatibleHwnd, HDC memoryDC)
{
	BITMAP bmpInfo;
	GetObject(GetCurrentObject(memoryDC, OBJ_BITMAP), sizeof(BITMAP), &bmpInfo);

	HDC DC = GetDC(compatibleHwnd);
	HDC tmpDC = CreateCompatibleDC(DC);
	HBITMAP bmpCopy = CreateCompatibleBitmap(DC, bmpInfo.bmWidth, bmpInfo.bmHeight);
	ReleaseDC(compatibleHwnd, DC);

	HBITMAP tmpBmp = reinterpret_cast<HBITMAP>(SelectObject(tmpDC, bmpCopy));
	BitBlt(tmpDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, memoryDC, 0, 0, SRCCOPY);

	SelectObject(tmpDC, tmpBmp);
	DeleteDC(tmpDC);

	return bmpCopy;
}

HBITMAP createColorMask(HWND compatibleHwnd, HDC srcDC, COLORREF color)
{
	BITMAP bmp;
	GetObject(GetCurrentObject(srcDC, OBJ_BITMAP), sizeof(BITMAP), &bmp);

	BITMAPINFOHEADER bmpInfo = { sizeof(BITMAPINFOHEADER), bmp.bmWidth, bmp.bmHeight, 1, 24, BI_RGB, 0, 0, 0, 0, 0 };
	void* bits;
	HBITMAP convBmp = CreateDIBSection(nullptr, reinterpret_cast<BITMAPINFO*>(&bmpInfo), DIB_RGB_COLORS, &bits, nullptr, 0);

	HBITMAP mask = CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, nullptr);

	HDC DC = GetDC(compatibleHwnd);
	HDC convDC = CreateCompatibleDC(DC);
	HDC maskDC = CreateCompatibleDC(DC);
	ReleaseDC(compatibleHwnd, DC);

	DeleteObject(SelectObject(convDC, convBmp));
	BitBlt(convDC, 0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);

	SelectObject(convDC, CreateSolidBrush(color));
	PatBlt(convDC, 0, 0, bmp.bmWidth, bmp.bmHeight, 0xA50065);		//PDxn

	DeleteObject(SelectObject(convDC, GetStockObject(WHITE_BRUSH)));

	HBITMAP oldBmp = reinterpret_cast<HBITMAP>(SelectObject(maskDC, mask));
	BitBlt(maskDC, 0, 0, bmp.bmWidth, bmp.bmHeight, convDC, 0, 0, SRCCOPY);

	DeleteDC(convDC);
	DeleteObject(convBmp);

	SelectObject(maskDC, oldBmp);
	DeleteDC(maskDC);

	return mask;
}

HRGN createRgnFromMask(HRGN* rgn, HBITMAP mask)
{
	BITMAP bmp;
	GetObject(mask, sizeof(BITMAP), &bmp);

	std::vector<uint8_t> bits;
	bits.resize(bmp.bmHeight * (0x10 - (bmp.bmWidth & 0xF) + bmp.bmWidth >> 0x3));
	GetBitmapBits(mask, bits.size()*2, bits.data());

	std::vector<RECT> regionRect;
	RECT currentRect = {0, 0, 0, 0};
	RECT boundingBox = {~0 >> 1, ~0 >> 1, ~0, ~0};
	uint8_t byte;
	regionRect.reserve(bmp.bmHeight);

	for (uint32_t i = 0; i < bmp.bmHeight; ++i)
	{
		bool black = false;
		for (uint32_t j = 0; j < 0x10 - (bmp.bmWidth & 0xF) + bmp.bmWidth >> 0x3; ++j)
		{
			byte = bits[i * (0x10 - (bmp.bmWidth & 0xF) + bmp.bmWidth >> 0x3) + j];
			for (uint8_t k = 0; k < 8; ++k, byte <<= 1)
			{
				if (byte & 0x80 && !black)
				{
					black = true;

					currentRect.left = j * 8 + k;
					currentRect.top = i;

					boundingBox.left = min(boundingBox.left, currentRect.left);
					boundingBox.top = min(boundingBox.top, currentRect.top);
				}
				else if (!(byte & 0x80) && black)
				{
					black = false;

					currentRect.right = j * 8 + k;
					currentRect.bottom = i+1;
					regionRect.push_back(currentRect);

					boundingBox.right = max(boundingBox.right, currentRect.right);
					boundingBox.bottom = max(boundingBox.bottom, currentRect.bottom);
				}
			}
		}
		if (black)
		{
			black = false;

			currentRect.right = (0x10 - (bmp.bmWidth & 0xF) + bmp.bmWidth >> 0x6) + 9;
			currentRect.bottom = i+1;
			regionRect.push_back(currentRect);

			boundingBox.right = max(boundingBox.right, currentRect.right);
			boundingBox.bottom = max(boundingBox.bottom, currentRect.bottom);
		}
	}

	RGNDATAHEADER rgnDataHeader;
	rgnDataHeader.dwSize = sizeof(RGNDATAHEADER);
	rgnDataHeader.iType = RDH_RECTANGLES;
	rgnDataHeader.nCount = regionRect.size();
	rgnDataHeader.nRgnSize = regionRect.size() * sizeof(RECT);
	rgnDataHeader.rcBound = boundingBox;

	RGNDATA* rgnData = reinterpret_cast<RGNDATA*>(new char[sizeof(RGNDATAHEADER) + sizeof(RECT)*regionRect.size()]);
	if (rgnData == nullptr)
	{
		DebugWin(L"oops!", L"rgnData memory allocation failed...");
		return nullptr;
	}
	rgnData->rdh = rgnDataHeader;

	for (uint32_t i = 0; i < regionRect.size() * sizeof(RECT); ++i)
	{
		rgnData->Buffer[i] = reinterpret_cast<char*>(regionRect.data())[i];
	}

	*rgn = ExtCreateRegion(nullptr, sizeof(RGNDATAHEADER) + sizeof(RECT) * regionRect.size(), rgnData);
	if (*rgn == nullptr)
		DebugWin(L"oops!", L"ExtCreateRegion failed...");

	return *rgn;
}

HBITMAP pasteDC(HWND compatibleHwnd, HDC dstDC, int dstWidth, int dstHeight, HDC srcDC, HRGN pasteRgn, POINT* pastePos)
{
	HBITMAP bitmap;

	OffsetRgn(pasteRgn, pastePos->x, pastePos->y);
	FillRgn(dstDC, pasteRgn, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
	OffsetRgn(pasteRgn, -pastePos->x, -pastePos->y);
	BitBlt(dstDC, pastePos->x, pastePos->y, dstWidth, dstHeight, srcDC, 0, 0, SRCAND);

	bitmap = changeBitmapSize(compatibleHwnd, srcDC, dstWidth, dstHeight);
	BitBlt(srcDC, 0, 0, dstWidth, dstHeight, dstDC, 0, 0, SRCCOPY);

	*pastePos = { 0, 0 };

	return bitmap;
}

void paintDCBitmapWhite(HDC DC, int srcWidth, int srcHeight, HRGN* rgn)
{
	PatBlt(DC, 0, 0, srcWidth, srcHeight, WHITENESS);
}

HBRUSH createChessPattern(HWND compatibleHwnd)
{
	HDC tempDC;
	HBITMAP pattern;
	HBITMAP oldPattern = createNewMemoryDC(compatibleHwnd, &tempDC, &pattern, 20, 20);
	RECT rect = { 0, 0, 10, 10 };
	FillRect(tempDC, &rect, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
	rect = { 10, 10, 20, 20 };
	FillRect(tempDC, &rect, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

	SelectObject(tempDC, oldPattern);
	DeleteDC(tempDC);

	HBRUSH patBrush = CreatePatternBrush(pattern);
	DeleteObject(pattern);

	return patBrush;
}

LRESULT CALLBACK drawAreaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static std::vector<POINT> toolLine;
	static POINT appBmpPos, appBmpRelPos, appBmpInitialPos, drawStartPt;
	static extLogPen selectedPen;
	static bool LMouseDown, hasSelectedArea;
	static HBITMAP vBmp, vToolBmp, tempToolBmp, appBmp, tempBmp, bufferBmp;
	static HBITMAP oldvBmp, oldvToolBmp, oldTempToolBmp, oldAppBmp, oldTempBmp, oldBufferBmp;
	static HDC metaDC, vDC, vToolDC, tempToolDC, appDC, tempDC, buffer;
	static uint16_t WndWidth, WndHeight;
	static uint16_t tool;
	static uint8_t shortProperties;
	static HRGN clipRgn, oldClipRgn;

	switch (msg)
	{
		case WM_CREATE:
		{
			RECT wndPts;
			GetClientRect(hwnd, &wndPts);
			WndWidth = wndPts.right - wndPts.left;
			WndHeight = wndPts.bottom - wndPts.top;

			selectedPen = { PS_GEOMETRIC | PS_SOLID, 1, {BS_SOLID, 0x0} };

			oldBufferBmp = createNewMemoryDC(hwnd, &buffer, &bufferBmp, WndWidth, WndHeight);

			oldvBmp = createNewMemoryDC(hwnd, &vDC, &vBmp, WndWidth, WndHeight);
			PatBlt(vDC, 0, 0, WndWidth, WndHeight, WHITENESS);

			oldvToolBmp = createNewMemoryDC(hwnd, &vToolDC, &vToolBmp, WndWidth, WndHeight);
			PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);

			oldTempToolBmp = createNewMemoryDC(hwnd, &tempToolDC, &tempToolBmp, WndWidth, WndHeight);
			PatBlt(tempToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);

			oldAppBmp = createNewMemoryDC(hwnd, &appDC, &appBmp, WndWidth, WndHeight);
			PatBlt(appDC, 0, 0, WndWidth, WndHeight, WHITENESS);

			oldTempBmp = createNewMemoryDC(hwnd, &tempDC, &tempBmp, WndWidth, WndHeight);

			clipRgn = nullptr;
			hasSelectedArea = false;

			data pointers;
			pointers.memoryDC = &appDC;
			pointers.rgnData = &toolLine;
			*reinterpret_cast<data*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams) = pointers;

			return 0;
		}

		case WM_SIZE:
		{
			WndWidth = LOWORD(lParam);
			WndHeight = HIWORD(lParam);

			bufferBmp = changeBitmapSize(hwnd, buffer, WndWidth, WndHeight);
			vBmp = changeBitmapSize(hwnd, vDC, WndWidth, WndHeight);
			vToolBmp = changeBitmapSize(hwnd, vToolDC, WndWidth, WndHeight);
			tempToolBmp = changeBitmapSize(hwnd, tempToolDC, WndWidth, WndHeight);
			tempBmp = changeBitmapSize(hwnd, tempDC, WndWidth, WndHeight);

			if(tool & UM_PEN)
				appBmp = changeBitmapSize(hwnd, appDC, WndWidth, WndHeight);

			InvalidateRect(hwnd, nullptr, FALSE);

			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			LMouseDown = true;
			SetCapture(hwnd);

			POINTS currentMousePos = MAKEPOINTS(lParam);

			//delete Area
			if (tool & UM_PATH)
			{
				appBmp = pasteDC(hwnd, vDC, WndWidth, WndHeight, appDC, clipRgn, &appBmpPos);
				paintDCBitmapWhite(vToolDC, WndWidth, WndHeight, &clipRgn);
				hasSelectedArea = false;

				InvalidateRect(hwnd, nullptr, FALSE);
			}

			if (tool & UM_PEN)
			{
				HPEN pen = ExtCreatePen(selectedPen.flags, selectedPen.width, &selectedPen.brushData, selectedPen.data.size(), selectedPen.data.empty() ? nullptr : selectedPen.data.data());
				if (pen == nullptr)
					DebugWin(L"error", L"ExtCreatePen failed with error: %d\nflags: %x\nwidth: %d\n", GetLastError(), selectedPen.flags, selectedPen.width);

				if (tool & (UMF_RECTANGLE | UMF_ELLIPSE))
				{
					SelectObject(appDC, pen);
					POINTSTOPOINT(drawStartPt, currentMousePos);

					BitBlt(tempDC, 0, 0, WndWidth, WndHeight, appDC, 0, 0, SRCCOPY);
				}
				else if(!(tool & UMF_FILL))
				{
					SelectObject(vDC, pen);
					MoveToEx(vDC, currentMousePos.x - (hasSelectedArea ? appBmpPos.x : 0), currentMousePos.y - (hasSelectedArea ? appBmpPos.y : 0), nullptr);

					SelectObject(appDC, CreatePen(selectedPen.flags & 0x7, selectedPen.width, selectedPen.brushData.lbColor));
					MoveToEx(appDC, currentMousePos.x - (hasSelectedArea ? appBmpPos.x : 0), currentMousePos.y - (hasSelectedArea ? appBmpPos.y : 0), nullptr);

					BeginPath(vDC);
				}

				if (hasSelectedArea)
					SelectClipRgn(appDC, clipRgn);
			}
			else if (tool & UM_PATH)
			{
				HBRUSH toolBrush = CreateSolidBrush(0x00FFEDC0);

				SelectObject(vToolDC, toolBrush);
				SelectObject(vToolDC, CreatePen(PS_DASH, 1, 0x0));

				if (tool & (UMF_RECTANGLE | UMF_ELLIPSE))
				{
					POINTSTOPOINT(drawStartPt, currentMousePos);
				}
				else if(!(tool & UMF_FILL))
				{
					HBRUSH patBrush = createChessPattern(hwnd);

					PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);

					OffsetRgn(clipRgn, appBmpPos.x, appBmpPos.y);
					FillRgn(tempToolDC, clipRgn, toolBrush);
					FrameRgn(tempToolDC, clipRgn, patBrush, 1, 1);
					OffsetRgn(clipRgn, -appBmpPos.x, -appBmpPos.y);

					DeleteObject(patBrush);

					toolLine.clear();
					toolLine.push_back({ currentMousePos.x, currentMousePos.y });

					BeginPath(appDC);
					MoveToEx(appDC, currentMousePos.x, currentMousePos.y, nullptr);
				}
			}
			else if (tool & UM_TRANSFORM)
			{
				HBRUSH patBrush = createChessPattern(hwnd);

				PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, BLACKNESS);
				PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);

				OffsetRgn(clipRgn, appBmpInitialPos.x, appBmpInitialPos.y);
				FillRgn(tempDC, clipRgn, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
				FrameRgn(vToolDC, clipRgn, patBrush, 1, 1);
				OffsetRgn(clipRgn, -appBmpInitialPos.x, -appBmpInitialPos.y);

				DeleteObject(patBrush);

				appBmpRelPos.x = LOWORD(lParam) - appBmpPos.x;
				appBmpRelPos.y = HIWORD(lParam) - appBmpPos.y;
			}
			return 0;
		}

		case WM_LBUTTONUP:
		{
			ReleaseCapture();
			POINTS currentMousePos = MAKEPOINTS(lParam);

			LMouseDown = false;
			if (tool & UM_PEN)
			{
				if (hasSelectedArea)
					SelectClipRgn(appDC, nullptr);

				if (tool & (UMF_RECTANGLE | UMF_ELLIPSE))
				{
					if (!hasSelectedArea)
						BitBlt(vDC, 0, 0, WndWidth, WndHeight, appDC, 0, 0, SRCCOPY);
					DeleteObject(SelectObject(appDC, GetStockObject(BLACK_PEN)));
				}
				else if (tool & UMF_FILL)
				{
					HBITMAP mask = createColorMask(hwnd, vDC, GetPixel(vDC, currentMousePos.x, currentMousePos.y));

					HDC wndDC = GetDC(hwnd);
					HDC maskDC = CreateCompatibleDC(wndDC);
					ReleaseDC(hwnd, wndDC);

					HBITMAP oldBmp = reinterpret_cast<HBITMAP>(SelectObject(maskDC, mask));
					HBRUSH oldBrush = reinterpret_cast<HBRUSH>(SelectObject(vDC, CreateSolidBrush(selectedPen.brushData.lbColor)));
					BitBlt(vDC, 0, 0, WndWidth, WndHeight, maskDC, 0, 0, 0xE20746);		//DSPDxax
				}
				else
				{
					EndPath(vDC);
					StrokePath(vDC);

					PatBlt(appDC, 0, 0, WndWidth, WndHeight, WHITENESS);

					DeleteObject(SelectObject(vDC, GetStockObject(BLACK_PEN)));
					DeleteObject(SelectObject(appDC, GetStockObject(BLACK_PEN)));
				}
			}
			else if (tool & UM_PATH)
			{
				if (toolLine.size() == 1)
					toolLine.pop_back();

				oldClipRgn = clipRgn;

				if (tool & (UMF_RECTANGLE | UMF_ELLIPSE))
				{
					hasSelectedArea = true;

					if (tool & UMF_RECTANGLE)
					{
						clipRgn = CreateRectRgn(drawStartPt.x, drawStartPt.y, max(min(currentMousePos.x, WndWidth - 1), 1), max(min(currentMousePos.y, WndHeight - 1), 1));
					}
					else if (tool & UMF_ELLIPSE)
					{
						clipRgn = CreateEllipticRgn(drawStartPt.x, drawStartPt.y, max(min(currentMousePos.x, WndWidth - 1), 1), max(min(currentMousePos.y, WndHeight - 1), 1));
					}
				}
				else if (tool & UMF_FILL)
				{
					HBITMAP mask = createColorMask(hwnd, vDC, GetPixel(vDC, currentMousePos.x, currentMousePos.y));
					createRgnFromMask(&clipRgn, mask);

					HBRUSH patBrush = createChessPattern(hwnd);

					PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);

					OffsetRgn(clipRgn, appBmpPos.x, appBmpPos.y);
					FillRgn(vToolDC, clipRgn, reinterpret_cast<HBRUSH>(GetCurrentObject(vToolDC, OBJ_BRUSH)));
					FrameRgn(vToolDC, clipRgn, patBrush, 1, 1);
					OffsetRgn(clipRgn, -appBmpPos.x, -appBmpPos.y);

					DeleteObject(patBrush);

					hasSelectedArea = true;
				}
				else
				{
					EndPath(appDC);

					hasSelectedArea = true;

					clipRgn = PathToRegion(appDC);
				}

				if (oldClipRgn != nullptr)
				{
					OffsetRgn(oldClipRgn, appBmpInitialPos.x, appBmpInitialPos.y);
					if(shortProperties == RGN_DIFF)
						CombineRgn(clipRgn, oldClipRgn, clipRgn, shortProperties);
					else
						CombineRgn(clipRgn, clipRgn, oldClipRgn, shortProperties);
				}

				RECT rgnBox;
				GetRgnBox(clipRgn, &rgnBox);

				PatBlt(appDC, rgnBox.left, rgnBox.top, rgnBox.right, rgnBox.bottom, WHITENESS);
				SelectClipRgn(appDC, clipRgn);

				BitBlt(appDC, 0, 0, WndWidth, WndHeight, vDC, 0, 0, SRCCOPY);

				SelectClipRgn(appDC, nullptr);

				BitBlt(appDC, 0, 0, rgnBox.right + 1 - rgnBox.left, rgnBox.bottom + 1 - rgnBox.top, appDC, rgnBox.left, rgnBox.top, SRCCOPY);

				appBmp = changeBitmapSize(hwnd, appDC, rgnBox.right + 1 - rgnBox.left, rgnBox.bottom + 1 - rgnBox.top);

				appBmpPos.x = rgnBox.left;
				appBmpPos.y = rgnBox.top;

				appBmpInitialPos.x = rgnBox.left;
				appBmpInitialPos.y = rgnBox.top;

				//region bitmap att
				OffsetRgn(clipRgn, -appBmpInitialPos.x, -appBmpInitialPos.y);

				PatBlt(tempToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);

				HBRUSH patBrush = createChessPattern(hwnd);

				PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);

				OffsetRgn(clipRgn, appBmpPos.x, appBmpPos.y);
				FillRgn(vToolDC, clipRgn, reinterpret_cast<HBRUSH>(GetCurrentObject(vToolDC, OBJ_BRUSH)));
				FrameRgn(vToolDC, clipRgn, patBrush, 1, 1);
				OffsetRgn(clipRgn, -appBmpPos.x, -appBmpPos.y);

				DeleteObject(patBrush);
				DeleteObject(SelectObject(vToolDC, GetStockObject(BLACK_PEN)));
				DeleteObject(SelectObject(vToolDC, GetStockObject(WHITE_BRUSH)));
			}
			else if (tool & UM_TRANSFORM)
			{
				DeleteObject(SelectObject(vToolDC, GetStockObject(WHITE_BRUSH)));
			}
			if(!hasSelectedArea)
				BitBlt(appDC, 0, 0, WndWidth, WndHeight, vDC, 0, 0, SRCCOPY);

			InvalidateRect(hwnd, nullptr, FALSE);
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			if (LMouseDown)
			{
				POINTS currentMousePos = MAKEPOINTS(lParam);

				if (tool & UM_PEN)
				{
					if (tool & UMF_RECTANGLE)
					{
						BitBlt(appDC, 0, 0, WndWidth, WndHeight, tempDC, 0, 0, SRCCOPY);
						Rectangle(appDC, drawStartPt.x - (hasSelectedArea ? appBmpPos.x : 0), drawStartPt.y - (hasSelectedArea ? appBmpPos.y : 0),
							currentMousePos.x - (hasSelectedArea ? appBmpPos.x : 0), currentMousePos.y - (hasSelectedArea ? appBmpPos.y : 0));
					}
					else if (tool & UMF_ELLIPSE)
					{
						BitBlt(appDC, 0, 0, WndWidth, WndHeight, tempDC, 0, 0, SRCCOPY);
						Ellipse(appDC, drawStartPt.x - (hasSelectedArea ? appBmpPos.x : 0), drawStartPt.y - (hasSelectedArea ? appBmpPos.y : 0),
							currentMousePos.x - (hasSelectedArea ? appBmpPos.x : 0), currentMousePos.y - (hasSelectedArea ? appBmpPos.y : 0));
					}
					else
					{
						LineTo(vDC, currentMousePos.x - (hasSelectedArea ? appBmpPos.x : 0), currentMousePos.y - (hasSelectedArea ? appBmpPos.y : 0));
						LineTo(appDC, currentMousePos.x - (hasSelectedArea ? appBmpPos.x : 0), currentMousePos.y - (hasSelectedArea ? appBmpPos.y : 0));
					}
				}
				else if (tool & UM_PATH)
				{
					PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);
					if (tool & UMF_RECTANGLE)
					{
						Rectangle(vToolDC, drawStartPt.x, drawStartPt.y, max(min(currentMousePos.x, WndWidth - 1), 1), max(min(currentMousePos.y, WndHeight - 1), 1));
					}
					else if (tool & UMF_ELLIPSE)
					{
						Ellipse(vToolDC, drawStartPt.x, drawStartPt.y, max(min(currentMousePos.x, WndWidth - 1), 1), max(min(currentMousePos.y, WndHeight - 1), 1));
					}
					else if(!(tool & UMF_FILL))
					{
						POINT mousePt;
						POINTSTOPOINT(mousePt, currentMousePos);
						toolLine.push_back({ max(min(mousePt.x, WndWidth - 1), 1), max(min(mousePt.y, WndHeight - 1), 1) });

						Polygon(vToolDC, toolLine.data(), toolLine.size());
						LineTo(appDC, max(min(mousePt.x, WndWidth), 1), max(min(mousePt.y, WndHeight), 1));
					}
				}
				else if (tool & UM_TRANSFORM)
				{
					if (shortProperties == UMF_STRETCH)
					{
						PatBlt(tempToolDC, 0, 0, WndWidth, WndHeight, BLACKNESS);

						RECT rgnPts;
						GetRgnBox(clipRgn, &rgnPts);

						StretchBlt(tempToolDC, appBmpInitialPos.x, appBmpInitialPos.y, rgnPts.right + currentMousePos.x - appBmpRelPos.x, rgnPts.bottom + currentMousePos.y - appBmpRelPos.y,
							appDC, 0, 0, rgnPts.right - rgnPts.left, rgnPts.bottom - rgnPts.top, SRCCOPY);

						HBITMAP regionFormat = createColorMask(hwnd, tempToolDC, 0xFFFFFF);
						HRGN tempRgn;
						createRgnFromMask(&tempRgn, regionFormat);
						DeleteObject(regionFormat);

						PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);
						HBRUSH chessPattern = createChessPattern(hwnd);
						FrameRgn(vToolDC, tempRgn, chessPattern, 1, 1);
						DeleteObject(tempRgn);
						DeleteObject(chessPattern);


						/*appBmpRelPos.x = currentMousePos.x;
						appBmpRelPos.y = currentMousePos.y;*/
					}
					else
					{
						appBmpPos.x = currentMousePos.x - appBmpRelPos.x;
						appBmpPos.y = currentMousePos.y - appBmpRelPos.y;
					}
				}
				InvalidateRect(hwnd, nullptr, FALSE);
			}
			return 0;
		}

		case WM_PAINT:
		{
			BitBlt(buffer, 0, 0, WndWidth, WndHeight, vDC, 0, 0, SRCCOPY);

			if (tool & UM_PEN && !(tool & 0xFF00))
			{
				if(LMouseDown)
					BitBlt(buffer, 0, 0, WndWidth, WndHeight, appDC, 0, 0, SRCCOPY);
			}
			else if (tool & UM_PEN && tool & (UMF_RECTANGLE | UMF_ELLIPSE))
			{
				BitBlt(buffer, 0, 0, WndWidth, WndHeight, appDC, 0, 0, SRCCOPY);
			}
			else if (tool & UM_PATH)
			{
				if(shortProperties != RGN_COPY)
					BitBlt(buffer, appBmpInitialPos.x, appBmpInitialPos.y, WndWidth, WndHeight, tempToolDC, 0, 0, SRCAND);
				BitBlt(buffer, 0, 0, WndWidth, WndHeight, vToolDC, 0, 0, SRCAND);
			}
			else if (tool & (UM_TRANSFORM | UM_CLIPAREA))
			{
				BitBlt(buffer, appBmpPos.x - appBmpInitialPos.x, appBmpPos.y - appBmpInitialPos.y, WndWidth, WndHeight, LMouseDown && shortProperties == UMF_STRETCH ? tempToolDC : tempDC, 0, 0, SRCCOPY);
				BitBlt(buffer, appBmpPos.x, appBmpPos.y, WndWidth, WndHeight, appDC, 0, 0, SRCAND);

				BitBlt(buffer, appBmpPos.x-appBmpInitialPos.x, appBmpPos.y - appBmpInitialPos.y, WndWidth, WndHeight, vToolDC, 0, 0, SRCAND);
			}

			if (tool & UM_TEST)
			{
				HDC rgnTest;
				HBITMAP rgnTestBmp;
				rgnTestBmp = createNewMemoryDC(hwnd, &rgnTest, &rgnTestBmp, WndWidth, WndHeight);
				PatBlt(rgnTest, 0, 0, WndWidth, WndHeight, WHITENESS);

				SelectObject(rgnTest, GetStockObject(GRAY_BRUSH));
				Ellipse(rgnTest, 0, 0, WndWidth / 2, WndHeight / 2);
				Ellipse(rgnTest, WndWidth / 2, WndHeight / 2, WndWidth, WndHeight);

				SelectObject(rgnTest, GetStockObject(DKGRAY_BRUSH));
				Rectangle(rgnTest, 5 * WndWidth / 8, WndHeight / 8, 7 * WndWidth / 8, 3 * WndHeight / 8);
				Rectangle(rgnTest, WndWidth / 8, 5 * WndHeight / 8, 3 * WndWidth / 8, 7 * WndHeight / 8);

				HBITMAP mask = createColorMask(hwnd, rgnTest, 0xFFFFFF);
				HRGN rgn = createRgnFromMask(&rgn, mask);

				SelectClipRgn(rgnTest, rgn);
				SelectObject(rgnTest, CreateHatchBrush(HS_CROSS, 0xFF8040));
				PatBlt(rgnTest, 0, 0, WndWidth, WndHeight, PATCOPY);

				BitBlt(buffer, 0, 0, WndWidth, WndHeight, rgnTest, 0, 0, SRCCOPY);

				DeleteObject(SelectObject(rgnTest, GetStockObject(WHITE_BRUSH)));
				DeleteDC(rgnTest);

				DeleteObject(mask);
				DeleteObject(rgnTestBmp);
				DeleteObject(rgn);
			}

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			BitBlt(hdc, 0, 0, WndWidth, WndHeight, buffer, 0, 0, SRCCOPY);

			EndPaint(hwnd, &ps);
			return 0;
		}

		case WM_USER:
		{
			if ((tool & UM_TRANSFORM) && !(LOWORD(wParam) & UM_TRANSFORM))
			{
				appBmp = pasteDC(hwnd, vDC, WndWidth, WndHeight, appDC, clipRgn, &appBmpPos);
				paintDCBitmapWhite(vToolDC, WndWidth, WndHeight, &clipRgn);
				hasSelectedArea = false;

				InvalidateRect(hwnd, nullptr, FALSE);
			}

			switch (LOWORD(wParam) & 0xFF)
			{
				case UM_PROPERTIES:
					selectedPen = *reinterpret_cast<extLogPen*>(lParam);
					break;

				case UM_PEN:
					if (tool & (UM_PATH | UM_CLIPAREA))
						tool = UM_CLIPAREA | UM_PEN | (wParam & 0xFF00);
					else
						tool = LOWORD(wParam);
					break;

				case UM_PATH:
				case UM_TRANSFORM:
					tool = LOWORD(wParam);
					shortProperties = HIBYTE(wParam) ? HIBYTE(wParam) : RGN_COPY;
					break;

				case UM_TEST:
					tool = LOWORD(wParam);
					InvalidateRect(hwnd, nullptr, FALSE);
			}
			return 0;
		}

		case WM_USER+1:
		{
			if (wParam & UM_NEWFILE)
			{
				selectedPen = { PS_GEOMETRIC | PS_SOLID, 1, {BS_SOLID, 0x0} };
				PatBlt(vDC, 0, 0, WndWidth, WndHeight, WHITENESS);

				hasSelectedArea = false;
			}
			if (wParam & UM_PASTEIMAGE)
			{

				BITMAP bmpInfo;
				GetObject((reinterpret_cast<package*>(lParam)->bmpData), sizeof(BITMAP), &bmpInfo);

				HDC tmpDC;
				HBITMAP tmpBmp;
				createNewMemoryDC(hwnd, &tmpDC, &tmpBmp, bmpInfo.bmWidth, bmpInfo.bmHeight);
				HBITMAP oldBmp = reinterpret_cast<HBITMAP>(SelectObject(tmpDC, (reinterpret_cast<package*>(lParam)->bmpData)));

				if (wParam & UMF_BUFFER)
				{
					BitBlt(appDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, tmpDC, 0, 0, SRCCOPY);
					appBmp = changeBitmapSize(hwnd, appDC, bmpInfo.bmWidth, bmpInfo.bmHeight);

					SelectObject(vToolDC, GetStockObject(WHITE_BRUSH));
					SelectObject(vToolDC, CreatePen(PS_DASH, 1, 0x0));

					PatBlt(vToolDC, 0, 0, WndWidth, WndHeight, WHITENESS);

					toolLine = reinterpret_cast<package*>(lParam)->rgnData;
					if (toolLine.empty())
					{
						toolLine.clear();
						toolLine.push_back({ appBmpPos.x, appBmpPos.y });
						toolLine.push_back({ bmpInfo.bmWidth + appBmpPos.x, appBmpPos.y });
						toolLine.push_back({ bmpInfo.bmWidth + appBmpPos.x, bmpInfo.bmHeight + appBmpPos.y });
						toolLine.push_back({ appBmpPos.x, bmpInfo.bmHeight + appBmpPos.y });
					}

					Polygon(vToolDC, toolLine.data(), toolLine.size());
					CreatePolygonRgn(toolLine.data(), toolLine.size(), ALTERNATE);

					DeleteObject(SelectObject(vToolDC, GetStockObject(BLACK_PEN)));
				}
				else
				{
					RECT wndPts;
					RECT clientPts;
					GetWindowRect(hwnd, &wndPts);
					GetClientRect(hwnd, &clientPts);
					POINT windowPos = { wndPts.left, wndPts.top };
					ScreenToClient(GetParent(hwnd), &windowPos);
					const int leftBorderWidth = clientPts.left - wndPts.left;
					const int rightBorderWidth = wndPts.right - clientPts.right;
					const int topBorderHeight = clientPts.top - wndPts.top;
					const int bottomBorderHeight = wndPts.bottom - clientPts.bottom;

					MoveWindow(hwnd, windowPos.x, windowPos.y, leftBorderWidth + rightBorderWidth + bmpInfo.bmWidth, topBorderHeight + bottomBorderHeight + bmpInfo.bmHeight, TRUE);
					BitBlt(vDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, tmpDC, 0, 0, SRCCOPY);
				}

				SelectObject(tmpDC, oldBmp);
				DeleteDC(tmpDC);
				DeleteObject(reinterpret_cast<package*>(lParam)->bmpData);
			}

			tool = UM_TRANSFORM;
			InvalidateRect(hwnd, nullptr, TRUE);
			return 0;
		}

		case WM_CLOSE:
			return 1;

		case WM_DESTROY:
			DeleteObject(clipRgn);

			DeleteObject(SelectObject(vDC, oldvBmp));
			DeleteDC(vDC);

			DeleteObject(SelectObject(vToolDC, oldvToolBmp));
			DeleteDC(vToolDC);

			DeleteObject(SelectObject(appDC, oldAppBmp));
			DeleteDC(appDC);

			return 0;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}