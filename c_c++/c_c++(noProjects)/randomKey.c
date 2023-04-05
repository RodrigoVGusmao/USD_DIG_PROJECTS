#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

main()
{
    srand(time(NULL));

    int size = rand() % 6 + 1, compara;

    char key[6];
    char input[6];
    char character[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

    printf("discover the password\nsize: %d\n", size);

    for(compara = 0; compara < size; compara++)
    {
        key[compara] = character[rand() % 35];
    }

    printf("character-per-character:");

    for(compara = 0; compara < size; compara++)
    {
        printf("%c", key[compara]);
    }

    printf("\nstring:%s\n", key);

    while(strcmp(input, key) != 0)
    {
        printf("password:");
        scanf("%s", input);
    }

    printf("congrulations!!!!!!!!!!");

    while(1)
    {
        getchar();
    }
}
