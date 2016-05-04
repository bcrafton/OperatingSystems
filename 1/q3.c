#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_BUFFER_SIZE 50

int get_num()
{
	char buffer[MAX_BUFFER_SIZE];
	int ret;

	printf("Enter a number: ");
	while(1)
	{
		fgets(buffer, MAX_BUFFER_SIZE, stdin);
		if(buffer != NULL)
		{
			unsigned int endline_index = (unsigned int) strlen(buffer) - 1;
			buffer[endline_index] = '\0';

			sscanf(buffer, "%d", &ret);
			return ret;
		}
	}
}

int main(int argc, char **argv)
{
	int ret = fork();
	if (ret < 0)
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	} 
	else if (ret == 0)
	{
		return get_num();
	}
	else
	{
		int child_ret;
		int w = wait(&child_ret);
		if(WIFEXITED(child_ret))
		{
			printf("Child exited with status %d\n", WEXITSTATUS(child_ret));
		}
	}
	return 0;
}