#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../Conjunto_de_Structs/Structs.h"

/*
  Função responsável por gerar a posição de partida inicial de cada onibus
*/
void gerar_posicao(int indice_onibus)
{

  // Gera o primeiro número aleatório no intervalo especificado
  conjunto_de_onibus[indice_onibus].ponto = rand() % ((numero_de_pontos - 1) - 0 + 1) + 0;
}

/*
  Função que coloca os pontos do seu ponto de partida  gerado aleatoriamente
*/
void *inicializar_onibus_pontos(void *args)
{
  int indice = (int)*((int *)args);

  gerar_posicao(indice);

  Ponto_de_Onibus *auxiliar = lista_de_pontos->inicio;

  while (1)
  {
    if (conjunto_de_onibus[indice].ponto == auxiliar->identificador_ponto)
    {
      pthread_mutex_lock(&auxiliar->mutex_do_ponto);
      conjunto_de_onibus[indice].ponteiro_para_ponto = auxiliar; // aponta para o ponto atual
      printf("Onibus %d foi inicializado no ponto %d\n", conjunto_de_onibus[indice].identificador_onibus, conjunto_de_onibus[indice].ponteiro_para_ponto->identificador_ponto);
      pthread_mutex_unlock(&auxiliar->mutex_do_ponto);
      break;
    }
    auxiliar = auxiliar->proximo;
  }
}

void *movimentar_onibus(void *args)
{
  int indice = (int)*((int *)args);

  gerar_posicao(indice);

  while (1)
  {

    // pthread_mutex_lock(&mutex_auxiliar);

    if (pthread_mutex_trylock(&conjunto_de_onibus[indice].ponteiro_para_ponto->mutex_do_ponto) == 0)
    { // significa que o onibus conseguiu o mutex do ponto em questão

      // coloca na estrutura ponto o onibus que está presente no momento
      conjunto_de_onibus[indice].ponteiro_para_ponto->identificador_ponto = conjunto_de_onibus[indice].identificador_onibus;

      pthread_cond_broadcast(&conjunto_de_onibus[indice].chegou_onibus);

      printf("Onibus %d chegou no ponto %d\n", conjunto_de_onibus[indice].identificador_onibus, conjunto_de_onibus[indice].ponteiro_para_ponto->identificador_ponto);

      while (conjunto_de_onibus[indice].capacidade > 0 && conjunto_de_onibus[indice].ponteiro_para_ponto->fila_presente_no_ponto != NULL)
      {
        // pega o indice do passageiro que está na primeira posição do ponto
        int indice_passageiro = dequeue(conjunto_de_onibus[indice].ponteiro_para_ponto->fila_presente_no_ponto);
        passageiros_no_onibus[indice_passageiro].onibus_de_entrada = conjunto_de_onibus[indice].identificador_onibus;
        pthread_cond_signal(&passageiros_no_onibus[indice_passageiro].entrou_onibus); // sinaliza para o passageiro que o onibus chegou
        printf("Passageiro %d entrou no onibus %d\n", passageiros_no_onibus[indice_passageiro].Identificador_Passageiro, conjunto_de_onibus[indice].identificador_onibus);
        conjunto_de_onibus[indice].capacidade--;
      }

      pthread_mutex_unlock(&conjunto_de_onibus[indice].ponteiro_para_ponto->mutex_do_ponto);
    }
    // caso no qual o onibus deve prosseguir para o próximo ponto

    conjunto_de_onibus[indice].ponteiro_para_ponto = conjunto_de_onibus[indice].ponteiro_para_ponto->proximo;

    // Tempo variável entre pontos (por exemplo, entre 1 e 3 segundos)
    unsigned int tempo_variavel = 1000000 * (1 + rand() % 3); // Em microssegundos
    usleep(tempo_variavel);

    // significa que não existem mais passageiros em nenhum ponto da lista
    if (passageiros_que_sobraram == 0)
    {
      // pthread_mutex_unlock(&mutex_auxiliar);
      break;
    }
  }
  pthread_exit(0);
}

/*
   Função responsável por inicializar cada um dos onibus nos seus pontos de partida e realizar a movimentação
   dos onibus em torno dos pontos presentes na lista circular
 */

void *criar_onibus(void *args)
{
  inicializar_onibus_pontos(args);
  movimentar_onibus(args);
}

void inicializar_onibus(int quantidade_onibus, int quantidade_assentos)
{
  conjunto_de_onibus = calloc(quantidade_onibus, sizeof(onibus_circulantes));
  for (int a = 0; a < quantidade_onibus; a++)
  {

    conjunto_de_onibus[a].ponto = -1;
    conjunto_de_onibus[a].capacidade = quantidade_assentos;
    conjunto_de_onibus[a].identificador_onibus = a;
    conjunto_de_onibus[a].ponteiro_para_ponto = NULL;
    pthread_cond_init(&conjunto_de_onibus[a].chegou_onibus, NULL);
    pthread_cond_init(&conjunto_de_onibus[a].liberar_onibus, NULL);
  }
}