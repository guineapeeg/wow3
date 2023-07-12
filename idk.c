while(loopStop > 0){

    pthread_mutex_lock(&lock);

    if(globalCounter == actualArguments->assignedThread){

            for(int mCounter = 0; mCounter < p; mCounter++){

                for(int i = 0; i < buffer_size; i++){
                buffer[i] = '\0';
                }

                for(int counter = 0; counter < buffer_size && c != EOF; counter++){  //


                    buffer[counter] = c;
                    c = fgetc(threadFile[mCounter]);                    

                    if(c == EOF){
                    int boolean = 0;
                    for(int i2 = 0; i2<buffer_size; i2++){
                        if(isdigit(buffer[i2])){
                            boolean = 1;
                            i2 = 100;
                        }
                    }

                    if(boolean){
                    float testNum = atof(buffer);
                    floats[floatCounter] = testNum;
                    floatCounter++;
                    }

                    //counter = buffer_size + 10;
                    loopStop--;
                    printf("Loop stop: %d", loopStop);

                   }else{
                    buffer[counter] = c;
                   }

            }

            if(c != EOF){

                int boolean = 0;
            for(int i2 = 0; i2<buffer_size; i2++){
            if(isdigit(buffer[i2])){
                boolean = 1;
                i2 = 100;
            }
            


            if(boolean){

            float testNum = atof(buffer);

            printf("%f", testNum);

            floats[floatCounter] = testNum;
            floatCounter++;

            }

            
            
            
        }
            }

        globalCounter = (globalCounter + 1) % num_threads;
        pthread_mutex_unlock(&lock);

        }else{
            pthread_mutex_unlock(&lock);
        }       
    }
