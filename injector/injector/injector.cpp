// injector.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "injector.h"
#include "getProcess.h"
#include"dllInject.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance

// Forward declarations of functions included in this code module:
INT_PTR DLGProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR spyDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_INJECTOR), NULL, DLGProc);
    return 0;
}

INT_PTR DLGProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static processInfo* pInfo;
    static processInfo* subPInfo;
    static processInfo* childPInfo;
    static DWORD pID;
    static BOOL bHooked;
    static HWND spyWindow;

    static HANDLE process;
    static LPVOID function;

    switch (msg)
    {
    case WM_INITDIALOG:
        //SetTimer(hwnd, 0, 1000, NULL);
        bHooked = FALSE;
        getProcessList(&pInfo);
        setComboBoxProcessList(GetDlgItem(hwnd, IDC_PROCESS), pInfo, &subPInfo, 0);
        spyWindow = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SPY), hwnd, spyDlgProc);
        ShowWindow(spyWindow, SW_HIDE);
        //SendMessage(GetDlgItem(hwnd, IDC_PROCESS), CB_ADDSTRING);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_PROCESS:
            if (HIWORD(wParam) == CBN_SELENDOK)
            {
                //TCHAR string[MAX_PATH];
                SendMessage(GetDlgItem(hwnd, IDC_CHILD), CB_RESETCONTENT, 0, 0);
                size_t index = SendMessage(GetDlgItem(hwnd, IDC_PROCESS), CB_GETCURSEL, 0, 0);
                //SendMessage(GetDlgItem(hwnd, IDC_PROCESS), CB_GETLBTEXT, SendMessage(GetDlgItem(hwnd, IDC_PROCESS), CB_GETCURSEL, 0, 0), (LPARAM)string);
                /*size_t index = findString(subPInfo, string);
                if (index == ~0)
                    break;*/
                setComboBoxProcessList(GetDlgItem(hwnd, IDC_CHILD), pInfo, &childPInfo, pID = subPInfo[index].processID);
            }
            break;

        case IDC_CHILD:
            if (HIWORD(wParam) == CBN_SELENDOK)
            {
                size_t index = SendMessage(GetDlgItem(hwnd, IDC_CHILD), CB_GETCURSEL, 0, 0);
                pID = childPInfo[index].processID;
            }
            break;

        case IDHOOK:
        if(bHooked == FALSE)
        {
            CHAR DLLPath[MAX_PATH];
            GetWindowTextA(GetDlgItem(hwnd, IDC_DLL), DLLPath, MAX_PATH);
            if (injectDLL(&process, &function,pID, DLLPath, NULL) == FALSE)
            {
                SetWindowText(GetDlgItem(hwnd, IDHOOK), TEXT("Unhook"));
                EnableWindow(GetDlgItem(hwnd, IDC_CHILD), FALSE);
                EnableWindow(GetDlgItem(hwnd, IDC_PROCESS), FALSE);
                EnableWindow(GetDlgItem(hwnd, IDC_DLL), FALSE);
                EnableWindow(GetDlgItem(hwnd, IDSPY), TRUE);
                SetWindowText(GetDlgItem(spyWindow, IDC_SPY), TEXT(""));
                bHooked = TRUE;
            }
        }
        else
        {
            if (releaseDLL(process, function) == FALSE)
            {
                SetWindowText(GetDlgItem(hwnd, IDHOOK), TEXT("Hook"));
                EnableWindow(GetDlgItem(hwnd, IDC_CHILD), TRUE);
                EnableWindow(GetDlgItem(hwnd, IDC_PROCESS), TRUE);
                EnableWindow(GetDlgItem(hwnd, IDC_DLL), TRUE);
                EnableWindow(GetDlgItem(hwnd, IDSPY), FALSE);
                bHooked = FALSE;
            }
        }
        break;

        case IDSPY:
            ShowWindow(spyWindow, SW_NORMAL);
            break;

        case IDC_REFRESH:
            SendMessage(GetDlgItem(hwnd, IDC_PROCESS), CB_RESETCONTENT, 0, 0);
            SendMessage(GetDlgItem(hwnd, IDC_CHILD), CB_RESETCONTENT, 0, 0);
            getProcessList(&pInfo);
            setComboBoxProcessList(GetDlgItem(hwnd, IDC_PROCESS), pInfo, &subPInfo, 0);
            break;

        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }
        return TRUE;

    /*case WM_TIMER:
        if (SendMessage(GetDlgItem(hwnd, IDC_PROCESS), CB_GETDROPPEDSTATE, 0, 0))
            return TRUE;

        SendMessage(GetDlgItem(hwnd, IDC_PROCESS), CB_RESETCONTENT, 0, 0);
        getProcessList(&pInfo);
        setComboBoxProcessList(GetDlgItem(hwnd, IDC_PROCESS), pInfo, &subPInfo, 0);
        return TRUE;*/

    case WM_DESTROY:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}

INT_PTR spyDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COPYDATA:
    {
        PCOPYDATASTRUCT log = (PCOPYDATASTRUCT)lParam;
        SendMessage(GetDlgItem(hwnd, IDC_SPY), EM_REPLACESEL, FALSE, (LPARAM)log->lpData);
        return TRUE;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }
        return TRUE;
    }
    case WM_DESTROY:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}