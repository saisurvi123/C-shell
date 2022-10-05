// include "headers.h"


# include "headers.h"
void getpermissions(char file1[],char per_str[]){
    struct stat stats;
    stat(file1,&stats);
    int i=0;
    if(S_ISDIR(stats.st_mode)){
        per_str[i++]='d';
    }
    else if(S_ISREG(stats.st_mode))
        per_str[i++]='-';
    else
        per_str[i++]='l';
    if(stats.st_mode & S_IRUSR){
        per_str[i++]='r';
    }
    else{
        per_str[i++]='-';
    }
    if(stats.st_mode & S_IWUSR){
        per_str[i++]='w';
    }
    else{
        per_str[i++]='-';
    }
    if(stats.st_mode & S_IXUSR){
        per_str[i++]='x';
    }
    else{
        per_str[i++]='-';
    }
   
    if(stats.st_mode & S_IRGRP){
        per_str[i++]='r';
    }
    else{
        per_str[i++]='-';
    }
    if(stats.st_mode & S_IWGRP){
        per_str[i++]='w';
    }
    else{
        per_str[i++]='-';
    }
    if(stats.st_mode & S_IXGRP){
        per_str[i++]='x';
    }
    else{
        per_str[i++]='-';
    }
   
    if(stats.st_mode & S_IROTH){
        per_str[i++]='r';
    }
    else{
        per_str[i++]='-';
    }
    if(stats.st_mode & S_IWOTH){
        per_str[i++]='w';
    }
    else{
        per_str[i++]='-';
    }
    if(stats.st_mode & S_IXOTH){
        per_str[i++]='x';
    }
    else{
        per_str[i++]='-';
    }
    per_str[i++]='\0';
    
}
void print_time(char *file){
    struct stat stats;
    stat(file,&stats);
    struct passwd * pass_w;
    pass_w = getpwuid(stats.st_uid);
    struct group* group1;
    group1 = getgrgid(stats.st_gid);
    char str[100];
    strftime(str, sizeof(str), "%b %d %H:%M", localtime(&stats.st_mtime)); 
    // printf("%s ",str);
    int u=stats.st_nlink;
    int v=stats.st_size; 
    printf("%d ",u);
    printf("%s ",pass_w->pw_name);
    printf("%s ",group1->gr_name);
    printf("%d %s ",v,str);   
    // printf("%d %s %s %d %s %s\n",u, pass_w -> pw_name, group1 -> gr_name,v, str, file1);
}
int compare(const void *a, const void *b) { 
    const char **str_a = (const char **)a;
    const char **str_b = (const char **)b;
    return strcasecmp(*str_a, *str_b);
}
void color_code(char file[]){
    struct stat stats;
    stat(file,&stats);
    if(S_ISDIR(stats.st_mode)){
        printf("\x1b[34m");
    }
    else if(stats.st_mode & S_IXUSR){
        printf("\x1b[32m");
    }
    else printf("\x1b[37m");
    
} 
void helper(char* home_dir,int af,int lf,char* dir_name){
    char permissions_str[10000];
    struct stat check;
    stat(dir_name,&check);
    char *list[10000];
    int x=0;
    if(S_ISDIR(check.st_mode)){
        // printf("welcome to our directory\n");
        DIR *drr;
        struct dirent *dir;
        char temp[PATH_MAX];
        int len=strlen(dir_name);
        // we simply reallocated the memory of directory name
        if(dir_name[len-1]!='/'){
            char* temp2=(char*)malloc((len+2)*sizeof(char));
            strcpy(temp2,dir_name);
            temp2[len]='/';
            temp2[len+1]='\0';
            dir_name=temp2;
        }
            // printf("hello\n");
        drr = opendir(dir_name);
        strcpy(temp,dir_name);
            // printf("hello\n");
        if(lf==0){
            // printf("hello1\n");
            while ((dir = readdir(drr)) != NULL){
                if((dir -> d_name[0] == '.' && af == 1) || (dir -> d_name[0] != '.')){
                    list[x]=(char*)malloc(strlen(dir->d_name)+1);
                    strcpy(list[x++],dir->d_name);
                    // printf("%s\n", dir->d_name);
                }
                
            }
            qsort(list,x,sizeof(char*),compare);
            for(int i=0;i<x;i++){
                strcat(temp, list[i]);
                color_code(temp);
                printf("%s\n",list[i]);
                printf("\x1b[37m");
                strcpy(temp, dir_name);
            }
            closedir(drr);
        }
        else if(lf==1){
            // printf("hello\n");
            x=0;
            while((dir = readdir(drr)) != NULL)
            {
                if((dir -> d_name[0] == '.' && af == 1) || (dir -> d_name[0] != '.')){
                    list[x]=(char*)malloc(strlen(dir->d_name)+1);
                    strcpy(list[x++],dir->d_name);    
                    // strcat(temp,dir->d_name);
                }
            }
            // printf("hello2\n");
            qsort(list,x,sizeof(char*),compare);
            for(int i=0;i<x;i++){
                strcat(temp, list[i]);
                    // printf("%s\n",temp);
                getpermissions(temp,permissions_str);
                printf("%s ",permissions_str);
                print_time(temp);
                color_code(temp);
                printf("%s\n",list[i]);
                printf("\x1b[37m");
                    // detailedPrint(duplicate, dir -> d_name);
                strcpy(temp, dir_name);
            }
            closedir(drr);
        }
    }
    else{
        // printf("entered\n");
        // we dont care of -a for files
        // printf("hello3\n");
        if(lf==0){
            // printf("hello\n");
            color_code(dir_name);
            printf("%s\n",dir_name);
            printf("\x1b[37m");
        }
        else if(lf==1){
            getpermissions(dir_name,permissions_str);
            printf("%s ",permissions_str);
            print_time(dir_name);
            color_code(dir_name);
            printf("%s\n",dir_name);
            printf("\x1b[37m");

        }
    }
}
void changepath(char* home_dir,char* given_path,char* final_path){
    if(given_path[0]=='~'){
        strcpy(final_path,home_dir);
        strcat(final_path,given_path+1);
    }
    else{
        strcpy(final_path,given_path);
    }
}

void ls_command(char* home_dir,char** commands){
    // findig no of commands
    int lf=0;
    int af=0;
    int command_len=0;
    for(int i=0;commands[i]!=NULL;i++){
        // printf("%s\n",commands[i])
        command_len++;
    }
    // printf("%d\n",command_len);
    int x;
    optind=0;
    while((x=getopt(command_len,commands,"la"))!=-1){
        // printf("%c\n",x);
        if(x=='a'){
            af=1;
        }
        else if(x=='l'){
            lf=1;
        }
        else if(x=='?'){
            return;
        }
    }
    // printf("%d %d\n",lf,af);
    int f=0;
    int k=0;
    char path[PATH_MAX];
    struct stat stats;
    
    while(k<command_len){
        if(strcmp(commands[k],"ls")!=0 && commands[k][0]!='-'){
            changepath(home_dir,commands[k],path);
            if(stat(path,&stats)<0){
                printf("No such file or directory to execute ls\n");
            }
            else{
                helper(home_dir,af,lf,path);
            }
            f=1;
        }

        k++;
    }    


    // direct full ls thingss
    if(!f){
        helper(home_dir,af,lf,".");
    }

    
}
