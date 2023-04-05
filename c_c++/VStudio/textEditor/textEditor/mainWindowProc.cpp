#include "mainWindowProc.h"

#include<string>
#include<windowsx.h>
#include<emmintrin.h>

#include "errors.h"

#define UWM_GETTEXT WM_USER

template <typename T>
struct hWndsN {
	T textArea;
};

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//color
	static bool darkMode;
	static COLORREF bkgrndColor;
	static COLORREF userTextColor;
	static COLORREF sysTextColor;
	static HBRUSH bkgrndColorBrush = NULL;

	const unsigned int textAreaId = 1;

	const int maxHwnds = 1;
	union hWnds {
		 hWndsN<HWND> names;

		HWND hWnds[maxHwnds];
	} static hWnds;

	union hWndsSize {
		hWndsN<RECT> names;

		RECT hWnds[maxHwnds];
	} static hWndsRect;

	static int currentMargin;

	switch (msg)
	{
	case WM_CREATE:
	{
		DWORD type;
		DWORD value;
		DWORD count = 4;

		/*LSTATUS st = RegGetValue(HKEY_CURRENT_USER,
			TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"), TEXT("AppsUseLightTheme"),
			RRF_RT_DWORD, &type, &value, &count);
		if (st != ERROR_SUCCESS || type != REG_DWORD)
		{
			MessageBox(NULL, ERROR_REG_LIGHT_MODE_MSG, TEXT("Error!"), MB_ICONERROR | MB_OK);
			return 0;
		}
		
		if (value == 0)
		{
			bkgrndColor = RGB(0x12, 0x12, 0x12);
			userTextColor = RGB(0xFF, 0xFF, 0xFF);
			sysTextColor = RGB(0xA0, 0xA0, 0xA0);
			bkgrndColorBrush = CreateSolidBrush(bkgrndColor);
			darkMode = true;
			SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(bkgrndColorBrush));
			UpdateWindow(hwnd);
		}
		else
		{
			bkgrndColor = RGB(0xFF, 0xFF, 0xFF);
			userTextColor = RGB(0x00, 0x00, 0x00);
			sysTextColor = RGB(0x60, 0x60, 0x60);
		}*/

		hWndsRect.names.textArea = { 0, 0, 0, 0 };
		RECT childRect = translateRect(hwnd, hWndsRect.names.textArea);
		hWnds.names.textArea = CreateWindow(TEXT("Edit"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
			childRect.left, childRect.top, childRect.right, childRect.bottom, hwnd, reinterpret_cast<HMENU>(1), GetModuleHandle(0), NULL);

		return 0;
	}

	case WM_SIZE:
	{
		HDWP hDWP = BeginDeferWindowPos(2);
		if (hDWP == NULL)
		{
			MessageBox(NULL, ERROR_NOT_ENOUGH_RES_MSG, TEXT("Error!"), MB_ICONERROR | MB_OK);
			return 0;
		}
		for (size_t i = 0; i < maxHwnds; ++i)
		{
			hDWP = DeferWindowPos(hDWP, hWnds.hWnds[i], NULL, 0, 0, LOWORD(lParam) + hWndsRect.hWnds[i].right, HIWORD(lParam) + hWndsRect.hWnds[i].bottom, SWP_NOMOVE | SWP_NOOWNERZORDER);
		}

		if (EndDeferWindowPos(hDWP) == 0)
		{
			return 0;
		}

		return 0;
	}

	case UWM_GETTEXT:
	{
		size_t bufferSize = GetWindowTextLength(hWnds.names.textArea);

		PTCHAR msg = reinterpret_cast<PTCHAR>(HeapAlloc(GetProcessHeap(), LMEM_FIXED, sizeof(TCHAR)* (bufferSize + 1)));
		if (msg == NULL)
		{
			*reinterpret_cast<PTCHAR*>(lParam) = NULL;
			return 0;
		}

		*reinterpret_cast<PTCHAR*>(lParam) = msg;

		return GetWindowText(hWnds.names.textArea, msg, bufferSize + 1);
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case textAreaId:
		{
			switch (HIWORD(wParam))
			{
			case EN_VSCROLL:
				PostMessage(hwnd, WM_COMMAND, MAKEWPARAM(LOWORD(wParam), EN_CHANGE), lParam);
				return 0;

			case EN_CHANGE:
				HDC textDC = GetDC(reinterpret_cast<HWND>(lParam));
				paintLines(textDC, reinterpret_cast<HWND>(lParam), &currentMargin, TRUE, sysTextColor, bkgrndColor);
				ReleaseDC(reinterpret_cast<HWND>(lParam), textDC);
				return 0;
			}
		}
		}
		return 0;
	}

	/*case WM_CTLCOLOREDIT:
	{
		SetTextColor(reinterpret_cast<HDC>(wParam), userTextColor);
		SetBkColor(reinterpret_cast<HDC>(wParam), bkgrndColor);
		return reinterpret_cast<LRESULT>(bkgrndColorBrush);
	}*/

	case WM_DESTROY:
	{
		PTCHAR text;
		SendMessage(hwnd, UWM_GETTEXT, 0, reinterpret_cast<LPARAM>(&text));
		MessageBox(NULL, text, L"content", MB_OK);
		HeapFree(GetProcessHeap(), 0, text);
		if (darkMode)
			DeleteObject(bkgrndColorBrush);
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

RECT translateRect(HWND hwnd, RECT offset)
{
	RECT clientSize;
	GetClientRect(hwnd, &clientSize);

	offset.left += clientSize.left;
	offset.top += clientSize.top;
	offset.right += clientSize.right - clientSize.left;
	offset.bottom += clientSize.bottom - clientSize.top;

	return offset;
}

void paintLines(HDC dc, HWND textHWnd, int* currentMargin, BOOL defaultColor, COLORREF textColor, COLORREF bkgrnd)
{
	RECT editRect;
	SendMessage(textHWnd, EM_GETRECT, 0, reinterpret_cast<LPARAM>(&editRect));

	TEXTMETRIC textM;
	GetTextMetrics(dc, &textM);

	unsigned int lineCount = SendMessage(textHWnd, EM_GETLINECOUNT, 0, 0);
	unsigned int maxLinesShown = ((editRect.bottom - editRect.top) / textM.tmHeight) + 1;
	unsigned int firstLine = SendMessage(textHWnd, EM_GETFIRSTVISIBLELINE, 0, 0);

	int margin = std::to_string(min(firstLine + maxLinesShown, lineCount)).size() * textM.tmAveCharWidth + 10;
	if (*currentMargin != margin)
	{
		*currentMargin = margin;
		SendMessage(textHWnd, EM_SETMARGINS, EC_LEFTMARGIN, margin);
		UpdateWindow(textHWnd);
	}

	HPEN old = reinterpret_cast<HPEN>(SelectObject(dc, GetStockObject(NULL_PEN)));
	SelectObject(dc, old);
	if (defaultColor == FALSE)
	{
		SetTextColor(dc, textColor);
		SetBkColor(dc, bkgrnd);
	}

	for (size_t i = 0; i <= maxLinesShown; ++i)
	{
		int charIndex = SendMessage(textHWnd, EM_LINEINDEX, i + firstLine, 0);
		if (charIndex == -1)
			break;

		std::wstring offset = std::to_wstring(i + firstLine + 1);
		TextOut(dc, 5, HIWORD(SendMessage(textHWnd, EM_POSFROMCHAR, charIndex, 0)), const_cast<wchar_t*>(offset.c_str()), offset.size());
	}
}