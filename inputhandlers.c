#include "headers.h"

extern int errno;
char * supported_commands[100]= {"cd","pwd","echo","ls","pinfo","clear","history","nightswatch","setenv","unsetenv","jobs","kjob","fg","bg","overkill","quit"};
extern char * usr, pwd[1024], opwd[1024], host [1024];
extern struct Job* jhead;
int ans;
char *getPathtoDisplay (char *path){
    
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
char * getSystemPath(char *path){


    char *temp= malloc(1024);
    //printf("here in systempath");
    if(path==NULL){
        strcpy(temp,opwd);
        return temp; 
    }
    
    strcpy(temp,path);
    int pathlength=strlen(path);
    if(path[0]!='~')
        return temp;

    if(path[0]=='~'&& pathlength==1)
    {
        strcpy(temp,opwd);
        return temp;

    }
        
    getcwd(pwd,1024);
    strcpy(temp,opwd);
    strcat(temp,&path[1]);
    return temp;

}
char ** tokenizer(char * temp1, char * delimiters){
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
char * reconstructcommand( char ** tokens){

    char * command = malloc(1024);
    if(tokens==NULL){
        return NULL;
    }
    if(tokens[0]==NULL){
        return NULL;
    }
    strcpy(command,tokens[0]);
    strcat(command," ");
    for(int i=1; tokens[i]!=NULL; i++){
        strcat(command,tokens[i]);
        strcat(command," ");
    }
    return command;

}
void startExecutionofCommand(char ** tokenizedCommand , char * delimiter){
    //printf("here in start");
    set_err(1);
    char * command= reconstructcommand(tokenizedCommand);
    if(command==NULL){
        return;
    }
    int flag=-1;
    //printf("Command is:%s\n",tokenizedCommand[0]);
        for(int i=0; i<16;i++){
        if(strcmp(tokenizedCommand[0],supported_commands[i])==0)
        {
            flag=i;
            //printf("here in forloop");

        }
    }
    addHistory(command);
    
        if(flag==0){

            cd(tokenizedCommand);
            getcwd(pwd,1000);

        }
        else if(flag==1){

            printf("%s\n",pwd);

        }
        else if(flag==2){

            echo(command);

        }
        else if(flag==3){

            lsChainStarter(tokenizedCommand);

        }
        else if(flag==4){

            pinfostarter(tokenizedCommand);

        }
        else if(flag==5){
            
            printf("\e[1;1H\e[2J");

        }
        else if(flag==6){

            history(tokenizedCommand);

        }
        else if(flag==7){
            nightswatch(tokenizedCommand);
        }
        else if(flag==8){
            setEnv(tokenizedCommand);
        }
        else if(flag==9){

            unsetEnv(tokenizedCommand);

        }
        else if(flag==10){

            kjobsexecuter();

        }
        else if(flag==11){
            kjob(tokenizedCommand);

        }
        else if(flag==12){
            fg(tokenizedCommand);

        }
        else if(flag==13){
            bg(tokenizedCommand);

        }
        else if(flag==14){

            overkill();

        }
        else if(flag==15){

            overkill();
            printf("\e[1;1H\e[2J");
            exit(EXIT_SUCCESS);

        }
        else{

            runCommandChain(command,tokenizedCommand);
           // Zombies();

        }

}
void logicalEvaluator(char * colonSplit){

    char boo[1024];
    int length= strlen(colonSplit);
    resetskip();
    setskip(colonSplit);
    if(length>1023)
        return;
    int j=0;
    for(int i=0; i< length;i++){
        if(((colonSplit[i]=='@'||colonSplit[i]=='$')&&i==0)||((colonSplit[i]=='@'||colonSplit[i]=='$')&&i==length-1))
            continue;
        if(colonSplit[i]=='@'||colonSplit[i]=='$')
            boo[j++]= colonSplit[i];       
    }

    char ** tokens = tokenizer(colonSplit,"@$");
    int countoftokens=0;
    for(int i=0;tokens[i]!=NULL;i++){
        countoftokens++;
    }
    //printf("count of tokens: %d count of &@ %d\n", countoftokens,j);
    if(countoftokens!=j+1){
        printf("Wrong format for $ or @\n. Check.\n");
    }
    
    char ** pipeSeperatedInput;
    j=-1;
    for(int i=0;tokens[i]!=NULL;i++,j++){


        if(i==0){
            fflush(stdin);
            fflush(stdout);
            pipeSeperatedInput=tokenizer(tokens[i],"|");
            piper(pipeSeperatedInput);
            ans=geterr();
            continue;
            
        }
        else{
            if(boo[j]=='@'&&shouldSkip()&&ans==0)
            {
                continue;

            }
            if(boo[j]=='$'&&shouldSkip()&&ans==1){
                
                continue;
                
            }
            fflush(stdin);
            fflush(stdout);
            pipeSeperatedInput=tokenizer(tokens[i],"|");
            piper(pipeSeperatedInput);
            if(boo[j]=='@'){
                ans=ans&geterr();
            }
            else{
                ans=ans|geterr();
            }

        }

    }
    
}


void splitSemicolons(char *input){
    char ** colonSplits = tokenizer(input,";");
    //printf("here in SplitSemicolon");
    char ** pipeSeperatedInput;
    for(int i=0; colonSplits[i]!=NULL;i++)
    {

        logicalEvaluator(colonSplits[i]);
        

    }
    print_err(ans);

}
