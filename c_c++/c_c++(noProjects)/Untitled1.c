#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main()
{
    unsigned short pronome, verbo;

    srand(time(NULL));

    pronome = rand() % 8;
    verbo = rand() % 5;

    printf("%d  ,   %d\n", pronome, verbo);

    switch(pronome)
    {
        case 0:
            printf("eu");
            break;
        case 1:
            printf("voce");
            break;
        case 2:
            printf("ele");
            break;
        case 3:
            printf("ela");
            break;
        case 4:
            printf("nos");
            break;
        case 5:
            printf("voces");
            break;
        case 6:
            printf("eles");
            break;
        case 7:
            printf("elas");
            break;
    }

    return 0;
}
