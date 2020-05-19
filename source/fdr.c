/**
Grupo 1
Diogo Fernandes 49992
Paulo Alvares 49460
*/
#include <time.h>
#include <stdio.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <unistd.h>

#include "fdr.h"
#include "fmc.h"

FILE *fileRecord;

struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[1024]; 
} message; 

/** Funcao principal do FDR
 * 
 * 
 */ 
int flightDataRecorder(void * input){
    printf("[FDR] Flight Data Recorder \n");
    printf("[FDR] A ler do FMC\n");
    //read message
    
    key_t key; 
    int msgid; 
    
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
    

    
    
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
    // CICLO PARA ESTAR SEMPRE A ESCUTA DA MENSAGEM
    for(;;){
        // msgrcv to receive message 
        msgrcv(msgid, &message, sizeof(message), 1, 0); 
        writeToRecord(message.mesg_text);
        //while msgrcv != 0 (message.text).split(',') e write to record.
        printf("[FDR] escrevi para ficheiro\n");
        // display the message 
        printf("[FDR] Dados Recebidos: %s \n",  
                    message.mesg_text);
        sleep(10);
    }
     
  
    // to destroy the message queue 
    msgctl(msgid, IPC_RMID, NULL); 

    return 0;
}
/** funcao para escrever os records para um ficheiro csv
 * a estrutar do ficheiro e do tipo <timestamp,speed,thrust>
 */
// FALTA DAR SPLIT DA MENSAGEM 
void writeToRecord(char* recMesg){

    fileRecord = fopen("/tmp/fdrBlackbox.csv", "a");
    fprintf(fileRecord, "%s\n", recMesg);
    fclose(fileRecord);

    return;
}
