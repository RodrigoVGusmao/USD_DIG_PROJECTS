/**##############################################################
// Disciplina   : Linguagem e Técnicas de Programação
// Professor   : Ricardo Jose Menezes Maia
// Descrição   : soma 4 números
// Autor(a)    : Rodrigo Vieira Gusmão
// Data        : 12/04/2021
#################################################################*/

#include<stdio.h>

//definição de utf8_ready(). Não tem necessidade de ler esse bloco de código
#if defined(__WIN32) || defined(__WIN32__) || defined(__WIN64) || defined(__WIN64__)
#include<windows.h>
int utf8_ready()
{
	return (int)~0 & SetConsoleOutputCP(CP_UTF8) & SetConsoleCP(CP_UTF8);
}
#else
int utf8_ready() return 1;
#endif

//inicio do código
int main()
{
	int A, B;
	int MAIOR, MENOR;

	//a função utf8_ready() serve apenas para garantir que os caracteres sejam imprimidos corretamente
	if(!utf8_ready()) return 1;

	printf("digite dois números inteiros: ");
	scanf("%d %d", &A, &B);

	MAIOR = max(A, B);
	MENOR = min(A, B);

	printf("%d é maior que %d", MAIOR, MENOR);

	return 0;
}
