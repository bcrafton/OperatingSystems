#include "shell.h"

static void get_args(char *in, char **argv, int size);
static void close_all_fd(int fd[][2], int num);

void prompt()
{
	printf("$ ");
	fflush(stdout);
}

void reset_command(command_t *command)
{
	// this code has memory leaks
	command->stdin_redirect = NULL;
	command->stdout_redirect = NULL;
	command->background = 0;
	command->num_sub_commands = 0;
}

void read_command(char *line, command_t *command)
{
	command->background = 0;
	command->stdin_redirect = NULL;
	command->stdout_redirect = NULL;

	char* next = strtok(line, "|");
	int i;
	for(i=0; i<MAX_SUB_COMMANDS && next != NULL; i++)
	{
		command->sub_commands[i].line = strdup(next);
		command->num_sub_commands++;
		next = strtok(NULL, "|");
	}

	for(i=0; i<command->num_sub_commands; i++)
	{
		get_args(command->sub_commands[i].line, command->sub_commands[i].argv, MAX_ARGS);
	}
}


/* This function will execute the command after it has been read and parsed for stuff
   It will wait for all of the sub commands to finish before returning.
*/
void execute_command(command_t* command)
{
	int pid[MAX_SUB_COMMANDS];
	int fd[MAX_SUB_COMMANDS-1][2];
	int child, i;
	for(child=0; child<command->num_sub_commands-1; child++)
	{
		pipe(fd[child]);
	}
	for(child=0; child<command->num_sub_commands; child++)
	{
		pid[child] = fork();
		if(pid[child] == 0)
		{
			for(i=0; i<command->num_sub_commands-1; i++)
			{
				if(child == i)
				{
					close(1);
					dup(fd[i][1]);

				}
				close(fd[i][1]);

				if(child == i+1)
				{
					close(0);
					dup(fd[i][0]);
				}
				close(fd[i][0]);
			}

			if(child == 0)
			{
				if(command->stdin_redirect != NULL)
				{
					close(0);

					int fd = open(command->stdin_redirect, O_RDONLY);
					if (fd < 0)
					{
						fprintf(stderr, "%s: File not found\n", command->stdin_redirect);
						exit(1);
					}
				}
			}
			if(child == command->num_sub_commands-1)
			{
				if(command->stdout_redirect)
				{
					close(1);

					int fd = open(command->stdout_redirect, O_WRONLY | O_CREAT, 0660);
					if (fd < 0)
					{
						fprintf(stderr, "%s: Cannot create file\n", command->stdout_redirect);
						exit(1);
					}
				}
			}

			execvp(command->sub_commands[child].argv[0], command->sub_commands[child].argv);
			printf("%s: Command not found\n", command->sub_commands[child].argv[0]);
			exit(1);
		}
	}

	close_all_fd(fd, command->num_sub_commands-1);

	if(command->background)
	{
		printf("[%d]\n", pid[command->num_sub_commands-1]);
		prompt();
	}
	
	for(i = 0; i < command->num_sub_commands; i++)
	{
		int status;
		while (-1 == waitpid(pid[i], &status, 0));
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
			printf("Process %i (pid: %d) failed\n", i, pid[i]);
		}
		else if(i == command->num_sub_commands-1 && command->background)
		{
			//printf("[%d]\n finished", pid[command->num_sub_commands-1]);
		}
	}
}

int command_handler(command_t* command, List* l)
{
	int pid = fork();
	if(pid == 0)
	{
		execute_command(command);
	}
	else
	{
		if(command->background)
		{
			list_append(pid, l);
		}
		else
		{
			int status;
			while(waitpid(pid, &status, WNOHANG)==0);
			prompt();
		}
	}
	return pid;
}


static void get_args(char *in, char **argv, int size)
{
	int i;
	for(i=0; i<strlen(in); i++)
	{
		if(in[i] == '\n')
		{
			in[i] = '\0';
		}
	}

	char* next = strtok(in, " ");
	for(i=0; i<size-1 && next != NULL; i++)
	{
		argv[i] = strdup(next);
		next = strtok(NULL, " ");
	}
	argv[i] = NULL;
}

static void close_all_fd(int fd[][2], int num)
{
	int i;
	for(i=0; i<num; i++)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}
}
