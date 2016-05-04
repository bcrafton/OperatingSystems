#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef struct stat stat_t;

void print_permissions(unsigned long permissions);
void handle_info(const char* path);
void handle_link(const char* src, const char* dst);
void handle_symlink(const char* src, const char* dst);
void handle_rm(const char* path);


int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		perror("Did not get a command!\n");
	}
	else if(strcmp(argv[1], "info") == 0)
	{
		if(argc < 3)
		{
			perror("Command: info did not get enough arguments!\n");
		}
		else
		{
			handle_info(argv[2]);
		}
	}
	else if(strcmp(argv[1],"link") == 0)
	{	
		if(argc < 4)
		{
			perror("Command: link did not get enough arguments!\n");
		}
		else
		{
			handle_link(argv[2], argv[3]);
		}
	}
	else if(strcmp(argv[1],"symlink") == 0)
	{
		if(argc < 4)
		{
			perror("Command: symlink did not get enough arguments!\n");
		}
		else
		{
			handle_symlink(argv[2], argv[3]);
		}
	}
	else if(strcmp(argv[1],"rm") == 0)
	{
		if(argc < 3)
		{
			perror("Command: rm did not get enough arguments!\n");
		}
		else
		{
			handle_rm(argv[2]);
		}
	}
	return 0;
}

void handle_info(const char* path)
{
	stat_t info;
	int ret = stat(path, &info);
	printf("Inode: %ld\n", (long) info.st_ino);
	printf("Size: %lld bytes\n", (long long) info.st_size);
	print_permissions((unsigned long) info.st_mode);
}

void handle_link(const char* src, const char* dst)
{
	int ret = link(src, dst);
}

void handle_symlink(const char* src, const char* dst)
{
	int ret = symlink(src, dst);
}

void handle_rm(const char* path)
{
	int ret = remove(path);
}

void print_permissions(unsigned long permissions)
{
	const char lookup[] = {'x', 'w', 'r'};
	char permission_string[10];

	int i;
	for(i=0; i < 9; i++)
	{
		if( (permissions & (1 << i)) == (1 << i) )
		{
			permission_string[9 - i - 1] = lookup[i%3];
		}
		else
		{
			permission_string[9 - i - 1] = '-';
		}
	}
	permission_string[9] = 0;
	printf("Permissions: %s\n", permission_string);
}
