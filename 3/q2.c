#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUB_COMMANDS 5
#define MAX_ARGS 10


typedef struct SubCommand
{
	char *line;
	char *argv[MAX_ARGS];
} SubCommand;


typedef struct Command
{
	SubCommand sub_commands[MAX_SUB_COMMANDS];
	int num_sub_commands;
} Command;


void get_args(char *in, char **argv, int size)
{
	unsigned int endline = strlen(in) - 1;
	if(in[endline] == '\n')
	{
		in[endline] = '\0';
	}

	char* next = strtok(in, " ");
	
	int i=0;
	for(i=0; i<size-1 && next != NULL; i++)
	{
		argv[i] = strdup(next);
		next = strtok(NULL, " ");
	}
	argv[i] = NULL;
}

void print_args(char **argv)
{
	int i;
	for(i=0; argv[i] != NULL; i++)
	{
		printf("argv[%d] = '%s'\n", i, argv[i]);
	}
}

void ReadCommand(char *line, Command *command)
{
	char* next = strtok(line, "|");
	while(command->num_sub_commands < MAX_SUB_COMMANDS && next != NULL)
	{
		command->sub_commands[command->num_sub_commands].line = strdup(next);
		command->num_sub_commands++;
		next = strtok(NULL, "|");
	}

	int i;
	for(i=0; i<command->num_sub_commands; i++)
	{
		get_args(command->sub_commands[i].line, command->sub_commands[i].argv, MAX_ARGS);
	}
}

void PrintCommand(Command *command)
{
	int i;
	for(i=0; i<command->num_sub_commands; i++)
	{
		printf("Sub command #%d\n", i);
		print_args(command->sub_commands[i].argv);
	}
}

int main()
{
	 char s[200];

	 printf("Enter a string: \n");
	 fgets(s, sizeof s, stdin);

	 Command command;
	 command.num_sub_commands = 0;

	 ReadCommand(s, &command);
	 PrintCommand(&command);

	 return 0;
}
