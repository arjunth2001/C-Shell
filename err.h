#ifndef __ERR_H
#define __ERR_H
void set_err(int n);
int geterr();
void print_err(int err_no);
void resetskip();
int shouldSkip();
void setskip(char * input);
#endif