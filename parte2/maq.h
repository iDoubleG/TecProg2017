#ifndef MAQ_H
#define MAQ_H
#include "pilha.h"

#define MAXMEM 100

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  int ip;
  int rbp;/*Variavel registrador de base, guarda o endereço da pilha de
			excecucao onde comeca o uso de variaveis locais*/
  int pos[2];/*Posicao x e y da maquina na grid*/
  int ncristais;/*Numero de cristais carregados pela maquina*/
  int time;/*A que time a maquina pertence*/
  int patente;/*Qual a posicao dentro do time a qual a maquina pertence*/
  int vida;/*Contador de vida da Maquina, inicialmente com 1 de vida*/
  int matou;/*Contador de abates da Maquina*/
} Maquina;

Maquina *cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);
#endif