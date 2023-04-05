#include<errno.h>
#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifdef _WIN32
#include<windows.h>
BOOL setConsoleUtfEncoding()
{
	return SetConsoleCP(CP_UTF8) && SetConsoleOutputCP(CP_UTF8);
}
#else
#define TRUE 1
#define FALSE 0
typedef unsigned int BOOL;
BOOL setConsoleUtfEncoding() return TRUE;
#endif // _WIN32

/**General purpose functions**/

char* sprintfAlloc(char** dest, const char* restrict format, ...)
{
	va_list args;
	va_start(args, format);
	int size = vsnprintf(NULL, 0, format, args);
	if(size < 0) goto vsnprintf_fail;
	va_end(args);

	char* rStr = malloc((size+1)*sizeof(*rStr));
	if(rStr == NULL) goto bad_alloc;

	va_start(args, format);
	size = vsprintf(rStr, format, args);
	if(size < 0) goto vsnprintf_fail;
	va_end(args);

	if(dest != NULL)
		*dest = rStr;

	return rStr;
	//exception list
	vsnprintf_fail:
	bad_alloc:
		free(rStr);
		va_end(args);
		return NULL;
}

///The character \t indicates the end of a column, \n indicates the end of a line and \0 indicates the end of the table, all of them are mandatory
///You can't use any format other than %[flags]s for strings, an optional %c after each %[flags]s for \t, and another optional ending %c for LF character

///If the number of columns in the first line is less than the number of columns in any other line the code will fail
///It is a good idea to put all the labels in the first entry of the list to ensure that the first line has the same quantity of \t's
///as the line with more \t's
void printList(const char* format, const char* list)
{
	const char* listStart = list;

	/*unsigned char columnCount;
	for(size_t i = 0; list[i] != '\n'; ++i)
	{
		if(list[i] == '\0') goto invalid_list;
		if(list[i] == '\t') ++columnCount;
	}*/

	size_t formatAlloc = 1;
	char* partFormat = malloc(sizeof(*partFormat));

	size_t listAlloc = 1;
	char* partList = malloc(sizeof(*partList));

	//print table
	while(listStart[0] != '\0')
	{
		const char* formatStart = format;

		//print line
		//for(IIterator j = 0;/* j < columnCount*/; ++j)
		while(strchr(formatStart, '%') != NULL && strchr(list, '\0') >= listStart)
		{
			//prepare format string
			if(strchr(format, '\0') < formatStart) break;

			unsigned short formatEnd;
			for(formatEnd = 1; formatStart[formatEnd] != '\0' && formatStart[formatEnd] != '%'; ++formatEnd);

			if(formatEnd+1 > formatAlloc)
			{
				free(partFormat);
				if((partFormat = malloc((formatEnd+1)*sizeof(*partFormat))) == NULL) goto bad_alloc;
				formatAlloc = formatEnd+1;
			}
			strncpy(partFormat, formatStart, formatEnd);
			partFormat[formatEnd] = '\0';

			formatStart = &formatStart[formatEnd];

			//prepare cell string
			//if(strchr(list, '\0') < listStart) goto invalid_list;

			unsigned short listEnd;
			for(listEnd = 0; listStart[listEnd] != '\n' && listStart[listEnd] != '\t'; ++listEnd)
				if(listStart[listEnd] == '\0') goto invalid_list;

			if(listEnd+1 > listAlloc)
			{
				free(partList);
				if((partList = malloc((listEnd+1)*sizeof(*partList))) == NULL) goto bad_alloc;
				listAlloc = listEnd+1;
			}
			strncpy(partList, listStart, listEnd);
			partList[listEnd] = '\0';

			listStart += (listEnd+1)*sizeof(*listStart);

			//print cell
			printf(partFormat, partList);
		}
		//printf(" %x(%c) ", listStart[0], listStart[0]);
		listStart = strchr(listStart, '\n')+sizeof(*listStart);
	}

	if(0)
	bad_alloc:
		perror("\n\nerror! Bad alloc.");

	if(0)
	invalid_list:
		perror("\nInvalid list");

	free(partFormat);
	free(partList);
	return;
}

char* removeLine(char** list, unsigned int line)
{
	char* startPoint = *list;
	char* endPoint = strchr(startPoint, '\n');

	for(;line > 0;--line)
	{
		startPoint = endPoint;
		endPoint = strchr(&startPoint[1], '\n');
		if(endPoint == NULL) goto line_out_of_range;
	}
	startPoint[0] = '\0';
	char* temp = malloc((strlen(*list)+strlen(endPoint)+1)*sizeof(*temp));
	if(temp == NULL) goto bad_alloc;

	strcpy(temp, *list);
	strcat(temp, endPoint);

	free(*list);
	*list = temp;
	return temp;
	//exception list
	bad_alloc:
		free(temp);
		return NULL;

	line_out_of_range:
		return *list;
}

///cellLength can be NULL, in this case the pointer will be only returned, otherwise the pointer would be both returned and stored in cellLength
int* getMaxLenght(const char* list, int** cellLength)
{
	const char* startList = list;
	int* sArray = malloc(sizeof(*sArray));
	if(sArray == NULL) goto bad_alloc;
	sArray[0] = 0;

	while(startList[0] != '\0')
	{
		for(size_t i = 1;startList[0] != '\n';++i)
		{
			if(i > sArray[0])
			{
				int* temp = realloc(sArray, (i+1)*sizeof(*sArray));
				if(temp == NULL) goto bad_alloc;
				sArray = temp;

				sArray[i] = 0;
			}
			sArray[0] = max(i, sArray[0]);

			size_t size;
			for(size = 0; startList[size] != '\t'; ++size);
			sArray[i] = max(size, sArray[i]);
			startList = strchr(startList, '\t')+sizeof(*startList);
		}
		startList += sizeof(*startList);
	}

	if(cellLength != NULL)
		*cellLength = sArray;

	return sArray;
	//exception list
	bad_alloc:
		free(sArray);
		return NULL;
}

/**application specific functions**/

char* putLine(char** list, const char* line)
{
	char* startPoint = *list;
	char* strtolListEnd;

	long ID = strtol(startPoint, &strtolListEnd, 10) * (strtolListEnd[0] == '\t' ? 1 : 0);
	while(ID < strtol(line, &strtolListEnd, 10) * (strtolListEnd[0] == '\t' ? 1 : 0))
	{
		startPoint = strchr(&startPoint[1], '\n');
		if(startPoint == NULL)
		{
			startPoint = strchr(*list, '\0') - sizeof(*startPoint);
			break;
		}

		ID = strtol(&startPoint[1], &strtolListEnd, 10) * (strtolListEnd[0] == '\t' ? 1 : 0);
	}

	char* tempList = malloc((strlen(line) + strlen(*list) + 1)*sizeof(*tempList));
	if(tempList == NULL) goto bad_alloc;

	strncpy(tempList, *list, (&startPoint[1] - *list)/sizeof(**list));
	strcpy(&tempList[(&startPoint[1] - *list)/sizeof(**list)], line);
	strcat(tempList, &startPoint[1]);

	free(*list);
	*list = tempList;
	return tempList;

	//exceptions
	bad_alloc:
		free(tempList);
		return NULL;
}

///FALSE if not found. Position starting with 1 otherwise
BOOL hasID(const char* list, const char* ID)
{
	const char* startPoint = list;
	size_t IDSize = strlen(ID);

	for(size_t i = 1;startPoint != NULL; ++i)
	{
		if(strncmp(&startPoint[1], ID, IDSize) == 0)
			return i;
		startPoint = strchr(&startPoint[1], '\n');
	}
	return FALSE;
}

///If ID is NULL or the integer represented by ID is out of range of int the behavior is undefined
///The function will fail if it try to convert a string without numbers to int
char* getNextPossibleID(const char* list, char** ID)
{
	int startPoint = atoi(*ID);
	const char* currentID = list;

	while(startPoint > atoi(currentID))
		if((currentID = strchr(&currentID[1], '\n')) == NULL)
			return *ID;

	while(startPoint++ == atoi(currentID))
		if((currentID = strchr(&currentID[1], '\n')) == NULL)
			return *ID;

	//alloc for data
	char* newID = malloc((snprintf(NULL, 0, "%d", --startPoint)+1)*sizeof(*newID));
	if(newID == NULL) goto bad_alloc;

	sprintf(newID, "%d", startPoint);

	free(*ID);
	*ID = newID;
	return newID;
	//exception list
	bad_alloc:
		free(newID);
		return NULL;
}

int main()
{
	if(setConsoleUtfEncoding() == FALSE) return 1;

	char* productList =	strdup("IDs\t"	  "PRODUTOS\t"	"PREÇO\t"	"QUANT\t"	"\n"			//each column in the table must end with a \t and each line with a \n,
								 "0\t"	      "ovos\t"	 "0.34\t"	  "665\t"	"\n"			//so in the end of each line must have a \t\n to signal the end of a column and line.
								 "1\t"	     "leite\t"	 "1.77\t"	  "566\t"	"\n"			//Since the table is a terminated in zero string, it must have an ending \0,
								 "2\t"	    "queijo\t"	 "7.24\t"	  "435\t"	"\n"			//so the last line must end with \t\n\0 to end of column, end of line and end of string.
								 "3\t"	    "batata\t"	 "5.98\t"	  "832\t"	"\n"
								 "4\t"	    "alface\t"	 "3.35\t"	  "389\t"	"\n"
								 "5\t"	   "cenoura\t"	"13.78\t"	  "353\t"	"\n"
								 "7\t"	    "banana\t"	 "9.22\t"	  "118\t"	"\n"
								"12\t"	     "arroz\t"	 "1.88\t"	  "657\t"	"\n"
								"13\t"	       "sal\t"	 "1.28\t"	  "170\t"	"\n"
								"14\t"	      "suco\t"	 "1.87\t"	   "81\t"	"\n"
								"15\t"	    "tomate\t"	 "2.10\t"	  "206\t"	"\n"
								"16\t"	    "cebola\t"	 "6.23\t"	  "660\t"	"\n"
								"17\t"	   "farinha\t"	"14.83\t"	  "736\t"	"\n"
								"19\t"	    "salame\t"	 "0.21\t"	  "807\t"	"\n"
								"20\t"	 "mortadela\t"	"10.89\t"	  "290\t"	"\n"
								"21\t"	     "milho\t"	 "9.73\t"	   "31\t"	"\n"
								"22\t"	  "manteiga\t"	"13.97\t"	  "980\t"	"\n"
								"23\t"	    "pipoca\t"	"14.32\t"	  "170\t"	"\n"
								"25\t"	"detergente\t"	"12.45\t"	  "656\t"	"\n"
								"33\t"	  "sabonete\t"	 "8.83\t"	  "441\t"	"\n");
	char* nextID = strdup("34");
	char* firstPossibleID = strdup("6");
	char* format = strdup("%03s\t%-10s\t%5s\t%5s\n");
	char* halfFormat = strdup("%03s\t%-10s\t\n");
	int* tagMax = getMaxLenght(productList, NULL);
	if(productList == NULL || nextID == NULL || firstPossibleID == NULL || format == NULL || halfFormat == NULL || tagMax == NULL) goto bad_alloc;

	char command = 0;
	while(command != 's' && command != 'x')
	{
		if(command == 'h')
		{
			printf("digite 'l' para listar os produtos\n"
				   "digite 'a' para adicionar produto\n"
				   "digite 'r' para remover produto\n"
				   "digite 's' para sair do programa\n");
		}
		else if(command == 'l')
		{
			printList(format, productList);
		}
		else if(command == 'a')
		{
			printList(halfFormat, productList);
			printf("\nDigite a linha da tabela no formato 'ID||produto||preço||quantidade' sem as aspas.\nQualquer espaço será considerado como parte da coluna\n"
				   "'|' pode ser escrito com shift+\\. Geralmente '\\' está entre o 'z' e o shift esquerdo\n\n"
				   "Para definir o ID automaticamente pode substituir o campo ID por 'c' ou 'p',\nadicionar no final ou no primeiro espaço livre respectivamente\n\n"
				   "linha:");

			size_t column = 1;
			size_t inputSize = 0;
			size_t inputReserve = 3;
			char* input = malloc((1<<inputReserve)*sizeof(*input));
			int ch;
			while((ch = getchar()) != '\n' && ch != EOF)
			{
				if(inputSize == 0 && (ch == 'c' || ch == 'p'))
				{
					inputSize += strlen(ch == 'c' ? nextID : firstPossibleID);
					if(inputSize+2 >= 1<<inputReserve)
					{
						char* temp = realloc(input, (1<<++inputReserve)*sizeof(*input));
						if(temp == NULL)
						{
							free(input);
							goto bad_alloc;
						}
						input = temp;
					}

					strcpy(input, ch == 'c' ? nextID : firstPossibleID);
					getNextPossibleID(productList, ch == 'c' ? &nextID : &firstPossibleID);
				}
				else if(ch != '|' || input[inputSize-1] != '|')
				{
					input[inputSize++] = ch;
					if(inputSize+2 >= 1<<inputReserve)
						input = realloc(input, (1<<++inputReserve)*sizeof(*input));
				}
				else
				{
					input[inputSize-1] = '\t';
					++column;
				}
			}
			input[inputSize] = '\t';
			input[inputSize+1] = '\n';
			input[inputSize+2] = '\0';

			strchr(input, '\t')[0] = '\0';

			if(!hasID(productList, input) && column == 4)
			{
				strchr(input, '\0')[0] = '\t';
				putLine(&productList, input);
				getMaxLenght(productList, &tagMax);

				free(format);
				sprintfAlloc(&format, "%%0%ds\t%%-%ds\t%%%ds\t%%%ds\t\n", tagMax[1], tagMax[2], tagMax[3], tagMax[4]);

				free(halfFormat);
				sprintfAlloc(&halfFormat, "%%0%ds\t%%-%ds\t\n", tagMax[1], tagMax[2]);
			}
			else if(column == 4)
			{
				printf("ID já existente\n");
			}
			else
			{
				printf("formato incorreto\n");
			}
			free(input);
		}
		else if(command == 'r')
		{
			printList(halfFormat, productList);
			printf("Especifique qual ID:");

			size_t inputSize = 0;
			size_t inputReserve = 3;
			char* input = malloc((1<<inputReserve)*sizeof(*input));
			int ch;
			while((ch = getchar()) != '\n' && ch != EOF)
			{
				input[inputSize++] = ch;
				if(inputSize+1 >= 1<<inputReserve)
					{
						char* temp = realloc(input, (1<<++inputReserve)*sizeof(*input));
						if(temp == NULL)
						{
							free(input);
							goto bad_alloc;
						}
						input = temp;
					}
			}
			input[inputSize] = '\t';
			input[inputSize+1] = '\0';

			size_t line = hasID(productList, input);
			if(line)
			{
				removeLine(&productList, line-1);
				free(tagMax);
				getMaxLenght(productList, &tagMax);

				free(format);
				sprintfAlloc(&format, "%%0%ds\t%%-%ds\t%%%ds\t%%%ds\t\n", tagMax[1], tagMax[2], tagMax[3], tagMax[4]);

				free(halfFormat);
				sprintfAlloc(&format, "%%0%ds\t%%-%ds\t\n", tagMax[1], tagMax[2]);
			}
			else
			{
				printf("ID não existente\n");
			}
			free(input);
		}

		printf("digite 'h' para listar possiveis comandos\n"
			   "comando:");

		int trash;
		while((trash = getchar()) != EOF && trash != '\n')
			command = trash;
	}

	//exception list
	if(0)
	bad_alloc:
		printf("\n\nerror! Bad alloc.\n");

	free(productList);
	free(nextID);
	free(firstPossibleID);
	free(format);
	free(halfFormat);
	free(tagMax);
	return 0;
}
