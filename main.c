#include "headers.h"

extern int errno;
extern char * usr, pwd[1024], opwd[1024], host [1024];
extern char previousdir[1024];
extern int stdinbackup;
extern int stdoutbackup;

int main(int argc, char const *argv[]){

    signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
    signal(SIGCHLD,myHandler);
    stdinbackup=dup(0);
    stdoutbackup=dup(1);

    getcwd(opwd, 1000);
    initialiseHistory();
    printf("\e[1;1H\e[2J");
    usr=getenv("USER");
    char * input =malloc(1024);
    gethostname(host, 1000);
    getcwd(opwd, 1000);
    getcwd(pwd,1000);
    getcwd(previousdir,1000);
    int size=1024;
    char * disp;
    int sizein;
    //printf("\n");
    while(1){

        //Zombies();
        signal(SIGINT, SIG_IGN);
	    signal(SIGTSTP, SIG_IGN);
        signal(SIGCHLD,myHandler);
        dup2(stdinbackup,0);
        dup2(stdoutbackup,1);

        usr=getenv("USER");
        gethostname(host, 1000);
        getcwd(pwd,1000);
        printf("\033[1;32m");
        printf("%s@\033[1;34m%s", usr, host);
        printf("\033[1;33m");
        printf(":%s >",disp=getPathtoDisplay(pwd));
        printf("\033[0m");
        sizein=getline(&input, &size, stdin);
        if(sizein<0){
            overkill();
            printf("\e[1;1H\e[2J");
            exit(EXIT_SUCCESS);
        }
        //printf(" The input is: %s\n", input);
        fflush(stdin);
        fflush(stdout);
        splitSemicolons(input);
        Zombies();
        free(disp);
        


    }


    return 0;

}