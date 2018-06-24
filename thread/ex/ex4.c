/*************************************************************************
    > File Name: ex4.c
    > Author: sucre
    > Created Time: 2018年06月21日 星期四 10时43分32秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int array[] = {1, 1, 1, 2, 2, 2};
#define NR_TOTAL 6
#define NR_CPU 2
#define NR_CHILD (NR_TOTAL/NR_CPU)

struct param{
	int *array;
	int start;
	int end;
};

struct result{
	int sum;
};

void *compute(void *arg)
{
	struct param *param;
	struct result *result;
	int sum = 0;
	int i;

	param =(struct param*) arg;
	for(i = param->start; i < param->end; i++)
		sum += param->array[i];

	result = malloc(sizeof(struct result));
	result->sum = sum;
	return result;
}

int main()
{
	pthread_t workers[NR_CPU];
	struct param params[NR_CPU];
	int i;

	for(i = 0; i < NR_CPU; i++)
	{
		struct param *param;
		param = &params[i];
		param->array = array;
		param->start = i * NR_CHILD;
		param->end = (i + 1) * NR_CHILD;
		pthread_create(&workers[i], NULL, compute, param);
	}
	int sum;
	for(i = 0; i < NR_CPU; i++)
	{
		struct result *result;
		pthread_join(workers[i], (void**)&result);
		sum += result->sum;
		free(result);
	}
	printf("sum = %d\n", sum);
	return 0;
}

