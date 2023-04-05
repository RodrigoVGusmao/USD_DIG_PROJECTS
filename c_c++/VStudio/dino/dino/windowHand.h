#ifndef WINDOWHAND_H
#define WINDOWHAND_H

#include<cstdint>
#include<iostream>
#include<string>

#include<SDL.h>

namespace sdlWindow
{
class WindowHand
{
    public:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        WindowHand(std::string, int32_t, int32_t, int32_t, int32_t);
         void setWinColor(uint8_t, uint8_t, uint8_t);
         void setWinColor(uint8_t);
         void blitSurfaceToCenter(SDL_Texture*, SDL_Rect);
         void blitSurface(SDL_Texture*, const SDL_Rect*, const SDL_Point*);
        ~WindowHand();
    private:
        int16_t winWidth;
        int16_t winHeight;
};
}

#endif // WINDOWHAND_H
