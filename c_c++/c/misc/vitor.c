#include <stdio.h>
#include<stdlib.h>
#include <conio.h>

int main(){
    FILE *arquivo;
    char nome[100], letras[100];
    int c;
    arquivo = '\0';
    printf("Entre com o nome do arquivo, o ponto e a extensão: ");
    int ch;
    for(size_t i = 0; ((ch = getchar()) != '\n') && ch != EOF; i++)
    {
        nome[i] = ch;
        nome[i+1] = '\0';
    }
    arquivo = fopen(nome, "a");
    printf("Digite um caracter ou uma string: ");
    gets(letras);
    //scanf("%s[^\n]", letras);
    for(c = 0; letras[c]; c++){
        putc(letras[c], arquivo);
    }
    fclose(arquivo);
    return 1;
}
