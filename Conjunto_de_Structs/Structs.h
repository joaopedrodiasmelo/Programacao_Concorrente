#ifndef STRUCTS_H
#define STRUCTS_H
#include "../Fila/Fila.h"
#include <pthread.h>
#include <time.h>

/*
   Struct do ponto ------------------------------------------------------------------------------------
*/

typedef struct Ponto
{
  struct Ponto *anterior;
  struct Ponto *proximo;
  tipoFILA *fila_presente_no_ponto;
  int identificador_ponto;
  int onibus_presente;
  pthread_mutex_t mutex_do_ponto;

} Ponto_de_Onibus;

typedef struct Lista_circular
{
  Ponto_de_Onibus *inicio;
  Ponto_de_Onibus *final;

} Lista_circular_pontos;

void inicializar_Lista_Circular();

/*
   Struct do Onibus ------------------------------------------------------------------------------------
*/
typedef struct tipo_onibus
{
  int ponto;
  int capacidade;
  int identificador_onibus;
  Ponto_de_Onibus *ponteiro_para_ponto;
  pthread_cond_t chegou_onibus;
  pthread_cond_t liberar_onibus;
} onibus_circulantes;

/*
   Função responsável por inicializar cada um dos onibus nos seus pontos de partida e realizar a movimentação
   dos onibus em torno dos pontos presentes na lista circular
 */
void *criar_onibus(void *args);

void inicializar_onibus(int quantidade_onibus, int quantidade_assentos);

/*
   Struct do Passageiro ------------------------------------------------------------------------------------
*/

typedef struct Passageiros
{
  int Identificador_Passageiro;
  int onibus_de_entrada;
  int Ponto_Partida;
  int Ponto_Chegada;
  pthread_cond_t entrou_onibus; // variável para auxiliar para verificar se entrou um onibus no ponto onde o passageiro está

} Passageiro;

/*
 Função responsável por inicializar o vetor de passageiros e inserir cada passageiro na fila dos pontos
*/
void inicializar_passageiros(int P, int S);

/*
  função responsável por alocar cada passageiro na fila do ponto de partidada do seu ponto de partidada
  e gerar uma thread que acompanha o percurso do onibus
 */

void *thread_passageiro(void *args);

/*
  Conjunto de variáveis globais definidas na MAIN
*/

extern onibus_circulantes *conjunto_de_onibus;
extern int numero_de_pontos;
extern Lista_circular_pontos *lista_de_pontos; // lista que contém os pontos com as filas de passageiros
extern Passageiro *passageiros_no_onibus;
extern int passageiros_que_sobraram;
extern pthread_mutex_t mutex_auxiliar;

#endif