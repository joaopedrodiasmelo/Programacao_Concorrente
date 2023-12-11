#include <stdio.h>
#include <stdlib.h>
#include "../Conjunto_de_Structs/Structs.h"

int quantidade_pontos = 0; // variável global para auxiliar na identificação de cada ponto

// cria um Ponto de onibus  da Lista ,atribuindo o seu valor e colocando seus ponterios como NULL
Ponto_de_Onibus *Criar_Ponto()
{
    Ponto_de_Onibus *ponto = calloc(1, sizeof(Ponto_de_Onibus));
    ponto->identificador_ponto = quantidade_pontos;
    ponto->onibus_presente = -1;
    ponto->fila_presente_no_ponto = criar_fila();
    pthread_mutex_init(&ponto->mutex_do_ponto, NULL);
    quantidade_pontos++;

    return ponto;
}

void inicializar_Lista_Circular()
{

    lista_de_pontos = calloc(numero_de_pontos, sizeof(Lista_circular_pontos));
    for (int a = 0; a < numero_de_pontos; a++)
        lista_de_pontos[a].f
}
