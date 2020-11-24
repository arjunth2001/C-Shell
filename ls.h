#ifndef __LS_H
#define __LS_H
void lsChainStarter(char ** tokenizedCommand);
void ls(char * argument, int showall, int list, int count);
char * getPerms(struct stat s);
#endif