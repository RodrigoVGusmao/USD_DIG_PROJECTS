#ifndef TEXTUREHAND_H
#define TEXTUREHAND_H

#include<string>

#include<SDL.h>
#include<SDL_image.h>

namespace sdlTexture
{

class TextureHand
{
    public:
        SDL_Texture* texture;

        TextureHand(SDL_Renderer*, std::string, const Uint8*, const Uint8*, const Uint8*);
        ~TextureHand();

    protected:
        SDL_Surface* loadedSurface;
};

} //namespace side

#endif // TEXTUREHAND_H

