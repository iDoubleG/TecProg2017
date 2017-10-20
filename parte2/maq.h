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
  int pos[2];//this new
  int ncristais;//this new
  int time;//this new
  int patente;//this new
  int vida;//this new
  int matou;//this new
} Maquina;

Maquina *cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);
