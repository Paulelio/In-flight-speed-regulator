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
#include <string.h> 

#include "fmc.h"

#define peso 79000
#define period 1 //in milliseconds 
#define NACQUI 1 //valor de quantos em quantos ciclos vai ser enviada info para o fdr
#define LIMIT_INTERVAL 0.05 //valor de intervalo aceitavel da velocidade final
#define SAMPLE_INT = 30000 //intervalo entre medicoes 30s (30000 ms)

/** Funcoes para uso de relogios
 * int clock_getres(clockid_t clock_id, struct timespec *res);
 * int clock_gettime(clockid_t clock_id, struct timespec *tp);
 * int clock_settime(clockid_t clock_id, const struct timespec *tp);
 */

int vel_final = 0.0;
double vel = 0.0;
double thrust = 0.0;

struct timespec *last_time;

struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[1024];
} fdr_message; 

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
 */
int sched_setattrFMC(pid_t pid, 
              const struct sched_attr *attr,
                                unsigned int flags) {
    return syscall(__NR_sched_setattr, pid, attr, flags);
} 

/** Funcao para calcular a velocidade
 * Calcula a velocidade num certo instante de tempo com um certo thrust
 * Parameters: time - instante de tempo
 * Returns: velocidade resultante
 */ 
void computeSpeed(struct timespec *time, double drag){
    time_t result;
    long nano_result;
    printf("ComputeSpeed antes do if \n");
    if ((time->tv_nsec - last_time->tv_nsec) < 0) {
        result = time->tv_sec - last_time->tv_sec - 1;
        nano_result = (time->tv_nsec - last_time->tv_nsec) + 1000000000;
    } else {
        result = time->tv_sec - last_time->tv_sec;
        nano_result = time->tv_nsec - last_time->tv_nsec;
    }
    printf("Segundos = %d \n nanosec = %f\n, thrust = %f\n, drag = %f\n", result, nano_result, thrust, drag);
    double new_vel = vel + ((thrust + drag)/(peso/10000^2)) * (result + nano_result/1000000000);
    last_time = time; //atualiza os
    vel = new_vel;    //valores antigos
    printf("Velocidade compute speed %f\n", vel);
    
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
    printf("No Flight Management\n");

    struct sched_attr attrFMC = {
        .size = sizeof(attrFMC),
        .sched_policy = SCHED_DEADLINE,
        .sched_runtime = 10 * 1000 * 1000, // 10 000 000 microsegundos = 10 segundos
        .sched_period = 2 * 1000 * 1000 * 1000 * 1000, //1 000 000 000 nanosegundos = 1 segundos
        .sched_deadline = 11 * 1000 * 1000 // 11 000 000 microsegundos = 11 segundos -- deadline não pode ser maior que o período!
    };
	//printf("Debug attributes %d %d %d %d",attr->sched_runtime, attr->sched_period, attr->sched_deadline, attr->size);

    struct aviao_t * aviao = (struct aviao_t*) input;

    last_time = malloc(sizeof *last_time);

    int altitude = (*aviao).altitude;
    vel = (*aviao).vel_init;
    vel_final = (*aviao).vel_final;
    //printf("Valores da estrutura: altitude %i, velocidade inicial %d, velocidade final %d \n", altitude, vel_init, vel_final);

    double drag = computeDrag(altitude);
    //printf("Drag = %f\n", drag);

    //--MESSAGE QUEUE CODE--
    // write message
    key_t key; 
    int msgid;
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
    fdr_message.mesg_type = 1;
    char *buffer = (char *) malloc(1024);

    int cycle_num = 1;

    //printf("Adquirir timespec\n");
    //time struct
    struct timespec *tp = malloc(sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, tp);

    //set sched attribute
    sched_setattrFMC(0, &attrFMC, 0);

    for(;;){
        printf("No for do FMC\n");
        printf("Vel mal entra no for = %f\n", vel);

        printf("Antes do if\n");
        // Envia mensagem a cada NACQUI ciclos
        if(cycle_num % NACQUI == 0 || verifySpeedLim(vel)){
            //printf("A enviar para o FDR\n");
            //printf("A escrever dados: \n");

            long current_timestamp = (unsigned)time(NULL);
            // printf("antes da escrita %s\n", buffer);
            printf("%ld,%f,%f\n", current_timestamp, vel, drag);
            snprintf(buffer, 255, "%d,%f,%f", current_timestamp, vel, drag);
            printf("depois da escrita %s\n", buffer);
            strncpy(fdr_message.mesg_text, buffer, sizeof(fdr_message.mesg_text)); 

            // msgsnd to send message 
            msgsnd(msgid, &fdr_message, sizeof(fdr_message), 0); 

            // display the message 
            printf("Dados enviados: %s \n", fdr_message.mesg_text); 

            if(verifySpeedLim(vel)){
                printf("Chegou ao limite aceitavel de velocidade\n");
                free(tp);
                // TEMOS QUE FAZER FREE DOS MALLOCS TOOOOOODOS
                exit(0);
            }
        }
        computeSpeed(tp, drag);

        cycle_num ++;
        sched_yield();
    }
    
}