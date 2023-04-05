#if defined(__WIN32) || defined(__WIN64) || defined(_WIN32_) || defined(__WIN64__)
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

#include<stdio.h>
#include<stdint.h>

uint64_t fatorial(uint8_t fator)
{
	if(fator > 1)
	{
		return fator*fatorial(--fator);
	}
	else
	{
		return 1;
	}
}

int main()
{
	uint8_t fator;
	printf("insira o fatorial:");
	scanf("%d", &fator);

	printf("%d", fatorial(fator));
}
