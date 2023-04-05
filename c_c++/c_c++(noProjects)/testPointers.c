#include<stdio.h>

int soma(px, py)
int *px, *py;
{
    *px = 3;
    *py = 5;
}

main()
{
    int a, b;
    a = 5;
    b = 6;

    soma(&a, &b);
    printf("%d + %d", a, b);
}
