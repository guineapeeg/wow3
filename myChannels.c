#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <string.h>

typedef struct{
    char channelPath[20];
    float low_pass_filter_value;
    float amplification;
    int localDone;
} ThreadArgument;

int buffer_size;
int num_threads;
char *metadata_file_path = "/home/vboxuser/Desktop/C assignments/assignment3/metadata.txt";
int lock_config;
int global_checkpointing;
//temporary
char *output_file_path = "/home/vboxuser/Desktop/C assignments/assignment3/output.txt";



int number_of_input_files;
int bufferSize = 2;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;

int done;


void* readFileThread(void *args);

int main(){

    num_threads = 4;
    number_of_input_files = 4;

    int p = number_of_input_files / num_threads;

    FILE * metadataFile;
    char *line = NULL;
    size_t len = 0;
    //ssize_t read;
    metadataFile = fopen(metadata_file_path, "r");
    //read = 
    getline(&line, &len, metadataFile);


    
    

    char firstNum[20];
    char secondNum[20];

    int k = 0;

    ThreadArgument filesInformation[number_of_input_files];
    for(int m = 0; m < number_of_input_files; m++)
    {
        fscanf(metadataFile, "%s %s %s", filesInformation[m].channelPath, firstNum, secondNum);
        filesInformation[m].low_pass_filter_value = atof(firstNum);
        filesInformation[m].amplification = atof(secondNum);
        filesInformation[m].localDone = k;
        k++;
    }


    pthread_t tid[num_threads];
    ThreadArgument allOfThreads[num_threads][p];
    

    for(int i = 0; i < num_threads; i++){
        
        int threadArgCount = 0;

        for(int ic = i; ic < number_of_input_files; ic = ic + num_threads){
            
            
            allOfThreads[i][threadArgCount] = filesInformation[ic];
            //trying
            allOfThreads[i][threadArgCount].localDone = i;
            //tryinggggggg
            threadArgCount++;
        }

        
        pthread_create(&(tid[i]), NULL, readFileThread, allOfThreads[i]); 

    }

    

    while(1){
    }

    return 0;

}

int globalCounter = 0;
int miniCounter = 0;

void* readFileThread(void * args){

    ThreadArgument *actualArguments = args;

    FILE *threadFile;
    threadFile = fopen(actualArguments->channelPath, "r");

    //int resRead;
    char buffer[bufferSize];
    char c;

    c = fgetc(threadFile);

    // acquire a lock

    while(1){ //while c != EOF
    pthread_mutex_lock(&lock);
    if(globalCounter == actualArguments->localDone){

        //buffer[0] = '\0';
        
        for(int counter = 0; counter < bufferSize && c != EOF; counter++){
         
        
        buffer[counter] = c;
        c = fgetc(threadFile);

        if(c == EOF){

            printf("Buffer with 2 bytes: from %s ------> %s \n",actualArguments->channelPath, buffer);

        }
        }

        if(c != EOF){
        printf("Buffer with 2 bytes: from %s ------> %s \n",actualArguments->channelPath, buffer);
        }

        globalCounter = (globalCounter + 1) % num_threads;
        pthread_cond_signal(&cond1);

        
    }else{
        //pthread_mutex_unlock(&lock);
        pthread_cond_wait(&cond1, &lock);
        

        }
        pthread_mutex_unlock(&lock);
    }

    // while(1){
    //     if(globalCounter == actualArguments->localDone){

    //         globalCounter = (globalCounter + 1) % num_threads;
    //         pthread_cond_signal(&cond1);
    //     }else{
    //         pthread_cond_wait(&cond1, &lock);
    //     }

        

    // }

    
                   
    // Finally release mutex

        
   
        return NULL;


}