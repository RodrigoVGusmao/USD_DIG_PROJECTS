/*#include "main.h"

// a sample exported function
void DLL_EXPORT SomeFunction(const LPCSTR sometext)
{
    MessageBoxA(0, sometext, "DLL Message", MB_OK | MB_ICONINFORMATION);
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}*/

#include "main.h"

typedef unsigned __int32 QWORD;

typedef struct IIDT {
    DWORD Import_Lookup_RVA;
    DWORD Time_Date_Stamp;
    DWORD Fwd_Chain;
    DWORD Name_RVA;
    DWORD IAT_RVA;
} IMAGE_IMPORT_DIRECTORY, *PIMAGE_IMPORT_DIRECTORY;

typedef WINBOOL (*bProcPB)(PBYTE);
typedef WINBOOL (*bProcV)();
typedef SHORT (*sProcI)(int);
typedef INT_PTR (*pProcHStHPROCL)(HINSTANCE, LPCDLGTEMPLATEW, HWND, DLGPROC, LPARAM);

bProcPB pGetKeyboardState = NULL;
bProcV pGetInputState = NULL;
sProcI pGetKeyState = NULL;
pProcHStHPROCL pDialogBoxIndirectParamW = NULL;

// a sample exported function
void DLL_EXPORT DLLloaded()
{
    MessageBox(0, TEXT("DLL successfully loaded"), TEXT("keyboard Hook dll!"), MB_OK | MB_ICONINFORMATION);
}

WINBOOL DLL_EXPORT __stdcall GetKeyboardStateHook(PBYTE lpKeyState)
{
    //MessageBox(0, TEXT("GetKeyboardState called"), TEXT("GetKeyboardState called"), MB_OK | MB_ICONINFORMATION);
    printf("GetKeyboardState called\n");

    return pGetKeyboardState(lpKeyState);
}

WINBOOL DLL_EXPORT __stdcall GetInputStateHook()
{
    //MessageBox(0, TEXT("GetInputState called"), TEXT("GetInputState called"), MB_OK | MB_ICONINFORMATION);
    printf("GetInputState called\n");

    return pGetInputState();
}

WINBOOL DLL_EXPORT __stdcall GetKeyStateHook(int nVirtKey)
{
    //MessageBox(0, TEXT("GetKeyState called"), TEXT("GetKeyState called"), MB_OK | MB_ICONINFORMATION);
    printf("GetKeyState called: %x\n", nVirtKey);

    return pGetKeyState(nVirtKey);
}

INT_PTR DLL_EXPORT __stdcall DialogBoxIndirectParamWHook(HINSTANCE hInst, LPCDLGTEMPLATEW hDialogTemplate, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
    //MessageBox(0, TEXT("GetKeyState called"), TEXT("GetKeyState called"), MB_OK | MB_ICONINFORMATION);
    printf("DialogBoxIndirectParamW called: %p, %p, %p, %p, %lx\n", hInst, hDialogTemplate, hWndParent, lpDialogFunc, dwInitParam);

    return pDialogBoxIndirectParamW(hInst, hDialogTemplate, hWndParent, lpDialogFunc, dwInitParam);
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    DLLloaded();

    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // attach to process
            // return FALSE to fail DLL load

            AllocConsole();
            FILE* outputDescriptor = _fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_APPEND), "w");
            *stdout = *outputDescriptor;

            PIMAGE_DOS_HEADER DOSStub = (PIMAGE_DOS_HEADER)LoadLibraryA("Kernel32.dll");
            PIMAGE_FILE_HEADER FileHeader = (PIMAGE_FILE_HEADER)((PCHAR)DOSStub + (DOSStub->e_lfanew + 4));
            PIMAGE_OPTIONAL_HEADER optionalHeader = (PIMAGE_OPTIONAL_HEADER)((PCHAR)FileHeader + sizeof(IMAGE_FILE_HEADER));
            PIMAGE_DATA_DIRECTORY dataDir = (PIMAGE_DATA_DIRECTORY)(&optionalHeader->DataDirectory);
            PIMAGE_EXPORT_DIRECTORY ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PCHAR)DOSStub + dataDir[0].VirtualAddress);

            size_t i;
            for(i = 0; ((char*)(ExportDirectory->AddressOfNames + DOSStub))[i] != 0; ++i)
            {
                if(strcmp("HeapAlloc", (char*)((char*)(ExportDirectory->AddressOfNames + DOSStub))[i]) == 0)
                    break;
            }
            if(ImportDirectory[i].Name_RVA == 0)
                return FALSE;

            //first
            LPVOID pvSectionStartingAddress = NULL;
            size_t uiSectionSize = 0;
            DWORD oldProtection = 0;

            for(size_t i = 0; i < FileHeader->NumberOfSections; ++i)
            {
                PIMAGE_SECTION_HEADER sectionHeaders = (PIMAGE_SECTION_HEADER)&dataDir[optionalHeader->NumberOfRvaAndSizes];
                if((LPVOID)sectionHeaders[i].VirtualAddress <= (LPVOID)(ImportDirectory[i].IAT_RVA) &&
                   (LPVOID)(sectionHeaders[i].VirtualAddress + sectionHeaders[i].Misc.VirtualSize) >= (LPVOID)(ImportDirectory[i].IAT_RVA))
                {
                    pvSectionStartingAddress = (PVOID)((QWORD)sectionHeaders[i].VirtualAddress + (QWORD)DOSStub);
                    uiSectionSize = sectionHeaders[i].Misc.VirtualSize;
                    VirtualProtect(pvSectionStartingAddress, uiSectionSize, PAGE_READWRITE, &oldProtection);
                }
            }

            //(LPVOID)(ImportDirectory[i].IAT_RVA + (PCHAR)DOSStub)

            QWORD* LookupTable = (QWORD*)(ImportDirectory[i].Import_Lookup_RVA + (PCHAR)DOSStub);
            size_t j;
            for(j = 0; LookupTable[j] != 0; ++j)
            {
                if(strcmp("GetKeyboardState", (char*)(LookupTable[j] + (PCHAR)DOSStub) + 2) == 0)
                {
                    QWORD* IAT = (QWORD*)(ImportDirectory[i].IAT_RVA + (PCHAR)DOSStub);

                    pGetKeyboardState = (bProcPB)IAT[j];

                    IAT[j] = (QWORD)&GetKeyboardStateHook;
                }
                if(strcmp("GetInputState", (char*)(LookupTable[j] + (PCHAR)DOSStub) + 2) == 0)
                {
                    QWORD* IAT = (QWORD*)(ImportDirectory[i].IAT_RVA + (PCHAR)DOSStub);

                    pGetInputState = (bProcV)IAT[j];

                    IAT[j] = (QWORD)&GetInputStateHook;
                }
                if(strcmp("GetKeyState", (char*)(LookupTable[j] + (PCHAR)DOSStub) + 2) == 0)
                {
                    QWORD* IAT = (QWORD*)(ImportDirectory[i].IAT_RVA + (PCHAR)DOSStub);

                    pGetKeyState = (sProcI)IAT[j];

                    IAT[j] = (QWORD)&GetKeyStateHook;
                }

                if(strcmp("DialogBoxIndirectParamW", (char*)(LookupTable[j] + (PCHAR)DOSStub) + 2) == 0)
                {
                    QWORD* IAT = (QWORD*)(ImportDirectory[i].IAT_RVA + (PCHAR)DOSStub);

                    pDialogBoxIndirectParamW = (pProcHStHPROCL)IAT[j];

                    IAT[j] = (QWORD)&DialogBoxIndirectParamWHook;
                }
            }
            VirtualProtect(pvSectionStartingAddress, uiSectionSize, oldProtection, &oldProtection);
            break;
        }

        case DLL_PROCESS_DETACH:
            // detach from process
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
