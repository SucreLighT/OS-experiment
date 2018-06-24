#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define THREAD_NUM 20	// 假设进行20次传递 
int output = 1;			 
pthread_t tid[THREAD_NUM];

void *ring(void *arg)
{
	int *count;
	count = (int *) arg;	
	if(output > THREAD_NUM + 1 )	// 一共显示21次结果 
		exit(0);
	if((*count == THREAD_NUM))		// 最后一个进程 
	{
		printf("T%d   output=%d\n", *count, output);
		output++;
		*count = 1;					// 递归调用，回到进程1，形成环 
		pthread_create(&tid[0], NULL, ring, count);
	}
	else
	{
		printf("T%d   output=%d\n", *count, output);
		output++;
		*count = (*count) + 1;		// 递归调用，传向下一个进程 
		pthread_create(&tid[(*count) + 1], NULL, ring, count);
	}
	return NULL;
}

int main()
{
	int count = 1;

	printf("thread number:%d\n", THREAD_NUM);

	pthread_create(&tid[0], NULL, ring, &count);
	while(1);
}
