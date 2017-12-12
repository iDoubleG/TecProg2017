
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
#include "chamSis.h"

#define TAM_GRADE 10
#define TROPAS_POR_EXERCITO 1
#define RELOGIO 5
#define N_TIMES 2
#define N_CRISTAIS 2
#define RODADAS 1

// macros para pegar uma celula da arena
#define arenaG arena.grade[i][j]
#define arenaGR1 arena.grade[aleat11][aleat12]
#define arenaGR2 arena.grade[aleat21][aleat22]

INSTR diretriz[] = {
  {SISM, {NUM, 0}},
  {SISM, {NUM, 1}},
  {SISM, {NUM, 2}},
  {SISM, {NUM, 3}},
  {SISM, {NUM, 4}},
  {SISM, {NUM, 5}}
};

typedef struct {
	Maquina *exercitos_ativos[256][TROPAS_POR_EXERCITO];
	int tempo_corrido;
	int contador_exercitos;
	Celula grade[TAM_GRADE][TAM_GRADE*2];
	Celula base[N_TIMES];
} Arena;

Arena arena;

void InsereArena() {

	int aleat21, aleat22; // indices aleatorios para aleatoriedade dos cristais
	int cristaisBase;

	arena.tempo_corrido = 0;

	srand(time(NULL));

	// Criacao da arena
	for (int i = 0; i < TAM_GRADE; i++) {
		for (int j = 0; j < (TAM_GRADE*2); j++) {
			// designa "NADA" as casas da matriz que nao serao utilizadas (por ser hexagonal)
      		if ((i % 2 == 0 && j % 2 != 0) || (j % 2 == 0 && i % 2 != 0))
      			{arenaG.terreno = NADA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
      		// designa "ARMADILHA", "AGUA", "AREIA", "ESTRADA" ou "RUELA" aleatoriamente
      		// as casas utilizaveis da matriz com probabilidade predefinida
      		else {
				int aleat1 = rand() % 100;
				if (aleat1 < 5) {arenaG.terreno = ARMADILHA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
				else if (aleat1 < 25) {arenaG.terreno = AGUA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
				else if (aleat1 < 45) {arenaG.terreno = AREIA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
				else if (aleat1 < 55) {arenaG.terreno = ESTRADA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
				else {arenaG.terreno = RUELA; arenaG.pos[0] = i; arenaG.pos[1] = j;}
			arena.grade[i][j].cristais = 0;
      		}
   		}
	}

	cristaisBase = N_CRISTAIS;

	// designa os cristais aleatoriamente a matriz
	while (cristaisBase > 0) {
		aleat21 = rand() % (TAM_GRADE - 2) + 1; //indices
		aleat22 = rand() % (TAM_GRADE*2 - 2) + 1; //aleatorios

		if (arenaGR2.terreno != NADA) {
			arenaGR2.cristais++;
			arenaGR2.terreno = CRISTAL;
			cristaisBase--;
			//cristais numero_cristais x_cristal y_cristal -- para apres.py
			printf("cristais %d %d %d\n", arenaGR2.cristais, aleat21, aleat22);
		}
	}
}
void Atualiza_arredores (Maquina *m, int centroi, int centroj){
	m->arredores.centro = buscaCel(centroi, centroj);
	m->arredores.acima = buscaCel(centroi, centroj-2);
	m->arredores.esquerda_cima = buscaCel(centroi-1, centroj-1);
	m->arredores.esquerda_baixo = buscaCel(centroi-1, centroj+1);
	m->arredores.baixo = buscaCel(centroi, centroj+2);
	m->arredores.direita_baixo = buscaCel(centroi+1, centroj+1);
	m->arredores.direita_cima = buscaCel(centroi+1, centroj-1);
}

void InsereExercito (INSTR *diretriz, int equipe) { //chamar a função com a número arena.contador_exercitos no lugar da int equipe

	int aleat11, aleat12;
	int aleat21, aleat22;
    while(1) {
	  // designa as bases aleatoriamente a matriz
	  aleat11 = rand() % (TAM_GRADE - 2) + 1; // indices
	  aleat12 = rand() % (TAM_GRADE*2 - 2) + 1; // aleatorios

	  if (arenaGR1.terreno != NADA && arenaGR1.terreno != BASE && arenaGR1.terreno != CRISTAL) {
	    arenaGR1.terreno = BASE;
	    arenaGR1.time = equipe;
	    arenaGR1.pos[0] = aleat11;
	    arenaGR1.pos[1] = aleat12;
	    arena.base[equipe] = arenaGR1;
	    //base footage/base{numero_da_equipe}.png base_x base_y -- para apres.py
	    printf("base footage/base%d.png %d %d\n", equipe, aleat11, aleat12);
		  break;
	  }
    }

    int cont = 0;
    int tropas = TROPAS_POR_EXERCITO;
    while (tropas > 0) {
		aleat21 = rand() % (TAM_GRADE - 2) + 1; //indices
		aleat22 = rand() % (TAM_GRADE*2 - 2) + 1; //aleatorios

		if (arenaGR2.terreno != NADA && arenaGR2.time == 0 && arenaGR2.ocupado == 0) {
			Maquina *tropa = cria_maquina(diretriz);
		    tropa->time = equipe;
		    tropa->pos[0] = aleat21;
		    tropa->pos[1] = aleat22;
		    tropa->patente = cont;
			Atualiza_arredores(tropa, aleat21, aleat22); /*Armazena as informacoes sobre os arredores do robo, embora nao possam acessa-las sem o sistema*/
			arenaGR2.ocupado = 1;
			arena.exercitos_ativos[equipe][cont] = tropa;
			cont++;
			tropas--;
		}
	}
	printf("rob footage/roberto%d.png\n", equipe); //this new puce
	printf("move %d %d %d %d %d\n", equipe, aleat21, aleat22, aleat21, aleat22); // this new puce
}

void RemoveExercito (Maquina *derrotado) {/*Funcao remove as tropas eliminadas*/
	destroi_maquina(derrotado);
}

void Atualiza () {
	for (int i=0; i<N_TIMES; i++)
		for (int j=0; j<TROPAS_POR_EXERCITO; j++){
			exec_maquina(arena.exercitos_ativos[i][j], RELOGIO);
		}
	arena.tempo_corrido++;
}

Celula buscaCel (int i, int j){
	if(i >= TAM_GRADE || j >= TAM_GRADE*2 || i < 0 || j < 0){
		Celula cel_nula; /*Celula que representa os limites, bordas da arena*/
		return cel_nula;
	}
	return arena.grade[i][j];
}

Maquina *buscaMaq (int patente, int i, int j){
  int pos1[2];
  pos1[0] = i;
  pos1[1] = j;
  for(int k = 0; k < N_TIMES; k++){
    if((arena.exercitos_ativos[k][patente])->pos == pos1)
      return arena.exercitos_ativos[k][patente];
  }
}



OPERANDO Sistema(OPERANDO op, Maquina *m) {
//Os switch de j que vão de 0 a 5 representam as direções das ações, em sentido horário, começando por cima
  OPERANDO valor_de_retorno;
	int i = op.val.n / 10;
//Primeiro algarismo indica o tipo de ação
	int j = op.val.n % 10;
//Segundo algarismo indica a direção da ação
	switch(i){
		case 0:/*Mover. Retorna 1 se não há ninguém na célula
						retorna 0 se o movimento não é possível*/
		valor_de_retorno = SisMov(j, m);
		return valor_de_retorno;

		case 1://Atacar.
		valor_de_retorno = SisAtq(j, m);
		return valor_de_retorno;

		case 2://Recolher. Verifica quantos cristais há na célula e retorna esse valor.
		valor_de_retorno = SisRec(m);
		return valor_de_retorno;

		case 3://Depositar. Verifica se há alguém na célula e deposita os cristais na máquina se não houver ninguém
		valor_de_retorno = SisDep(m);
		return valor_de_retorno;
	}
}

int main (int ac, char **av){
	InsereArena();
	for (int i=0; i<N_TIMES; i++){
		InsereExercito(diretriz, i);
	}
	for (int i=0; i<RODADAS; i++)
		Atualiza();
	printf("fim\n");
	return 0;
}
