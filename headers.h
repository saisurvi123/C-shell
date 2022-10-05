#ifndef _headers_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <termios.h>

// #include <conio.h>
int fg_flag;
int foreground_pid;
int ctrz;
int child_pid;
char* curr_job;
void discover(char** commands,char* home_dir);
void newprocess(char** commands,int bg,int processes[],int* cnt);
void display_history(int x);
void add_history(char line[]);
void convert_to_tilda1(char* home_dir,char* current_path,char* final_path);
void convert_to_tilda2(char* home_dir,char* path,char* final_path,char* final_prev);
int display_shell(char* home_dir);
void runcommand(char** command,char* home_dir,char* prev_path,int bg,int preocesses[],int* count);
void ls_command(char* home_dir,char** commands);
void pinfo_id(char* home_dir,int pid);
void redirection( char *home_dir, char *prev_dir,int processes[],int * cnt, int bg,char** commands);
void jobs_print(char ** commands,int procs[],char * home_dir,int *cnt);
void fg_command(char ** commands,int procs[],int* cnt);
void bg_command(char ** commands,int procs[],int* cnt);
void sig(char ** commands,int procs[],int* cnt);





#endif