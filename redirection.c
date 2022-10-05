#include "headers.h"

void redirection( char *home_dir, char *prev_dir,int processes[],int * cnt, int bg,char** commands){
    int a = 0;
    int inflag = 0;
    int outflag = 0;
    char inp_file[1000];
    char out_file[1000];
    int p, q, ptr;
    // int x,y,z;
    int y = 1;
    int j = 0;
    int f1=0;
    int f2=0;
    char **parameters = (char **)malloc(100 * sizeof(char *));
    for (int i = 0; i < 100; i++){
        parameters[i] = NULL;
    }
    int pipe_flag = 0;
    int pipe_count = 0;
    for (int i = 0; commands[i] != NULL; i++){
        if (strcmp(commands[i], "<") == 0)
        {
            inflag = 1;
            strcpy(inp_file, commands[i + 1]);
        }
        if (strcmp(commands[i], ">") == 0)
        {
            outflag = 1;
            // printf("hello2\n");
            strcpy(out_file, commands[i + 1]);
        }
        if (strcmp(commands[i], ">>") == 0)
        {
            outflag = 1;
            // printf("hello3\n");
            strcpy(out_file, commands[i + 1]);
            a = 1;
        }
        if (strcmp("|", commands[i]) == 0)
        {
            // printf("hello4\n");
            pipe_count++;
            pipe_flag = 1;
        }
    }
    // no redirections and piping
    if (!inflag && !outflag && !pipe_count)
    {
        // printf("hello_error\n");
        runcommand(commands, home_dir, prev_dir, bg,processes,cnt);
        return;
    }
    // A pipe is a system call that creates a unidirectional communication link between two file descriptors.
    //  The pipe system call is called with a pointer to an array of two integers. Upon return, the first element
    //  of the array contains the file descriptor that corresponds to the output of the pipe (stuff to be read).
    // The second element of the array contains
    //  the file descriptor that corresponds to the input of the pipe (the place where you write stuff).
    // Whatever bytes are sent into the input of the pipe can be read from the other end of the pipe.
    int read_write[2 * pipe_count];
    if (pipe_count > 0)
    {
        int x = 0;
        while (x < 2 * pipe_count)
        {
            pipe(read_write + x);
            x += 2;
        }
    }
    // for(int i=0;commands[i]!=NULL;i++){
    //     printf("%s\n",commands[i]);
    // }
    for (int i = 0; commands[i] != NULL; i++)
    {
        // printf("hurrah\n");
        // printf("%d %d\n",outflag,inflag);
        // printf("%s\n",commands[i]);
        // printf("%s\n",commands[i]);
        // parameters[j]=NULL;cat 
        if (inflag)
        {
            if (strcmp(commands[i], "|") == 0)
            {
                j=0;
                pipe_count--;
            }
            if (strcmp(commands[i], "<") == 0)
            {
                // printf("hello motto\n");
                ptr = open(inp_file, O_RDONLY);
                if (ptr < 0)
                {
                    printf("NO such file\n");
                    return;
                }
                if (pipe_count > 0)
                {
                    f1=1;
                    int se=fork();
                    if (se){
                        wait(NULL);
                        close(read_write[y]);
                        
                    }
                    else
                    {
                        p = dup2(ptr, 0);
                        q = dup2(read_write[y], 1);
                        // commands[j]
                        execvp(parameters[0], parameters);
                        close(read_write[y - 1]);
                        
                    }
                }
                else if (outflag == 1)
                {
                    // printf("outflag\n");
                    if (fork() == 0)
                    {
                        int p = dup2(ptr, 0);
                        if (a)
                        {
                            int ptr1 = open(out_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
                            if (ptr1 >= 0)
                            {
                                int q = dup2(ptr1, 1);
                                execvp(parameters[0], parameters);
                            }
                        }
                        else
                        {
                            int ptr1 = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                            if (ptr1 >= 0)
                            {
                                int q = dup2(ptr1, 1);
                                execvp(parameters[0], parameters);
                            }
                        }
                    }
                    return;
                }
                else
                {
                    if (fork() == 0)
                    {
                        int p = dup2(ptr, 0);
                        execvp(parameters[0], parameters);
                    }
                    else                    {
                        wait(NULL);
                    }
                    return;
                }
            }
        }
        if ((!strcmp(commands[i], ">>") || !strcmp(commands[i], ">")) && pipe_flag)
        {
            if (fork() == 0)
            {
                int p = dup2(read_write[y - 1], 0);
                close(read_write[y]);
                if (a)
                {
                    int ptr1 = open(out_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
                    if (ptr1 >= 0)
                    {
                        int q = dup2(ptr1, 1);
                        execvp(parameters[0], parameters);
                    }
                    else{
                        wait(NULL);
                    }
                }
                else
                {
                    int ptr1 = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                    if (ptr1 >= 0)
                    {
                        int q = dup2(ptr1, 1);
                        execvp(parameters[0], parameters);
                    }
                    else{
                        wait(NULL);
                    }
                }
            }
            else
            {
                wait(NULL);
                close(read_write[y - 1]);
            }
        }
        if(!strcmp(commands[i], "|") && pipe_flag == 1)
        {
            int p,q;
            pipe_count--;
            
            if(f2 == 0 && f1 == 1){
                f2 = 1;
            }
            else if(f1 == 1)
            {
                y += 2;
                int se=fork();
                if(se)
                {
                    // commands[j] = NULL;
                     wait(NULL);
                    close(read_write[y]);
                    close(read_write[y- 3]);
                }
                else
                {
                    q = dup2(read_write[y], 1);
                    p = dup2(read_write[y - 3], 0);
                    
                    close(read_write[y - 2]);
                    close(read_write[y - 1]);
                    execvp(parameters[0], parameters);
                    exit(0);
                   
                }
            }
            else{
                if(fork() == 0){
                    // for(int i=0;parameters[i]!=NULL;i++){
                    //     printf("%s ",parameters[i]);
                    // }printf("\n");
                    int p = dup2(read_write[y], 1);
                    execvp(parameters[0], parameters);
                    // printf("%s: Command not found\n", commands[0]);
                    close(read_write[y - 1]);
                    exit(0);
                }
                else
                {
                    wait(NULL);
                    close(read_write[y]);
                }
                f1 = 1;
                f2 = 1;
            }
            j = 0;
        }
        if (pipe_flag == 0 && inflag == 0 ){
            
            if (strcmp(commands[i], ">>")==0)
            {
                int ptr = open(out_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
                if (fork() == 0){
                    // printf("hello motto\n");
                    // printf("%s\n",parameters[0]);
                    // for(int i=1;parameters[i]!=NULL;i++){
                    //     printf("%s ",parameters[i]);
                    // }printf("\n");
                    int q = dup2(ptr, 1);
                    execvp(parameters[0], parameters);
                    // printf("%s: Command not found\n", commands[0]);
                    exit(0);
                }
                return;
            }
            else if (strcmp(commands[i], ">")==0)
            {
                int ptr = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                if (fork() == 0){
                    // printf("%s\n", parameters[0]);
                    // for (int i = 0; parameters[i] != NULL; i++)
                    // {
                    //     printf("%s ", parameters[i]);
                    // }
                    // printf("\n");
                    int q = dup2(ptr, 1);
                    execvp(parameters[0], parameters);
                    // printf("%s: Command not found\n", commands[0]);
                    exit(0);
                }
                else{
                    wait(NULL);
                }
                return;
            }
        }

        if (strcmp(commands[i], "|") != 0 && strcmp(commands[i], "<") != 0 && strcmp(commands[i], ">") != 0 && strcmp(commands[i], ">>") != 0)
        {
            parameters[j] = (char *)malloc(strlen(commands[i]) + 1);
            strcpy(parameters[j], commands[i]);
            j++;
        }
    }
    if(pipe_flag== 1 && outflag == 0)
    {
        if(fork() == 0){
            // commands[j] = NULL;
            int x = dup2(read_write[y - 1], 0);
            close(read_write[y]);
            execvp(parameters[0], parameters);
            // printf("%s: Command not found\n", commands[0]);
            exit(0);
        }
        else{
            wait(NULL);
            close(read_write[y- 1]);
        }
    }
}
