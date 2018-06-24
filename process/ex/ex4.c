#include <stdio.h>
#include <unistd.h>

int global = 0;

void child()
{
    for (int i = 0; i < 3; i++) {
        global++;
        printf("In child, global = %d\n", global);
        sleep(1);
    }
}

void parent()
{
    for (int i = 0; i < 3; i++) {
        global++;
        printf("In parent, global = %d\n", global);
        sleep(1);
    }
} 

int main()
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
        child();
    else
        parent();
    return 0;
}

/*
./a.out
In parent, global = 1
In child, global = 1
In parent, global = 2
In child, global = 2
In parent, global = 3
In child, global = 3
$ _
���ӽ��̷��ʵ��Ǹ��Ե�ȫ�ֱ��������ֽ��̵ĸ�������*/
