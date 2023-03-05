#pragma once
/* 
 * locked_queue.h -- public interface to the locked queue module
 */
#include <stdint.h>
#include <stdbool.h>
#include "queue.h"  // Include the queue module for functionality

/* the locked queue representation is hidden from users of the module */
typedef struct lqueue locked_queue_t;

/* create an empty locked queue */
locked_queue_t* lqopen(void);        

/* deallocate a locked queue, frees everything in it */
void lqclose(locked_queue_t *lqp);   

/* put element at the end of the locked queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(locked_queue_t *lqp, void *elementp); 

/* get the first first element from locked queue, removing it from the queue */
void* lqget(locked_queue_t *lqp);

/* apply a function to every element of the locked queue */
void lqapply(locked_queue_t *lqp, void (*fn)(void* elementp));

/* search a locked queue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the locked queue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */
void* lqsearch(locked_queue_t *lqp, 
               bool (*searchfn)(void* elementp,const void* keyp),
               const void* skeyp);

/* concatenatenates elements of lq2 into lq1
 * lq2 is dealocated, closed, and unusable upon completion 
 */
void lqconcat(locked_queue_t *lq1p, locked_queue_t *lq2p);
