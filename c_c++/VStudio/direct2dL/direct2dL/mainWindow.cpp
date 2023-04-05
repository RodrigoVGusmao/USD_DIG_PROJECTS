#define WIN32_LEAN_AND_MEAN

#include<windows.h>

#include<vector>
#include<string>

#include"mainWindow.h"
#include"graphics.h"

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static wchar_t character;
	static std::vector<bool> vkeys;

	switch (msg)
	{
	case WM_CREATE:
	{
		character = '\0';
		vkeys.resize(256);

		return 0;
	}
	case WM_CHAR:
	{
		character = static_cast<TCHAR>(wParam);
		InvalidateRect(hwnd, nullptr, TRUE);

		return 0;
	}
	case WM_KEYDOWN:
	{
		vkeys[wParam] = true;
		InvalidateRect(hwnd, nullptr, TRUE);
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	}
	case WM_KEYUP:
	{
		vkeys[wParam] = false;
		InvalidateRect(hwnd, nullptr, TRUE);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC DC = BeginPaint(hwnd, &ps);
		TextOut(DC, 0, 0, &character, 1);
		for (uint8_t i = 0; ++i != 0;)
		{
			TextOut(DC, 25+50*(i/16), 25+50*(i%16), vkeys[i] ? TEXT("T") : TEXT("F"), 1);
		}
		EndPaint(hwnd, &ps);

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return 0;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK monitorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static monitorData* frameRate;

	switch (msg)
	{
	case WM_CREATE:
	{
		frameRate = reinterpret_cast<monitorData*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetTimer(hwnd, 0, 300, NULL);
		return 0;
	}
	case WM_TIMER:
	{
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		TEXTMETRIC tm;
		std::string FPSmsg = "FPS: " + std::to_string(frameRate->FPS);
		std::string effectiveFPSmsg = "Effective FPS: " + std::to_string(frameRate->effectiveFPS);

		HDC DC = BeginPaint(hwnd, &ps);
		GetTextMetrics(DC, &tm);
		TextOutA(DC, 0, 0, FPSmsg.c_str(), static_cast<int>(FPSmsg.length()));
		TextOutA(DC, 0, 1*tm.tmHeight, effectiveFPSmsg.c_str(), static_cast<int>(effectiveFPSmsg.length()));
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return 0;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}