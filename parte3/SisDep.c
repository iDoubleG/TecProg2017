#include <stdio.h>
#include <stdlib.h>
#include "chamSis.h"
#include "arena.h"
#define TAM_GRADE 10

OPERANDO SisDep(Maquina *m){ //modularização de Sistema, Depositar
	OPERANDO valor_de_retorno;
	int aux1;
	int aux2;
	Celula cel;
	aux1 = m->pos[0];
	aux2 = m->pos[1];
	cel = buscaCel(aux1, aux2);
	cel.cristais += m->ncristais;
	valor_de_retorno.val.n = 1;
	valor_de_retorno.t = NUM;
	return valor_de_retorno;
}
