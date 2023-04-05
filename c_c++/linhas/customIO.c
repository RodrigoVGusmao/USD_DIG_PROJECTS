#include "customIO.h"

#include <stdio.h>
#include <stdint.h>

void askInput(char* msg, char* fmt, int numArgs, int argsMin, int cicle, void (*func)(uint64_t), void (*textPre)(void))
{
    union {
        int16_t args_v[4];
        uint64_t args_a;
    } args;
    uint32_t start;
    textPre();
    printf("%s (formato: %s)\n", msg, fmt);
    int fmtChars = 0;
    for(;fmt[fmtChars] != ' ' && fmt[fmtChars] != '\0'; ++fmtChars);
    ++fmtChars;
    int ch = 0, extract, argc = 0;
    do
    {
        if(ch == '\n')
        {
            textPre();
            printf("Parametros insuficientes: (formato: %s)\n", fmt+fmtChars*(argc % numArgs));
        }
        textPre();
        do
        {
            int result = getInt(&ch, &extract);
            if(extract == 1)
            {
                args.args_a <<= 16;
                args.args_a |= (uint16_t)result;
                ++argc;
            }

            if(argc % numArgs == 0 && argc >= argsMin)
            {
                func(args.args_a);
            } else if (argc == numArgs && cicle)
            {
                start = args.args_a;
            }

        } while (ch != '\n');
        flush(&ch);
    } while(argc % numArgs != 0 || argc < argsMin);

    if(cicle)
    {
        args.args_a <<= 32;
        args.args_a |= start;
        func(args.args_a);
    }
}

void askHexInput(char* msg, char* fmt, int numArgs, int argsMin, int cicle, void (*func)(uint64_t), void (*textPre)(void))
{
    union {
        int16_t args_v[4];
        uint64_t args_a;
    } args;
    uint32_t start;
    textPre();
    printf("%s (formato: %s)\n", msg, fmt);
    int fmtChars = 0;
    for(;fmt[fmtChars] != ' ' && fmt[fmtChars] != '\0'; ++fmtChars);
    ++fmtChars;
    int ch = 0, extract, argc = 0;
    do
    {
        if(ch == '\n')
        {
            textPre();
            printf("Parametros insuficientes: (formato: %s)\n", fmt+fmtChars*(argc % numArgs));
        }
        textPre();
        do
        {
            int result = getHex(&ch, &extract);
            if(extract == 1)
            {
                args.args_a <<= 32;
                args.args_a |= (uint32_t)result;
                ++argc;
            }

            if(argc % numArgs == 0 && argc >= argsMin)
            {
                func(args.args_a);
            } else if (argc == numArgs && cicle)
            {
                start = args.args_a;
            }

        } while (ch != '\n');
        flush(&ch);
    } while(argc % numArgs != 0 || argc < argsMin);

    if(cicle)
    {
        args.args_a <<= 32;
        args.args_a |= start;
        func(args.args_a);
    }
}

int getInt(int *lastChar, int *extract)
{
    int ch, negative = 1;
    while((ch = getchar()) != '-' && !(ch >= '0' && ch <= '9') && ch != '\n' && ch != EOF);

    *extract = 1;
    if(ch == '\n' || ch == EOF)
    {
        *lastChar = ch;
        *extract = 0;
        return 0;
    }

    if(ch == '-')
    {
        negative = -1;
        ch = getchar();
    }

    int result = 0;
    while(ch >= '0' && ch <= '9')
    {
        result *= 10;
        result += ch - '0';
        ch = getchar();
    }
    result *= negative;

    *lastChar = ch;

    return result;
}

int getHex(int *lastChar, int *extract)
{
    int ch;
    while((ch = getchar()) != '\n' && !((ch >= '0' && ch <= '9') || (ch >+ 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) && ch != EOF);

    *extract = 1;
    if(ch == '\n' || ch == EOF)
    {
        *lastChar = ch;
        *extract = 0;
        return 0;
    }

    int result = 0;
    while((ch >= '0' && ch <= '9') || (ch >+ 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
    {
        result *= 16;
        result += ch <= '9' ? ch - '0' : ch <= 'F' ? ch - 'A' + 10 : ch -'f' + 10;
        ch = getchar();
    }
    *lastChar = ch;

    return result;
}

void flush(int *ch)
{
    if(*ch != '\n' && *ch != EOF)
        while((*ch = getchar()) != '\n' && *ch != EOF);
}
