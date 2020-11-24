#include "headers.h"
char * usr, pwd[1024], opwd[1024], host [1024];
char previousdir[1024];
int stdinbackup;
int stdoutbackup;
struct Job* jhead = NULL; 