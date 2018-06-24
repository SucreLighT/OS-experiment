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

/*ִ�н����
 *./a.out
 *pid = 5327
 *In parent process
 *pid = 0
 *In child process
 *�����̴�fork���ش�����ִ�У��ڸ������У�fork�����ӽ���PID
 *�ӽ��̴�fork���ش���ʼִ�У����ӽ����У�fork����0 
 */ 
