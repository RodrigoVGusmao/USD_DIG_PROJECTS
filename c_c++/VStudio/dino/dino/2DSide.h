#ifndef TWODSIDE_H
#define TWODSIDE_H

#include<algorithm>
#include<cstdint>
#include<iostream>
#include<math.h>
#include<vector>

#include<SDL.h>

#include"constants.h"

namespace side
{
typedef SDL_Point velocity;
typedef std::vector<bool> collisionMap;

struct element
{
    SDL_Rect clip;
    SDL_Point position;
    SDL_Point oldPosition;
    collisionMap bitMap;
};

class staticBackgroundObject
{
    public:
        element image;

        staticBackgroundObject();
        void speedCalculation(uint16_t, velocity);
        void setPosition(int32_t, int32_t);
        void setPosition(SDL_Point);

    protected:
        int16_t posx;
        int16_t posy;
};

class staticObject : public staticBackgroundObject
{
    public:
        bool collision;
        SDL_Point collPoint;
        staticObject();
        void getCollisionState(side::element);
        void getCollisionMap(side::collisionMap, uint32_t);
        bool detectPerPixelCollision(side::element);
        ~staticObject();

    protected:
        SDL_Rect intersectSec;
};

class animation
{
    public:
        animation(uint8_t, uint16_t, const SDL_Rect*);
        void setAnimation(uint8_t, uint16_t, uint16_t, uint16_t);
        ~animation();

    protected:
        SDL_Rect* clipImage;
        uint8_t movementsNum;
        uint16_t clipNum;
        uint16_t (*animationClips)[3];
        uint16_t currentClip;
        uint32_t frames;
};

class backgroundObject : public animation, public staticBackgroundObject
{
    public:
        backgroundObject(uint8_t, uint16_t, const SDL_Rect*);
        bool playAnimation(uint8_t, uint16_t);
};

class object : public staticObject , public animation
{
    public:
        object(uint8_t, uint16_t, const SDL_Rect*);
        bool playAnimation(uint8_t, uint16_t);
        ~object();
};

typedef staticObject staticCharacter;
typedef object character;

} //namespace side

#endif // TWODSIDE_H
