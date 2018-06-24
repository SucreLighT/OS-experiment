/*************************************************************************
    > File Name: ex2.c
    > Author: sucre
    > Created Time: 2018年06月21日 星期四 10时01分09秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *compute(void *arg)
{
	char* string = arg;
	int i;
	for(i = 0; i < 3; i++)
	{
		puts(string);
		sleep(1);
	}
}

int main()
{
	pthread_t worker_tid;
	pthread_create(&worker_tid, NULL, &compute, "worker");
	compute("master");
	return 0;
}
