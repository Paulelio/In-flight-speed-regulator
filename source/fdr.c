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
#include <string.h>
#include <stdlib.h>

#include "fdr.h"

FILE *fileRecord;

struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[1024]; 
} message; 

/** funcao para escrever os records para um ficheiro csv
 * a estrutar do ficheiro e do tipo <timestamp,speed,thrust>
 */
// FALTA DAR SPLIT DA MENSAGEM 
void writeToRecord(char recMesg[]){

    fileRecord = fopen("fdrBlackbox.csv", "a");
    fprintf(fileRecord, "%s\n", recMesg);
    printf("[FDR] - recebido: %s\n",recMesg);
    fclose(fileRecord);

    return;
}

/** Funcao principal do FDR
 * 
 * 
 */ 
int flightDataRecorder(void * input){
    printf("[FDR] Flight Data Recorder \n");
    printf("[FDR] A ler do FMC\n");
    //read message
    
    key_t keyFDR; 
    int msgid; 
    
    // ftok to generate unique key 
    keyFDR = ftok("progfile", 65); 
    
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(keyFDR, 0666 | IPC_CREAT); 
    char *last_mesg = (char *) malloc(1024);

    // CICLO PARA ESTAR SEMPRE A ESCUTA DA MENSAGEM
    
    // msgrcv to receive message 
    //msgrcv(msgid, &message, sizeof(message), 1, 0); 
    //sleep(10);
    while(msgrcv(msgid, &message, sizeof(message), 1, 0) != 0){
        //write to fdrBlackbox
        if(strcmp(message.mesg_text, last_mesg) != 0){
            writeToRecord(message.mesg_text);
            printf("last message: %s\n", last_mesg);
            printf("messafe.mesg_text %s\n", message.mesg_text);
            strncpy(last_mesg, message.mesg_text, 1024);
            // display the message 
            printf("[FDR] Dados Recebidos: %s \n", message.mesg_text);
        } 
        else{
            sleep(3);
        }            
    } 
    //printf("[FDR] escrevi para ficheiro\n");z
    
     // to destroy the message queue 
    msgctl(msgid, IPC_RMID, NULL); 
    
    return 0;
}

