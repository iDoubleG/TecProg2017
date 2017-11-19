#ifndef ARENA_H
#define ARENA_H

#include "instr.h"
#include "maq.h"
//#define CLOCK 5

void InsereArena();
void InsereExercito(INSTR *diretriz, int equipe);
void RemoveExercito(Maquina *derrotado);
void Atualiza();
Celula buscaCel(int i, int j);
Maquina *buscaMaq(int patente, int i, int j);
OPERANDO Sistema(OPERANDO op, Maquina *m);
#endif