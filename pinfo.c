#include "headers.h"
extern int errno;
 void pinfo(char * spid){

     
     char filepath[1000];
     sprintf(filepath,"/proc/%s/stat",spid);
     FILE * statfile = fopen(filepath,"r");
     if(statfile==NULL){
         printf("Error in finding Info for pid: %s\n",spid);
         set_err(0);
         return;
     }
     printf("pid -- %s\n",spid);
     int argumentCount=1;
     char buffer[1000];
     while(fscanf(statfile,"%s",buffer)==1){
         if(argumentCount==3){
             printf("Process Status -- %s\n",buffer);

         }

         if(argumentCount==23){
             printf("memory -- %s {Virtual Memory}\n", buffer);
        }
        argumentCount++;
     }
     fclose(statfile);
     sprintf(filepath,"/proc/%s/exe",spid);
     int length;
     if((length=readlink(filepath,buffer,1000))==-1)
     {
         printf("Cannot find Executable Path for pid: %s\n", spid);
         set_err(0);
         return;
     }
     if(length!=1000)
        buffer[length]='\0';

     printf("Executable Path -- %s\n",buffer);

}

 void pinfostarter(char ** command){
    
    int flag=1;

    for(int i=1;command[i]!=NULL;i++){
        pinfo(command[i]);
        printf("\n");
        flag=0;

    }

    if(flag){
        char spid[100];
        sprintf(spid,"%d",getpid());
        pinfo(spid);
    }

}