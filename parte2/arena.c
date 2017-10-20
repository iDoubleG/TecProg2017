#include <stdio.h>
#include <stdlib.h>
#include "maq.h"

#define EXERCITO 500
#define TIMESTEP 50

//arena.h -> criar uma array de ponteiros para os exercitos

Maquina *InsereExercito (INSTR diretriz) {
	Maquina *tropa = cria_maquina(diretriz);
	tropa->time = i;
	tropa->posicao = j;
	return tropa;
}

void RemoveExercito (Maquina *derrotado) {//poupa trabalho, enviar somente a tropa a ser eliminada
	destroi_maquina(derrotado);
}

void Atualiza (Maquina *tropas[], int rodadas, int nexercitos) {
	for (int i=0; i<EXERCITO; i++)
		for (int j=0; j<nexercitos; j++)
			exec_maquina(tropas[i][j], TIMESTEP);//timestep = 1
}

int main (int ac, char **av){
	Maquina *tropas[nexercitos][EXERCITO];
	for (int i=0; i<nexercitos; i++){
		for (int j=0; j<EXERCITO; j++){
			tropas[i][j]InsereExercito(diretriz);
		}
	}
}