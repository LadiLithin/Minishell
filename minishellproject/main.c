#include "minshell.h"
char *external_commands[155];
char prompt[50]="minishell$";
pid_t pid=0;
int job_count=0;
job *head=NULL;

int main()
{
    char input_string[100];
    system("clear");

    signal(SIGINT,signal_handler);
    signal(SIGTSTP,signal_handler);
    
    extract_external_commands(external_commands);
    scan_input(prompt,input_string);
    return 0;
} 