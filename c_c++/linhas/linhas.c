#include<stdio.h>
#include<stdint.h>
#include<math.h>

#include"pixel.h"
#include"customIO.h"

///funções da atividade
void imprimeRetaDDA(int x, int y, int xf, int yf)
{
    float dx = xf-x;        // Delta X
    float dy = yf-y;        // Delta Y
    float step;             // Step do DDA
    float xinc;             // Incremento de X
    float yinc;             // Incremento de Y

    // Calculo do STEP
    //step = max (abs(dx), abs(dy));
    step = (abs(dx) > abs(dy) ? abs(dx) : abs(dy) );

    // Calculo dos Incrementos
    xinc = dx/step;
    yinc = dy/step;

    // X e Y a serem usandos no PrintPixel
    int ax = x;
    int ay = y;

    // X e Y auxiliares a contagem
    float bx = x;
    float by = y;
    float maior = (abs(dx) > abs(dy) ? abs(dx) : abs(dy));

    printPixel(ax, ay);

    for(int i = 0 ; i < maior ; i++) {

        bx = bx+xinc;
        by = by+yinc;
        ax = round(bx);
        ay = round(by);

        printPixel(ax, ay);
    }
}

void imprimeRetaBresenham(int x, int y, int xf, int yf)
{
    int dx = xf-x;
    int dy = yf-y;

    int8_t quadrante = (dy < 0 && dx == 0 ? 0 :                             //caso dy seja negativo e dx seja igual a 0, considere dx como negativo
                        (dx & 0x8000) >> 14) | ((dy & 0x8000) >> 15);       //determina em qual quadrante a linha se encontra: dx >= 0 && dy >= 0 1 quadrante(0), dx < 0 && dy >= 0 2 quadrante(2)
                                                                            // dx <= 0 && dy < 0 3 quadrante(3) e dx >= 0 && dy < 0 4 quadrante(1)
    dx = quadrante & 2 ? -dx : dx;                                          // dx = |dx|;
    dy = quadrante & 1 ? -dy : dy;                                          // dy = |dy|;
    int8_t octante = dx > dy;
    int temp = dx > dy ? dy : dx;                                           // maior valor vai para dx e o menor para dy
    dx = dx > dy ? dx : dy;
    dy = temp;

    int32_t ax = 0;                                                         //x 32 bits formado pela concatenação de -x(16 bits) e x(16 bits)
    int32_t ay = 0;                                                         //y 32 bits formado pela concatenação de -y(16 bits) e y(16 bits)

    int ref = 2*dy - dx;

    printPixel(x, y);

    for(int i = 0; i < dx; ++i)
    {
        ax += 0xFFFF0001;                                                   //subtrai 1 da porção -x de ax e simultaneamente adiciona 1 a porção x de ax
        if(ref < 0)
        {
            ref += 2*dy;
        }
        else
        {
            ref+=2*(dy-dx);
            ay += 0xFFFF0001;                                               //subtrai 1 da porção -y de ay e simultaneamente adiciona 1 a porção y de ay
        }
        printPixel((int16_t)(((octante ? ax : ay) >> (quadrante & 2)*8) + x),   //decide em qual octante e quadrante deve imprimir a linha
                   (int16_t)(((octante ? ay : ax) >> (quadrante & 1)*16) + y)); //dependendo do quadrante usa a porção negativa ou positiva de ax e ay
    }                                                                           //e a depender do octante decide se troca ou não a ordem de ax e ay
}

void imprimeCirculoPitagoras(int xC, int yC, int r)
{
	int x,y,r2;
	r2=r*r;
	y=r;
	for(x=0;x<=y;x++)
	{
		y=round(sqrt(r2-x*x));

		//1° octante de cada quadrante
		printPixel(xC+y,yC+x);
		printPixel(xC+y,yC-x);
		printPixel(xC-y,yC+x);
		printPixel(xC-y,yC-x);

		//2° octante de cada quadrante
		printPixel(xC+x,yC+y);
		printPixel(xC+x,yC-y);
		printPixel(xC-x,yC+y);
		printPixel(xC-x,yC-y);
	}
}

void imprimeCirculoPolar(int x, int y, int r)
{
    float fx = x - 0.5f, fy = y - 0.5f, fr = r;
    float passo = asin(1.f/fr);
    float maxRep = M_PI_4/passo;

    for(int i = 0; i <= maxRep+1; i++ )
    {
        //1° octante de cada quadrante
        printPixel(fr*cos(i*passo) + fx, fr*sin(i*passo) + fy);
        printPixel(-fr*cos(i*passo) + fx, fr*sin(i*passo) + fy);
        printPixel(fr*cos(i*passo) + fx, -fr*sin(i*passo) + fy);
        printPixel(-fr*cos(i*passo) + fx, -fr*sin(i*passo) + fy);

        //2° octante de cada quadrante
        printPixel(fr*sin(i*passo) + fx, fr*cos(i*passo) + fy);
        printPixel(-fr*sin(i*passo) + fx, fr*cos(i*passo) + fy);
        printPixel(fr*sin(i*passo) + fx, -fr*cos(i*passo) + fy);
        printPixel(-fr*sin(i*passo) + fx, -fr*cos(i*passo) + fy);
    }
}

void imprimeCirculoBresenham(int x, int y, int r, int p)
{
    printPixel(0,r);
	printPixel(0,-r);
	printPixel(r,0);
	printPixel(-r,0);

	p = 1 - r;
	y = r;

	while(x < y){
		if(p < 0){
			p = p + 2*x + 1;

		}else if(p >= 0){
			y--;
			p = p + 2*x + 1 - 2*y;
		}
            //1° octante de cada quadrante
            printPixel(y,x);
			printPixel(-y,x);
			printPixel(-y,-x);
			printPixel(y,-x);

            //2° octante de cada quadrante
			printPixel(x,y);
			printPixel(-x,y);
			printPixel(-x,-y);
			printPixel(x,-y);

			x++;
	}
}

///callbacks
static void* func;

void argument4(uint64_t args)
{
    union expanded{
        int16_t args_v[4];
        uint64_t args_a;
    };
    ((void (*)(int, int, int, int))func)(((union expanded)args).args_v[3], ((union expanded)args).args_v[2], ((union expanded)args).args_v[1], ((union expanded)args).args_v[0]);
}

void argument3(uint64_t args)
{
    union expanded{
        int16_t args_v[4];
        uint64_t args_a;
    };
    ((void (*)(int, int, int))func)(((union expanded)args).args_v[2], ((union expanded)args).args_v[1], ((union expanded)args).args_v[0]);
}

void argument2(uint64_t args)
{
    union expanded{
        int16_t args_v[4];
        uint64_t args_a;
    };
    ((void (*)(int, int))func)(((union expanded)args).args_v[1], ((union expanded)args).args_v[0]);
}

void argument4o2(uint64_t args)
{
    union expanded{
        int32_t args_v[2];
        uint64_t args_a;
    };
    ((void (*)(int, int))func)(((union expanded)args).args_v[1], ((union expanded)args).args_v[0]);
}

///função main
int main()
{
    init();

    int option = 8;
    do
    {
        int ch, extract;
        switch(option)
        {
        case 1:
            func = imprimeRetaDDA;
            askInput("Digite o ponto inicial e o ponto final:", "x1 y1 x2 y2", 4, 4, 0, argument4, fitTextRight);
            break;
        case 2:
            func = imprimeRetaBresenham;
            askInput("Digite o ponto inicial e o ponto final:", "x1 y1 x2 y2", 4, 4, 0, argument4, fitTextRight);
            break;
        case 3:
            func = imprimeRetaDDA;
            askInput("Digite os pontos do poligono(min = 2):", "x y", 2, 4, 1, argument4, fitTextRight);
            break;
        case 4:
            func = imprimeRetaBresenham;
            askInput("Digite os pontos do poligono(min = 2):", "x y", 2, 4, 1, argument4, fitTextRight);
            break;
        case 5:
            func = imprimeCirculoPitagoras;
            askInput("Digite o ponto inicial e o ponto final:", "x y r", 3, 3, 0, argument3, fitTextRight);
            break;
        case 6:
            func = imprimeCirculoPolar;
            askInput("Digite o ponto inicial e o ponto final:", "x y r", 3, 3, 0, argument3, fitTextRight);
            break;
        case 7:
            func = imprimeCirculoBresenham;
            askInput("Digite o ponto inicial e o ponto final:", "x y r", 3, 3, 0, argument3, fitTextRight);
            break;
        case 8:
            func = setViewport;
            askInput("Digite o tamanho do plano cartesiano:", "nx ny px py", 4, 4, 0, argument4, fitTextRight);
            printCartesianPlane();
            break;
        case 9:
            func = setPixelAspect;
            askInput("Digite a resolução do pixel:", "rx ry", 2, 2, 0, argument2, fitTextRight);
            printCartesianPlane();
            break;
        case 10:
            func = setColorScheme;
            askHexInput("Digite o esquema de cores:", "hexFore hexBack", 2, 2, 0, argument4o2, fitTextRight);
            printCartesianPlane();
            break;
        }
        ch = 0;

        fitTextRight();
        printf("0 - Sair\n");
        fitTextRight();
        printf("1 - Imprime uma reta (DDA)\n");
        fitTextRight();
        printf("2 - Imprime uma reta (Bresenham)\n");
        fitTextRight();
        printf("3 - Imprime um polígono (DDA)\n");
        fitTextRight();
        printf("4 - Imprime um polígono (Bresenham)\n");
        fitTextRight();
        printf("5 - Imprime um círculo (pitágoras)\n");
        fitTextRight();
        printf("6 - Imprime um círculo (coordenadas polares)\n");
        fitTextRight();
        printf("7 - Imprime um círculo (Bresenham)\n");
        fitTextRight();
        printf("Entrada:\n");

        fitTextRight();
        option = getInt(&ch, &extract);
        if(extract == 0)
            option = -1;

        if(ch != '\n' && ch != EOF)
            flush(&ch);

        printCartesianPlane();

    } while(option != 0);
    getchar();
    return 0;
}
