#include "instr.h"

/* Célula */
typedef struct {
  Terreno terreno;
  short int cristais;
  short int ocupado;
  int pos[2];
  int base;//numero vai ser o time; 0 = nao tem time
} Celula;


void InsereArena();
void InsereExercito(INSTR *p, int n);
void RemoveExercito(Maquina *m);
void Atualiza();
Celula buscaCel(int i, int j);
Maquina *buscaMaq(int patente, int i, int j);
int Sistema(int o, Maquina *m);
