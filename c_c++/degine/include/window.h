#ifndef WINDOW_H
#define WINDOW_H

#include<string>

#include <SDL.h>

//creates and prepares window
namespace degine
{
	namespace window
	{
		class window
		{
			public:
			virtual ~window() {};
			virtual void windowEventHandler(SDL_WindowEvent) {};
			SDL_Window* getWindow();

			protected:
			SDL_Window* pWindow = NULL;

			private:
		};

		class GLWindow : public window
		{
			public:
				GLWindow(const std::string& title = "SDL with OpenGL window",
							int w = 640, int h= 480, bool vSync = true,
							Uint32 flags = SDL_WINDOW_SHOWN);
				virtual void windowEventHandler(SDL_WindowEvent e);
				virtual ~GLWindow();

			protected:
			SDL_GLContext glContext;

			private:
		};
	}
}

#endif // WINDOW_H
