#include "headers.h"
int recog(char* temp){
    struct stat stats;
    int x=stat(temp,&stats);
    if(strcmp("-f",temp)==0)return 1;
    else if(strcmp("-d",temp)==0)return 2;
    else if((x!=-1 && S_ISDIR(stats.st_mode) )|| temp[0]=='~' || temp[0]=='.' )return 3;
    else return 4;
}
void recur_find(char* dir_name,char* file,int fl){
    DIR *drr;
    struct dirent *dir;
    drr = opendir(dir_name);
    while ((dir = readdir(drr)) != NULL){
        // printf("%s\n",dir->d_name);
        char temp[PATH_MAX];
        strcpy(temp,dir_name);
        strcat(temp,dir->d_name);
        // printf("%s\n",temp);
        if(temp[strlen(temp)-1]!='/')strcat(temp,"/");
        struct stat stats;
        int x=stat(temp,&stats);
        // printf("%d\n",x);
        if(x!=-1 && S_ISDIR(stats.st_mode) && strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")){
            recur_find(temp,file,fl);
        }
        else if(strcmp(dir->d_name,file)==0){
            printf("%s%s\n",dir_name,file);
            return;
        }
    }
    closedir(drr);
}
void print_all(char* dir_name,int f,int d){
    DIR *drr;
    struct dirent *dir;
    drr = opendir(dir_name);
    while ((dir = readdir(drr)) != NULL){
        // printf("%s\n",dir->d_name);
        char temp[PATH_MAX];
        strcpy(temp,dir_name);
        strcat(temp,dir->d_name);
        struct stat stats;
        int x=stat(temp,&stats);
        // printf("%d\n",x);
        if(x!=-1 && S_ISDIR(stats.st_mode) && strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")){
            if(d){
                printf("%s\n",temp);
            }
            if(temp[strlen(temp)-1]!='/')strcat(temp,"/");
            print_all(temp,f,d);
        }
        else{
            if(f && strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")){
                printf("%s\n",temp);
            }
        }
    }
    closedir(drr);

}

void discover(char** commands,char* home_dir){
    int file=1;
    int dir=1;
    char curr_dir[PATH_MAX]="./";
    char *req_file=NULL;
    int flag=0;
    for(int i=1;commands[i]!=NULL;i++){
        int code=recog(commands[i]);
        if(code==1){
            if(file==1 && dir==1){
                file=1;
                dir=0;
            }
            else if(file==0 && dir==1){
                file=1;
                dir=1;
            }
        }
        else if(code==2){
            if(file==1 && dir==1){
                file=0;
                dir=1;
            }
            else if(file==1 && dir==0){
                file=1;
                dir=1;
            }
        }
        else if(code==3){

            if(commands[i][0]=='~'){
                strcpy(curr_dir,home_dir);
                strcat(curr_dir,commands[i]+1);
            }
            else{
                strcpy(curr_dir,commands[i]);
            }
            if(curr_dir[strlen(curr_dir)-1]!='/')strcat(curr_dir,"/");
            // printf("%s\n",curr_dir);
        }
        else{
            // printf("hello\n");
            req_file=(char*)malloc(strlen(commands[i]));
            strcpy(req_file,commands[i]+1);
            req_file[strlen(req_file)-1]='\0';
            flag=1;
            file=0;
            dir=0;
        }
        // printf("%d\n",code);
    }
    // printf("%s\n",curr_dir);
    if(!flag){
        if(dir){
            printf("%s\n",curr_dir);
        }
        print_all(curr_dir,file,dir);
    }
    else{
        // printf("searching for a file\n");
        // printf("%s\n",curr_dir);
        // printf("%s\n",req_file);
        recur_find(curr_dir,req_file,0);
    }
    // printf("%d %d %s %s\n",file,dir,curr_dir,req_file);    
}
