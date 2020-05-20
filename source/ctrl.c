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

#define KP 10
#define KI 0
#define KD 1.5

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


/** Funcao principal do ctrl
 * 
 * 
 */ 
void controlAlgorithm(void * input){
    struct sched_attr attrCTRL = {
        .size = sizeof (attrCTRL),
        .sched_policy = SCHED_DEADLINE,
        .sched_runtime = 10 * 1000 * 1000,
        .sched_period = 2 * 1000 * 1000 * 1000 * 1000,
        .sched_deadline = 11 * 1000 * 1000
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
    
    //while(1)
    //buscar speed
    //computar thrust
    //guardar thrust para fmc
    //sleep periodo

    double error;
    double integral;
    double derivative;

    double error_prior = 0.0;
    double integral_prior = 0.0;

    double vel_atual = 0.0;
    
    double vel_final = ((intptr_t)input) / 3.6;
    double thrust = 0.0;
    double iteration_time = 1.0; //??
    
    
    semSpeed = sem_open("sem_Speed", O_CREAT);
    semThrust = sem_open("sem_Thrust", O_CREAT);

    sched_setattrCTRL(0, &attrCTRL, 0);
    
    for(;;){
        printf("[CTRL] no for\n");
        
        //sem_wait(semSpeed);
        
        vel_atual = shmp->speed;
        //sem_post(semSpeed);
        
        //adicionar if para terminar com speed > x valor

        error = vel_final - vel_atual;
        integral = integral_prior + error * iteration_time;
        derivative = (error - error_prior) / iteration_time;
        thrust = KP * error + KI * integral + KD * derivative;

        //sem_wait(semThrust);
        shmp->thrust = thrust;
        //sem_post(semThrust);

        error_prior = error;
        integral_prior = integral;
        
        //printf("no ctrl - vel atual = %f, erro = %f \n", vel_atual, error);
        //printf("thrust no crtl = \n", thrust);
        sleep(10); //em NRT
        sched_yield(); //em RT
    }

    /* sem_close(semSpeed);
    sem_close(semThrust);

    sem_unlink("sem_Speed");
    sem_unlink("sem_Thrust"); */
}
 