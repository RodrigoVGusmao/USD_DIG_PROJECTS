#include"pixel.h"

#ifndef WINDOW
#include<stdio.h>

#define USD_VERTICAL "\u2502"           ///2502, 2503 for bold
#define USD_HORIZONTAL "\u2500"         ///2500, 2501 for bold
#define USD_CENTER "\u253C"             ///253C, 254B dor full bold, 253F for horizontal bold, 2542 for vertical bold
#define USD_PIXEL "\u2588"

static POINTS aspectRatio = {2, 1};
static HANDLE consoleOut;
static RECT viewport;
static CONSOLE_SCREEN_BUFFER_INFOEX csbi;
static SHORT line;

int printPixel(int x, int y)
{
    if(x < viewport.left || -y < viewport.top || x >= viewport.right || -y >= viewport.bottom)
        return -1;

    COORD position = {aspectRatio.x*(x-viewport.left)-aspectRatio.x/2, aspectRatio.y*(-y-viewport.top)-aspectRatio.y/2};
    for(SHORT i = 0; i < aspectRatio.y; ++i, ++position.Y)
    {
        SetConsoleCursorPosition(consoleOut, position);
        for(SHORT j = 0; j < aspectRatio.x; ++j)
        {
            printf(USD_PIXEL);
        }
    }

    return 0;
}

void setPixelAspect(SHORT x, SHORT y)
{
    aspectRatio.x = x;
    aspectRatio.y = y;
}

void setViewport(int minX, int minY, int maxX, int maxY)
{
    viewport.left = minX;
    viewport.top = minY;
    viewport.right = maxX;
    viewport.bottom = maxY;
}

void printCartesianPlane(void)
{
    line = 0;
    COORD writeTarget = {0, 0};
    DWORD out;
    FillConsoleOutputCharacter(consoleOut, ' ', csbi.dwSize.X*csbi.dwMaximumWindowSize.Y, writeTarget, &out);

    COORD position = {-viewport.left*aspectRatio.x, 0};
    for(SHORT i = aspectRatio.y*(viewport.bottom-viewport.top); i > 0; --i)
    {
        SetConsoleCursorPosition(consoleOut, position);
        printf(USD_VERTICAL);
        ++position.Y;
    }

    position.X = 0;
    position.Y = -viewport.top*aspectRatio.y;
    SetConsoleCursorPosition(consoleOut, position);
    for(SHORT i = aspectRatio.x*(viewport.right-viewport.left); i > 0; --i)
    {
        printf(USD_HORIZONTAL);
    }

    position.X = -viewport.left*aspectRatio.x;
    SetConsoleCursorPosition(consoleOut, position);
    printf(USD_CENTER);
}

void resetLine(void)
{
    if(GetConsoleScreenBufferInfoEx(consoleOut, &csbi) == 0) goto USD_ERR_FAILED;

    COORD writeTarget = {0, 0};
    DWORD out;
    FillConsoleOutputCharacter(consoleOut, ' ', csbi.dwSize.X*csbi.dwCursorPosition.Y+csbi.dwCursorPosition.X, writeTarget, &out);

    line = 0;
    return;

    USD_ERR_FAILED:
        printf("GetConsoleScreenBufferInfoEx failed: %lu", GetLastError());
        exit(0);
}

void fitTextRight(void)
{
    COORD position = {aspectRatio.x*(viewport.right-viewport.left)+1, line++};
    SetConsoleCursorPosition(consoleOut, position);
}

void setColorScheme(COLORREF foreground, COLORREF background)
{
    csbi.ColorTable[0] = background;
    csbi.ColorTable[7] = foreground;

    if(SetConsoleScreenBufferInfoEx(consoleOut, &csbi) == 0) goto USD_ERR_FAILED;

    return;
    USD_ERR_FAILED:
        printf("SetConsoleScreenBufferInfoEx failed with code: %lu", GetLastError());
        exit(0);
}

#ifdef _WIN32
void init(void)
{
    char* err;

    consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

    csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    if(GetConsoleScreenBufferInfoEx(consoleOut, &csbi) == 0) {err = "GetConsoleScreenBufferInfoEx"; goto USD_ERR_FAILED;}

    csbi.ColorTable[0] = RGB(0, 0, 0);
    csbi.ColorTable[7] = RGB(255, 255, 255);

    if(SetConsoleScreenBufferInfoEx(consoleOut, &csbi) == 0) {err = "SetConsoleScreenBufferInfoEx"; goto USD_ERR_FAILED;}

    if(SetConsoleCP(CP_UTF8) == 0) {err = "SetConsoleCP"; goto USD_ERR_FAILED;}
    if(SetConsoleOutputCP(CP_UTF8) == 0) {err = "SetConsoleOutputCP"; goto USD_ERR_FAILED;}

    return;

    USD_ERR_FAILED:
        printf("%s failed with code: %lu", err, GetLastError());
        exit(0);
}
#else
void init(void) {}
#endif
#endif
