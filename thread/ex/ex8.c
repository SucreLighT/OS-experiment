#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#define CAPACITY 4
int buffer[CAPACITY];
int in;
int out;

int buffer_is_empty()
{
	return in == out;
}

int buffer_is_full()
{
	return (in + 1) % CAPACITY == out;
}

int get_item()
{
	int item;

	item = buffer[out];
	out = (out + 1) % CAPACITY;
	return item;
}

void put_item(int item)
{
	buffer[in] = item;
	in = (in + 1) % CAPACITY;
}

// 使用条件变量实现信号量sema_t
typedef struct{
	int value;	// value记录了信号量的值
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

void sema_init(sema_t *sema, int value)
{
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

// 等待 
void sema_wait(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	while(sema->value <= 0)	// 如果信号量的值小于等于0，则等待条件变量
		pthread_cond_wait(&sema->cond, &sema->mutex);
	sema->value--;	// 将信号量的值减一
	pthread_mutex_unlock(&sema->mutex);
}
// 唤醒 
void sema_signal(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	++sema->value;	// 将信号量的值加一
	pthread_cond_signal(&sema->cond);	// 唤醒等待条件变量的线程
	pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex_sema;
sema_t empty_buffer_sema;	// empty_buffer_sema的数值表示空buffer的数量
sema_t full_buffer_sema;	// full_buffer_sema的数值表示满buffer的数量

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
	int i;
	int item;

	for(i = 0; i < ITEM_COUNT; i++)
	{
		sema_wait(&full_buffer_sema);	// 消费者需要一个满的buffer,申请请信号量full_buffer_sema 
		sema_wait(&mutex_sema);	// mutex_sema用于对共享变量进行互斥访问

		item = get_item();

		sema_signal(&mutex_sema);
		sema_signal(&empty_buffer_sema);	// 产生一个新的空buffer,释放信号量empty_buffer_sema
		printf("  consume item:%c\n",item);
	}
	return NULL;
}

void *produce()
{
	int i;
	int item;

	for(i = 0; i < ITEM_COUNT; i++)
	{
		sema_wait(&empty_buffer_sema);
		sema_wait(&mutex_sema);

		item = 'a' + i;
		put_item(item);

		sema_signal(&mutex_sema);
		sema_signal(&full_buffer_sema);
		printf("produce item:%c\n", item);
	}
	return NULL;
}

int main()
{
	pthread_t consumer_tid;
		
	sema_init(&mutex_sema, 1);
	sema_init(&empty_buffer_sema, CAPACITY - 1);
	sema_init(&full_buffer_sema, 0);

	pthread_create(&consumer_tid, NULL, consume, NULL);
	produce();	// 主线程作为生产者 
	pthread_join(consumer_tid, NULL);
	return 0;
}
