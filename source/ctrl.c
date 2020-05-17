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

#include "ctrl.h"
#include "fmc.h"

#define KP 10
#define KI 0
#define KD 1.5

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
        .sched_period = 1 * 1000 * 1000 * 1000,
        .sched_deadline = 11 * 1000 * 1000
    };

    pid_t pid = syscall(SYS_gettid);

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

    double vel_atual;
    double vel_final;
    double thrust;
    double iteration_time; //??

    if (sched_setattrCTRL(pid, &attrCTRL, 0)){
        perror("sched_setattr()");
    }
    
    for(;;){

        vel_atual = f_get_speed();
        error = vel_final - vel_atual;
        integral = integral_prior + error * iteration_time;
        derivative = (error - error_prior) / iteration_time;
        thrust = KP * error + KI * integral + KD * derivative;
        f_set_thrust(thrust);
        error_prior = error;
        integral_prior = integral;
        sleep(iteration_time);
        
        sched_yield();
    }
}
