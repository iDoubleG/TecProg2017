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
#include <time.h>   /* necessário para garantir aleatoriedade */
#include <math.h>   /*      a distribuicao dos terrenos       */
#include "maq.h"
#include "arena.h"

#define GRID_TAM 10
#define EXERCITO 500
#define TROPAS_POR_EXERCITO 500
#define TIMESTEP 50
#define N_TIMES 2
#define N_CRISTAIS 10

// macros para pegar uma celula da arena
#define arenaG arena.grid[i][j]                  
#define arenaGR1 arena.grid[random11][random12]
#define arenaGR2 arena.grid[random21][random22]

typedef struct {
	Maquina exercitosAtivos[256][TROPAS_POR_EXERCITO];
	int tempoCorrido;
	int contadorExercitos;
	Celula grid[GRID_TAM*2][GRID_TAM];
} Arena;

Arena arena;

void InsereArena() {

	int random21, random22; // indices aleatorios para aleatoriedade dos cristais
	int cristaisBase;


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

void InsereExercito (INSTR diretriz, int equipe) { //chamar a função com a número arena.contadorExercitos no lugar da int equipe
	
	int random11, random12;
  while(true) {
	  // designa as bases aleatoriamente a matriz
	  random11 = rand() % (GRID_TAM - 2) + 1; // indices
	  random12 = rand() % (tamanhoY - 2) + 1; // aleatorios

	  if (arenaGR1.terreno != NADA && arenaGR1.terreno != BASE) {
	    arenaGR1.terreno = BASE;
		  break;
	  }
  }

	for(int i = 0; i < TROPAS_POR_EXERCITO; i++){//cria máquinas e guarda cada uma em um lugar da lista de exercitos ativos
    Maquina *tropa = cria_maquina(diretriz);
    arena.exercitosAtivos[equipe][i] = tropa;
    tropa->time = equipe;
    tropa->pos[0] = random11;
    tropa->pos[1] = random12;
    tropa->patente = i;
	}
	arenaGR1.ocupado = 1;
}

void RemoveExercito (Maquina *derrotado) {//poupa trabalho, enviar somente a tropa a ser eliminada
	destroi_maquina(derrotado);
}

void Atualiza (Maquina *tropas[256][], int nexercitos) {
	for (int i=0; i<EXERCITO*nexercitos; i++)
		for (int j=0; j<nexercitos; j++)
			exec_maquina(tropas[i][j], TIMESTEP);//timestep = 50
	tempoCorrido++;
}

Celula buscaCel (int i, int j){
	return grid[i][j];
}

Maquina *buscaMaq (int patente, int i, int j){
  int pos1[2];
  pos1[0] = i;
  pos1[1] = j;
  for(int k = 0; k < N_TIMES; k++){
    if((arena.exercitosAtivos[k][patente])->pos == pos1){
      return arena.exercitosAtivos[k][patente];
    }
  }
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
			if (aux1 < 2) return 0; //borda de cima
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return 1;
			}
			else return 0;
			case 1:
			if (aux1 == 0 || aux2 == GRID_TAM) return 0; //borda de cima e direita
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return 1;
			}
			else return 0;
			case 2:
			if (aux1 == 2*GRID_TAM || aux2 == GRID_TAM) return 0; //borda de baixo e direita
			cel = buscaCel(aux1+1, aux2+1);
	  	if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return 1;
			}
			else return 0;
			case 3:
			if (aux1 > (2*GRID_TAM - 2)) return 0; //borda de baixo
			cel = buscaCel(aux1+2, aux2);
	  	if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return 1;
			}
			else return 0;
			case 4:
			if (aux1 == 2*GRID_TAM || aux2 == 0) return 0; //borda de baixo e esquerda
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return 1;
			}
			else return 0;
			case 5:
			if (aux1 == 0 || aux2 == 0) return 0; //borda de cima e esquerda
			cel = buscaCel(aux1-1, aux2-1);
			if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return 1;
			}
			else return 0;	
		}
		
		case 1://Atacar
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
			case 0:
			if (aux1 < 2) return 0; //borda de cima
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(aux1-2, aux2));
				return 1;
			}
			else return 0;
			case 1:
			if (aux1 == 0 || aux2 == GRID_TAM) return 0; //borda de cima e direita
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(aux1-1, aux2+1));
				return 1;
			}
			else return 0;
			case 2:
			if (aux1 == 2*GRID_TAM || aux2 == GRID_TAM) return 0; //borda de baixo e direita
			cel = buscaCel(aux1+1, aux2+1);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(aux1+1, aux2+1));
				return 1;
			}
			else return 0;
			case 3:
			if (aux1 > (2*GRID_TAM - 2)) return 0; //borda de baixo
			cel = buscaCel(aux1+2, aux2);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(aux1+2, aux2));
				return 1;
			}
			else return 0;
			case 4:
			if (aux1 == 2*GRID_TAM || aux2 == 0) return 0; //borda de baixo e esquerda
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(aux1+1, aux2-1));
				return 1;
			}
			else return 0;
			case 5:
			if (aux1 == 0 || aux2 == 0) return 0; //borda de cima e esquerda
			cel = buscaCel(aux1-1, aux2-1);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(aux1-1, aux2-1));
				return 1;
			}
			else return 0;	
		}
		
		case 2://Recolher. Verifica quantos cristais há na célula e retorna esse valor.
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
			case 0:
			if (aux1 < 2) return 0; //borda de cima
			cel = buscaCel(aux1-2, aux2);
			return cel.cristais;
			case 1:
			if (aux1 == 0 || aux2 == GRID_TAM) return 0; //borda de cima e direita
			cel = buscaCel(aux1-1, aux2+1);
			return cel.cristais;
			case 2:
			if (aux1 == 2*GRID_TAM || aux2 == GRID_TAM) return 0; //borda de baixo e direita
			cel = buscaCel(aux1+1, aux2+1);
			return cel.cristais;
			case 3:
			if (aux1 > (2*GRID_TAM - 2)) return 0; //borda de baixo
			cel = buscaCel(aux1+2, aux2);
			return cel.cristais;
			case 4:
			if (aux1 == 2*GRID_TAM || aux2 == 0) return 0; //borda de baixo e esquerda
			cel = buscaCel(aux1+1, aux2-1);
			return cel.cristais;
			case 5:
			if (aux1 == 0 || aux2 == 0) return 0; //borda de cima e esquerda
			cel = buscaCel(aux1-1, aux2-1);
			return cel.cristais;
		}
		
		case 3://Depositar. Verifica se há alguém na célula e deposita os cristais na máquina se não houver ninguém
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
		  if (aux1 < 2) return 0; //borda de cima
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 1:
			if (aux1 == 0 || aux2 == GRID_TAM) return 0; //borda de cima e direita
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 2:
			if (aux1 == 2*GRID_TAM || aux2 == GRID_TAM) return 0; //borda de baixo e direita
			cel = buscaCel(aux1+1, aux2+1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 3:
			if (aux1 > (2*GRID_TAM - 2)) return 0; //borda de baixo
			cel = buscaCel(aux1+2, aux2);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 4:
			if (aux1 == 2*GRID_TAM || aux2 == 0) return 0; //borda de baixo e esquerda
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return 1;
			}
			else return 0;
			case 5:
			if (aux1 == 0 || aux2 == 0) return 0; //borda de cima e esquerda
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
  InsereArena();
	for (int i=0; i<N_TIMES; i++){
		InsereExercito(diretriz, i);
	}
	for (int i=0; i<rodadas; i++)
		Atualiza (tropas);
	
}