#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <string.h>

typedef struct{
    char channelPath[20];
    float low_pass_filter_value;
    float amplification;
} ThreadArgument;

int buffer_size;
int num_threads;
char *metadata_file_path = "/home/vboxuser/Desktop/C assignments/assignment3/metadata.txt";
int lock_config;
int global_checkpointing;
//temporary
char *output_file_path = "/home/vboxuser/Desktop/C assignments/assignment3/output.txt";

FILE* fpMetadata;

int number_of_input_files;


int lock = 0;

int bufferSize = 2;



int test;

void* readFileThread2(void * args){

    ThreadArgument *actualArguments = args;

//////////////////////////////////
    FILE *fptr;
    fptr = fopen(actualArguments->channelPath, "r");
    //fptr = fopen("input/file_1.txt", "r");

    //printf("This: %s", actualArguments->channelPath);
    
    int resRead;
    char buffer[bufferSize];
    char c;

    for(int counter = 0; counter < bufferSize; counter++){
        c = fgetc(fptr);
        //printf("%c", c);
        buffer[counter] = c;
    }

    // if (fptr != NULL){
    //     do{
            // for(int i = 0; i<bufferSize && c != EOF; i++){
            // c = fgetc(fptr);
            // buffer[i] = c;


    //     }
    // }while(c != EOF);
    // }else{
    //     printf("Error \n");
    // }

    printf("%s", buffer);


            // if(buffer[i] != '\n' && buffer[i] != EOF){
               
            // }
            // }
            


    // int newSampleValue;
/////////////////////////////////
    

    //reading with buffer

    //free(actualArguments);

    //return NULL;

}

pthread_mutex_t lockSchedule = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;


int value = 1;


void* readFileThread(void * args){ 

    ThreadArgument *actualArguments = args;

    while(1){
        pthread_mutex_lock(&lock);
        
    }


    // while(1){
    // pthread_mutex_lock(&lockSchedule);
    // pthread_cond_wait(&cond1, &lockSchedule);

    //unsigned long i = 0;
    //test += 1;

    //printf("From thread %s, started %d \n", actualArguments->channelPath, test); 
    
    //for(i = 0; i<(0xFFFFFFFF); i++);

    //printf("\n Job finished %s: %d", actualArguments->channelPath, test);
    // printf("hello");

    // pthread_cond_signal(&cond2);


    // }   

    // pthread_mutex_unlock(&lockSchedule);



}


int main(){

    num_threads = 4;
    number_of_input_files = 4;

    FILE * metadataFile;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    metadataFile = fopen(metadata_file_path, "r");
    read = getline(&line, &len, metadataFile);
    
    //number_of_input_files = atoi(line); dont remove
    //each thread works on p files

    
    int p = number_of_input_files / num_threads;

    char firstNum[20];
    char secondNum[20];

    ThreadArgument filesInformation[number_of_input_files];
    for(int m = 0; m < number_of_input_files; m++)
    {
        fscanf(metadataFile, "%s %s %s", filesInformation[m].channelPath, firstNum, secondNum);
        filesInformation[m].low_pass_filter_value = atof(firstNum);
        filesInformation[m].amplification = atof(secondNum);
    }


    pthread_t tid[num_threads];

    //ThreadArgument *workloadThread = malloc((sizeof workloadThread) * i);
    //creating thread
    ThreadArgument allOfThreads[num_threads][p];
    

    for(int i = 0; i < num_threads; i++){
        
        //ThreadArgument workloadThread[p] ;
        //ThreadArgument *workloadThread = allOfThem[i-1];
        

        int threadArgCount = 0;

        for(int ic = i; ic < number_of_input_files; ic = ic + num_threads){
            allOfThreads[i][threadArgCount] = filesInformation[ic];
            threadArgCount++;
        }

        
        pthread_create(&(tid[i]), NULL, readFileThread, allOfThreads[i]); //workloadThread
        //pthread_join(tid[i], NULL);

        if(i == 0){
            pthread_cond_signal(&cond1);
        }
        

    }

    

    while(1){
    }

    //pthread_join(tid[0], NULL);
    //pthread_join(tid[1], NULL);
    //pthread_join(tid[2], NULL);
    //pthread_join(tid[3], NULL);

    //free(workloadThread);

    return 0;


}