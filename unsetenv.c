#include "headers.h"

void unsetEnv(char ** input){
    int count =0;
    for(int i=0; input[i]!=NULL;i++){
        count++;
    }

    if(count!=2){
        printf("Error in unsetenv: Invalid number of Commands\n");
        set_err(0);
        return;
    }

    if(unsetenv(input[1])==-1){
        perror("unsetenv:");
        set_err(0);
        return;
    }

}