/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h> 
#include <sys/mman.h>
#include <linux/sched.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

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

struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[100]; 
} message; 

struct sched_attr {
    uint32_t size;
    uint32_t sched_policy;
    uint64_t sched_flags;
    int32_t sched_nice;
    uint32_t sched_priority;
    uint64_t sched_runtime;
    uint64_t sched_deadline;
    uint64_t sched_period;
};

/**
 * Funcao set attribute para scheduling
 * 
 */
int sched_setattrFMC(pid_t pid, 
              const struct sched_attr *attr,
                                unsigned int flags) {
    return syscall(__NR_sched_setattr, pid, attr, flags);
} 

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
void f_set_thrust(double new_thrust){
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

    struct sched_attr attrFMC = {
        .size = sizeof (attrFMC),
        .sched_policy = SCHED_DEADLINE,
        .sched_runtime = 10 * 1000 * 1000, // 10 000 000 microsegundos = 10 segundos
        .sched_period = 1 * 1000 * 1000 * 1000, //1 000 000 000 nanosegundos = 1 segundos
        .sched_deadline = 11 * 1000 * 1000 // 11 000 000 microsegundos = 11 segundos
    };
	//printf("Debug attributes %d %d %d %d",attr->sched_runtime, attr->sched_period, attr->sched_deadline, attr->size);

	if (sched_setattrFMC(0, &attrFMC, 0)){
        perror("sched_setattr()");
    }	

    struct aviao_t * aviao = (struct aviao_t*) input;

    int altitude = (*aviao).altitude;
    vel_init = (*aviao).vel_init;
    vel_final = (*aviao).vel_final;

    printf("Valores da estrutura: altitude %i, velocidade inicial %d, velocidade final %d \n", altitude, vel_init, vel_final);

    double drag = computeDrag(altitude);
    printf("Drag = %f\n", drag);

    printf("A enviar para o FDR");
    
    // write message
    key_t key; 
    int msgid; 
  
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
    message.mesg_type = 1; 
  
    printf("Write Data : "); 
    message.mesg_text = ["2,300,200", "3,400,215"]; 
  
    // msgsnd to send message 
    msgsnd(msgid, &message, sizeof(message), 0); 
  
    // display the message 
    printf("Data send is : %s \n", message.mesg_text); 
    
    //while(1)
    //
    //computeSpeed(time)
    //verify velocity requirements
    //update time
    //sleep(periodo)
}
