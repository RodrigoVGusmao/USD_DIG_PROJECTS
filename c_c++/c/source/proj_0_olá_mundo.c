#define UNICODE
#define _UNICODE

#include<stdio.h>

#ifdef _WIN32
#include<windows.h>
#endif

int main()
{
    #ifdef _WIN32
    if(!SetConsoleOutputCP(CP_UTF8))    return 1;
    if(!SetConsoleCP(CP_UTF8))          return 1;
    #endif

    printf("olá mundo!!");

    getchar();

    return 0;
}
