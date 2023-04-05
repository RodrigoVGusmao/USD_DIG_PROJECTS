#include "2DSide.h"

using namespace side;

staticBackgroundObject::staticBackgroundObject()
{
    //ctor
}

void staticBackgroundObject::speedCalculation(uint16_t fps, velocity v)
{
    image.oldPosition = image.position;

    posx += (v.x/fps);
    posy += (v.y/fps);

    image.position.x = (int32_t)posx;
    image.position.y = (int32_t)posy;
}

void staticBackgroundObject::setPosition(int32_t xPosition, int32_t yPosition)
{
    posx = xPosition;
    posy = yPosition;
    image.position.x = xPosition;
    image.position. y = yPosition;

    image.oldPosition = image.position;
}

void staticBackgroundObject::setPosition(SDL_Point position)
{
    posx = position.x;
    posy = position.y;
    image.position.x = position.x;
    image.position. y = position.y;

    image.oldPosition = image.position;
}

staticObject::staticObject()
{
    //ctor
}

void staticObject::getCollisionState(side::element object)
{
    //AABB collision
    SDL_Rect objectRect = {object.position.x, object.position.y, object.clip.w, object.clip.h};
    SDL_Rect thisRect = {image.position.x, image.position.y, image.clip.w, image.clip.h};

    collision = SDL_IntersectRect(&objectRect, &thisRect, &intersectSec);

    if(collision)
    {
        float objSin = (static_cast<int64_t>(object.position.y)-object.oldPosition.y)/hypot(object.position.x-object.oldPosition.x, object.position.y-object.oldPosition.y);
        float objCos = (static_cast<int64_t>(object.position.x)-object.oldPosition.x)/hypot(object.position.x-object.oldPosition.x, object.position.y-object.oldPosition.y);

        float intersectSin = intersectSec.h/hypot(intersectSec.w, intersectSec.x);

        if(intersectSin >= objSin)
        {
            collPoint = {object.position.x-intersectSec.w, object.position.y-static_cast<int>(intersectSec.w*objSin/objCos)};
        }
        else
        {
            collPoint = {object.position.x-static_cast<int>(intersectSec.h*objCos/objSin), object.position.y-intersectSec.h};
        }
    }
}

void staticObject::getCollisionMap(side::collisionMap sprite, uint32_t spriteWidth)
{
    image.bitMap.resize(static_cast<uint64_t>(image.clip.w) * image.clip.h);

    for(int32_t i = 0; i < image.clip.h; i++)
        for(int32_t j = 0; j < image.clip.w; j++)
            image.bitMap[j + image.clip.w*i] = sprite[image.clip.x + j + image.clip.y * spriteWidth + (spriteWidth - image.clip.w)*i];
}

bool staticObject::detectPerPixelCollision(side::element object)
{
    getCollisionState(object);
    collisionMap testArea(static_cast<uint64_t>(intersectSec.w) * intersectSec.h, false);

}

staticObject::~staticObject()
{
    //dtor
}

animation::animation(uint8_t actionsNumber, uint16_t clipsNumber, const SDL_Rect* clips)
{
    movementsNum = actionsNumber;
    clipNum = clipsNumber;
    currentClip = 0;
    frames = 0;

    clipImage = new SDL_Rect[clipsNumber];
    animationClips = new uint16_t[actionsNumber][3];

    for(uint8_t i=0;i<clipNum;i++)
        clipImage[i] = clips[i];
}

void animation::setAnimation(uint8_t animationPos, uint16_t clipOffset, uint16_t numOfClips, uint16_t time)
{
    if(animationPos > movementsNum)
        throw 1;
    if(clipOffset > clipNum)
        throw 2;
    if(clipOffset + numOfClips - 1 > clipNum)
        throw 3;

    animationClips[animationPos][0] = clipOffset;
    animationClips[animationPos][1] = clipOffset + numOfClips - 1;
    animationClips[animationPos][2] = time;
}

animation::~animation()
{
    delete [] clipImage;
    delete [] animationClips;
}

backgroundObject::backgroundObject(uint8_t actionsNumber, uint16_t clipsNumber, const SDL_Rect* clips) : animation(actionsNumber, clipsNumber, clips)
{
    //ctor
}

bool backgroundObject::playAnimation(uint8_t animationPos, uint16_t fps)
{
    if(1000*frames/fps >= animationClips[animationPos][2])
    {
        frames = 0;
        currentClip++;
    }
    frames++;
    if(currentClip > animationClips[animationPos][1]-animationClips[animationPos][0])
    {
        frames = 0;
        currentClip = 0;
        return true;
    }
    image.clip = clipImage[animationClips[animationPos][0]+currentClip];
    return false;
}

object::object(uint8_t actionsNumber, uint16_t clipsNumber, const SDL_Rect* clips):animation(actionsNumber, clipsNumber, clips)
{
    //ctor
}

bool object::playAnimation(uint8_t animationPos, uint16_t fps)
{
    if(1000*frames/fps >= animationClips[animationPos][2])
    {
        frames = 0;
        currentClip++;
    }
    frames++;
    if(currentClip > animationClips[animationPos][1]-animationClips[animationPos][0])
    {
        frames = 0;
        currentClip = 0;
        return true;
    }
    image.clip = clipImage[animationClips[animationPos][0]+currentClip];
    return false;
}

object::~object()
{
    //dtor
}
