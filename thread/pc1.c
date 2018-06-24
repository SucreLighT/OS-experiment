#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<ctype.h>

#define LEN 4
#define COUNT 8

char buffer[2][LEN];
int in[2] = {0, 0};
int out[2] = {0, 0};

int buffer_is_empty(int num)
{
	return in[num] == out[num];
}

int buffer_is_full(int num)
{
	return (in[num] + 1) % LEN == out[num];
}

void put_item(int num, char item)
{
	buffer[num][in[num]] = item;
	in[num] = (in[num] + 1) % LEN;
}

char get_item(int num)
{
	char item;
	item = buffer[num][out[num]];
	out[num] = (out[num] + 1) % LEN;
	return item;
}

pthread_mutex_t mutex_buffer1,mutex_buffer2;
pthread_cond_t wait_empty_buffer1,wait_empty_buffer2;
pthread_cond_t wait_full_buffer1,wait_full_buffer2;

void *produce(void *arg)
{
	int i;
	char item;

	for(i = 0; i < COUNT; i++)
	{
		pthread_mutex_lock(&mutex_buffer1);
		while(buffer_is_full(0))	// 0��ʾ��һ�������� 
			pthread_cond_wait(&wait_empty_buffer1, &mutex_buffer1);	// �ȴ���һ��������Ϊ�� 

		item = 'a' + i;
		put_item(0,item);			// ���һ��������д��Ԫ�� 
		printf("produce item:%c\n", item);

		pthread_cond_signal(&wait_full_buffer1);	
		pthread_mutex_unlock(&mutex_buffer1);
	}
	return NULL;
}

// �ӵ�һ��������ȡ��Ԫ�أ�д��ڶ��������� 
void *calculate(void *arg)
{
	int i;
	char item;
	for(i = 0; i < COUNT; i++)
	{
		pthread_mutex_lock(&mutex_buffer1);
		while(buffer_is_empty(0))	// ��һ��������Ϊ�գ���ȴ���Ϊ�� 
			pthread_cond_wait(&wait_full_buffer1, &mutex_buffer1);
		item = get_item(0);			// �ӵ�һ��������ȡ��һ��Ԫ�� 
		pthread_cond_signal(&wait_empty_buffer1);
		pthread_mutex_unlock(&mutex_buffer1);

		pthread_mutex_lock(&mutex_buffer2);
		while(buffer_is_full(1))	// �ڶ���������Ϊ������ȴ���Ϊ�� 
			pthread_cond_wait(&wait_empty_buffer2, &mutex_buffer2);
		put_item(1, toupper(item));	// ��ڶ���������д��һ��Ԫ�� 
		printf("  calculate item:%c\n", toupper(item));
		pthread_cond_signal(&wait_full_buffer2);
		pthread_mutex_unlock(&mutex_buffer2);
	}
	return NULL;
}

void *consume(void *arg)
{
	int i;
	char item;

	for(i = 0; i < COUNT; i++)
	{
		pthread_mutex_lock(&mutex_buffer2);
		while(buffer_is_empty(1))
			pthread_cond_wait(&wait_full_buffer2, &mutex_buffer2);

		item = get_item(1);
		printf("    consume item:%c\n", item);

		pthread_cond_signal(&wait_empty_buffer2);
		pthread_mutex_unlock(&mutex_buffer2);
	}
	return NULL;
}


int main()
{
	pthread_t tid1, tid2, tid3;
		
	pthread_mutex_init(&mutex_buffer1, NULL);
	pthread_mutex_init(&mutex_buffer2, NULL);
	pthread_cond_init(&wait_empty_buffer1, NULL);
	pthread_cond_init(&wait_empty_buffer2, NULL);
	pthread_cond_init(&wait_full_buffer1, NULL);
	pthread_cond_init(&wait_full_buffer2, NULL);
	
	pthread_create(&tid1, NULL, produce, NULL);
	pthread_create(&tid2, NULL, calculate, NULL);
	pthread_create(&tid3, NULL, consume, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	
	return 0;
}
