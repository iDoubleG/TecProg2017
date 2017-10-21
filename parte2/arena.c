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
<<<<<<< HEAD
#include "maq.h"
#include "arena.h"

#define GRID_TAM 1000
#define EXERCITO 500
#define TIMESTEP 50


typedef struct {
	int grid[GRID_TAM*2][GRID_TAM];
	int ExercitosAtivos[EXERCITO];
} Arena;
=======
#include <time.h>
#include <math.h>

int comparador(int rola, int rezendeMeSalva, int base[rezendeMeSalva][rezendeMeSalva]) {
	if (rola > rezendeMeSalva * rezendeMeSalva) rola = 0;
	int contador1 = rola / rezendeMeSalva;
	int contador2 = rola % rezendeMeSalva;
	if (contador2 == 0) contador1--;
	else contador2--;
	if (base[contador1][contador2] > 0) return comparador(base, rola + 1, rezendeMeSalva);
	else return rola;
}

int main() {
>>>>>>> 738d14860fa8413418ec78557fc0e164d380e476

	int rezendeViado = 50;
	int rezendeGay = 25;
	int REZENDEDESGRACADOPAUNOCUDOINFERNO = 25;
	int certo, cu1, xereca2;
	int cont1, cont2, cont3, cont4, cont5;

	int grid[rezendeViado][rezendeGay];
	srand(time(NULL));

<<<<<<< HEAD
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
	
=======
	// Criacao da arena
	for (int i = 0; i < rezendeViado; i++) {
		for (int j = 0; j < rezendeGay; j++) {
      		if ((i % 2 == 0 && j % 2 != 0) || (j % 2 == 0 && i % 2 != 0)) {
      			grid[i][j] = 0;
      		}
      		else {
				int random1 = rand() % 100;
				if (random1 < 5) {grid[i][j] = 5; cont5++;}
				else if (random1 < 25) {grid[i][j] = 4; cont4++;}
				else if (random1 < 45) {grid[i][j] = 3; cont3++;}
				else if (random1 < 55) {grid[i][j] = 2; cont2++;}
				else {grid[i][j] = 1; cont1++;}
      		}
      		printf("%d", grid[i][j]);
   		}
   		printf("\n");
	}
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("%d %d %d %d %d", cont1, cont2, cont3, cont4, cont5);

	// Colocar Bases
	/*int rezendeMeSalva = ceil(sqrt(REZENDEDESGRACADOPAUNOCUDOINFERNO));
	int base[rezendeMeSalva][rezendeMeSalva];

	for (int i = 0; i < REZENDEDESGRACADOPAUNOCUDOINFERNO; i++) {
		// Selecão da área
		int random2 = rand() % (rezendeMeSalva * rezendeMeSalva);
		int base1 = random2 / rezendeMeSalva;
		int base2 = random2 % rezendeMeSalva;
		int retorno = random2 * rezendeMeSalva + base2;
		if (base2 == 0) base1--;
		else base2--;
		if (base[base1][base2] > 0) {
			certo = comparador(retorno + 1, rezendeMeSalva, base);
			base1 = certo / rezendeMeSalva;
			base2 = certo % rezendeMeSalva;
		}
		else certo = retorno;
		base[base1][base2] = 1;

		// Aleatioriedade
		cu1 = rezendeViado/rezendeMeSalva;
		xereca2 = rezendeGay/rezendeMeSalva;
		for (int i = 0; i < cu1; i++) {
			for (int j = 0; j < xereca2; j++) {
				int random3 = rand() % (cu1 * xereca2);
			}
		}
	}*/

	return 0;
>>>>>>> 738d14860fa8413418ec78557fc0e164d380e476
}