#include "headers.h"


void redirectHandler(char * pipeSeperatedInput, int input, int output){


   // printf("%s\n",pipeSeperatedInput);
    char ** tokens = tokenizer(pipeSeperatedInput,"\t\r\n ");
    char readfile[1024];
    char writefile[1024];
    strcpy(writefile,"\0");
    strcpy(readfile,"\0");
    int isAppend=0;
    int stdin_copy=dup(0), stdout_copy=dup(1);
    dup2(input,0);
    dup2(output,1);



    for(int i=0;tokens[i]!=NULL;i++){

        //printf("%s\n",tokens[i]);
        if(strcmp(tokens[i],">")==0){

            if(i<1023&&tokens[i+1]!=NULL){

                strcpy(writefile,tokens[i+1]);
                strcpy(tokens[i+1],">");
                i++;

            }
            else{
                dup2(stdin_copy, 0);
                dup2(stdout_copy, 1);
                close(stdin_copy);
                close(stdout_copy);
                fprintf(stderr, "No output File for >\n");
                set_err(0);
                return;
            }

        }
        else if(strcmp(tokens[i],">>")==0){

            if(i<1023&&tokens[i+1]!=NULL){

                strcpy(writefile,tokens[i+1]);
                strcpy(tokens[i+1],">>");
                i++;
                isAppend=1;

            }
            else{
                dup2(stdin_copy, 0);
                dup2(stdout_copy, 1);
                close(stdin_copy);
                close(stdout_copy);
                fprintf(stderr, "No output File for >>\n");
                set_err(0);
                return;
            }
        }
        else if(strcmp(tokens[i],"<")==0){
             //printf("%s\n",tokens[i+1]);
            if(i<1023&&tokens[i+1]!=NULL){

                strcpy(readfile,tokens[i+1]);
                strcpy(tokens[i+1],"<");
                i++;

            }
            else{
                
                dup2(stdin_copy, 0);
                dup2(stdout_copy, 1);
                close(stdin_copy);
                close(stdout_copy);
                fprintf(stderr, "No output File for <\n");
                set_err(0);
                return;

            }

        }

    }

    if(strcmp(readfile,"\0")!=0){
                int fd = open(readfile, O_RDONLY);
                if (fd == -1)
                {
                    dup2(stdin_copy, 0);
                    dup2(stdout_copy, 1);
                    close(stdin_copy);
                    close(stdout_copy);
                    //printf("%s\n",readfile);

                    perror("Opening file for reading in redirection:");
                    set_err(0);
                    return;
                }
                dup2(fd, 0);
                close(fd);


    }
    if(strcmp(writefile,"\0")!=0){
            int fd;
            if(isAppend){
                fd = open(writefile, O_CREAT | O_WRONLY | O_APPEND, 0644);

            }else{
                fd = open(writefile, O_CREAT | O_WRONLY | O_TRUNC, 0644);

            }
            if (fd == -1)
                {
                    dup2(stdin_copy, 0);
                    dup2(stdout_copy, 1);
                    close(stdin_copy);
                    close(stdout_copy);

                    perror("Opening File for writing in redirection: ");
                    set_err(0);
                    return;
                }
                dup2(fd, 1);
                close(fd);

    }
    char **newtokens = (char **)malloc(1024);
    int i,j;

    for( i=0,j=0;tokens[i]!=NULL;i++){
        if(strcmp(tokens[i],">")==0||strcmp(tokens[i],">>")==0||strcmp(tokens[i],"<")==0){
            continue;
        }
        else{
            newtokens[j]=tokens[i];
            j++;
        }

    }
    newtokens[j]=NULL;
    startExecutionofCommand(newtokens,"\r\t\n ");
    dup2(stdin_copy, 0);
    dup2(stdout_copy, 1);
    close(stdin_copy);
    close(stdout_copy);
    return;
}