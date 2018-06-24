#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd_src, fd_des;
	if(argc != 3)
	{
		printf("Command Format Error!\n");
		exit(-1);
	}
	
	fd_src = open (argv[1], O_RDONLY);
	if(fd_src == -1)
	{
		printf("Open SrcFile Error!\n");
		exit(-1);
	}
	
	fd_des = open(argv[2], O_CREAT|O_WRONLY|O_EXCL);
	if(fd_des == -1)
	{
		printf("Open DesFile Error!\n");
		exit(-1);
	}
		
	char buf [1];
	int count = read(fd_src, buf, sizeof(buf));
	
	while(count != -1 && count != 0)
	{
		write(fd_des, buf, sizeof(buf));
		count = read(fd_src, buf, sizeof(buf));
	}

	close(fd_src);
	close(fd_des);
	return 0;
}
