#include<stdlib.h>
#include<stdint.h>
#include<errno.h>

typedef struct {
    size_t size;
    size_t reserved;
    char* charArray;
} string;

#define getLineString() fGetString(stdin, '\n')
#define fGetLineString(file) fGetString(file, '\n')
#define getString(delimiter) fGetString(stdin, delimiter);

char* fGetString(FILE* file, char delimiter)
{
    string str = {0, 1, malloc(sizeof(char))};
    for(int ch = getc(file); ch != delimiter && ch != EOF; ch = getc(file))
    {
        ++str.size;
        if(str.size == str.reserved)
        {
            str.reserved *= 2;
            char* temp = realloc(str.charArray, sizeof(char)*str.reserved);
            if(temp == NULL)
            {
                free(str.charArray);
                return NULL;
            }
            str.charArray = temp;
        }
        str.charArray[str.size-1] = ch;
    }
    str.charArray[str.size] = '\0';

    return str.charArray;
}

long getLong(char delimiter, int8_t* erro)
{
    *erro = 0;
    char* input = getString(delimiter);
    if(input == NULL)
    {
        *erro = 1;
        return 0;
    }
    char* end;
    int value = strtol(input, &end, 10);
    free(input);
    if(input == end)
    {
        *erro = 2;
        return 0;
    }
    if(errno == ERANGE)
    {
        *erro = 3;
        return value;
    }
    return value;
}

float getFloat(char delimiter, int8_t* erro)
{
    *erro = 0;
    char* input = getString(delimiter);
    if(input == NULL)
    {
        *erro = 1;
        return 0;
    }
    char* end;
    float value = strtof(input, &end);
    free(input);
    if(input == end)
    {
        *erro = 2;
        return 0;
    }
    if(errno == ERANGE)
    {
        *erro = 3;
        return value;
    }
    return value;
}
