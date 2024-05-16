#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>


#define MaxItems 5 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand(); // Produce a random item
        sem_wait(&empty);  // Wait for an empty slot in the buffer
        pthread_mutex_lock(&mutex);  // Acquire the mutex to enter the critical section
        
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno), buffer[in], in);
        in = (in + 1) % BufferSize;  // Move to the next position in the buffer

        pthread_mutex_unlock(&mutex);  // Release the mutex to exit the critical section
        sem_post(&full);  // Signal that a slot in the buffer is full
    }
}

void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full);  // Wait for a filled slot in the buffer
        pthread_mutex_lock(&mutex);  // Acquire the mutex to enter the critical section
        
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n", *((int *)cno), item, out);
        out = (out + 1) % BufferSize;  // Move to the next position in the buffer

        pthread_mutex_unlock(&mutex);  // Release the mutex to exit the critical section
        sem_post(&empty);  // Signal that a slot in the buffer is empty
    }
}


int main()
{   
    pthread_t pro[5], con[5];
    pthread_mutex_init(&mutex, NULL);  // Initialize the mutex
    sem_init(&empty, 0, BufferSize);  // Initialize the empty semaphore with the buffer size
    sem_init(&full, 0, 0);  // Initialize the full semaphore with 0

    int a[5] = {1, 2, 3, 4, 5};  // Just used for numbering the producer and consumer

    // Create producer threads
    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }

    // Create consumer threads
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    // Wait for producer threads to finish
    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL);
    }

    // Wait for consumer threads to finish
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);  // Destroy the mutex
    sem_destroy(&empty);  // Destroy the empty semaphore
    sem_destroy(&full);  // Destroy the full semaphore

    return 0;
}