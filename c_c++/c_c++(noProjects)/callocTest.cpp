/**testa malloc realloc calloc e free**/

#include<stdio.h>
#include<stdlib.h>

void imprimString(char* str)
{
    printf("%s", str);
}

int main()
{
    unsigned short caracteresMax;
    char *string;
    int compara;

    printf("numeros de caracteres:");
    scanf("%d", &caracteresMax);

    string = (char*)calloc(caracteresMax+1, sizeof(char));

    for(compara = 0; compara < caracteresMax; compara++)
    {
        printf("numero %d:", compara);
        setbuf(stdin, NULL);
        scanf("%c", &string[compara]);
    }
    imprimString(string);
    return 0;
}
