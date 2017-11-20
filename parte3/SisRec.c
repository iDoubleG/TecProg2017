#include <stdio.h>
#include <stdlib.h>
#include "chamSis.h"
#include "arena.h"
#define TAM_GRADE 10

OPERANDO SisRec(Maquina *m){ //modularização de Sistema, Recolher
	OPERANDO valor_de_retorno;
	int aux1;
	int aux2;
	Celula cel;
	aux1 = m->pos[0];
	aux2 = m->pos[1];
	cel = buscaCel(aux1, aux2);
	valor_de_retorno.val.n = cel.cristais;
	valor_de_retorno.t = NUM;
	return valor_de_retorno;
}
