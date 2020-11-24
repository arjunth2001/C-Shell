#include "headers.h"


void piper( char ** pipeSeperatedInput){

    int countOfCommand=0;
    for(int i=0; pipeSeperatedInput[i]!=NULL;i++){
        countOfCommand++;
    }
    if(!countOfCommand){
        return;
    }
    if(countOfCommand==1){
        redirectHandler(pipeSeperatedInput[0],0,1);
    }
    else{
        int fides[2],input=0;

        for(int i=0;i<countOfCommand;i++){
            if(i==countOfCommand-1){

                redirectHandler(pipeSeperatedInput[i],input,1);
                return;
            }
            pipe(fides);
            redirectHandler(pipeSeperatedInput[i],input,fides[1]);
            close(fides[1]);
            input=fides[0];
        }
    
    }




}