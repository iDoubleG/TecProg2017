#include "instr.h"

/* Célula */
typedef struct {
  Terreno terreno;
  short int cristais;
  short int ocupado;
  int pos[2];
  int base;//numero vai ser o time; 0 = nao tem time
} Celula;