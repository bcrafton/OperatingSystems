
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_args(char *in, char **argv, int max_args)
{
	unsigned endline_index = strlen(in) - 1;
	if(in[endline_index] = '\n')
	{
		in[endline_index] = '\0';
	}

	int word_count = 0;
	char* next = strtok(in, " \n");
	while(next != NULL)
	{
		argv[word_count] = strdup(next);
		word_count++;
		next = strtok(NULL, " ");
	}
	return word_count;
}

void print_args(int argc, char **argv)
{
	int i;
	for(i=0; i<argc; i++)
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
	 argc = get_args(s, argv, 10);
	 print_args(argc, argv);
}