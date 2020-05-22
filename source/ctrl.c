/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h> 
#include <sys/mman.h>
#include <linux/sched.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h> 

#include "ctrl.h"

#define KP 0.7 // reduzir muito (3,4,5) -- evita que o controlo tenho uma grande variacao
#define KI 0.06
#define KD 0.3
#define MAX_THRUST 242000
#define MIN_THRUST 0
#define LIMIT_INTERVAL 0.05


#define SHM_KEY 0x1234

struct shmseg {
   double speed;
   double thrust;
};

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

sem_t *semSpeed;
sem_t *semThrust;

/**
 * Funcao set attribute para scheduling
 * 
 */
int sched_setattrCTRL(pid_t pid, 
              const struct sched_attr *attr,
                                unsigned int flags) {
    return syscall(__NR_sched_setattr, pid, attr, flags);
}

/** Funcao para verificar se a velocidade atual estah a 5% da velocidade final
 * Parameters: speed - velocidade atual
 * Returns: true - se estiver a menos de 5%, false se estiver fora desse limite
 */ 
bool verifySpeedCTRL(double speed, double vel_final){
    if(speed < vel_final * (1 - LIMIT_INTERVAL) || speed > vel_final * (1 + LIMIT_INTERVAL)){ //passar para constantes os valores limite #define
        return false;
    }
    return true;
}

/** Funcao principal do ctrl
 * 
 * 
 */ 
void controlAlgorithm(void * input){
    struct sched_attr attrCTRL = {
        .size = sizeof (attrCTRL),
        .sched_policy = SCHED_DEADLINE,
        .sched_runtime = 9 * 1000,
        .sched_period = 100 * 1000 * 1000,
        .sched_deadline = 14 * 1000,
    };

    //--Inicializacao shared memory--//
    int shmid;
    struct shmseg *shmp;
    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);

    if (shmid == -1) {
      perror("Shared memory");
      return;
    }
   
    // Attach to the segment to get a pointer to it.
    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1) {
      perror("Shared memory attach");
      return;
    }

    double error;
    double integral;
    double derivative;

    double error_prior = 0.0;
    double integral_prior = 0.0;

    double vel_atual = 0.0;
    
    double vel_final = ((intptr_t)input) / 3.6;
    //double thrust = MAX_THRUST/2;
    double iteration_time = 0.01; //1cs
    
    
    semSpeed = sem_open("sem_Speed", O_CREAT);
    semThrust = sem_open("sem_Thrust", O_CREAT);
    double thrust = shmp->thrust;
    sched_setattrCTRL(0, &attrCTRL, 0);

    for(;;){
        printf("[CTRL] no for\n");
        
        sem_wait(semThrust);
        vel_atual = shmp->speed;
        printf("[CTRL] Vel: %f\n", vel_atual);

        if(verifySpeedCTRL(vel_atual, vel_final)){
            printf("[CTRL] Acabou o calculo\n A sair...\n");
            return;
        }
        
        error = vel_final - vel_atual;
        integral = integral_prior + error * iteration_time;
        derivative = (error - error_prior) / iteration_time;
        thrust = thrust + ((KP * error) + (KI * integral) + (KD * derivative));
        
        if(thrust > MAX_THRUST){
             thrust = MAX_THRUST;
        }
        else if (thrust < MIN_THRUST){
            thrust = MIN_THRUST;
        }
       
        printf("[CTRL] erro %f,\n integral %f,\n derivative %f\n, error prior %f,\n integral prior %f\n", error, integral, derivative, error_prior, integral_prior);

        shmp->thrust = thrust;
        printf("[CTRL] Thrust: %f\n", thrust);
        sem_post(semThrust);

        error_prior = error;
        integral_prior = integral;
        
        //sleep(10); //em NRT
        sched_yield(); //em RT
        //sleep(5);

        /* 
        
        erro = vel_final - vel_atual
        txVar = erro/vel_atual
        if(txVar > 0)
            thrust = 242000 * (1-txVar)
        else if(txVar < 0)
            thrust = 242000 * txVar
        
         */
    }
} 
 