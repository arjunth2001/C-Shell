#include "headers.h"

extern int errno;
extern char previousdir[1024];

void cd(char **input){

    int count=0;
    for(int i=0; input[i]!=NULL; i++){
        count++;
    }
    char * argument;
    if(count==1){

        argument=malloc(10);
        strcpy(argument,"~");

    }
    else{
        argument=strtok(input[1],"\n");
    }
    int flag=0;
    
    if(strcmp(argument,"-")==0){
        argument=malloc(1000);
        strcpy(argument,previousdir);
        flag=1;
    }
    getcwd(previousdir,1000);
    char *pathToChange= getSystemPath(argument);
    int cdStatus= chdir(pathToChange);
    if(cdStatus==-1)
    {
        perror("cd Command");
        set_err(0);
        
    }
    else if(flag){
        printf("%s\n",getPathtoDisplay(argument));
    }

    free(pathToChange);

}