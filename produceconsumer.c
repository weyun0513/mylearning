/*
Producer thread will keep incrementing fover numUnits by 1
Consumer thread will keep incrementing fover numUnits by 1
MAX_BUFFER_SIZE is the max
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_BUFFER_SIZE 5
#define TRUE 1
#define FALSE 0

volatile int numUnits = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t spaceAvaliable = PTHREAD_COND_INITIALIZER;
pthread_cond_t unitAvaliable = PTHREAD_COND_INITIALIZER;

void *producer(void *input)
{
    int status;
    while (TRUE)
    {
        status = pthread_mutex_lock(&mutex);
        /* LOCK*/
        if (status != 0)
        {
            exit(EXIT_FAILURE);
        }
        /*PRODUCE AN ITEM IF SPACE IS AVALIABLE*/
        while (!(numUnits < MAX_BUFFER_SIZE))
        {
            printf("Producer must block.\n");
            status = pthread_cond_wait(&spaceAvaliable, &mutex);
            if (status != 0)
            {
                printf("Producer fail to wait on spaceAvaliable");
                exit(EXIT_FAILURE);
            }
        }

        numUnits++;
        printf("Producer INCRE to %d\n", numUnits);
        if (numUnits == 1)
        {
            status = pthread_cond_signal(&unitAvaliable);
        }

        status = pthread_mutex_unlock(&mutex);
        /* LOCK*/
        if (status != 0)
        {
            printf("Producer fail unlock");
            exit(EXIT_FAILURE);
        }
        sleep(2);
    }
    return NULL;
}

void *consumer(void *input)
{
    int status;
    while (TRUE)
    {
        status = pthread_mutex_lock(&mutex);
        /* LOCK*/
        if (status != 0)
        {
            printf("Consumer fail lock");
            exit(EXIT_FAILURE);
        }
        while (!(numUnits > 0))
        {
            printf("Consumer must block\n");
            status = pthread_cond_wait(&unitAvaliable, &mutex);
            if (status != 0)
            {
                printf("Consumer fail to wait on unitAvaliable");
                exit(EXIT_FAILURE);
            }
        }
        numUnits--;
        printf("Consumer decremented numUnits to %d\n", numUnits);
        /*PRODUCE AN ITEM IF SPACE IS AVALIABLE*/
        if (numUnits == MAX_BUFFER_SIZE - 1)
        { /*Comsumer must wake up producer*/
            pthread_cond_signal(&spaceAvaliable);
            if (status != 0)
            {
                printf("Consumer fail to signial spaceavaliable to producer");
                exit(EXIT_FAILURE);
            }
        }
        status = pthread_mutex_unlock(&mutex);
        /* LOCK*/
        if (status != 0)
        {
            printf("Consumer fail unlock");
            exit(EXIT_FAILURE);
        }
        sleep(1);

        return NULL;
    }
}

int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;
    int status, threadInput1, threadInput2;
    void *result;
    threadInput1 = 1;
    threadInput2 = 2;
    // status=pthread_create(
    //     &thread1,NULL,threadFunction,(void*)&threadInput1)
    // ;
    status = pthread_create(
        &thread1, NULL, producer, NULL);
    if (status != 0)
    {
        printf("pthread_create fail");
        exit(1);
    }
    status = pthread_join(thread1, &result);
    if (status != 0)
    {
        printf("pthread_join fail");
        exit(1);
    }

    status = pthread_create(
        &thread2, NULL, consumer, NULL);
    if (status != 0)
    {
        printf("pthread_create fail");
        exit(1);
    }
    status = pthread_join(thread2, &result);
    if (status != 0)
    {
        // printf("pthread_join fail");
        exit(1);
    }

    exit(EXIT_SUCCESS);
}
