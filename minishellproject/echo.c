#include "minshell.h"
void echo(char *input_string,int status)
{
    char *str;
    str=input_string+4;
    while(*str==' '||*str=='\t')
    {
        str++;
    }
    if(*str=='\0')
    {
        return;
    }
    if(strcmp(str,"$$")==0)
    {
        printf("%d\n",getpid());
    }
    else if(strcmp(str,"$?")==0)
    {
        printf("%d\n",status);
    }
    else if(strcmp(str,"$SHELL")==0)
    {
        char *shell=getenv("SHELL");
        if(shell!=NULL)
        {
            printf("%s\n",shell);
        }
    }
    else
    {
        printf("%s\n",str);
    }
}