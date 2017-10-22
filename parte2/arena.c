/*
1 = normal
2 = caminho
3 = areia
4 = agua
5 = bomba
6 = base
flag -lm (math.h precisa)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "arena.h"
#define GRID_TAM 1000
#define EXERCITO 500
#define TIMESTEP 50
#define arenaG arena.grid[i][j].terreno
#define arenaGR arena.grid[random11][random12].terreno

typedef struct {
	Celula grid[GRID_TAM*2][GRID_TAM];
	int ExercitosAtivos[EXERCITO];
} Arena;


void InsereArena() {

	int times = 25;
	int timesBases;
	int random11, random12;
	int comparador;

	Arena arena;

	srand(time(NULL));

	// Criacao da arena
	for (int i = 0; i < (GRID_TAM * 2); i++) {
		for (int j = 0; j < GRID_TAM; j++) {
      		if ((i % 2 == 0 && j % 2 != 0) || (j % 2 == 0 && i % 2 != 0)) {
      			arenaG = NADA;
      		}
      		else {
				int random1 = rand() % 100;
				if (random1 < 5) {arenaG.terreno = ARMADILHA; arenaT.posi = {i, j};}
				else if (random1 < 25) {arenaG.terreno = AGUA; arenaT.posi = {i, j};}
				else if (random1 < 45) {arenaG.terreno = AREIA; arenaT.posi = {i, j};}
				else if (random1 < 55) {arenaG.terreno = ESTRADA; arenaT.posi = {i, j};}
				else {arenaG.terreno = RUELA; arenaT.posi = {i, j};}
      		}
   		}
	}

	timesBases = times;

	while (timesBases > 0) {
		random11 = rand() % (GRID_TAM - 2) + 1;
		random12 = rand() % (tamanhoY - 2) + 1;

		if (arenaGR.terreno != NADA && arenaGR.terreno != BASE) {
			arenaGR.terreno = BASE;
			timesBases--;
		}
	}
}

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