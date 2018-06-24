#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#define CAPACITY 4
int buffer[CAPACITY];
int in = 0;
int out = 0;

// 写指针和读指针相同，当前缓冲区为空 
int buffer_is_empty()
{
	return in == out;
}

// 写指针与读指针相邻且指向最后一个元素，当前缓冲区为满 
int buffer_is_full()
{
	return (in + 1) % CAPACITY == out;
}

// 获取读指针指向的元素，同时读指针后移 
int get_item()
{
	int item;

	item = buffer[out];
	out = (out + 1) % CAPACITY;
	return item;
}

// 将元素写入到写指针当前的位置，同时写指针后移 
void put_item(int item)
{
	buffer[in] = item;
	in = (in + 1) % CAPACITY;
}

pthread_mutex_t mutex;
pthread_cond_t wait_empty_buffer;	// 等待一个空缓冲区 
pthread_cond_t wait_full_buffer;	// 等待一个满缓冲区 

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
	int i;
	int item;

	for(i = 0; i < ITEM_COUNT; i++)
	{
		pthread_mutex_lock(&mutex);
		while(buffer_is_empty())	// 缓冲区为空 
			pthread_cond_wait(&wait_full_buffer, &mutex);	// 等待一个满缓冲区 

		item = get_item();	// 取出一个数据 
		printf("   consume item:%c\n", item);

		pthread_cond_signal(&wait_empty_buffer);	// 唤醒等待空缓冲区的生产者 
		pthread_mutex_unlock(&mutex);	// 释放空缓冲区 
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
		while(buffer_is_full())	// 缓冲区为满 
			pthread_cond_wait(&wait_empty_buffer, &mutex);	// 等待一个空缓冲区 

		item = 'a' + i;
		put_item(item);	// 写入一个数据 
		printf("produce item:%c\n", item);

		pthread_cond_signal(&wait_full_buffer);	// 唤醒等待满缓冲区的消费者 
		pthread_mutex_unlock(&mutex);	// 释放一个满缓冲区 
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
