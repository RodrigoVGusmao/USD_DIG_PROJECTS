#include "dllInject.h"

#include<stdint.h>

BOOL injectDLL(HANDLE *process, LPVOID* function, DWORD pID, const PCHAR szDllPath, HWND receptorWindow)
{
    //needs update
    #ifdef INTEL_32_BIT
    #define ASSEMBLY_OFFSET_DLL_NAME 1
    #define ASSEMBLY_OFFSET_LOAD_LIBRARY_A 6
        const uint8_t arru8AsmCode[] = { 0x68, 0xDE, 0xAD, 0xBE, 0xEF,                              //push  0xEFBEADDE                          //37    cmt:1
                                        0xB8, 0xDE, 0xAD, 0xBE, 0xEF,                               //mov   eax, 0xEFBEADDE                     //47    cmt:6
                                        0xFF, 0xD0,                                                 //call  eax
                                        0x31, 0xC0,                                                 //xor   eax, eax

                                        0xC3                                                        //retn
        };

    #else
    #define ASSEMBLY_OFFSET_DLL_NAME 6
    #define ASSEMBLY_OFFSET_LOAD_LIBRARY_A 16
    #define ASSEMBLY_OFFSET_GET_PROC_ADDRESS 50
    #define ASSEMBLY_OFFSET_HWND 67
    #define ASSEMBLY_OFFSET_FREE_LIBRARY 100
    #define ASSEMBLY_OFFSET_RELEASE_LIBRARY 84
        const uint8_t arru8AsmCode[] = { 0x48, 0x83, 0xEC, 0x28,                                     //sub   rsp, 28h
                                        0x48, 0xB9, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, //mov   rcx, 0xEFBEADDEEFBEADDE(DLL_NAME_STR)   //cmt:6
                                        0x48, 0xB8, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, //mov   rax, 0xEFBEADDEEFBEADDE(&LoadLibraryA)  //cmt:16
                                        0xFF, 0xD0,                                                 //call  rax

                                        0x48, 0x85, 0xC0,                                           //test  rax, rax
                                        0x74, 0x36,                                                 //jne   add_rsp
                                        0x48, 0x89, 0x05, 0x34, 0x00, 0x00, 0x00,                   //mov   [rip+0x34], rax
                                        0x48, 0x89, 0xC1,                                           //mov   rcx, rax
                                        0x48, 0x8D, 0x15, 0x44, 0x00, 0x00, 0x00,                   //mov   rdx, [db_"RegisterWindowHandle"_0]
                                        0x48, 0xB8, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, //mov   rax, [GetProcAddress]                   //cmt:50
                                        0xFF, 0xD0,                                                 //call  rax

                                        0x48, 0x85, 0xC0,                                           //test  rax, rax
                                        0x74, 0x0E,                                                 //jne   add_rsp
                                        0x48, 0xB9, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, //mov   rcx, 0xEFBEADDEEFBEADDE(HWND)           //cmt:67
                                        0xFF, 0xD0,                                                 //call  rax

                                        0x31, 0xC0,                                                 //xor   eax, eax
                                        0x48, 0x83, 0xC4, 0x28,                                     //add   rsp, 28h

                                        0xC3,                                                       //retn

                                        0x48, 0x83, 0xEC, 0x28,                                     //sub   rsp, 28h                                //cmt:84
                                        0x48, 0xB9, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, //mov   rcx, 0xEFBEADDEEFBEADDE                 //cmt:90
                                        0x48, 0xB8, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, //mov   rax, 0xEFBEADDEEFBEADDE(&LoadLibraryA)  //cmt:100
                                        0xFF, 0xD0,                                                 //call  rax
                                        0x48, 0x83, 0xC4, 0x28,                                     //add   rsp, 28h
                                        0xC3,                                                       //retn

                                        0x52, 0x65, 0x67, 0x69, 0x73, 0x74, 0x65, 0x72, 0x57, 0x69, //db    "RegisterWi",
                                        0x6E, 0x64, 0x6F, 0x77, 0x48, 0x61, 0x6E, 0x64, 0x6C, 0x65, //      "ndowHandle",
                                        0x00                                                        //      0
        };
    #endif

    BOOL error;

    size_t dllPathLen = strlen(szDllPath) + 1;
    HMODULE(*pLoadLibraryA)(LPCSTR) = &LoadLibraryA;
    FARPROC(*pGetProcAddress)(HMODULE, LPCSTR) = &GetProcAddress;
    BOOL(*pFreeLibrary)(HMODULE) = &FreeLibrary;
    HANDLE hThread;
    DWORD exitCode;

    *process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pID);
    if (*process == NULL)
    {
        error = TRUE;
        goto USD_ERR_OPEN_PROCESS;
    }

    *function = VirtualAllocEx(*process, NULL, sizeof(arru8AsmCode)+dllPathLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (*function == NULL)
    {
        error = TRUE;
        goto USD_ERR_VIRTUAL_ALLOC_EX_FUNCTION;
    }

    *(INT_PTR*)(arru8AsmCode + ASSEMBLY_OFFSET_DLL_NAME) = (INT_PTR)*function + sizeof(arru8AsmCode);
    *(HMODULE(**)(LPCSTR))(arru8AsmCode + ASSEMBLY_OFFSET_LOAD_LIBRARY_A) = (HMODULE(*)(LPCSTR))pLoadLibraryA;
    *(FARPROC(**)(HMODULE, LPCSTR))(arru8AsmCode + ASSEMBLY_OFFSET_GET_PROC_ADDRESS) = (FARPROC(*)(HMODULE, LPCSTR))pGetProcAddress;
    *(HWND*)(arru8AsmCode + ASSEMBLY_OFFSET_HWND) = (HWND)receptorWindow;
    *(BOOL(**)(HMODULE))(arru8AsmCode + ASSEMBLY_OFFSET_FREE_LIBRARY) = (BOOL(*)(HMODULE))pFreeLibrary;

    WriteProcessMemory(*process, *function, arru8AsmCode, sizeof(arru8AsmCode), NULL);
    WriteProcessMemory(*process, (uint8_t*)(*function)+sizeof(arru8AsmCode), szDllPath, dllPathLen, NULL);

    if (FlushInstructionCache(*process, *function, sizeof(arru8AsmCode)+dllPathLen) == FALSE)
    {
        error = TRUE;
        goto USD_ERR_INSTRUCTION_CACHE_FLUSH;
    }

    hThread = CreateRemoteThread(*process, NULL, 0, (PTHREAD_START_ROUTINE)*function, NULL, 0, NULL);
    if (hThread == NULL)
    {
        error = TRUE;
        goto USD_ERR_CREATE_REMOTE_THREAD;
    }

    WaitForSingleObject(hThread, INFINITE);

    if (GetExitCodeThread(hThread, &exitCode) == 0)
    {
        error = TRUE;
        goto USD_ERR_GET_EXIT_CODE_THREAD;
    }
    else
    {
        error = exitCode;
    }

    USD_ERR_GET_EXIT_CODE_THREAD:
    CloseHandle(hThread);
    return error;

    USD_ERR_CREATE_REMOTE_THREAD:
    USD_ERR_INSTRUCTION_CACHE_FLUSH:
    VirtualFreeEx(*process, *function, 0, MEM_RELEASE);

    USD_ERR_VIRTUAL_ALLOC_EX_FUNCTION:
    CloseHandle(*process);

    USD_ERR_OPEN_PROCESS:

    return error;
}

BOOL releaseDLL(HANDLE process, LPVOID function)
{
    BOOL error;
    HANDLE hThread;
    DWORD exitCode;

    hThread = CreateRemoteThread(process, NULL, 0, (PTHREAD_START_ROUTINE)((uint8_t*)function+ASSEMBLY_OFFSET_RELEASE_LIBRARY), NULL, 0, NULL);
    if (hThread == NULL)
    {
        error = TRUE;
        goto USD_ERR_CREATE_REMOTE_THREAD;
    }

    WaitForSingleObject(hThread, INFINITE);

    if (GetExitCodeThread(hThread, &exitCode) == 0)
    {
        error = TRUE;
        goto USD_ERR_GET_EXIT_CODE_THREAD;
    }
    else
    {
        error = FALSE;
    }

    USD_ERR_GET_EXIT_CODE_THREAD:
    CloseHandle(hThread);

    USD_ERR_CREATE_REMOTE_THREAD:
    VirtualFreeEx(process, function, 0, MEM_RELEASE);
    CloseHandle(process);

    return error;
}