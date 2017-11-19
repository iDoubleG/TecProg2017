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
#include "instr.h"

#define GRID_TAM 10
#define TROPAS_POR_EXERCITO 2
#define CLOCK 5
#define N_TIMES 2
#define N_CRISTAIS 10
#define RODADAS 1

// macros para pegar uma celula da arena
#define arenaG arena.grid[i][j]                  
#define arenaGR1 arena.grid[random11][random12]
#define arenaGR2 arena.grid[random21][random22]

INSTR diretriz[] = {
  /*{SISM, 0},*/
  {SISM, {NUM, 1}},
  {SISM, {NUM, 2}},
  {SISM, {NUM, 3}},
  {SISM, {NUM, 4}},
  {SISM, {NUM, 5}}
};

typedef struct {
	Maquina *exercitosAtivos[256][TROPAS_POR_EXERCITO];
	int tempoCorrido;
	int contadorExercitos;// O que isso faz, Puce??
	Celula grid[GRID_TAM*2][GRID_TAM];
	Celula base[N_TIMES]; // this new
} Arena;

Arena arena;

void InsereArena() {

	int random21, random22; // indices aleatorios para aleatoriedade dos cristais
	int cristaisBase;

	arena.tempoCorrido = 0;

	srand(time(NULL));

	// Criacao da arena
	for (int i = 0; i < (GRID_TAM * 2); i++) {
		for (int j = 0; j < GRID_TAM; j++) {
			// designa "NADA" as casas da matriz que nao serao utilizadas (por ser hexagonal)
      		if ((i % 2 == 0 && j % 2 != 0) || (j % 2 == 0 && i % 2 != 0))
      			{arenaG.terreno = NADA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
      		// designa "ARMADILHA", "AGUA", "AREIA", "ESTRADA" ou "RUELA" aleatoriamente
      		// as casas utilizaveis da matriz com probabilidade predefinida
      		else {
				int random1 = rand() % 100;
				if (random1 < 5) {arenaG.terreno = ARMADILHA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
				else if (random1 < 25) {arenaG.terreno = AGUA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
				else if (random1 < 45) {arenaG.terreno = AREIA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
				else if (random1 < 55) {arenaG.terreno = ESTRADA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
				else {arenaG.terreno = RUELA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
			arena.grid[i][j].cristais = 0; // this new
      		}
   		}
	}

	cristaisBase = N_CRISTAIS;

	// designa os cristais aleatoriamente a matriz
	while (cristaisBase > 0) {
		random21 = rand() % (GRID_TAM*2 - 2) + 1; //indices
		random22 = rand() % (GRID_TAM - 2) + 1; //aleatorios

		if (arenaGR2.terreno != NADA && arenaGR2.terreno != BASE) {// verificar veracidade
			arenaGR2.cristais++; // this changed1
			cristaisBase--;
		}
	}
}

void InsereExercito (INSTR *diretriz, int equipe) { //chamar a função com a número arena.contadorExercitos no lugar da int equipe

	int random11, random12;
	int random21, random22;
    while(1) {
	  // designa as bases aleatoriamente a matriz
	  random11 = rand() % (GRID_TAM*2 - 2) + 1; // indices
	  random12 = rand() % (GRID_TAM - 2) + 1; // aleatorios

	  if (arenaGR1.terreno != NADA && arenaGR1.terreno != BASE) {
	    arenaGR1.terreno = BASE;
	    arenaGR1.time = equipe;// this new
	    arenaGR1.pos[0] = random11;// this new
	    arenaGR1.pos[1] = random12;// this new
	    arena.base[equipe] = arenaGR1;// this new
		  break;
	  }
    }

    int cont = 0;
    int tropas = TROPAS_POR_EXERCITO;
    while (tropas > 0) {
		random21 = rand() % (GRID_TAM*2 - 2) + 1; //indices
		random22 = rand() % (GRID_TAM - 2) + 1; //aleatorios

		if (arenaGR2.terreno != NADA && arenaGR2.time == 0 && arenaGR2.ocupado == 0) {
			for (int i=0; i<5; i++){
				printf("%dhahaha\n", diretriz[i].op.val.n);
			}

			Maquina *tropa = cria_maquina(diretriz);
			for (int i=0; i<5; i++){
				printf("%dNOT FUNNY\n", tropa->prog[i].op.val.n);//soh quero pontuar aqui que isso funciona
			}
		    tropa->time = equipe;
		    tropa->pos[0] = random21;
		    tropa->pos[1] = random22;
		    tropa->patente = cont;
			arenaGR2.ocupado = 1;
			arena.exercitosAtivos[equipe][cont] = tropa;
			cont++;
			tropas--;
		}
	}
	for (int i=0; i<5; i++){
				printf("%dNOT FUNNY\n", arena.exercitosAtivos[equipe][i]->prog[i].op.val.n);//soh quero pontuar aqui que isso funciona
			}
}

void RemoveExercito (Maquina *derrotado) {//poupa trabalho, enviar somente a tropa a ser eliminada
	destroi_maquina(derrotado);
}

void Atualiza () {
	for (int i=0; i<TROPAS_POR_EXERCITO; i++)
		for (int j=0; j<N_TIMES; j++){
			printf("tropa: %d, 	time: %d\n", i, j);
			exec_maquina(arena.exercitosAtivos[i][j], CLOCK);//CLOCK = 50
		}
	arena.tempoCorrido++;
}

Celula buscaCel (int i, int j){
	return arena.grid[i][j];
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

OPERANDO Sistema(OPERANDO op, Maquina *m) {
//Os switch de j que vão de 0 a 5 representam as direções das ações, em sentido horário, começando por cima
  OPERANDO return_value;
	int i = op.val.n / 10;
//Primeiro algarismo indica o tipo de ação
	int j = op.val.n % 10;
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
			if (aux1 < 2) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			else printf("ERROU PRA CIMA LIXO   %d  %d\n", j, op.val.n);  	 //borda de cima
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}

			case 1:
			if (aux1 == 0 || aux2 == GRID_TAM) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			else printf("ERROU PRA DIREITA CIMA LIXO\n");  	 //borda de cima e direita
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}

			case 2:
			if (aux1 == 2*GRID_TAM || aux2 == GRID_TAM){
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			else printf("ERROU PRA DIREITA BAIXO LIXO\n");  	 //borda de baixo e direita
			cel = buscaCel(aux1+1, aux2+1);
	  	if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}

			case 3:
			if (aux1 > (2*GRID_TAM - 2)) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			else printf("ERROU PRA BAIXO LIXO\n");  	 //borda de baixo
			cel = buscaCel(aux1+2, aux2);
	  	if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}

			case 4:
			if (aux1 == 2*GRID_TAM || aux2 == 0) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			else printf("ERROU PRA ESQUERDA BAIXO LIXO\n");  	 //borda de baixo e esquerda
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}

			case 5:
			if (aux1 == 0 || aux2 == 0) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			else printf("ERROU PRA ESQUERDA CIMA LIXO\n");  	 //borda de cima e esquerda
			cel = buscaCel(aux1-1, aux2-1);
			if(cel.ocupado == 0) {
			  cel.ocupado = m->patente;
			  return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	
		}
		
		case 1://Atacar
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
			case 0:
			if (aux1 < 2){
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de cima
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(m->patente, aux1-2, aux2));
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	
			case 1:
			if (aux1 == 0 || aux2 == GRID_TAM) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de cima e direita
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(m->patente, aux1-1, aux2+1));
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	
			case 2:
			if (aux1 == 2*GRID_TAM || aux2 == GRID_TAM) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de baixo e direita
			cel = buscaCel(aux1+1, aux2+1);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(m->patente, aux1+1, aux2+1));
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	
			case 3:
			if (aux1 > (2*GRID_TAM - 2)) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de baixo
			cel = buscaCel(aux1+2, aux2);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(m->patente, aux1+2, aux2));
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	
			case 4:
			if (aux1 == 2*GRID_TAM || aux2 == 0) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de baixo e esquerda
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(m->patente, aux1+1, aux2-1));
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	
			case 5:
			if (aux1 == 0 || aux2 == 0) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de cima e esquerda
			cel = buscaCel(aux1-1, aux2-1);
			if(cel.ocupado > 0) {
			  destroi_maquina(buscaMaq(m->patente, aux1-1, aux2-1));
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	
		}
		
		case 2://Recolher. Verifica quantos cristais há na célula e retorna esse valor.
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
			case 0:
			if (aux1 < 2) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de cima
			cel = buscaCel(aux1-2, aux2);
			return_value.val.n = cel.cristais;
			return_value.t = NUM;
			return return_value;
			case 1:
			if (aux1 == 0 || aux2 == GRID_TAM) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de cima e direita
			cel = buscaCel(aux1-1, aux2+1);
			return_value.val.n = cel.cristais;
			return_value.t = NUM;
			return return_value;
			case 2:
			if (aux1 == 2*GRID_TAM || aux2 == GRID_TAM) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de baixo e direita
			cel = buscaCel(aux1+1, aux2+1);
			return_value.val.n = cel.cristais;
			return_value.t = NUM;
			return return_value;
			case 3:
			if (aux1 > (2*GRID_TAM - 2)) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de baixo
			cel = buscaCel(aux1+2, aux2);
		  return_value.val.n = cel.cristais;
			return_value.t = NUM;
			return return_value;
			case 4:
			if (aux1 == 2*GRID_TAM || aux2 == 0) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de baixo e esquerda
			cel = buscaCel(aux1+1, aux2-1);
			return_value.val.n = cel.cristais;
			return_value.t = NUM;
			return return_value;
			case 5:
			if (aux1 == 0 || aux2 == 0) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}  	 //borda de cima e esquerda
			cel = buscaCel(aux1-1, aux2-1);
			return_value.val.n = cel.cristais;
			return_value.t = NUM;
			return return_value;
		}
		
		case 3://Depositar. Verifica se há alguém na célula e deposita os cristais na máquina se não houver ninguém
		aux1 = m->pos[0];
		aux2 = m->pos[1];
		switch(j){
		  if (aux1 < 2) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			} //borda de cima
			cel = buscaCel(aux1-2, aux2);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			case 1:
			if (aux1 == 0 || aux2 == GRID_TAM) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			} //borda de cima e direita
			cel = buscaCel(aux1-1, aux2+1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			case 2:
			if (aux1 == 2*GRID_TAM || aux2 == GRID_TAM) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			} //borda de baixo e direita
			cel = buscaCel(aux1+1, aux2+1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			case 3:
			if (aux1 > (2*GRID_TAM - 2)) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			} //borda de baixo
			cel = buscaCel(aux1+2, aux2);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			case 4:
			if (aux1 == 2*GRID_TAM || aux2 == 0) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			} //borda de baixo e esquerda
			cel = buscaCel(aux1+1, aux2-1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}
			case 5:
			if (aux1 == 0 || aux2 == 0) {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			} //borda de cima e esquerda
			cel = buscaCel(aux1-1, aux2-1);
			if(cel.ocupado == 0) {
				cel.cristais = m->ncristais;
				return_value.val.n = 1;
			  return_value.t = NUM;
			  return return_value;
			}
			else {
			  return_value.val.n = 0;
			  return_value.t = NUM;
			  return return_value;
			}	
		}
	}
}

int main (int ac, char **av){
	for (int i; i<5; i++){
		printf("%d", diretriz[i].op.val.n);
	}
	InsereArena();
	for (int i=0; i<N_TIMES*TROPAS_POR_EXERCITO; i++){
		InsereExercito(diretriz, i);
	}
	/*for (int i=0; i<5; i++){
				printf("%dNOT FUNNY\n", tropa->prog[i].op.val.n);//soh quero pontuar aqui que isso funciona
			}*/
	for (int i=0; i<RODADAS; i++)
		Atualiza();
	return 0;
}