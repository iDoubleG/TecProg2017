/*
1 = normal
2 = caminho
3 = areia
4 = agua
5 = bomba
6 = base
flag -lm (math.h precisa)
hikaru é meu amigo
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "maq.h"
#include "arena.h"

#define GRID_TAM 10
#define EXERCITO 500
#define TIMESTEP 50
#define N_TIMES 2
#define N_CRISTAIS 10
#define arenaG arena.grid[i][j]
#define arenaGR1 arena.grid[random11][random12]
#define arenaGR2 arena.grid[random21][random22]

typedef struct {
	Celula grid[GRID_TAM*2][GRID_TAM];
	int ExercitosAtivos[EXERCITO];
	int tempoCorrido;
} Arena;


void InsereArena() {

	int random11, random12;
	int random21, random22;
	int timesBases;
	int cristaisBase;

	Arena arena;

	srand(time(NULL));

	// Criacao da arena
	for (int i = 0; i < (GRID_TAM * 2); i++) {
		for (int j = 0; j < GRID_TAM; j++) {
      		if ((i % 2 == 0 && j % 2 != 0) || (j % 2 == 0 && i % 2 != 0))
      			arenaG = NADA;
      		else {
				int random1 = rand() % 100;
				if (random1 < 5) {arenaG.terreno = ARMADILHA; arenaG.posi = {i, j};}
				else if (random1 < 25) {arenaG.terreno = AGUA; arenaG.posi = {i, j};}
				else if (random1 < 45) {arenaG.terreno = AREIA; arenaG.posi = {i, j};}
				else if (random1 < 55) {arenaG.terreno = ESTRADA; arenaG.posi = {i, j};}
				else {arenaG.terreno = RUELA; arenaG.posi = {i, j};}
      		}
   		}
	}

	timesBases = N_TIMES;

	while (timesBases > 0) {
		random11 = rand() % (GRID_TAM - 2) + 1;
		random12 = rand() % (tamanhoY - 2) + 1;

		if (arenaGR1.terreno != NADA && arenaGR1.terreno != BASE) {
			arenaGR1.terreno = BASE;
			timesBases--;
		}
	}

	cristaisBase = N_CRISTAIS;

	while (cristaisBase > 0) {
		random21 = rand() % (GRID_TAM - 2) + 1;
		random22 = rand() % (tamanhoY - 2) + 1;

		if (arenaGR2.terreno != NADA && arenaGR2.terreno != BASE && arenaGR2.cristais == 0) {
			arenaGR2.cristais = 1;
			cristaisBase--;
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