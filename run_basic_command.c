#include "headers.h"
void runcommand(char** command,char* home_dir,char* prev_path,int bg,int processes[],int* cnt){
    
    if(strcmp("echo",command[0])==0){
        for(int i=1;command[i]!=NULL;i++){
            printf("%s ",command[i]);
        }printf("\n");
    }
    else if(strcmp("pwd",command[0])==0){
        char dir[PATH_MAX];
        getcwd(dir,sizeof(dir));
        printf("%s\n",dir);
    }
    else if(strcmp("cd",command[0])==0){
        char finalp[PATH_MAX];
        // printf("hello\n");
        // printf("%ld\n",strlen(command[1]));
        if(command[1]==NULL){
            strcpy(finalp,home_dir);
        }
        else{
            convert_to_tilda2(home_dir,command[1],finalp,prev_path);
        }
        int y=chdir(finalp);
        if(y<0){
            printf("error in chdir\n");
            return;
        }
    }
    else if(strcmp("ls",command[0])==0){
        ls_command(home_dir,command);
    }
    else if(strcmp("pinfo",command[0])==0){
        if(command[1]==NULL){
            int id=getpid();
            pinfo_id(home_dir,id);
        }
        else{
            int id=atoi(command[1]);
            pinfo_id(home_dir,id);
        }
    }
    else if(strcmp("history",command[0])==0){
        FILE* ptr=fopen("history.txt","a");
        fclose(ptr);
        if(command[1]==NULL)display_history(10);
        else display_history(atoi(command[1]));
    }
    else if(strcmp("discover",command[0])==0){
        discover(command,home_dir);
    }
    else if(strcmp(command[0],"exit")==0){
        _exit(-1);
    }
    else if(strcmp(command[0],"jobs")==0){
        jobs_print(command,processes,home_dir,cnt);
    }
    else if(strcmp(command[0],"fg")==0){
        fg_command(command,processes,cnt);
    }
    else if(strcmp(command[0],"bg")==0){
        bg_command(command,processes,cnt);
    }
    else if(strcmp(command[0],"sig")==0){
        sig(command,processes,cnt);
    }
    else{
        time_t begin = time(NULL);

         newprocess(command,bg,processes,cnt);
         time_t end = time(NULL);
        // double time_taken = ((double)t) / CLOCKS_PER_SEC;
        if(end-begin>1){
            printf("took %ld seconds\n", (end - begin));
        }
           
    }
}