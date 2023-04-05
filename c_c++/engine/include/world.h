#ifndef WORLD_H
#define WORLD_H

#include<cstdint>
#include<string>
#include<vector>
#include<boost\dynamic_bitset.hpp>
#include<chrono>
#include<limits>
#include<thread>
#include<mutex>

#include<SDL.h>

//due to race conditions on physics and render threads (before new colliding position is calculated but after calculation of new speed position), ghost sprites can appear when colliding two objects

namespace engine
{
	class base
	{
		public:
			base(std::string title, int w = 640, int h = 480, Uint32 windowFlags = SDL_WINDOW_SHOWN, Uint32 rendererFlags = SDL_RENDERER_ACCELERATED,
					int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED);
			base(base&) = delete;
			base& operator=(const base&) = delete;
			base& operator=(const base&&) = delete;
			virtual ~base();

			virtual bool eventLoop();
			bool eventHandler();
			virtual void eventLoopCleanup() {}

		protected:
			SDL_Window* window;
			SDL_Renderer* renderer;
			SDL_Event event;
			const Uint8* keyStates;
	};

	struct sprite {
			const SDL_Texture*& texture;
			const boost::dynamic_bitset<>& collisionTexture;
			SDL_Rect dimensions;
	};

	class object {
		public:
			object(const sprite& objectSprite, SDL_Rect position);
			virtual ~object() = default;
			//size_t getSpriteID();
			SDL_FRect getPosition();

			//virtual void loop(float timeElapsed, void* data) {};

		protected:
			SDL_FRect position;
			const sprite* objectSprite;
	};

	struct animation {
		double SPF = double();
		size_t curSprite = 0;
		std::vector<std::reference_wrapper<sprite>> sprites = std::vector<std::reference_wrapper<sprite>>();
	};

	class animObject : public object {
		public:
			using object::object;
			void insertAnimation(animation anim);

			virtual void animLoop(double timeElapsed, void* data);

			friend class entity;
		private:
			std::vector<animation> animations = std::vector<animation>();
			double time = 0.0;
	};

	class entity : public animObject {
		public:
			using animObject::animObject;
			void setSpeed(float speed, float speedX, float speedY);
			float getSpeed(float* speedX, float* speedY);
			void calculateSpeed(double timeElapsed);
			void testCollision(std::vector<boost::dynamic_bitset<>>& collisionsMaps, std::vector<object*> objects);

			virtual void loop(double timeElapsed, std::vector<boost::dynamic_bitset<>>& collisionsMaps, std::vector<object*> objects, void* data);

		private:
			float speedX = float(), speedY = float();
			SDL_FRect oldPosition;
	};

	class render : public base {
		public:
			render(std::string title, int w = 640, int h = 480, Uint32 windowFlags = SDL_WINDOW_SHOWN, Uint32 rendererFlags = SDL_RENDERER_ACCELERATED,
					int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED);
			virtual ~render();

			void loadImage(std::string filepath, bool createCollision = false);
			void loadSprite(SDL_Rect dimension);
			void loadSprite(SDL_Rect dimension, size_t texture);

			virtual void renderLoop(double timeElapsed) = 0;

		protected:
			std::vector<boost::dynamic_bitset<>> collisionMaps;
			std::vector<SDL_Texture*> textures;
			std::vector<sprite> sprites;
	};

	class physics : public render
	{
		public:
			using render::render;
			virtual ~physics() = default;
			virtual void physicsLoop(double timeElapsed) = 0;

			void loadObject();

		protected:
			std::vector<object*> objects = std::vector<object*>();
			std::vector<entity*> entities = std::vector<entity*>();
	};

	class game : public physics
	{
		public:
			game(std::string title, int w = 640, int h = 480, Uint32 rendererFlags = SDL_RENDERER_ACCELERATED, double FPS = std::numeric_limits<double>::max(),
					Uint32 windowFlags = SDL_WINDOW_SHOWN, int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED);
			virtual ~game() = default;

			virtual void setup() = 0;
			static void mainThread(game& game);
		private:
			void initThreads();
			void cleanupThreads();
			void signalQuit();
			void outerRenderLoop();
			void outerPhysicsLoop();

			std::thread physicsThread;
			std::thread renderThread;

			std::chrono::duration<double> SPF;
			std::chrono::duration<double> currentSPF;
			bool running = true;
	};
}

#endif // WORLD_H
