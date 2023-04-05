#define GLEW_STATIC

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<stdio.h>
#include<stdarg.h>

#include "program.h"
#include "scene.h"
#include "shaderProgram.h"
#include "camera.h"
#include "errorHandling.h"

const int width = 640, height = 480;
const char* const title = "OpenGL Scene";

bool createWindow(const char* title, int width, int height, void* data);
GLboolean windowLoop(void* data);
void windowInput(void* data);
void freeWindow(void* data);
bool functionManagerInit(void);

void debugMessage(const char* msg, ...)
{
	va_list list;
	va_start(list, msg);
	vprintf(msg, list);
	va_end(list);
}

bool startup(programStruct* data)
{
	if (createWindow(title, width, height, &(data->window)) == false) return false;
	if (functionManagerInit() == false) return false;
	glViewport(0, 0, width, height);

	createShaderPrograms(&data->scene.camera.program);
	startupScene(&data->scene, (float)width / (float)height);

	return true;
}

bool loop(programStruct data)
{
	while (!windowLoop(data.window))
	{
		windowInput(data.window);

		cameraLoop(&data.scene.camera);

		renderScene(data.scene);
	}
	return true;
}

void cleanup(programStruct data)
{
	cleanupScene(data.scene);
	deleteShaderPrograms(data.scene.camera.program);
	freeWindow(data.window);
}

//sub-functions

bool functionManagerInit(void)
{
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		debugMessage((const char*)glewGetErrorString(error));
		return false;
	}

	return true;
}

bool createWindow(const char* title, int width, int height, void* data)
{
	TRY
	{
		if (glfwInit() == GLFW_FALSE)
			THROW(USD_ERR_GLFW_INIT, "Could not initialize GLFW\n");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		*(GLFWwindow**)data = glfwCreateWindow(width, height, title, NULL, NULL);
		if (*(GLFWwindow**)data == NULL)
			THROW(USD_ERR_GLFW_CREATE_WINDOW, "Could not create GLFW window\n");

		glfwMakeContextCurrent(*(GLFWwindow**)data);
		return true;
	}
	CATCH(USD_ERR_GLFW_INIT)
		glfwTerminate();

	CATCH(USD_ERR_GLFW_CREATE_WINDOW)
		debugMessage(GET_MSG());
	return false;
}

GLboolean windowLoop(void* data)
{
	glfwSwapBuffers((GLFWwindow*)data);
	glfwPollEvents();
	return glfwWindowShouldClose((GLFWwindow*)data);
}

#define USD_WIREFRAME 3
#define USD_PRESS 4

void windowInput(void* data)
{
	static int flags = USD_WIREFRAME;

	if (glfwGetKey((GLFWwindow*)data, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose((GLFWwindow*)data, GL_TRUE);

	if (glfwGetKey((GLFWwindow*)data, GLFW_KEY_F1) == GLFW_PRESS)
		flags |= USD_PRESS;

	if ((flags & 4) && glfwGetKey((GLFWwindow*)data, GLFW_KEY_F1) == GLFW_RELEASE)
	{
		flags ^= USD_WIREFRAME | USD_PRESS;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE ^ flags);
	}
}

void freeWindow(void* data)
{
	glfwDestroyWindow((GLFWwindow*) data);
	glfwTerminate();
}