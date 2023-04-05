#pragma once

//generic errors
#define US_ERR_GENERIC_OK 0
#define US_ERR_GENERIC_OK_MSG "Everything went well!"

#define US_ERR_GENERIC_ALREADY_RUNNING 1
#define US_ERR_GENERIC_ALREADY_RUNNING_MSG "There is another instance of the program running!"

//window errors
#define US_ERR_WINDOW_UNKNOW 0x100
#define US_ERR_WINDOW_UNKNOW_MSG "Something went wrong with the window!"

#define US_ERR_WINDOW_CLASS 0x101
#define US_ERR_WINDOW_CLASS_MSG "Could not create a new window class!"

#define US_ERR_WINDOW_ADJUST 0x102
#define US_ERR_WINDOW_ADJUST_MSG "Could not adjust window size!"

#define US_ERR_WINDOW_CREATE 0x103
#define US_ERR_WINDOW_CREATE_MSG "Could not create a new window!"

//direct errors
#define US_ERR_DIRECTX9_UNKNOW 0x200
#define US_ERR_DIRECTX9_UNKNOW_MSG "Something went wrong with directX9!"

#define US_ERR_DIRECTX9_INVALID_INTERFACE 0x201
#define US_ERR_DIRECTX9_INVALID_INTERFACE_MSG "Invalid directX9 interface!"

#define US_ERR_DIRECTX9_CREATE_INTERFACE 0x202
#define US_ERR_DIRECTX9_CREATE_INTERFACE_MSG "Could not create directX9 interface!"

#define US_ERR_DIRECTX9_INVALID_DEVICE 0x203
#define US_ERR_DIRECTX9_INVALID_DEVICE_MSG "Invalid directX9 device!"

#define US_ERR_DIRECTX9_CREATE_DEVICE 0x204
#define US_ERR_DIRECTX9_CREATE_DEVICE_MSG "Could not create directX9 device!"

#define US_ERR_DIRECTX9_RESET_DEVICE 0x204
#define US_ERR_DIRECTX9_RESET_DEVICE_MSG "Could not reset directX9 device!"

#define US_ERR_DIRECTX9_CLEAR_BACKGROUND 0x206
#define US_ERR_DIRECTX9_CLEAR_BACKGROUND_MSG "Could not clear directx9 device's background!"

#define US_ERR_DIRECTX9_PAINT 0x207
#define US_ERR_DIRECTX9_PAINT_MSG "Could not paint directx9 device!"

#define US_ERR_DIRECTX9_BEGIN_SCENE 0x208
#define US_ERR_DIRECTX9_BEGIN_SCENE_MSG "Could not begin directx9 scene!"

#define US_ERR_DIRECTX9_END_SCENE 0x209
#define US_ERR_DIRECTX9_END_SCENE_MSG "Could not end directx9 scene!"

//high resolution timer errors
#define US_ERR_HIGH_RES_TIMER_UNKNOW 0x300
#define US_ERR_HIGH_RES_TIMER_UNKNOW_MSG "Something went wrong with high resolution timer!"

#define US_ERR_HIGH_RES_TIMER_CREATE 0x301
#define US_ERR_HIGH_RES_TIMER_CREATE_MSG "Could not create high resolution timer!"

#define US_ERR_HIGH_RES_TIMER_GET 0x302
#define US_ERR_HIGH_RES_TIMER_GET_MSG "Could not get high resolution timer time!"