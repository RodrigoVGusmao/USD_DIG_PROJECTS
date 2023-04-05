//ver f
/*def hiper_periodo(processos, qnt):
    # Hiper Periodo � o maior periodo dentre todos os processos
    temp = 0
    for i in range(qnt):
        if processos[i][3] > temp:
            temp = processos[i][3]
    return temp

def escolher_menor_deadline(processos, qnt, deadlines):
    menor_deadline = 10000
    escolhido = -1
    for i in range(qnt):
        if deadlines[i] < menor_deadline:
            menor_deadline = deadlines[i]
            escolhido = i
    return escolhido

def edf(processos, qnt):
    relogio = 0
    deadlines = [0] * qnt
    for i in range(qnt):
        deadlines[i] = processos[i][2]
    periodos = [0] * qnt
    for i in range(qnt):
        periodos[i] = processos[i][3]
    print(f"Processos: {processos}")
    print(f"Deadlines: {deadlines}")
    print(f"Periodos: {periodos}\n")
    contador = [0] * qnt

    while True:
        escolhido = escolher_menor_deadline(processos, qnt, deadlines)
        print(f"Processo Escolhido: {escolhido}")
        if periodos[escolhido] >= relogio:
            relogio += processos[escolhido][1]
            print(f"Processo: P{escolhido} executando...")
            print(f"Relogio: {relogio}")
            print(f"Burst Time do Processo P{escolhido}: processos[escolhido][1]")

            print(f"Deadline ANTERIOR do Processo : {deadlines[escolhido]}")
            deadlines[escolhido] += processos[escolhido][3]
            print(f"Deadline do Processo P{escolhido} Atualizada: {deadlines[escolhido]}")

            print(f"Periodo ANTERIOR do Processo: {periodos[escolhido]}")
            periodos[escolhido] += processos[escolhido][3]
            print(f"Periodo do Processo P{escolhido} Atualizado: {periodos[escolhido]}\n")
            contador[escolhido] += 1
        if relogio >= 20:
            break

    for i in range(qnt):
        print(f"O Processo P{i} Executou {contador[i]} vezes")

            #0  1  2  3
            #id bt dl p
processos = [
            [0, 3, 7, 20],
            [1, 2, 4, 5],
            [2, 2, 8, 10]
            ]

qnt = len(processos)
edf(processos, qnt)*/

#include<stdlib.h>
#include<stdio.h>

typedef struct
{
    int id;
    int burstTime;
    int deadLine;
    int periodo;
    int periodoSoma;
    int quantidade;
} processo;

void edf(processo* processos, size_t len);

int main()
{
    processo processos[] = {{0, 3, 7, 20, 20, 0}, {1, 2, 4, 5, 5, 0}, {2, 2, 8, 10, 10, 0}};

    edf(processos, sizeof(processos)/sizeof(processo));

    return 0;
}

processo* escolher_menor_deadline(processo* processos, size_t len)
{
	if(len <= 1)
		return processos;
	
	processo* menor = processos[0].deadLine < processos[1].deadLine ? &(processos[0]) : &(processos[1]);
	for(size_t i = 2; i < len; ++i)
	{
		menor = menor->deadLine < processos[i].deadLine ? menor : &(processos[i]);
	}
	
	return menor;
}

void edf(processo* processos, size_t len)
{
    int* deadlines = &(processos[0].deadLine);
    int* periodos = &(processos[0].periodo);

    printf("Processos: \n");
    for(size_t i = 0; i < len; ++i)
        printf("%d %d %d %d\n", processos[i].id, processos[i].burstTime, processos[i].deadLine, processos[i].periodo);
    printf("Deadlines: ");
    for(size_t i = 0; i < len; ++i)
        printf("%d ", processos[i].deadLine);
    printf("\nPeriodos: ");
    for(size_t i = 0; i < len; ++i)
        printf("%d ", processos[i].periodo);
        
	int relogio = 0;
    do
    {
    	processo* escolhido = escolher_menor_deadline(processos, len);
    	printf("\n\nProcesso escolhido: %d, %d, %d, %d\n", escolhido->id, escolhido->burstTime, escolhido->deadLine, escolhido->periodo);
    	if(escolhido->periodoSoma >= relogio)
    	{
    		relogio += escolhido->burstTime;
    		printf("Executando processo %d...\n", escolhido->id);
    		printf("Relogio: %d\n", relogio);
            printf("Burst Time do Processo %d: %d\n", escolhido->id, escolhido->burstTime);

            printf("Deadline ANTERIOR do Processo %d: %d\n", escolhido->id, escolhido->deadLine);
            escolhido->deadLine += escolhido->periodo;
            printf("Deadline do Processo %d Atualizada: %d\n", escolhido->id, escolhido->deadLine);

            printf("Periodo ANTERIOR do Processo %d: %d\n", escolhido->id, escolhido->periodo);
            escolhido->periodoSoma += escolhido->periodoSoma;
            printf("Periodo do Processo %d Atualizado: %d", escolhido->id, escolhido->periodoSoma);
            escolhido->quantidade += 1;
    	}
	}while(relogio < 20);
	
	printf("\n\n");
	for(size_t i = 0; i < len; ++i)
	{
		printf("O processo %d foi executado %d vezes\n", processos[i].id, processos[i].quantidade);
	}
}