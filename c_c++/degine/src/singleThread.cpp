#include"thread.h"

degine::thread::singleThread::~singleThread(){}

void degine::thread::singleThread::mainLoop()
{
	bool quit = false;
	std::chrono::time_point<std::chrono::steady_clock> prev = std::chrono::steady_clock::now(), cur;
	do {
		cur = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeElapsed = cur-prev;
		physics.physicsLoop(timeElapsed.count());
		render.renderLoop(timeElapsed.count());
		prev = cur;

		if(SDL_PollEvent(&event))
			quit = eventHandler(event);
	} while(!quit);
}
bool degine::thread::singleThread::eventHandler(SDL_Event e)
{
	if(e.type == SDL_QUIT)
		return true;
	if(e.type == SDL_WINDOWEVENT)
	{
		window.windowEventHandler(e.window);
	}

	return false;
}
