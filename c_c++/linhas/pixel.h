#ifndef PIXEL_H
#define PIXEL_H
//#define WINDOW

#ifdef _WIN32
#include<windows.h>
#endif

#include<stdint.h>

///Funções responsáveis por controlar o console
/// init: inicializa o terminal virtual no ambiente windows e o suporte a utf8, função vazia em outros sistemas operacionais
/// printPixel: função para imprimir pixel
/// printCartesianPlane: função responsável por abrir espaço e imprimir o plano cartesiano. Qualquer texto que esteja na area de desenho do plano será empurrado para a esquerda

void init(void);
void setViewport(int minX, int minY, int maxX, int maxY);
int printPixel(int x, int y);
void printCartesianPlane(void);
void resetLine(void);
void fitTextRight(void);
void setPixelAspect(SHORT x, SHORT y);
void setColorScheme(COLORREF foreground, COLORREF background);

#endif
