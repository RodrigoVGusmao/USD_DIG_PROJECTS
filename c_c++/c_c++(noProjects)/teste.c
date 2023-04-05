#define GLEW_STATIC

//c
#include<stdio.h>

//graphics
#include<SDL.h>
#include<glew.h>
#include<SDL_opengl.h>

//constants
//#define bool char
//#define true 1
//#define false 0
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

bool init(SDL_Window** window)
{
    SDL_GLContext context;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("couldn't initialize SDL:%s", SDL_GetError());
        return true;
    }

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    *window = SDL_CreateWindow("learnOpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        printf("couldn't create SDL window:%s", SDL_GetError());
        return true;
    }

    context = SDL_GL_CreateContext(*window);
    if(context == NULL)
    {
        printf("couldn't create SDL context:%s", SDL_GetError());
        return true;
    }

    if(SDL_GL_SetSwapInterval(1) < 0)
    {
        printf("couldn't set Vsync:%s\npress 'enter' to continue...", SDL_GetError());
        getchar();
    }

    return false;
}

bool initGlew()
{
    glewExperimental= GL_TRUE;
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        printf("couldn't initialize GLEW:%s", glewGetErrorString(error));
        return true;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    return false;
}

int main(int argc, char* argv[])
{
    SDL_Window *window;

    init(&window);
    initGlew();

    return 0;
}
