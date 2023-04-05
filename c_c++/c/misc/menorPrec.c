#ifdef _WIN32
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
int GetLastError()
{
	return 0;
}
#endif // _WIN32

#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<float.h>
#include<wchar.h>

#define ITERADOR_MAX 3

char* getString(char** string);
int getFloat(float *value, int *ch);
void flushIOBuffer(int ch);

struct produto {
	float preco;
	char* nome;
};

int main()
{
	setlocale(LC_ALL, "");
	fwide(stdout, 1);
	fputwc(L"\uFEFF", stdout);
	if(!utf8_ready())
	{
		printf("Nao foi possivel configurar o console.\nAlguns elementos podem nao ser apresentados corretamente. (Windows Error: %ld)\nContinuar mesmo assim? (S/n) ", GetLastError());
		int ch;
		if((ch = getchar()) == 'n' || ch == 'N')
			return 0;

		while ((ch = getchar()) != '\n' && ch != EOF);
	}

	struct produto menor = {FLT_MAX, NULL};
	struct produto atual = menor;

	for(int i = 0; i < ITERADOR_MAX;)
	{
		wprintf(L"\n%d\u00BA produto: ", ++i);
		if(getString(&atual.nome) == NULL)
			return 1;

		wprintf(L"preço: R$ ");
		atual.preco = 0.0f;
		int ch;
		getFloat(&atual.preco, &ch);
		flushIOBuffer(ch);

		if(menor.preco > atual.preco)
		{
			free(menor.nome);
			menor.nome = calloc(strlen(atual.nome), sizeof(char));
			strcpy(menor.nome, atual.nome);
			menor.preco = atual.preco;
		}
	}

	wprintf(L"O produto %s, custa R$%.2f e é o produto mais barato\n", menor.nome, menor.preco);
	getchar();

	return 0;
}

char* getString(char** string)
{
	free(*string);
	*string = malloc(sizeof(char));
	if(*string == NULL) return NULL;

	int ch, reserve = 1, size = 0;
	while((ch = getchar()) != '\n' && ch != EOF)
	{
		if(size > reserve)
		{
			reserve *= 2;
			char* tmp;
			tmp = realloc(*string, sizeof(char)*reserve);
			if(tmp == NULL)
			{
				free(*string);
				return NULL;
			}
			*string = tmp;
		}
		(*string)[size++] = ch;
	}
	return *string;
}

int getFloat(float *value, int *ch)
{
	while((unsigned int)((*ch = getchar())-'0') > 9 && *ch != '.' && *ch != ',' && *ch != '-' && *ch != EOF && *ch !='\n');		//limpa todos os caracteres inválidos
	if(*ch == '\n' || *ch == EOF) return 1;

	float expoente;
	do{
		if(*ch == '.' || *ch == ',')
			expoente = 1;
		else
		{
			*value *= 10;
			*value += (float)(*ch-'0');
			expoente *= 10;
		}
	}while(((unsigned char)((*ch = getchar())-'0') <= 9 || *ch == '-' || *ch == '.' || *ch == ',') && *ch != EOF && *ch !='\n');	//le todos os caracteres até encontrar um caractere inválido

	if(expoente > 0)
		*value /= expoente;

	return 0;
}

void flushIOBuffer(int ch)
{
	if(ch != '\n' && ch != EOF)
		while ((ch = getchar()) != '\n' && ch != EOF);
}
