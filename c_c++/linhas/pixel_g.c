#include"pixel.h"

#ifdef WINDOW
#include<stdio.h>

#define USD_VERTICAL "\u2502"           ///2502, 2503 for bold
#define USD_HORIZONTAL "\u2500"         ///2500, 2501 for bold
#define USD_CENTER "\u253C"             ///253C, 254B dor full bold, 253F for horizontal bold, 2542 for vertical bold

#define WM_USD_SET_VIEWPORT WM_USER
#define WM_USD_PRINT_PIXEL WM_USER+1
#define WM_USD_PRINT_CARTESIAN_PLANE WM_USER+2
#define WM_USD_SET_PIXEL_ASPECT WM_USER+3
#define WM_USD_SET_COLOR_SCHEME WM_USER+4

static const DWORD wndStyle = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

static POINT aspectRatio = {1, 1};

static HINSTANCE hInstance;
static HWND mainHWnd;
static HANDLE consoleOut;
static CONSOLE_SCREEN_BUFFER_INFOEX csbi;

static RECT viewport;

static HDC backBufferDC;
static HBITMAP oldBackBufferBitmap;

static HDC cartesianPlaneDC;
static HBITMAP oldCartesianBitmap;
static HPEN oldCartesianPen;

static HDC pixelDC;
static HBITMAP oldPixelBitmap;

static HBRUSH background;
static HBRUSH foreground;

void drawPixelBackBuffer()
{
    HDC DC = GetDC(mainHWnd);
    HBITMAP bmp = CreateCompatibleBitmap(DC, aspectRatio.x, aspectRatio.y);
    ReleaseDC(mainHWnd, DC);
    if(bmp == NULL) goto USD_ERR_CREATE_COMPATIBLE_BITMAP;
    DeleteObject(SelectObject(pixelDC, (HGDIOBJ)bmp));

    RECT rect = {0, 0, aspectRatio.x+1, aspectRatio.y+1};
    FillRect(pixelDC, &rect, foreground);

    return;

    USD_ERR_CREATE_COMPATIBLE_BITMAP:
    {
        wchar_t msg[sizeof("CreateCompatibleBitmap failed: 0x%d\nWas 'init()' called?")+6];
        swprintf(msg, sizeof(msg), L"CreateCompatibleBitmap failed: 0x%x\nWas 'init()' called?", GetLastError());
        MessageBoxW(NULL, msg, L"error", MB_ICONERROR);
        return;
    }
}

int printPixel(int x, int y)
{
    return SendMessage(mainHWnd, WM_USD_PRINT_PIXEL, x, y);
}

void updateBackBufferSize()
{
    HDC DC = GetDC(mainHWnd);
    HBITMAP bmp = CreateCompatibleBitmap(DC, aspectRatio.x*(viewport.right-viewport.left), aspectRatio.y*(viewport.bottom-viewport.top));
    ReleaseDC(mainHWnd, DC);
    if(bmp == NULL) goto USD_ERR_CREATE_COMPATIBLE_BITMAP;
    DeleteObject(SelectObject(backBufferDC, (HGDIOBJ)bmp));

    return;

    USD_ERR_CREATE_COMPATIBLE_BITMAP:
    {
        wchar_t msg[sizeof("CreateCompatibleBitmap failed: 0x%d\nWas 'init()' called?")+6];
        swprintf(msg, sizeof(msg), L"CreateCompatibleBitmap failed: 0x%x\nWas 'init()' called?", GetLastError());
        MessageBoxW(NULL, msg, L"error", MB_ICONERROR);
        return;
    }
}

void drawCartesianPlaneBackBuffer()
{
    HDC DC = GetDC(mainHWnd);
    HBITMAP bmp = CreateCompatibleBitmap(DC, aspectRatio.x*(viewport.right-viewport.left), aspectRatio.y*(viewport.bottom-viewport.top));
    ReleaseDC(mainHWnd, DC);
    if(bmp == NULL) goto USD_ERR_CREATE_COMPATIBLE_BITMAP;
    DeleteObject(SelectObject(cartesianPlaneDC, (HGDIOBJ)bmp));

    RECT rect = {0, 0, aspectRatio.x*(viewport.right-viewport.left)+1, aspectRatio.y*(viewport.bottom-viewport.top)+1};
    FillRect(cartesianPlaneDC, &rect, background);

    MoveToEx(cartesianPlaneDC, -viewport.left*aspectRatio.x, 0, NULL);
    LineTo(cartesianPlaneDC, -viewport.left*aspectRatio.x, aspectRatio.y*(viewport.bottom-viewport.top));

    MoveToEx(cartesianPlaneDC, 0, -viewport.top*aspectRatio.y, NULL);
    LineTo(cartesianPlaneDC, aspectRatio.x*(viewport.right-viewport.left), -viewport.top*aspectRatio.y);

    return;

    USD_ERR_CREATE_COMPATIBLE_BITMAP:
    {
        wchar_t msg[sizeof("CreateCompatibleBitmap failed: 0x%d\nWas 'init()' called?")+6];
        swprintf(msg, sizeof(msg), L"CreateCompatibleBitmap failed: 0x%x\nWas 'init()' called?", GetLastError());
        MessageBoxW(NULL, msg, L"error", MB_ICONERROR);
        return;
    }
}

void setPixelAspect(SHORT x, SHORT y)
{
    SendMessage(mainHWnd, WM_USD_SET_PIXEL_ASPECT, x, y);
}

void setViewport(int minX, int minY, int maxX, int maxY)
{
    SendMessage(mainHWnd, WM_USD_SET_VIEWPORT, MAKEWPARAM((int16_t)minX, (int16_t)minY), MAKELPARAM((int16_t)maxX, (int16_t)maxY));
}

void printCartesianPlane(void)
{
    SendMessage(mainHWnd, WM_USD_PRINT_CARTESIAN_PLANE, 0, 0);
}

void resetLine(void)
{
    if(GetConsoleScreenBufferInfoEx(consoleOut, &csbi) == 0) goto USD_ERR_FAILED;

    COORD writeTarget = {0, 0};
    DWORD out;
    FillConsoleOutputCharacter(consoleOut, ' ', csbi.dwSize.X*csbi.dwCursorPosition.Y+csbi.dwCursorPosition.X, writeTarget, &out);

    COORD position = {0, 0};
    SetConsoleCursorPosition(consoleOut, position);

    return;
    USD_ERR_FAILED:
        printf("GetConsoleScreenBufferInfoEx failed: %lu", GetLastError());
        exit(0);
}

void fitTextRight(void){}

void setColorScheme(COLORREF foreground, COLORREF background)
{
    SendMessage(mainHWnd, WM_USD_SET_COLOR_SCHEME, foreground, background);
}

LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CREATE:
    {
        HDC DC = GetDC(hWnd);
        backBufferDC = CreateCompatibleDC(DC);
        HBITMAP bmp = CreateCompatibleBitmap(DC, aspectRatio.x*(viewport.right-viewport.left), aspectRatio.y*(viewport.bottom-viewport.top));
        if(bmp == NULL) goto USD_ERR_CREATE_COMPATIBLE_BITMAP;
        oldBackBufferBitmap = (HBITMAP)SelectObject(backBufferDC, (HGDIOBJ)bmp);

        cartesianPlaneDC = CreateCompatibleDC(DC);
        bmp = CreateCompatibleBitmap(DC, aspectRatio.x*(viewport.right-viewport.left), aspectRatio.y*(viewport.bottom-viewport.top));
        if(bmp == NULL) goto USD_ERR_CREATE_COMPATIBLE_BITMAP;
        oldCartesianBitmap = (HBITMAP)SelectObject(cartesianPlaneDC, (HGDIOBJ)bmp);
        HPEN whitePen = CreatePen(PS_SOLID, 1, 0xFFFFFF);
        if(whitePen == NULL) goto USD_CREATE_PEN;
        oldCartesianPen = SelectObject(cartesianPlaneDC, whitePen);

        pixelDC = CreateCompatibleDC(DC);
        bmp = CreateCompatibleBitmap(DC, aspectRatio.x, aspectRatio.y);
        if(bmp == NULL) goto USD_ERR_CREATE_COMPATIBLE_BITMAP;
        oldPixelBitmap = (HBITMAP)SelectObject(pixelDC, (HGDIOBJ)bmp);

        ReleaseDC(hWnd, DC);

        background = CreateSolidBrush(0x000000);
        if(background == NULL) goto USD_CREATE_SOLID_BRUSH;

        foreground = CreateSolidBrush(0xFFFFFF);
        if(foreground == NULL) goto USD_CREATE_SOLID_BRUSH;

        drawPixelBackBuffer();
        return 0;
    }

    case WM_USD_SET_VIEWPORT:
    {
        viewport.left = (int16_t)(wParam >> 16);
        viewport.top = (int16_t)wParam;
        viewport.right = (int16_t)(lParam >> 16);
        viewport.bottom = (int16_t)lParam;

        updateBackBufferSize();
        drawCartesianPlaneBackBuffer();

        RECT windowSize = {0, 0, aspectRatio.x*(viewport.right-viewport.left), aspectRatio.y*(viewport.right-viewport.left)};
        AdjustWindowRect(&windowSize, wndStyle, FALSE);
        SetWindowPos(hWnd, HWND_TOP, 0, 0, windowSize.right-windowSize.left, windowSize.bottom-windowSize.top, SWP_NOMOVE | SWP_NOZORDER);
        return 0;
    }

    case WM_USD_PRINT_PIXEL:
    {
        if((int)wParam < viewport.left || -(int)lParam < viewport.top || (int)wParam >= viewport.right || -(int)lParam >= viewport.bottom)
            return -1;

        BitBlt(backBufferDC, aspectRatio.x*((int)wParam-viewport.left)-aspectRatio.x/2, aspectRatio.y*(-(int)lParam-viewport.top)-aspectRatio.y/2, aspectRatio.x, aspectRatio.y,
                pixelDC, 0, 0, SRCCOPY);

        InvalidateRect(hWnd, NULL, FALSE);

        return 0;
    }

    case WM_USD_PRINT_CARTESIAN_PLANE:
    {
        resetLine();
        BitBlt(backBufferDC, 0, 0, aspectRatio.x*(viewport.right-viewport.left), aspectRatio.y*(viewport.right-viewport.left), cartesianPlaneDC, 0, 0, SRCCOPY);
        InvalidateRect(hWnd, NULL, FALSE);

        return 0;
    }

    case WM_USD_SET_PIXEL_ASPECT:
    {
        aspectRatio.x = (SHORT)wParam;
        aspectRatio.y = (SHORT)lParam;

        updateBackBufferSize();
        drawCartesianPlaneBackBuffer();
        drawPixelBackBuffer();

        RECT windowSize = {0, 0, aspectRatio.x*(viewport.right-viewport.left), aspectRatio.y*(viewport.right-viewport.left)};
        AdjustWindowRect(&windowSize, wndStyle, FALSE);
        SetWindowPos(hWnd, HWND_TOP, 0, 0, windowSize.right-windowSize.left, windowSize.bottom-windowSize.top, SWP_NOMOVE | SWP_NOZORDER);

        return 0;
    }

    case WM_USD_SET_COLOR_SCHEME:
    {
        HPEN pen = CreatePen(PS_SOLID, 1, wParam);
        if(pen == NULL) goto USD_CREATE_PEN;
        DeleteObject(SelectObject(cartesianPlaneDC, (HGDIOBJ)pen));

        DeleteObject(foreground);
        foreground = CreateSolidBrush(wParam);
        if(foreground == NULL) goto USD_CREATE_SOLID_BRUSH;

        DeleteObject(background);
        background = CreateSolidBrush(lParam);
        if(background == NULL) goto USD_CREATE_SOLID_BRUSH;

        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC DC = BeginPaint(hWnd, &ps);
        BitBlt(DC, 0, 0, aspectRatio.x*(viewport.right-viewport.left), aspectRatio.y*(viewport.bottom-viewport.top), backBufferDC, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        DeleteObject(SelectObject(backBufferDC, (HGDIOBJ)oldBackBufferBitmap));
        DeleteDC(backBufferDC);

        DeleteObject(SelectObject(cartesianPlaneDC, (HGDIOBJ)oldCartesianPen));
        DeleteObject(SelectObject(cartesianPlaneDC, (HGDIOBJ)oldCartesianBitmap));
        DeleteDC(cartesianPlaneDC);

        DeleteObject(SelectObject(pixelDC, (HGDIOBJ)oldPixelBitmap));
        DeleteDC(pixelDC);

        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);

    USD_CREATE_PEN:
    {
        wchar_t msg[sizeof("CreatePen failed: 0x%x")+6];
        swprintf(msg, sizeof(msg), L"CreatePen failed: 0x%x", GetLastError());
        MessageBoxW(NULL, msg, L"error", MB_ICONERROR);
        exit(0);
    }
    USD_CREATE_SOLID_BRUSH:
    {
        wchar_t msg[sizeof("CreateSolidBrush failed: 0x%x")+6];
        swprintf(msg, sizeof(msg), L"CreateSolidBrush failed: 0x%x", GetLastError());
        MessageBoxW(NULL, msg, L"error", MB_ICONERROR);
        exit(0);
    }
    USD_ERR_CREATE_COMPATIBLE_BITMAP:
    {
        wchar_t msg[sizeof("CreateCompatibleBitmap failed: 0x%d\nWas 'init()' called?")+6];
        swprintf(msg, sizeof(msg), L"CreateCompatibleBitmap failed: 0x%x\nWas 'init()' called?", GetLastError());
        MessageBoxW(NULL, msg, L"error", MB_ICONERROR);
        exit(0);
    }
}

DWORD WINAPI thread(LPVOID dummy)
{
    char* errMsg;

    hInstance = GetModuleHandleW(NULL);

    WNDCLASSW clss;
    clss.style = CS_HREDRAW | CS_VREDRAW;
    clss.lpfnWndProc = windowProc;
    clss.cbClsExtra = 0;
    clss.cbWndExtra = 0;
    clss.hInstance = hInstance;
    clss.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    clss.hCursor = LoadCursor(hInstance, IDI_APPLICATION);
    clss.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    clss.lpszMenuName = NULL;
    clss.lpszClassName = L"pixelMain";
    ATOM pixelClass = RegisterClassW(&clss);
    if(pixelClass == 0) {errMsg = "RegisterClassW"; goto USD_ERR_FAILED;}

    mainHWnd = CreateWindowExW(0, (LPWSTR)MAKEINTATOM(pixelClass), L"plano cartesiano", wndStyle, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);
    if(mainHWnd == NULL) {errMsg = "CreateWindowExW"; goto USD_ERR_FAILED;}

    ShowWindow(mainHWnd, SW_SHOW);

    MSG msg;
    while(GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    exit(msg.lParam);

    USD_ERR_FAILED:
        printf("%s failed: %lu", errMsg, GetLastError());
        exit(0);
}

void init(void)
{
    char* err;
    consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

    csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    if(GetConsoleScreenBufferInfoEx(consoleOut, &csbi) == 0) {err = "GetConsoleScreenBufferInfoEx"; goto USD_ERR_FAILED;}

    if(SetConsoleCP(CP_UTF8) == 0) {err = "SetConsoleCP"; goto USD_ERR_FAILED;}
    if(SetConsoleOutputCP(CP_UTF8) == 0) {err = "SetConsoleOutputCP"; goto USD_ERR_FAILED;}

    if(CreateThread(NULL, 0, thread, NULL, 0, NULL) == NULL) {err = "CreateThread"; goto USD_ERR_FAILED;}
    return;

    USD_ERR_FAILED:
        printf("%s failed: %lu", err, GetLastError());
        exit(0);
}
#endif
