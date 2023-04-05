/**##############################################################
Algoritmo "media"
// Disciplina   : Linguagem e Técnicas de Programação
// Professor   : Ricardo Jose Menezes Maia
// Descrição   : soma 4 números
// Autor(a)    : Rodrigo Vieira Gusmão
// Data        : 12/04/2021
Var
// Seção de Declarações das variáveis
   iteracao, resultado, valor :Inteiro

Inicio
// Seção de Comandos, procedimento, funções, operadores, etc...
   resultado := 0
   iteracao := 0
   enquanto iteracao <> 4 FACA
         escreval("digite o ", iteracao+1, "° número:")
         leia(valor)
         resultado := resultado + valor
         iteracao := iteracao + 1
   fimenquanto

   escreval("a soma é ", resultado)
Fimalgoritmo
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

///inicio do código.
#define QUANTIDADE_DE_ENTRADAS 4

int main()
{
	int iteracao, valor;
	int resultado = 0;

	//a função utf8_ready() serve apenas para garantir que os caracteres sejam imprimidos corretamente
	if(!utf8_ready()) return 1;

	for(iteracao=0;iteracao != QUANTIDADE_DE_ENTRADAS;)
	{
		printf("digite o %d° número:", ++iteracao);
		scanf("%d", &valor);
		resultado += valor;
	}

	printf("a soma é %d", resultado);

	return 0;
}
