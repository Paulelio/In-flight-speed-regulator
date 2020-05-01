/**
Grupo 1
Diogo Fernandes 
Paulo Alvares 49460
*/

/**
Funcao main que inicializa e coordena o sistema
*/
#include <stdio.h>

struct aviao_t {
	int altitude,
	int peso
};

int main(int argc, char** argv) {

	if(argc != 3){
		printf("Uso: ./aircraft <altitude> <velocidade_init> <velocidade_final>\n");
		return -1;
	}

	int altitude = atoi(argv[1]);
	double drag = getDrag(altitude);
	//double vel_init = atol(argv[2]);
	//double vel_final = atol(argv[3]);


	

} 

double getDrag(int h){
	return -100000 + 1,87 * altitude;
}
