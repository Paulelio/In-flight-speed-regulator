/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/

#ifndef _FMC_H
#define _FMC_H

/*	Definir interface do computador de gestao de voo	*/

/* Estrutura aviao que tem os valores iniciais */
struct aviao_t {
	int altitude;
	int vel_init;
	int vel_final;
};

/**
 * Funcao corpo do Flight Management Computer
 */
void flightManagement(void* input);

/**
 * Funcao que recebe o thrust e retorna a speed, relativa a esse thrust,
 * baseada na funcao V(Ti) = V(Ti-1) + ((T+D)/Mmod) * (Ti - Ti-1)
 * onde Mmod = peso/10000^2
 */
void computeSpeed(double drag);

double computeDrag(int h);
#endif
