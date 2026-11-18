#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

extern char *external_commands[155];
extern char prompt[50];
extern pid_t pid;

typedef struct job
{
    int job_no;
    pid_t pid;
    char command[100];
    int status;
    struct job *link;
}job;

extern int job_count;
extern job *head;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);
void execute_external_cmd(char *input_string);
void signal_handler(int signum);
void copy_change(char *prompt, char *input_string);
void n_pipe(char *input_string);
int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void extract_external_commands(char **external_commands);

void insert_job(pid_t pid,char *command);
void print_jobs(void);
void remove_job(pid_t pid);
job *find_job(int job_no);
void delete_jobs(int job_no);
void fg_cmd(char *input_string);
void bg_cmd(char *input_string);

#endif