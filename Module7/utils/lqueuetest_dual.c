#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "locked_queue.h"

#define NUM_THREADS 2
#define NUM_ELEMENTS 1000

void* producer(void* arg) {
    locked_queue_t* lq = (locked_queue_t*)arg;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        int* element = malloc(sizeof(int));
        *element = i;
        lqput(lq, element);
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    locked_queue_t* lq = (locked_queue_t*)arg;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        int* element = (int*)lqget(lq);
        printf("Got element: %d\n", *element);
        free(element);
    }
    pthread_exit(NULL);
}

int main() {
    locked_queue_t* lq = lqopen();
    pthread_t threads[NUM_THREADS];

    pthread_create(&threads[0], NULL, producer, (void*)lq);
    pthread_create(&threads[1], NULL, consumer, (void*)lq);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    lqclose(lq);
    return 0;
}
