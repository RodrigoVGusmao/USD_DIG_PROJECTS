#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

#if defined(__WIN32) || defined(__WIN64)
#include<windows.h>
int utf8_ready()
{
	return ~0 & SetConsoleCP(CP_UTF8) & SetConsoleOutputCP(CP_UTF8);
}
#else
int utf8_ready()
{
	return 1;
}
#endif // defined

typedef int operation (int, int);

int nil(int a, int b)
{
	return a+b;
}
int multi(int a, int b)
{
	return a*b;
}
int divi(int a, int b)
{
	return a/b;
}

union value
{
	struct exp *exp;
	int num;
};

struct exp
{
	union value num1;
	operation* op;
	int num2;
};

struct exp* initNextExp(size_t* size, uint8_t* flag, int8_t* multiplier, struct exp** exps)
{
	*multiplier = 1;
	*size += *flag--;
	*exps = realloc(*exps, *size*sizeof(struct exp));
	if(*exps == NULL) goto error;
	(*exps)[*size-1].num1.exp = exps[*size-2];
	(*exps)[*size-1].num2 = 0;

	error:
	return *exps;
}

int main()
{
	struct exp* exps = malloc(sizeof(struct exp));
	int ch;
	size_t size = 1;
	uint8_t nextExpr;
	int8_t multiplier;

	if(!utf8_ready()) goto error;
	if(exps == NULL) goto error;

	while(1)
	{
		exps[0].num1.num = 0;
		exps[0].op = nil;
		exps[0].num2 = 0;

		multiplier = 1;

		printf("\ndigite a expressão: ");
		while((ch = getc(stdin)) != '\n' && ch != EOF)
		{
			switch(ch)
			{
			case 'x':
			case '*':
				if(initNextExp(&size, &nextExpr, &multiplier, &exps) == NULL) goto error;
				exps[size-1].op = multi;
				break;

			case '/':
			case ':':
				if(initNextExp(&size, &nextExpr, &multiplier, &exps) == NULL) goto error;
				exps[size-1].op = divi;
				break;

			case '+':
				if(initNextExp(&size, &nextExpr, &multiplier, &exps) == NULL) goto error;
				exps[size-1].op = nil;
				break;

			case 'q':
				goto exit;

			default:
				if(ch == '-')
				{
					if(exps[size-1].num2 != 0)
					{
						if(initNextExp(&size, &nextExpr, &multiplier, &exps) == NULL) goto error;
						exps[size-1].op = nil;
					}

					multiplier = -1;
				}
				if(ch >= '0' && ch <= '9')
				{
					exps[size-1].num2 *= 10;
					exps[size-1].num2 += multiplier*(ch - '0');
				}
				nextExpr = 1;
				break;
			}
		}
		for(size_t i = 0;i<size;++i)
		{
			//printf("%d %c %d\t", exps[i].num1.num, exps[i].op==nil ? '+' : (exps[i].op==multi ? '*' : '/'), exps[i].num2);
			size_t next = i<size-1 ? i+1 : i;
			if(exps[i].op == nil && exps[next].op != nil)
			{
				exps[next].num2 = exps[next].op(exps[i].num2, exps[next].num2);
				exps[next].num1.num = exps[i].num1.num;
				exps[next].op = exps[i].op;
				printf("multi %d = %d %c %d\n", exps[next].num2, exps[i].num2, exps[next].op == multi ? '*' : '/', exps[next].num2);
			}
			else
			{
				exps[next].num1.num = exps[i].op(exps[i].num1.num, exps[i].num2);
				printf("nil %d = %d %c %d\n", exps[next].num1.num, exps[i].num1.num, '+', exps[i].num2);
			}
		}
		printf("%d", exps[size-1].num1);
		size = 1;
	}
	exit:
	free(exps);

	return 0;

	error:
	return 1;
}
