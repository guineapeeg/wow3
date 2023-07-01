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
char *metadata_file_path;
int lock_config;
int global_checkpointing;
char *output_file_path;

void* readFileThread(void * args){
    ThreadArgument *actualArguments = args;

    char* length = "/home/vboxuser/Desktop/C assignments/assignment3/input/file_1.txt";
    char result[strlen(length)];

    char* fileBeginning = "/home/vboxuser/Desktop/C assignments/assignment3/input/file_";
    char *fileToOpen = ".txt";

    //char buf[strlen(length)];
    sprintf(result, "%s%d", fileBeginning, actualArguments->threadCount);

    strcat(result, fileToOpen);

    printf("%s", result);

    FILE *fptr;
    fptr = fopen(result, "r");

    //unsigned char data;
    int bufferSize = actualArguments->bufferSize;
    int resRead;
    char buffer[bufferSize];
    char c;

    if (fptr != NULL){

        for(int i = 0; i<bufferSize; i++){
            c = fgetc(fptr);
            buffer[i] = c;
            printf("%c \n", buffer[i]);
        }

    }else{
        printf("not opened \n");
    }
    

    //reading with buffer

    //free(actualArguments);

    return NULL;

}

int main(){

    num_threads = 2;

    // pthread_t thread_id;
    // print("Before thread \n");
    // pthread_create(&thread_id. NULL, threadFunction, NULL);
    // pthread_join(thread_id, NULL);
    // printf("After thread");

    FILE* fp;
    fp = fopen("metadata.txt", "w");
    fputs("2", fp);    
    

    for(int i = 1; i <= num_threads; i++){
        
        pthread_t thread_id;

        ThreadArgument *args = malloc(sizeof *args);
        
        args->bufferSize = 1; 
        args->threadCount = i;

        printf("Thread number %d", i);
        pthread_create(&thread_id, NULL, readFileThread, args);
        pthread_join(thread_id, NULL);
        printf("After thread");

        free(args);
    }

    //char myString[100];
    //fgets(myString, 100, fptr);

    // printf("%s", myString);

    fclose(fp);

    //printf("hello");

    return 0;





}