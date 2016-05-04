#include "shell.h"

static void check_background_commands(List* l);

char directory[1024];

int main()
{
	int flags = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, flags | O_NONBLOCK);
	char buffer[1024];

	getcwd(directory, 1024);

	List* l = list_constructor();
	command_t command;
	reset_command(&command);

	prompt();
	while(1)
	{
		if(read(0, buffer, 1023) > 0)
		{
			// read the command into the struct
			read_command(buffer, &command);
			// parse the command for background execution, change directory, or file redirection
			parse_command(&command);
			/* call the command handler.
			   this will execute the command. handles both background and non-background
			*/
			int pid = command_handler(&command, l);
	 		if(pid == 0)
	 		{
	 			return 0;
	 		}
			// reset the command.
			reset_command(&command);
		}
		// check for background commands and remove them from the wait list.
		check_background_commands(l);
	}
	return 0;
}

void check_background_commands(List* l)
{
	int i;
	for(i = 0; i < l->size; i++)
	{
		int status;
		if(waitpid(list_get(i, l), &status, WNOHANG) == list_get(i, l))
		{
			list_remove(i, l);
		}
	}
}

void parse_command(command_t *command)
{
	sub_command_t* last_sub_command = &(command->sub_commands[command->num_sub_commands-1]);
	unsigned int last_arg_index = 0;
	char** argv = last_sub_command->argv;

	int i, j;
	for(i=0; argv[i] != NULL; i++)
	{
		last_arg_index = i;
	}

	if(argv == NULL)
		return;

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
	if(strcmp(command->sub_commands[0].argv[0], "cd") == 0)
	{
		unsigned char success = change_directory(command->sub_commands[0].argv[1]);
		if(!success)
		{
			printf("%s: no such directory\n", command->sub_commands[0].argv[1]);
		}
		else
		{
			chdir(directory);
		}
		for(i=0; i<command->num_sub_commands-1; i++)
		{
			command->sub_commands[i] = command->sub_commands[i+1];
		}
		command->num_sub_commands--;
	}
}

unsigned char change_directory(char* new_dir)
{
	DIR *dir;
	struct dirent *entry;

	if (!(dir = opendir(directory)))
		return 0;

	while(entry = readdir(dir))
	{
		if (entry->d_type == DT_DIR)
		{
			if(strcmp(entry->d_name, new_dir) == 0)
			{
				strcat(directory, "/");
				strcat(directory, new_dir);
				closedir(dir);
				return 1;
			}
		}
	}
	closedir(dir);
	return 0;
}
