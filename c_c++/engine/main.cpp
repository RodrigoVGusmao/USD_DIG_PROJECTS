#include<exception>
#include<random>
#include<iostream>

#include "world.h"

class mainCharacter : public engine::entity
{
	public:
		using engine::entity::entity;

		void loop(double timeElapsed, std::vector<boost::dynamic_bitset<>>& collisionsMaps, std::vector<object*> objects, void* character)
		{
			uint_least8_t movement = static_cast<uint_least8_t>(reinterpret_cast<intptr_t>(character));
			double xSpeed = 0.0, ySpeed = 0.0;
			if(movement & 0x4)
				xSpeed = movement & 0x1 ? 1.0 : -1.0;
			if(movement & 0x8)
				ySpeed = movement & 0x2 ? 1.0 : -1.0;

			setSpeed(200.0, xSpeed, ySpeed);

			calculateSpeed(timeElapsed);
			testCollision(collisionsMaps, objects);
		}
};

class rendering : public engine::game
{
	public:
		using engine::game::game;

		void setup()
		{
			//loadImage("testPanelDuh.png");

			loadImage("spriteSheetNumbers.png");
			for(size_t i = 0; i < 100; ++i)
			{
				loadSprite({i%10*50, i/10*50, 50, 50});
				objects.push_back(new engine::object(sprites.back(), {(99-i)%10*50, (99-i)/10*50, 50, 50}));
			}

			loadImage("spriteSheetTransparency.png", true);
			size_t playerSpritesId = sprites.size()-1;
			for(size_t i = 0; i < 16; ++i)
			{
				loadSprite({i%4*50, i/4*50, 50, 50});
			}

			std::random_device dev;
			std::uniform_int_distribution<int> distribution(0,500);

			entities.push_back(new mainCharacter(sprites.at(100), {distribution(dev), distribution(dev), 50, 50}));
			entities[entities.size()-1]->setSpeed(200.f, 1.f, 1.f);

			engine::animation ballAnim;
			ballAnim.SPF = 1.0/5.0;
			for(size_t i = playerSpritesId; i < sprites.size(); ++i)
				ballAnim.sprites.push_back(sprites.at(i));

			entities.back()->insertAnimation(ballAnim);

			objects.push_back(new engine::object(101, {250, 250, 50, 50}));
		}

		bool eventLoop()
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				characterMovement |= 0x10;
				break;

			case SDL_QUIT:
				return false;
			}

			characterMovement = ((static_cast<bool>(keyStates[SDL_SCANCODE_D])) << 0) |
							((static_cast<bool>(keyStates[SDL_SCANCODE_S])) << 1) |
							((static_cast<bool>(keyStates[SDL_SCANCODE_D]) ^ static_cast<bool>(keyStates[SDL_SCANCODE_A])) << 2) |
							((static_cast<bool>(keyStates[SDL_SCANCODE_S]) ^ static_cast<bool>(keyStates[SDL_SCANCODE_W])) << 3);

			return true;
		}

		void renderLoop(double timeElapsed)
		{
			SDL_RenderClear(renderer);
			//SDL_Rect dst = {0, 0, 0, 0};

			//SDL_QueryTexture(textures[0], NULL, NULL, &dst.w, &dst.h);

			//SDL_RenderCopy(renderer, textures[0], NULL, &dst);

			/*for(size_t i = 0; i < 100; ++i)
			{
				SDL_RenderCopy(renderer, textures[sprites[i]->index], &sprites[i]->dimensions, &sprites[99-i]->dimensions);
			}*/

			for(size_t i = 0; i < objects.size(); ++i)
			{
				SDL_FRect fdst = objects[i]->getPosition();
				SDL_Rect dst = {static_cast<int>(fdst.x), static_cast<int>(fdst.y), static_cast<int>(fdst.w), static_cast<int>(fdst.h)};
				SDL_RenderCopy(renderer, textures[sprites[objects[i]->getSpriteID()].index], &sprites[objects[i]->getSpriteID()].dimensions, &dst);
			}

			for(size_t i = 0; i < entities.size(); ++i)
			{
				entities[i]->animLoop(timeElapsed, NULL);
				SDL_FRect fdst = entities[i]->getPosition();
				SDL_Rect dst = {static_cast<int>(fdst.x), static_cast<int>(fdst.y), static_cast<int>(fdst.w), static_cast<int>(fdst.h)};
				SDL_RenderCopy(renderer, textures[sprites[entities[i]->getSpriteID()].index], &sprites[entities[i]->getSpriteID()].dimensions, &dst);
			}

			SDL_RenderPresent(renderer);
		}

		void physicsLoop(double timeElapsed)
		{
			for(size_t i = 0; i < entities.size(); ++i)
			{
				entities[i]->loop(timeElapsed, collisionMaps, objects, reinterpret_cast<void*>(characterMovement));
				/*SDL_Rect pos = entities[i]->getPosition();
				float speedX, speedY;
				float speed = entities[i]->getSpeed(&speedX, &speedY);
				if(pos.x >= 500-pos.w || pos.x <= 0)
					speedX *= -1.f;
				if(pos.y >= 500-pos.h || pos.y <= 0)
					speedY *= -1.f;
				entities[i]->setSpeed(speed, speedX, speedY);*/
			}
			SDL_Delay(50);
		}

	private:
		uint_least8_t characterMovement = 0;
};

int main( int, char*[] )
{
	try {
		rendering game("SDL window", 500, 500, SDL_RENDERER_PRESENTVSYNC, 60);
		rendering::mainThread(game);
	}catch(std::exception& e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "error", e.what(), nullptr);
		return 1;
	}

    return 0;
}
