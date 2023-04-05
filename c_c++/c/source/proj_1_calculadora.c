#define UNICODE
#define _UNICODE

#include<stdio.h>
#include<stdlib.h>
#ifdef _WIN32
#include<windows.h>
#endif // _WIN32

#define CLEAR 0
#define OVERFLOW 1
#define UNDERFLOW 2
#define NAN 3
#define IMPRECISE 4

typedef long (*operation)(long var1, long var2, char* flag);

long sum(long var1, long var2, char* flag)
{
    *flag = CLEAR;

    if(var1 > 0 && var2 > 0 && var1 + var2 < 0)
        *flag = OVERFLOW;

    if(var1 < 0 && var2 < 0 && var1 + var2 > 0)
        *flag = UNDERFLOW;

    return var1 + var2;
}

long mul(long var1, long var2, char* flag)
{
    *flag = CLEAR;

    unsigned long result = (unsigned long)(abs(var1)) * (unsigned long)(abs(var2));

    if(result < abs(var1) || result < abs(var2))
        *flag = OVERFLOW;
    else if(var1 > 0 && var2 > 0 && result > LONG_MAX)
        *flag = OVERFLOW;
    if((var1 < 0 && var2 > 0) || (var1 < 0 && var2 > 0))
    {
        return (long)result * -1;
    }
    return (long)result;
}

long division(long var1, long var2, char* flag)
{
    *flag = CLEAR;
    if(var2 == 0)
    {
        *flag = NAN;
        return 0;
    }
    long result = var1/var2;
    if(result*var2 != var1)
        *flag = IMPRECISE;

    return result;
}

int main()
{
    #ifdef _WIN32
    if(!SetConsoleCP(CP_UTF8))          return 1;
    if(!SetConsoleOutputCP(CP_UTF8))    return 1;
    #endif // _WIN32

    printf("escreva expressão:");

    int ch;
    long var1 = 0, var2 = 0, var3 = 0;
    operation op = NULL, op2 = NULL;
    char operation = FALSE;
    char error = CLEAR;
    char imprecise = FALSE;
    char negative = FALSE;
    do
    {
        ch = getc(stdin);

        if((unsigned int)(ch-'0') < 9)
        {
            operation = TRUE;
            var2 *= 10;
            if(!negative)
            {
                var2 += ch-'0';
            }
            else
            {
                var2 -= ch-'0';
            }
        }
        else
        {
            if(ch == '+' || ch == '-' || ch == '*' || ch == 'x' || ch == 'X' || ch == '/' || ch == '\\' || ch == ':' || ch == '\n')
            {
                if(op != NULL && operation == TRUE)
                {
                    if(op2 != NULL)
                    {
                        if(op2 == sum && op != sum)
                        {
                            var2 = op(var1, var2, &error);
                            var1 = var3;
                            var3 = 0;
                            op = op2;
                        }
                        else
                        {
                            var1 = op2(var3, var1, &error);
                            var3 = 0;
                        }
                    }
                    else if(ch != '\n')
                    {
                        op2 = op;
                        var3 = var1;
                        var1 = var2;
                        var2 = 0;
                    }

                    if(ch == '\n')
                    {
                        var1 = op(var1, var2, &error);
                    }
                }
                else if(operation == TRUE)
                {
                    var1 = var2;
                    var2 = 0;
                }

                negative = FALSE;
            }

            switch(ch)
            {
            case '+':
                op = sum;
                goto common;                        //go to last switch case label

            case '-':
                if(operation == TRUE)
                    op = sum;
                negative = TRUE;
                goto common;                        //go to last switch case label

            case '*':
            case 'x':
            case 'X':
                op = mul;
                goto common;                        //go to last switch case label

            case '/':
            case '\\':
            case ':':
                op = division;
                goto common;                        //go to last switch case label

            case '\n':
            common:
                operation = FALSE;
                break;
            }
        }

        if(error == IMPRECISE)
        {
            imprecise = TRUE;
            error = CLEAR;
        }
    }while(ch != EOF &&ch != '\n' && error == CLEAR);

    if(error != CLEAR)
    {
        if(error == OVERFLOW || error == UNDERFLOW)
            printf("Resultado excedeu limite da variável!\n");
        if(error == NAN)
            printf("Impossível dividir por zero!\n");
    }
    else
    {
        if(imprecise == 1)
            printf("As casas decimais foram desconsideradas para a divisão!\n");

        printf("resultado: %ld\n", var1);
    }

    getchar();

    return 0;
}
