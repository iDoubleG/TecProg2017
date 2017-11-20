#include <stdio.h>
#include <stdlib.h>
#include "chamSis.h"
#include "arena.h"
#define TAM_GRADE 10


OPERANDO SisAtq(int i, Maquina *m){ //modularização de Sistema, Ataque
	OPERANDO valor_de_retorno;
	int aux1;
	int aux2;
	Celula cel;
	aux1 = m->pos[0];
	aux2 = m->pos[1];
	switch(i){
		case 0:
		if (aux1 < 2){
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}  	 //borda de cima
		cel = buscaCel(aux1-2, aux2);
		if(cel.ocupado > 0) {
			destroi_maquina(buscaMaq(m->patente, aux1-2, aux2));
			valor_de_retorno.val.n = 1;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		else {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		case 1:
		if (aux1 == 0 || aux2 == TAM_GRADE) {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}  	 //borda de cima e direita
		cel = buscaCel(aux1-1, aux2+1);
		if(cel.ocupado > 0) {
			destroi_maquina(buscaMaq(m->patente, aux1-1, aux2+1));
			valor_de_retorno.val.n = 1;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		else {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		case 2:
		if (aux1 == 2*TAM_GRADE || aux2 == TAM_GRADE) {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}  	 //borda de baixo e direita
		cel = buscaCel(aux1+1, aux2+1);
		if(cel.ocupado > 0) {
			destroi_maquina(buscaMaq(m->patente, aux1+1, aux2+1));
			valor_de_retorno.val.n = 1;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		else {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		case 3:
		if (aux1 > (2*TAM_GRADE - 2)) {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}  	 //borda de baixo
		cel = buscaCel(aux1+2, aux2);
		if(cel.ocupado > 0) {
			destroi_maquina(buscaMaq(m->patente, aux1+2, aux2));
			valor_de_retorno.val.n = 1;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		else {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		case 4:
		if (aux1 == 2*TAM_GRADE || aux2 == 0) {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}  	 //borda de baixo e esquerda
		cel = buscaCel(aux1+1, aux2-1);
		if(cel.ocupado > 0) {
			destroi_maquina(buscaMaq(m->patente, aux1+1, aux2-1));
			valor_de_retorno.val.n = 1;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		else {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		case 5:
		if (aux1 == 0 || aux2 == 0) {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}  	 //borda de cima e esquerda
		cel = buscaCel(aux1-1, aux2-1);
		if(cel.ocupado > 0) {
			destroi_maquina(buscaMaq(m->patente, aux1-1, aux2-1));
			valor_de_retorno.val.n = 1;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
		else {
			valor_de_retorno.val.n = 0;
			valor_de_retorno.t = NUM;
			return valor_de_retorno;
		}
	}
}
