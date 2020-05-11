/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/
#include <time.h>
#include <stdio.h>

#include "fdr.h"
#include "fmc.h"

FILE *fileRecord;



/** Funcao principal do FDR
 * 
 * 
 */ 
int flightDataRecorder(void * input){
    printf("o input e este \n");
    return 0;
}
/** funcao para escrever os records para um ficheiro csv
 * a estrutar do ficheiro e do tipo <timestamp,speed,thrust>
 * 
 * 
 * 
 */

void writeToRecord(time_t timestamp, double speed, double thrust){

    fileRecord = fopen("/tmp/fdrBlackbox.csv", "a");
    fprintf(fileRecord, "%ld, %f, %f\n", timestamp, speed, thrust);
    fclose(fileRecord);

    return;
}
