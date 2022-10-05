#include"headers.h"
void pinfo_id(char* home_dir,int pid){
    char str[100];
    sprintf(str,"%d",pid);
    char path_info[PATH_MAX]="/proc/";
    char temp[PATH_MAX];
    strcat(path_info,str);
    strcpy(temp,path_info);
    strcat(path_info,"/stat");
    char temp1[1000];
    printf("pid : %d\n",pid);
    FILE* ptr=fopen(path_info,"r");
    int f=0;
    long long int tpgid,pgrp;
    fscanf(ptr, "%*s %*s %*c %*s %lld %*s %*s %lld", &pgrp, &tpgid);
    if(tpgid==pgrp){
        f=1;
    }
    fclose(ptr);
    ptr=fopen(path_info,"r");
    if(ptr!=NULL){
        fscanf(ptr,"%s",temp1);
        int i=0;
        while(i<2){
            fscanf(ptr,"%s",temp1);
            i++;
        }
        printf("process Status : %s",temp1);
        if(f){
            printf("+");
        }
        printf("\n");
    }
    else{
        printf("No process with given pid\n");
        return;
    }
    fclose(ptr);
    strcpy(path_info,temp);
    strcat(path_info,"/statm");
    ptr=fopen(path_info,"r");
    if(ptr==NULL){
        printf("No process with given pid\n");
        return;
    }
    fscanf(ptr,"%s",temp1);
    printf("memory : %s {virtual memory}\n",temp1);
    
    // printf("\n");
    strcpy(path_info,temp);
    strcat(path_info,"/exe");
    char path_exec[PATH_MAX];
    char out[PATH_MAX];
    readlink(path_info,path_exec,PATH_MAX);
    // printf("executable Path : %s\n",path_exec);
    convert_to_tilda1(home_dir,path_exec,out);
    printf("executable Path : %s\n",out);


    






}