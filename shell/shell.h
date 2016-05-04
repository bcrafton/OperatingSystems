#ifndef SHELL_H_
#define SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include "list.h"

#define MAX_SUB_COMMANDS 5
#define MAX_ARGS 10
#define LAST_ARG_INDEX (MAX_ARGS - 1)


typedef struct sub_command_t
{
	char *line;
	char *argv[MAX_ARGS];
} sub_command_t;


typedef struct command_t
{
	sub_command_t sub_commands[MAX_SUB_COMMANDS];
	int num_sub_commands;
	char *stdin_redirect;
	char *stdout_redirect;
	int background;
} command_t;

void prompt();

void read_command(char *line, command_t *command);
void parse_command(struct command_t *command);
void reset_command(command_t* command);
int command_handler(command_t* command, List* l);
void execute_command(command_t* command);

unsigned char change_directory(char* new_dir);

#endif
