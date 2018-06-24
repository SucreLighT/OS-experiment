#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void my_system(int argc, char *argv[])
{
	pid_t pid;
	
	pid = fork();
	if(pid == 0)
	{
		printf("\n");
		execvp(argv[1], argv + 1);
		/* √¸¡Ó–– ‰»Î"./a.out echo HELLO WORLD"
		 * argv[1]=echo,argv+1=HELLO WORLD
		*/ 
	}
	else if((pid = fork()) < 0)
		printf("Process Creation Falied!\n");	
	else 
	{
		wait(NULL);
	}
}
	
int main(int argc, char *argv[])
{
	my_system(argc, argv);
		
	return 0;
}	
	
