#include<stdio.h>
#define STDIO_H

#include"vector_c.h"

int main()
{
	int_v IntVector = initVector(0, void, NULL);
	int value = 25;
	insertInt(&IntVector, &value);
	insertInt(&IntVector, &value);

	((int*)(IntVector.buffer))[0] = 50;

	printf("%d\n", ((int*)(IntVector.buffer))[0]);
	printf("%d", ((int*)(IntVector.buffer))[1]);
}
