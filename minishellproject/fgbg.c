#include "minshell.h"
void fg_cmd(char *input_string)
{

    int job_no;
    pid_t ret;
    char *token;
    job *temp;
    int status;
    token=strtok(input_string," \t");
    token=strtok(NULL," \t");
    if(token==NULL)
    {
        printf("INFO: fg<job number>\n");
        return;
    }
    job_no=atoi(token);
    if(job_no<=0)
    {
        printf("Invalid job number\n");
        return;
    }
    temp=find_job(job_no);
    if(temp==NULL)
    {
        printf("fg: jobs not found\n");
        return;
    }
    printf("%s\n",temp->command);
    fflush(stdout);
    if(kill(temp->pid,SIGCONT)==-1)
    {
        //signal(SIGCHLD,signal_handler);
        return;
    }
    pid=temp->pid;
    ret=waitpid(temp->pid,&status,WUNTRACED);
    if(ret==-1)
    {
        perror("fg: waitpid\n");
    }
    else if(WIFSTOPPED(status))
    {
        temp->status=0;
        printf(ANSI_COLOR_MAGENTA"[%d]+ Stopped   %s\n"ANSI_COLOR_RESET,temp->job_no,temp->command);
    }
    else if(WIFEXITED(status)||WIFSIGNALED(status))
    {
        delete_jobs(job_no);
    }
    pid=0;
}
void bg_cmd(char *input_string)
{
    int job_no;
    job *temp;
    char *token;
    token=strtok(input_string," ");
    token=strtok(NULL," ");
    if(token==NULL)
    {
        printf("Info: bg <jobs number>\n");
        return;
    }
    job_no=atoi(token);
    if(job_no<=0)
    {
        printf("Invalid job number\n");
        return;
    }
    temp=find_job(job_no);
    if(temp==NULL)
    {
        printf("Info: jobs not found\n");
        return;
    }
    if(kill(temp->pid,SIGCONT)==-1)
    {
        return;
    }
    temp->status=1;
    printf(ANSI_COLOR_MAGENTA"[%d]+ Running    %s &\n"ANSI_COLOR_RESET,temp->job_no,temp->command);
}