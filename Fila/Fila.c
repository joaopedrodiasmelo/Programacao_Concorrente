#include "Fila.h"
#include <stdio.h>
#include <stdlib.h>

tipoFILA *criar_fila()
{ // cria um tipo fila ou seja o ponteiro para a fila ,colocando o início e o fim como NULL
    tipoFILA *fila = calloc(1, sizeof(tipoFILA));
    fila->final = NULL;
    fila->inicio = NULL;

    return fila;
}

tipoNO *criar_no_fila(int index)
{ // cria um no da fila ,atribuindo o seu valor e colocando seus ponterios como NULL
    tipoNO *no = calloc(1, sizeof(tipoNO));
    no->prox = NULL;
    no->anterior = NULL;
    no->index_do_passageiro = index;

    return no;
}

void queue(tipoFILA *fila, int index)
{ /*essa função ira enserir um elemento na ultima posição da fila*/
    tipoNO *no = criar_no_fila(index);

    if (fila->inicio == NULL && fila->final == NULL)
    { // fila está vazia
        fila->inicio = no;
        fila->final = no;
    }

    else
    { // fila não está vazia
        fila->final->prox = no;
        no->anterior = fila->final;
        fila->final = no;
    }
}
/*essa função ira retornar o indice do passageiro do início da fila e após esse retorno irá remover tal elemento da fila*/
int dequeue(tipoFILA *fila)
{
    tipoNO *auxiliar;
    int indice;

    if (fila->inicio != NULL && fila->final != NULL) // verifica se a fila não esta vazia
    {
        if (fila->inicio == fila->final)
        { // fila com um unico elemento
            auxiliar = fila->inicio;
            indice = auxiliar->index_do_passageiro;
            fila->inicio = NULL;
            fila->final = NULL;

            return indice;
        }

        else
        { // fila com mais de um elemento

            auxiliar = fila->inicio;
            indice = auxiliar->index_do_passageiro;
            fila->inicio = auxiliar->prox;
            auxiliar->prox->anterior = NULL;

            return indice;
        }
    }
}

void liberar_fila(tipoFILA **fila)
{ // função que libera a fila criada dinâmicamente

    tipoFILA *auxiliar = *fila;

    tipoNO *auxiliar2 = auxiliar->inicio;
    tipoNO *auxiliar3 = NULL;

    while (auxiliar2 != NULL)
    {
        auxiliar3 = auxiliar2;
        auxiliar2 = auxiliar2->prox;
        free(auxiliar3);
    }

    free(auxiliar);

    *fila = NULL;
}