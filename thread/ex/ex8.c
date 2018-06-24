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

// ʹ����������ʵ���ź���sema_t
typedef struct{
	int value;	// value��¼���ź�����ֵ
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

void sema_init(sema_t *sema, int value)
{
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

// �ȴ� 
void sema_wait(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	while(sema->value <= 0)	// ����ź�����ֵС�ڵ���0����ȴ���������
		pthread_cond_wait(&sema->cond, &sema->mutex);
	sema->value--;	// ���ź�����ֵ��һ
	pthread_mutex_unlock(&sema->mutex);
}
// ���� 
void sema_signal(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	++sema->value;	// ���ź�����ֵ��һ
	pthread_cond_signal(&sema->cond);	// ���ѵȴ������������߳�
	pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex_sema;
sema_t empty_buffer_sema;	// empty_buffer_sema����ֵ��ʾ��buffer������
sema_t full_buffer_sema;	// full_buffer_sema����ֵ��ʾ��buffer������

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
	int i;
	int item;

	for(i = 0; i < ITEM_COUNT; i++)
	{
		sema_wait(&full_buffer_sema);	// ��������Ҫһ������buffer,�������ź���full_buffer_sema 
		sema_wait(&mutex_sema);	// mutex_sema���ڶԹ���������л������

		item = get_item();

		sema_signal(&mutex_sema);
		sema_signal(&empty_buffer_sema);	// ����һ���µĿ�buffer,�ͷ��ź���empty_buffer_sema
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
	produce();	// ���߳���Ϊ������ 
	pthread_join(consumer_tid, NULL);
	return 0;
}
