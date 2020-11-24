
#include "headers.h"

extern struct Job* jhead;
extern int stdinbackup;
extern int stdoutbackup;
int totaljobs=0;

void push(struct Job** jhead_ref, int no, int pid, char status[100], char command[100]) { 
    struct Job* current;
    struct Job* new_Job = (struct Job*) malloc(sizeof(struct Job)); 
    new_Job->no  = no;
    new_Job->pid  =pid;
    strcpy(new_Job->status,status);
    strcpy(new_Job->command,command); 

    if (*jhead_ref == NULL 
        || (*jhead_ref)->no 
               >= new_Job->no) { 
        new_Job->next = *jhead_ref; 
        *jhead_ref = new_Job; 
    } 
    else { 
        current = *jhead_ref; 
        while (current->next != NULL 
               && current->next->no < new_Job->no) { 
            current = current->next; 
        } 
        new_Job->next = current->next; 
        current->next = new_Job; 
    } 
} 

void push2(struct Job** jhead_ref, int no, int pid, char status[100], char command[100]) 
{ 
    struct Job* new_Job = (struct Job*) malloc(sizeof(struct Job)); 
    new_Job->no  = no;
    new_Job->pid  =pid;
    strcpy(new_Job->status,status);
    strcpy(new_Job->command,command);

    new_Job->next = (*jhead_ref); 
    (*jhead_ref)    = new_Job; 
} 
void deleteJob(struct Job **jhead_ref, int no) 
{  
    struct Job* temp = *jhead_ref, *prev; 
  
 
    if (temp != NULL && temp->no == no) 
    { 
        *jhead_ref = temp->next;   
        free(temp);                
        return; 
    } 
  
    while (temp != NULL && temp->no != no) 
    { 
        prev = temp; 
        temp = temp->next; 
    } 

    if (temp == NULL) return; 
  

    prev->next = temp->next; 
  
    free(temp);  
} 
int getjobnofrompid(struct Job* jhead, int pid) 
{ 

    if (jhead == NULL) 
        return -1; 
    if (jhead->pid == pid) 
        return jhead->no; 
  
    return getjobnofrompid(jhead->next, pid); 
}
int getpidfromjobno(struct Job* jhead, int no) 
{ 

    if (jhead == NULL) 
        return -1; 
    if (jhead->no == no) 
        return jhead->pid; 
  
    return getpidfromjobno(jhead->next, no); 
}
int searchjobBypid(struct Job* jhead, int pid) 
{ 

    if (jhead == NULL) 
        return 0; 
    if (jhead->pid == pid) 
        return 1; 
  
    return searchjobBypid(jhead->next, pid); 
}

int searchjobByno(struct Job* jhead, int no) 
{ 

    if (jhead == NULL) 
        return 0; 
    if (jhead->no == no) 
        return 1; 
  
    return searchjobByno(jhead->next, no); 
}

void updatestatus(struct Job* jhead, int jobno, char status[100]) 
{ 
    struct Job* current = jhead;
    while (current != NULL) 
    { 
        if (current->no == jobno) 
            strcpy(current->status,status);
        current = current->next; 
    } 
}

int getstatusbypid(int pid){
    char filepath[1000];
    sprintf(filepath,"/proc/%d/stat",pid);
    FILE * statfile = fopen(filepath,"r");
    if(statfile==NULL){
        printf("Error in finding Info for pid: %d\n",pid);
        set_err(0);
        return -1;
    }
    int argumentCount=1;
    char buffer[1000];
    while(fscanf(statfile,"%s",buffer)==1){
        if(argumentCount==3){
            if(strcmp("T",buffer)==0){
                return 0;
            }
            else{
                return 1;
            }

        }
        argumentCount++;
     }
     fclose(statfile);
     return -1;

}

void checkstatus(struct Job* jhead){
    
    struct Job* current = jhead;
    while (current != NULL) 
    {   
        int status= getstatusbypid(current->pid);
        if(status==-1){
           current = current->next; 
           continue; 
        }
        if(status==0){
            
            updatestatus(jhead,current->no,"Stopped");

        }
        else{
            updatestatus(jhead,current->no,"Running");
        }
        current = current->next; 
    }



}

void kjob(char ** input){


    int count=0;
    for(int i=0; input[i]!=NULL;i++){
        count++;
    }

    if(count!=3){
        printf("Invalid no. of Arguments for kjob\n");
        set_err(0);
        return;
    }
    int jobno, sig;
    jobno=atoi(input[1]);
    sig=atoi(input[2]);

    if( jobno <= 0 || jobno > totaljobs){
        printf("Invalid jobno in kjobs\n");
        set_err(0);
        return;
    }
    int isvalidjob=searchjobByno(jhead,jobno);
    if(!isvalidjob){
        printf("Invalid jobno in kjobs\n");
        set_err(0);
        return;
    }
    if(sig<0){
        printf("Invalid signal number in kjob\n");
        set_err(0);
    }
    if(kill(getpidfromjobno(jhead,jobno),sig)!=-1){

        if(sig==SIGCONT){
                updatestatus(jhead,getjobnofrompid(jhead,getpidfromjobno(jhead,jobno)),"Running");
        }
        if(sig==SIGSTOP){
                updatestatus(jhead,getjobnofrompid(jhead,getpidfromjobno(jhead,jobno)),"Stopped");
        }
        if(sig==SIGTSTP){
                updatestatus(jhead,getjobnofrompid(jhead,getpidfromjobno(jhead,jobno)),"Stopped");
        }
        
    }
    else{
        perror("kjobs:");
        set_err(0);
    }

}


void kjobs(struct Job* jhead) 
{ 
    if (jhead == NULL) 
       return; 
    
    printf("[%d] %s %s [%d]\n",jhead->no,jhead->status,jhead->command,jhead->pid); 
    kjobs(jhead->next);
    
} 
void kjobsexecuter(){
    checkstatus(jhead);
    kjobs(jhead);

}

void fg(char ** input){
    int count=0;
    for(int i=0; input[i]!=NULL;i++){
        count++;
    }

    if(count!=2){
        printf("Invalid no. of Arguments for fg\n");
        set_err(0);
        return;
    }
    int jobno = atoi(input[1]);
    int isvalidno=searchjobByno(jhead,jobno);
    if(!isvalidno){
        printf("Invalid Job No. Specified\n");
        set_err(0);
        return;
        
    }

    int status;
    int pid = getpidfromjobno(jhead,jobno);
    if(pid==-1){
        printf("Invalid PID in fg\n");
        set_err(0);
        return;
    }

    deleteJob(&jhead,jobno);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(stdinbackup, pid);
    kill(pid, SIGCONT);
    waitpid(pid, &status, WUNTRACED);
    if(WIFSTOPPED(status)!=0)
    {
            push(&jhead,jobno,pid,"Stopped",getNamefromPID(pid));
            printf("[%d] Stopped %s [%d]\n", jobno,getNamefromPID(pid),pid);
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
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);

}

void bg( char ** input){
    int count=0;
    for(int i=0; input[i]!=NULL;i++){
        count++;
    }

    if(count!=2){
        printf("Invalid no. of Arguments for bg\n");
        set_err(0);
        return;
    }
    int jobno = atoi(input[1]);
    int isvalidno=searchjobByno(jhead,jobno);
    if(!isvalidno){
        printf("Invalid Job No. Specified\n");
        set_err(0);
        return;
        
    }

    int status;
    int pid = getpidfromjobno(jhead,jobno);
    if(pid==-1){
        printf("Invalid PID in bg\n");
        set_err(0);
        return;
    }
    if(kill(pid, SIGCONT)==-1){
         set_err(0);
         perror("bg: kill: ");
    }
    updatestatus(jhead,jobno,"Running");

}
void overkill(){
    
    int i=totaljobs;

    struct Job* current = jhead;
    while (current != NULL) 
    { 
        kill(current->pid,SIGKILL);
        current = current->next; 
    }
    while(i){
        deleteJob(&jhead,i);
        i--;
    }
    printf("This is power. Overkill Executed Successfully.\n");
    totaljobs=0;

}

