#include <stdio.h>
#include <unistd.h> 

int main()
{
    pid_t pid;

    pid = fork(); 
    if (pid == 0) 
        printf("In child:  child  PID = %d, parent PID = %d\n", getpid(), getppid()); 
    else
        printf("In parent: parent PID = %d, child  PID = %d\n", getpid(), pid); 
    return 0;
}

/*
 *$ ./a.out
 *In parent: parent PID = 9145, child  PID = 9146
 *In child:  child  PID = 9146, parent PID = 9145
 *fork()返回值为0表示在子进程中，非0则在父进程中 
*/ 
