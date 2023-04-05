#include "main.h"

typedef unsigned __int64 QWORD;

typedef struct IIDT {
    DWORD Import_Lookup_RVA;
    DWORD Time_Date_Stamp;
    DWORD Fwd_Chain;
    DWORD Name_RVA;
    DWORD IAT_RVA;
} IMAGE_IMPORT_DIRECTORY, *PIMAGE_IMPORT_DIRECTORY;

typedef WINBOOL (*bProcHPS)(HWND, LPPAINTSTRUCT);

size_t EndPaintIATIndex;
QWORD* IAT;

bProcHPS pEndPaint = NULL;

// a sample exported function
void DLL_EXPORT DLLloaded()
{
    MessageBox(0, TEXT("DLL successfully loaded"), TEXT("EndPaintHookMessage"), MB_OK | MB_ICONINFORMATION);
}

WINBOOL DLL_EXPORT __stdcall EndPaintHook(HWND hwnd, LPPAINTSTRUCT ps)
{
    TextOutW(ps->hdc, 0, 0, L"function hooked", sizeof(L"function hooked")/sizeof(wchar_t));

    return pEndPaint(hwnd, ps);
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // attach to process
            // return FALSE to fail DLL load

            PIMAGE_DOS_HEADER DOSStub = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
            PIMAGE_FILE_HEADER FileHeader = (PIMAGE_FILE_HEADER)((PCHAR)DOSStub + (DOSStub->e_lfanew + 4));
            PIMAGE_OPTIONAL_HEADER optionalHeader = (PIMAGE_OPTIONAL_HEADER)((PCHAR)FileHeader + sizeof(IMAGE_FILE_HEADER));
            PIMAGE_DATA_DIRECTORY dataDir = (PIMAGE_DATA_DIRECTORY)(&optionalHeader->DataDirectory);
            PIMAGE_IMPORT_DIRECTORY ImportDirectory = (PIMAGE_IMPORT_DIRECTORY)((PCHAR)DOSStub + dataDir[1].VirtualAddress);

            size_t i;
            for(i = 0; ImportDirectory[i].Name_RVA != 0; ++i)
            {
                if(strcmp("USER32.dll", (char*)(ImportDirectory[i].Name_RVA + (PCHAR)DOSStub)) == 0)
                    break;
            }
            if(ImportDirectory[i].Name_RVA == 0)
                return FALSE;

            QWORD* LookupTable = (QWORD*)(ImportDirectory[i].Import_Lookup_RVA + (PCHAR)DOSStub);
            for(EndPaintIATIndex = 0; LookupTable[EndPaintIATIndex] != 0; ++EndPaintIATIndex)
            {
                if(strcmp("EndPaint", (char*)(LookupTable[EndPaintIATIndex] + (PCHAR)DOSStub) + 2) == 0)
                    break;
            }
            if(LookupTable[EndPaintIATIndex] == 0)
                return FALSE;

            IAT = (QWORD*)(ImportDirectory[i].IAT_RVA + (PCHAR)DOSStub);

            pEndPaint = (bProcHPS)IAT[EndPaintIATIndex];

            IAT[EndPaintIATIndex] = (QWORD)&EndPaintHook;

            break;
        }

        case DLL_PROCESS_DETACH:
            IAT[EndPaintIATIndex] = (QWORD)pEndPaint;
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
