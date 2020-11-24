#include "headers.h"

extern int errno;
extern char * usr, pwd[1024], opwd[1024], host [1024];
extern struct Job* jhead;
extern int totaljobs;
extern int stdinbackup;
extern int stdoutbackup;
 
struct Bg* head=NULL;
char *getPathtoDisplay2(char *path){
    
    char *newpath=malloc(1024);
    char *newpath2=malloc(1024);
    strcpy(newpath2,path);
    //printf("here in getpathtodisplay");
    strcpy(newpath,"~");
    int lengthofpath= strlen(path);
    int lengthofhome= strlen(opwd);
    if(lengthofpath < lengthofhome)
    {
        
        free(newpath);
        return newpath2;

    }
        
    if(strncmp(path, opwd, lengthofhome) == 0)
    {
        path=path+lengthofhome;
        strcat(newpath,path);
       // printf("meh\n");
        free(newpath2);
        return newpath;
    }
   // printf("moo\n");
    free(newpath);
    return newpath2;
}

char * getNamefromPID(int pid){
     char * filepath= malloc(1024);
     sprintf(filepath,"/proc/%d/stat",pid);
     FILE * statfile = fopen(filepath,"r");
     if(statfile==NULL){
         printf("Error in finding Info for pid: %d\n",pid);
         set_err(0);
         return "";
     }
     int argumentCount=1;
     char buffer[1000];
     while(fscanf(statfile,"%s",buffer)==1){
         if(argumentCount==2){
             strcpy(filepath,buffer);

         }
        argumentCount++;
     }
     fclose(statfile);
     int length= strlen(filepath);
     filepath[length-1]='\0';
     filepath++;
     return filepath;

}

void insertBgListner( int pid, char * pname) 
{ 
    //printf("%d\n inserted",pid);
    struct Bg** head_ref=&head;
    struct Bg* new_node = (struct Bg*) malloc(sizeof(struct Bg)); 
    new_node->pid  = pid;
    strcpy(new_node->pname,pname);
    new_node->next = (*head_ref); 
    (*head_ref)    = new_node; 
}
  
void deleteBgListner(int pid) 
{  
    struct Bg **head_ref=&head;
    struct Bg* temp = *head_ref, *prev; 
    while (temp != NULL && temp->pid == pid) 
    { 
        *head_ref = temp->next; 
        free(temp);             
        temp = *head_ref;        
    } 
    while (temp != NULL) 
    { 

        while (temp != NULL && temp->pid != pid) 
        { 
            prev = temp; 
            temp = temp->next; 
        } 
        if (temp == NULL) return; 
  
        prev->next = temp->next; 
  
        free(temp); 
  
        temp = prev->next; 
    } 
} 
void Zombies() 
{ 
    struct Bg* node = head; 
    int toDelete[1024];
    int count = 0;
    //printf("here\n");
    while (node != NULL) 
    { 
        int status;
        //printf("In z: %d\n",node->pid);
        int returnvalue = waitpid(node->pid, &status, WNOHANG);
        if(returnvalue==node->pid){
            if(WIFEXITED(status))
            {
                fflush(stdout);
                fflush(stderr);
                fprintf(stderr,"\n%s with pid %d exited normally\n", node->pname,node->pid);
                usr=getenv("USER");
                gethostname(host, 1000);
                getcwd(pwd,1000);
                fprintf(stderr,"\033[1;32m\r%s@\033[1;34m%s", usr, host);
                fprintf(stderr,"\033[1;33m:%s >\033[0m",getPathtoDisplay2(pwd));
                toDelete[count]=node->pid;
                count++;
            }
            else if(WIFSIGNALED(status))
            {
                fflush(stdout);
                fflush(stderr);
                fprintf(stderr,"\n%s with pid %d exited with errors.\n", node->pname,node->pid);
                usr=getenv("USER");
                gethostname(host, 1000);
                getcwd(pwd,1000);
                fprintf(stderr,"\033[1;32m\r%s@\033[1;34m%s", usr, host);
                fprintf(stderr,"\033[1;33m:%s >\033[0m",getPathtoDisplay2(pwd));

                toDelete[count]=node->pid;
                count++;

            }
        
        }
            
         
        node = node->next; 
    } 

    for(int i=0; i<count; i++){
        deleteBgListner(toDelete[i]);
        deleteJob(&jhead,getjobnofrompid(jhead, toDelete[i]));
    }
} 

void myHandler(int signum){

    Zombies();

}


char ** myTokenizer(char * temp1, char * delimiters){
    char *str=malloc(1024);
    //printf("here in tokenizer");
    strcpy(str,temp1);
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
    char ** tokens;
    tokens=malloc(10000);

    char *temp=strtok(str, delimiters);

	int start = 0;

	while(temp)
	{
		tokens[start] = malloc(strlen(temp)+1);
		strcpy(tokens[start],temp);
		temp = strtok(NULL, delimiters);
        start++;
	}

	tokens[start] = NULL;

    return tokens;
    
}


void runCommandinBackground(char * command){
    //printf("Runs as Background");
    char * str= malloc(1024);
    strcpy(str,command);

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
    signal(SIGCHLD,myHandler);
    int pid = fork();
    char ** tokens= myTokenizer(str,"\r\n\t ");
    if(pid < 0){
        perror("Child process is not created in Background Process:");
        set_err(0);
        return;
    }
    if(pid==0){
        setpgid(0,0);
       /* for(int i=0; tokens[i]!=NULL;i++){
            printf("\n%s\n",tokens[i]);
        }*/
        signal(SIGINT, SIG_DFL);
	    signal(SIGTSTP, SIG_DFL);

        int flag= execvp(tokens[0],tokens);
        if(flag==-1)
        {
            perror("Process couldn't start");
            set_err(0);
            exit(-1);

        }
        
    }
    
    if(pid>0){
        setpgid(pid,pid);
        printf("\n[+%d] %d\n", ++totaljobs,pid);
        insertBgListner(pid,str);
        push(&jhead,totaljobs,pid,"Stopped",str);
        
    }

}

void runCommandinForeground(char * command){
    //printf("Runs as Foreground\n");
    
    char * str= malloc(1024);
    strcpy(str,command);

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
    int pid = fork();
    char ** tokens= myTokenizer(str,"\r\n\t ");
    if(pid<0)
    {
        perror("Child was not created in Foreground Process: ");
        set_err(0);
        return;
    }
    if(pid==0){

       /* for(int i=0; tokens[i]!=NULL;i++){
            printf("\n%s\n",tokens[i]);
        }*/
        fflush(stdout);
        fflush(stdin);
        fflush(stderr);
        setpgid(0,0);
        signal(SIGINT, SIG_DFL);
	    signal(SIGTSTP, SIG_DFL);
        //printf("meh\n");
        int flag= execvp(tokens[0],tokens);
        if(flag==-1){
            perror("Process couldn't start");
            set_err(0);
            //printf("moo");
            exit(-1);
        }



    }
    int status;
    if(pid>0){
        
        
        setpgid(pid,pid);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(stdinbackup, pid);
        waitpid(pid, &status, WUNTRACED);
        if (WIFSTOPPED(status)) {
                insertBgListner(pid,getNamefromPID(pid));
                push(&jhead,++totaljobs,pid,"Stopped",getNamefromPID(pid));
                printf("[%d] Stopped %s [%d]\n",totaljobs, getNamefromPID(pid),pid);
                set_err(0);
        }
        if(!WIFEXITED(status)&&WIFSIGNALED(status)){
            set_err(0);
        }
        else if(WIFEXITED(status)&&WEXITSTATUS(status)!=0){
            set_err(0);
        }
        tcsetpgrp(stdinbackup, getpgrp());
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        

    }

}

void runCommandChain(char * command, char** tokenizedCommand){
    
    int countamb=0;
    for(int i=0; tokenizedCommand[i]!=NULL;i++){
        for(int j=0;j<strlen(tokenizedCommand[i]);j++){
            if(tokenizedCommand[i][j]=='&')
            countamb++;
        }
    }

    int currcount=0;
    char ** tokenizedInput= myTokenizer(command,"&");
    for(int i = 0; tokenizedInput[i]!=NULL;i++){

        if(currcount<countamb){
            runCommandinBackground(tokenizedInput[i]);
            currcount++;
        }
        else{
           // printf("came here");
            fflush(stdin);
            fflush(stdout);
            runCommandinForeground(tokenizedInput[i]);
        }

    }

    
}