/**##############################################################
Disciplina  : Linguagem e Técnicas de Programação
Questão		: Simulado 5
Professor   : Ricardo Jose Menezes Maia
Descrição   : soma 4 números
Autor(a)    : Rodrigo Vieira Gusmão
Data        : 12/04/2021
#################################################################*/

#if defined(__WIN32) || defined(__WIN32__) || defined(__WIN64) || defined(__WIN64__)
#include<windows.h>
int utf8_ready()
{
	return (int)~0 & SetConsoleOutputCP(CP_UTF8) & SetConsoleCP(CP_UTF8);
}
#else
int utf8_ready() return 1;
#endif

#include<stdio.h>
#include<inttypes.h>

#define MAX_VALUES 3											//set to '~(((size_t)~0)>>1)' for maximum value

typedef float operation(float, uint8_t);

float soma(float numero, uint8_t digito)
{
	return numero+digito;
}

float subt(float numero, uint8_t digito)
{
	return numero-digito;
}

struct vectorObject
{
	size_t allocContainer;
	size_t sizeContainer;
	void* object;
};

typedef struct vectorObject vectorFloat;

void* allocFloat(struct vectorObject* vector, float value)
{
	++vector->sizeContainer;
	if(vector->sizeContainer >= vector->allocContainer)
	{
		vector->allocContainer << 1;
		if((vector->object = realloc(vector->object, sizeof(float)*vector->allocContainer)) == NULL) goto bad_alloc;
	}

	float* address = vector->object;
	address[vector->sizeContainer-1] = value;

	bad_alloc:
	return vector->object;
}

int main()
{
	int ch;
	unsigned int inteiro;
	vectorFloat nums = {1, 0, malloc(sizeof(float))};

	do
	{
		printf("entre com a opção da ordem dos valores (1-crescente, 2-decrescente, 3-pódio): ");
		inteiro = getchar()-'0';
		while((ch = getchar()) != '\n' && ch != EOF);
	}while (inteiro-- > 3);

	printf("entre com os números (A B C ...): ");
	while((unsigned int)((ch = getchar())-'0') > 9 && ch != '.' && ch != ',' && ch != '-' && ch != EOF && ch !='\n');
	while(ch != '\n' && ch != EOF)
	{
		operation* ope = soma;
		float temp = 0;
		uint8_t usarExpoente = 0;
		uint16_t expoenteNegativo = 0;
		do
		{
			if(ch == '-')
				ope = subt;
			else if(ch == '.' || ch == ',')
				usarExpoente = 1;
			else
			{
				temp *= 10;
				temp = ope(temp, (float)(ch-'0'));
				if(usarExpoente != 0)
					++expoenteNegativo;
			}
		}while((unsigned char)((ch = getchar())-'0') <= 9 || ch == '-' || ch == '.' || ch == ',' && ch != EOF && ch !='\n');

		for(;expoenteNegativo > 0; --expoenteNegativo) temp /= 10;
		if(allocFloat(&nums, temp) == NULL) goto bad_alloc;

		//limpa o buffer até o fim do buffer ou proximo caracter válido
		if(ch != EOF && ch != '\n') while((unsigned char)((ch = getchar())-'0') > 9 && ch != '.' && ch != ',' && ch != '-' && ch != EOF && ch !='\n');
		if(nums.sizeContainer >= MAX_VALUES) break;
	}

	//reorganiza array em ordem crescente
	for(size_t i = 0; i < nums.sizeContainer; ++i)
	{
		for(size_t j = i+1; j < nums.sizeContainer; ++j)
		{
			float *addr, temp;
			addr = nums.object;

			temp = addr[i];
			if(temp > addr[j])
			{
				addr[i] = addr[j];
				addr[j] = temp;
			}
		}
	}

	float *addr = nums.object;
	size_t i=nums.sizeContainer*inteiro/(3*inteiro-2);				//determinando o index para que tenha de acordo com a variavel 'inteiro' respectivamente inicio(0), fim(1) e meio(2) do buffer
	do
	{
		printf("%f ", addr[i%nums.sizeContainer]);
	}while(++i % nums.sizeContainer != nums.sizeContainer*inteiro/(3*inteiro-2));

	return 0;

	bad_alloc:
	return 1;
}
