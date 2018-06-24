#include <stdio.h>
#include <unistd.h>

int main()
{ 
    pid_t pid;

    pid = fork();
    printf("pid = %d\n", pid);
    if (pid == 0)
        printf("In child process\n");
    else
        printf("In parent process\n");
    return 0;
}

/*执行结果：
 *./a.out
 *pid = 5327
 *In parent process
 *pid = 0
 *In child process
 *父进程从fork返回处继续执行，在父进程中，fork返回子进程PID
 *子进程从fork返回处开始执行，在子进程中，fork返回0 
 */ 
