#include <stdio.h>
#include <stdlib.h>
#include "../Conjunto_de_Structs/Structs.h"

int quantidade_pontos = 0; // variável global para auxiliar na identificação de cada ponto

// cria um Ponto de onibus  da Lista ,atribuindo o seu valor e colocando seus ponterios como NULL
Ponto_de_Onibus *Criar_Ponto()
{
    Ponto_de_Onibus *ponto = calloc(1, sizeof(Ponto_de_Onibus));
    ponto->anterior = NULL;
    ponto->proximo = NULL;
    ponto->identificador_ponto = quantidade_pontos;
    ponto->onibus_presente = -1;
    ponto->fila_presente_no_ponto = criar_fila();
    pthread_mutex_init(&ponto->mutex_do_ponto, NULL);
    quantidade_pontos++;

    return ponto;
}

// função responsável por conectar os pontos da lista circular
void criar_pontos()
{
    Ponto_de_Onibus *pontos;

    for (int a = 0; a < numero_de_pontos; a++)
    {
        pontos = Criar_Ponto();

        // lista está vazia
        if (lista_de_pontos->inicio == NULL && lista_de_pontos->final == NULL)
        {
            lista_de_pontos->inicio = pontos;
            lista_de_pontos->final = pontos;
            pontos->anterior = pontos; // aponta para si mesmo
            pontos->proximo = pontos;  // aponta para si mesmo
        }
        else
        { // fila não está vazia

            lista_de_pontos->final->proximo = pontos;
            pontos->anterior = lista_de_pontos->final;
            pontos->proximo = lista_de_pontos->inicio; // aponta para o início para formar a circularidade
            lista_de_pontos->final = pontos;
        }
    }
}

void inicializar_Lista_Circular()
{

    lista_de_pontos = calloc(numero_de_pontos, sizeof(Lista_circular_pontos));
    lista_de_pontos->inicio = NULL;
    lista_de_pontos->final = NULL;
    criar_pontos();
}

