#if defined(__WIN32) || defined(__WIN64) || defined(__WIN32__) || defined(__WIN64__)
#include<windows.h>
int utf8_ready()
{
	return ~0 & SetConsoleCP(CP_UTF8) & SetConsoleOutputCP(CP_UTF8);
}
#else
int utf8_ready()
{
	return 1;
}
#endif

#include<stdio.h>
#include<inttypes.h>

struct data
{
	uint8_t idade;
	void* sistema;

}

int main()
{
	if(fopen("USER_ID", "r") == NULL)

	if(!utf8_ready()) goto error;
	while(1)
	{

	}

	error:
	return 1;
}
