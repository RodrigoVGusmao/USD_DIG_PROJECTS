#ifndef PIXEL_G_H
#define PIXEL_G_H

#include<windows.h>
#include<stdint.h>

#define USD_PIXEL "\u2588\u2588"

///Funções responsáveis por controlar o console
/// init: inicializa o terminal virtual no ambiente windows e o suporte a utf8, função vazia em outros sistemas operacionais
/// printPixel: função para imprimir pixel
/// printCartesianPlane: função responsável por abrir espaço e imprimir o plano cartesiano. Qualquer texto que esteja na area de desenho do plano será empurrado para a esquerda

uint8_t lines;

void init(void);
void setViewport(int minX, int minY, int maxX, int maxY);
void setConsole(int x, int y);
int printPixel(int x, int y);
void printCartesianPlane(void);
void resetLine(void);
void fitTextRight(void);

#endif
