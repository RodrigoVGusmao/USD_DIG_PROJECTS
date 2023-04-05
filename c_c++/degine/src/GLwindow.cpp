//#define GLEW_STATIC

#include "window.h"

#include<string>
#include<stdexcept>

#include<SDL.h>
#include<glew.h>
#include<SDL_opengl.h>

#include"ctrycatch.h"

GLenum initGlew()
{
	glewExperimental = GL_TRUE;
	return glewInit();
}
// 4.1 ~= GLES
degine::window::GLWindow::GLWindow(const std::string& title, int w, int h,
								bool vSync, Uint32 flags)
{
	GLenum error = GLEW_OK;
	TRY
	{
		THROW_ON_FAIL(SDL_Init(SDL_INIT_VIDEO) < 0, WDD_ERR_SDL_INIT);

		THROW_ON_FAIL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) ||
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1) ||
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
							SDL_GL_CONTEXT_PROFILE_CORE),
						WDD_ERR_SDL_GL_SET_ATTRIBUTE);

		THROW_ON_FAIL((pWindow = SDL_CreateWindow(title.c_str(),
					SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
					flags | SDL_WINDOW_OPENGL)) == NULL,
						WDD_ERR_SDL_CREATE_WINDOW);

		THROW_ON_FAIL((glContext = SDL_GL_CreateContext(pWindow)) == NULL,
						WDD_ERR_SDL_GL_CREATE_CONTEXT);

		THROW_ON_FAIL(SDL_GL_SetSwapInterval(vSync) < 0,
						WDD_ERR_SDL_GL_SET_SWAP_INTERVAL);

		THROW_ON_FAIL((error = initGlew()) != GLEW_OK, WDD_ERR_USD_INIT_GLEW);

		glViewport(0, 0, w, h);

		return;
	}FALL_THROUGH_CATCH(WDD_ERR_USD_INIT_GLEW, (
								std::string("failed to initialize glew: ")
								+ (char*)glewGetErrorString(error)).c_str())
	{

	}
	FALL_THROUGH_CATCH(WDD_ERR_SDL_GL_SET_SWAP_INTERVAL, (
								std::string("failed to set vSync: ")
								+ SDL_GetError()).c_str())
	{
		SDL_GL_DeleteContext(glContext);
	}
	FALL_THROUGH_CATCH(WDD_ERR_SDL_GL_CREATE_CONTEXT, (
								std::string("failed to create GL context: ")
								+ SDL_GetError()).c_str())
	{
		SDL_DestroyWindow(pWindow);
	}
	FALL_THROUGH_CATCH(WDD_ERR_SDL_CREATE_WINDOW, (
								std::string("failed to create SDL window: ")
								+ SDL_GetError()).c_str())
	{

	}
	FALL_THROUGH_CATCH(WDD_ERR_SDL_GL_SET_ATTRIBUTE, (
								std::string("failed to set OpenGL Attributes: ")
								+ SDL_GetError()).c_str())
	{
		SDL_Quit();
	}
	FALL_THROUGH_CATCH(WDD_ERR_SDL_INIT, (
								std::string("failed to init SDL: ")
								+ SDL_GetError()).c_str())
	{
		throw std::runtime_error(WHAT);
	}
}

degine::window::GLWindow::~GLWindow()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

void degine::window::GLWindow::windowEventHandler(SDL_WindowEvent e)
{
	switch(e.event)
	{
	case SDL_WINDOWEVENT_RESIZED:
		glViewport(0, 0, e.data1, e.data2);
		break;
	}
}
