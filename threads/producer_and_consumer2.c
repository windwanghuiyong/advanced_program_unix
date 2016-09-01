#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t			Mutex;
pthread_mutexattr_t		MutexAttr;
pthread_cond_t			Cond = PTHREAD_COND_INITIALIZER;
int Resource = 10;

void *producer(void *arg)
{
	int id = *(int *)arg;
    while (1)
    {
        pthread_mutex_lock(&Mutex);
		while (Resource > 0)	// 若 wait 函数中满足条件和加锁之间的时间间隙内，其他进程抢先修改了 Resource
		{
			printf("p%d: wait\n", id);
    		pthread_cond_wait(&Cond, &Mutex);
		}
        if (Resource == 0)
        {
            Resource += 10;
            printf("p%d: %d\n", id, Resource);
        }
        pthread_mutex_unlock(&Mutex);
        sleep(5);
    }
    return NULL;
}

void *consumer(void *arg)
{
	int id = *(int *)arg;
	while (1)
    {
        pthread_mutex_lock(&Mutex);
        if (Resource > 0)
        {
            Resource--;
            printf("c%d: %d\n", id, Resource);
        }
		if (Resource == 0)
		{
    		printf("c%d: signal\n", id);
    		pthread_cond_signal(&Cond);
		}
		pthread_mutex_unlock(&Mutex);
        sleep(2);
    }
	return NULL;
}

int main(void)
{
	int p[3] = {1, 2, 3};
	void *retval;
    pthread_t tid_p;
	pthread_t tid_c1, tid_c2, tid_c3;

    if (pthread_mutexattr_init(&MutexAttr) == -1)
        perror("pthread_mutexattr_init error");

    if (pthread_mutexattr_settype(&MutexAttr, PTHREAD_MUTEX_NORMAL) == -1)
        perror("pthread_mutexattr_settype error");

    if (pthread_mutex_init(&Mutex, &MutexAttr) == -1)
        perror("pthread_mutex_init error");
	
    if (pthread_create(&tid_p, NULL, producer, &p[0])== -1)
        perror("pthread_create error");
	
    if (pthread_create(&tid_c1, NULL, consumer, &p[0])== -1)
        perror("pthread_create error");
	
    if (pthread_create(&tid_c2, NULL, consumer, &p[1])== -1)
        perror("pthread_create error");
	
    if (pthread_create(&tid_c3, NULL, consumer, &p[2])== -1)
        perror("pthread_create error");
	
	pthread_join(tid_p,  &retval);
	printf("retval = %d\n", *(int *)retval);
    pthread_join(tid_c1, &retval);
	printf("retval = %d\n", *(int *)retval);
	pthread_join(tid_c2, &retval);
	printf("retval = %d\n", *(int *)retval);
    pthread_join(tid_c3, &retval);
	printf("retval = %d\n", *(int *)retval);
	
    return 0;
}
