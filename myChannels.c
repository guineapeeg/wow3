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

void* readFileThread(void * args){

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

    return NULL;

}

int main(){

    num_threads = 2;
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

    
    

    for(int i = 1; i <= num_threads; i++){
        
        pthread_t thread_id;

        ThreadArgument *args = malloc((sizeof *args) * p);

        ThreadArgument workloadThread[p];

        int threadArgCount = 0;

        for(int ic = i-1; ic < number_of_input_files; ic = ic + num_threads){
            workloadThread[threadArgCount] = filesInformation[ic];
            threadArgCount++;
        }

        for(int m = 0; m < p; m++)
        {
        printf("%d struct number: %f %s %f", m, workloadThread[m].amplification, workloadThread[m].channelPath, workloadThread[m].low_pass_filter_value);

        }

        


        //while(1){
        // while(__sync_bool_compare_and_swap(&lock, 0, 1) != 0);
        //     //critical section
    

        

        // pthread_create(&thread_id, NULL, readFileThread, args);
        // pthread_join(thread_id, NULL);

        //     lock = 0;
        //}

        

        free(args);
    }

    return 0;


}