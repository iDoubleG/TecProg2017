
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
#define TROPAS_POR_EXERCITO 1
#define CLOCK 5
#define N_TIMES 1
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
			arena.grid[i][j].cristais = 0;
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
void Atualiza_arredores (Maquina *m, int centroi, int centroj){ // this new
	m->arredores.centro = buscaCel(centroi, centroj);
	m->arredores.acima = buscaCel(centroi-2, centroj);
	m->arredores.esquerda_cima = buscaCel(centroi-1, centroj+1);
	m->arredores.esquerda_baixo = buscaCel(centroi+1, centroj+1);
	m->arredores.baixo = buscaCel(centroi+2, centroj);
	m->arredores.direita_baixo = buscaCel(centroi+1, centroj-1);
	m->arredores.direita_cima = buscaCel(centroi-1, centroj-1);
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
	    arenaGR1.time = equipe;
	    arenaGR1.pos[0] = random11;
	    arenaGR1.pos[1] = random12;
	    arena.base[equipe] = arenaGR1;
		  break;
	  }
    }

    int cont = 0;
    int tropas = TROPAS_POR_EXERCITO;
    while (tropas > 0) {
		random21 = rand() % (GRID_TAM*2 - 2) + 1; //indices
		random22 = rand() % (GRID_TAM - 2) + 1; //aleatorios

		if (arenaGR2.terreno != NADA && arenaGR2.time == 0 && arenaGR2.ocupado == 0) {
			Maquina *tropa = cria_maquina(diretriz);
		    tropa->time = equipe;
		    tropa->pos[0] = random21;
		    tropa->pos[1] = random22;
		    tropa->patente = cont;
				Atualiza_arredores(tropa, random21, random22); //this new
			arenaGR2.ocupado = 1;
			arena.exercitosAtivos[equipe][cont] = tropa;
			cont++;
			tropas--;
		}
	}
	/*for (int i=0; i<2; i++){
		for (int j=0; j<5; j++){
			printf("%dNOT FUNNY\n", arena.exercitosAtivos[equipe][i]->prog[j].op.val.n);//soh quero pontuar aqui que isso funciona
		}
	}*/

}

void RemoveExercito (Maquina *derrotado) {//poupa trabalho, enviar somente a tropa a ser eliminada
	destroi_maquina(derrotado);
}

void Atualiza () {
	for (int i=0; i<N_TIMES; i++)
		for (int j=0; j<TROPAS_POR_EXERCITO; j++){
			exec_maquina(arena.exercitosAtivos[i][j], CLOCK);
		}
	arena.tempoCorrido++;
}

Celula buscaCel (int i, int j){
	if(i >= GRID_TAM*2 || j >= GRID_TAM || i < 0 || j < 0){ // this new
		Celula nullcel; //uma célula padrão que representa o limite da grade
		return nullcel;
	}
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


OPERANDO SisMov(int i, Maquina *m){ //modularização de Sistema, Mover
	OPERANDO return_value;
	int aux1;
	int aux2;
	Celula cel;
	aux1 = m->pos[0];
	aux2 = m->pos[1];
	switch(i){
		case 0:
		if (aux1 < 2) {
			return_value.val.n = 0;
			return_value.t = NUM;
			return return_value;
		}
		//borda de cima
		cel = buscaCel(aux1-2, aux2);
		if(cel.ocupado == 0) {
			cel.ocupado = m->patente;
			Atualiza_arredores(m, aux1-2, aux2); // this new
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
		//borda de cima e direita
		cel = buscaCel(aux1-1, aux2+1);
		if(cel.ocupado == 0) {
			cel.ocupado = m->patente;
			Atualiza_arredores(m, aux1-1, aux2+1); //this new
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
		//borda de baixo e direita
		cel = buscaCel(aux1+1, aux2+1);
		if(cel.ocupado == 0) {
			cel.ocupado = m->patente;
			Atualiza_arredores(m, aux1+1, aux2+1); //this new
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
		//borda de baixo
		cel = buscaCel(aux1+2, aux2);
		if(cel.ocupado == 0) {
			cel.ocupado = m->patente;
			Atualiza_arredores(m, aux1+2, aux2); //this new
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
		//borda de baixo e esquerda
		cel = buscaCel(aux1+1, aux2-1);
		if(cel.ocupado == 0) {
			cel.ocupado = m->patente;
			Atualiza_arredores(m, aux1+1, aux2-1); //this new
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
		//borda de cima e esquerda
		cel = buscaCel(aux1-1, aux2-1);
		if(cel.ocupado == 0) {
			cel.ocupado = m->patente;
			Atualiza_arredores(m, aux1-1, aux2-1); //this new
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
OPERANDO SisAtc(int i, Maquina *m){ //modularização de Sistema, Ataque
	OPERANDO return_value;
	int aux1;
	int aux2;
	Celula cel;
	aux1 = m->pos[0];
	aux2 = m->pos[1];
	switch(i){
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
}
OPERANDO SisRec(int i, Maquina *m){ //modularização de Sistema, Recolher
	OPERANDO return_value;
	int aux1;
	int aux2;
	Celula cel;
	aux1 = m->pos[0];
	aux2 = m->pos[1];
	switch(i){
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
}
OPERANDO SisDep(int i, Maquina *m){ //modularização de Sistema, Depositar
	OPERANDO return_value;
	int aux1;
	int aux2;
	Celula cel;
	aux1 = m->pos[0];
	aux2 = m->pos[1];
	switch(i){
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
		case 0:/*Mover. Retorna 1 se não há ninguém na célula
						retorna 0 se o movimento não é possível*/
		return_value = SisMov(j, m);
		return return_value;

		case 1://Atacar
		return_value = SisAtc(j, m);
		return return_value;

		case 2://Recolher. Verifica quantos cristais há na célula e retorna esse valor.
		return_value = SisRec(j, m);
		return return_value;

		case 3://Depositar. Verifica se há alguém na célula e deposita os cristais na máquina se não houver ninguém
		return_value = SisDep(j, m);
		return return_value;
	}
}

int main (int ac, char **av){
	InsereArena();
	for (int i=0; i<N_TIMES; i++){
		InsereExercito(diretriz, i);
	}
	/*for (int i=0; i<N_TIMES; i++){
		for (int j=0; j<TROPAS_POR_EXERCITO; j++)
			for (int k=0; k<5; k++)
				printf("%dNOT FUNNY\n", arena.exercitosAtivos[i][j]->prog[k].op.val.n);//soh quero pontuar aqui que isso funciona
	}*/
	for (int i=0; i<RODADAS; i++)
		Atualiza();
	return 0;
}
