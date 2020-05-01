/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/

#include "fmc.h"
#define peso 79000

double last_time = 0.0;
double vel = 0.0;
double drag = 0.0;
double thrust = 0.0;

/** Funcao para calcular a velocidade
 * Calcula a velocidade num certo instante de tempo com um certo thrust
 * Parameters: time - instante de tempo
 * Returns: velocidade resultante
 */ 
double getSpeed(double time){
    double new_vel = vel + (thrust + drag)/(peso/10000^2) * (time - last_time);
    last_time = time; //atualiza os
    vel = new_vel;    //valores antigos
    return vel;
}

/** Funcao para calcular o Drag
 * Calcula o drag dada uma certa altitude h
 * Parameters: h - altitude
 * Returns: drag
 */ 
double getDrag(int h){
	return -100000 + 1,87 * h;
}

/** Funcao de comunicacao com ctrl
 * Retorna a velocidade atual
 */ 
double f_get_speed(){
    return vel;
}

/** Funcao de comunicacao com ctrl
 * Altera o thrust para um novo valor
 */ 
double f_set_thrust(double new_thrust){
    thrust = new_thrust;
}