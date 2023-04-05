#pragma once

struct monitorData {
	float FPS;
	float effectiveFPS;
};

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK monitorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);