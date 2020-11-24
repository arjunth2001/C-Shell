#ifndef __INPUT_H
#define __INPUT_H
char *getPathtoDisplay (char *path);
char * getSystemPath(char *path);
char ** tokenizer(char * str, char * delimiters);
void startExecutionofCommand(char ** tokenizedCommand , char * delimiter);
void splitSemicolons(char *input);
#endif