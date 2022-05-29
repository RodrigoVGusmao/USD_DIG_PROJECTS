#pragma once

#include<windows.h>

BOOL injectDLL(HANDLE* process, LPVOID* function, DWORD pID, const PCHAR szDllPath, HWND receptorWindow);
BOOL releaseDLL(HANDLE process, LPVOID function);