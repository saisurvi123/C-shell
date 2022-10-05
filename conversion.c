#include"headers.h"
void convert_to_tilda1(char* home_dir,char* current_path,char* final_path){
    int f=0;
    for(int i=0;i<strlen(home_dir);i++){
        if(home_dir[i]!=current_path[i]){
            f=1;
            break;
        }
    }
    if(f){
        strcpy(final_path,current_path);
        return;
    }
    final_path[0]='~';
    int s=strlen(home_dir);
    int e=strlen(current_path)-1;
    int x=1;
    for(int i=s;i<=e;i++){
        final_path[x++]=current_path[i];
    }
    final_path[x]='\0';
}
void convert_to_tilda2(char* home_dir,char* path,char* final_path,char* final_prev){
    strcpy(final_path,home_dir);
    if(path[0]=='~'){
        strcat(final_path,path+1);
        // printf("%s\n",final_path);
    }
    else if(path[0]=='-'){
        strcpy(final_path,final_prev);
    }
    else{
        strcpy(final_path,path);
        strcpy(final_prev,final_path);
    }
    // if(getcwd(final_prev,sizeof(final_prev))==NULL){
    //     printf("error in finding current path");
    //     // return -1;
    // }
    getcwd(final_prev,PATH_MAX);
    // printf("%s",final_prev);
}