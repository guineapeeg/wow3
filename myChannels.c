#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <string.h>

typedef struct{
    char* channelPath;
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

void* readFileThread(void * args){

    ThreadArgument *actualArguments = args;

//////////////////////////////////
    FILE *fptr;
    fptr = fopen(actualArguments->channelPath, "r");
    //fptr = fopen("input/file_1.txt", "r");

    printf("This: %s", actualArguments->channelPath);
    
    int resRead;
    char buffer[bufferSize];
    char c;

    if (fptr != NULL){
        do{
            for(int i = 0; i<bufferSize && c != EOF; i++){
            c = fgetc(fptr);
            buffer[i] = c;
        }
    }while(c != EOF);
    }else{
        printf("Error \n");
    }

    printf("%s", buffer);


            // if(buffer[i] != '\n' && buffer[i] != EOF){
               
            // }
            // }
            


    // int newSampleValue;
/////////////////////////////////
    

    //reading with buffer

    //free(actualArguments);

    return NULL;

}

int main(){

    num_threads = 3;

    FILE * metadataFile;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    metadataFile = fopen(metadata_file_path, "r");
    read = getline(&line, &len, metadataFile);
    
    number_of_input_files = atoi(line);
    printf("Number of input files: %d", number_of_input_files);
    //each thread works on p files

    int p = number_of_input_files / num_threads;

    char test[20];
    char fileN[20];
    char firstNum[20];
    char secondNum[20];
    

    for(int i = 1; i <= num_threads; i++){
        
        pthread_t thread_id;

        ThreadArgument *args = malloc(sizeof *args);

        //while(1){
        while(__sync_bool_compare_and_swap(&lock, 0, 1) != 0);
            //critical section
        
        //read = getline(&line, &len, metadataFile);
        // while(fgets(test, 20, metadataFile) != NULL){

        // }

        fscanf(metadataFile, "%s %s %s", fileN, firstNum, secondNum);

        printf("Printing from %d, fileName: %s", i, fileN);

        
        // args->channelPath = *line;
        // printf("Printing from %d: %s", i, args->channelPath);

        // read = getline(&line, &len, metadataFile);
        // args->low_pass_filter_value = atof(line);
        // printf("Printing from %d: %d", i, args->low_pass_filter_value);

        // read = getline(&line, &len, metadataFile);
        // args->amplification = atof(line);
        // printf("Printing from %d: %s", i, args->channelPath);

        // printf("Thread number %d", i);
        // pthread_create(&thread_id, NULL, readFileThread, args);
        // pthread_join(thread_id, NULL);
        // printf("After thread");


            //
            lock = 0;
        //}

        

        free(args);
    }

    return 0;


}