#include "TextureHand.h"

using namespace sdlTexture;
TextureHand::TextureHand(SDL_Renderer* winRenderer, std::string path, const uint8_t* r = NULL, const uint8_t* g = NULL, const uint8_t* b = NULL)
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
}

TextureHand::TextureHand(SDL_Renderer* winRenderer, std::string path, const uint8_t* rgb = NULL) : TextureHand::TextureHand(winRenderer, path, rgb, rgb, rgb) {}

void TextureHand::setCollisionBits(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t pR, pG, pB;

    bitCollisionMap.resize(static_cast<uint64_t>(loadedSurface->w) * loadedSurface->h);
    for (uint32_t i = 0; i < loadedSurface->w * loadedSurface->h; i++)
    {
        uint32_t pixel = *(static_cast<uint32_t*>(loadedSurface->pixels)+i);
        SDL_GetRGB(pixel, loadedSurface->format, &pR, &pG, &pB);
        if (pR == r && pG == g && pB == b)
            bitCollisionMap[i] = false;
        else
            bitCollisionMap[i] = true;
    }
}

void TextureHand::setCollisionBits(uint8_t rgb)
{
    setCollisionBits(rgb, rgb, rgb);
}

void TextureHand::setCollisionBits(std::string path, uint8_t r, uint8_t g, uint8_t b)
{
    loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)
        throw 5;

    setCollisionBits(r, g, b);
}

void TextureHand::setCollisionBits(std::string path, uint8_t rgb)
{
    setCollisionBits(path, rgb, rgb, rgb);
}

void TextureHand::freeSurface()
{
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
}

TextureHand::~TextureHand()
{
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;
    SDL_DestroyTexture(texture);
    texture = NULL;
}
