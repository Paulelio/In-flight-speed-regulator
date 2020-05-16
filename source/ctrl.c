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
int sched_setattr(pid_t pid, 
              const struct sched_attr *attr,
                                unsigned int flags) {
    return syscall(__NR_sched_setattr, pid, attr, flags);
}

/** Funcao principal do ctrl
 * 
 * 
 */ 
int controlAlgorithm(void * input){
    struct sched_attr attrCTRL = {
        .size = sizeof (attrCTRL),
        .sched_policy = SCHED_DEADLINE,
        .sched_runtime = 10 * 1000 * 1000,
        .sched_period = 1 * 1000 * 1000 * 1000,
        .sched_deadline = 11 * 1000 * 1000
    };
 
	if (sched_setattr(getpid(), &attrCTRL, 0)){
		perror("sched_setattr()");
    }

    //processar input ???
    printf("Control Algorithm\n");
    //while(1)
    //buscar speed
    //computar thrust
    //guardar thrust para fmc
    //sleep periodo

    return 0;
}
