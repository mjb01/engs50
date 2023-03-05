#include <pthread.h>
#include "queue.h"

typedef struct {
    queue_t *queue;         // underlying queue
    pthread_mutex_t lock;   // lock for thread safety
} locked_queue_t;

/* Create a new locked queue */
locked_queue_t* lqopen(void) {
    locked_queue_t *lqp = (locked_queue_t*)malloc(sizeof(locked_queue_t));
    if (lqp != NULL) {
        lqp->queue = qopen();   // create a new queue
        pthread_mutex_init(&(lqp->lock), NULL);   // initialize the lock
    }
    return lqp;
}

/* Add an element to the locked queue */
int32_t lqput(locked_queue_t *lqp, void *elementp) {
    if (lqp == NULL || elementp == NULL) {
        return 1;   // error: invalid argument
    }

    pthread_mutex_lock(&(lqp->lock));   // lock the queue

    int32_t result = qput(lqp->queue, elementp);

    pthread_mutex_unlock(&(lqp->lock));   // unlock the queue

    return result;
}

/* Remove and return the first element from the locked queue */
void* lqget(locked_queue_t *lqp) {
    if (lqp == NULL) {
        return NULL;   // error: invalid argument
    }

    pthread_mutex_lock(&(lqp->lock));   // lock the queue

    void *elementp = qget(lqp->queue);

    pthread_mutex_unlock(&(lqp->lock));   // unlock the queue

    return elementp;
}

/* Apply a function to each element of the locked queue */
void lqapply(locked_queue_t *lqp, void (*fn)(void* elementp)) {
    if (lqp == NULL || fn == NULL) {
        return;   // error: invalid argument
    }

    pthread_mutex_lock(&(lqp->lock));   // lock the queue

    qapply(lqp->queue, fn);

    pthread_mutex_unlock(&(lqp->lock));   // unlock the queue
}

/* Search for an element in the locked queue */
void* lqsearch(locked_queue_t *lqp, bool (*searchfn)(void* elementp, const void* keyp), const void* skeyp) {
    if (lqp == NULL || searchfn == NULL || skeyp == NULL) {
        return NULL;   // error: invalid argument
    }

    pthread_mutex_lock(&(lqp->lock));   // lock the queue

    void *elementp = qsearch(lqp->queue, searchfn, skeyp);

    pthread_mutex_unlock(&(lqp->lock));   // unlock the queue

    return elementp;
}

/* Deallocate and free the locked queue */
void lqclose(locked_queue_t *lqp) {
    if (lqp == NULL) {
        return;   // error: invalid argument
    }

    pthread_mutex_lock(&(lqp->lock));   // lock the queue

    qclose(lqp->queue);   // deallocate the underlying queue
    free(lqp);             // free the locked queue object
    lqp = NULL;

    pthread_mutex_unlock(&(lqp->lock));   // unlock the queue
    pthread_mutex_destroy(&(lqp->lock));  // destroy the lock
}
