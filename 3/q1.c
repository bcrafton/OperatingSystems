
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main()
{
	 char s[200];
	 char *argv[10];
	 int argc;

	 // Read a string from the user
	 printf("Enter a string: ");
	 fgets(s, sizeof s, stdin);
	 // Extract arguments and print them
	 get_args(s, argv, 10);
	 print_args(argv);
}