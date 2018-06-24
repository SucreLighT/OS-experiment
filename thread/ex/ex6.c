/*************************************************************************
    > File Name: ex6.c
    > Author: sucre
    > Created Time: 2018年06月21日 星期四 11时03分40秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>

pthread_mutex_t mutex;
volatile int global  = 0;

void *compute(void *arg)
{
	int i;

	for(i = 0; i < 100 * 100 * 100; i++)
	{
		pthread_mutex_lock(&mutex);
		global++;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main()
{
	int i;
	pthread_t tids[3];

	pthread_mutex_init(&mutex, NULL);
	for(i = 0; i <3; i++)
		pthread_create(&tids[i], NULL, compute, NULL);
	for(i = 0; i < 3; i++)
		pthread_join(tids[i], NULL);
	pthread_mutex_destroy(&mutex);

	printf("global = %d\n",global);
	return 0;
}
