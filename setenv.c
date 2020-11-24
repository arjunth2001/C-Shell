#include "headers.h"

void setEnv(char **  input){

    int count =0;
    for(int i=0; input[i]!=NULL;i++){
        count++;
    }

    if(count<1 || count >3){
        printf("Error in setenv: Invalid number of Commands\n");
        set_err(0);
        return;
    }
    if(count==2){

        if(setenv(input[1],"",1)==-1){
            perror("setenv:");
            set_err(0);
            return;
        }

    }
    else{
        if(setenv(input[1],input[2],1)==-1){
            perror("setenv:");
            set_err(0);
            return;
        }
    }

}