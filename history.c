#include "headers.h"
void add_history(char line[]){
    if(strlen(line)==0)return;
    FILE* ptr=fopen("history.txt","r");
    if(!ptr)return;
    char* lines[1000];
    int i=0;
    char *temp=(char*)malloc(1001);
    size_t len=strlen(temp);
    while(getline(&temp,&len,ptr)!=-1){
        if(temp=="\n")continue;
        lines[i]=(char*)malloc(strlen(temp)+1);
        strcpy(lines[i],temp);
        i++;
    }
    fclose(ptr);
    int index=0;
    if(i-20>0)index=i-20;
    ptr=fopen("history.txt","w");
    for(int j=index;j<i;j++){
        fprintf(ptr,"%s",lines[j]);
    }
    strcat(line,"\n");
    fprintf(ptr,"%s",line);
    fclose(ptr);
    for(int j=0;j<i;j++){
        free(lines[j]);
    }

}

void display_history(int x){
    // printf("hello\n");
    FILE* ptr=fopen("history.txt","r");
    if(!ptr)return;
    char* lines[1000];
    int i=0;
    char *temp=(char*)malloc(1001);
    size_t len=strlen(temp);
    while(getline(&temp,&len,ptr)!=-1){
        // if(strcpy(temp,"\n")==0)continue;
        // printf("%s",temp);
        lines[i]=(char*)malloc(strlen(temp)+1);
        strcpy(lines[i],temp);
        // printf("%s",temp);
        i++;
    }
    fclose(ptr);
    int index=0;
    if(i-x>0)index=i-x;
    for(int j=index;j<i;j++){
        printf("%s",lines[j]);
    }
    for(int j=0;j<i;j++){
        free(lines[j]);
    }
    
}