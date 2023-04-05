#include<windows.h>

#include<cinttypes>
#include<string>

#include"constants.h"
#include"resource.h"
#include"file.h"

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND drawAreaHWnd, toolWnd, colorWnd, propWnd;
	static extLogPen pen, savePen;
	static OPENFILENAME commDlg;
	static data pointers;
	static package clipboardData;
	static bool saveState, fileLoaded;

	switch (msg)
	{
		case WM_CREATE:
		{
			RECT wndPts;
			GetClientRect(hwnd, &wndPts);
			const uint16_t WndWidth = wndPts.right - wndPts.left;
			const uint16_t WndHeight = wndPts.bottom - wndPts.top;

			const uint16_t toolAreaWidth = 59;
			const uint16_t toolAreaHeight = 143;
			const uint16_t colorAreaWidth = 200;
			const uint16_t colorAreaHeight = 200;
			const uint16_t propertiesAreaWidth = 200;
			const uint16_t propertiesAreaHeight = 50;

			const uint16_t drawAreaW = 500;
			const uint16_t drawAreaH = 500;

			saveState = true;
			pen = { PS_GEOMETRIC | PS_SOLID, 1, {BS_SOLID, 0x0} };
			savePen = pen;

			initFileCommDlg(&commDlg, hwnd);
			fileLoaded = false;

			drawAreaHWnd = CreateWindowEx(0, L"drawArea", nullptr, WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_SIZEBOX | WS_CAPTION | WS_SYSMENU, (WndWidth - drawAreaW) / 2, (WndHeight - drawAreaH) / 2, drawAreaW, drawAreaH,
				hwnd, reinterpret_cast<HMENU>(1), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), &pointers);

			toolWnd = CreateWindowEx(0, L"toolArea", nullptr, WS_DLGFRAME | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, 0, 0, toolAreaWidth, toolAreaHeight,
				hwnd, reinterpret_cast<HMENU>(2), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr);

			colorWnd = CreateWindowEx(0, L"colorArea", nullptr, WS_DLGFRAME | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, 0, WndHeight - colorAreaHeight, colorAreaWidth, colorAreaHeight,
				hwnd, reinterpret_cast<HMENU>(3), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr);

			propWnd = CreateWindowEx(0, L"propertiesArea", nullptr, WS_DLGFRAME | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, WndWidth - propertiesAreaWidth, 0, propertiesAreaWidth, propertiesAreaHeight,
				hwnd, reinterpret_cast<HMENU>(4), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr);

			return 0;
		}

		case WM_SIZE:
		{
			RECT wndPts;
			GetWindowRect(drawAreaHWnd, &wndPts);
			uint32_t drawAreaW = wndPts.right - wndPts.left;
			uint32_t drawAreaH = wndPts.bottom - wndPts.top;

			const uint16_t colorAreaWidth = 200;
			const uint16_t colorAreaHeight = 200;
			const uint16_t propertiesAreaWidth = 200;
			const uint16_t propertiesAreaHeight = 50;

			MoveWindow(drawAreaHWnd, (LOWORD(lParam) - drawAreaW) / 2, (HIWORD(lParam) - drawAreaH) / 2, drawAreaW, drawAreaH, TRUE);
			MoveWindow(colorWnd, 0, HIWORD(lParam) - colorAreaHeight, colorAreaWidth, colorAreaHeight, TRUE);
			MoveWindow(propWnd, LOWORD(lParam) - propertiesAreaWidth, 0, propertiesAreaWidth, propertiesAreaHeight, TRUE);

			return 0;
		}

		case WM_USER:
		{
			if (LOWORD(wParam) & UM_PROPERTIES)
			{
				if (LOWORD(wParam) == UMF_ALL)
				{
					pen = *reinterpret_cast<extLogPen*>(lParam);
				}
				else
				{
					if (LOWORD(wParam) & UMF_FLAGS)
						pen.flags = wParam & UMF_PACKED ? reinterpret_cast<extLogPen*>(lParam)->flags : lParam;

					if (LOWORD(wParam) & UMF_WIDTH)
						pen.width = wParam & UMF_PACKED ? reinterpret_cast<extLogPen*>(lParam)->width : static_cast<LONG>(lParam);

					if (LOWORD(wParam) & UMF_STYLE)
						pen.brushData.lbStyle = wParam & UMF_PACKED ? reinterpret_cast<extLogPen*>(lParam)->brushData.lbStyle : lParam;

					if (LOWORD(wParam) & UMF_COLOR)
						pen.brushData.lbColor = wParam & UMF_PACKED ? reinterpret_cast<extLogPen*>(lParam)->brushData.lbColor : lParam;

					if (LOWORD(wParam) & UMF_HATCH)
						pen.brushData.lbHatch = wParam & UMF_PACKED ? reinterpret_cast<extLogPen*>(lParam)->brushData.lbHatch : lParam;

					if (LOWORD(wParam) & UMF_DATA)
						pen.data = wParam & UMF_PACKED ? reinterpret_cast<extLogPen*>(lParam)->data : *reinterpret_cast<std::vector<DWORD>*>(lParam);
				}
			}

			switch (wParam & 0xFFFF0000)
			{
				case UMF_SAVESTATE:
					saveState = true;
					pen = savePen;
					SendMessage(propWnd, WM_USER, UMF_UPDATEVALUE, savePen.width);
					SendMessage(colorWnd, WM_USER, UMF_UPDATEVALUE, savePen.brushData.lbColor);
					break;

				case UMF_DONOTSAVE:
					saveState = false;
					break;
			}

			if (saveState == true)
				savePen = pen;

			SendMessage(drawAreaHWnd, WM_USER, wParam, reinterpret_cast<LPARAM>(&pen));
			SendMessage(propWnd, WM_USER + 1, wParam & 0xFF, 0);
			return 0;
		}

			case WM_COMMAND:
			{
				//send message back to toolAreaProc
				if (HIWORD(wParam) == LBN_SELCHANGE && LOWORD(wParam) == 0x40)
				{
					SendMessage(toolWnd, WM_COMMAND, wParam, lParam);
					return 0;
				}

				switch (LOWORD(wParam))
				{
					case ID_FILE_NEW:
						if (MessageBox(hwnd, L"Are you sure you want to create a new file?\n(all unsaved changes will be lost)", L"New File", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
						{
							SendMessage(drawAreaHWnd, WM_USER + 1, UM_NEWFILE, 0);
							fileLoaded = false;
						}
						break;

					case ID_FILE_OPEN:
					{
						std::wstring filePath, fileName;

						if (openFileCommDlg(hwnd, &commDlg, &filePath, &fileName))
						{
							if (MessageBox(hwnd, L"Are you sure you want to open this file?\n(all unsaved changes will be lost)", L"Open File", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
							{
								HBITMAP bmpFile = reinterpret_cast<HBITMAP>(LoadImage(NULL, filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
								if (bmpFile == NULL)
								{
									DebugWin(L"Error!!", L"couldn't load %s, on\n%s\nError:%d", fileName.c_str(), filePath.c_str(), GetLastError());
									break;
								}
								SendMessage(drawAreaHWnd, WM_USER + 1, UM_NEWFILE | UM_PASTEIMAGE, reinterpret_cast<LPARAM>(&bmpFile));		//drawArea is responsible for getting rid of the bitmap handler
								fileLoaded = true;
							}
						}
						break;
					}

					case ID_FILE_SAVE:
					case ID_FILE_SAVEAS:
					{
						std::wstring filePath, fileName;

						if (saveFileCommDlg(hwnd, &commDlg, &filePath, &fileName))
						{
							HDC drawAreaDC = GetDC(drawAreaHWnd);
							if (saveFile(drawAreaDC, filePath, reinterpret_cast<HBITMAP>(GetCurrentObject(*(pointers.memoryDC), OBJ_BITMAP))) == TRUE)
								DebugWin(L"error", L"Unable to open\n%s", filePath.c_str());
							ReleaseDC(drawAreaHWnd, drawAreaDC);
						}
						break;
					}

					case ID_FILE_EXIT:
						SendMessage(hwnd, WM_CLOSE, 0, 0);
						break;

					case ID_PASTE:
					{
						OpenClipboard(hwnd);
						if (IsClipboardFormatAvailable(CF_BITMAP))
						{
							HBITMAP copyBmp = reinterpret_cast<HBITMAP>(GetClipboardData(CF_BITMAP));

							HDC DC = GetDC(drawAreaHWnd);
							HDC copyMemoryDC = CreateCompatibleDC(DC);
							ReleaseDC(drawAreaHWnd, DC);

							HBITMAP oldBmp = reinterpret_cast<HBITMAP>(SelectObject(copyMemoryDC, copyBmp));
							clipboardData.bmpData = copyBitmap(drawAreaHWnd, copyMemoryDC);

							uint32_t format = RegisterClipboardFormat(L"regionarr");
							if (IsClipboardFormatAvailable(format))
							{
								HGLOBAL memHand = reinterpret_cast<HGLOBAL>(GetClipboardData(format));
								LPVOID data = GlobalLock(memHand);
								clipboardData.rgnData.reserve(GlobalSize(memHand)/sizeof(POINT));
								for (uint32_t i = 0; i < GlobalSize(memHand)/sizeof(POINT); ++i)
								{
									clipboardData.rgnData.push_back(reinterpret_cast<POINT*>(data)[i]);
								}
								GlobalUnlock(memHand);
							}
							else
							{
								clipboardData.rgnData.clear();
							}

							SendMessage(drawAreaHWnd, WM_USER + 1, UM_PASTEIMAGE | UMF_BUFFER, reinterpret_cast<LPARAM>(&clipboardData));

							SelectObject(copyMemoryDC, oldBmp);
							DeleteDC(copyMemoryDC);
						}
						CloseClipboard();
						break;
					}

					case ID_COPY:
					{
						HGLOBAL memHand = nullptr;
						if (pointers.rgnData->size())
						{
							memHand = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, pointers.rgnData->size()*sizeof(POINT));
							if (memHand)
							{
								LPVOID data = GlobalLock(memHand);
								for (uint32_t i = 0; i < pointers.rgnData->size(); ++i)
								{
									reinterpret_cast<POINT*>(data)[i] = pointers.rgnData->at(i);
								}
								GlobalUnlock(memHand);
							}
						}

						OpenClipboard(hwnd);
						EmptyClipboard();

						SetClipboardData(CF_BITMAP, copyBitmap(drawAreaHWnd, *pointers.memoryDC));

						if (pointers.rgnData->size())
						{
							uint32_t format = RegisterClipboardFormat(L"regionarr");
							SetClipboardData(format, memHand);
						}

						CloseClipboard();
						break;
					}

					default:
						break;
				}
			}
				return 0;

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		}
}