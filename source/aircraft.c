/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/

/**
Funcao main que inicializa e coordena o sistema
*/
#include <stdio.h>
#include <stdbool.h>
#include "fmc.h"

struct aviao_t {
	int altitude;
	double vel_atual;
};

/** Tabela 1
 * Valores minimos e maximos para as variaveis
 *	|               | minimum | maximum | units |
 *  |---------------|---------|---------|-------|
 * 	| engine thrust | 0       | 242     | kN    |
 * 	| speed         | 300     | 823     | km/h  |
 * 	| Altitude      | 1000    | 12500   | m     |
 * 
 * */
int main(int argc, char** argv) {

	if(argc != 3){
		printf("Uso: ./aircraft <altitude> <velocidade_init> <velocidade_final>\n");
		return -1;
	}

	int altitude = atoi(argv[1]);
	if(altitude > 12500 || altitude < 1000){
		printf("Altitude entre 12500 e 1000 sff.\n");
		return -1;
	}
	
	double vel_init = atol(argv[2]);
	double vel_final = atol(argv[3]);
	if(!verifySpeed(vel_init) || !verifySpeed(vel_final)){
		printf("Velocidades entre 300 e 823 sff.\n");
		return -1;
	}

	double drag = getDrag(altitude);
	struct aviao_t* aviao = (struct aviao_t*) malloc(sizeof(struct aviao_t));
	aviao->altitude = altitude;
	aviao->vel_atual = vel_init;

}

/**
 * Funcao para verificar se a velocidade estah dentro dos limites
 * Parameters: v - velocidade
 * Return: true if 300 <= v <= 823, false otherwise
 */
bool verifySpeed(double v){
	if(v < 300 || v > 823){
		return false;
	}
	return true;
}
