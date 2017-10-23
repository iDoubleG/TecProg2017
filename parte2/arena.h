#ifndef ARENA_H
#define ARENA_H
#include "instr.h"

void InsereArena();
void InsereExercito(INSTR *p, int n);
void RemoveExercito(Maquina *m);
void Atualiza();
Celula buscaCel(int i, int j);
Maquina *buscaMaq(int patente, int i, int j);
int Sistema(int o, Maquina *m);
#endif