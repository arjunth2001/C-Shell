
#include "headers.h"

extern int errno;
void echo(char * input){
    char * command = strtok(input, "\n");
    char *str=malloc(1024);
    strcpy(str,command);
   // printf("here: %s",str);
    while(str[0] == ' ' || str[0] == '\t' || str[0] == '\n'){
        str++;
    }
    int last=-1;
    for(int i=0; str[i]!='\0';i++){
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            last= i;
        }

    }
    str[last+1]='\0';
    char *temp=strtok(str, " \t");
    str+=(strlen(temp)+1);
    char *token;
    token=strtok(str," \t");
    while(token!=NULL)
    {
        printf("%s ",token);
        token= strtok(NULL," \t");
    }
    printf("\n");

}