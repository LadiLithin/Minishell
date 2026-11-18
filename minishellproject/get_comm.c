#include "minshell.h"
char *get_command(char *input_string)
{
    char *command;
    int i=0;
    command=malloc(strlen(input_string)+1);
    if(command==NULL)
    {
        return NULL; 
    }
    while(input_string[i]!='\0' && input_string[i]!=' ' && input_string[i]!='\n')
    {
        command[i]=input_string[i];
        i++;
    }
    command[i]='\0';
    return command;
}
void extract_external_commands(char **external_commands)
{
    int fd=open("external.txt",O_RDONLY);
    if(fd==-1)
    {
        printf("the file is not found\n");
        return;
    }
    char buffer[50];
    char ch;
    int i=0,j=0;
    while(read(fd,&ch,1)>0)
    {
        if(ch=='\r')
        {
            continue;
        }
        if(ch!='\n')
        {
            if(i<99)
            {
                buffer[i]=ch;
                i++;
            }
        }
        else
        {
            buffer[i]='\0';
            external_commands[j]=malloc(strlen(buffer)+1);
            if(external_commands[j]==NULL)
            {
                close(fd);
                return;
            }
            strcpy(external_commands[j],buffer);
            j++;
            i=0;
        }
    }
    if(i>0)
    {
        buffer[i]='\0';
        external_commands[j]=malloc(strlen(buffer)+1);
        if(external_commands[j]!=NULL)
        {
            strcpy(external_commands[j],buffer);
            j++;
        }   
    }
    external_commands[j]=NULL;
    close(fd);
}
int check_command_type(char *command)
{
    int i=0;
    char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval","set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source", "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "jobs", "fg" ,"bg" , NULL};
    i=0;
    while(builtins[i]!=NULL)
    {
        if(strcmp(command,builtins[i])==0)
        {
            return BUILTIN;
        }
        i++;
    }
    i=0;
    while(external_commands[i]!=NULL)
    {
        if(strcmp(external_commands[i],command)==0)
        {
            return EXTERNAL;
        }
        i++;
    }
    return NO_COMMAND;
}
void execute_external_cmd(char *input_string)
{
    char *cmd_with_options[strlen(input_string)];
    char *ch;
    int i=0;
    if(strchr(input_string,'|')!=NULL)
    {
        n_pipe(input_string);
        return;
    }
    ch=strtok(input_string," \t");
    while(ch!=NULL)
    {
        cmd_with_options[i]=ch;
        i++;
        ch=strtok(NULL," \t");
    }
    cmd_with_options[i]=NULL;
    if(cmd_with_options[0]==NULL)
    {
        return;
    }
    execvp(cmd_with_options[0],cmd_with_options);
}
