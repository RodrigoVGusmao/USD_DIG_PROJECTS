#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include"window.h"
#include"render.h"
#include"physics.h"

#include<thread>

namespace degine
{
	namespace thread
	{
		class thread
		{
			public:
			thread(double SPF, window::window& window, render::render& render,
					physics::physics& physics) :
				event(),
				SPF(SPF),
				window(window),
				render(render),
				physics(physics) {};
			virtual ~thread() {};
			virtual void mainLoop() = 0;
			virtual bool eventHandler(SDL_Event e) = 0;

			protected:
			SDL_Event event;
			double SPF;

			degine::window::window& window;
			degine::render::render& render;
			degine::physics::physics& physics;

			private:
		};

		class multiThread : public thread
		{
			public:
			multiThread(double SPF, window::window& window,
						render::render& render, physics::physics& physics);
			virtual ~multiThread();
			virtual void mainLoop();
			virtual bool eventHandler(SDL_Event e);

			void renderThreadProc();
			void physicsThreadProc();

			protected:
			bool quit;

			std::thread renderThread;
			std::thread physicsThread;

			private:
		};
		class singleThread : public thread
		{
			public:
			using degine::thread::thread::thread;
			virtual ~singleThread();
			virtual void mainLoop();
			virtual bool eventHandler(SDL_Event e);

			protected:

			private:
		};
	}
}

#endif // THREAD_H_INCLUDED
