/**##############################################################
// Disciplina   : Linguagem e Técnicas de Programação
// Professor   : Ricardo Jose Menezes Maia
// Descrição   : compara duas datas
// Autor(a)    : Rodrigo Vieira Gusmão
// Data        : 03/05/2021
#################################################################*/

#if defined(__WIN32) || defined(__WIN64)
#include<windows.h>
int utf8_ready()
{
	return ~0 & SetConsoleCP(CP_UTF8) & SetConsoleOutputCP(CP_UTF8);
}
#else
int utf8_ready()
{
	return 1;
}
#endif // defined

#include<stdio.h>
#include<inttypes.h>

#define COMPARE_AMOUNT 2

#define _S_SECONDS(a, b) ((a) & ~0x3F | ((b) & 0x3F))
#define _S_MINUTES(a, b) ((a) & ~0xFC0 | (((b) & 0x3F) << 6))
#define _S_HOURS(a, b) ((a) & ~0x1F000 | (((b) & 0x1F) << 0xC))
#define _S_DAYS(a, b) ((a) & ~0x3E0000 | (((b) & 0x1F) << 0x11))
#define _S_MONTHS(a, b) ((a) & ~0x3C00000 | (((b) & 0xF) << 0x16))
#define _S_YEARS(a, b) ((a) & ~0xFFFFFFFFFC000000 | (((b) & 0x3FFFFFFFFF) << 0x1A))

#define _G_SECONDS(a) ((a) & 0x3F)
#define _G_MINUTES(a) (((a) & 0xFC0) >> 6)
#define _G_HOURS(a) (((a) & 0x1F000) >> 0xC)
#define _G_DAYS(a) (((a) & 0x3E0000) >> 0x11)
#define _G_MONTHS(a) (((a) & 0x3C00000) >> 0x16)
#define _G_YEARS(a) (((a) & 0xFFFFFFFFFC000000) >> 0x1A)
#define _G_COMMON_ERA(a) ((a) & 0x8000000000000000)

typedef int64_t operation(int64_t, uint8_t);

int64_t soma(int64_t total, uint8_t digito)
{
	return total+digito;
}

int64_t subt(int64_t total, uint8_t digito)
{
	return total-digito;
}

int main()
{
	int64_t maiorData = ~((uint64_t)(~0) >> 1), entradaData;

	for(size_t i=0; i < COMPARE_AMOUNT;)
	{
		printf("digite data %d (hh:mm:ss dd/mm/aaaa)", ++i);

		int64_t num;
		operation* ope;
		uint8_t unidade = 0;
		int ch;
		while((unsigned char)((ch = getchar())-'0') > 9 && ch != EOF && ch != '\n');
		while(ch != EOF && ch != '\n')
		{
			ope = soma;
			num = 0;
			do{
				if(ch == '-') ope = subt;
				else
				{
					num *= 10;
					num = ope(num, ch-'0');
				}
			}while((unsigned char)((ch = getchar())-'0') <= 9 || ch == '-' && ch != EOF && ch != '\n');

			switch(unidade)
			{
			case 0:
				entradaData = _S_HOURS(entradaData, max(min(num, 23), 0));
				break;

			case 1:
				entradaData = _S_MINUTES(entradaData, max(min(num, 59), 0));
				break;

			case 2:
				entradaData = _S_SECONDS(entradaData, max(min(num, 59), 0));
				break;

			case 3:
				entradaData = _S_DAYS(entradaData, max(min(num, 31), 1));
				break;

			case 4:
				entradaData = _S_MONTHS(entradaData, max(min(num, 12), 1));
				break;

			case 5:
				entradaData = _S_YEARS(entradaData, num);
				break;
			}

			if(ch != '\n' && ch != EOF) while((unsigned char)((ch = getchar())-'0') > 9 && ch != '-' && ch != EOF && ch != '\n');
			if(unidade++ >= 5 && ch != EOF && ch != '\n') while((ch = getchar()) != EOF && ch != '\n');
		}
		maiorData = maiorData > entradaData ? maiorData : entradaData;
	}
	printf("%d:%d:%d %d/%d/%d%s", _G_HOURS(maiorData), _G_MINUTES(maiorData), _G_SECONDS(maiorData),
				 _G_DAYS(maiorData), _G_MONTHS(maiorData), abs(_G_YEARS(maiorData)), _G_COMMON_ERA(maiorData) ? " a.c.\n" : "\n");

	return 0;
}
