#include "minshell.h"
void insert_job(pid_t pid,char *command)
{
    job *new=malloc(sizeof(job));
    if(new==NULL)
    {
        printf("Memory is not allocated");
        return;
    }
    job_count++;
    new->job_no=job_count;
    new->pid=pid;
    strcpy(new->command,command);
    new->status=1;
    new->link=head;
    head=new;
} 
job *find_job(int job_no)
{
    job *temp=head;
    while(temp!=NULL)
    {
        if(temp->job_no==job_no)
        {
            return temp;
        }
        temp=temp->link;
    }
    return NULL;
}
void print_jobs(void)
{
    job *temp=head;
    job *next;
    //job *prev=NULL;
    int status;
    pid_t ret;
    while(temp!=NULL)
    {
        ret=waitpid(temp->pid,&status,WNOHANG | WUNTRACED | WCONTINUED);
        if(ret==temp->pid)
        {
            if(WIFSTOPPED(status))
            {
                temp->status=0;
            }
            else if(WIFCONTINUED(status))
            {
                temp->status=1;
            }
            else if(WIFEXITED(status) || WIFSIGNALED(status))
            {
                next=temp->link;
                int job_no=temp->job_no;
                printf(ANSI_COLOR_MAGENTA"[%d]+ Done     %s\n"ANSI_COLOR_RESET,temp->job_no,temp->command);
                // if(prev==NULL)
                // {
                //     head=next;
                // }
                // else
                // {
                //     prev->link=next;
                // }
                delete_jobs(job_no);
                temp=next;
                continue;
            }
        }
        if(temp->status==0)
        {
            printf(ANSI_COLOR_MAGENTA"[%d]+ Stopped      %s\n"ANSI_COLOR_RESET,temp->job_no,temp->command);
        }
        else
        {
            printf(ANSI_COLOR_MAGENTA"[%d]+ Running     %s\n"ANSI_COLOR_RESET,temp->job_no,temp->command);
        }
        //prev=temp;
        temp=temp->link;
    }
}
void remove_job(pid_t pid)
{
    job *temp=head;
    job *prev=NULL;
    while(temp!=NULL)
    {
        if(temp->pid==pid)
        {
            if(prev==NULL)
            {
                head=temp->link;
            }
            else
            {
                prev->link=temp->link;
            }
            free(temp);
            return;
        }
        prev=temp;
        temp=temp->link;
    }
}
void delete_jobs(int job_no)
{
    job *temp=head;
    job *prev=NULL;
    while(temp!=NULL)
    {
        if(temp->job_no==job_no)
        {
            if(prev==NULL)
            {
                head=temp->link;
            }
            else
            {
                prev->link=temp->link;
            }
            free(temp);
            return;
        }
        prev=temp;
        temp=temp->link;
    }
}