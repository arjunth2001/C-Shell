#include "headers.h"

int isQ()
{
    struct termios oldt, newt;
    int bytes;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ioctl(STDIN_FILENO, FIONREAD, &bytes);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if( bytes > 0 )
    {
        if(getchar()=='q'){
            return 1;
        }
    }

    return 0;
}

void nWInterrupt(){

    FILE * interfile = fopen("/proc/interrupts","r");
     if(interfile==NULL){
         printf("Error in finding Info for pid: Error in opening file\n");
         set_err(0);
         return;
     }
     int argumentCount=1;
     char buffer[1000];
     while(fgets(buffer,sizeof(buffer),interfile)){
         if(argumentCount==1|| argumentCount==3){
             printf("%s\n",buffer);

         }
        argumentCount++;
     }
     fclose(interfile);
     
}

void nWNewBorn(){
    FILE * loadfile = fopen("/proc/loadavg","r");
     if(loadfile==NULL){
         printf("Error in finding Info for pid: Error in opening file\n");
         set_err(0);
         return;
     }
     int argumentCount=1;
     char buffer[1000];
     while(fscanf(loadfile,"%s",buffer)==1){
         if(argumentCount==5){
             printf("%s\n",buffer);

         }
        argumentCount++;
     }
     fclose(loadfile);
}
void nightswatch(char ** command){
   // printf("meh");

    int count=0;
    int n=2;
    for(int i=0;command[i]!=NULL;i++){
        count++;
    }
    if(count==1||count==3||count>4){

        printf("Invalid No. of Arguments in nightswatch\n");
        set_err(0);
        return;

    }
    int isInterrupt=0;
    if(count==2){
        if(strcmp("interrupt",command[1])==0){
            isInterrupt=1;
        }
        else if(strcmp("newborn",command[1])==0){
            isInterrupt=0;
        }
        else{
            printf("Invalid Arguments in nightswatch\n");
            set_err(0);
            return;

        }
    }
    if(count==4){

        n=atoi(command[2]);
        if(n<1){
            printf("Invalid time");
            set_err(0);
            return;
        }
        if(strcmp("interrupt",command[3])==0){
            isInterrupt=1;
        }
        else if(strcmp("newborn",command[3])==0){
            isInterrupt=0;
        }
        else{
            printf("Invalid Arguments in nightswatch\n");
            set_err(0);
            return;

        }


    }
    while(1){
        //printf("moo\n");
        if(isInterrupt){
            nWInterrupt();
        }
        else{
            nWNewBorn();
        }
        clock_t initclock = clock();
        while (clock() - initclock < n * CLOCKS_PER_SEC){
                if(isQ()){
                    return;
                }
        }
    }



}