#include"thread.h"

#include<chrono>

degine::thread::multiThread::multiThread(double SPF, window::window& window,
										render::render& render,
										physics::physics& physics) :
	thread(SPF, window, render, physics),
	quit(false),
	renderThread(std::thread(this->renderThreadProc, this)),
	physicsThread(std::thread(this->physicsThreadProc, this))
{}

degine::thread::multiThread::~multiThread()
{
	quit = true;
	renderThread.join();
	physicsThread.join();
}

void degine::thread::multiThread::mainLoop()
{
	do {
		if(!SDL_WaitEvent(&event))
			throw std::runtime_error(
				std::string("failed to wait for SDL event: ")
										+(char*)SDL_GetError());
	} while(!eventHandler(event));
	quit = true;
}

bool degine::thread::multiThread::eventHandler(SDL_Event e)
{
	if(e.type == SDL_QUIT)
		return true;
	if(e.type == SDL_WINDOWEVENT)
	{
		window.windowEventHandler(e.window);
	}

	return false;
}

void degine::thread::multiThread::renderThreadProc()
{
	std::chrono::time_point<std::chrono::steady_clock> prev =
								std::chrono::steady_clock::now(), cur;
	while(!quit)
	{
		cur = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeElapsed = cur-prev;
		render.renderLoop(timeElapsed.count());
		if(timeElapsed.count() < SPF)
		{
			std::this_thread::sleep_for(std::chrono::duration<double>(SPF)
										-timeElapsed);
		}
		prev = cur;
	}
}
void degine::thread::multiThread::physicsThreadProc()
{
	std::chrono::time_point<std::chrono::steady_clock> prev =
								std::chrono::steady_clock::now(), cur;
	while(!quit)
	{
		cur = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeElapsed = cur-prev;
		physics.physicsLoop(timeElapsed.count());
		prev = cur;
	}
}
