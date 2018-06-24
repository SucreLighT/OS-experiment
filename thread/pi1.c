#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define LEN 10000		// ����ʹ��10000��������PI 

double array[LEN + 1];
double fir_output = 0;
double sec_output = 0;
double output = 0;

// ��ʼ������ 
void array_init()
{
	int i;
	for(i = 1; i <= LEN; i++)
	{
		if(i % 2 == 1)
			array[i] = 1.0 / (2 * (i - 1) + 1);
		else
			array[i] = 1.0 / (-2 * (i - 1) - 1);
	}
}

// �ӽ��̼��������һ��ĺ� 
void *sub_thread()
{
	int i;
	for(i = LEN / 2 + 1; i <= LEN; i++)
		sec_output += array[i];
}

int main()
{
	pthread_t tid;
	int i;

	array_init();
	pthread_create(&tid, NULL, sub_thread, NULL);

	// �����̼�������ǰһ��ĺ� 
	for( i = 1; i <= LEN / 2; i++)
		fir_output +=array[i];
	pthread_join(tid, NULL);	// �ȴ��ӽ��� 
	output = fir_output +sec_output;
	printf("PI = %lf\n", output * 4);
	return 0;
}

