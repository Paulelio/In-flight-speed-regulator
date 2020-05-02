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

}
/** funcao para escrever os records para um ficheiro csv
 * a estrutar do ficheiro e do tipo <timestamp,speed,thrust>
 * 
 * 
 * 
 */

void writeToRecord(time_t timestamp, double speed, double thrust){

    fileRecord = fopen("/tmp/fdrBlackbox.csv", "a");
    fprintf(fileRecord, "%s, %s, %s\n", timestamp, speed, thrust);
    fclose(fileRecord);
}