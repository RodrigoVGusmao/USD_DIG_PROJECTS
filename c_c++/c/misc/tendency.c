#include <stdio.h>
#include <stdlib.h>

#include <math.h>

typedef struct
{
    float x;
    float y;
}coord_pair;

typedef struct
{
    int size;
    int reserve;
    void* data;
}container;

container init_container()
{
    container defC = {0, 1, malloc(1)};
    return defC;
}

container* insert_coord_pair(container* cont, coord_pair value)
{
    container copyC = *cont;
    char resize = 0;
    while(copyC.reserve < copyC.size + sizeof(coord_pair))
    {
        copyC.reserve <<= 1;
        resize = 1;
    }
    if(resize)
    {
        coord_pair* temp;
        temp = realloc(copyC.data, copyC.reserve);
        if(temp == NULL)
            return NULL;
        copyC.data = temp;
    }
    ((coord_pair*)copyC.data)[copyC.size/sizeof(coord_pair)] = value;
    copyC.size += sizeof(coord_pair);
    *cont = copyC;
    return cont;
}

float getNextFloat(int *input)
{
    float value = 0.f;
    char exp = 0;
    int ch = *input;

    do{
        ch = getchar();
    }while((ch <= '0' || ch > '9') && ch != '-' && ch != '.' && ch != ',' && ch != '\n');

    if(ch == '\n')
        return 0.f;

    while((ch >= '0' && ch <= '9') || ch == '.' || ch == ',' || ch == '-')
    {
        if(ch == '-')
            value *= -1;
        else if(ch == '.' || ch == ',')
        {
            if(exp == 0)
                exp++;
            else
                return value;
        }
        else
        {
            if(exp != 0)
            {
                value += (float)(ch - '0')/pow(10.f, exp++);
            }
            else
            {
                value *= 10.f;
                value += (float)(ch - '0');
            }
        }
        ch = getchar();
    }
    *input = ch;
    return value;
}

int main()
{
    int input = 0;
    printf("insert data:\n");

    container pair_vector = init_container();
    if(pair_vector.data == NULL)
        return 1;
    while(input != EOF && input != '|')
    {
        coord_pair pair = {getNextFloat(&input), getNextFloat(&input)};

        if(insert_coord_pair(&pair_vector, pair) == NULL)
            return 2;
    }

    printf("x\ty\n");
    for(size_t i = pair_vector.size/sizeof(coord_pair); i != 0; --i)
    {
        printf("%d\t%f\t%f\n", i, ((coord_pair*)pair_vector.data)[i-1].x, ((coord_pair*)pair_vector.data)[i-1].y);
    }

    printf("x\ty\n");
    float Asum = 0.f, bSum = 0.f;
    int j = 1;
    //for(int j = pair_vector.size/sizeof(coord_pair); j != 0; --j)
		for(size_t i = pair_vector.size/sizeof(coord_pair); i != j; --i)
		{
			float a = (((coord_pair*)pair_vector.data)[i-1].y - ((coord_pair*)pair_vector.data)[i-1-j].y)/(((coord_pair*)pair_vector.data)[i-1].x - ((coord_pair*)pair_vector.data)[i-1-j].x);
			float b = ((coord_pair*)pair_vector.data)[i-1-j].y - ((coord_pair*)pair_vector.data)[i-1-j].x*a;
			Asum += a*j;
			bSum += b*j;
			printf("%d: y = %f*x + %f\n", i-1, a, b);
		}

    printf("\n%M: y = %f*x + %f\n", Asum/19, bSum/19);

    return 0;
}
