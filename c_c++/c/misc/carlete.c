#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#define times 2
    struct pessoa
    {
        char nome[50];
        int idade;
        float altura;
        char telefone[10];
    };
int main()
{
    struct pessoa amg[times];
    FILE *f;
    int i;

    for(i=0;i<times;i++)
    {
        amg[i].idade=0;
        amg[i].altura=0;
        strcpy(amg[i].telefone,"\0");
        strcpy(amg[i].nome,"\0");
    }

    f = fopen("amg2.txt", "w");
    if(f==NULL)
    {
        printf("Erro na abertura\n");
        system("pause");
        exit(1);
    }
    printf("\nBem-Vindo! Vamos criar sua lista de amigos? %.2f\n", 1.53f);

    for(i=0;i<times;i++)
    {
        printf("\nNome %d: ", i + 1);
        fflush(stdin);
        //getchar();
        scanf("%s", &amg[i].nome);
        printf("\nIdade %d: ", i + 1);
        scanf("%d", &amg[i].idade);
        printf("\nAltura %d: ", i + 1);
        scanf("%f", &amg[i].altura);
        printf("\nTelefone %d: ", i + 1);
        fflush(stdin);
        //getchar();
        scanf("%s", &amg[i].telefone);
        fprintf(f, "%s tem %d anos e %f de altura\ntel: %s\n\n", &amg[i].nome, amg[i].idade, amg[i].altura, &amg[i].telefone);
    }
    //fwrite(amg,sizeof(struct pessoa),times,f);

    printf("\nFim da lista, Obrigado!");
fclose(f);
getchar();
return 0;
}
