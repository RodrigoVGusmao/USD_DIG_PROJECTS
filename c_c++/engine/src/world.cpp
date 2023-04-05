#include "world.h"

#include<stdexcept>
#include<exception>

#include<cmath>

#include<SDL_image.h>

///base
engine::base::base(std::string title, int w, int h, Uint32 windowFlags, Uint32 rendererFlags, int x, int y) :
	window(),
	renderer(),
	event(),
	keyStates()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		goto EBD_ERR_SDL_INIT;
	}

	window = SDL_CreateWindow(title.c_str(), x, y, w, h, windowFlags);
	if(window == NULL)
	{
		goto EBD_ERR_SDL_CREATE_WINDOW;
	}

	renderer = SDL_CreateRenderer(window, -1, rendererFlags);
	if(renderer == NULL)
	{
		goto EBD_ERR_SDL_CREATE_RENDERER;
	}

	keyStates = SDL_GetKeyboardState(NULL);

	return;

	EBD_ERR_SDL_CREATE_RENDERER:
	SDL_DestroyWindow(window);
	EBD_ERR_SDL_CREATE_WINDOW:
	SDL_Quit();
	EBD_ERR_SDL_INIT:
	throw std::runtime_error(SDL_GetError());
}

engine::base::~base()
{
	eventLoopCleanup();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool engine::base::eventLoop()
{
	switch(event.type)
	{
	case SDL_QUIT:
		return false;
	}

	return true;
}

bool engine::base::eventHandler()
{
	SDL_WaitEvent(&event);

	return eventLoop();
}

///object
engine::object::object(const sprite& objectSprite, SDL_Rect position) : position(), objectSprite(&objectSprite)
{
	this->position = {static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.w), static_cast<float>(position.h)};
}

/*size_t engine::object::getSpriteID()
{
	return spriteID;
}*/

SDL_FRect engine::object::getPosition()
{
	return {position.x, position.y, position.w, position.h};
}

///animObject
void engine::animObject::insertAnimation(animation anim)
{
	animations.push_back(anim);
}

void engine::animObject::animLoop(double timeElapsed, void*)
{
	if(!animations.empty())
	{
		time += timeElapsed;
		if(time >= animations[0].SPF)
		{
			if(animations[0].curSprite == animations[0].sprites.size()-1)
			{
				animations[0].curSprite = 0;
			}
			objectSprite = &animations[0].sprites.at(++animations[0].curSprite);
			time = 0.0;
		}
	}
}

///entity
void engine::entity::setSpeed(float speed, float speedX, float speedY)
{
	if(speedX == 0.0 && speedY == 0.0)
	{
		this->speedX = 0.0;
		this->speedY = 0.0;
	} else
	{
		float multiplier = speed/sqrt(speedY*speedY+speedX*speedX);

		this->speedX = speedX*multiplier;
		this->speedY = speedY*multiplier;
	}
}

float engine::entity::getSpeed(float* speedX, float* speedY)
{
	if(speedX && speedY)
	{
		float mod = sqrt(this->speedY*this->speedY+this->speedX*this->speedX);
		if(speedX)
			*speedX = this->speedX/mod;
		if(speedY)
			*speedY = this->speedY/mod;
	}

	return sqrt(this->speedY*this->speedY+this->speedX*this->speedX);
}

void engine::entity::calculateSpeed(double timeElapsed)
{
	oldPosition = position;
	position.x += speedX*timeElapsed;
	position.y += speedY*timeElapsed;
}
#include<iostream>
void engine::entity::testCollision(std::vector<boost::dynamic_bitset<>>& collisionsMaps, std::vector<object*> objects)
{
	SDL_FRect testArea = {std::min(position.x, oldPosition.x), std::min(position.y, oldPosition.y), std::max(position.x, oldPosition.x)-testArea.x+position.w, std::max(position.y, oldPosition.y)-testArea.y+position.h};

	float slope = (position.y-oldPosition.y)/(position.x-oldPosition.x);
	float slopeI = (position.x-oldPosition.x)/(position.y-oldPosition.y);

	for(size_t i = 0; i < objects.size(); ++i)
	{
		SDL_FRect dummy, objectPos = objects[i]->getPosition();
		if(SDL_IntersectFRect(&objectPos, &testArea, &dummy) == SDL_FALSE)
		{
			continue;
		}

		if(i+1 == objects.size())
			std::cout << "collision detected: " << speedX << ' ' << speedY << " pos: " << position.x << '|' << position.y << " oldPos:" << oldPosition.x << '|' << oldPosition.y;
		else
			continue;

		SDL_FRect newPosition = position;
		if(speedX > 0.0)
		{
			float pY = slope*(objectPos.x-oldPosition.x+oldPosition.w) + oldPosition.y;
			float pY2 = slope*(objectPos.x-oldPosition.x) + oldPosition.y + oldPosition.h;
			if((pY >= objectPos.y && pY <= objectPos.y+objectPos.h) || (pY2 >= objectPos.y && pY2 <= objectPos.y+objectPos.h))
			{
				if(i+1 == objects.size())
						std::cout << " x positive collision detected: " << objectPos.x << ' ' << position.x+position.w;
				newPosition.x = objectPos.x-position.w;
				//positionX = {objectPos.x-position.w, pY, position.w, position.h};
				//position = oldPosition;
			}
		} else if(speedX < 0.0)
		{
			float pY = slope*(objectPos.x+objectPos.w-oldPosition.x+oldPosition.w) + oldPosition.y;
			float pY2 = slope*(objectPos.x+objectPos.w-oldPosition.x) + oldPosition.y + oldPosition.h;
			if((pY >= objectPos.y && pY <= objectPos.y+objectPos.h) || (pY2 >= objectPos.y && pY2 <= objectPos.y+objectPos.h))
			{
				if(i+1 == objects.size())
						std::cout << " x negative collision detected: " << objectPos.x+objectPos.w << ' ' << position.x;
				newPosition.x = objectPos.x+objectPos.w;
				//positionX = {objectPos.x+objectPos.w, pY, position.w, position.h};
				//position = oldPosition;
			}
		}

		if(speedY > 0.0)
		{
			float pX = slopeI*(objectPos.y-oldPosition.y+oldPosition.h) + oldPosition.x;
			float pX2 = slopeI*(objectPos.y-oldPosition.y) + oldPosition.x + oldPosition.w;
			if((pX >= objectPos.x && pX <= objectPos.x+objectPos.w) || (pX2 >= objectPos.x && pX2 <= objectPos.x+objectPos.w))
			{
				if(i+1 == objects.size())
						std::cout << " y positive collision detected: " << objectPos.y << ' ' << position.y+position.h;
				newPosition.y = objectPos.y-position.h;
				//positionY = {pX, objectPos.y-position.h, position.w, position.h};
				//position = oldPosition;
			}
		} else if(speedY < 0.0)
		{
			float pX = slopeI*(objectPos.y+objectPos.h-oldPosition.y+oldPosition.h) + oldPosition.x;
			float pX2 = slopeI*(objectPos.y+objectPos.h-oldPosition.y) + oldPosition.x + oldPosition.w;
			if((pX >= objectPos.x && pX <= objectPos.x+objectPos.w) || (pX2 >= objectPos.x && pX2 <= objectPos.x+objectPos.w))
			{
				if(i+1 == objects.size())
						std::cout << " y negative collision detected: " << objectPos.y+objectPos.h << ' ' << position.y;
				newPosition.y = objectPos.y+objectPos.h;
				//positionY = {pX, objectPos.y+objectPos.h, position.w, position.h};
				//position = oldPosition;
			}
		}

		//position = std::abs(positionX.x+positionX.y-oldPosition.x-oldPosition.y) < std::abs(positionY.x+positionY.y-oldPosition.x-oldPosition.y) ? positionX : positionY;
		position = newPosition;

		if(i+1 == objects.size())
			std::cout << //" posX: " << positionX.x << '|' << positionX.y << " posY:" << positionY.x << '|' << positionY.y <<
			 " pos: " << position.x << '|' << position.y << '|' << position.w << '|' << position.h /*<< " result comp: " <<
			 (std::abs(positionX.x+positionX.y-oldPosition.x-oldPosition.y) < std::abs(positionY.x+positionY.y-oldPosition.x-oldPosition.y))*/ << std::endl;
	}
}

void engine::entity::loop(double timeElapsed, std::vector<boost::dynamic_bitset<>>& collisionsMaps, std::vector<object*> objects, void*)
{
	calculateSpeed(timeElapsed);
	testCollision(collisionsMaps, objects);
}

///render
engine::render::render(std::string title, int w, int h, Uint32 windowFlags, Uint32 rendererFlags, int x, int y) :
	base(title, w, h, windowFlags, rendererFlags, x, y),
	collisionMaps(),
	textures(),
	sprites()
{
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		goto ERD_ERR_IMG_INIT;
	}

	return;

	ERD_ERR_IMG_INIT:
	throw std::runtime_error(IMG_GetError());
}

engine::render::~render()
{
	while(!textures.empty())
	{
		SDL_DestroyTexture(textures[textures.size()-1]);
		textures.pop_back();
	}
}

void engine::render::loadImage(std::string filepath, bool createCollision)
{
	{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	if(surface == NULL)
	{
		goto ERD_ERR_IMG_LOAD;
	}

	boost::dynamic_bitset<> collision;
	if(createCollision == true)
	{
		collision.resize(surface->h*surface->w);

		Uint8* bitmap = reinterpret_cast<Uint8*>(surface->pixels);
		for(size_t i = 0; i < surface->h*surface->w; ++i, bitmap += surface->format->BytesPerPixel)
		{
			Uint32 pixel = *reinterpret_cast<Uint32*>(bitmap);
			if(pixel & surface->format->Amask)
				collision.set(i);
		}
	}

	collisionMaps.push_back(collision);

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if(texture == NULL)
	{
		goto ERD_ERR_SDL_CREATE_TEXTURE_FROM_SURFACE;
	}

	textures.push_back(texture);
	}

	return;

	ERD_ERR_SDL_CREATE_TEXTURE_FROM_SURFACE:
	ERD_ERR_IMG_LOAD:
	throw std::runtime_error(IMG_GetError());
}

void engine::render::loadSprite(SDL_Rect dimension, size_t texture)
{
	engine::sprite spt;
	spt.index = texture;
	spt.dimensions = dimension;
	sprites.push_back(spt);
}

void engine::render::loadSprite(SDL_Rect dimension)
{
	loadSprite(dimension, textures.size()-1);
}

///game
engine::game::game(std::string title, int w, int h, Uint32 rendererFlags, double FPS, Uint32 windowFlags, int x, int y) :
	physics(title, w, h, windowFlags, rendererFlags, x, y),
	physicsThread(),
	renderThread(),
	SPF(std::chrono::duration<double>(1.f/FPS)),
	currentSPF(SPF) {}

void engine::game::initThreads()
{
	physicsThread = std::thread(this->outerPhysicsLoop, this);
	renderThread = std::thread(this->outerRenderLoop, this);
}

void engine::game::cleanupThreads()
{
	signalQuit();
	physicsThread.join();
	renderThread.join();
}

void engine::game::signalQuit()
{
	running = false;
}

void engine::game::outerRenderLoop()
{
	while(running)
	{
		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

		renderLoop(currentSPF.count());

		currentSPF = std::chrono::steady_clock::now() - start;
		if(currentSPF < SPF)
		{
			std::this_thread::sleep_for(SPF-currentSPF);
			currentSPF = SPF;
			//std::cout << "too fast! Sleeping for " << (SPF-currentSPF).count() << " seconds." << std::endl;
		}
		//std::cout << "SPF: " << SPF.count() << "\tFPS: " << 1.f/currentSPF.count() << std::endl;
	}
}
void engine::game::outerPhysicsLoop()
{
	std::chrono::time_point<std::chrono::steady_clock> prev = std::chrono::steady_clock::now(), cur;
	while(running)
	{
		cur = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeElapsed = cur - prev;
		physicsLoop(timeElapsed.count());
		//std::cout << "SPS: " << 1.f/timeElapsed.count() << std::endl;
		prev = cur;
	}
}

void engine::game::mainThread(game& game)
{
	game.setup();
	game.initThreads();
	while(game.eventHandler());
	game.cleanupThreads();
}
