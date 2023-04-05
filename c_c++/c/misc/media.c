/**##############################################################
Algoritmo "media"
// Disciplina   : Linguagem e Técnicas de Programação
// Professor   : Ricardo Jose Menezes Maia
// Descrição   : soma 4 números
// Autor(a)    : Rodrigo Vieira Gusmão
// Data        : 12/04/2021
Var
// Seção de Declarações das variáveis
   valor, media :real
   iteracao :inteiro

Inicio
// Seção de Comandos, procedimento, funções, operadores, etc...
   media := 0
   iteracao := 0
   enquanto iteracao <> 3 faca
            escreval("digite a ", iteracao+1, "ª nota:")
            leia(valor)
            media := media + valor
            iteracao := iteracao + 1
   fimenquanto

   escreval("a média é ", media/iteracao)
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
#define QUANTIDADE_DE_ENTRADAS 3

int main()
{
	int iteracao;
	float valor, resultado = 0;

	//a função utf8_ready() serve apenas para garantir que os caracteres sejam imprimidos corretamente
	if(!utf8_ready()) return 1;

	for(iteracao=0;iteracao != QUANTIDADE_DE_ENTRADAS;)
	{
		printf("digite a %dª nota:", ++iteracao);
		scanf("%f", &valor);
		resultado += valor;
	}

	printf("a média é %.2f", resultado/iteracao);

	return 0;
}
