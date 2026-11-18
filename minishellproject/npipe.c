#include "minshell.h"

void n_pipe(char *input_string)
{
    char *commands[20];
    char *token;
    int count = 0;
    int pipefd[2];
    int prev_fd=-1;
    pid_t pid;
    token=strtok(input_string, "|");
    while (token!=NULL && count<20)
    {
        commands[count]=token;
        count++;
        token=strtok(NULL,"|");
    }
    commands[count]=NULL;
    for(int i=0;i<count;i++)
    {
        if(i<count-1)
        {
            if(pipe(pipefd)==-1)
            {
                return;
            }
        }
        pid=fork();
        if(pid==-1)
        {
            return;
        }
        if(pid==0)
        {
            if(prev_fd!=-1)
            {
                dup2(prev_fd, 0);
                close(prev_fd);
            }
            if(i<count-1)
            {
                close(pipefd[0]);
                dup2(pipefd[1],1);
                close(pipefd[1]);
            }
            char *argv[20];
            char *arg;
            int j = 0;
            arg=strtok(commands[i]," \t");
            while(arg!=NULL && j<19)
            {
                argv[j]=arg;
                j++;
                arg=strtok(NULL," \t");
            }
            argv[j] = NULL;
            execvp(argv[0], argv);
            exit(1);
        }
        if (prev_fd != -1)
        {
            close(prev_fd);
        }
        if(i<count-1)
        {
            close(pipefd[1]);
            prev_fd=pipefd[0];
        }
    }
    for(int i=0;i<count;i++)
    {
        wait(NULL);
    }
}