#ifndef __JOBS_H
#define __JOBS_H
void push(struct Job** jhead_ref, int no, int pid, char status[100], char command[100]);
void deleteJob(struct Job **jhead_ref, int no);
int getjobnofrompid(struct Job* jhead, int pid);
int getpidfromjobno(struct Job* jhead, int no);
int searchjobBypid(struct Job* jhead, int pid);
int searchjobByno(struct Job* jhead, int no);
void kjobs(struct Job* jhead);
void updatestatus(struct Job* jhead, int jobno, char status[100]);
void kjob(char ** input);
void fg(char ** input);
void bg( char ** input);
void overkill();
void kjobsexecuter();
#endif