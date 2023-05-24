//线程1打印A、线程2打印B、线程3打印C，依次打印十遍（ABCABCABC…）
#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

sem_t sem[3];
void* task1(void* arg)
{
    char* name =(char*)arg;
    while(1)
    {
        sem_wait(&sem[0]);
        printf("A\n");
        sleep(1);
	printf("thread %ld ending working ... \n", pthread_self());
        sem_post(&sem[1]);
    }
    sem_destroy(&sem[0]);
    pthread_exit(NULL);
}

void* task2(void* arg)
{
    char* name =(char*)arg;
    while(1)
    {
        sem_wait(&sem[1]);
        printf("B\n");
        sleep(1);
	printf("thread %ld ending working ... \n", pthread_self());
        sem_post(&sem[2]);
    }
    sem_destroy(&sem[1]);
    pthread_exit(NULL);
}

void* task3(void* arg)
{
    char* name =(char*)arg;
    while(1)
    {
        sem_wait(&sem[2]);
        printf("C\n");
        sleep(1);
	printf("thread %ld ending working ... \n", pthread_self());
        sem_post(&sem[0]);
    }
    sem_destroy(&sem[2]);
    pthread_exit(NULL);
}

int main()
{
    sem_init(&sem[0],0 ,1);
    sem_init(&sem[1],0 ,0);
    sem_init(&sem[2],0 ,0);

    pthread_t ptid[3];
    if(pthread_create(&ptid[0],NULL,task1,"friend1")!=0)
    {
        perror("pthread_create error!");
        exit(1);
    };

    if(pthread_create(&ptid[1],NULL,task2,"friend2")!=0)
    {
        perror("pthread_create error!");
        exit(1);
    }

    if(pthread_create(&ptid[2],NULL,task3,"friend2")!=0)
    {
        perror("pthread_create error!");
        exit(1);
    }
    pthread_exit(NULL);
}


