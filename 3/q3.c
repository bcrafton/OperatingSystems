
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main()
{
	int ptc[2], ctp[2];
	pipe(ptc); pipe(ctp);

	int pid = fork();
	char i, buf;
	if (pid == 0)
	{
		close(ptc[WRITE]); close(ctp[READ]);
		write(ctp[WRITE], &i, 1);
		for (i = 0; i < 5; i++)
		{
			while(read(ptc[READ], &buf, 1) == 0);
			printf("%d. Child\n", i + 1);
			write(ctp[WRITE], &i, 1);
			fflush(stdout);
		}
	}
	else
	{
		close(ptc[READ]); close(ctp[WRITE]);
		for (i = 0; i < 5; i++)
		{
			while(read(ctp[READ], &buf, 1) == 0);
			printf("%d. Parent\n", i + 1);
			write(ptc[WRITE], &i, 1);
			fflush(stdout);
		}
		wait(NULL);
	}
}