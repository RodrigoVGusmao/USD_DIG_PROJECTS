/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <math.h>

int main()
{
    int base, power;
    scanf("%d", &base);
    scanf("%d", &power);

    printf("%d\n", (int)pow(base, power));

    float x, y, z;
    scanf("%f %f %f", &x, &y, &z);
    printf("%f + %f + %f = %f\n%f + %f + %f = %f", x, y, z, x+y+z, y, x, z, y+x+z);
    if(x+y+z == y+x+z)
    	printf(" V\n");

	printf("%d", (int)4.9999999999999999999999f);

    return 0;
}
