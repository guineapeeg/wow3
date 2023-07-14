#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <string.h>
#include <ctype.h>

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
pthread_mutex_t globalLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

//for individual locks
pthread_mutex_t arraOfIndividualLocks[20];
float answerLinesForOutput[20];

int p;

void* readFileThread(void *args);

int loopStop = 0;

int biggestNumOfLines = 0;

int main(int argc, char* argv[]){

    setbuf(stdout, NULL);

    if(argc == 7){

    buffer_size = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    metadata_file_path = argv[3];
    lock_config = atoi(argv[4]);
    global_checkpointing = atoi(argv[5]);
    output_file_path = argv[6];

    }else{
    buffer_size = 3;
    num_threads = 4;
    metadata_file_path = "/home/vboxuser/Desktop/C assignments/chfgjkjkhf/wow3/metadata.txt";
    lock_config = 1;
    global_checkpointing = 0;
    output_file_path = "/home/vboxuser/Desktop/C assignments/chfgjkjkhf/wow3/output.txt";
    }

    remove(output_file_path);
    FILE *fpStart = fopen(output_file_path, "w");
    fclose(fpStart);

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

    loopStop = number_of_input_files;

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
    

    for(int tCounter = 0; tCounter < p; tCounter++){

        threadFile[tCounter] = fopen(actualArguments[tCounter].channelPath, "r");

    }
    char buffer[buffer_size];
    char c;
    
    //dynamic array 
    pthread_mutex_lock(&lock);
    float numbersFromEachFile[p][20];
    int counterForFloatPosition[p];
    pthread_mutex_unlock(&lock);
    

    int endOfFile[p];
    for(int p2 = 0; p2<p; p2++){
        endOfFile[p2] = 1;
    }

    for(int cc = 0; cc<p; cc++){
        counterForFloatPosition[cc] = 0;
    }

    //int bufferCounter = 0;

    if(global_checkpointing == 1){


    while(loopStop > 0){

        pthread_mutex_lock(&lock);

        if(globalCounter == actualArguments->assignedThread){

            //printf("Hello\n");

            for(int fileQ = 0; fileQ < p; fileQ++){//for each file assigned to this thread

                //printf("Hello\n");

                if(endOfFile[fileQ] == 1){//if file hasnot ended yet

                    //printf("Hello\n");

                    for(int i = 0; i < buffer_size; i++){
                       buffer[i] = '\0';
                    }

                    //printf("Hello\n");

                    for(int counter = 0; counter < buffer_size; counter++){ //filling in buffer

                        c = fgetc(threadFile[fileQ]);
                        //printf("Hello\n");

                        if(c == EOF){
                            //printf("Hello\n");
                            endOfFile[fileQ] = 0;
                            counter = buffer_size;
                            loopStop--;
                        }else{
                            //printf("Hello\n");
                            buffer[counter] = c;
                        }

                    }//filling in buffer

                    //printf("Hello\n");

                    char buffer2[buffer_size];

                    for(int i = 0; i < buffer_size; i++){
                       buffer2[i] = '\0';
                    }
                    int buffer2Count = 0;

                    int boolean = 0;
                    for(int i2 = 0; i2<buffer_size; i2++){
                        if(isdigit(buffer[i2])){
                            buffer2[buffer2Count] = buffer[i2];
                            buffer2Count++;
                            boolean = 1;
                        }
                        //boolean = 1;
                    }

                    //printf("Hello\n");

                    if(boolean){
                    //printf("Hello\n");
                    float testNum = atof(buffer2);
                    //printf("Read number: %s", buffer);
                    //printf("Read number: %f", testNum);
                    numbersFromEachFile[fileQ][counterForFloatPosition[fileQ]] = testNum;
                    //printf("Hello\n");
                    counterForFloatPosition[fileQ] = counterForFloatPosition[fileQ] + 1;

                    if(counterForFloatPosition[fileQ] > biggestNumOfLines){
                        biggestNumOfLines = counterForFloatPosition[fileQ];
                    }
                    //printf("%s \n", actualArguments[0].channelPath);
                    //printf("Recorded number: %f \n", numbersFromEachFile[fileQ][counterForFloatPosition[fileQ] - 1]);
                    }

                    



                }//if file hasnot ended yet

                
                
            }//for each file assigned to this thread

            globalCounter = (globalCounter + 1) % num_threads;
            pthread_mutex_unlock(&lock);

        }else
        {
            pthread_mutex_unlock(&lock);
        }

    }//big while loop
    }else{// if global checkpointing is not one

        while(loopStop > 0){

            for(int fileQ = 0; fileQ < p; fileQ++){//for each file assigned to this thread

                if(endOfFile[fileQ] == 1){//if file hasnot ended yet

                    for(int i = 0; i < buffer_size; i++){
                       buffer[i] = '\0';
                    }

                    for(int counter = 0; counter < buffer_size; counter++){ //filling in buffer

                        c = fgetc(threadFile[fileQ]);

                        if(c == EOF){
                            endOfFile[fileQ] = 0;
                            counter = buffer_size;
                            loopStop--;
                        }else{
                            buffer[counter] = c;
                        }

                    }//filling in buffer

                    char buffer2[buffer_size];

                    for(int i = 0; i < buffer_size; i++){
                       buffer2[i] = '\0';
                    }
                    int buffer2Count = 0;

                    int boolean = 0;
                    for(int i2 = 0; i2<buffer_size; i2++){
                        if(isdigit(buffer[i2])){
                            buffer2[buffer2Count] = buffer[i2];
                            buffer2Count++;
                            boolean = 1;
                        }
                    }

                    if(boolean){
                    float testNum = atof(buffer2);
                    numbersFromEachFile[fileQ][counterForFloatPosition[fileQ]] = testNum;
                    counterForFloatPosition[fileQ] = counterForFloatPosition[fileQ] + 1;
                    }


                }//if file hasnot ended yet            
            }//for each file assigned to this thread

        }


    }
    //counting after reading from channels
    pthread_mutex_lock(&lock);

    

    // //alpha counting
    // printf("\n Alpha: %f \n", actualArguments->amplification);
    // printf("\n Alpha counting: \n");

    

    //number arrays are in n
    for(int fileQueued = 0; fileQueued < p; fileQueued++){
        int size = counterForFloatPosition[fileQueued];
        for(int innerCounter = 1; innerCounter < size; innerCounter++){

            float newValue;
            float newValue2;

            float alpha = actualArguments->amplification;
            newValue = alpha * numbersFromEachFile[fileQueued][innerCounter];
            newValue2 = (1-alpha) * numbersFromEachFile[fileQueued][innerCounter-1];
            newValue = newValue + newValue2;
            numbersFromEachFile[fileQueued][innerCounter] = newValue;
        }

    }   

    // for(int fileQueued = 0; fileQueued < p; fileQueued++){
    //     int size = counterForFloatPosition[fileQueued];
    //     for(int innerCounter = 0; innerCounter < size; innerCounter++){

    //         printf("%f \n", numbersFromEachFile[fileQueued][innerCounter]);
    //     }

    // }

    // printf("\n Beta: %f \n", actualArguments->low_pass_filter_value);
    // printf("\n Beta counting: \n");

    for(int fileQueued = 0; fileQueued < p; fileQueued++){
        int size = counterForFloatPosition[fileQueued];
        for(int innerCounter = 0; innerCounter < size; innerCounter++){

            float newValue;

            float beta = actualArguments->low_pass_filter_value;
            newValue = beta * numbersFromEachFile[fileQueued][innerCounter];
            numbersFromEachFile[fileQueued][innerCounter] = newValue;
        }

    }

    

    for(int fileQueued = 0; fileQueued < p; fileQueued++){
        printf("%s \n", actualArguments->channelPath);
        int size = counterForFloatPosition[fileQueued];
        for(int innerCounter = 0; innerCounter < size; innerCounter++){

            printf("%f \n", numbersFromEachFile[fileQueued][innerCounter]);
        }

    }

    pthread_mutex_unlock(&lock);

    //OUTPUTTING THE RESULT
    ///
    ///
    ///
    ///
    if(lock_config == 1){
    pthread_mutex_lock(&globalLock);
    

    for(int fileQueued = 0; fileQueued < p; fileQueued++){ //for each file this thread handles
        FILE *outputFile = fopen(output_file_path, "r");
        FILE *fTemp;

        char buff[50];
        char buff2[50];

        fTemp = fopen("replace.tmp", "w");

        float tempF = 0;     

        int size = counterForFloatPosition[fileQueued]; //how many floats we extracted from a file      
        int counting = 0;

        while (fgets(buff, 50, outputFile) != NULL)
        {

            if(counting < size){

                tempF = atof(buff);
                tempF = tempF + numbersFromEachFile[fileQueued][counting];
                sprintf(buff2, "%f", tempF);
                fputs(buff2, fTemp);
                putc('\n', fTemp);
                counting++;

            }else{
                fputs(buff, fTemp);
                //putc('\n', fTemp);
            }
            
        }

        while(counting < size){

            tempF = numbersFromEachFile[fileQueued][counting];
            sprintf(buff2, "%f", tempF);
            fputs(buff2, fTemp);
            putc('\n', fTemp);
            counting++;

        }

        fclose(fTemp);
        fclose(outputFile);
        

        remove(output_file_path);
        rename("replace.tmp", "output.txt");

    }   

    pthread_mutex_unlock(&globalLock);
    }

    if(lock_config == 2){

        for(int fileQueued = 0; fileQueued < p; fileQueued++){
        int size = counterForFloatPosition[fileQueued];
        for(int innerCounter = 0; innerCounter < size; innerCounter++){

            pthread_mutex_lock(&arraOfIndividualLocks[innerCounter]);

            answerLinesForOutput[innerCounter] = answerLinesForOutput[innerCounter] + numbersFromEachFile[fileQueued][innerCounter];

            pthread_mutex_unlock(&arraOfIndividualLocks[innerCounter]);
            
        }//dff

    }


    }

    //exit(0);


    return 0;

    

}