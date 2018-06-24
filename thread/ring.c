#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define THREAD_NUM 20	// �������20�δ��� 
int output = 1;			 
pthread_t tid[THREAD_NUM];

void *ring(void *arg)
{
	int *count;
	count = (int *) arg;	
	if(output > THREAD_NUM + 1 )	// һ����ʾ21�ν�� 
		exit(0);
	if((*count == THREAD_NUM))		// ���һ������ 
	{
		printf("T%d   output=%d\n", *count, output);
		output++;
		*count = 1;					// �ݹ���ã��ص�����1���γɻ� 
		pthread_create(&tid[0], NULL, ring, count);
	}
	else
	{
		printf("T%d   output=%d\n", *count, output);
		output++;
		*count = (*count) + 1;		// �ݹ���ã�������һ������ 
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
