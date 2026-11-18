#include "minshell.h"
void scan_input(char *prompt, char *input_string)
{
    while(1)
    {
        int status;
        int background;
        int len;
        char *command;
        int type;
        printf(ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET ,prompt);
        memset(input_string,'\0',100);
        fgets(input_string,100,stdin);
        if(input_string[0]=='\n')
        {
            continue;
        }
        input_string[strcspn(input_string,"\n")]='\0';
        char *str=strstr(input_string,"PS1=");
        if(str!=NULL)
        {
            if(input_string[4]!='\0' && input_string[4]!=' ')
            {
                strcpy(prompt,input_string+4);
            }
            else
            {
                printf(" Command %s not found\n",input_string+4);
            }
            continue;
        }
        background=0;
        len=strlen(input_string);
        while(len>0 && (input_string[len-1]==' '||input_string[len-1]=='\t'))
        {
            input_string[len-1]='\0';
            len--;
        }
        if(len>0 && input_string[len-1]=='&')
        {
            background=1;
            input_string[len-1]='\0';
            len--;
            while(len>0 && (input_string[len-1]==' '||input_string[len-1]=='\t'))
            {
                input_string[len-1]='\0';
                len--;
            }
        }
        command=get_command(input_string);
        type=check_command_type(command);
        if(type==EXTERNAL)
        {
            pid=fork();
            if(pid>0)
            {
                if(background==1)
                {
                    insert_job(pid,input_string);
                    printf("[%d] %d\n",head->job_no,pid);
                    pid=0;
                }
                else
                {
                    int ret;
                    ret=waitpid(pid,&status,WUNTRACED);
                    if(ret==-1)
                    {
                        perror("waitpid");
                    }
                    else if(WIFSTOPPED(status))
                    {
                        insert_job(pid,input_string);
                        job *temp=find_job(job_count);
                        if(temp!=NULL)
                        {
                            temp->status=0;
                        }
                        printf(ANSI_COLOR_MAGENTA"\n[%d]+ Stopped    %s\n"ANSI_COLOR_RESET,head->job_no,head->command);
                    }
                    pid=0;
                }
            }
            else if(pid==0)
            {
                signal(SIGINT,SIG_DFL);
                signal(SIGTSTP,SIG_DFL);
                execute_external_cmd(input_string);
                exit(1);
            }
        }
        else if(type==BUILTIN)
        {
            execute_internal_commands(input_string);    
        }
        else
        {
            printf("Command not found\n");
        }
        free(command);
    }   
}
void signal_handler(int signum)
{
    if(signum==SIGINT)
    {
        if(pid==0)
        {
            printf(ANSI_COLOR_GREEN"\n%s"ANSI_COLOR_RESET,prompt);
            fflush(stdout);
        }
    }
    else if(signum==SIGTSTP)
    {
        if(pid==0)
        {
            printf(ANSI_COLOR_GREEN"\n%s"ANSI_COLOR_RESET,prompt);
            fflush(stdout);
        }
    }
    else if(signum==SIGCHLD)
    {
        delete_jobs(head->job_no);
    }
}