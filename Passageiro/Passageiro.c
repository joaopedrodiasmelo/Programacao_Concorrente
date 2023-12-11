#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Conjunto_de_Structs/Structs.h"

void gerar_pontos(int *numero1, int *numero2, int intervaloInicial, int intervaloFinal)
{
  // Semente para a função rand() - geralmente é definida com o tempo atual
  srand((unsigned int)time(NULL));

  // Gera o primeiro número aleatório no intervalo especificado
  *numero1 = rand() % (intervaloFinal - intervaloInicial + 1) + intervaloInicial;

  // Gera o segundo número aleatório no intervalo especificado
  *numero2 = rand() % (intervaloFinal - intervaloInicial + 1) + intervaloInicial;
}

/*
   Função responsável por encontrar o ponto desejado na lista circular e inserir esse passageiro
   nesse ponto
*/
void pesquisar_posicao_lista(int ponto_desejado, int index)
{

  Ponto_de_Onibus *auxiliar = lista_de_pontos->inicio; // variável auxiliar que irá apontar para o primeiro elemento da lista

  while (1)
  {
    if (ponto_desejado == auxiliar->identificador_ponto)
    {
      queue(auxiliar->fila_presente_no_ponto, index); // insere o passageiro na fila do ponto desejado
      break;
    }

    auxiliar = auxiliar->proximo; // percorre a lista até encontrar o ponto em que o passageiro começa
  }
}
/*
   FUnção responsável por preencher os pontos com os passageiros
*/
void preencher_pontos(int index)
{
  int ponto_de_inicio = passageiros_no_onibus[index].Ponto_Partida;
  pesquisar_posicao_lista(ponto_de_inicio, index);
}

/*
  Função responsável por inicializar o vetor_de_passageiros com os seus dados
*/
void inicializar_passageiros(int P, int S)
{
  passageiros_no_onibus = calloc(P, sizeof(Passageiro));

  for (int a = 0; a < P; a++)
  {
    passageiros_no_onibus[a].Identificador_Passageiro = a;
    passageiros_no_onibus[a].onibus_de_entrada = -1;
    gerar_pontos(&passageiros_no_onibus[a].Ponto_Partida, &passageiros_no_onibus[a].Ponto_Chegada, 0, S - 1);
    pthread_cond_init(&passageiros_no_onibus[a].entrou_onibus, NULL);
    preencher_pontos(passageiros_no_onibus[a].Identificador_Passageiro);
  }
}
/*
  função responsável por alocar cada passageiro na fila do ponto de partidada do seu ponto de partidada
  e gerar uma thread que acompanha o percurso do onibus.
  Quando essa thread se encerrar vai ser gerado o arquivo de rastro do passageiro indicando o tempo de viagem, seu
  id e o seus pontos de partida e chegada
 */
void *thread_passageiro(void *args)
{
  int indice = (int)*((int *)args); // indice do passageiro

  pthread_mutex_lock(&mutex_auxiliar);
  //  printf("passageiro %d foi criado\n", passageiros_no_onibus[indice].Identificador_Passageiro);

  // registra o momento em que o passageiro chega no ponto
  struct timespec chegada_no_ponto;
  clock_gettime(CLOCK_MONOTONIC, &chegada_no_ponto);

  // coloco a thread passageiro para dormir até ela entrar em um onibus
  pthread_cond_wait(&passageiros_no_onibus[indice].entrou_onibus, &mutex_auxiliar);
  

  // registra o tempo de entrada do passageiro no onibus
  struct timespec entrada_onibus;
  clock_gettime(CLOCK_MONOTONIC, &entrada_onibus);

  // indica que o passageiro entrou em um onibus e vai esperar nesse ponto até descer
  while (conjunto_de_onibus[passageiros_no_onibus[indice].onibus_de_entrada].ponteiro_para_ponto->identificador_ponto != passageiros_no_onibus[indice].Ponto_Chegada)
  {
    // coloca a thread passageiro para dormir enquanto o onibus não chega em outro ponto
    pthread_cond_wait(&conjunto_de_onibus[passageiros_no_onibus[indice].onibus_de_entrada].chegou_onibus, &mutex_auxiliar);
  }

  // Registra o tempo de saída do ônibus
  struct timespec saida_onibus;
  clock_gettime(CLOCK_MONOTONIC, &saida_onibus);

  /*
  indica que o passageiro chegou no seu ponto de decida e desceu do onibus
  */

  conjunto_de_onibus[passageiros_no_onibus[indice].onibus_de_entrada].capacidade++;
  printf("Passageiro %d desceu do onibus %d e desceu no ponto %d\n", passageiros_no_onibus[indice].Identificador_Passageiro, conjunto_de_onibus[passageiros_no_onibus[indice].onibus_de_entrada].identificador_onibus, passageiros_no_onibus[indice].Ponto_Partida);
  passageiros_que_sobraram--;

  // salva os dados do passageiro do arquivo de rastro
  char nome_arquivo[50];
  sprintf(nome_arquivo, "passageiro%d.trace", passageiros_no_onibus[indice].Identificador_Passageiro);

  FILE *arquivo_rastro = fopen(nome_arquivo, "w");
  if (arquivo_rastro == NULL)
  {
    perror("Erro ao abrir o arquivo de rastro");
    pthread_exit(NULL);
  }

  fprintf(arquivo_rastro, "Chegou no ponto: %ld.%09ld\n", chegada_no_ponto.tv_sec, chegada_no_ponto.tv_nsec);
  fprintf(arquivo_rastro, "Entrou no ônibus: %ld.%09ld\n", entrada_onibus.tv_sec, entrada_onibus.tv_nsec);
  fprintf(arquivo_rastro, "Desceu do ônibus: %ld.%09ld\n", saida_onibus.tv_sec, saida_onibus.tv_nsec);

  fclose(arquivo_rastro);

  pthread_mutex_unlock(&mutex_auxiliar);
  pthread_exit(0);
}
