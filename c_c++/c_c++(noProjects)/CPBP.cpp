#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

char* key()
{
    char *key = (char*)rand();
    /*int compara;
    int lenght = rand() % 15 + 1;
    char character;
    char* key;

    printf("tamanho da senha: %d\n", lenght);

    for(compara = 0; compara < lenght; compara++)
    {
        character = rand() % 79 + 40;
        printf("%c\n%d\n", character,compara);
        strcat(key, character);
    }*/
    return key;
}

main()
{
    srand(time(NULL));

    printf("\n%s", key());

    return 0;
}
