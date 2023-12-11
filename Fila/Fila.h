#ifndef FILA_H
#define FILA_H
#define FILA_VAZIA -1
#define ELEMENTO_NAO_ENCONTRADO_FILA -1
#define ELEMENTO_PRESENTE_FILA 1

typedef struct no
{ /* criação de um no de uma fila ou seja um elemento , que ira possuir um ponteiro para o proximo
     elemento e um para o elemento anterior.*/

    struct no *prox;
    struct no *anterior;
    int index_do_passageiro; // variável responsável por identificar qual o passageiro que está na fila

} tipoNO;

typedef struct fila
{ // criaçao de uma struct fila que é um ponteiro para o primeiro e último elemento da fila
    tipoNO *inicio;
    tipoNO *final;

} tipoFILA;

tipoFILA *criar_fila();
tipoNO *criar_no_fila(int index);
void queue(tipoFILA *fila, int index);
int dequeue(tipoFILA *fila);
void liberar_fila(tipoFILA **fila);

#endif