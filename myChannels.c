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

int p;

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

    FILE * metadataFile;
    char *line = NULL;
    size_t len = 0;
    metadataFile = fopen(metadata_file_path, "r");
    getline(&line, &len, metadataFile);
    number_of_input_files = atoi(line);

    p = number_of_input_files / num_threads; //files per thread

    //constraints for provided floats
    char firstNum[10];
    char secondNum[10];

    ThreadArgument filesInformation[number_of_input_files];
    for(int m = 0; m < number_of_input_files; m++)
    {
        fscanf(metadataFile, "%s %s %s", filesInformation[m].channelPath, firstNum, secondNum);
        filesInformation[m].low_pass_filter_value = atof(firstNum);
        filesInformation[m].amplification = atof(secondNum);
    }


    pthread_t tid[num_threads];
    ThreadArgument allOfThreads[num_threads][p];
    

    //thread creation
    for(int i = 0; i < num_threads; i++){
        
        int threadArgCount = 0;

        for(int ic = i; ic < number_of_input_files; ic = ic + num_threads){
                      
            allOfThreads[i][threadArgCount] = filesInformation[ic];
            allOfThreads[i][threadArgCount].assignedThread = i;
            threadArgCount++;

        }
        
        pthread_create(&(tid[i]), NULL, readFileThread, allOfThreads[i]); 

    }

    while(1){
    }

    return 0;

}

int globalCounter = 0;

void* readFileThread(void * args){

    ThreadArgument *actualArguments = args;

    FILE *threadFile[p];
    char c;

    for(int tCounter = 0; tCounter < p; tCounter++){

        threadFile[tCounter] = fopen(actualArguments[tCounter].channelPath, "r");

    }
    char buffer[2];
    
    //dynamic int array 
    int b = 50;
    int* integers = (int*) malloc(b * sizeof(int));
    int integerCounter = 0;

    int continueRead = 1;

    while(1){

        pthread_mutex_lock(&lock);
        if(globalCounter == actualArguments->assignedThread){

            for(int mCounter = 0; mCounter < p; mCounter++){

                for(int i = 0; i < buffer_size; i++){

                buffer[i] = '\0';

                }

                c = fgetc(threadFile[mCounter]);
                buffer[0] = c;

                for(int counter = 1; counter < buffer_size && c != EOF; counter++){ 

                    
                    c = fgetc(threadFile[mCounter]);
                    buffer[counter] = c;


                    if(c == EOF){

                        int testNum = atoi(buffer);
                        printf("Integer form: %d", testNum);

                    integers[integerCounter] = testNum;
                    integerCounter++;
                    printf("Buffer %s: from %s ------>\n ", actualArguments[mCounter].channelPath, buffer);
                    continueRead = 0;

                   }

                }

                if( c!= EOF){

                    int testNum = atoi(buffer);
                    integers[integerCounter] = testNum;
                    integerCounter++;
                    printf("Integer form: %d", testNum);
                    printf("Buffer %s: from %s ------>\n", actualArguments[mCounter].channelPath, buffer);

                }

            }

            globalCounter = (globalCounter + 1) % num_threads;
            pthread_mutex_unlock(&lock);

        }else{
            pthread_mutex_unlock(&lock);
        }

        if(continueRead == 0){

            printf("Printing from thread %s whole array: \n", actualArguments->channelPath);

            for(int moor = 0; moor<= integerCounter; moor++){
                printf("%d \n", integers[moor]);
                 }

        }

    }

    

}