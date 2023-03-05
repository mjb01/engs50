#include "hash.h"
#include "queue.h"
#include <pthread.h>

/* A locked-hash table implementation using the hash and queue modules */

/* The entry struct stores a key-value pair */
typedef struct entry {
    void *value;
    char *key;
    int keylen;
} entry_t;

/* The locked-hash table struct */
typedef struct lh {
    hashtable_t *hash_table;
    queue_t *queue;
    pthread_mutex_t mutex;
} lh_t;

/* lhopen -- opens a locked-hash table with initial size hsize */
lh_t *lhopen(uint32_t hsize) {
    lh_t *lhtp = (lh_t *)malloc(sizeof(lh_t));
    if (lhtp == NULL) {
        return NULL;
    }

    lhtp->hash_table = hopen(hsize);
    if (lhtp->hash_table == NULL) {
        free(lhtp);
        return NULL;
    }

    lhtp->queue = qopen();
    if (lhtp->queue == NULL) {
        hclose(lhtp->hash_table);
        free(lhtp);
        return NULL;
    }

    if (pthread_mutex_init(&lhtp->mutex, NULL) != 0) {
        qclose(lhtp->queue);
        hclose(lhtp->hash_table);
        free(lhtp);
        return NULL;
    }

    return lhtp;
}

/* lhclose -- closes a locked-hash table */
void lhclose(lh_t *lhtp) {
    if (lhtp == NULL) {
        return;
    }

    pthread_mutex_lock(&lhtp->mutex);
    hclose(lhtp->hash_table);
    qclose(lhtp->queue);
    pthread_mutex_unlock(&lhtp->mutex);

    pthread_mutex_destroy(&lhtp->mutex);
    free(lhtp);
}

/* lhput -- puts an entry into a locked-hash table under designated key */
int32_t lhput(lh_t *lhtp, void *ep, const char *key, int keylen) {
    if (lhtp == NULL || ep == NULL || key == NULL || keylen <= 0) {
        return 1;
    }

    entry_t *entry = (entry_t *)malloc(sizeof(entry_t));
    if (entry == NULL) {
        return 1;
    }

    entry->value = ep;
    entry->key = (char *)malloc(keylen);
    if (entry->key == NULL) {
        free(entry);
        return 1;
    }
    strncpy(entry->key, key, keylen);
    entry->keylen = keylen;

    pthread_mutex_lock(&lhtp->mutex);
    int32_t result = hput(lhtp->hash_table, entry, key, keylen);
    if (result == 0) {
        result = qput(lhtp->queue, entry);
    }
    pthread_mutex_unlock(&lhtp->mutex);

    if (result != 0) {
        free(entry->key);
        free(entry);
    }

    return result;
}
/* lhget -- gets an entry from a locked-hash table under designated key */
void *lhget(lh_t *lhtp, const char *key, int32_t keylen) {
if (lhtp == NULL || key == NULL || keylen <= 0) {
return NULL;
}
pthread_mutex_lock(&lhtp->mutex);
entry_t *entry = hsearch(lhtp->hash_table, NULL, key, keylen);
if (entry != NULL) {
    void *result = entry->value;
    pthread_mutex_unlock(&lhtp->mutex);
    return result;
}

pthread_mutex_unlock(&lhtp->mutex);
return NULL;
}

/* lhapply -- applies a function to every entry in a locked-hash table */
void lhapply(lh_t *lhtp, void (*fn)(void *ep)) {
if (lhtp == NULL || fn == NULL) {
return;
}
pthread_mutex_lock(&lhtp->mutex);
qapply(lhtp->queue, fn);
pthread_mutex_unlock(&lhtp->mutex);
}

/* lhsearch -- searches for an entry in a locked-hash table using a comparator function */
void *lhsearch(lh_t *lhtp, void *key, int (*compar)(const void *, const void *)) {
if (lhtp == NULL || key == NULL || compar == NULL) {
return NULL;
}
pthread_mutex_lock(&lhtp->mutex);
entry_t *entry = hsearch(lhtp->hash_table, key, NULL, compar);
if (entry != NULL) {
    void *result = entry->value;
    pthread_mutex_unlock(&lhtp->mutex);
    return result;
}

pthread_mutex_unlock(&lhtp->mutex);
return NULL;
}
