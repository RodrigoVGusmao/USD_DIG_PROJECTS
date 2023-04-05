#include"render.h"

#include<string>
#include<stdexcept>

#include<SDL.h>
#include<SDL_image.h>

#include"ctrycatch.h"

unsigned int degine::render::render::refCount = 0;

degine::render::render::render(object::objects& objects, int imgFlags) :
objects(objects)
{
	TRY {
		++refCount;
		THROW_ON_FAIL(IMG_Init(imgFlags) & imgFlags != imgFlags,
						RDD_ERR_IMG_INIT);

		return;
	}FALL_THROUGH_CATCH(RDD_ERR_IMG_INIT, (
								std::string("failed to initialize SDL_image: ")
								+ (char*)IMG_GetError()).c_str())
	{
		throw std::runtime_error(WHAT);
	}
}

degine::render::render::~render()
{
	if(--refCount == 0)
		IMG_Quit();
}

SDL_Surface* degine::render::render::loadImage(std::string filePath)
{
	SDL_Surface *loadedImage, *compatibleFormat;
	TRY {
		loadedImage = IMG_Load(filePath.c_str());
		THROW_ON_FAIL(loadedImage == NULL, RDD_ERR_IMG_LOAD);

		compatibleFormat = SDL_ConvertSurfaceFormat(loadedImage,
												SDL_PIXELFORMAT_RGBA32, 0);
		THROW_ON_FAIL(compatibleFormat == NULL, RDD_ERR_CONVERT_SURFACE_FORMAT);

		return compatibleFormat;
	}FALL_THROUGH_CATCH(RDD_ERR_CONVERT_SURFACE_FORMAT, (
								std::string("failed to load ") + filePath + ": "
								+ (char*)SDL_GetError()).c_str())
	{
		SDL_FreeSurface(loadedImage);
	}
	FALL_THROUGH_CATCH(RDD_ERR_IMG_LOAD, (
								std::string("failed to load ") + filePath + ": "
								+ (char*)SDL_GetError()).c_str())
	{
		throw std::runtime_error(WHAT);
	}
}
