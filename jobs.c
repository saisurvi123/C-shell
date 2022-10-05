# include "headers.h"
typedef struct Pair{
    int ind;
    char* name;
}Pair;
int compare1 (const void * a, const void * b){
  Pair *data_1 = (Pair *)a;
  Pair *data_2 = (Pair *)b;
  return strcmp(data_1->name,data_2->name);
}

void jobs_print(char ** commands,int procs[],char * home_dir,int *cnt){
    int x=0;
    char str[200];
    char path[PATH_MAX];
    int pid_l[1000];
    Pair name_job[1000];
    // int jobnum[1000];
    char status[1000][100];
    char* names[1000];
    for(int i=0;i<1000;i++){
        if(procs[i]!=-1){
            x++;
            // printf("[%d] ",x);
            // sprintf(str,"%d",procs[i]);
            sprintf(path,"/proc/%d/stat",procs[i]);
            // printf("%s\n",path);
            FILE* ptr=fopen(path,"r");
            for(int i=0;i<3;i++){
                fscanf(ptr,"%s",str);
            }
            if(strcmp(str,"T")==0){
                // printf("Stopped ");
                strcpy(status[x-1],"Stopped");
            }
            else{
                // printf("Running ");
                strcpy(status[x-1],"Running");
            }
            sprintf(path,"/proc/%d/cmdline",procs[i]);
            ptr=fopen(path,"r");
            fscanf(ptr,"%s",str);
            pid_l[x-1]=procs[i];
            // Pair p1=(Pair*)malloc(sizeof(Pair));
            name_job[x-1].ind=x-1;
            name_job[x-1].name=(char*)malloc(strlen(str)+1);
            strcpy(name_job[x-1].name,str);
            // printf("%s [%d]\n",str,procs[i]);
        }
    }
    // printf("\n \n");
    qsort(name_job,x,sizeof(Pair),compare1);
    for(int i=0;i<x;i++){
        int in=name_job[i].ind;
        printf("[%d] %s %s [%d]\n",name_job[i].ind+1,status[in],name_job[i].name,pid_l[in]);
    }
}
int flag1=0;
int flag2=0;
void func1(int signal){
    flag1=1;
}
void func2(int signal){
    flag2=1;
}
void fg_command(char ** commands,int procs[],int* cnt){
    flag1=0;
    flag2=0;
    int len=0;
    for(int i=0;commands[i]!=NULL;i++){
        len++;
    }
    if(len >2 || len <2){
        printf("enter the valid no of args\n");
        return;
    }
    int req_job=atoi(commands[1]);
    int curr_job=1;
    for(int i=0;i<1000;i++){
        if(procs[i]!=-1){
            if(curr_job==req_job){
                int curr_pid=procs[i];
                signal(SIGTSTP,func1);
                signal(SIGINT,func2);
                kill(curr_pid,SIGCONT);
                procs[i]=-1;
                int stat;
                int x=waitpid(curr_pid,&stat,WNOHANG);
                while(x!=curr_pid){
                    if(flag1){
                        kill(curr_pid,SIGSTOP);
                        procs[*cnt] = curr_pid;
                        (*cnt)++;
                        flag1=0;
                        break;
                    }
                    else if(flag2){
                        kill(curr_pid,SIGINT);
                        flag2=0;
                        // printf("ctrlz encountered\n");
                        break;
                    }
                    x=waitpid(curr_pid,&stat,WNOHANG);
                }
                break;

            }
            curr_job++;
        }
    }
    if(curr_job!=req_job){
        printf("job number invalid\n");
    }
}
void bg_command(char ** commands,int procs[],int* cnt){
    int len=0;
    for(int i=0;commands[i]!=NULL;i++){
        len++;
    }
    if(len >2 || len <2){
        printf("enter the valid no of args\n");
        return;
    }
    int req_job=atoi(commands[1]);
    int curr_job=1;
    for(int i=0;i<1000;i++){
        if(procs[i]!=-1){
            if(curr_job==req_job){
                int curr_pid=procs[i];
                kill(curr_pid,SIGCONT);
                // procs[i]=-1;               
                break;
            }
            curr_job++;
        }
    }
    if(curr_job!=req_job){
        printf("job number invalid\n");
    }
}

void sig(char ** commands,int procs[],int* cnt){
    int sig_num=atoi(commands[2]);
    flag1=0;
    // flag2=0;
    
    int req_job=atoi(commands[1]);
    int curr_job=1;
    for(int i=0;i<1000;i++){
        if(procs[i]!=-1){
            if(curr_job==req_job){
                int curr_pid=procs[i];
                // signal(SIGTSTP,func1);
                // signal(SIGINT,func2);
                kill(curr_pid,sig_num);
                flag1=1;
                procs[i]=-1;
                break;

            }
            curr_job++;
        }
    }
    if(!flag1){
        printf("enter the valid <jobnum>\n");
    }
}