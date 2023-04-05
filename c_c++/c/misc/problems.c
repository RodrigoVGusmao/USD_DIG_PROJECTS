#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

typedef int characterIntervalProc(int ch);
typedef void* dataTypeProc(int ch, void** data, int* extraData);

void* getStdout(int *ch, characterIntervalProc charList, dataTypeProc dataType)
{
	void* data = NULL;
	int extra = 0;

	int chTemp;
	while((chTemp = getchar()) != EOF && charList(chTemp) != 0)
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
	}

	*ch = chTemp;
	return data;
}

int numberIntInterval(int ch)
{
	if((ch >= '0' && ch <= '9') || ch == '-')
	{
		return ~0;
	}

	return 0;
}

void* intProcessing(int ch, void** data, int* extraData)
{
	if(*data == NULL)
	{
		*data = malloc(sizeof(int));
		*(int*)(*data) = 0;
	}

	int temp = *(int*)(*data);
	if(ch == '-')
	{
		temp = 0;
		*extraData |= 1;
	}
	if(ch >= '0' && ch <= '9')
	{
		temp *= 10;
		temp += ch - '0';

		if(*extraData & 1)
		{
			temp *= -1;
			*extraData &= ~1;
		}
	}

	*(int*)(*data) = temp;
	return *data;
}

int getInt(int *ch, int *error)
{
	int* value = (int*)(getStdout(ch, numberIntInterval, intProcessing));
	if(value == NULL)
	{
		*error = ~0;
		return 0;
	}
	else
	{
		*error = 0;
		return *value;
	}
}

int main()
{
	printf("input a number:");
	int ch, error;
	int num = getInt(&ch, &error);

	int reverse = 0;
	for(; num != 0; num /= 10)
	{
		reverse *= 10;
		reverse += num % 10;
	}

	printf("reverse number: %d", reverse);

	if(error != 0)
		printf("Could not read stdout");

	return 0;
}
