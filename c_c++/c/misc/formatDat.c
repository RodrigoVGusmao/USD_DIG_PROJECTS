/**##############################################################
// Disciplina   : Linguagem e Técnicas de Programação
// Professor   : Ricardo Jose Menezes Maia
// Descrição   : formata data
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

char* mes[12] = {"de Janeiro de ", "de Fevereiro de ", "de Março de ", "de Abril de ", "de Maio de ", "de Junho de ",
				 "de Julho de ", "de Agosto de ", "de Setembro de ", "de Outubro de ", "de Novembro de ", "de Dezembro de "};

int main()
{
	printf("insira a data (dd/mm/aaaa): ");

	int ch, iteracao = 0;
	while((unsigned int)(ch = getchar()) - '0' > 9 && ch != '\n' && ch != EOF);
	while(ch != '\n' && ch != EOF)
	{
		int num = 0;
		do
		{
			num *= 10;
			num += ch - '0';
		}while((unsigned int)(ch = getchar()) - '0' <= 9 && ch != '\n' && ch != EOF);

		if(ch != '\n' && ch != EOF) while((unsigned int)(ch = getchar()) - '0' > 9 && ch != '\n' && ch != EOF);

		switch(iteracao++)
		{
		case 0:
			printf("%0dd ", max(min(num, 31), 1));
			break;
		case 1:
			printf("%s", mes[max(min(num-1, 11), 0)]);
			break;
		case 2:
			printf("%d\n", max(min(num, 12), 1));
			break;

		default:
			goto exit;
		}
	}
	exit:
	return 0;
}
