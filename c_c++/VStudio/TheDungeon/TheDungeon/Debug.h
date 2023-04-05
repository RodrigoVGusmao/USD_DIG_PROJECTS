#include<windows.h>

#include<cstdlib>
#include<cstdarg>
#include<string>

#ifndef _UNICODE
#define DebugWin aDebugWin
#define vDebugWin vaDebugWin
#endif

#ifdef _UNICODE
#define DebugWin wDebugWin
#define vDebugWin vwDebugWin
#endif //

#ifndef MSGBOXDEBUG
int vwDebugWin(const wchar_t* title, const wchar_t* text, va_list args)
{
	std::wstring output;

	int size = vwprintf(text, args);
	if (size < 0)
		return size;

	output.resize(size + 1);
	int returnValue = vswprintf(const_cast<wchar_t*>(output.c_str()), output.size(), text, args);

	MessageBoxW(NULL, output.c_str(), title, MB_OK);

	return returnValue;
}

int wDebugWin(const wchar_t* title, const wchar_t* text, ...)
{
	va_list args;
	va_start(args, text);
	int returnValue = vwDebugWin(title, text, args);
	va_end(args);

	return returnValue;
}

int vaDebugWin(const char* title, const char* text, va_list args)
{
	std::string output;

	int size = vprintf(text, args);
	if (size < 0)
		return size;

	output.resize(size + 1);
	int returnValue = vsnprintf(const_cast<char*>(output.c_str()), output.size(), text, args);

	MessageBoxA(NULL, output.c_str(), title, MB_OK);

	return returnValue;
}

int aDebugWin(const char* title, const char* text, ...)
{
	va_list args;
	va_start(args, text);
	int returnValue = vaDebugWin(title, text, args);
	va_end(args);

	return returnValue;
}
#endif

#define MSGBOXDEBUG