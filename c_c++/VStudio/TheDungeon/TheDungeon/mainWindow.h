#include<string>
#include<windows.h>
#include<cinttypes>

#include"constants.h"
#include"Debug.h"

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HDC bufferDC, mapDC;
	static HBITMAP characterBmp, characterMask, testMapBmp;
	static HBITMAP oldmapBmp, oldBufferBmp;
	static RECT wndPt;
	static int8_t characterIndex;
	static POINTS characterPosition;

	switch (msg)
	{	
		case WM_CREATE:
		{
			GetClientRect(hwnd, &wndPt);
			OffsetRect(&wndPt, -wndPt.left, -wndPt.top);

			characterBmp = LoadBitmapW(reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), MAKEINTRESOURCEW(IDB_CHARACTER));
			characterMask = GetColorMask(hwnd, characterBmp, RGB(0xFF, 0x00, 0xFF));

			testMapBmp = LoadBitmapW(reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), MAKEINTRESOURCEW(IDB_MAP_TEST));

			HDC screenDC = GetDC(hwnd);
			mapDC = CreateCompatibleDC(screenDC);
			oldmapBmp = reinterpret_cast<HBITMAP>(SelectObject(mapDC, testMapBmp));

			bufferDC = CreateCompatibleDC(screenDC);
			oldBufferBmp = reinterpret_cast<HBITMAP>(SelectObject(bufferDC, CreateCompatibleBitmap(screenDC, wndPt.right, wndPt.bottom)));
			ReleaseDC(hwnd, screenDC);

			characterPosition = { 0, 0 };

			return 0;
		}

		case WM_COMMAND:
		{
			switch (HIWORD(wParam))
			{
				case 1:
				{
					switch (LOWORD(wParam))
					{
						case IDM_KEY_W:
							characterIndex = 6;
							characterPosition.y += 1;
							break;

						case IDM_KEY_A:
							characterIndex = -3;
							characterPosition.x += 1;
							break;

						case IDM_KEY_S:
							characterIndex = 0;
							characterPosition.y -= 1;
							break;

						case IDM_KEY_D:
							characterIndex = 3;
							characterPosition.x -= 1;
							break;
					}
					break;
				}
			}
			InvalidateRect(hwnd, nullptr, TRUE);

			return 0;
		}

		case WM_PAINT:
		{
			StretchBlt(bufferDC, (wndPt.right - (bmpClipPx * clipScale)) / 2 + characterPosition.x * 16 * clipScale, (wndPt.bottom - (bmpClipPx * clipScale)) / 2 + characterPosition.y * 16 * clipScale,
				160 * clipScale, 160 * clipScale, mapDC, 0, 0, 10, 10, SRCCOPY);

			ApplyTransparency(bufferDC, (wndPt.right - (bmpClipPx* clipScale))/2 + (characterIndex < 0 ? (clipScale * bmpClipPx - 1) : 0), (wndPt.bottom - (bmpClipPx* clipScale)) / 2,
				clipScale * bmpClipPx * (characterIndex < 0 ? -1 : 1), clipScale * bmpClipPx,
				(characterIndex % 3) * bmpClipPx, std::abs(characterIndex / 3) * bmpClipPx, bmpClipPx, bmpClipPx, characterBmp, characterMask);

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			BitBlt(hdc, 0, 0, wndPt.right, wndPt.bottom, bufferDC, 0, 0, SRCCOPY);
			EndPaint(hwnd, &ps);

			return 0;
		}

		case WM_DESTROY:
		{
			DeleteObject(characterBmp);
			DeleteObject(characterMask);

			DeleteObject(SelectObject(mapDC, oldmapBmp));
			DeleteDC(mapDC);

			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
}