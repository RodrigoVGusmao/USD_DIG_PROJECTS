#ifndef PIXEL_H
#define PIXEL_H
//#define WINDOW

#ifdef _WIN32
#include<windows.h>
#endif

#include<stdint.h>

///Fun��es respons�veis por controlar o console
/// init: inicializa o terminal virtual no ambiente windows e o suporte a utf8, fun��o vazia em outros sistemas operacionais
/// printPixel: fun��o para imprimir pixel
/// printCartesianPlane: fun��o respons�vel por abrir espa�o e imprimir o plano cartesiano. Qualquer texto que esteja na area de desenho do plano ser� empurrado para a esquerda

void init(void);
void setViewport(int minX, int minY, int maxX, int maxY);
int printPixel(int x, int y);
void printCartesianPlane(void);
void resetLine(void);
void fitTextRight(void);
void setPixelAspect(SHORT x, SHORT y);
void setColorScheme(COLORREF foreground, COLORREF background);

#endif
