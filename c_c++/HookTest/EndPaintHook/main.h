#ifndef __MAIN_H__
#define __MAIN_H__

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <inttypes.h>
#include <stdio.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void DLL_EXPORT DLLloaded();
WINBOOL DLL_EXPORT __stdcall EndPaintHook(HWND hwnd, LPPAINTSTRUCT ps);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
