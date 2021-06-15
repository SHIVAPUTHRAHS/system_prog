#include<stdio.h>
#include<pthread.h>

int temp=0;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;

void *thread1()
{
	printf("in thread 1\n");
	pthread_mutex_lock(&mutex1);
	if(temp<5)
		pthread_cond_wait(&cond1,&mutex1);
		printf("Thread 1:Turning on the cpu fan\n");
		sleep(1);
		printf("unlocking mutex\n");
		pthread_mutex_unlock(&mutex1);
		printf("end of thread 1\n");
		pthread_exit(NULL);
}

void *thread2()
{
	sleep(1);
	printf("in thread 2\n");
	pthread_mutex_lock(&mutex1);
	int i;
	for(i=0;i<10;i++)
	{
		temp=i;
		printf("thread 2:temp=%d\n",i);
		if(temp==5)
		{
			printf("thread 2 sending signal\n");
			pthread_cond_signal(&cond1);
			printf("thread 2 unlocking mutex\n");
			pthread_mutex_unlock(&mutex1);
			sleep(1);
			printf("thread 2 taking lock\n");
			pthread_mutex_lock(&mutex1);
		}
		sleep(1);
	}
	printf("thread 2 unlocking mutex\n");
	pthread_mutex_unlock(&mutex1);
	printf("end of thread 2\n");
	pthread_exit(NULL);
}

int main()
{
	printf("in default thread\n");
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,thread1,0);
	pthread_create(&tid2,NULL,thread2,0);
	pthread_join(tid1,0);
	pthread_join(tid2,0);
	printf("destroying mutex and conditional variables\n");
	pthread_mutex_destroy(&mutex1);
	pthread_cond_destroy(&cond1);
	printf("end of default thread\n");
	pthread_exit(NULL);
}

