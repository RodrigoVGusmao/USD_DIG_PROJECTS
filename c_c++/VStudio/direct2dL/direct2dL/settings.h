#pragma once
#include<windows.h>

/***************
window variables
***************/
const int windowPosX = CW_USEDEFAULT;
const int windowPosY = CW_USEDEFAULT;
const bool fullscreen = false;

//should not use CW_USEDEFAULT
//#define SET_SIZE_AS_CLIENT
const int windowWidth = 640;
const int windowHeight = 480;

/***************
input variables
***************/
#define MAX_CONTROLLERS 4

#ifdef SET_SIZE_AS_CLIENT
#define GET_WINDOW_SIZE(lpRect, dwStyle, hasMenu) AdjustWindowRect((lpRect), (dwStyle), (hasMenu))
#else
#define GET_WINDOW_SIZE(lpRect, dwStyle, hasMenu) 1
#endif // SET_SIZE_AS_CLIENT
