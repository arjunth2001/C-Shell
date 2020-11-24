#ifndef __STRUCT_H
#define __STRUCT_H
struct Bg 
{ 
    int pid; 
    char pname[100];
    struct Bg *next; 
};
struct Job 
{ 
    int no,pid;
    char status[100];
    char command[100]; 
    struct Job *next; 
}; 
#endif
