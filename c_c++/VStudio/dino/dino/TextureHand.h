#ifndef TEXTUREHAND_H
#define TEXTUREHAND_H

#include<string>
#include<vector>

#include<SDL.h>
#include<SDL_image.h>

namespace sdlTexture
{
typedef std::vector<bool> collisionMap;

class TextureHand
{
    public:
        SDL_Texture* texture;
        collisionMap bitCollisionMap;

        TextureHand(SDL_Renderer*, std::string, const uint8_t*, const uint8_t*, const uint8_t*);
        TextureHand(SDL_Renderer*, std::string, const uint8_t*);
        void setCollisionBits(uint8_t, uint8_t, uint8_t);
        void setCollisionBits(uint8_t);
        void setCollisionBits(std::string, uint8_t, uint8_t, uint8_t);
        void setCollisionBits(std::string, uint8_t);
        void freeSurface();
        ~TextureHand();

    protected:
        SDL_Surface* loadedSurface;
};

} //namespace side

#endif // TEXTUREHAND_H

