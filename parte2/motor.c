#include <stdio.h>
#include "maq.h"

INSTR programa[] = {
  {PUSH, 3},
  {PUSH, 6},
  {CALL, 5},
  {PRN,  0},
  {END,  0},
  {ADD,  0},
  {RET,  0}
};

int main(int ac, char **av) {
  Maquina *maq = cria_maquina(programa);
  Maquina *maq2 = cria_maquina(fibonacci);
  Maquina *maq3 = cria_maquina(fat);
  exec_maquina(maq, 3);
  exec_maquina(maq2, 40);
  puts("---");
  exec_maquina(maq, 10);
  puts("---");
  exec_maquina(maq2, 10000);
  puts("-----");
  exec_maquina(maq3, 1000);
  puts("-----");
  destroi_maquina(maq);
  destroi_maquina(maq2);
  destroi_maquina(maq3);
  return 0;
}
