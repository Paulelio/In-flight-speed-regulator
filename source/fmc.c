/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/
#include <time.h>
#include <stdbool.h>
#include "fmc.h"
#define peso 79000
#define period 1 //in milliseconds 
#define NACQUI 5 //valor de quantos em quantos ciclos vai ser enviada info para o fdr
#define LIMIT_INTERVAL 0.05 //valor de intervalo aceitavel da velocidade final
#define SAMPLE_INT = 30000 //intervalo entre medicoes 30s (30000 ms)

int vel_init = 0;
int vel_final = 0;

double vel = 0.0;
double thrust = 0.0;

double last_time = 0.0;

/** Funcoes para uso de relogios
 * int clock_getres(clockid_t clock_id, struct timespec *res);
 * int clock_gettime(clockid_t clock_id, struct timespec *tp);
 * int clock_settime(clockid_t clock_id, const struct timespec *tp);
 */

/** Funcao para calcular a velocidade
 * Calcula a velocidade num certo instante de tempo com um certo thrust
 * Parameters: time - instante de tempo
 * Returns: velocidade resultante
 */ 
void computeSpeed(double time, double drag){
    double new_vel = vel + (thrust + drag)/(peso/10000^2) * (time - last_time);
    last_time = time; //atualiza os
    vel = new_vel;    //valores antigos
}

/** Funcao para calcular o Drag
 * Calcula o drag dada uma certa altitude h
 * Parameters: h - altitude
 */ 
double computeDrag(int h){
	return (1.87 * h) - 100000; //dependendo da restante implementacao pode ser local
}

/** Funcao de comunicacao com ctrl
 * Retorna a velocidade atual
 */ 
double f_get_speed(){
    return vel; //adicionar locks
}

/** Funcao de comunicacao com ctrl
 * Altera o thrust para um novo valor
 */ 
double f_set_thrust(double new_thrust){
    thrust = new_thrust; //adicionar locks
}

/** Funcao para verificar se a velocidade atual estah a 5% da velocidade final
 * Parameters: speed - velocidade atual
 * Returns: true - se estiver a menos de 5%, false se estiver fora desse limite
 */ 
bool verifySpeedLim(double speed){
    if(speed < vel_final * (1 - LIMIT_INTERVAL) || speed > vel_final * (1 + LIMIT_INTERVAL)){ //passar para constantes os valores limite #define
        return false;
    }
    return true;
}

/** Funcao principal do FMC
 * 
 * 
 */ 
void flightManagement(void * input){
    printf("in flight management\n");
    struct aviao_t * aviao = (struct aviao_t*) input;

    int altitude = (*aviao).altitude;
    vel_init = (*aviao).vel_init;
    vel_final = (*aviao).vel_final;

    printf("Valores da estrutura: altitude %i, velocidade inicial %d, velocidade final %d \n", altitude, vel_init, vel_final);

    double drag = computeDrag(altitude);
    printf("Drag = %f\n", drag);
    
    
    //while(1)
    //
    //computeSpeed(time)
    //verify velocity requirements
    //update time
    //sleep(periodo)
}