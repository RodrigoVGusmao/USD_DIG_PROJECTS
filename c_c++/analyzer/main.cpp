#include "main.h"

typedef unsigned __int64 QWORD;

typedef struct IIDT {
    DWORD Import_Lookup_RVA;
    DWORD Time_Date_Stamp;
    DWORD Fwd_Chain;
    DWORD Name_RVA;
    DWORD IAT_RVA;
} IMAGE_IMPORT_DIRECTORY, *PIMAGE_IMPORT_DIRECTORY;

typedef int (*iProcSzVar)(const char*, ...);

uint8_t printFunctionAssembly[] = { 0x51,                                                       //push  rcx
                                    0x52,                                                       //push  rdx
                                    0x41, 0x50,                                                 //push  r8
                                    0x41, 0x51,                                                 //push  r9
                                    0x48, 0x8B, 0x54, 0x24, 0x20,                               //mov   rdx, [rsp + 20h]
                                    0x48, 0x8D, 0x0D, 0x1C, 0x00, 0x00, 0x00,                   //lea   rcx, [rip+1Ch]
                                    0x48, 0xB8, 0x0D, 0xF0, 0xAD, 0xBA, 0x0D, 0xF0, 0xAD, 0xBA, //mov   rax, 0xBAADF00DBAADF00D     //offset 20     //&printf
                                    0x48, 0x83, 0xEC, 0x28,                                     //sub   rsp, 28h
                                    0xFF, 0xD0,                                                 //call  rax
                                    0x48, 0x83, 0xC4, 0x28,                                     //add   rsp, 28h
                                    0x41, 0x59,                                                 //pop   r9
                                    0x41, 0x58,                                                 //pop   r8
                                    0x5A,                                                       //pop   rdx
                                    0x59,                                                       //pop   rcx
                                    0x58,                                                       //pop   rax
                                    0xC3,                                                       //retn
                                    '%', 's', '\n', '\0'                                        //db    "%s\n", 0
};

uint8_t hookFunctionGeneric[] = {   0x48, 0xB8, 0x0D, 0xF0, 0xAD, 0xBA, 0x0D, 0xF0, 0xAD, 0xBA, //mov   rax, 0xBAADF00DBAADF00D     //offset 02     //&original
                                    0x50,                                                       //push rax
                                    0x48, 0xB8, 0x0D, 0xF0, 0xAD, 0xBA, 0x0D, 0xF0, 0xAD, 0xBA, //mov   rax, 0xBAADF00DBAADF00D     //offset 13     //&function name
                                    0x50,                                                       //push rax
                                    0x48, 0xB8, 0x0D, 0xF0, 0xAD, 0xBA, 0x0D, 0xF0, 0xAD, 0xBA, //mov   rax, 0xBAADF00DBAADF00D     //offset 24     //&printFunction
                                    0xFF, 0xE0                                                  //jmp rax

};

// a sample exported function
void DLL_EXPORT DLLloaded()
{
    MessageBox(0, TEXT("DLL successfully loaded"), TEXT("analyzer Hook dll!"), MB_OK | MB_ICONINFORMATION);
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

            PIMAGE_DOS_HEADER DOSStub = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
            PIMAGE_FILE_HEADER FileHeader = (PIMAGE_FILE_HEADER)((PCHAR)DOSStub + (DOSStub->e_lfanew + 4));
            PIMAGE_OPTIONAL_HEADER optionalHeader = (PIMAGE_OPTIONAL_HEADER)((PCHAR)FileHeader + sizeof(IMAGE_FILE_HEADER));
            PIMAGE_DATA_DIRECTORY dataDir = (PIMAGE_DATA_DIRECTORY)(&optionalHeader->DataDirectory);
            PIMAGE_IMPORT_DIRECTORY ImportDirectory = (PIMAGE_IMPORT_DIRECTORY)((PCHAR)DOSStub + dataDir[1].VirtualAddress);
            void* hookedTable = VirtualAlloc(NULL, sizeof(hookFunctionGeneric)*(dataDir[12].Size/8), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

            //first
            LPVOID pvSectionStartingAddress = NULL;
            size_t uiSectionSize = 0;
            DWORD oldProtection = 0;

            for(size_t i = 0; i < FileHeader->NumberOfSections; ++i)
            {
                PIMAGE_SECTION_HEADER sectionHeaders = (PIMAGE_SECTION_HEADER)&dataDir[optionalHeader->NumberOfRvaAndSizes];
                if(sectionHeaders[i].VirtualAddress <= ImportDirectory[0].IAT_RVA &&
                   sectionHeaders[i].VirtualAddress + sectionHeaders[i].Misc.VirtualSize >= ImportDirectory[0].IAT_RVA)
                {
                    pvSectionStartingAddress = (PVOID)((INT_PTR)sectionHeaders[i].VirtualAddress + (INT_PTR)DOSStub);
                    uiSectionSize = sectionHeaders[i].Misc.VirtualSize;
                    VirtualProtect(pvSectionStartingAddress, uiSectionSize, PAGE_READWRITE, &oldProtection);
                }
            }

            uint8_t *printFunction = (uint8_t*)VirtualAlloc(NULL, 23, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
            CopyMemory(printFunction, printFunctionAssembly, sizeof(printFunctionAssembly));
            *(INT_PTR*)(printFunction + 20) = (INT_PTR)&printf;
            *((INT_PTR*)(&(hookFunctionGeneric[24]))) = (INT_PTR)(printFunction);

            size_t i;
            size_t j;
            size_t k;
            for(i = 0, k = 0; ImportDirectory[i].Name_RVA != 0; ++i)
            {

                QWORD* LookupTable = (QWORD*)(ImportDirectory[i].Import_Lookup_RVA + (PCHAR)DOSStub);
                QWORD* IAT = (QWORD*)(ImportDirectory[i].IAT_RVA + (PCHAR)DOSStub);
                for(j = 0; LookupTable[j] != 0; ++j, ++k)
                {
                    CopyMemory((uint8_t*)(hookedTable)+k*sizeof(hookFunctionGeneric), hookFunctionGeneric, sizeof(hookFunctionGeneric));
                    *(INT_PTR*)((uint8_t*)(hookedTable)+k*sizeof(hookFunctionGeneric)+2) = IAT[j];
                    *(INT_PTR*)((uint8_t*)(hookedTable)+k*sizeof(hookFunctionGeneric)+13) = (INT_PTR)((LookupTable[j] + (PCHAR)DOSStub) + 2);
                    IAT[j] = (INT_PTR)(hookedTable)+k*sizeof(hookFunctionGeneric);
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
