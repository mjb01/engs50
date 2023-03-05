#pragma once
/* 
 * locked_hash.h -- public interface to the locked queue module
 */
#include "hash.h"
#include "queue.h"
#include <pthread.h>

typedef struct {
    hashtable_t *ht;
    pthread_mutex_t lock;
} lockedhashtable_t;

/* lhopen -- opens a locked hash table with initial size hsize */
lockedhashtable_t *lhopen(uint32_t hsize);

/* lhclose -- closes a locked hash table */
void lhclose(lockedhashtable_t *lhtp);

/* lhput -- puts an entry into a locked hash table under designated key 
 * returns 0 for success; non-zero otherwise
 */
int32_t lhput(lockedhashtable_t *lhtp, void *ep, const char *key, int keylen);

/* lhget -- gets an entry from a locked hash table with designated key
 * returns a pointer to the entry or NULL if not found
 */
void *lhget(lockedhashtable_t *lhtp, const char *key, int32_t keylen);

/* lhapply -- applies a function to every entry in locked hash table */
void lhapply(lockedhashtable_t *lhtp, void (*fn)(void* ep));

/* lhsearch -- searches for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if
 * not found
 */
void *lhsearch(lockedhashtable_t *lhtp, 
	      bool (*searchfn)(void* elementp, const void* searchkeyp), 
	      const char *key, 
	      int32_t keylen);

/* lhremove -- removes and returns an entry under a designated key
 * using a designated search fn -- returns a pointer to the entry or
 * NULL if not found
 */
void *lhremove(lockedhashtable_t *lhtp, 
	      bool (*searchfn)(void* elementp, const void* searchkeyp), 
	      const char *key, 
	      int32_t keylen);