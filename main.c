#include "headers.h"
// void checker(){
//     DIR *drr=opendir("/proc");
//     if(drr==NULL){
//         return;
//     }
//     struct dirent *dir;
//     char name[1000];
//     char temp[10000];
//     char* name_list[1000];
//     char st;
//     int pid,x;
//     int pid_list[1000];
//     int l=0;
//     while((dir=readdir(drr))!=NULL){
//         if(dir->d_name[0]>='0' && dir->d_name[0]<='9')
//         {
//             strcpy(temp,"/proc/");
//             strcat(temp,dir->d_name);
//             strcat(temp,"/stat");
//             FILE *ptr=fopen(temp,"r");
//             if(ptr!=NULL){
//                 int id=getpid();
//                 fscanf(ptr,"%d",&pid);
//                 fscanf(ptr," %s",name);
//                 fscanf(ptr," %c ",&st);
//                 fscanf(ptr," %d ",&x);
//                 // printf("%s %d\n",name,pid);
//                 if(x==id){
//                     pid_list[l]=pid;
//                     name_list[l]=(char*)malloc(strlen(name)+1);
//                     strcpy(name_list[l],name);
//                     l++;
//                 }
//             }
//             fclose(ptr);
//         }
//     }
//     closedir(drr);
//     int code;
//     char buffer[1000];
//     char output[10000];
//     int fl;
//     while((fl=waitpid(-1,&code,WNOHANG))>0){
//         // printf("%d\n",fl);
//         int flag=0;
//         if (WIFEXITED(code)) {
//             flag=1;
//         }
//         strcpy(name," ");
//         int j=0;
//         while(j<l){
//             if(pid_list[j]==fl){
//                 strcpy(name,name_list[j]);

//                 // printf("hurray\n");
//                 break;
//             }
//             j++;
//         }
//         if(flag){
//             printf("\n%s with pid %d has exited normally\n",name,fl);
//         }
//         else{
//             printf("\n%s with pid %d has exited abnormally\n",name,fl);
//         }
//     }

// }
// void sigintHandler(int sig_num)
// {
//     if(fg_flag==1)
//     {
//         if(kill(foreground_pid,9)==0)
//             fg_flag = 0;
//         return;
//     }
//     else
//     {
// 	   signal(SIGINT,sigintHandler);
// 	   fflush(stdout);
//     }
// }

// void sigstpHandler(int sig_num)
// {
//     if(fg_flag==1)
//     {
//         if(kill(foreground_pid,SIGTSTP)==0)
//             ctrz = 1;
//         return;
//     }
//     else
//     {
//        signal(SIGSTOP,sigstpHandler);
//        fflush(stdout);
//     }
// }
void die(const char *s)
{
    perror(s);
    exit(1);
}
struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void signalHandler2(int sig)
{
    return;
}
void signalHandler1(int sig)
{
    return;
}
void parsing_space_and_colon(char *home_dir, char *inp, int bg, char *prev_dir, int processes[], int *cnt)
{
    char temp[MAX_INPUT];
    strcpy(temp, inp);
    char *statements[10000];
    int i = 0;
    char *str = strtok(temp, ";");
    while (str != NULL)
    {
        statements[i++] = str;
        str = strtok(NULL, ";");
    }
    statements[i] = '\0';
    // dividing with respect to tab and spaces
    char *command[10000];
    for (int j = 0; j < i; j++)
    {
        char *str = strtok(statements[j], " \t");
        int y = 0;
        while (str != NULL)
        {
            command[y++] = str;
            // printf("%s\n",str);
            str = strtok(NULL, " \t");
        }
        command[y] = NULL;
        // printf("%s\n",command[0]);
        // if(bg){
        //     fg_flag=0;
        // }
        // else{
        //     fg_flag=1;
        // }
        redirection( home_dir, prev_dir, processes, cnt,bg,command);
    }
}


int main()
{
    char home_dir[PATH_MAX];
    char prev_dir[PATH_MAX];
    int processes[2000];
    // get the present dir from where u are invoking shell
    getcwd(home_dir, sizeof(home_dir));
    // signal(SIGCHLD, func1);
    strcpy(prev_dir, home_dir);
    // ctrz=0;
    // printf("%s\n",home_dir);
    for (int i = 0; i < 2000; i++)
    {
        processes[i] = -1;
    }
    int stat;
    int cnt = 0;
    // signal(SIGINT, SIG_IGN);
    // signal(SIGTSTP, SIG_IGN);
    // signal(SIGTTOU, SIG_IGN);
    char ** initial;
    initial=(char**)malloc(sizeof(char*)*2);
    initial[0]=(char*)malloc(strlen("clear")+1);
    strcpy(initial[0],"clear");
    initial[1]=NULL;
    runcommand(initial,home_dir,home_dir,0,processes,&cnt);
    printf("\033[1m\033[33m\n            *** Welcome to My Shell  ***           \n\033[0m");
    printf("\n");
    while (1)
    {
        fg_flag = 0;
        signal(SIGINT, signalHandler2);
        signal(SIGTSTP, signalHandler1);        
        int x = display_shell(home_dir);
        char inp[MAX_INPUT];
        // getchar();
        if (x == -1)
        {
            printf("error in displaying shell\n");
            return 0;
        }

        char c;

        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 27)
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2){ 
                        // printf("\rarrow key: %s", buf);
                    }
                }
                else if (c == 127){ // backspace
                    if (pt > 0)
                    {
                        if (inp[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9)
                { // TAB character
                    // inp[pt++] = c;
                    char curr_dir[PATH_MAX];
                    getcwd(curr_dir, sizeof(curr_dir));
                    DIR *drr;
                    struct dirent *dir;
                    drr = opendir(curr_dir);
                    char target[1000];
                    int y=0;
                    for(int i=strlen(inp)-1;i>=0;i--){
                        if(inp[i]==' ')break;
                        target[y++]=inp[i];
                    }
                    // printf("%d\n",y);
                    target[y]='\0';
                    // printf("%s\n",target);
                    char* match_list[1000];
                    for(int i=0;i<y/2;i++){
                        char t=target[i];
                        target[i]=target[y-i-1];
                        target[y-i-1]=t;                        
                    }

                    int z=0;
                    while ((dir = readdir(drr)) != NULL)
                    {
                        if(strncmp(dir->d_name,target,y)==0){
                            match_list[z]=(char*)malloc(strlen(dir->d_name)+1);
                            strcpy(match_list[z],dir->d_name);
                            z++;
                        }
                        // printf("%s\n",dir->d_name);
                        
                    }
                    closedir(drr);
                    if(z==1){
                        for(int i=y;i<strlen(match_list[0]);i++){
                            printf("%c",match_list[0][i]);
                            inp[pt++]=match_list[0][i];
                        }
                        
                    }
                    else if(z>1){
                        printf("\n");
                        int minima=INT_MAX;
                        for(int i=0;i<z;i++){
                            printf("%s\n",match_list[i]);
                            if(minima>strlen(match_list[i])){
                                minima=strlen(match_list[i]);
                            }
                        }
                        // printf("%d\n",minima);
                        // printf("%dy %dz\n",y,z);
                        for(int col=y;col<minima;col++){
                            int f=1;
                            for(int row=1;row<z;row++){
                                if(match_list[row-1][col]!=match_list[row][col]){
                                    f=0;
                                    break;
                                }
                            }
                            if(!f){
                                break;
                            }
                            else{
                                inp[pt++]=match_list[0][col];
                            }
                        }
                        x = display_shell(home_dir);
                        printf("%s",inp);
                    }
                }
                else if (c == 4){
                    exit(0);
                }
                else{
                    printf("%d\n", c);
                }
            }
            else
            {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        // scanf(" %[^\n]", inp);
        printf("\x1b[37m");
        printf("\n");
        // dividing string using strtok
        char temp[MAX_INPUT];
        strcpy(temp, inp);
        add_history(inp);
        char *start = temp;
        char *statements[10000];
        int i = 0;
        char *str = strtok(temp, "&");
        while (str != NULL && strcmp(str, "") != 0)
        {
            statements[i++] = str;
            str = strtok(NULL, "&");
        }
        statements[i] = '\0';
        for (int j = 0; j < i; j++)
        {
            int bg = 0;
            if (inp[strlen(statements[j]) + (statements[j] - start)] == '&')
            {
                bg = 1;
            }
            parsing_space_and_colon(home_dir, statements[j], bg, prev_dir, processes, &cnt);
        }
        int chk = waitpid(-1, &stat, WNOHANG);
        if (chk > 0)
        {
            if (!WIFEXITED(stat))
            {
                for (int i = 0; i < 1000; i++)
                {
                    if (processes[i] == chk)
                    {
                        processes[i] = -1;
                    }
                }
                printf("\nProcess with pid %d exited abnormally\n", chk); // checking for process ending abnormally
            }
        }
       
        int j=0;
        while(j<1000){
            if (kill(processes[j], 0) == -1)
            {
                printf("\nProcess with pid %d exited noramlly\n", processes[j]);
                processes[j] = -1;
            }
            j++;
        }
        // int arr[1000];
        // for(int i=0;i<1000;++i)
        // {
        // 	int status;
        // 	if(arr[i]>0 && waitpid(arr[i],&status,WNOHANG) == arr[i])
        // 	{
        // 		// printf("%s with pid %d exited normally\n",namepid[i],ar[i]);
        // 		arr[i] = -1;
        // 		// namepid[i] = NULL;
        // 	}
        // }
    }

    return 0;
}