#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUB_COMMANDS 5
#define MAX_ARGS 10
#define LAST_ARG_INDEX (MAX_ARGS - 1)


typedef struct SubCommand
{
	char *line;
	char *argv[MAX_ARGS];
} SubCommand;


typedef struct Command
{
	SubCommand sub_commands[MAX_SUB_COMMANDS];
	int num_sub_commands;
	char *stdin_redirect;
	char *stdout_redirect;
	int background;
} Command;

void get_args(char *in, char **argv, int size);
void print_args(char **argv);
void ReadCommand(char *line, Command *command);
void PrintCommand(Command *command);
void ReadRedirectsAndBackground(struct Command *command);

int main()
{
	 char s[200];

	 printf("Enter a string: \n");
	 fgets(s, sizeof s, stdin);

	 Command command;
	 command.num_sub_commands = 0;

	 ReadCommand(s, &command);
	 ReadRedirectsAndBackground(&command);
	 PrintCommand(&command);

	 return 0;
}

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
	command->background = 0;
	command->stdin_redirect = NULL;
	command->stdout_redirect = NULL;

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
		printf("Command #%d\n", i);
		print_args(command->sub_commands[i].argv);
	}
	printf("\n");
	if(command->stdin_redirect != NULL)
	{  printf("Redirect stdin: %s\n", command->stdin_redirect);  }	
	else
	{  printf("Redirect stdin: None\n");  }	
	
	if(command->stdout_redirect != NULL)
	{  printf("Redirect stdout: %s\n", command->stdout_redirect);  }
	else
	{  printf("Redirect stdout: None\n");  }
	
	if(command->background)
	{  printf("Background: yes\n");  }
	else
	{  printf("Background: no\n");  }
}

void ReadRedirectsAndBackground(struct Command *command)
{
	SubCommand* last_sub_command = &(command->sub_commands[command->num_sub_commands-1]);
	unsigned int last_arg_index;
	char** argv = last_sub_command->argv;

	int i, j;
	for(i=0; argv[i] != NULL; i++)
	{
		last_arg_index = i;
	}

	for(i=last_arg_index; i>=0; i--)
	{
		if(argv[i] != NULL)
		{
			if(strcmp(argv[i],"&") == 0)
			{
				command->background = 1;
				for(j=i; j<last_arg_index; j++)
				{
					argv[j] = argv[j+1];
				}
				argv[last_arg_index] = NULL;
				last_arg_index -= 1;
			}
			else if(strcmp(argv[i],">") == 0 && 
				argv[i+1] != NULL)
			{
				command->stdout_redirect = argv[i+1];
				for(j=i; j<last_arg_index; j++)
				{
					argv[j] = argv[j+2];
				}
				argv[last_arg_index] = NULL;
				argv[last_arg_index-1] = NULL;
				last_arg_index -= 2;
			}
			else if(strcmp(argv[i],"<") == 0)
			{
				command->stdin_redirect = argv[i+1];
				for(j=i; j<last_arg_index; j++)
				{
					argv[j] = argv[j+2];
				}
				argv[last_arg_index] = NULL;
				argv[last_arg_index-1] = NULL;
				last_arg_index -= 2;
			}
		}
	}
}

