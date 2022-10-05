#include "headers.h"
int foreground_pid;

int indicator = 0;
void newprocess(char** commands,int bg,int processes[],int* cnt){
    indicator=0;
    int len=0;
    char* main_comm=(char*)malloc(sizeof(commands[0])+1);
    strcpy(main_comm,commands[0]);
    // printf("%s\n",commands[0]);
    while(commands[len]!=NULL){
        len++;
    }
    char *parameters[len+1];
    for(int i=0;i<len;i++){
        parameters[i]=(char*)malloc(strlen(commands[i])+1);
        strcpy(parameters[i],commands[i]);
        // printf("%s\n",parameters[i]);
    }
    parameters[len]=NULL;
    // int x=fork();
    // if(x==0){
    //     if(bg==1){
    //         setpgid(0,0);
    //     }
    //     if(execvp(main_comm,parameters)==-1){
    //         printf("invalid command\n");
    //     }
    // }
    // else if(x>0){
    //     if(bg==0){
    //         waitpid(x,NULL,0);
    //     }
    //     else{
    //         printf("[%d]\n", x);
    //     }
    // }
    // else{
    //     printf("error in creating child process\n");
    // }

    if(bg)
    {
        int pid = fork();
        processes[*cnt] = pid;
        if(pid>0){
            printf("[%d]\n",pid);
        }
        (*cnt)++;
        setpgid(0,0);
        if(pid == 0)
        {
            execvp(parameters[0], parameters);
            printf("%s: Command not found\n", parameters[0]);
            _exit(-1);
        }
        
       
    }
    else
    {
        
         int pid = fork();
        if(pid == 0)
        {
            execvp(parameters[0], parameters);
            printf("%s: Command not found\n", parameters[0]);
            exit(0);
        }
        else
        {
            child_pid=pid;
            curr_job=(char*)malloc(strlen(parameters[0])+1);
            strcpy(curr_job,parameters[0]);
            int status;
            foreground_pid = pid;
            // signal(SIGTSTP, anotherSignalHandler);
            waitpid(pid, &status, WUNTRACED);
            if(indicator == 1)
            {
                processes[*cnt] = pid;
                (*cnt)++;
                indicator = 0;
            }
            if(WIFSTOPPED(status))
               {
                    printf("%s with PID %d stopped\n", curr_job, pid);
                    // strcpy(jobs[back_g].job_name, CURR_JOB);
                    processes[*cnt] = child_pid;
                    *cnt=*cnt+1;
               }
            }
        // fg_flag=0;
    }
}
