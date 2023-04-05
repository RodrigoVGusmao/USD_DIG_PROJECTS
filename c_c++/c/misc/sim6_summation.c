#if defined(__WIN32) || defined(__WIN64) || defined(_WIN32_) || defined(__WIN64__)
#include<windows.h>
int utf8_ready()
{
	return (SetConsoleCP(CP_UTF8) && SetConsoleOutputCP(CP_UTF8));
}
#else
int utf8_ready()
{
	return 1;
}
#endif // defined

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdarg.h>
#include<math.h>

#include"vector_c.h"

#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // PI

#define TRY char* _try_error_message = "ERR_OK";
#define THROW(s, type) {_try_error_message = s; goto _##type##_error_u;}
#define GET_ERROR() fprintf(stderr, _try_error_message)
#define CATCH(type) if(0) _##type##_error_u:

typedef double (summationFunc)(double*);

///'result' pode receber NULL

/**caso 'format' seja NULL nada é feito é 'result' recebe 0 a menos que seja NULL
   'format' não diferencia maiúsculas de minúsculas**/

/**usa notação polonesa reversa para calcular expressão
   formato 'f'(float) para número na lista de argumentos e '+'; '-'; '*'; '/'; '^' para operações e 'k' para iterador;**/

///os próximos comandos esperam argumentos, caso não tenha argumentos suficientes processados, o comportamento é indefinido

/**use 'pn' para operações customizadas, sendo n (size_t) o número de argumentos double da função; formato da função: func(double* args)
   os argumentos serão os n últimos argumentos processados
   note que é esperado que os argumentos estejam na ordem inversa (abc p3 = p(c, b, a)), então inverta a ordem na função ou no formato da somatória
   caso 'n' seja omitido será considerado n=0 e a função receberá NULL como argumento
   nota: não é recomendável usar funções que alterem de alguma forma objetos, matrizes ou ponteiros que são usados como argumento na somátoria**/

/**use 'vt' para acessar o enésimo (n) termo da matriz 'v' de tipo 't'
   'n' será convertido em int e será o último argumento processado
   't' pode assumir os valores 'f'(float) ou 'd'(int), se 't' for omitido ou invalido, a operação é ignorada**/

///exemplo: summation(2, 0, 'fff+^f/f-kfp3v'), 4.0f, 2.0f, 3.0f, 5.0f, 6.0f, 7.0f, 8.0f, funcao, matriz); termo individual: matriz[funcao((2+3)^4/5-6, k, 8)]
///expressão total: matriz[funcao((2+3)^4/5-6, 0, 8)] + matriz[funcao((2+3)^4/5-6, 1, 8)] + matriz[funcao((2+3)^4/5-6, 2, 8)];

int summation(double* result, uint32_t maxIterations, uint32_t iterator, const char* format, ...);

int_v fibonacci;

double fact_double(double* args);

double mod(double value, double modMax)
{
	if(value < 0)
		return mod(value+modMax, modMax);
	else if(value >= modMax)
		return mod(value-modMax, modMax);
	else
		return value;
}

double insertValueSum(double* args)
{
	int value = (int)args[0];
	insertValue(&fibonacci, int, &value);
	return args[0];
}

int main()
{
	TRY
	{
		initVector(&fibonacci, 2, int, 0, 1);
		reserveVector(&fibonacci, int, 10);

		if(!utf8_ready()) THROW("impossivel definir CP para UTF8\n", void);

		int iteracoes;
		printf("insira a quantidade de repetições da somatória: ");
		scanf("%d", &iteracoes);

		double result;
		if(summation(&result, iteracoes, 1, "fkp1/", 1.0, fact_double) > 0) THROW("falha ao reservar memoria para somatória\n", void);
		printf("somatória:%lf\n", result);
		printf("fibonacci: %d %d\n", ((int*)(fibonacci.buffer))[0], ((int*)(fibonacci.buffer))[1]);
		if(summation(&result, 10, 1, "kvdkf-vd+p1", fibonacci.buffer, 1.0f, fibonacci.buffer, insertValueSum) > 0) THROW("falha ao reservar memoria para somatória\n", void);
		printf("result:%f\n", result);
		printf("fibonacci: %d %d %d %d %d %d %d %d %d %d\n", ((int*)(fibonacci.buffer))[0], ((int*)(fibonacci.buffer))[1], ((int*)(fibonacci.buffer))[2], ((int*)(fibonacci.buffer))[3], ((int*)(fibonacci.buffer))[4], ((int*)(fibonacci.buffer))[5], ((int*)(fibonacci.buffer))[6],((int*)(fibonacci.buffer))[7],((int*)(fibonacci.buffer))[8],((int*)(fibonacci.buffer))[9]);

		return 0;
	}
	CATCH(void)
	{
		GET_ERROR();
		return 1;
	}
}

int v_summation(double *result, uint32_t maxIterations, uint32_t iterator, const char* format, va_list list)
{
	if(format == NULL)
	{
		if(result != NULL)
			*result = 0;
		return 0;
	}
	double_v RPNstack;
	 initVector(&RPNstack, 0, double, NULL);

	size_t size = strlen(format);
	double term;
	*result = 0;
	for(size_t j = iterator; j < maxIterations+1; ++j)
	{
		va_list argv;
		va_copy(argv, list);
		for(size_t i = 0; i < size; ++i)
		{
			double op1, op2;
			if(format[i])
			switch(format[i] | 0x20)
			{
			case 'f':
			{
				double value = va_arg(argv, double);
				if(insertValue(&RPNstack, double, &value) == NULL) goto error;
				break;
			}

			case 'k':
				op1 = j;
				insertValue(&RPNstack, double, &op1);
				break;

			case 'p':
			{
				char* endPtr;
				long int argc = strtol(&format[i+1], &endPtr, 10);

				double_v args;
				initVector(&args, 0, double, NULL);

				summationFunc* proc = va_arg(argv, summationFunc*);
				for(size_t k = 0; k < argc; ++k)
				{
					getLastValue(&RPNstack, double, &op1);
					if(insertValue(&args, double, &op1) == NULL) goto error;
				}

				op2 = proc((double*)args.buffer);
				if(insertValue(&RPNstack, double, &op2) == NULL) goto error;
				destroyVector(&args);
				break;
			}

			case 'v':
			{
				getLastValue(&RPNstack, double, &op1);

				if((format[++i] | 0x20) == 'd')
					op1 = (double)(va_arg(argv, int*)[(int)op1]);
				else if((format[i] | 0x20) == 'f')
					op1 = (double)(va_arg(argv, float*)[(int)op1]);

				insertValue(&RPNstack, double, &op1);
				break;
			}

			case '+':
			case '-':
			case '*':
			case '/':
			case '^':
				getLastValue(&RPNstack, double, &op2);
				getLastValue(&RPNstack, double, &op1);
				switch(format[i] | 0x20)
				{
				case '+':
					op1 += op2;
					break;
				case '-':
					op1 -= op2;
					break;
				case '*':
					op1 *= op2;
					break;
				case '/':
					op1 /= op2;
					break;
				case '^':
					op1 = pow(op1, op2);
					break;
				}
				if(insertValue(&RPNstack, double, &op1) == NULL) goto error;
				break;
			}
		}
		va_end(argv);
		getLastValue(&RPNstack, double, &term);
		*result += term;
	}

	destroyVector(&RPNstack);
	return 0;
	error:
		destroyVector(&RPNstack);
		return 1;
}

///usa notação polonesa reversa para calcular expressão
///formato 'f'(float) para número na lista de argumentos e '+'; '-'; '*'; '/'; '^' para operações e 'k' para iterador;
///use 'pn' para operações customizadas, sendo n (size_t) o número de argumentos double da função; formato da função: func(double* args)
///note que é esperado que os argumentos estejam na ordem inversa, então inverta a ordem na função ou no formato da somatória

///exemplo: summation(2, 0, 'fff+^f/f-kfp3'), 4.0f, 2.0f, 3.0f, 5.0f, 6.0f, 7.0f, 8.0f, funcao); termo individual: funcao((2+3)^4/5-6, k, 8)
///expressão total: funcao((2+3)^4/5-6, 0, 8) + funcao((2+3)^4/5-6, 1, 8) + funcao((2+3)^4/5-6, 2, 8);

int summation(double* result, uint32_t maxIterations, uint32_t iterator, const char* format, ...)
{
	va_list argv;
	va_start(argv, format);
	int return_value = v_summation(result, maxIterations, iterator, format, argv);
	va_end(argv);
	return return_value;
}

double fact_double(double* args)
{
	double factorial = args[0]--;
	if(args[0] > 0)
		return factorial*fact_double(args);
	else
		return 1;
}
