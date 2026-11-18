#include "minshell.h"

void execute_internal_commands(char *input_string)
{
    char *command;
    command=get_command(input_string);
    if(command==NULL)
    {
        return;
    }
    if(strcmp(command,"exit")==0)
    {
        free(command);
        exit(0);
    }
    else if(strcmp(command,"pwd")==0)
    {
        char path[1000];
        if(getcwd(path,sizeof(path))!=NULL)
        {
            printf("%s\n",path);
        }
    }
    else if(strcmp(command,"echo")==0)
    {
        echo(input_string,0);
    }
    else if(strcmp(command,"cd")==0)
    {
        chdir(input_string+3);
    }
    else if(strcmp(command,"jobs")==0)
    {
        print_jobs();
    }
    else if(strcmp(command,"fg")==0)
    {
        fg_cmd(input_string);
    }
    else if(strcmp(command,"bg")==0)
    {
        bg_cmd(input_string);
    }
    
    free(command);
}