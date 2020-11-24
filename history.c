#include "headers.h"

extern char * usr, pwd[1024], opwd[1024], host [1024];
struct node
{
    char command[1024];
    struct node *next;
};
typedef struct node node;
int available=1;
struct queue
{
    int count;
    node *front;
    node *rear;
};

typedef struct queue queue;
char filename[1024];
queue *q;
char previous [1024];

void trimLeading(char * str)
{
    int index, i, j;

    index = 0;


    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }


    if(index != 0)
    {

        i = 0;
        while(str[i + index] != '\0')
        {
            str[i] = str[i + index];
            i++;
        }
        str[i] = '\0'; 
    }
}


void initialize()
{
    q = malloc(sizeof(queue));
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
    strcpy(previous,"");
}

int isempty()
{
    return (q->rear == NULL);
}

void enqueue( char * command)
{
    if(command==NULL){
        return;
    }
    char str[1024];
    strcpy(str,command);
    trimLeading(str);
    int last=-1;
    for(int i=0; str[i]!='\0';i++){
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            last= i;
        }

    }
    str[last+1]='\0';
    

    if(previous!=NULL){
        if(strcmp(previous,str)==0){
            return;
        }
    }


    
    node *tmp;
    tmp = malloc(sizeof(node));
    strcpy(tmp->command,str);

    tmp->next = NULL;
    if(!isempty())
    {
        q->rear->next = tmp;
        q->rear = tmp;
    }
    else
    {
        q->front = q->rear = tmp;
    }
    q->count++;
}

void dequeue()
{
    node *tmp;
    tmp = q->front;
    q->front = q->front->next;
    q->count--;
    free(tmp);
}

void displayHistory(node *head, int n , int curr)
{
    if(q->count==0){
        printf("No history available\n");
        return;
    }
    if(head == NULL)
    {
        return;
    }
    else
    {
            
            if(q->count-curr<=n){

                printf("%s\n",head->command);

            }
            displayHistory(head->next,n,curr+1);
    }
}
void writeHistory(node * head, FILE *history){
    if(head == NULL)
    {
        return;
    }
    else
    {
        
        fprintf(history, "%s\n", strtok(head->command, "\n"));
        writeHistory(head->next,history);
    }
    
}
void updateHistory(node * head){
   
    FILE *history = fopen(filename, "w");
    //printf("%s\n",filename);

	if(history == NULL)
	{
		perror("Update Shell History:");
        set_err(0);
		return;
	}

    writeHistory(q->front,history);
    fclose(history);

}

void initialiseHistory(){
    initialize();
    strcpy(filename, opwd);
    strcat(filename,"/history.txt");


    FILE *file;
    int file_exists;
    file=fopen(filename,"r");
    if (file==NULL){
        file_exists=0;
    }
        
    else {
        file_exists=1; 
        fclose(file);
    }
    if (file_exists==1)
    {
       file=fopen(filename,"r+b");
    }
    else
    {
        file=fopen(filename,"w+b");
    }
    
    if (file==NULL)
    {
       printf ("Error in initialising History!\n");
       set_err(0);
       available=0;
       fclose(file);
       return;
    }
     char command[1024];
     char ** temp;
    while(fgets(command, 1000, file) != NULL)
    {   
        enqueue(command);
        strcpy(previous,command);
    }
    fclose(file);

}

void addHistory(char *command)
{
    if(q->count == 20)
        dequeue();
    enqueue(command);
    strcpy(previous,command);
    updateHistory(q->front);
}

void history(char ** command){
    int count=0;
    int n=10;
    for(int i=0;command[i]!=NULL;i++){
        count++;
    }
    if(count > 2){

        printf("Invalid No. of Arguments in nightswatch\n");
        set_err(0);
        return;

    }
    if(count==2){
        n = atoi(command[1]);
		if(n <= 0 )
		{
			printf("Invalid Num in history\n");
            set_err(0);
			return;

		}
        if(n>20){
            n=20;
        }


    }

    displayHistory(q->front,n,0);

    

}