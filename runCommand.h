#ifndef __RUNCOMMAND_H
#define __RUNCOMMAND_H
void insertBgListner(int pid, char * pname);
void deleteBgListner( int pid);
void Zombies();
char ** myTokenizer(char * temp1, char * delimiters);
void runCommandinBackground(char * command);
void runCommandinForeground(char * command);
void runCommandChain(char * command, char** tokenizedCommand);
char * getNamefromPID(int pid);
void myHandler(int signum);
#endif