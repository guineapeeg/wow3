#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <string.h>

typedef struct{
    int bufferSize;
    int threadCount;
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

void* readFileThread(void * args){
    ThreadArgument *actualArguments = args;

    char* length = "/home/vboxuser/Desktop/C assignments/assignment3/input/file_1.txt";
    char result[strlen(length)];

    char* fileBeginning = "/home/vboxuser/Desktop/C assignments/assignment3/input/file_";
    char *fileToOpen = ".txt";

    sprintf(result, "%s%d", fileBeginning, actualArguments->threadCount);

    strcat(result, fileToOpen);

    printf("%s", result);

    

//////////////////////////////////
    // FILE *fptr;
    // fptr = fopen(result, "r");

    // int bufferSize = actualArguments->bufferSize;
    // int resRead;
    // char buffer[bufferSize];
    // char c;

    

    // if (fptr != NULL){

    //     do{

    //         for(int i = 0; i<bufferSize && c != EOF; i++){
    //         c = fgetc(fptr);
    //         buffer[i] = c;

    //         if(buffer[i] != '\n' && buffer[i] != EOF){
               
    //         }
    //         }
            

    //     }while(c != EOF);

        

    // }else{
    //     printf("not opened \n");
    // }

    // int newSampleValue;
/////////////////////////////////
    

    //reading with buffer

    //free(actualArguments);

    return NULL;

}

int main(){

    num_threads = 2;
    //number_of_input_files = 3;


    

    // pthread_t thread_id;
    // print("Before thread \n");
    // pthread_create(&thread_id. NULL, threadFunction, NULL);
    // pthread_join(thread_id, NULL);
    // printf("After thread");

    FILE * metadataFile;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    metadataFile = fopen(metadata_file_path, "r");
    read = getline(&line, &len, metadataFile);
    
    number_of_input_files = atoi(line);
    printf("Number of input files: %d", number_of_input_files);
    

    for(int i = 1; i <= num_threads; i++){
        
        pthread_t thread_id;

        ThreadArgument *args = malloc(sizeof *args);
        
        args->bufferSize = 2; 
        args->threadCount = i;

        //compare and swap
        //while(1){
            while(__sync_bool_compare_and_swap(&lock, 0, 1) != 0);
            //critical section
        // printf("Thread number %d", i);
        // pthread_create(&thread_id, NULL, readFileThread, args);
        // pthread_join(thread_id, NULL);
        // printf("After thread");
            for (int i2 = 0; i2 < 3; i2++){
                read = getline(&line, &len, metadataFile);
                printf("Printing from %d: %s", i, line);
            }


            //
            lock = 0;
        //}

        

        free(args);
    }

    //char myString[100];
    //fgets(myString, 100, fptr);

    // printf("%s", myString);

    //fclose(fpMetadata);

    //printf("hello");

    return 0;





}