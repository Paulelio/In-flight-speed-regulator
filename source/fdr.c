/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/
#include <time.h>
#include <stdio.h>
#include <sys/ipc.h> 
#include <sys/msg.h>

#include "fdr.h"
#include "fmc.h"

FILE *fileRecord;

struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[100]; 
} message; 

/** Funcao principal do FDR
 * 
 * 
 */ 
int flightDataRecorder(void * input){
    printf("Flight Data Recorder \n");
    printf("A ler do FMC");
    //read message
    
    key_t key; 
    int msgid; 
    
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
  
    // msgrcv to receive message 
    msgrcv(msgid, &message, sizeof(message), 1, 0); 
  
    // display the message 
    printf("Data Received is : %s \n",  
                    message.mesg_text); 
  
    // to destroy the message queue 
    msgctl(msgid, IPC_RMID, NULL); 

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
