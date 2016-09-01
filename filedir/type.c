#include <sys/stat.h>
#include <stdio.h>

int print_filetypes(int argc, char const *argv[])
{
	int         i;
	struct stat buf;
	char        *ptr;

	for (i = 1; i < argc; i++)
	{
		printf("%s: ", argv[i]);

		if (lstat(argv[i], &buf) < 0)
		{
		perror("lstat error\n");
    	continue;
		}

		if (S_ISREG(buf.st_mode))
			ptr = "regular";
		else if (S_ISDIR(buf.st_mode))
			ptr = "directory";
    	else if (S_ISCHR(buf.st_mode))
    		ptr = "character special";
    	else if (S_ISBLK(buf.st_mode))
    		ptr = "block special";
    	else if (S_ISFIFO(buf.st_mode))
    		ptr = "FIFO";
    	else if (S_ISLINK(buf.st_mode))
    		ptr = "symbolic link";
    	else if (S_ISSOCK(buf.st_mode))
    		ptr = "socket";
    	else
    		ptr = "unknown mode";

    	printf("%s\n", ptr);
	}
 	exit(0);
}
