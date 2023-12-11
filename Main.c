#include <stdio.h>
#include <stdlib.h>
#include "./Conjunto_de_Structs/Structs.h"

// Variáveis Globais
onibus_circulantes *conjunto_de_onibus;
int numero_de_pontos;
Lista_circular_pontos *lista_de_pontos; // lista que contém os pontos com as filas de passageiros
Passageiro *passageiros_no_onibus;
int passageiros_que_sobraram;
pthread_mutex_t mutex_auxiliar;

int main()
{
    int quant_pontos, quant_passageiros, quant_assentos, quant_onibus;

    printf("Digite o número de pontos de onibus\n");
    scanf("%d", &quant_pontos);

    printf("Digite o número de onibus\n");
    scanf("%d", &quant_onibus);

    printf("Digite o número de passageiros\n");
    scanf("%d", &quant_passageiros);

    printf("Digite o número de assentos presentes no onibus\n");
    scanf("%d", &quant_assentos);

    // inicializando as variáveis globais
    numero_de_pontos = quant_pontos;
    passageiros_que_sobraram = quant_passageiros;
    inicializar_onibus(quant_onibus, quant_assentos);
    inicializar_Lista_Circular();
    pthread_mutex_init(&mutex_auxiliar, NULL);
    inicializar_passageiros(quant_passageiros, quant_pontos);

    // inicializando as threads dos Passageiros
    pthread_t *threads_de_passageiros = calloc(quant_passageiros, sizeof(pthread_t));
    for (int a = 0; a < quant_passageiros; a++)
    {
        // Aloca espaço para o índice
        int *indice_passageiros = malloc(sizeof(int));
        *indice_passageiros = a;

        if (pthread_create(&threads_de_passageiros[a], 0, (void *)thread_passageiro, (void *)indice_passageiros) != 0)
        {
            printf("Erro ao criar thread do passageiro \n");
            exit(0);
        }
    }

    // inicializando as threads dos Onibus
    pthread_t *threads_de_onibus = calloc(quant_onibus, sizeof(pthread_t));

    // Semente para a função rand() - geralmente é definida com o tempo atual
    srand((unsigned int)time(NULL));

    for (int b = 0; b < quant_onibus; b++)
    {
        // Aloca espaço para o índice
        int *indice_onibus = malloc(sizeof(int));
        *indice_onibus = b;
        if (pthread_create(&threads_de_onibus[b], 0, (void *)criar_onibus, (void *)indice_onibus) != 0)
        {
            printf("Erro ao criar thread do Onibus \n");
            exit(0);
        }
    }

    // Aguarde todas as threads terminarem antes de encerrar o programa
    for (int a = 0; a < quant_passageiros; a++)
    {
        pthread_join(threads_de_passageiros[a], NULL);
    }
    for (int b = 0; b < quant_onibus; b++)
    {
        pthread_join(threads_de_onibus[b], NULL);
    }

    return 0;
}