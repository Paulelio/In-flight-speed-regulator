/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/

#ifndef _FMC_H
#define _FMC_H

/*	Definir interface do computador de gestao de voo	*/

/**
 * Funcao que recebe o thrust e retorna a speed, relativa a esse thrust,
 * baseada na funcao V(Ti) = V(Ti-1) + ((T+D)/Mmod) * (Ti - Ti-1)
 * onde Mmod = peso/10000^2
 */
double getSpeed(double thrust, double time);

double getDrag(int h);
#endif
