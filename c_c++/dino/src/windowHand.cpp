#include "WindowHand.h"

using namespace sdlWindow;
WindowHand::WindowHand(std::string winName, int32_t xPos, int32_t yPos, int32_t width, int32_t height)
{
    window = SDL_CreateWindow(winName.c_str(), xPos, yPos, width, height, SDL_WINDOW_SHOWN);
    if(window == NULL)
        throw 3;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
        throw 4;

    winWidth = width;
    winHeight = height;
}

void WindowHand::setWinColor(uint8_t red, uint8_t green, uint8_t blue)
{
    SDL_SetRenderDrawColor(renderer, red, green, blue, 0);
}

void WindowHand::setWinColor(uint8_t color)
{
    SDL_SetRenderDrawColor(renderer, color, color, color, 0);
}

void WindowHand::blitSurfaceToCenter(SDL_Texture* texture, SDL_Rect clip)
{
    SDL_Rect center = {.x = (winWidth-clip.w)/2,
                       .y = (winHeight-clip.h)/2,
                       .w = clip.w,
                       .h = clip.h};

    SDL_RenderCopy(renderer, texture, &clip, &center);
}

void WindowHand::blitSurface(SDL_Texture* texture, const SDL_Rect* clip, const SDL_Point* position)
{
    SDL_Rect imgAttr = {position->x, position->y, clip->w, clip->h};
    SDL_RenderCopy(renderer, texture, clip, &imgAttr);
}

WindowHand::~WindowHand()
{
    SDL_RenderClear(renderer);
    SDL_DestroyWindow(window);
}
