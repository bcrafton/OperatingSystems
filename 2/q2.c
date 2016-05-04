#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, const char * argv[]) {
    int fds[2];
    pipe(fds);

    // Create one child process for more
    int pid = fork();
    if (pid == 0) {
		// Close write end of pipe
		close(fds[1]);
		// Duplicate read end of pipe in standard input
		close(0);
		dup(fds[0]);
		// Child launches command "wc"
		char *argv[2];
		argv[0] = "wc";
		argv[1] = NULL;
		execvp(argv[0], argv);
    } 
    // Create another child process for ls
    int pid2 = fork();
    if (pid2 == 0) {
		// Close read end of pipe
		close(fds[0]);
		// Duplicate write end of pipe in standard output
		close(1);
		dup(fds[1]);
		// Parent launches command "ls -l"
		char *argv[3];
		argv[0] = "ls";
		argv[1] = "-l";
		argv[2] = NULL;
		execvp(argv[0], argv);
    }

    close(fds[0]);
    close(fds[1]);

    // wait for the more process to finish
    int status;
    waitpid(pid, &status, 0);
    waitpid(pid2, &status, 0);
	
    printf("Process %d finished\n", pid);
    printf("Process %d finished\n", pid2);
    return 0;
}
