#define GLEW_STATIC

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"program.h"

int main()
{
	programStruct data;
	if (startup(&data) == false)
	{
		return 1;
	}

	loop(data);

	cleanup(data);

	return 0;
}