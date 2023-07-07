#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <string.h>

typedef struct{
    char channelPath[20];
    float low_pass_filter_value;
    float amplification;
    int assignedThread;
} ThreadArgument;

int buffer_size;
int num_threads;
char *metadata_file_path;
int lock_config;
int global_checkpointing;
char *output_file_path;

int number_of_input_files;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

void* readFileThread(void *args);

int main(int argc, char* argv[]){

    if(argc == 7){

    buffer_size = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    metadata_file_path = argv[3];
    lock_config = atoi(argv[4]);
    global_checkpointing = atoi(argv[5]);
    output_file_path = argv[6];

    }else{
    buffer_size = 2;
    num_threads = 4;
    metadata_file_path = "/home/vboxuser/Desktop/C assignments/assignment3/metadata.txt";
    lock_config = 1;
    global_checkpointing = 0;
    output_file_path = "/home/vboxuser/Desktop/C assignments/assignment3/output.txt";
    }

    //number_of_input_files = 4;

    

    FILE * metadataFile;
    char *line = NULL;
    size_t len = 0;
    //ssize_t read;
    metadataFile = fopen(metadata_file_path, "r");
    //read = can be used for read line 
    getline(&line, &len, metadataFile);
    number_of_input_files = atoi(line);
    printf("number of input files read: %d", number_of_input_files);
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
    ThreadArgument allOfThreads[num_threads][p];
    

    for(int i = 0; i < num_threads; i++){
        
        int threadArgCount = 0;

        for(int ic = i; ic < number_of_input_files; ic = ic + num_threads){
            
            
            allOfThreads[i][threadArgCount] = filesInformation[ic];
            //trying
            allOfThreads[i][threadArgCount].assignedThread = i;
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
    
    char c;

    c = fgetc(threadFile);

    // acquire a lock
    char buffer[2];

    while(1){ //while c != EOF

    pthread_mutex_lock(&lock);
    //pthread_cond_wait(&cond1, &lock);
    //printf("In thread %s \n", actualArguments->channelPath);
    if(globalCounter == actualArguments->assignedThread){
        //strcpy(buffer, "");
        //buffer[1] = '0';
        
        //strcpy(buffer, "");
        buffer[0] = '\0';
        buffer[1] = '\0';

        
        
        for(int counter = 0; counter < buffer_size && c != EOF; counter++){
         
        
        buffer[counter] = c;
        c = fgetc(threadFile);

        if(c == EOF){

            printf("Buffer with 2 bytes: from %s ------> %s , global Counter: %d \n",actualArguments->channelPath, buffer, globalCounter);

        }
        }

        if(c != EOF){
        printf("Buffer with 2 bytes: from %s ------> %s, global Counter: %d \n",actualArguments->channelPath, buffer, globalCounter);
        }

        

        globalCounter = (globalCounter + 1) % num_threads;
        //pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);

        
    }else{
        //pthread_mutex_unlock(&lock);
        pthread_mutex_unlock(&lock);
    
        }
        
    }

}