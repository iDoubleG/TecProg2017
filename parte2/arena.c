#include <stdio.h>
#include <stdlib.h>
#include "maq.h"
#include "arena.h"

#define GRID_TAM 1000
#define EXERCITO 500
#define TIMESTEP 50


typedef struct {
	int grid[GRID_TAM*2][GRID_TAM];
	int ExercitosAtivos[EXERCITO];
} Arena;

Maquina *InsereExercito (INSTR diretriz) {
	Maquina *tropa = cria_maquina(diretriz);
	tropa->time = i;
	tropa->posicao = j;
	return tropa;
}

void RemoveExercito (Maquina *derrotado) {//poupa trabalho, enviar somente a tropa a ser eliminada
	destroi_maquina(derrotado);
}

void Atualiza (Maquina *tropas[256][], int nexercitos) {
	for (int i=0; i<EXERCITO*nexercitos; i++)
		for (int j=0; j<nexercitos; j++)
			exec_maquina(tropas[i][j], TIMESTEP);//timestep = 1
}

int main (int ac, char **av){
	Maquina *tropas[256][EXERCITO];
	for (int i=0; i<nexercitos; i++){
		for (int j=0; j<EXERCITO; j++){
			tropas[i][j]InsereExercito(diretriz);
		}
	}
	for (int i=0; i<rodadas; i++)
		Atualiza (tropas);
	
}