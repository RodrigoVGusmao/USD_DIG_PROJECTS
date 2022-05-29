#include "getProcess.h"

#ifdef _UNICODE
#define tstrncmp(A, B, C) wcsncmp(A, B, C)
#else
#define tstrncmp(A, B, C) strncmp(A, B, C)
#endif

BOOL getProcessList(processInfo** pInfo)
{
	if (pInfo == NULL)
		return TRUE;
	PROCESSENTRY32 pe32;
	size_t nProcess = 0;
	size_t nReservedProcessInfo = 1;
	HANDLE currentProcessHeap = GetProcessHeap();
	HANDLE hToolhelpSnapshoot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	pe32.dwSize = sizeof(PROCESSENTRY32);
	*pInfo = (processInfo*)HeapAlloc(currentProcessHeap, 0, sizeof(processInfo));
	if (*pInfo == NULL)
		return TRUE;

	if (Process32First(hToolhelpSnapshoot, &pe32))
	{
		do {
			if (nProcess == nReservedProcessInfo)
			{
				nReservedProcessInfo *= 2;
				LPVOID temp = HeapReAlloc(currentProcessHeap, 0, *pInfo, nReservedProcessInfo * sizeof(processInfo));
				if (temp == NULL)
					return TRUE;

				*pInfo = (processInfo*)temp;
			}
			(*pInfo)[nProcess].processID = pe32.th32ProcessID;
			(*pInfo)[nProcess].parentID = pe32.th32ParentProcessID;
			memcpy((*pInfo)[nProcess++].szExeFile, pe32.szExeFile, MAX_PATH);
		} while (Process32Next(hToolhelpSnapshoot, &pe32));

		nReservedProcessInfo = nProcess;
		LPVOID temp = HeapReAlloc(currentProcessHeap, 0, *pInfo, nProcess * sizeof(processInfo));
		if (temp == NULL)
			return TRUE;

		*pInfo = (processInfo*)temp;
		memset(&(*pInfo)[nProcess-1], 0, sizeof(processInfo));
	}
	return FALSE;
}

BOOL setComboBoxProcessList(HWND hComboBox, const processInfo* pInfo, processInfo** subPInfo, DWORD parentID)
{
	size_t nCopyProcess = 0;
	size_t nReservedProcessInfo = 1;
	HANDLE currentProcessHeap = GetProcessHeap();

	if (subPInfo != NULL)
	{
		*subPInfo = (processInfo*)HeapAlloc(currentProcessHeap, 0, sizeof(processInfo));
		if (*subPInfo == NULL)
			return TRUE;
	}

	for (size_t nProcess = 0; pInfo[nProcess].processID != 0 || pInfo[nProcess].szExeFile[0] != 0; ++nProcess)
	{
		if (parentID == 0)
			for (size_t nProcess2 = 0; pInfo[nProcess2].processID != 0 || pInfo[nProcess2].szExeFile[0] != 0; ++nProcess2)
			{
				if (pInfo[nProcess].parentID == pInfo[nProcess2].processID)
				{
					if (tstrncmp(pInfo[nProcess2].szExeFile, TEXT("explorer.exe"), MAX_PATH) == 0 || tstrncmp(pInfo[nProcess2].szExeFile, TEXT("cmd.exe"), MAX_PATH) == 0)
					{
						break;
					}
					/*if (tstrncmp(pInfo[nProcess2].szExeFile, TEXT("csrss.exe"), MAX_PATH) == 0)
						break;*/
					goto USD_OK_ENDFOR;
				}
			}
		else if (parentID == -1);
		else if (pInfo[nProcess].parentID != parentID)
			continue;

		if (subPInfo != NULL)
		{
			if (nCopyProcess == nReservedProcessInfo)
			{
				nReservedProcessInfo *= 2;
				LPVOID temp = HeapReAlloc(currentProcessHeap, 0, *subPInfo, nReservedProcessInfo * sizeof(processInfo));
				if (temp == NULL)
					return TRUE;

				*subPInfo = (processInfo*)temp;
			}

			memcpy(&(*subPInfo)[nCopyProcess++], &pInfo[nProcess], sizeof(processInfo));
		}

		SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)pInfo[nProcess].szExeFile);
	USD_OK_ENDFOR:;
	}

	if (subPInfo != NULL)
	{
		nReservedProcessInfo = nCopyProcess;
		LPVOID temp = HeapReAlloc(currentProcessHeap, HEAP_ZERO_MEMORY, *subPInfo, nReservedProcessInfo * sizeof(processInfo));
		if (temp == NULL)
			return TRUE;

		*subPInfo = (processInfo*)temp;
	}

	return FALSE;
}

size_t findString(const processInfo* pInfo, const PTCHAR name)
{
	for (size_t nProcess = 0; pInfo[nProcess].processID != 0 || pInfo[nProcess].szExeFile[0] != 0; ++nProcess)
	{
		if (tstrncmp(pInfo[nProcess].szExeFile, name, MAX_PATH) == 0)
			return nProcess;
	}

	return ~0;
}