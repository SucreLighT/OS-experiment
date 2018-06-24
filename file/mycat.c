#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int fd;
	if(argc != 2)
	{
		printf("Command Format Error!\n");
		exit(-1);
	}
	
	fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		printf("Open File Error!\n");
		exit(-1);
	}
	
	char buf[1];
	int count = read(fd, buf, sizeof(buf));

	while(count != -1 && count != 0)
	{
		printf("%s", buf);
		count = read(fd, buf, sizeof(buf));
	}

	close(fd);
	return 0;
}
