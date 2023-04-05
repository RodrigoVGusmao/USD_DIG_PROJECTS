#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdint.h>
#include "resource.h"

#define DLL_NAME "..\\..\\..\\EndPaintHook\\bin\\Debug\\EndPaintHook.dll"
#define EXE_NAME "functionHookTest.exe"

typedef HMODULE (__attribute__((stdcall))*hMProcTsz)(const TCHAR*);

//#define INTEL_32_BIT

#ifdef INTEL_32_BIT
#define ASSEMBLY_OFFSET_DLL_NAME 1
#define ASSEMBLY_OFFSET_LOAD_LIBRARY_A 6
const uint8_t arru8AsmCode[] = {    0x68, 0xDE, 0xAD, 0xBE, 0xEF,                               //push  0xEFBEADDE                          //37    cmt:1
                                    0xB8, 0xDE, 0xAD, 0xBE, 0xEF,                               //mov   eax, 0xEFBEADDE                     //47    cmt:6
                                    0xFF, 0xD0,                                                 //call  eax
                                    0x31, 0xC0,                                                 //xor   eax, eax

                                    0xC3                                                        //retn
                               };

#else
#define ASSEMBLY_OFFSET_DLL_NAME 6
#define ASSEMBLY_OFFSET_LOAD_LIBRARY_A 16
const uint8_t arru8AsmCode[] = {    0x48, 0x83, 0xEC, 0x28,                                     //sub   rsp, 28h
                                    0x48, 0xB9, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, //mov   rcx, 0xEFBEADDEEFBEADDE         //37    cmt:6
                                    0x48, 0xB8, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, //mov   rax, 0xEFBEADDEEFBEADDE         //47    cmt:16
                                    0xFF, 0xD0,                                                 //call  rax
                                    0x31, 0xC0,                                                 //xor   eax, eax
                                    0x48, 0x83, 0xC4, 0x28,                                     //add   rsp, 28h

                                    0xC3                                                        //retn
                               };
#endif

HANDLE getProcessByExeName(const char* acExeName)
{
    HANDLE hProcess = NULL;

    PROCESSENTRY32 sPe32Entry;
    sPe32Entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE)
    {
        #define MESSAGE_BOX_MESSAGE "could not create process snapshot: "
        char buffer[sizeof(MESSAGE_BOX_MESSAGE)+10];
        sprintf(buffer, MESSAGE_BOX_MESSAGE "0x%lX", GetLastError());
        MessageBoxA(NULL, buffer, "Error", MB_ICONERROR);
        #undef MESSAGE_BOX_MESSAGE

        goto USD_ERR_SNAPSHOT;
    }
    if(Process32First(hSnapshot, &sPe32Entry) == TRUE)
    {
        do{
            if(strcmp(sPe32Entry.szExeFile, acExeName) == 0)
                {
                    hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
                                    FALSE, sPe32Entry.th32ProcessID);
                    if(hProcess == NULL)
                    {
                        #define MESSAGE_BOX_MESSAGE "could not open process: "
                        char buffer[sizeof(MESSAGE_BOX_MESSAGE)+10];
                        sprintf(buffer, MESSAGE_BOX_MESSAGE "0x%lX", GetLastError());
                        MessageBoxA(NULL, buffer, "Error", MB_ICONERROR);
                        #undef MESSAGE_BOX_MESSAGE

                        goto USD_ERR_OPEN_PROCESS;
                    }
                    break;
                }
        }while(Process32Next(hSnapshot, &sPe32Entry) == TRUE);
    }
    USD_ERR_OPEN_PROCESS:
    CloseHandle(hSnapshot);

    USD_ERR_SNAPSHOT:
    return hProcess;
}

void LoadDllOnProcess(HANDLE hProcess, const char* szDllPathName)
{
    PVOID pvDllName;
    PVOID pvAssemblyCode;
    hMProcTsz pLoadLibraryA;
    HANDLE hThread;

    pvDllName = VirtualAllocEx(hProcess, NULL, (strlen(szDllPathName)+1) * sizeof(szDllPathName[0]), MEM_COMMIT, PAGE_READWRITE);
    if(pvDllName == NULL)
    {
        #define MESSAGE_BOX_MESSAGE "could not allocate space on remote process: "
        char buffer[sizeof(MESSAGE_BOX_MESSAGE)+10];
        sprintf(buffer, MESSAGE_BOX_MESSAGE "0x%lX", GetLastError());
        MessageBoxA(NULL, buffer, "Error", MB_ICONERROR);
        #undef MESSAGE_BOX_MESSAGE

        goto USD_ERR_ALLOC_DLL_NAME;
    }

    pvAssemblyCode = VirtualAllocEx(hProcess, NULL, sizeof(arru8AsmCode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if(pvAssemblyCode == NULL)
    {
        #define MESSAGE_BOX_MESSAGE "could not allocate space on remote process: "
        char buffer[sizeof(MESSAGE_BOX_MESSAGE)+10];
        sprintf(buffer, MESSAGE_BOX_MESSAGE "0x%lX", GetLastError());
        MessageBoxA(NULL, buffer, "Error", MB_ICONERROR);
        #undef MESSAGE_BOX_MESSAGE

        goto USD_ERR_ALLOC_ASSEMBLY_CODE;
    }

    pLoadLibraryA = &LoadLibraryA;
    WriteProcessMemory(hProcess, pvDllName, szDllPathName, strlen(szDllPathName) * sizeof(szDllPathName[0]), NULL);
    WriteProcessMemory(hProcess, pvAssemblyCode, arru8AsmCode, sizeof(arru8AsmCode), NULL);
    WriteProcessMemory(hProcess, (PCHAR)pvAssemblyCode+ASSEMBLY_OFFSET_DLL_NAME, &pvDllName, sizeof(INT_PTR), NULL);
    WriteProcessMemory(hProcess, (PCHAR)pvAssemblyCode+ASSEMBLY_OFFSET_LOAD_LIBRARY_A, (PVOID)(&pLoadLibraryA), sizeof(PVOID), NULL);

    if(FlushInstructionCache(hProcess, arru8AsmCode, sizeof(arru8AsmCode)) == 0)
    {
        #define MESSAGE_BOX_MESSAGE "could not flush instruction cache of remote process: "
        char buffer[sizeof(MESSAGE_BOX_MESSAGE)+10];
        sprintf(buffer, MESSAGE_BOX_MESSAGE "0x%lX", GetLastError());
        MessageBoxA(NULL, buffer, "Error", MB_ICONERROR);
        #undef MESSAGE_BOX_MESSAGE

        goto USD_ERR_FLUSH_L1I_CACHE;
    }

    hThread = CreateRemoteThread(hProcess, NULL, 0, (PTHREAD_START_ROUTINE)pvAssemblyCode, NULL, 0, NULL);
    if(hThread == NULL)
    {
        #define MESSAGE_BOX_MESSAGE "could not create thread space on remote process: "
        char buffer[sizeof(MESSAGE_BOX_MESSAGE)+10];
        sprintf(buffer, MESSAGE_BOX_MESSAGE "0x%lX", GetLastError());
        MessageBoxA(NULL, buffer, "Error", MB_ICONERROR);
        #undef MESSAGE_BOX_MESSAGE

        goto USD_ERR_CREATE_REMOTE_THREAD;
    }

    if(WaitForSingleObject(hThread, INFINITE) == WAIT_FAILED)
    {
        #define MESSAGE_BOX_MESSAGE "Error waiting for thread to finish: "
        char buffer[sizeof(MESSAGE_BOX_MESSAGE)+10];
        sprintf(buffer, MESSAGE_BOX_MESSAGE "0x%lX", GetLastError());
        MessageBoxA(NULL, buffer, "Error", MB_ICONERROR);
        #undef MESSAGE_BOX_MESSAGE
    }

    CloseHandle(hThread);

    USD_ERR_CREATE_REMOTE_THREAD:
    USD_ERR_FLUSH_L1I_CACHE:
    VirtualFreeEx(hProcess, pvAssemblyCode, 0, MEM_RELEASE);

    USD_ERR_ALLOC_ASSEMBLY_CODE:
    VirtualFreeEx(hProcess, pvDllName, 0, MEM_RELEASE);

    USD_ERR_ALLOC_DLL_NAME:
    return;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hWndLabel;
    static BOOL isHooked;

    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        hWndLabel = GetDlgItem(hwndDlg, 3);
        //isHooked = TRUE;
    }
    return TRUE;

    case WM_CTLCOLORSTATIC:
    {
        if((HWND)lParam == hWndLabel)
        {
            SetBkColor((HDC)wParam, GetSysColor(COLOR_3DFACE));
            SetTextColor((HDC)wParam, 0xFF << 8*isHooked);
            return (INT_PTR)GetSysColorBrush(COLOR_3DFACE);
        }
    }
    return FALSE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            case 1:
            {
                if(isHooked)
                    return TRUE;

                isHooked = 2;
                SetWindowText(hWndLabel, TEXT("Please Wait...(Getting Process Name)"));
                UpdateWindow(hWndLabel);

                HANDLE hProcess = getProcessByExeName(EXE_NAME);
                if(hProcess == NULL)
                {
                    return TRUE;
                }

                SetWindowText(hWndLabel, TEXT("Please Wait...(Loading dll on process)"));
                UpdateWindow(hWndLabel);

                LoadDllOnProcess(hProcess, DLL_NAME);

                CloseHandle(hProcess);

                isHooked = TRUE;
                SetWindowText(hWndLabel, TEXT("Hooked"));
                UpdateWindow(hWndLabel);

                break;
            }

            case 2:
            {
                isHooked = FALSE;
                SetWindowText(hWndLabel, TEXT("Not hooked"));
                UpdateWindow(hWndLabel);
                break;
            }
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
