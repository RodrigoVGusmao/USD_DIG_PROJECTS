#include "TextureHand.h"

using namespace sdlTexture;
TextureHand::TextureHand(SDL_Renderer* winRenderer, std::string path, const Uint8* r, const Uint8* g, const Uint8* b)
{
    loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)
        throw 5;

    if(r != NULL && g != NULL && b != NULL)
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, *r, *g, *b));
    }

    texture = SDL_CreateTextureFromSurface(winRenderer, loadedSurface);
    if(texture == NULL)
        throw 6;
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
}

TextureHand::~TextureHand()
{
    SDL_DestroyTexture(texture);
}
