#ifndef MISC_U_H
#define MISC_U_H

#if defined(__WIN32) || defined(__WIN64) || defined(_WIN32_) || defined(__WIN64__)
#include<windows.h>
int utf8_ready()
{
	return (SetConsoleCP(CP_UTF8) && SetConsoleOutputCP(CP_UTF8));
}
int changeFont(wchar_t* fontName)
{
    struct _CONSOLE_FONT_INFOEX* fontInfo = calloc(1, sizeof(CONSOLE_FONT_INFOEX)+wcslen(fontName)*sizeof(wchar_t));
	fontInfo->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	memcpy(&fontInfo->FaceName, fontName, wcslen(fontName) * sizeof(wchar_t));
    return SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, fontInfo);
}
#else
int utf8_ready()
{
	return 1;
}
int changeFont(wchar_t* fontName)
{
    return 1;
}
#endif // defined

#define TRY char* _try_error_message = "ERR_OK";
#define THROW(s, type) {_try_error_message = s; goto _##type##_error_u;}
#define GET_ERROR() fprintf(stderr, _try_error_message)
#define CATCH(type) if(0) _##type##_error_u:

#endif
