#include"exprCalc.h"

#include<stdio.h>
#include<math.h>

int main()
{
	int lastChar = 0;
	int error;

	int a, b, N;
	printf("insert a, b and N: (a b N)");
	a = get_int(&lastChar, &error);
	if(lastChar == '\n' || lastChar == EOF)
		printf("insert b and N (b N): ");
	else
		clear_for_int(&lastChar);
	b = get_int(&lastChar, &error);
	if(lastChar == '\n' || lastChar == EOF)
		printf("insert N (N): ");
	else
		clear_for_int(&lastChar);
	N = get_int(&lastChar, &error);

	double m = ((double)a-(double)b)/(double)N;

	lastChar = 0;
	cStore data;
	data = get_calc(&lastChar, &error);

	cVar indexList = getVariablesIndex(data);

	/*for(size_t j = 0; j < indexList.size/sizeof(size_t); ++j)
	{
		changeVariableValue(&data, indexList, j, b);
	}*/
	double first, next;
	//next = solveExpression(data);
	next = 0.0;

	double area;
	double areaSum = 0.0;
	for(size_t i = 0; i <= N; ++i)
	{
		for(size_t j = 0; j < indexList.size/sizeof(size_t); ++j)
		{
			changeVariableValue(&data, indexList, j, i*m + b);
		}
		first = next;
		next = solveExpression(data, &realMode);
		area = (first + next)*m/2;
		if(i > 0)
			areaSum += area;
		printf("index: %f, result: %f, area: %f, area total: %f\n", i*m + b, next, area, areaSum);
	}

	printf("area total: %f, inverse: %.20f\n", areaSum, 1/areaSum);
	destroyCStore(&data);
	destroyCVar(&indexList);

	return 0;
}
