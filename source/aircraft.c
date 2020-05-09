/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/

/**
Funcao main que inicializa e coordena o sistema
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h> 
#include <sys/mman.h>
#include <sched.h>

#include "fmc.h"
#include "ctrl.h"
#include "fdr.h"

/** Tabela 1
 * Valores minimos e maximos para as variaveis
 *	|     Name      | Minimum | Maximum | Units |
 *  |---------------|---------|---------|-------|
 * 	| Engine thrust | 0       | 242     | kN    |
 * 	| Speed         | 300     | 823     | km/h  |
 * 	| Altitude      | 1000    | 12500   | m     |
 * 	Peso = 79000 kg
 */


/** Funcao para verificar se a velocidade estah dentro dos limites
 * Parameters: v - velocidade
 * Return: true if 300 <= v <= 823, false otherwise
 */
bool verifySpeed(int v){
	if(v < 300 || v > 823){
		return false;
	}
	return true;
}

/** Funcao para verificar se a altitude estah dentro dos limites
 * Parameters: h - altitude
 * Return: true if 300 <= v <= 823, false otherwise
 */
bool verifyHeight(int h){
	if(h < 1000 || h > 12500){
		return false;
	}
	return true;
}

int main(int argc, char** argv) {

	if(argc != 4){
		printf("Uso: ./aircraft <altitude> <velocidade_init> <velocidade_final>\n");
		printf("Exemplo: ./aircraft 4000 500 600\n");
		return -1;
	}

	//explicar no relatorio que estamos a trabalhar com inteiros apenas

	//data verifiers
	int altitude = atoi(argv[1]);
	int vel_init = atoi(argv[2]);
	int vel_final = atoi(argv[3]);
	
	if(!verifyHeight(altitude)){
		printf("Altitude entre 1000 e 12500.\n");
		return -1;
	}

	if(!verifySpeed(vel_init) || !verifySpeed(vel_final)){
		printf("Velocidades entre 300 e 823.\n");
		return -1;
	}

	mlockall(MCL_FUTURE || MCL_CURRENT); // TEMPORARIO!!!!! comeca o bloqueio de parte ou todos os virtual address space dos processos para a RAM 
	//provavelmente so o corrente -- verificar na doc
	struct aviao_t* aviao = (struct aviao_t*) malloc(sizeof(struct aviao_t));
	aviao->altitude = altitude;
	aviao->vel_init = vel_init;
	aviao->vel_final = vel_final;
	printf("passei o struct\n");
	
	
	
	/**
	 * Criacao de threads para correrem as
	 * varias atividades
	 * 
	 */
	pthread_t fmc_thread, ctrl_thread, fdr_thread;
	//typedef struct sched_attr attr;
	struct sched_attr attr;
	attr.sched_policy = SCHED_DEADLINE;

	//thread do Flight Management Computer
	attr.sched_runtime = 30000000;
	attr.sched_period = 100000000;
	attr.sched_deadline = attr.sched_period;
		
	attr->size = sizeof(struct attr);
	printf("Debug attributes %d %d %d %d",attr->sched_runtime, attr->sched_period, attr->sched_deadline, attr->size);

	if (sched_setattr(fmc_thread, &attr, 0))
		perror("sched_setattr()");

	pthread_create(&fmc_thread, NULL, &flightManagement, (void *) aviao);
	//adicionar argumentos de inicializacao - altitude e velocidades

	//thread do Control Algorithm
	attr.sched_runtime = 30000000;
	attr.sched_period = 100000000;
	attr.sched_deadline = attr.sched_period;
 
	if (sched_setattr(ctrl_thread, &attr, 0))
		perror("sched_setattr()");

	pthread_create(&ctrl_thread, NULL, &controlAlgorithm, NULL);
	
	//thread do Flight Data Recorder
	attr.sched_runtime = 30000000;
	attr.sched_period = 100000000;
	attr.sched_deadline = attr.sched_period;

	if (sched_setattr(fdr_thread, &attr, 0))
		perror("sched_setattr()");

	pthread_create(&fdr_thread, NULL, &flightDataRecorder, NULL);
	
	pthread_join(fmc_thread, NULL); //-- ver exemplo nos slides
	pthread_join(ctrl_thread, NULL);
	pthread_join(fdr_thread, NULL);


	free(aviao);
}

