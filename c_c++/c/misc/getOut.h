#ifndef GET_OUT_H
#define GET_OUT_H

#define _USE_MATH_DEFINES
#include<math.h>

#include<stdio.h>
#include<stdlib.h>

//functions returns 0 if value 'ch' is not on interval, using extra custom data for processing;
typedef int characterIntervalProc(int ch, void** extraData);
//functions allocate and modify data, processes 'ch' character from getchar(), has extra user defined custom data for processing;
typedef void* dataTypeProc(int ch, void** data, void** extraData);

typedef void* postPtocessingProc(void** data, void** extraData);

void* getStdout(int *ch, characterIntervalProc charList, dataTypeProc dataType)
{
	void* data = NULL;
	void* extra = (void*)0;

	int chTemp = *ch;
	if(chTemp == 0)
		chTemp = getchar();
	if(charList(chTemp, &extra) == 0)
	{
		*ch = chTemp;
		return data;
	}
	do
	{
		void* temp = dataType(chTemp, &data, &extra);
		if(temp != NULL)
		{
			data = temp;
		}
		else
		{
			*ch = chTemp;
			return NULL;
		}
	}while((chTemp = getchar()) != EOF && charList(chTemp, &extra) != 0);

	*ch = chTemp;
	return data;
}

//custom functions, name convention typeNumberInterval and typeProcessing, 'type' cannot have a space.
//ex.: int, double, float, char, are valid, but unsigned float, signed int, struct custom, are invalid
//int functions
void* NullProcessing(int ch, void** data, void** extraData)
{
	return 1;
}

int intNumberInterval(int ch, void** extraData)
{
	if((ch >= '0' && ch <= '9') || ch == '-')
	{
		return ~0;
	}

	return 0;
}

int intInverseNumberInterval(int ch, void** extraData)
{
	if(ch == '\n')
		return 0;
	return ~intNumberInterval(ch, extraData);
}

void* intProcessing(int ch, void** data, void** extraData)
{
	if(*data == NULL)
	{
		*data = malloc(sizeof(int));
		if(*data == NULL)
			return NULL;
		*(int*)(*data) = 0;
	}

	int temp = *(int*)(*data);
	if(ch == '-')
	{
		temp = 0;
		*(int*)extraData |= 1;
	}
	else if(ch >= '0' && ch <= '9')
	{
		temp *= 10;
		int alg = ch - '0';

		if(*(int*)extraData & 1)
		{
			alg *= -1;
			*(int*)extraData &= ~1;
		}
		temp += alg;
	}

	*(int*)(*data) = temp;
	return *data;
}

//double functions
int doubleNumberInterval(int ch, void** extraData)
{
	//test for E
	if((*(int*)extraData) & 4)
	{
		printf("E\n");
		return 0;
	}

	if((ch >= '0' && ch <= '9') || ch == '.' || ch == ',' || ch == '-' || ch == 'E')
	{
		return ~0;
	}

	return 0;
}

int doubleInverseNumberInterval(int ch, void** extraData)
{
	if(ch == '\n')
		return 0;
	return ~doubleNumberInterval(ch, extraData);
}

int get_int (int* ch, int* error);

void* doubleProcessing(int ch, void** data, void** extraData)
{
	if(*data == NULL)
	{
		*data = malloc(sizeof(double));
		if(*data == NULL)
			return NULL;
		*(int*)(*data) = 0;
	}

	int expoent, lastChar, error;
	double temp = *(double*)(*data);
	if(ch == 'E')
	{
		*(int*)extraData |= 4;

		printf("%f\n", temp);
		lastChar = 0;
		expoent = get_int(&lastChar, &error);
		if(error != 0)
			return NULL;
		temp *= pow(10.0, (double)expoent);
		printf("%f\n", temp);
	}
	else if(ch == '.' || ch == ',')
	{
		*(int*)extraData |= 0x102;

	}
	else if(ch == '-')
	{
		temp = 0;
		*(int*)extraData |= 1;
	}
	if(ch >= '0' && ch <= '9')
	{
		double alg;
		if((*(int*)extraData) & 2)
		{
			int exp = (*(int*)extraData) >> 8;
			alg = (double)(ch - '0')/pow(10.0, (double)exp);
			(*(int*)extraData) = ((*(int*)extraData) & 0xF) + ((++exp) << 8);
		}
		else
		{
			temp *= 10.0;
			alg = (double)(ch - '0');
		}

		if(*(int*)extraData & 1)
		{
			alg *= -1.0;
		}
		temp += alg;
	}

	*(double*)(*data) = temp;
	return *data;
}

//custom wrappers
//wrapper template, need to declare in global space
#define getInput(type)																		\
type get_##type (int* ch, int* error) {														\
	type* value = (type*)(getStdout((ch), type##NumberInterval, type##Processing));			\
	if(value == NULL)																		\
	{																						\
		*error = ~0;																		\
		type temp = {0};																	\
		return temp;																		\
	}																						\
	else																					\
	{																						\
		*error = 0;																			\
		type returnVal = *value;															\
		free(value);																		\
		return returnVal;																	\
	}																						\
}																							\
void clear_for_##type (int* ch) {															\
	getStdout((ch), type##InverseNumberInterval, NullProcessing);							\
}

getInput(int)
getInput(double)
#endif
