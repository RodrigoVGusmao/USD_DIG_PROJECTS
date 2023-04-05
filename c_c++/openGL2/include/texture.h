#ifndef TEXTURE_H
#define TEXTURE_H

#include<glut.h>

#include<stdint.h>
#include<windows.h>

//� responsabilidade de quem chamou liberar a vari�vel bitmap
int getBitmapFromBMP(void** bitmap, const char* filename, RECT* dimensions);

int LoadTexture(GLuint* textures, const void* bitmap, RECT imageDimensions, RECT clipDimension, size_t count, size_t offsetX, size_t offsetY, size_t cPerLine);

#endif // TEXTURE_H
