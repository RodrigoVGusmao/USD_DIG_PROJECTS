#include "FPSHand.h"

#include<SDL.h>

using namespace sdlFps;
FPSHand::FPSHand()
{
    startTime = SDL_GetTicks();
    delta = 0;
    fps = 0;
}

void FPSHand::frameRateCalc()
{
    delta = SDL_GetTicks()-startTime;
    fps = 1000/delta;
    startTime = SDL_GetTicks();
}

void FPSHand::capFPS(uint16_t fpscap)
{
    if(SDL_GetTicks()-startTime < 1000/fpscap)
        SDL_Delay(1000/fpscap -SDL_GetTicks()+startTime);
}
