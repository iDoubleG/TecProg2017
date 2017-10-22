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
#include <time.h>   /* necessário para garantir aleatoriedade */
#include <math.h>   /*      a distribuicao dos terrenos       */
#include "maq.h"
#include "arena.h"

#define GRID_TAM 10
#define EXERCITO 500
#define TIMESTEP 50
#define N_TIMES 2
#define N_CRISTAIS 10

// macros para pegar uma celula da arena
#define arenaG arena.grid[i][j]                  
#define arenaGR1 arena.grid[random11][random12]
#define arenaGR2 arena.grid[random21][random22]

typedef struct {
	Celula grid[GRID_TAM*2][GRID_TAM];
	int ExercitosAtivos[EXERCITO];
	int tempoCorrido;
} Arena;


void InsereArena() {

	int random11, random12; // indices aleatorios para aleatoriedade das bases
	int random21, random22; // indices aleatorios para aleatoriedade dos cristais
	int timesBases;
	int cristaisBase;

	Arena arena;

	srand(time(NULL));

	// Criacao da arena
	for (int i = 0; i < (GRID_TAM * 2); i++) {
		for (int j = 0; j < GRID_TAM; j++) {
			// designa "NADA" as casas da matriz que nao serao utilizadas (por ser hexagonal)
      		if ((i % 2 == 0 && j % 2 != 0) || (j % 2 == 0 && i % 2 != 0))
      			{arenaG = NADA; arenaG.pos = {i, j};}
      		// designa "ARMADILHA", "AGUA", "AREIA", "ESTRADA" ou "RUELA" aleatoriamente
      		// as casas utilizaveis da matriz com probabilidade predefinida
      		else {
				int random1 = rand() % 100;
				if (random1 < 5) {arenaG.terreno = ARMADILHA; arenaG.pos = {i, j};}
				else if (random1 < 25) {arenaG.terreno = AGUA; arenaG.pos = {i, j};}
				else if (random1 < 45) {arenaG.terreno = AREIA; arenaG.pos = {i, j};}
				else if (random1 < 55) {arenaG.terreno = ESTRADA; arenaG.pos = {i, j};}
				else {arenaG.terreno = RUELA; arenaG.pos = {i, j};}
      		}
   		}
	}

	timesBases = N_TIMES;

	// designa as bases aleatoriamente a matriz
	while (timesBases > 0) {
		random11 = rand() % (GRID_TAM - 2) + 1; // indices
		random12 = rand() % (tamanhoY - 2) + 1; // aleatorios

		if (arenaGR1.terreno != NADA && arenaGR1.terreno != BASE) {
			arenaGR1.terreno = BASE;
			timesBases--;
		}
	}

	cristaisBase = N_CRISTAIS;

	// designa os cristais aleatoriamente a matriz
	while (cristaisBase > 0) {
		random21 = rand() % (GRID_TAM - 2) + 1; //indices
		random22 = rand() % (tamanhoY - 2) + 1; //aleatorios

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
			exec_maquina(tropas[i][j], TIMESTEP);//timestep = 50
}

Celula buscaCel (int i, int j){
	return grid[i][j];
}

int Sistema(int op, Maquina *m) {
//Os switch de j que vão de 0 a 5 representam as direções das ações, em sentido horário, começando por cima
	int i = op/10;
//Primeiro algarismo indica o tipo de ação
	int j = op % 10;
//Segundo algarismo indica a direção da ação
	Celula cel;
	int aux1;
	int aux2;
//Variaveis de suporte
	switch(i){
		case 0://Mover. Retorna 1 se não há ninguém na célula
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
			case 0:
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado == 0) return 1;
			else return 0;
			case 1:
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado == 0) return 1;
			else return 0;
			case 2:
			cel = buscaCel(aux1+1, aux2+1);
			if(cel.ocupado == 0) return 1;
			else return 0;
			case 3:
			cel = buscaCel(aux1+2, aux2);
			if(cel.ocupado == 0) return 1;
			else return 0;
			case 4:
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado == 0) return 1;
			else return 0;
			case 5:
			cel = buscaCel(aux1-1, aux2-1);
			if(cel.ocupado == 0) return 1;	
			else return 0;	
		}
		case 1://Atacar(IMPLEMENTAÇÃO INCOMPLETA)
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
			case 0:
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado > 0) {
				return 1;
			}
			else return 0;
			case 1:
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado > 0) {
				return 1;
			}
			else return 0;
			case 2:
			cel = buscaCel(aux1+1, aux2+1);
			if(cel.ocupado > 0) {
				return 1;
			}
			else return 0;
			case 3:
			cel = buscaCel(aux1+2, aux2);
			if(cel.ocupado > 0) {
				return 1;
			}
			else return 0;
			case 4:
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado > 0) {
				return 1;
			}
			else return 0;
			case 5:
			cel = buscaCel(aux1-1, aux2-1);
			if(cel.ocupado > 0) {
				return 1;
			}
			else return 0;	
		}
		case 2://Recolher. Verifica quantos cristais há na célula e retorna esse valor.
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
			case 0:
			cel = buscaCel(aux1-2, aux2);
			return cel.cristais;
			case 1:
			cel = buscaCel(aux1-1, aux2+1);
			return cel.cristais;
			case 2:
			cel = buscaCel(aux1+1, aux2+1);
			return cel.cristais;
			case 3:
			return cel.cristais;
			case 4:
			cel = buscaCel(aux1+1, aux2-1);
			return cel.cristais;
			case 5:
			cel = buscaCel(aux1-1, aux2-1);
			return cel.cristais;
		}
		case 3://Depositar. Verifica se há alguém na célula e deposita os cristais na máquina se não houver ninguém
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 1:
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 2:
			cel = buscaCel(aux1+1, aux2+1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 3:
			cel = buscaCel(aux1+2, aux2);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 4:
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 5:
			cel = buscaCel(aux1-1, aux2-1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;	
		}
	}
}

int main (int ac, char **av){
	Maquina *tropas[256][EXERCITO];
	for (int i=0; i<nexercitos; i++){
		for (int j=0; j<EXERCITO; j++){
			tropas[i][j] = InsereExercito(diretriz);
		}
	}
	for (int i=0; i<rodadas; i++)
		Atualiza (tropas);
	
}
