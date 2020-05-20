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
#include <semaphore.h>
#include <linux/sched.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <fcntl.h> 

#include "fmc.h"
#include "ctrl.h"
#include "fdr.h"

sem_t *semSpeed;
sem_t *semThrust;

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
	
	//FAZER AQUI A CRIAÇÃO DA SHARED MEMORY
	semSpeed = sem_open("sem_Speed", O_CREAT);
    semThrust = sem_open("sem_Thrust", O_CREAT);
	// NAO SE PODE FAZER SHARED MEMORY DEPOIS DESTE COMENTARIO
	mlockall(MCL_FUTURE || MCL_CURRENT); // TEMPORARIO!!!!! comeca o bloqueio de parte ou todos os virtual address space dos processos para a RAM 
	
	//provavelmente so o corrente -- verificar na doc
	struct aviao_t* aviao = (struct aviao_t*) malloc(sizeof(struct aviao_t));
	aviao->altitude = altitude;
	aviao->vel_init = vel_init;
	aviao->vel_final = vel_final;

	/**
	 * Criacao de threads para correrem as
	 * varias atividades
	 * 
	 */
	pthread_t fmc_thread, ctrl_thread, fdr_thread;

	

	//thread do Control Algorithm-RT
	pthread_create(&ctrl_thread, NULL, (void*) &controlAlgorithm, (void *) (intptr_t) vel_final);
	//thread do Control Algorithm-NRT para teste
	//pthread_create(&ctrl_thread, NULL, (void*) &controlAlgorithmNRT, NULL);

	//thread do Flight Management Computer
	pthread_create(&fmc_thread, NULL, (void*) &flightManagement, (void *) aviao);
	//adicionar argumentos de inicializacao - altitude e velocidades

	//thread do Flight Data Recorder
	pthread_create(&fdr_thread, NULL, (void*) &flightDataRecorder, NULL);
	


	pthread_join(fmc_thread, NULL); //-- ver exemplo nos slides
	pthread_join(ctrl_thread, NULL);
	pthread_join(fdr_thread, NULL);

	free(aviao);
}

