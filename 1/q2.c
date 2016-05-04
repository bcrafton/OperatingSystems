
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 50

void echo()
{
	char buffer[MAX_BUFFER_SIZE];
	while(1)
	{
		fgets(buffer, MAX_BUFFER_SIZE, stdin);
		if(buffer != NULL)
		{
			unsigned int endline_index = (unsigned int) strlen(buffer) - 1;
			buffer[endline_index] = '\0';
			printf("%s\n", buffer);
		}
	}
}

int main(int argc, char **argv)
{
	echo();
	return 0;
}
