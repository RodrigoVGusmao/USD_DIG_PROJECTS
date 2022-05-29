#pragma once

#include<windows.h>
#include<TlHelp32.h>

typedef struct {
	DWORD processID;
	DWORD parentID;
	TCHAR szExeFile[MAX_PATH];
} processInfo;

BOOL getProcessList(processInfo** pInfo);
BOOL setComboBoxProcessList(HWND hComboBox, const processInfo* pInfo, processInfo** subPInfo, DWORD parentID);
size_t findString(const processInfo* pInfo, const PTCHAR name);