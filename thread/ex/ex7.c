#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#define CAPACITY 4
int buffer[CAPACITY];
int in = 0;
int out = 0;

// дָ��Ͷ�ָ����ͬ����ǰ������Ϊ�� 
int buffer_is_empty()
{
	return in == out;
}

// дָ�����ָ��������ָ�����һ��Ԫ�أ���ǰ������Ϊ�� 
int buffer_is_full()
{
	return (in + 1) % CAPACITY == out;
}

// ��ȡ��ָ��ָ���Ԫ�أ�ͬʱ��ָ����� 
int get_item()
{
	int item;

	item = buffer[out];
	out = (out + 1) % CAPACITY;
	return item;
}

// ��Ԫ��д�뵽дָ�뵱ǰ��λ�ã�ͬʱдָ����� 
void put_item(int item)
{
	buffer[in] = item;
	in = (in + 1) % CAPACITY;
}

pthread_mutex_t mutex;
pthread_cond_t wait_empty_buffer;	// �ȴ�һ���ջ����� 
pthread_cond_t wait_full_buffer;	// �ȴ�һ���������� 

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
	int i;
	int item;

	for(i = 0; i < ITEM_COUNT; i++)
	{
		pthread_mutex_lock(&mutex);
		while(buffer_is_empty())	// ������Ϊ�� 
			pthread_cond_wait(&wait_full_buffer, &mutex);	// �ȴ�һ���������� 

		item = get_item();	// ȡ��һ������ 
		printf("   consume item:%c\n", item);

		pthread_cond_signal(&wait_empty_buffer);	// ���ѵȴ��ջ������������� 
		pthread_mutex_unlock(&mutex);	// �ͷſջ����� 
	}
	return NULL;
}

void *produce(void *arg)
{
	int i;
	int item;

	for(i = 0; i < ITEM_COUNT; i++)
	{
		pthread_mutex_lock(&mutex);
		while(buffer_is_full())	// ������Ϊ�� 
			pthread_cond_wait(&wait_empty_buffer, &mutex);	// �ȴ�һ���ջ����� 

		item = 'a' + i;
		put_item(item);	// д��һ������ 
		printf("produce item:%c\n", item);

		pthread_cond_signal(&wait_full_buffer);	// ���ѵȴ����������������� 
		pthread_mutex_unlock(&mutex);	// �ͷ�һ���������� 
	}
	return NULL;
}

int main()
{
	pthread_t consumer_tid;
		
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&wait_empty_buffer, NULL);
	pthread_cond_init(&wait_full_buffer, NULL);

	pthread_create(&consumer_tid, NULL, consume, NULL);
	produce(NULL);
	pthread_join(consumer_tid, NULL);
	return 0;
}
