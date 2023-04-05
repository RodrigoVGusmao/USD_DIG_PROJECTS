#include<windows.h>

#include<cinttypes>
#include<string>
#include<vector>

#include"constants.h"
#include"dialog.h"
#include"debug.h"

int getDigitsNum(int value)
{
	if (value / 10 == 0)
		return 1;
	else
		return getDigitsNum(value / 10) + 1;
}

LRESULT CALLBACK toolWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static std::vector<HWND> buttons;
	static HWND listBox;
	static uint8_t tool;

	RECT wndPts;
	GetClientRect(hwnd, &wndPts);

	const uint16_t wndWidth = wndPts.right - wndPts.left;
	const uint16_t wndHeight = wndPts.bottom - wndPts.top;

	const uint8_t btnYCount = 3;

	switch (msg)
	{
		case WM_CREATE:
		{
			listBox = CreateWindowEx(0, L"listBox", nullptr, WS_CHILD | WS_BORDER | LBS_NOTIFY, 0, 0, 0, 0,
				GetParent(hwnd), reinterpret_cast<HMENU>(0x40), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr);

			SendMessage(listBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"free shape"));
			SendMessage(listBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"rectangle"));
			SendMessage(listBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"ellipse"));
			SendMessage(listBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"vector"));
			SendMessage(listBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"fill"));

			for (uint16_t j = 0; j < btnYCount; ++j)
			{
				for (uint16_t i = 0; i < 2; ++i)
				{
					buttons.push_back(CreateWindowEx(0, L"Button", nullptr, WS_VISIBLE | WS_CHILD | BS_ICON | BS_NOTIFY,
						(wndWidth / 2 - 25) / 2 + i * wndWidth / 2, (wndHeight / btnYCount - 25) / 2 + j * wndHeight / btnYCount, 25, 25,
						hwnd, reinterpret_cast<HMENU>(i + j*2), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
					SendMessage(buttons[i + j*2], BM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(LoadIcon(nullptr, IDI_EXCLAMATION)));
				}
			}
			SendMessage(GetParent(hwnd), WM_USER, UMF_SAVESTATE | UM_PEN, PS_SOLID);
			return 0;
		}
		
		case WM_COMMAND:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				switch (LOWORD(wParam))
				{
					default:
					case 0:
						SendMessage(GetParent(hwnd), WM_USER, UMF_SAVESTATE | UM_PEN, PS_SOLID);
						break;

					case 1:
						SendMessage(GetParent(hwnd), WM_USER, UMF_DONOTSAVE | UM_PEN, PS_SOLID);
						SendMessage(GetDlgItem(GetParent(hwnd), 3), WM_USER, UM_PROPERTIES | UMF_COLOR, 0x00FFFFFF);
						SendMessage(GetDlgItem(GetParent(hwnd), 4), WM_USER, UM_PROPERTIES | UMF_WIDTH, 15);
						break;

					case 2:
						SendMessage(GetParent(hwnd), WM_USER, UMF_DONOTSAVE | UM_PATH | (RGN_COPY << 8), PS_NULL);
						break;

					case 3:
						SendMessage(GetParent(hwnd), WM_USER, UMF_DONOTSAVE | UM_TRANSFORM, PS_NULL);
						break;

					case 4:
						SendMessage(GetParent(hwnd), WM_USER, UM_TEST, PS_SOLID);
						break;
				}
			}
			else if (HIWORD(wParam) == BN_DBLCLK)
			{
				switch (LOWORD(wParam))
				{
					case 0:
						MoveWindow(listBox, (wndWidth / 2 - 25) / 2 + 25, (wndHeight / btnYCount - 25) / 2, 80, 120, TRUE);
						ShowWindow(listBox, SW_SHOW);
						BringWindowToTop(listBox);
						tool = UMF_SAVESTATE | UM_PEN;
						break;

					case 2:
						MoveWindow(listBox, (wndWidth / 2 - 25) / 2 + 25, (wndHeight / btnYCount - 25) / 2 + wndHeight / btnYCount, 80, 120, TRUE);
						ShowWindow(listBox, SW_SHOW);
						BringWindowToTop(listBox);
						tool = UMF_DONOTSAVE | UM_PATH | (RGN_COPY << 8);
						break;
				}
			}
			else if (HIWORD(wParam) == LBN_SELCHANGE) //message sent by wndProc
			{
				uint8_t itemIndex = SendMessage(reinterpret_cast<HWND>(lParam), LB_GETCURSEL, 0, 0);
				MoveWindow(listBox, 0, 0, 0, 0, TRUE);
				ShowWindow(listBox, SW_HIDE);

				SendMessage(GetParent(hwnd), WM_USER, tool | (1 << (itemIndex+11) & 0xF000), PS_SOLID);
			}
			return 0;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK colorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static std::vector<HWND> scrollbar;
	static std::vector<HWND> labels;
	static std::vector<uint8_t> scrollPos;
	static HWND colorRect;
	static HBRUSH colorRectRGB;

	switch (msg)
	{
		case WM_CREATE:
		{
			RECT wndPts;
			GetClientRect(hwnd, &wndPts);
			
			const uint16_t wndWidth = wndPts.right - wndPts.left;
			const uint16_t wndHeight = wndPts.bottom - wndPts.top;

			const uint16_t scrollbarCount = 3;
			const uint16_t scrollbarWidth = 12;
			const uint16_t colorBarHeight = 25;

			TEXTMETRIC tm;
			HDC dc = GetDC(hwnd);
			GetTextMetrics(dc, &tm);
			ReleaseDC(hwnd, dc);

			colorRect = CreateWindowEx(0, L"static", nullptr, WS_VISIBLE | WS_CHILD, 7, wndHeight - colorBarHeight - 7, wndWidth - 14, colorBarHeight,
				hwnd, reinterpret_cast<HMENU>(scrollbarCount), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr);

			colorRectRGB = CreateSolidBrush(0);

			scrollPos.resize(3, ~0);
			for (uint8_t i = 0; i < scrollbarCount; i++)
			{
				scrollbar.push_back(CreateWindowEx(0, L"Scrollbar", nullptr, WS_VISIBLE | WS_CHILD | SBS_VERT,
					(wndWidth / scrollbarCount - scrollbarWidth) / 2 + i * wndWidth / scrollbarCount, 14 + tm.tmHeight, scrollbarWidth, wndHeight - colorBarHeight - tm.tmHeight - 28,
					hwnd, reinterpret_cast<HMENU>(i), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));

				labels.push_back(CreateWindowEx(0, L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER,
					(wndWidth / scrollbarCount - 4*tm.tmAveCharWidth) / 2 + i * wndWidth / scrollbarCount, 7, 4 * tm.tmAveCharWidth, tm.tmHeight,
					hwnd, reinterpret_cast<HMENU>(i), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));

				SetScrollRange(scrollbar.back(), SB_CTL, 0, 255, FALSE);
				SetScrollPos(scrollbar.back(), SB_CTL, scrollPos[i], FALSE);
			}
			return 0;
		}

		case WM_VSCROLL:
		{
			uint8_t id = GetWindowLongPtr(reinterpret_cast<HWND>(lParam), GWLP_ID);
			switch (LOWORD(wParam))
			{
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
					scrollPos[id] = HIWORD(wParam);
					break;

				default:
					break;
			}
			SetScrollPos(scrollbar[id], SB_CTL, scrollPos[id], TRUE);

			std::wstring buffer;
			buffer.resize(4);
			swprintf(const_cast<wchar_t*>(buffer.c_str()), buffer.size(), L"%d", static_cast<uint8_t>(~scrollPos[id]));
			SetWindowText(labels[id], buffer.c_str());

			DeleteObject(colorRectRGB);
			colorRectRGB = CreateSolidBrush(RGB(~scrollPos[0], ~scrollPos[1], ~scrollPos[2]));
			InvalidateRect(colorRect, nullptr, TRUE);

			SendMessage(GetParent(hwnd), WM_USER, UM_PROPERTIES | UMF_COLOR, RGB(~scrollPos[0], ~scrollPos[1], ~scrollPos[2]));
			return 0;
		}

		case WM_CTLCOLORSTATIC:
		{
			if (reinterpret_cast<HWND>(lParam) == colorRect)
				return reinterpret_cast<INT_PTR>(colorRectRGB);
		}

		case WM_CTLCOLOREDIT:
		{
			uint8_t id = GetWindowLongPtr(reinterpret_cast<HWND>(lParam), GWLP_ID);
			SetTextColor(reinterpret_cast<HDC>(wParam), 0xFF << ((id) * 8));
			return reinterpret_cast<INT_PTR>(GetStockObject(WHITE_BRUSH));
		}

		case WM_USER:
			scrollPos[0] = ~(lParam) >> 0x0;
			scrollPos[1] = ~(lParam) >> 0x8;
			scrollPos[2] = ~(lParam) >> 0x10;

			for (uint8_t i = 0; i < 3; ++i)
			{
				SetScrollPos(scrollbar[i], SB_CTL, scrollPos[i], TRUE);

				std::wstring buffer;
				buffer.resize(4);
				swprintf(const_cast<wchar_t*>(buffer.c_str()), buffer.size(), L"%d", static_cast<uint8_t>(~scrollPos[i]));
				SetWindowText(labels[i], buffer.c_str());
			}

			DeleteObject(colorRectRGB);
			colorRectRGB = CreateSolidBrush(RGB(~scrollPos[0], ~scrollPos[1], ~scrollPos[2]));
			InvalidateRect(colorRect, nullptr, TRUE);

			if(HIWORD(wParam) == false)
				SendMessage(GetParent(hwnd), WM_USER, UM_PROPERTIES | UMF_COLOR, lParam);
			return 0;

		case WM_COMMAND:
		{
			if (HIWORD(wParam) == EN_UPDATE)
			{
				std::wstring windowText;
				windowText.resize(GetWindowTextLength(reinterpret_cast<HWND>(lParam)) + 1);
				GetWindowText(reinterpret_cast<HWND>(lParam), const_cast<wchar_t*>(windowText.c_str()), windowText.size());
				scrollPos[LOWORD(wParam)] = ~_wtoi(windowText.c_str());
				SetScrollPos(scrollbar[LOWORD(wParam)], SB_CTL, scrollPos[LOWORD(wParam)], TRUE);

				DeleteObject(colorRectRGB);
				colorRectRGB = CreateSolidBrush(RGB(~scrollPos[0], ~scrollPos[1], ~scrollPos[2]));
				InvalidateRect(colorRect, nullptr, TRUE);

				SendMessage(GetParent(hwnd), WM_USER, UM_PROPERTIES | UMF_COLOR, RGB(~scrollPos[0], ~scrollPos[1], ~scrollPos[2]));
			}

			return 0;
		}

		case WM_DESTROY:
			DeleteObject(colorRectRGB);
			return 0;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void clearToolOptions(std::vector<HANDLE> options)
{
	for (uint16_t i = 0; i < options.size(); ++i)
	{
		SendMessage(static_cast<HWND>(options[i]), WM_CLOSE, 0, 0);
	}
}

LRESULT CALLBACK toolPropertiesWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static std::vector<HANDLE> properties;
	static HWND valueField;
	static HWND scrollbar;
	static HWND sizeText;
	static uint16_t scrollPos;
	static uint8_t tool;
	static extLogPen penProp;
	static uint16_t wndWidth, wndHeight;

	switch (msg)
	{
		case WM_CREATE:
		{
			RECT wndPts;
			GetWindowRect(hwnd, &wndPts);

			wndWidth = wndPts.right - wndPts.left;
			wndHeight = wndPts.bottom - wndPts.top;

			const uint16_t scrollbarHeight = 12;

			TEXTMETRIC tm;
			HDC dc = GetDC(hwnd);
			GetTextMetrics(dc, &tm);
			ReleaseDC(hwnd, dc);

			sizeText = CreateWindowEx(0, L"static", L"Size:", WS_VISIBLE | WS_CHILD | SS_LEFT, 7, (wndHeight / 2 - tm.tmHeight+4) / 2, 5 * tm.tmAveCharWidth, tm.tmHeight,
				hwnd, reinterpret_cast<HMENU>(1), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr);

			valueField = CreateWindowEx(0, L"edit", L"1", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
				14 + 5 * tm.tmAveCharWidth, (wndHeight / 2 - tm.tmHeight) / 2, 5 * tm.tmAveCharWidth, tm.tmHeight+4,
				hwnd, reinterpret_cast<HMENU>(2), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr);

			scrollbar = CreateWindowEx(0, L"scrollbar", nullptr, WS_VISIBLE | WS_CHILD | SBS_HORZ, 7, (3 * wndHeight / 2 - scrollbarHeight) / 2, wndWidth - 14, scrollbarHeight,
				hwnd, reinterpret_cast<HMENU>(3), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr);

			scrollPos = 1;
			SetScrollRange(scrollbar, SB_CTL, 1, 999, FALSE);
			SetScrollPos(scrollbar, SB_CTL, scrollPos, FALSE);

			SendMessage(hwnd, WM_USER + 1, UM_PEN, 0);

			return 0;
		}

		case WM_HSCROLL:
		{
			switch (LOWORD(wParam))
			{
				case SB_THUMBTRACK:
				case SB_THUMBPOSITION:
					scrollPos = HIWORD(wParam);
					break;
				default:
					break;
			}
			SetScrollPos(scrollbar, SB_CTL, scrollPos, TRUE);

			std::wstring value;
			value.resize(getDigitsNum(scrollPos)+1);
			swprintf(const_cast<wchar_t*>(value.c_str()), value.size(), L"%d", scrollPos);
			SetWindowText(valueField, value.c_str());

			penProp.width = scrollPos;
			SendMessage(GetParent(hwnd), WM_USER, UM_PROPERTIES | UMF_WIDTH, scrollPos);
			return 0;
		}

		case WM_USER:
		{
			scrollPos = lParam;
			SetScrollPos(scrollbar, SB_CTL, scrollPos, TRUE);

			std::wstring value;
			value.resize(getDigitsNum(scrollPos) + 1);
			swprintf(const_cast<wchar_t*>(value.c_str()), value.size(), L"%d", scrollPos);
			SetWindowText(valueField, value.c_str());

			if (HIWORD(wParam) == false)
			{
				penProp.width = scrollPos;
				SendMessage(GetParent(hwnd), WM_USER, UM_PROPERTIES | UMF_WIDTH, lParam);
			}
			return 0;
		}

		case WM_USER+1:
		{
			if (wParam & 0xFE && wParam ^ tool)
			{
				RECT wndPts;
				GetWindowRect(hwnd, &wndPts);
				ScreenToClient(GetParent(hwnd), reinterpret_cast<POINT*>(&wndPts));

				TEXTMETRIC tm;
				HDC dc = GetDC(hwnd);
				GetTextMetrics(dc, &tm);
				ReleaseDC(hwnd, dc);

				clearToolOptions(properties);
				properties.clear();

				switch (wParam & 0xFF)
				{
					case UM_PEN:
						MoveWindow(hwnd, wndPts.left, wndPts.top, wndWidth, wndHeight + tm.tmHeight * 40, TRUE);
						properties.reserve(21);
						properties.push_back(CreateWindowEx(0, L"static", L"Joint Type:", WS_VISIBLE | WS_CHILD | SS_LEFT | WS_DISABLED, 7, wndHeight + tm.tmHeight / 2, tm.tmAveCharWidth * 12, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(0x17), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"static", nullptr, WS_VISIBLE | WS_CHILD | SS_ETCHEDFRAME,
							tm.tmAveCharWidth * 12, wndHeight + tm.tmHeight, wndWidth - tm.tmAveCharWidth * 13, 1,
							hwnd, reinterpret_cast<HMENU>(0x18), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Round", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 3 * tm.tmHeight / 2, wndWidth / 2, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_JOIN_ROUND), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Bevel", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7 + wndWidth / 2, wndHeight + 3 * tm.tmHeight / 2, wndWidth / 2, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_JOIN_BEVEL), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Miter", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 3 * tm.tmHeight, wndWidth / 2, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_JOIN_MITER), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						SendMessage(static_cast<HWND>(properties[2]), BM_SETCHECK, 1, 0);

						properties.push_back(CreateWindowEx(0, L"static", L"End Type:", WS_VISIBLE | WS_CHILD | SS_LEFT | WS_DISABLED, 7, wndHeight + 5 * tm.tmHeight, tm.tmAveCharWidth * 10, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(0x27), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"static", nullptr, WS_VISIBLE | WS_CHILD | SS_ETCHEDFRAME,
							tm.tmAveCharWidth * 11, wndHeight + 11 * tm.tmHeight / 2, wndWidth - tm.tmAveCharWidth * 12, 1,
							hwnd, reinterpret_cast<HMENU>(0x28), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Round", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 6 * tm.tmHeight, wndWidth / 2, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_ENDCAP_ROUND), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Square", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7 + wndWidth / 2, wndHeight + 6 * tm.tmHeight, wndWidth / 2, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_ENDCAP_SQUARE), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Flat", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 15 * tm.tmHeight / 2, wndWidth / 2, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_ENDCAP_FLAT), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						SendMessage(static_cast<HWND>(properties[7]), BM_SETCHECK, 1, 0);

						properties.push_back(CreateWindowEx(0, L"static", L"Style:", WS_VISIBLE | WS_CHILD | SS_LEFT | WS_DISABLED, 7, wndHeight + 19 * tm.tmHeight / 2, tm.tmAveCharWidth * 7, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(0x37), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"static", nullptr, WS_VISIBLE | WS_CHILD | SS_ETCHEDFRAME,
							tm.tmAveCharWidth * 7, wndHeight + 10 * tm.tmHeight, wndWidth - tm.tmAveCharWidth * 8, 1,
							hwnd, reinterpret_cast<HMENU>(0x38), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", nullptr, WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 11 * tm.tmHeight, 2 * tm.tmAveCharWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_SOLID), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", nullptr, WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7 + wndWidth / 2, wndHeight + 11 * tm.tmHeight, 2 * tm.tmAveCharWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_DASH), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", nullptr, WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 25 * tm.tmHeight / 2, 2 * tm.tmAveCharWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_DOT), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", nullptr, WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7 + wndWidth / 2, wndHeight + 25 * tm.tmHeight / 2, 2 * tm.tmAveCharWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_DASHDOT), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", nullptr, WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 14 * tm.tmHeight, 2 * tm.tmAveCharWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_DASHDOTDOT), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Custom", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7 + wndWidth / 2, wndHeight + 14 * tm.tmHeight, wndWidth / 2, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(PS_USERSTYLE), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						SendMessage(static_cast<HWND>(properties[12]), BM_SETCHECK, 1, 0);

						properties.push_back(CreateWindowEx(0, L"static", L"Brush:", WS_VISIBLE | WS_CHILD | SS_LEFT | WS_DISABLED, 7, wndHeight + 16 * tm.tmHeight, tm.tmAveCharWidth * 7, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(0x47), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"static", nullptr, WS_VISIBLE | WS_CHILD | SS_ETCHEDFRAME,
							tm.tmAveCharWidth * 8, wndHeight + 33 * tm.tmHeight/2, wndWidth - tm.tmAveCharWidth * 9, 1,
							hwnd, reinterpret_cast<HMENU>(0x48), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"static", nullptr, WS_VISIBLE | WS_CHILD | SS_OWNERDRAW | SS_NOTIFY | SS_SUNKEN,
							wndWidth/4, wndHeight + 35 * tm.tmHeight/2, wndWidth/2, wndWidth/2,
							hwnd, reinterpret_cast<HMENU>(0xFFFF), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));

						EnableWindow(sizeText, 1);
						EnableWindow(valueField, 1);
						EnableWindow(scrollbar, 1);

						penProp.flags = PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND;
						penProp.brushData.lbStyle = BS_SOLID;
						penProp.brushData.lbColor = 0x0;
						break;

					case UM_PATH:
						MoveWindow(hwnd, wndPts.left, wndPts.top, wndWidth, wndHeight + tm.tmHeight * 9, TRUE);
						properties.reserve(7);
						properties.push_back(CreateWindowEx(0, L"static", L"Mode:", WS_VISIBLE | WS_CHILD | SS_LEFT | WS_DISABLED, 7, wndHeight + 0.5f*tm.tmHeight, 6 * tm.tmAveCharWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(0x17), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"static", nullptr, WS_VISIBLE | WS_CHILD | SS_ETCHEDFRAME,
							7*tm.tmAveCharWidth, wndHeight + tm.tmHeight, wndWidth - 8 * tm.tmAveCharWidth, 1,
							hwnd, reinterpret_cast<HMENU>(0x18), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Copy Region", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 1.5f * tm.tmHeight, wndWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(RGN_COPY), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Add Region", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 3 * tm.tmHeight, wndWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(RGN_OR), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Diff Region", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 4.5f * tm.tmHeight, wndWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(RGN_DIFF), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Intersect Region", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 6*tm.tmHeight, wndWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(RGN_AND), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Invert Region", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 7.5f * tm.tmHeight, wndWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(RGN_XOR), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						SendMessage(static_cast<HWND>(properties[2]), BM_SETCHECK, 1, 0);

						EnableWindow(sizeText, 0);
						EnableWindow(valueField, 0);
						EnableWindow(scrollbar, 0);
						break;

					case UM_TRANSFORM:
						MoveWindow(hwnd, wndPts.left, wndPts.top, wndWidth, wndHeight + tm.tmHeight * 6, TRUE);
						properties.reserve(5);
						properties.push_back(CreateWindowEx(0, L"static", L"Mode:", WS_VISIBLE | WS_CHILD | SS_LEFT | WS_DISABLED, 7, wndHeight + 0.5f * tm.tmHeight, 6 * tm.tmAveCharWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(0x17), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"static", nullptr, WS_VISIBLE | WS_CHILD | SS_ETCHEDFRAME,
							7 * tm.tmAveCharWidth, wndHeight + tm.tmHeight, wndWidth - 8 * tm.tmAveCharWidth, 1,
							hwnd, reinterpret_cast<HMENU>(0x18), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Move", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 1.5f * tm.tmHeight, wndWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(UMF_MOVE), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Stretch", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 3 * tm.tmHeight, wndWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(UMF_STRETCH), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						properties.push_back(CreateWindowEx(0, L"button", L"Rotate", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 7, wndHeight + 4.5f * tm.tmHeight, wndWidth, tm.tmHeight,
							hwnd, reinterpret_cast<HMENU>(UMF_ROTATE), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), nullptr));
						SendMessage(static_cast<HWND>(properties[2]), BM_SETCHECK, 1, 0);

						EnableWindow(sizeText, 0);
						EnableWindow(valueField, 0);
						EnableWindow(scrollbar, 0);
						break;

					default:
						MoveWindow(hwnd, wndPts.left, wndPts.top, wndWidth, wndHeight, TRUE);
						EnableWindow(sizeText, 0);
						EnableWindow(valueField, 0);
						EnableWindow(scrollbar, 0);
						tool = 0;
						break;
				}

				tool = wParam;
				InvalidateRect(hwnd, nullptr, TRUE);
			}

			return 0;
		}

		case WM_COMMAND:
		{
			if (HIWORD(wParam) == EN_UPDATE)
			{
				std::wstring windowText;
				windowText.resize(GetWindowTextLength(reinterpret_cast<HWND>(lParam)) + 1);
				GetWindowText(reinterpret_cast<HWND>(lParam), const_cast<wchar_t*>(windowText.c_str()), windowText.size());
				scrollPos = _wtoi(windowText.c_str());
				SetScrollPos(scrollbar, SB_CTL, scrollPos, TRUE);

				penProp.width = scrollPos;

				SendMessage(GetParent(hwnd), WM_USER, UM_PROPERTIES | UMF_WIDTH, scrollPos);
			}
			else if (HIWORD(wParam) == BN_CLICKED && tool == UM_PEN)
			{
				if (LOWORD(wParam) == 0xFFFF)
				{

				}
				else if (LOWORD(wParam) & 0xFF || reinterpret_cast<HANDLE>(lParam) == properties[12])
				{
					penProp.flags &= 0xFFFFFF00;
					penProp.flags |= LOWORD(wParam);

					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[13] ? properties[12] : properties[13]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[14] ? properties[12] : properties[14]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[15] ? properties[12] : properties[15]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[16] ? properties[12] : properties[16]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[17] ? properties[12] : properties[17]), BM_SETCHECK, 0, 0);

					if (reinterpret_cast<HANDLE>(lParam) == properties[17])
					{
						DialogBoxParam(reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)), MAKEINTRESOURCE(IDD_CUSTOM_PEN), hwnd, customPenProc, reinterpret_cast<LPARAM>(&penProp.data));
					}
					else
					{
						penProp.data.clear();
					}
				}
				else if (LOWORD(wParam) & 0xF00 || reinterpret_cast<HANDLE>(lParam) == properties[7])
				{
					penProp.flags &= 0xFFFFF0FF;
					penProp.flags |= LOWORD(wParam);

					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[8] ? properties[7] : properties[8]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[9] ? properties[7] : properties[9]), BM_SETCHECK, 0, 0);
				}
				else if (LOWORD(wParam) & 0xF000 || reinterpret_cast<HANDLE>(lParam) == properties[2])
				{
					penProp.flags &= 0xFFFF0FFF;
					penProp.flags |= LOWORD(wParam);

					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[3] ? properties[2] : properties[3]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[4] ? properties[2] : properties[4]), BM_SETCHECK, 0, 0);
				}

				SendMessage(reinterpret_cast<HWND>(lParam), BM_SETCHECK, 1, 0);
				SendMessage(GetParent(hwnd), WM_USER, UM_PROPERTIES | UMF_PENDATA | UMF_DATA, reinterpret_cast<LPARAM>(&penProp));
			}
			else if (HIWORD(wParam) == BN_CLICKED && tool == UM_PATH)
			{
				if (LOWORD(wParam) & 0xFF)
				{
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[3] ? properties[2] : properties[3]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[4] ? properties[2] : properties[4]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[5] ? properties[2] : properties[5]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[6] ? properties[2] : properties[6]), BM_SETCHECK, 0, 0);
				}

				SendMessage(reinterpret_cast<HWND>(lParam), BM_SETCHECK, 1, 0);
				SendMessage(GetParent(hwnd), WM_USER, UM_PATH | (LOWORD(wParam) << 8), 0);
			}
			else if (HIWORD(wParam) == BN_CLICKED && tool == UM_TRANSFORM)
			{
				if (LOWORD(wParam) & 0xFF)
				{
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[3] ? properties[2] : properties[3]), BM_SETCHECK, 0, 0);
					SendMessage(static_cast<HWND>(reinterpret_cast<HANDLE>(lParam) == properties[4] ? properties[2] : properties[4]), BM_SETCHECK, 0, 0);
				}

				SendMessage(reinterpret_cast<HWND>(lParam), BM_SETCHECK, 1, 0);
				SendMessage(GetParent(hwnd), WM_USER, UM_TRANSFORM | (LOWORD(wParam) << 8), 0);
			}
			InvalidateRect(hwnd, nullptr, TRUE);

			return 0;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(hwnd, &ps);
			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);

			if (tool == UM_PEN)
			{
				MoveToEx(hdc, 7 + 3 * tm.tmAveCharWidth, wndHeight + 23 * tm.tmHeight / 2, nullptr);
				SelectObject(hdc, CreatePen(PS_SOLID, 1, 0x0));
				LineTo(hdc, wndWidth / 2 - tm.tmAveCharWidth, wndHeight + 23 * tm.tmHeight / 2);

				MoveToEx(hdc, 7 + 3 * tm.tmAveCharWidth + wndWidth / 2, wndHeight + 23 * tm.tmHeight / 2, nullptr);
				DeleteObject(SelectObject(hdc, CreatePen(PS_DASH, 1, 0x0)));
				LineTo(hdc, wndWidth - tm.tmAveCharWidth, wndHeight + 23 * tm.tmHeight / 2);

				MoveToEx(hdc, 7 + 3 * tm.tmAveCharWidth, wndHeight + 13 * tm.tmHeight, nullptr);
				DeleteObject(SelectObject(hdc, CreatePen(PS_DOT, 1, 0x0)));
				LineTo(hdc, wndWidth / 2 - tm.tmAveCharWidth, wndHeight + 13 * tm.tmHeight);

				MoveToEx(hdc, 7 + 3 * tm.tmAveCharWidth + wndWidth / 2, wndHeight + 13 * tm.tmHeight, nullptr);
				DeleteObject(SelectObject(hdc, CreatePen(PS_DASHDOT, 1, 0x0)));
				LineTo(hdc, wndWidth - tm.tmAveCharWidth, wndHeight + 13 * tm.tmHeight);

				MoveToEx(hdc, 7 + 3 * tm.tmAveCharWidth, wndHeight + 29 * tm.tmHeight/2, nullptr);
				DeleteObject(SelectObject(hdc, CreatePen(PS_DASHDOTDOT, 1, 0x0)));
				LineTo(hdc, wndWidth / 2 - tm.tmAveCharWidth, wndHeight + 29 * tm.tmHeight/2);

				BeginPath(hdc);

				MoveToEx(hdc, wndWidth / 8, wndHeight + 20 * tm.tmHeight + 5 * wndWidth/8, nullptr);
				LineTo(hdc, wndWidth / 4, wndHeight + 25 * tm.tmHeight + 5 * wndWidth / 8);
				LineTo(hdc, wndWidth / 2, wndHeight + 20 * tm.tmHeight + 5 * wndWidth / 8);
				LineTo(hdc, 3*wndWidth / 4, wndHeight + 25 * tm.tmHeight + 5 * wndWidth / 8);
				LineTo(hdc, 7*wndWidth / 8, wndHeight + 20 * tm.tmHeight + 5 * wndWidth / 8);

				EndPath(hdc);

				DeleteObject(SelectObject(hdc, ExtCreatePen(penProp.flags, scrollPos, &penProp.brushData, penProp.data.size(), penProp.data.empty() ? nullptr : penProp.data.data())));
				StrokePath(hdc);

				DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
			}
			EndPaint(hwnd, &ps);
			return 0;
		}

		case WM_CTLCOLORSTATIC:
			SetBkColor(reinterpret_cast<HDC>(wParam), 0x00FFFFFF);
			SetTextColor(reinterpret_cast<HDC>(wParam), 0x0);
			return reinterpret_cast<LRESULT>(GetStockObject(WHITE_BRUSH));

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}