#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include<math.h>

///clips
const Uint8 colorKey[] = {0x40, 0xCA, 0xC9};
const SDL_Rect cloudClip = {86, 2, 46, 13};
const SDL_Rect groundClip = {2, 54, 1200, 12};
const SDL_Rect cactusClip[] = {
    {228, 2, 17, 35},
    {245, 2, 34, 35},
    {279, 2, 51, 35},
    {332, 2, 25, 50},
    {357, 2, 50, 50},
    {407, 2, 75, 50}
};
const SDL_Rect characterClip[] = {
    {936, 2, 44, 47},
    {980, 2, 44, 47},
    {1068, 2, 44, 47},
    {1112, 2, 59, 47},
    {1171, 2, 59, 47}
};
const SDL_Rect numbers[] = {
    {655, 2, 9, 11},
    {665, 2, 9, 11},
    {675, 2, 9, 11},
    {685, 2, 9, 11},
    {695, 2, 9, 11},
    {705, 2, 9, 11},
    {715, 2, 9, 11},
    {725, 2, 9, 11},
    {735, 2, 9, 11},
    {745, 2, 9, 11}
};
const SDL_Rect highScore[] = {
    {755, 2, 9, 11},
    {765, 2, 9, 11}
};

///global
const uint16_t winWidth = 640;
const uint16_t winHeight = 480;
const uint16_t fpsCap = 100;
const uint8_t fpsDigitNum = static_cast<uint8_t>(log10(fpsCap)+1);

const uint16_t baseYPosition = 2*winHeight/3;
const uint16_t gravity = 2491;

const int16_t backgroundSpeed = -59;
const uint8_t cloudNum = 11;

const int16_t movementSpeed = -327;
const int16_t jumpSpeed = -706;
const uint8_t groundNum = 2;

const uint8_t cactusNum = 3;

const uint8_t walkFrequency = 67;
const uint16_t playerXPosition = winWidth/10;

const uint8_t scoreDigitNum = 5;
const uint8_t digitDistance = 2;
const uint16_t scoreOffsetX = winWidth - scoreDigitNum*(numbers[0].w+digitDistance) - 14 + digitDistance;
const uint16_t scoreOffsetY = 69;
const uint8_t pointFrequency = walkFrequency*3/2;

#endif // CONSTANTS_H_INCLUDED
