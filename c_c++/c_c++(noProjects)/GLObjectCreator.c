#include<stdio.h>
#include<math.h>

#define PI  3.14159265359

int main()
{
    int vertices;
    int degrees;
    int i;

    printf("How many vertices(discounting center's vertice):\n");
    scanf("%d", &vertices);
    if(vertices < 3)
    {
        printf("Need at least 3 vertices\n");
        return 1;
    }

    degrees = 360/vertices;

    printf("center's degrees:%d\n", degrees);

    printf("\nvertices:\n");
    printf("TabCode%f, %f, %f,\n", 0.0f, 0.0f, 0.0f);

    for(i=0;i<vertices;i++)
    {
        printf("TabCode%f, %f, %f,\n", cos(i*degrees*(PI/180)), 0.0f, sin(i*degrees*(PI/180)));
    }

    printf("\nindices:\n");
    for(i=0;i<vertices-1;i++)
    {
        printf("TabCode0, %d, %d,\n", i+1, i+2);
    }
    printf("TabCode0, %d, 1,\n", i+1);

    fflush(stdin);
    getchar();
    return 0;
}
